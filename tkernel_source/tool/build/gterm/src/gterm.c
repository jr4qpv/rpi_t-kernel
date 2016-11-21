/*
	gterm.c		gterm : メイン

	Copyright (C) 1996-2013 by Personal Media Corporation
*/
#include	<stdio.h>
#include	<setjmp.h>
#include	<signal.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<string.h>
#include	<memory.h>
#include	<stdarg.h>
#include	<unistd.h>
#include	<errno.h>
#include	<sys/time.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#include	"devio.h"
#include	"gterm.h"

/* 開始終了メッセージ */
static	char	startmsg[] = {"<< Gterm ver 2.50 : 130118 >>\n"};
#define	END_MSG		"\n<< exit Gterm >>\n"

/* 各種定義 */
#define	MAX_NEST	32			/* コマンド最大ネスト数	*/
#define	MAX_HIST	100			/* 履歴保存数		*/
#define	MAX_DEFS	(32 * 1024)		/* 最大 def サイズ	*/
#define	MAX_ALIASES	(32 * 1024)		/* 最大 alias サイズ	*/

/* プロンプト文字列定義 */
#define	MAX_PROMPT	12
static	char	prompt[MAX_PROMPT][8+1] = {		/* 8 文字	*/
		"> \0\0\0\0\0\0",		/* 1  : "> "	*/
		"% \0\0\0\0\0\0",		/* 2  : "% "	*/
		"? \0\0\0\0\0\0\0",		/* 3  : "? "	*/
		">\0\0\0\0\0\0\0",		/* 4  : ">"	*/
		"\0\0\0\0\0\0\0\0",		/* 5  : --	*/
		"\0\0\0\0\0\0\0\0",		/* 6  : --	*/
		"\0\0\0\0\0\0\0\0",		/* 7  : --	*/
		"\0\0\0\0\0\0\0\0",		/* 8  : --	*/
		"\0\0\0\0\0\0\0\0",		/* 9  : --	*/
		"\0\0\0\0\0\0\0\0",		/* 10 : --	*/
		"\0\0\0\0\0\0\0\0",		/* 11 : --	*/
		"\0\0\0\0\0\0\0\0"};		/* 12 : --	*/

static	uchar	linebuf[LINE_LEN];	/* 入力行バッファ		*/
static	uchar	*curp, *endp;		/* 入力ポインタ			*/
static	char	lbuf[LINE_LEN];		/* 入力行バッファ 2		*/
static	char	llbuf[LINE_LEN];	/* 入力行バッファ 3		*/
static	uchar	last[4] = {0,0,0,0};	/* 出力した行頭文字		*/

static	FILE	*logfp = NULL;		/* ログファイル			*/
static	FILE	*fp;			/* ファイル			*/
static	FILE	*infp[MAX_NEST];	/* 入力ファイル			*/
static	uchar	*args[MAX_NEST];	/* 入力パラメータ		*/
static	int	nest = 0;		/* ネスト数			*/
static	int	mode = 0;		/* 内部動作モード		*/
static	int	expand = 0;		/* 展開フラグ			*/
static	int	LFflg = 0;		/* LF 出力フラグ		*/
static	int	repcnt[MAX_NEST];	/* 繰り返し回数			*/
static	uchar	*repbuf;		/* 繰り返し用バッファ		*/

static	uchar	cmd_ch = '.';		/* 疑似コマンドの先頭文字	*/

static	int	wait_flg = 0;		/* 待ちフラグ			*/
static	int	do_break = 0;		/* ブレークフラグ		*/
static	int	cons_off = 0;		/* コンソール出力オフ		*/

	int	out_wait = 0;		/* 出力待ち時間 (usec)		*/
static	int	in_wait = 0;		/* 入力待ち時間	(msec)		*/

static	int	ts_flg = 0;		/* タイムスタンプの有無		*/
static	int	line_top = 0;		/* 先頭行			*/
static	long	basetime = -1;		/* 基準時刻			*/
static	long	basetime_us;		/* 基準時刻			*/

#define	HIST_SZ	(MAX_HIST * LINE_LEN)	/* 履歴バッファサイズ		*/
static	uchar	*hist;			/* 履歴バッファ			*/
static	int	hcnt = MAX_HIST;	/* 履歴数			*/

static	uchar	*def_top, *def_end;	/* 定義バッファ			*/
static	uchar	*alias_top, *alias_end;	/* エイリアスバッファ		*/

#define	LFOUT	{Putc(LF); LFflg++;}	/* LF 出力			*/

static	sigjmp_buf	int_jump;	/* ジャンプバッファ		*/
static	int	break_ch = 0x3;		/* ブレーク文字			*/
	int	ignore_echo;		/* エコー無視 モード		*/
static	int	filecd;			/* ファイル系文字コード		*/
					/* 0:EUC,1:SJIS			*/

/* 疑似コマンド ID 定義 */
#define	CMD_NONE	0
#define	CMD_QUIT	1
#define	CMD_CD		2
#define	CMD_PWD		3
#define	CMD_TON		4
#define	CMD_TOFF	5
#define	CMD_WAIT	6
#define	CMD_ALIAS	7
#define	CMD_UNALIAS	8
#define	CMD_DEF		9
#define	CMD_UNDEF	10
#define	CMD_ESC		11
#define	CMD_DO		12
#define	CMD_SH		13
#define	CMD_HELP	14
#define	CMD_PROM	15
#define	CMD_REPT	16
#define	CMD_CMNT	17
#define	CMD_PRINT	18
#define	CMD_INPUT	19
#define	CMD_BREAK	20
#define	CMD_CONT	21
#define	CMD_HIST	22
#define	CMD_CON		23
#define	CMD_COFF	24
#define	CMD_SYMT	25
#define	CMD_EVAL	26
#define	CMD_LOAD	27
#define	CMD_LOADFL	32
#define	CMD_TSON	33
#define	CMD_TSOFF	34
#define	CMD_OUTWAIT	35
#define	CMD_INWAIT	36

/* 疑似コマンド定義 */
typedef	struct {
	int	id;
	char	*str;
} CMD;
static	CMD	command[] = {
	{CMD_QUIT,	"q"},
	{CMD_CD,	"cd"},
	{CMD_PWD,	"pwd"},
	{CMD_TON,	"ton"},
	{CMD_TOFF,	"toff"},
	{CMD_WAIT,	"wait"},
	{CMD_ALIAS,	"alias"},
	{CMD_UNALIAS,	"unalias"},
	{CMD_DEF,	"def"},
	{CMD_UNDEF,	"undef"},
	{CMD_ESC,	"esc"},
	{CMD_DO,	"do"},
	{CMD_SH,	"!"},
	{CMD_HELP,	"?"},
	{CMD_PROM,	"prompt"},
	{CMD_REPT,	"#"},
	{CMD_CMNT,	"*"},
	{CMD_PRINT,	"pr"},
	{CMD_INPUT,	"?def"},
	{CMD_BREAK,	"break"},
	{CMD_CONT,	"cont"},
	{CMD_LOAD,	"load"},
	{CMD_LOADFL,	"flload"},
	{CMD_HIST,	"hist"},
	{CMD_CON,	"con"},
	{CMD_COFF,	"cof"},
	{CMD_SYMT,	"symt"},
	{CMD_EVAL,	"@"},
	{CMD_TSON,	"tson"},
	{CMD_TSOFF,	"tsoff"},
	{CMD_OUTWAIT,	"outwait"},
	{CMD_INWAIT,	"inwait"},
	{CMD_NONE,	NULL} };

/* ヘルプメッセージ定義 */
static	char	*help_message[] = {
	"	.* [comment]		.q			\n",
	"	.esc c			.prompt n 'c..c'	\n",
	"	.cd [dir]		.pwd			\n",
	"	.ton [fn]		.toff			\n",
	"	.alias [cmd str]	.unalias cmd | ALL	\n",
	"	.def id replaced_id	.undef id | ALL		\n",
	"	.pr [str]		.?def id ...		\n",
	"	.do fn			.wait n			\n",
	"	.break			.cont			\n",
	"	.! [sh_cmd]		.# rep_cnt cmd		\n",
	"	.hist			.?			\n",
	"	.symt map_file [addr]	.@ sym | adr | ? |?adr  \n",
	"	.load file [addr]	.flload s-file [xe@*]	\n",
	"	.con			.cof			\n",
	"	.tson			.tsoff			\n",
	"	.outwait [n.n(msec)]	.inwait [n(msec)]	\n",
	NULL};

/*
	ファイルへの文字列出力
*/
void	FPuts(const uchar *s, FILE *fp)
{
	int c, c2, pre;

	if (!filecd) {
		while ((c = *s++) != 0) {
			if (c != CR) fputc(c, fp);
		}
		return;
	}
					/* SJIS 対応			*/
	pre = 0;
	while ((c = *s++) != 0) {
		if (!iseucknj(c)) {
			if ((c == LF) && (pre != CR)) {
				fputc(CR, fp);
			}
			fputc(c, fp);
			pre = c;
			continue;
		}
		if (((c2 = *s) == 0)
			|| (!iseucknj(c2))) {
			fputc(c, fp);
			pre = c;
			continue;
		}
		pre = c2;
		euctosj(&c, &c2);
		fputc(c, fp);
		fputc(c2, fp);
		s++;
	}
}

/*
	コンソール(TTY) への 1 文字出力
*/
void	Putc(uchar c)
{
static	int	pre1;
	int	i;

	TYout(c);

	if (!logfp) return;

	if (!filecd) {
		if (c != CR) fputc(c, logfp);
		return;
	}
	if (pre1 == 0) {
		if (iseucknj(c)) 
			pre1 = c;
		else
			fputc(c, logfp);
	} else {
		if (iseucknj(c))  {
			i = c;
			euctosj(&pre1, &i);
			fputc(pre1, logfp);
			fputc(i, logfp);
			pre1 = 0;
		} else {
			fputc(pre1, logfp);
			pre1 = c;
		}
	}
}

/*
	コンソール(TTY) への文字列出力
*/
void	Puts(uchar *s)
{
	uchar	c;

	TYouts(s);
	if (!logfp) return;
	FPuts(s, logfp);
}

/*
	コンソール(TTY) への書式文字列出力
*/
void	Print(char *fmt, ...)
{
	va_list	ap;
	char	buf[LINE_LEN];

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);
	Puts(buf);
}

/*
	タイムスタンプの表示
*/
static	void	time_stump(void)
{
	long	sec, usec;
	struct timeval	tv;
	struct timezone	tz;

	gettimeofday(&tv, &tz);

	if (basetime == -1) {
		basetime = tv.tv_sec;
		basetime_us = tv.tv_usec;
	}
	sec = tv.tv_sec - basetime;
	usec = tv.tv_usec - basetime_us;
	while (usec < 0) {sec--; usec += 1000 * 1000;}

	Print("[%04d.%03d] ", sec, usec / 1000);
}

/*
	プロンプトの表示
*/
static	void	show_prompt(void)
{
	if (mode) {
		Puts(prompt[mode - 1]);
	}
}

/*
	コンソール(TTY)/コマンドファイルからの 1 文字入力
	引数:int f 	0
			非0	強制 TTY 入力
	戻り値		0	入力なし
			>0	入力文字
*/
int	Getc(int f)
{
	int	c, d;
static	int	pre1, pre2;

	for (;;) {
		if (nest == 0 || f) return TYin();
		if (wait_flg) return 0;

		if (pre2) {
			c = pre2; pre2 = 0; return c;
		}
		if (pre1) {
			c = pre1; pre1 = 0;
		} else {
			c = getc(infp[nest-1]);
			if (filecd && c == CR) {
				d = getc(infp[nest-1]);
				if (d == LF) {
					c = d;
				} else {
					pre1 = d;
				}
			}
		}
		if (c != EOF) {
			if ((!filecd) || (!issjisk1(c))) return c;
			d = getc(infp[nest-1]);
			if ((d == EOF) || (!issjisk2(d))) {
				pre1 = d; return c;
			}
			sjtoeuc(&c, &d);
			pre2 = d;
			return c;
		}
		pre1 = pre2 = 0;
		fclose(infp[nest-1]);
		wait_flg = do_break = 0;
		if (--nest == 0) {
			if ((c = TYin()) == 0 && last[2] == 0) show_prompt();
			return c;
		}
	}
}

/*
	回線からの 1 文字入力
	引  数: int flg		0	TTY 入力に影響を与えない
				非0	TTY 入力を削除する
		int tmout	>= 0	タイムアウト(ミリ秒)
				-1	タイムアウトなし
	戻り値:	< 0			タイムアウト
		>= 0			回線からの 1 文字
*/
int	RSgetc(int flg, int tmout)
{
	int rs_ty, key;

	while(1) {
		rs_ty = PollRSTY(tmout);
		if (rs_ty == 0) break;	/* タイムアウト	*/
		if (rs_ty & 0x2) break;	/* 回線入力あり */
					/* TTY入力あり	*/
		if (flg) {
			key = TYin();
			if (tmout >= 0 && key == break_ch)
				break;
		} else
			break;
	}

	return RSin();
}

/*
	次のコマンドがあるとき 1
 */
static	int	nextcmd(void)
{
	if (nest > 0		/* コマンドファイル実行モード	*/
	    || repcnt[0] > 0) {	/* 繰返しの場合			*/
		return 1;
	}
	return 0;
}

/*
	コンソール(TTY)/コマンドファイルからの 1 行入力

	eclipse の場合、コンソールのエコーバックがなされているので、
	ignore_echo が有効な場合、TTY に対するエコーバックを行わない。
*/
static	int	get_line(int f)
{
	uchar	*p;
	int	c;
	int	rep = 0;

	if (f == 0 && repcnt[nest]) {
		rep++;
		strcpy(linebuf, &repbuf[LINE_LEN * nest]);
		repcnt[nest]--;
		endp = curp = linebuf + strlen(linebuf);

	} else while ( (c = Getc(f)) != LF && c != CR) {
		if (c < 0 || (f == 0 && c == 0)) return 0;

		switch (Keytest(c)) {
		case 'A'-'@':
			while (curp > linebuf) {cur_bwd(); curp--;}
			break;
		case 'E'-'@':
			while (curp < endp) {cur_fwd(); curp++;}
			break;
		case 'P'-'@':
		case CUR_UP:	c = 0;
		case 'N'-'@':
		case CUR_DWN:
			if (c) {if (++hcnt >= MAX_HIST) hcnt = MAX_HIST - 1;}
			else   {if (--hcnt < 0) hcnt = 0;}
			memcpy(linebuf, hist + (hcnt * LINE_LEN), LINE_LEN);
			line_erase();
			while (curp > linebuf) {cur_bs(); curp--;}
			while (*curp) TYout(*curp++);
			TYflush();
			endp = curp;
			break;
		case LINE_ERASE:
			line_erase();
			endp = curp;
			break;
		case BS:
		case DEL:
			if (curp == linebuf) break;
			if (isk2(*(curp-1)) && (curp - 1 > linebuf)
							&& isk1(*(curp-2))) {
				if (curp < endp)
					memcpy(curp - 1, curp, endp - curp);
				cur_bs(); curp--; endp--;
			}
			if (curp < endp) memcpy(curp - 1, curp, endp - curp);
			cur_bs(); curp--; endp--;
			break;
		case 'D'-'@':
			if (curp >= linebuf) {
				cur_del();
				if (curp < endp) {
					memcpy(curp, curp + 1, endp - curp);
					endp--;
				}
			}
			break;
		case 'X'-'@':
		case CANCEL:
			line_erase();
			while (curp > linebuf) {cur_bs(); curp--;}
			endp = curp;
			break;
		case 'F'-'@':
		case CUR_FWD:
			if (curp >= endp) break;
			if (isk1(*curp) && (curp + 1 < endp)
						&& isk2(*(curp+1))) {
				cur_fwd(); curp++;
			}
			cur_fwd(); curp++;
			break;
		case 'B'-'@':
		case CUR_BWD:
			if (curp == linebuf) break;
			if (isk2(*(curp-1)) && (curp - 1 > linebuf)
						&& isk1(*(curp-2))) {
				cur_bwd(); curp--;
			}
			cur_bwd(); curp--;
			break;
		default:
			if (c < ' ' && c != TAB) continue;
			if (curp < endp) {
				uchar *cp;
				for (cp = endp; --cp >= curp;) *(cp+1) = *cp;
				cur_end();
			}
			*curp++ = c;
			endp++;
			if (!ignore_echo)
				if (!nest || expand || f) TYout(c);
		}
		TYflush();
	}
	*endp = 0;

	if (*linebuf) {
		if (nest == 0 && rep == 0) {
			if (linebuf[0] != '!') {
				if (f == 0) {
					memcpy(hist, hist + LINE_LEN,
							HIST_SZ - LINE_LEN);
					memcpy(hist + HIST_SZ - LINE_LEN,
							linebuf, LINE_LEN);
				}
				if (logfp) FPuts(linebuf, logfp);
			}
		} else if (expand && logfp) FPuts(linebuf, logfp);
	}
	hcnt = MAX_HIST;
	if (!ignore_echo)
		if (!nest || expand || f) LFOUT;
	return 1;
}

/*
	コンソール(TTY)/コマンドファイルからの 1 行入力
*/
static	uchar	* linein(uchar *s)
{
	if (s) Puts(s);
	endp = curp = linebuf;
	if (get_line(1) == 0) return NULL;
	endp = curp = linebuf;
	return curp;
}

/*
	プログラム終了
*/
static	void	finish(void)
{
	sys_end(0);
}

/*
	プログラム再開
*/
static	void	restart(void)
{
	TYreinit();
}

/*
	プログラムブレーク
*/
void	int_break(void)
{
	siglongjmp(int_jump, 1);
}

/*
	ファイルのオープン
*/
FILE	*f_open(uchar *path, uchar *mode)
{
	uchar	c;
	uchar	buf[LINE_LEN];
	uchar	*p;
	uchar	*bp = buf;

	while (c = *path++) {
		if (c == '~' && (p = getenv("HOME"))) {
			strcpy(bp, p);
			bp += strlen(bp);
		} else *bp++ = c;
	}
	*bp = 0;
	return fopen(buf, mode);
}

/*
	空白のスキップ
*/
static	uchar	*skipsp(uchar *p)
{
	while (*p && *p <= ' ') p++;
	return p;
}

/*
	区切り文字のスキップ
*/
static	uchar	*skipdelm(uchar *p)
{
	while ((*p && *p <= ' ') || *p == ',' || *p == '.') p++;
	return p;
}

/*
	項目の取得
*/
void	get_term(uchar *buf, uchar **pp)
{
	uchar	*p;

	p = skipsp(*pp);
	while ((*buf = *p) > ' ') { buf++; p++;}
	*buf = 0;
	*pp = skipsp(p);
}

/*
	ID 項目の取得
*/
static	int	get_id(uchar *buf, uchar **pp)
{
	uchar	c, *p;
	int	n;

static char idchar[] = {
   /*	0  1  2  3  4  5  6  7	8  9  a  b  c  d  e  f	*/
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		/*  0- F */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		/* 10-1F */
	0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,		/* 20-2F */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,		/* 30-3F */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,		/* 40-4F */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 0, 0, 1,		/* 50-5F */
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,		/* 60-6F */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0		/* 70-7F */
};
#define	is_sid(c)	((c)>=0x80||idchar[c])
#define	is_id(c)	((c)>=0x80||(idchar[c]&1))

	for (n = 0, p = *pp; c = *p; p++) {
		if (is_sid(c)) {
			if (c == '$') while (*(p+1) == '{') {p++; n++;}
			do {*buf++ = c; n++; c = *++p;}
			while (is_id(c));
			break;
		}
	}
	*buf = 0;
	*pp = p;
	return	n;
}

/*
	出力の継続のチェック
*/
static	int	more(void)
{
	uchar	c;

	Puts(".. (more) ? ");
	while ((c = TYin()) == 0);
	if (!ignore_echo)
		LFOUT;
	return (c != 'y' && c != 'Y') ? 1 : 0;
}

/*
	定義/エイリアスの出力
*/
static	uchar	*out_def(uchar *p)
{
	uchar	*q;

	q = p + strlen(p) + 1;
	Print(".. %s : %s\n", p, q);
	return q + strlen(q) + 1;
}

/*
	定義/エイリアス全体の出力
*/
static	void	list_def(int alias)
{
	uchar	*p, *top, *end;
	int	cnt;

	top = (alias) ? alias_top : def_top;
	end = (alias) ? alias_end : def_end;

	cnt = 10;
	for (p = top; p < end;) {
		if (--cnt <= 0) {
			if (more()) break;
			cnt = 10;
		}
		p = out_def(p);
	}
}

/*
	定義/エイリアスの全クリア
*/
static	void	reset_def(int alias)
{
	if (alias) alias_end = alias_top;
	else	   {def_end = def_top;	expand = 0;}
}

/*
	定義/エイリアスの登録
*/
static	void	put_def(int alias, uchar *buf, uchar *p)
{
	int	i, j;
	uchar	*end;

	end = (alias) ? alias_end : def_end;

	if ((i = strlen(buf)) + (j = strlen(p)) + 2 + end >=
		(alias? (alias_top + MAX_ALIASES) : (def_top + MAX_DEFS))) {
		Puts((alias)? ".. Too many aliases\n" :
			      ".. Too many defines\n");
	} else {
		strcpy(end, buf);
		end += i + 1;
		strcpy(end, p);
		end += j + 1;
		if (alias) alias_end = end;
		else {
			def_end = end;
			if (strcmp("EXPAND", buf) == 0) expand = 1;
		}
	}
}

/*
	定義/エイリアスの削除
*/
static	void	rem_def(int alias, uchar *p)
{
	int	i;
	uchar	*pp;

	if (!alias && strcmp("EXPAND", p) == 0) expand = 0;

	pp = p + strlen(p) + 1;
	pp += strlen(pp) + 1;

	if ((i = ((alias) ? alias_end : def_end) - pp) > 0) memcpy(p, pp, i);

	if (alias) alias_end = p + i;
	else	   def_end = p + i;
}

/*
	定義/エイリアスの検索
*/
static	uchar* find_def(int alias, uchar *buf)
{
	uchar	*p;
	uchar	*end;

	p = (alias) ? alias_top : def_top;
	end = (alias) ? alias_end : def_end;

	for (; p < end && (*p != *buf || strcmp(buf, p));) {
		p += strlen(p) + 1;
		p += strlen(p) + 1;
	}
	if (p < end) return p;
	return NULL;
}

/*
	エイリアスの処理
*/
static	int	alias_line(uchar *d, uchar *s)
{
	uchar	buf[LINE_LEN], sbuf[LINE_LEN];
	uchar	*p, *pp, *ss, *sx, *dd;
	int	n, loop;

	loop = 0;
	dd = d;
AGAIN:
	ss = s;
	get_term(buf, &s);

	if (p = find_def(1, buf)) {
		p += strlen(p) + 1;
		sx = s;
		while (*p) {
			pp = p;
			n = get_id(buf, &p);
			strncpy(d, pp, p - pp);
			d += p - pp;
			if (buf[0] == '$' && buf[2] == 0 &&
					buf[1] >= '1' && buf[1] <= '9') {
				d -= n;
				for (ss = s, n = buf[1] - '0'; --n >= 0;)
					get_term(d, &ss);
				if (ss > sx) sx = ss;
				d += strlen(d);
			}
		}
		*d++ = ' ';
		strcpy(d, sx);
		if (++loop <= 16) {
			strcpy(s = sbuf, d = dd);
			goto AGAIN;
		}
	} else {
		strcpy(d, ss);
	}

	d += strlen(d);
	if (dd < d) {
		while (dd < d) {
			if (*(d - 1) > ' ') break;
			--d;
		}
		*d = 0;
	}

	n = d - dd;
	if (*dd == '!') {
		p = hist + HIST_SZ - LINE_LEN;
		if (--n > 0) {
			while (p >= hist && strncmp(p, dd + 1, n))
					p -= LINE_LEN;
		}
		if (p >= hist) strcpy(linebuf, p);
		else	       linebuf[0] = 0;
		line_erase();
		Puts("= ");
		while (curp > linebuf) {cur_bs(); curp--;}
		while (*curp) TYout(*curp++); TYflush();
		endp = curp;
		*dd = 0;
		return 1;
	}

	n = 0;

	/* check if multi-line ";;" */
	for (d = dd; *d; d++) {
		if (*d == '\\') {
			if (*++d == 0) break;
		} else if (*d == ';' && *(d+1) == ';') {
			strcpy(d, d+1);
			*d = '\n';
			n++;
		}
	}

	if (n) {
		if (nest >= MAX_NEST) {
			Puts(".. Too many nest\n");
			n = 0;
		} else if (!(fp = tmpfile ())) {
			Puts(".. Can't open tmpfile \n");
			n = 0;
		} else {
			strcat(dd, "\n");
			if (filecd) {
				FPuts(dd, fp);
			} else {
				fwrite(dd, strlen(dd), 1, fp);
			}
			rewind(fp);
			infp[nest] = fp;
			*(args[nest++]) = 0;
		}
		*dd = 0;
	}
	return n;
}

/*
	パラメータの処理
*/
static	void	args_line(uchar *d, uchar *s)
{
	int	n;
	uchar	*p, *dd, *ss;
	uchar	buf[LINE_LEN];

	dd = d;
	if (nest) {
		while (*s) {
			ss = s;
			n = get_id(buf, &s);
			strncpy(d, ss, s - ss);
			d += s - ss;
			if (buf[0] == '$' && buf[2] == 0 &&
					buf[1] >= '1' && buf[1] <= '9') {
				d -= n;
				p = args[nest-1];
				for (n = buf[1] - '0'; --n >= 0;)
							get_term(d, &p);
				d += strlen(d);
			}
		}
		*d = 0;
	} else {
		strcpy(d, s);
	}
}

/*
	一行の処理
*/
static	void	make_line(uchar *d, uchar *s, int flg)
{
	uchar	buf[LINE_LEN], sbuf[LINE_LEN];
	uchar	c, *ss, *p, *dd;
	int	n, exp, loop;

	dd = d;
	loop = 0;
AGAIN:
	exp = 0;
	while (*s) {
		ss = s;
		n = get_id(buf, &s);
		strncpy(d, ss, s - ss);
		d += s - ss;
		if (flg && (n < 2 || buf[0] != '$')) n = 0;
		if (n && (p = find_def(0, buf + (buf[0]=='$' ? 1 : 0)))) {
			strcpy(d -= n, p + strlen(p) + 1);
			d += strlen(d);
			exp++;
		}
	}
	*d = 0;
	if (exp && (++loop) <= 16) {
		strcpy(s = sbuf, d = dd);
		goto AGAIN;
	}

	/* remove \ { }, convert  ^X ==> X-'@' */
	for (; c = *dd; dd++) {
		if (c == '\\') {
			strcpy(dd, dd+1);
			if (*dd == 0) break;
		} else if (c == '{' || c == '}') {
			strcpy(dd, dd+1);
			dd--;
		} else if (c == '^' && (c = *(dd+1) - '@') < ' ') {
			strcpy(dd, dd+1);
			*dd-- = c;
		}
	}
}

/*
	タイムアウト
*/
static	void	time_out(void)
{
	wait_flg = 0;
}

/*
	疑似コマンド処理
*/
static	int	do_cmd(uchar *p)
{
	int	er = 0;
	int	i;
	uchar	buf[LINE_LEN];
	uchar	*q, *qq;
	uchar	**pp;
	CMD	*cmd;

	p = skipsp(p + 1);
	switch(*p) {
	case '*':
	case '@':
	case '!':
	case '#':	buf[0] = *p++;	buf[1] = '\0';	p = skipsp(p);
			break;
	default:	get_term(buf, &p);
	}

	for (cmd = command; cmd->id && strcmp(cmd->str, buf); cmd++);

	switch (cmd->id) {
	case CMD_NONE:
		return 0;
	case CMD_QUIT:
		return -1;
	case CMD_ESC:
		if (*p) cmd_ch = *p;
		else	Print(".. Esc ch: %c\n", cmd_ch);
		break;
	case CMD_CD:
		if (*p) {
			get_term(buf, &p);
			if (er = chdir((buf[0])? buf : NULL)) break;
		}
	case CMD_PWD:
		if (getcwd(buf, 256) == NULL) er = 1;
		else	Print(".. dir: %s\n", buf);
		break;
	case CMD_TON:
		if (*p)	get_term(buf, &p);
		else	strcpy(buf, DEF_LOGFN);
		if (!(logfp = f_open(buf, "w"))) er = 1;
		else	Print(".. Trace ON: %s\n", buf);
		break;
	case CMD_TOFF:
		if (logfp) {
			fclose(logfp);logfp = NULL;
			Puts(".. Trace OFF\n");
		}
		break;
	case CMD_PROM:
		if (i = *p) {
			if ((i -= '0') >= 1 && i <= MAX_PROMPT) {
				while (*p && *p++ != '\'');
				for (q = p; *q && *q != '\''; q++);
				*q = 0;
				strncpy(prompt[i-1], p, 8);
			} else {
			      Puts(".. Usage: .prompt n 'c..c' : n = 1..12\n");
			}
		} else {
			Puts(".. Prompt: ");
			for (i = 0; i < MAX_PROMPT; i++) {
				if (i && (i % 6) == 0) Puts("\n           ");
				Print("%d:'%s'  ", i+1, prompt[i]);
			}
			LFOUT;
		}
		break;
	case CMD_WAIT:
		if ((i = atoi(p)) > 0) {
			if (!cons_off) Print(".. wait %d sec\n", i);
			signal(SIGALRM, (void*)time_out);
			alarm(i);
			wait_flg = 1;
		}
		break;
	case CMD_ALIAS:
		if (*p) {
			get_term(buf, &p);
			if (q = find_def(1, buf)) {
				if (*p) rem_def(1, q);
				else	out_def(q);
			}
			if (*p) put_def(1, buf, p);
			else if (!q) goto NOT_ALIAS;
		} else {
			list_def(1);
		}
		break;
	case CMD_UNALIAS:
		if (*p) {
			get_term(buf, &p);
			if (strcmp(buf, "ALL")) {
				if (q = find_def(1, buf)) rem_def(1, q);
				else {
NOT_ALIAS:				Print(".. Not aliased: %s\n", buf);
				}
			} else {
				reset_def(1);
			}
		}
		break;
	case CMD_DEF:
		if (*p) {
			get_term(buf, &p);
			if (q = find_def(0, buf)) {
				if (*p)	rem_def(0, q);
				else	out_def(q);
			}
			if (*p) put_def(0, buf, p);
			else if (!q) goto NOT_DEF;
		} else {
			list_def(0);
		}
		break;
	case CMD_UNDEF:
		if (*p) {
			get_term(buf, &p);
			if (strcmp(buf, "ALL")) {
				if (q = find_def(0, buf)) rem_def(0, q);
				else {
NOT_DEF:				Print(".. Not defined: %s\n", buf);
				}
			} else {
				reset_def(0);
			}
		}
		break;
	case CMD_DO:
		if (*p) {
			if (nest >= MAX_NEST) {
				Puts(".. Too many nest\n");
				break;
			}
			get_term(buf, &p);
			if (fp = f_open(buf, "r")) {
				infp[nest] = fp;
				strcpy(args[nest++], p);
			} else {
				Print(".. Can't open: %s\n", buf);
			}
		}
		break;
	case CMD_SH:
		if (*p) er = do_system(p);
		else	er = escape();
		break;
	case CMD_HELP:
		for (pp = (uchar **)help_message; *pp; pp++) Puts(*pp);
		break;
	case CMD_REPT:
		if (isdigit(*p)) {
			get_term(buf, &p);
			if ((i = atoi(buf)) < 0) i = 1;
			repcnt[nest] = i;
			strcpy(&repbuf[nest*LINE_LEN], p);
		} else {
			Puts(".. Usage: .# nn command\n");
		}
		break;
	case CMD_PRINT:
		Puts(p);
		LFOUT;
	case CMD_CMNT:
		break;
	case CMD_INPUT:
		while (*p) {
			get_term(buf, &p);
			for (;;) {
				Print(".. %s ? ", buf);
				if (!(q = linein(NULL))) goto NEXT;
				if (*q) {
					if (qq = find_def(0, buf))
						rem_def(0, qq);
					put_def(0, buf, q);
					break;
				}
			}
		}
		break;
	case CMD_BREAK:
		do_break = 1;
		break;
	case CMD_CONT:
		do_break = 0;
		break;
	case CMD_HIST:
		q = hist + HIST_SZ - LINE_LEN - LINE_LEN;
		for (i = 0; q >= hist; q -= LINE_LEN) {
			Print("%2d: %s\n", ++i, q);
			if ((i % 10) == 0) {if (more()) break;}
		}
		break;
	case CMD_CON:
		cons_off = 0;
		break;
	case CMD_COFF:
		cons_off = 1;
		break;

	case CMD_LOAD:
		if (*p) {
			do_load(kXMODEM | kLOAD, p);
			last[2] = mode = 0;
			/*LFOUT;*/
			TYflush();
		}
		break;
	case CMD_LOADFL:
		if (*p) {
			do_load(kFLLO, p);
			last[2] = mode = 0;
			/*LFOUT;*/
			TYflush();
		}
		break;
	case CMD_SYMT:
		get_term(buf, &p);
		symload(buf, p);
		break;
	case CMD_EVAL:
		get_term(buf, &p);
		symeval(buf, p);
		break;
	case CMD_TSON:
		ts_flg = 1;
		line_top = 0;
		basetime = -1;
		break;
	case CMD_TSOFF:
		ts_flg = 0;
		break;
	case CMD_OUTWAIT:
		if (*p) {
			i = strtol((char*)p, (char**)&p, 10) * 1000;
			if (*p++ == '.') {
				int	n;
				for (n =100; (*p >= '0' && *p <= '9'); p++) {
					i += (*p - '0') * n;
					if ((n /= 10) == 0) break;
				}
			}
			if (i >= 0) out_wait = i;
		}
		Print("outwait = %d.%03d msec\n",
					out_wait / 1000, out_wait % 1000);
		break;
	case CMD_INWAIT:
		if (*p && (i = atoi(p)) >= 0) in_wait = i;
		Print("inwait = %d msec\n", in_wait);
		break;
	}
NEXT:
	if (er) Puts(".. ERROR ..\n");
	TYflush();
	RSclr();
	if (mode == 0) RSputc(CR);
	if (!nest || expand) show_prompt();
	return 1;
}

/*
	TE_VCOM 環境の取得
*/
static	char *getvcomf(char **args)
{
#define TE_VCOMKW	"te_vcom"
#define TE_VCOMFN	".te_vcom"
#define TE_VCOMPATH	"TE_VCOMPATH"

	char	*te_vcompath;
	char	*home;
	char	*fpath = NULL;
	FILE	*fp = NULL;
	char	buf[1024];
	char	*ret;

	if (!*args) return *args;
	if (strcmp(*args, TE_VCOMKW)) return *args;

	te_vcompath = getenv(TE_VCOMPATH);
	if (te_vcompath) {
		fpath = (char*)malloc(strlen(te_vcompath) + 1);
		strcpy(fpath, te_vcompath);
	} else {
		home = getenv("HOME");
		if (home) {
			fpath = (char *) malloc(strlen(home) + 2
							+ strlen(TE_VCOMFN));
			strcpy(fpath, home);
			strcat(fpath, "/");
			strcat(fpath, TE_VCOMFN);
		} else {
			fpath = (char *) malloc(3 + strlen(TE_VCOMFN));
			strcpy(fpath, "./");
			strcat(fpath, TE_VCOMFN);
		}
	}

	fp = fopen(fpath, "r");
	if (fp == NULL) {
		fprintf(stderr, ".. Cannot open(%s)\n", fpath);
		goto err_ret;
	}
	ret = fgets(buf, sizeof(buf), fp);
	fclose(fp); fp = NULL;
	if (ret == NULL) {
		fprintf(stderr, ".. Cannot read 2 from %s\n", fpath);
		goto err_ret;
	}
	ret = strchr(buf, '\n');
	if (ret) *ret = 0;
	ret = strdup(buf);
	if (ret == NULL) {
		fprintf(stderr, ".. %s\n", strerror(ENOMEM));
		goto err_ret;
	}
	*args = ret;
	return ret;
err_ret:
	if (fp) fclose(fp);
	if (fpath) free(fpath);
	return NULL;
}

/*
	現在時間 (ミリ秒単位)
*/
static	unsigned int	gettime(void)
{
	struct timeval	tv;
	long long	ms;

	gettimeofday(&tv, NULL);
	ms = tv.tv_sec * 1000LL + tv.tv_usec / 1000;

	return ms & 0xffffffff;
}

/*
	メインプログラム
*/
int	main(int ac, uchar **av)
{
	uchar	c, c1, *ln, *p;
	int	i, j, quit, xinterrupt, use_stdin, sjis, ret;
	int	noflow, par, clen, sbit, baud, tmo;
	int	loadmode, loadmem;
	uchar	loadpath[LINE_LEN];
	unsigned int	idle;
	struct	stat	st;

	/* 初期化 */
	alias_end = alias_top = calloc(MAX_ALIASES, 1);
	def_end = def_top = calloc(MAX_DEFS, 1);
	hist = calloc(MAX_HIST, LINE_LEN);
	repbuf = calloc(MAX_NEST, LINE_LEN);


	ln = NULL;
	fp = NULL;
	noflow = par = sbit = 0;
	clen = 8;
	baud = DEF_BAUD;
	use_stdin = ignore_echo = sjis = 0;

	/* 起動パラメータの取得 */
	while ((p = *++av) && *p == '-') {
		switch (*++p) {
		case 'c':
		case 'C':	if (*++p == '\0') p = *++av;
				if (!(fp = f_open(p, "r")))
				fprintf(stderr, ".. Can't open: %s\n", p);
				break;
		case 'l':
		case 'L':	if (*++p == '\0') p = *++av;
				ln = p;
				break;
		case 'x':
		case 'X':	if (*++p != '\0') goto OPTERR;
				noflow = 1;
				break;
		case 'y':
		case 'Y':	if (*++p != '\0') goto OPTERR;
				sbit = 1;
				break;
		case 'z':
		case 'Z':	if (*++p == '\0') p = *++av;
				clen = strtol(p, NULL, 10);
				if (clen < 5 || clen > 8) {
			    fprintf(stderr, ".. Bad data length: %d\n", clen);
					goto OPTERR1;
				}
				break;
		case 'i':
		case 'I':	if (*++p != '\0') goto OPTERR;
				use_stdin = 1;
				break;
		case 'e':
		case 'E':	if (*++p != '\0') goto OPTERR;
				ignore_echo = 1;
				break;
		case 'f':
		case 'F':	switch(*++p) {
				  case 'e':
				  case 'E': filecd = 0; break;
				  case 's':
				  case 'S': filecd = 1; break;
				  default:  goto OPTERR;
				}
				if (*(p + 1) != '\0') goto OPTERR;
				break;
		case 's':
		case 'S':	if (*++p != '\0') goto OPTERR;
				sjis = 1;
				break;
		case 'p':
		case 'P':	switch(*++p) {
				case 'e':
				case 'E': par = 2; break;
				case 'o':
				case 'O': par = 1; break;
				default:  goto OPTERR;
				}
				break;
		case 'b':
		case 'B':	if (*(p + 1) != '\0') goto OPTERR;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':	if (*(p + 1) == '\0') {
					switch(*p) {
					case 'b':
					case 'B': baud = 115200; break;
					case '5': baud = 57600;	break;
					case '3': baud = 38400;	break;
					case '1': baud = 19200;	break;
					case '9': baud = 9600;	break;
					case '4': baud = 4800;	break;
					case '2': baud = 2400;	break;
					default:  baud = *p;
					}
				} else {
					baud = atoi(p);
				}
				if (checkBaud(baud) < 0) {
			fprintf(stderr, ".. Bad baud rate: %d\n", baud);
					goto OPTERR1;
				}
				break;
		case 'h':
		case 'H':
		case '?':
  fprintf(stderr, "usage: gterm [option]\n");
  fprintf(stderr, "  -l<dev> Line device (default: /dev/ttyS1, /dev/com2)\n");
  fprintf(stderr, "  -c<fn>  Command file (default: env GTERM, ~/.gterm)\n");
  fprintf(stderr, "  -<baud> Baud rate (230400, 115200, 19200, 1200 ..)\n");
  fprintf(stderr, "     -b   115200 bps\n");
  fprintf(stderr, "     -5   57600 bps\n");
  fprintf(stderr, "     -3   38400 bps\n");
  fprintf(stderr, "     -1   19200 bps (default)\n");
  fprintf(stderr, "     -9   9600 bps\n");
  fprintf(stderr, "     -4   4800 bps\n");
  fprintf(stderr, "     -2   2400 bps\n");
  fprintf(stderr, "  -y      Stop 2 bits (default: 1 bit)\n");
  fprintf(stderr, "  -z<n>   Data length 5-8 (default: 8)\n");
  fprintf(stderr, "  -pe     Even parity (default: no parity)\n");
  fprintf(stderr, "  -po     Odd parity\n");
  fprintf(stderr, "  -x      RTS/CTS flow control off (default: on)\n");
  fprintf(stderr, "  -i      Use stdin/stdout for console(/dev/tty)\n");
  fprintf(stderr, "  -e      Console echo off (default: on)\n");
  fprintf(stderr, "  -fe     File encoding is EUC(default)\n");
  fprintf(stderr, "  -fs     File encoding is Shift JIS\n");
  fprintf(stderr, "  -s      Shift JIS console mode (default: EUC)\n");
  fprintf(stderr, "  -V      Print version\n");
  fprintf(stderr, "  -h -?   This help message\n");
			sys_end(0);
		case 'V':
  fprintf(stderr, "%s", startmsg);
			sys_end(0);
		default:
OPTERR:
			fprintf(stderr, ".. Illegal option: %s\n", *av);
OPTERR1:
			fprintf(stderr, ".. for help, use gterm -h\n");
			sys_end(0);
		}
	}

	/* コマンドファイルのオープン */
	if (!fp) {
		p = getenv("GTERM");
		if (!(fp = f_open(p ? p : (uchar*)".gterm", "r"))) {
			fp = f_open((uchar *)"~/.gterm", "r");
		}
	}
	if (fp) {infp[0] = fp; nest = 1;}

	/* コンソール(TTY) の初期化 */
	if (TYinit(use_stdin, ignore_echo)) {
		fprintf(stderr, ".. TTY INIT error\n");
		sys_end(1);
	}

	/* 文字コードの設定 */
	SetCode(sjis);

	if (ln == NULL) {
		ln = (uchar*)DEF_LINE;/* デフォルトのデバイス		*/
	} else {
					/* TE_CVCOM 環境の初期化	*/
		if (getvcomf((char**)&ln) == NULL) {
			sys_end(1);
		}
	}

	if (stat(ln, &st) == -1) {
		/* ソケットのオープン */
		ret = SOinit(ln);
		if (ret == -1) {
			sys_end(1);
		}
	} else if (S_ISCHR(st.st_mode)) {
		/* 回線の初期化 */
		ret = RSinit(ln, clen, par, sbit, noflow, baud);
		if (ret == -1) {
			sys_end(1);
		}
	} else if (S_ISSOCK(st.st_mode)) {
		/* UNIXドメインソケットのオープン */
		ret = SOlocalinit(ln);
		if (ret == -1) {
			sys_end(1);
		}
	} else {
		TYouts(".. Unknown Device\n");
		sys_end(1);
	}

	/* ネスト入力用バッファの初期化 */
	for (i = 0; i < MAX_NEST; i++) {
		if (!(args[i] = calloc(LINE_LEN, 1))) {
			TYouts(".. Can't allocate memory\n");
			sys_end(1);
		}
	}

	/* 初期コマンドファイルへのパラメータ保存 */
	if (nest) {
		ln = args[0];
		while (p = *av++) {
			strcpy(ln, p);
			ln += strlen(ln);
			*ln++ = ' ';
		}
		*ln = 0;
	}

	/* 開始メッセージ */
	TYouts(startmsg);

	signal(SIGHUP, (void*)finish);
	signal(SIGQUIT, (void*)finish);
	signal(SIGTERM, (void*)finish);
	signal(SIGCONT, (void*)restart);

	loadmode = 0;
	mode = 1;
	quit = 0;
	def_end = def_top;
	alias_end = alias_top;
	*linebuf = 0;
	endp = curp = linebuf;
	xinterrupt = 0;
	last[2] = 0;

	/* 再開エントリの設定 */
	if (sigsetjmp(int_jump, 0)) {
		sigset_t set;

		while (nest) fclose(infp[--nest]);
		wait_flg = do_break = cons_off = 0;
		Puts("^C");
		RSputc('C'-'@');
		show_prompt();
		xinterrupt++;

		sigemptyset(&set);
		sigaddset(&set, SIGINT);
		sigprocmask(SIG_UNBLOCK, &set, NULL);
	}
	signal(SIGINT, (void*)int_break);

	idle = gettime();

	/* 定常ループ処理 */
  do {

	/* コンソール出力のフラッシュ */
	TYflush();

	/* 回線からの入力の処理 */
	tmo = (mode > 0 && nextcmd())? 0: -1;
	while ((i = RSgetc(0, tmo)) > 0) {
		c = i;
		if (c == 'S'-'@' || c == 'Q'-'@') continue;
		if (c == 0x10) {	/* DLE */
			loadmode = 1;
			continue;
		}
		if (loadmode) {		/* recv コマンドの応答処理 */
			if (loadmode == 1) {
				if (c != 'S' && c != 's') {
					loadmode = 0;
				} else {
					loadmode = 2;
					loadmem = (c == 's') ? kMEMIMG : 0;
				}
				continue;
			}
			if (c == '\n') {
				loadpath[loadmode - 2] = 0;
				loadmode = 0;
				do_load(kRECV | kXMODEM | loadmem, loadpath);
				last[2] = mode = 0;
				LFOUT;
				TYflush();
			} else if (c > ' ') {
				loadpath[loadmode++ - 2] = c;
			}
			continue;
		}

		/* 回線からの入力文字の処理 */

		mode = 0;
		last[0] = last[1];
		last[1] = last[2];
		last[2] = c;

		if (c == ESC) {
			while ((c = RSin()) <= 0);
			switch(c & 0x7f) {
			case 'A':	cur_up();	break;
			case 'B':	cur_dwn();	break;
			case 'C':	cur_fwd();	break;
			case 'D':	cur_bwd();	break;
			case 'H':	cur_home();	break;
			case 'Y':	while ((c = RSin()) <= 0);
					while ((c1 = RSin()) <= 0);
					cur_addr(c-' ', c1-' '); break;
			case 'E':	scrn_clr();	break;
			case 'J':	scrn_erase();	break;
			case 'K':	scrn_erase();	break;
			case 'X':	delm_on();	break;
			case 'W':	delm_off();	break;
			case 'R':	rev_on();	break;
			case 'O':	rev_off();	break;
			}
		} else if (c == BS) {
			cur_bs();
		} else if (!cons_off) {
			if (ts_flg && line_top++ == 0) {
				/* 行の先頭のとき、タイムスタンプ表示 */
				time_stump();
			}
			if (!LFflg || c != LF) {
				if (c < ' ' && (c !=LF && c !=CR && c !=TAB)) {
					Putc('^');
					c += '@';
				}
				Putc(c);
				if (c == LF) line_top = 0;
			}
		}
		LFflg = 0;
		idle = gettime();

		if (nextcmd()) {
			/* 待ちを長くするほど、プロンプト判定の精度が高くなる
			 * が、その分待ってからコマンドを発行することになるの
			 * で遅くなる。
			 */
			tmo = in_wait;
		}
	}

	/* 回線からの入力が無くなるのを待つ */
	if (!nextcmd() && in_wait > 0 && gettime() - idle < in_wait) {
		sleep_ms(10);
		continue;
	}
	TYflush();

	if (mode == 0) {
		/* プロンプトによりコマンド入力モードに入るかのチェック */
		endp = curp = linebuf;
		for (i = 0; i < MAX_PROMPT; i++) {
			if ((j = strlen(prompt[i])) &&
					strcmp(prompt[i] + (j > 3 ? j - 3 : 0),
					&last[j > 3 ? 0 : 3 - j]) == 0) {
				mode = i + 1;
				break;
			}
		}
		if (xinterrupt && mode == 0) {
			mode = 1;
			show_prompt();
		}
	}

	if (mode) {			/* コマンド入力モード */
		loadmode = 0;
LOOP:
		/* コマンド行の入力 */
		if (get_line(do_break)) {
			/* Alias, Argument の処理 */
			xinterrupt = 0;
			endp = curp = linebuf;
			if (alias_line(lbuf, linebuf)) goto LOOP;
			args_line(llbuf, lbuf);
			make_line(lbuf, llbuf, 1);
			last[2] = 0;
			if (lbuf[0] != cmd_ch || (quit = do_cmd(lbuf)) == 0) {
				/* 疑似コマンドでない場合は回線に出力 */
				make_line(lbuf, llbuf, 0);
				RSputs(lbuf);
				last[2] = mode = 0;
			}
		}

	} else {			/* 直接入力モード */
		/* コンソールからの入力文字を回線に出力 */
		while (c = TYin()) {
			if (c == DEL) c = BS;
			else if (c == BS) c = DEL;
			else if (c == LF) c = CR;
			else if (c < ' ') {
				switch (Keytest(c)) {
				case CUR_UP:	c = 'A';	break;
				case CUR_DWN:	c = 'B';	break;
				case CUR_FWD:	c = 'C';	break;
				case CUR_BWD:	c = 'D';	break;
				case CUR_UPx:	c = 'K'-'@';	break;
				case CUR_DWNx:	c = 'J'-'@';	break;
				case CUR_FWDx:	c = 'L'-'@';	break;
				case CUR_BWDx:	c = 'H'-'@';	break;
				case CANCEL:	c = 'U'-'@';	break;
				case SCRN_CLR:	c = 'E';	break;
				case LINE_ERASE:c = 'K';	break;
				}
				if (c > ' ') {
					RSputc(ESC);	sleep_ms(80);
					RSputc(c);	sleep_ms(80);
					c = 0;
				}
			}
			if (c) RSputc(c);	/* 回線に出力 */
		}
	}
  } while (quit >= 0);

	/* 終了メッセージ */
	TYouts(END_MSG);
	sys_end(0);
}
