/*
	databox.c	データボックスコンパイラ

	Copyright (C) 1989-2012 by Personal Media Corporation
*/

#if	0
/*======================================================================

■ 動作方法：

	databox［-b]［-n] [-l] [-s[a]] 入力ファイル名 [出力ファイル名]

		-b : ビッグエンディアン形式
		-n : データボックス形式でない。ヘッダ部分なしで、単なるバイナリ
		     データを生成するために使用する。
		-l : リスティング出力 (*.lst)
		-s : ソース出力
		-a : ソース出力のアライメントを long にする

	 -- 出力ファイル名省略時は標準出力
	 -- リスティングファイル名は、(入力ファイル名).lst
	 -- 何らかのエラーがあるとメッセージを表示し、そこで実行は停止する。

■ 入力ファイル形式:

	全体の形式:
		<データタイプ指定>
		<データ番号指定>
		<データ指定>

		<データ番号指定>
		<データ指定>
		    : :

		上記の繰り返し。

		-n オプション指定の場合は<データ指定＞のみとなる。

	コメント:
		'--' 以降の文字は行末までコメントとみなして読み飛ばす。

	区切り文字:
		空白、制御文字(改行、タブ含む)、','、 ';'、'('、')'、'{'、'}'
		はすべて空白と見なされる。
		行の区切りはコメントの終了以外の特別の意味は持たない。

	データタイプ指定:
		% の後にデータタイプを式(後述)で指定する。
		その後に属性を指定する。
		-n オプション指定の場合はデータタイプ指定はあってはいけない。

		例:	% 1  0

	データ番号指定:
		# の後に、データ番号、ユーザー情報を式(後述)で指定する。
		-n オプション指定の場合はデータ番号指定はあってはいけない。

		例:	# 1 0 0

	データ指定:
		# 指定の直後から、次の % または # までデータとみなす。

	データ項目:
		データ項目としては、文字列と式がある。

	TRON 文字列:
		" で囲まれた EUC 漢字文字列で、TRON コードに変換される。

		最後の " の直後に数字が続く場合は、その数字は文字数を示し、
		文字数に満たない場合は０がパッドされる。

		エスケープコード:
			\n, \f, \t, \r, \b	制御コード(上位バイトは 0)
			\(8 進数 3 桁まで)	任意のコード
			\x(16 進数 4 桁まで)	任意のコード

		半角文字(英数字)の場合は、上位バイトに 0 がパッドされ、
		WARNING がでる。

		例:	"漢字文字列\0"
			"図形"16	-- "図形"の後に 14 個の 0 が入る

	ASCII 文字列:
		' で囲まれた ASCII 文字列。EUC 漢字文字列は
		そのままとなり、エンディアンの変換は行われない。
		奇数文字数の場合は、最後にバイト 0 がパッドされる。

		最後の ' の直後に数字が続く場合は、その数字は文字数 / 2 を示し
		文字数に満たない場合は 0  がパッドされる。

		エスケープコード:
			\n, \f, \t, \r, \b	制御コード
			\(8 進数 3 桁まで)	任意のコード
			\x(16 進数 2 桁まで)	任意のコード

		例:	'ASCII'		-- 最後にバイト 0 がパッドされる。
			'TEXT'16	-- 'TEXT'の後に 28 個のバイト 0 が入る

	イメージデータ:
		` で囲まれた■□の並びはイメージデーターとして処理される。

		例:	`□□□□□□■■□□□□□□□□`	-- 0x03 0x00

	CBMP:
	CBMPIMG:
		$CBMP(compac planes pixbits rowbyte rect data) で CBMP 形式の
		データを生成する。
		data は ■□ の並びで指定する。■は黒に、□は白に変換される。

	    ※	planes == 1, pixbits == 0x808, comapc == 0 以外のデータは処理
		できない。

	    ※	$CBMP の代りに $CBMPIMG とすると、ビットマップのイメージデータ
		のみを生成する (BMP 構造体自身は生成されない)

	式:
		データ要素、または任意個のデータ要素をオペレータで連結したもの

		オペレータは、'+' '-'  '*'  '/' で、左から順番に演算される。
		式中に空白等の区切り文字があってはいけない。

		式のデータ長は、BYTE(8bit)、WORD(16bit)、LONG(32bit)のいずれか
		で、式の結果は式中のデータ要素で最も長いデータ長になる。

		例:	0x100H+0x10Y	-- 0x110 (WORD)
			128/2L		-- 64	 (LONG)

	データ要素:
		<数値>		BYTE/WORD/LONG (BASE に従う)
		<数値>Y		BYTE( 8ビット)
		<数値>H		WORD(16ビット)
		<数値>L		LONG(32ビット)

		数値は、以下の表現が可能
		<10 進数字列>		10 進数
		0x<16 進数字列>		16 進数
		0<8 進数字列>		8 進数

		<シンボル>		シンボル値(BYTE/WORD/LONG)
					BYTE/WORD/LONG はシンボルの定義値による
		<シンボル>:Y		シンボル値(BYTE)
		<シンボル>:H		シンボル値(WORD)
		<シンボル>:L		シンボル値(LONG)
		( <シンボル>:W	シンボル値(WORD) )

	シンボル定義:
		英字('_'を含む)で始まる最大 16 文字までの英数字のシンボル
		最大 1000 個。

		.<シンボル> = <式>
			シンボルの値を<式>の値とする。
			シンボルのデータ長は<式>のデータ長となる。

		.<シンボル>:
			現在のデータオフセットをシンボルの値とする。
			シンボルのデータ長は設定されない(不定)。
			このシンボルを参照したときの BASE の設定によりデータ長
			が決定される。

			データオフセットは、個々のデータ指定の先頭を 0 とした
			バイトオフセットとなる。

		シンボルにはグローバルシンボルと、ローカルシンボルがある。

		グローバルシンボル:
			最初のデータ番号指定の前に定義したシンボルで、
			全体にわたって有効。

		ローカルシンボル:
			データ番号指定の後に定義したシンボルで、そのデータ番号
			のデータ内でのみ有効。

		シンボルの値の再設定は可能であるが、WARNING がでる。
		..<シンボル> とした場合は、WARNING はでない。

		例:
			.Global = 0x1111L
			% 1 0
			# 1 0 0
			Global		-- 0x1111 0x0000
			Global:W-1	-- 0x1110
			.offset:
			offset		-- 0x0006
			#2 0 0
			offset:L	-- 0x0004 0x0000
			.offset:

		シンボルの値の前方参照は可能であるが、シンボルが
		まだ未定義の場合、データ長は BASE の設定にしたがった長さと見な
		すため、シンボルの値のデータ長が異なるときには、明示的に表記す
		る必要がある。

		例:
			A
			undef
			.A:
			.undef = 10L

			 の場合、A の値は不正になるので、
			以下の記述とする必要がある。

			A:
			undef:L    -- または undef+0L など
			.A:
			.undef = 10L


	システムシンボル:

		BASE	-- 数値の基数を示す。
			   初期値は 0。 0 以外を設定すると、数字列はその値を
			   基数として解釈される。
			   LONG の値を設定すると、LONG の数字となる。同様に、
			   BYTE の値を設定すれば BYTE の数値、WORD の値を設定
			   すれば WORD の数値となる。デフォルトは WORD。
			   例:
				.BASE = 16
				0ffff 0aaaa 5555	-- 16 進数(WORD)
				.BASE = 0
				5555			-- 10 進数(WORD)
				.BASE = 16L
				0ffff 0aaaa 5555	-- 16 進数(LONG)
				.BASE = 0L
				5555			-- 10 進数(LONG)

		OFFSET	-- その時点のオフセットを示す。
			   参照時の BASE の設定によりデータ長が決定される。
			   例:
				OFFSET+2	-- "文字"へのオフセット(WORD)
				"文字"

				OFFSET+4L	-- "文字"へのオフセット(LONG)
				"文字"

		ALIGN	-- アラインメントを指定する。次に.ALIGN が指定され
			   るまで#???や.????:によるオフセットは、指定したア
			   ラインメントにあわせられる。
			例:
				[00]はパディング
							.ALIGN = 1
				0000	01		.p01:	1Y
				0001	01		.p02:	1Y
							.ALIGN = 2
				0002	01 [00]		.p03:	1Y
				0004	01 [00 00 00]	.p04:	1Y
							.ALIGN = 4
				0008	01		.p05:	1Y
							.ALIGN = 1
				0009	01		.p06:	1Y

		標準データタイプ指定(WORD)

			PTR_DATA	1
			PICT_DATA	2
			PAT_DATA	3
			BMAP_DATA	4
			FIG_DATA	5
			TEXT_DATA	6
			PARTS_DATA	7
			MENU_DATA	8
			GMENU_DATA	9
			PANEL_DATA	10
			USER_DATA	64

■ 出力ファイル形式:

	 - - - - - -+-----------------------+
		    |	ヘッダ ( 16 バイト) |
	 イ	    |- - - - - - - - - - - -|
	 ン	+---|	インデックス  1     |		N: データ項目数
	 デ	|   |   (10 + 16 N バイト)  |---+-+
	 ッ	+-->|- - - - - - - - - - - -|	| |
	 ク	+---|	インデックス  2     |	| |
	 ス	|   |			    |	| |
	 部	+-->|- - - - - - - - - - - -|	| |
		    |			    |	| |
		   //			    //	| |
		    |			    |	| |
	 - - - - - -+-----------------------+	| |
		    |	データ本体 A	    |<--+ |
		    |			    |	  |
	 デ	    |- - - - - - - - - - - -|	  |
	 ｜	    |	データ本体 B	    |<----+
	 タ	    |			    |  : :
	 部	    |- - - - - - - - - - - -|
		    |			    |
		   //			    //
		    |			    |
	 - - - - - -+-----------------------+

■ リスティング形式:

	先頭にインデックス部の内が、16 進数で出力される。
	データ部は、ソース行の直後に対応するデータが 16 進数で出力される。
	データ部のアドレスは、データ部の先頭を 0 としたアドレスで、
	インデックス部はふくまれない。

======================================================================*/
#endif

/* クロス環境のヘッダ */
#include <stdio.h>
#include <ctype.h>

/* BTRON 仕様の型定義 */
typedef char		B;	/* 符号付き  8ビット整数 */
typedef short		H;	/* 符号付き 16ビット整数 */
typedef int		W;	/* 符号付き 32ビット整数 */
typedef unsigned char	UB;	/* 符号無し  8ビット整数 */
typedef unsigned short  UH;	/* 符号無し 16ビット整数 */
typedef unsigned int	UW;	/* 符号無し 32ビット整数 */

typedef	unsigned char	BYTE;
typedef	short		WORD;
typedef	unsigned short	UWORD;
typedef	int		LONG;
typedef	unsigned int	ULONG;
typedef	unsigned short	TCODE;

#define	FAST		register

typedef struct {
	H	ntyp;		/* ボックス内のデータタイプ ID の数 */
	H	ixsize;		/* インデックス部の総バイト数 */
	H	resv[6];
} DBOX_HDR;

typedef struct {
	W	id;		/* データ番号 */
	W	pos;		/* 対応するデータ本体へのオフセット */
	W	size;		/* データ本体のバイトサイズ */
	W	info[2];	/* 内部使用領域 */
} DNUM_INX;

typedef struct {
	W	npos;		/* 次のタイプのインデックスへのオフセット */
	W	typ;		/* データタイプ ID */
	UW	attr;		/* データの属性 */
	W	nd;		/* このタイプに属するデータ項目の数 */
	DNUM_INX dix[1];	/* 項目インデックス配列 (nd 個の要素) */
} DTYP_INX;

#define	INDEXSZ		1024 * 20


FILE	*infile, *oufile, *tmpf, *lstfile;

WORD	bigend = 0;
WORD	cpuend = 0;
WORD	noheader = 0;
WORD	list = 0;
WORD	source = 0;
LONG	lstaddr = 0L;
WORD	ch;
LONG	dcnt, tycnt, idno, lineno;
BYTE	 *dps;
DTYP_INX *dp;
BYTE	ofn[128];
BYTE	ifn[128];
#define	BUF_SIZE	1024
BYTE	buf[BUF_SIZE];
BYTE	*bufp;

/* symbol table */
#define	MAX_SYM		2000
WORD	max_sym, pass2;
WORD	sym_level = 0;
LONG	id_base = 0L;

WORD	system_symbols;

typedef enum LV {
	LV_UNDEF	= 0,
	LV_BYTE		= 1,
	LV_WORD		= 2,
	LV_LONG		= 3
} LV;

struct {
	BYTE	nam[16];
	LONG	val;
	LV	lv;
	WORD	level;
} symbol[MAX_SYM];

typedef struct {
	LONG	val;
	LV	lv;
} DmpBuf;

DmpBuf	dbuf[BUF_SIZE * 4];
DmpBuf	*dbufp;

#define	TO_WORD(x,y)	((x << 8) | y) & 0x7f7f
#define	PAD_WORD(x)	x & 0xff
#define	U_BYTE(x)	(x >> 8) & 0xff
#define	L_BYTE(x)	x & 0xff
#define	CWORD(x)	(bigend ? x : ((x & 0xff)<<8)|((x>>8) & 0xff))
#define CLONG(x)	(bigend ? x : ((x & 0xff)<<24)|((x & 0xff00)<<8)|((x & 0xff0000)>>8)|((x >>24) & 0xff))

#define	TO_XWORD(x,y)	(bigend ? (x<<8|y):(y<<8|x))

#define	XCWORD(x)	(bigend == cpuend ? x :((x & 0xff)<<8)|((x>>8) & 0xff))
#define XCLONG(x)	(bigend == cpuend ? x :((x & 0xff)<<24)|((x & 0xff00)<<8)|((x & 0xff0000)>>8)|((x >>24) & 0xff))

#define ISSP(c) 	(c == ' ')

#ifdef	WIN			/* for MS-DOS or Win */
#define	KANJI(c)	((c >= 0x80 && c < 0xa0) || c >= 0xe0)
#define	HANKANA(c)	(c >= 0xa0 && c < 0xe0)
UH	sjistojis (UH sj);
#define	chgtojis(x)	(sjistojis(x))
#else				/* for Sun */
#define	KANJI(c)	(c & 0x80)
#define	HANKANA(c)	(c == 250)
#define	chgtojis(x)	(x & 0x7f7f)
#endif

/*======================================================================*/
main(ac,av)
/*======================================================================*/
WORD	ac;
BYTE	**av;
{
	FAST BYTE	*cp;
	FAST WORD	i;
	union	{
		BYTE	b[2];
		WORD	w;
	} chk;

	/* CPU エンディアンのチェック */
	chk.w = 0xAA55;
	cpuend = (chk.b[0] == 0x55) ? 0 : 1;	/* 0: little, 1: big	*/

	/* get option switches */
	while (--ac && *(cp = (*++av)) == '-' ) 
	    while (*++cp)
		switch (*cp) {
		    case 'b': case 'B': 	/* big endian */
			bigend = 1;	break;
		    case 'n': case 'N': 	/* no header */
			noheader = 1;	break;
		    case 'l': case 'L': 	/* listing */
			list = 1;	break;
		    case 's': case 'S': 	/* source */
			source = 2;	break;
		    case 'a': case 'A': 	/* source alignment */
			source = 4;	break;
		    default :
			usage();
		}

	
	if (ac < 1 || ac > 2) usage();
	strcpy (ifn, cp);
#ifdef	WIN
	if ((infile = fopen((char *)ifn,"r")) == NULL)
					eexit("Input file not found");
#else
	if ((infile = fopen(ifn,"r")) == NULL)
					eexit("Input file not found");
#endif
	cp = *++av;
	if (ac <= 1) {oufile = stdout; ofn[0] = 0;}
	else {
	    strcpy (ofn,cp);
#ifdef	WIN
	    oufile = fopen((char *)ofn, source == 0 ? "wb" : "w");
#else
	    oufile = fopen(ofn, "w");
#endif
	    if (oufile == NULL) eexit ("Output file cant open");
	}
	tmpf = tmpfile();			/* open temporary file */

	if (list) {
	    for (i = strlen(ifn); --i > 0;)
			if (ifn[i] == '.') {ifn[i] = 0; break;}
	    strcat (ifn, ".lst");
#ifdef	WIN
	    if ((lstfile = fopen((char *)ifn,"w")) == NULL)
					eexit ("Listing file cant open");
#else
	    if ((lstfile = fopen(ifn,"w")) == NULL)
					eexit ("Listing file cant open");
#endif
	}

	make_data();				/* make data file */

	fclose(infile);
	fclose(oufile);

	return 0;
}
/*----------------------------------------------------------------------*/
WORD get_ch()					/* get 1 character	*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	c;

	c = fgetc (infile);
	if (pass2 && lstfile && c != EOF) fputc (c, lstfile);
	return (c);
}
/*----------------------------------------------------------------------*/
WORD in_ch(flg)					/* get 1 character	*/
/*----------------------------------------------------------------------*/
int	flg;
{
	/* ignore chars after COMMENT till NL */

static	WORD	unget_ch = -1;
static	WORD	nlflg = 0;
static	WORD	kanji2 = 0;
	LONG	pos;
	DmpBuf	*p;
	char	*fmt;

	if (nlflg) {
	   if (pass2 && lstfile && (dbufp != dbuf)) {
		fprintf (lstfile, "=%04x: ", lstaddr);
		for (p = dbuf; p < dbufp; p++) {
		    switch ( p->lv ) {
		    case LV_BYTE: fmt = "%02x "; lstaddr += 1; break;
		    case LV_WORD: fmt = "%04x "; lstaddr += 2; break;
		    case LV_LONG: fmt = "%08x "; lstaddr += 4; break;
		    default: ;
		    }
		    fprintf (lstfile, fmt, p->val);
		}
		fprintf (lstfile, "\n");
	    }
	    dbufp = dbuf;
	    nlflg = 0;
	}

	if (unget_ch != -1) {
	    ch = unget_ch;
	    unget_ch = -1;

	} else {
	    if ((ch = get_ch()) == '-') {
		if ((ch = get_ch()) == '-')
		    while ((ch = get_ch()) != '\n' && ch != EOF);
		else {unget_ch = ch; ch = '-';}
	    }
	}
	if (ch == '\n') {
	    lineno++;
	    ch = ' ';
	    nlflg++;
	    kanji2 == 0;
	} else if (ch != EOF) {
#ifdef	WIN
	    if (kanji2 == 1) {
		kanji2 = 0;
	    } else {
		if (KANJI(ch)) {
		    kanji2 = 1;
		}
		if (flg) {
		    if (ch < ' ' && ch != '\t')
			ch = ' ';
		} else {
		    if (ch <= ' ' || ch == ',' || ch == ';' || ch =='(' ||
			ch == ')' || ch == '{' || ch == '}') ch = ' ';
		}
	    }
#endif
	    if (flg) {
		if (ch < ' ' && ch != '\t') ch = ' ';
	    } else {
		if (ch < ' '  || ch == ',' || ch == ';' || ch == '(' ||
		    ch == ')' || ch == '{' || ch == '}' )  ch = ' ';
	    }
	}
	return (ch);
}
/*----------------------------------------------------------------------*/
skip_space()					/* skip space		*/
/*----------------------------------------------------------------------*/
{
	while (ISSP(ch)) in_ch(0);
}
/*----------------------------------------------------------------------*/
next_ch()				/* get next non-space char	*/
/*----------------------------------------------------------------------*/
{
	do {in_ch(0);} while (ISSP(ch));
}
/*----------------------------------------------------------------------*/
WORD search_symbol (sym)			/* search symbol	*/
/*----------------------------------------------------------------------*/
BYTE	*sym;
{
	FAST WORD	i;

	for (i = max_sym; --i >= 0; ) {
	    if ( (symbol[i].level == sym_level || symbol[i].level == 0) &&
			strncmp (sym, symbol[i].nam, 16) == 0) break;
	}
	return (i);
}
/*----------------------------------------------------------------------*/
LONG get_symbol (p, pp, lv)			/* get symbol value	*/
/*----------------------------------------------------------------------*/
FAST BYTE	*p;
BYTE		**pp;
LV		*lv;	/* シンボル値のビット長を返す */
{
	FAST WORD	i, f;
	BYTE		sym[17];

	memset (sym, 0, 17);
	for (i = 0; isalnum(*p) || *p == '_'; p++) {if (i < 16) sym[i++] = *p;}

	f = 0;
	*lv = symbol[0].lv; /* BASE */
	if (*p == ':') {
	    f = 1; p++;
	    switch (*p++) {
	    case 'L':
		*lv = LV_LONG;	break;
	    case 'W':
	    case 'H':
		*lv = LV_WORD;	break;
	    case 'Y':
		*lv = LV_BYTE;	break;
	    default:
		f = 0; p--;
	    }
	}
	*pp = p;

	/* local symbols */
	if ((i = search_symbol(sym)) >= 0) {
	    if (f == 0) {
		if ((*lv = symbol[i].lv) == LV_UNDEF)
		    *lv = symbol[0].lv; /* BASE */
	    }
	    return ((i == 1)? (ftell(tmpf) - id_base) : symbol[i].val);
	}

	if (pass2) errexit ("Undefined Symbol", sym);
	return (0L);
}
/*----------------------------------------------------------------------*/
LONG get_value (vp)				/* get value		*/
/*----------------------------------------------------------------------*/
LONG	*vp;
{
	FAST WORD	i;
	FAST LONG	v;
	LV		lv, l;
	BYTE		*p;

	*vp = 0;
	if (!isalnum(ch) && ch != '-') return (-1);

	for (i = 0; !ISSP(ch);in_ch(0))
		if (i < BUF_SIZE)
			buf[i++] = ch;
		else
			fprintf (stderr,"WARNING: value too long AT LINE %d\n", lineno);

	buf[i] = 0;

	lv = LV_BYTE;
	i = 0; /* + */
	for (p = buf; *p; ) {
	    bufp = p;
	    if (isalpha(*p)) {
		v = get_symbol (p, &p, &l);
	    } else {
#ifdef	WIN
		v = strtoul ((char *) p, (char *) &p, (int) symbol[0].val);
#else
		v = strtoul (p, &p, (WORD)symbol[0].val);
#endif
		switch ( *p++ ) {
		case 'L': case 'l':	l = LV_LONG; break;
		case 'H': case 'h':	l = LV_WORD; break;
		case 'Y': case 'y':	l = LV_BYTE; break;
		default:		l = symbol[0].lv; --p; /* BASE */
		}
	    }
	    if ( l > lv ) lv = l;

	    if (i == 1) 	*vp -= v;
	    else if (i == 2)	*vp *= v;
	    else if (i == 3)	*vp /= v;
	    else		*vp += v;

	    if (*p == '+') {i = 0; p++;}
	    else if (*p == '-') {i = 1; p++;}
	    else if (*p == '*') {i = 2; p++;}
	    else if (*p == '/') {i = 3; p++;}
	    else if (*p) errexit ("Illegal Value", p);
	}
	return (lv);
}
/*----------------------------------------------------------------------*/
def_symbol (sym, v, lv, warn)			/* define symbol	*/
/*----------------------------------------------------------------------*/
BYTE	*sym;
LONG	v;
LV	lv;
WORD	warn;
{
	FAST WORD	i;

	if ((i = search_symbol(sym)) < 0) {
	    i = max_sym++;
	    if (max_sym >= MAX_SYM) errexit ("too many symbols", sym);
	    symbol[i].level = (warn & 0x1000)? 0 : sym_level;
	} else if (pass2 == 0 && i && i != 2) {
		if (i < system_symbols)
fprintf (stderr, "WARNING: system symbol '%s' is re-defined AT LINE %d\n",
					sym, lineno);
		else if (warn & 1)
fprintf (stderr, "WARNING: symbol '%s' is multi-defined AT LINE %d\n",
					sym, lineno);
	}
	strncpy (symbol[i].nam, sym, 16);
	symbol[i].val = v;
	symbol[i].lv = lv;

}
/*----------------------------------------------------------------------*/
init_symbol ()					/* initialize symbols	*/
/*----------------------------------------------------------------------*/
{
	system_symbols = 0;

	system_symbols++; def_symbol ("BASE",	0L,  LV_WORD,  1);
							/* must be 1st Entry */
	system_symbols++; def_symbol ("OFFSET", 0L,  LV_UNDEF, 1);
							/* must be 2nd Entry */
	system_symbols++; def_symbol ("ALIGN", 1L,  LV_UNDEF, 1);
							/* must be 3rd Entry */
	system_symbols++; def_symbol ("PTR_DATA",    1L,  LV_WORD, 1);
	system_symbols++; def_symbol ("PICT_DATA",   2L,  LV_WORD, 1);
	system_symbols++; def_symbol ("PAT_DATA",    3L,  LV_WORD, 1);
	system_symbols++; def_symbol ("BMAP_DATA",   4L,  LV_WORD, 1);
	system_symbols++; def_symbol ("FIG_DATA",    5L,  LV_WORD, 1);
	system_symbols++; def_symbol ("TEXT_DATA",   6L,  LV_WORD, 1);
	system_symbols++; def_symbol ("PARTS_DATA",  7L,  LV_WORD, 1);
	system_symbols++; def_symbol ("MENU_DATA",   8L,  LV_WORD, 1);
	system_symbols++; def_symbol ("GMENU_DATA",  9L,  LV_WORD, 1);
	system_symbols++; def_symbol ("PANEL_DATA", 10L,  LV_WORD, 1);
	system_symbols++; def_symbol ("USER_DATA",  64L,  LV_WORD, 1);
}
/*----------------------------------------------------------------------*/
put_symbol()					/* put symbol value	*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	i;
	FAST WORD	warn = 0;
	LONG		v;
	LV		lv;
	BYTE		sym[17];

	next_ch();
	if (ch == '.') next_ch();
	else warn++;

	if (ch == '!') { next_ch(); warn |= 0x1000;}

	if (!isalpha(ch)) errexit ("Illegal Symbol format", NULL);

	memset (sym, 0, 17);
	for (i = 0; isalnum(ch) || ch == '_'; in_ch(0))
					if (i < 16) sym[i++] = ch;
	skip_space();
	if (ch == ':') {
	   {				/* .ALIGN */
		W	align;

		align = symbol[2].val - 1;	/* bit mask*/
		if (align < 0)
			align = 0;
		while (dcnt & align)
			put_byte(0);
	   }
	   v = ftell(tmpf) - id_base;
	   lv = LV_UNDEF;
	   next_ch();
	} else {
	   if (ch != '=') errexit ("Illegal Symbol Definition format", NULL);
	   next_ch();
	   lv = get_value (&v);
	}
	def_symbol (sym, v, lv, warn);
}
/*----------------------------------------------------------------------*/
put_byte(n)				/* put byte to temp file	*/
/*----------------------------------------------------------------------*/
BYTE	n;
{
	if (pass2) {
		dbufp->val = (LONG)n & 0xffL;
		dbufp->lv = LV_BYTE;
		dbufp++;
	}
	fputc(n, tmpf);
	dcnt += 1;
}
/*----------------------------------------------------------------------*/
put_word(n)				/* put word to temp file	*/
/*----------------------------------------------------------------------*/
WORD	n;
{
	if (pass2) {
		dbufp->val = (LONG)n & 0xffff;
		dbufp->lv = LV_WORD;
		dbufp++;
	}
	n = CWORD(n);
	fputc(U_BYTE(n), tmpf);
	fputc(L_BYTE(n), tmpf);
	dcnt += 2;
}
/*----------------------------------------------------------------------*/
put_long(n)				/* put long to temp file	*/
/*----------------------------------------------------------------------*/
LONG	n;
{
	if (pass2) {
		dbufp->val = n;
		dbufp->lv = LV_LONG;
		dbufp++;
	}
	n = CLONG(n);
	fputc((n >> 24) & 0xff, tmpf);
	fputc((n >> 16) & 0xff, tmpf);
	fputc((n >>  8) & 0xff, tmpf);
	fputc((n      ) & 0xff, tmpf);
	dcnt += 4;
}
/*----------------------------------------------------------------------*/
WORD hex(c)					/* get hex char value	*/
/*----------------------------------------------------------------------*/
WORD	c;
{
	if (isdigit(c)) return (c -'0');
	if (c >= 'A' && c <='F') return ( c -'A' + 10);
	if (c >= 'a' && c <='f') return ( c -'a' + 10);
	return (-1);
}
/*----------------------------------------------------------------------*/
put_kstring()				/* put kanji string "..."	*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	i, n, j, cnt;
	UWORD	k, jisch;

	in_ch(1);
	cnt = 0;
	while ( ch != '"' ) {
	    if (KANJI(ch)) {
	    	if (HANKANA(ch)) {
		    fprintf(stderr,
			"WARNING: hankaku KANA exists AT LINE %d\n",lineno);
		    n = ch;
#ifndef	WIN
		    in_ch(1);		/* Skip KANA Flag (not WIN)*/
#endif
		    jisch = n << 8 | ch;
		} else {
		    n = ch;
		    in_ch(1);
		    k = n << 8 | ch;
		    jisch = chgtojis(k);
		}
		put_word(jisch);
		cnt++;
	    } else {
		if (ch == EOF) return;
		if (ch == '\\') {
		    if (in_ch(1) >= '0' && ch <= '7') {
			n = 0;
			for (i = 0; i < 3; i++) {
			    n = n * 8 + ch - '0';
			    if (in_ch(1) < '0' || ch > '7') break;
			}
			put_word (PAD_WORD(n));
			cnt++;
			continue;
		    }
		    if (ch == 'X' || ch == 'x') {
			n = 0;
			for (i = 0; i < 4; i++) {
			    in_ch(1);
			    if ((j = hex(ch)) < 0) break;
			    n = n * 16 + j;
			}
			put_word (n);
			cnt++;
			if (i >= 4) in_ch(1);
			continue;
		    }
		    if (ch == 'n') ch = '\n';
		    else if (ch == 'f') ch = '\f';
		    else if (ch == 't') ch = '\t';
		    else if (ch == 'r') ch = '\r';
		    else if (ch == 'b') ch = '\b';
		} else {
		    fprintf (stderr,
		"WARNING: hankaku '%c' exists AT LINE %d\n", ch, lineno);
		}
		put_word (PAD_WORD(ch));
		cnt++;
	    }
	    in_ch(1);
	}

	in_ch(0);
	if (isdigit(ch)) {
	    n = 0;
	    do {n = n * 10 + (ch - '0'); in_ch(0);} while (isdigit(ch));
	    if ((n -= cnt) > 0) do {put_word(0);} while (--n);
	}
	skip_space();
}

/*----------------------------------------------------------------------*/
put_string()				/* put ASCII string '...'	*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	i, c, j, cnt, kcnt, pc;

	in_ch(1);
	cnt = kcnt = 0;
	while ( (c = ch) != '\'' ) {
	    if (KANJI(c)) {
#ifdef 	WIN
		if (HANKANA(c))		/* hankaku kana */
#else
		if ((++kcnt & 1) && c == 0250)	/* hankaku kana */
#endif
		    fprintf(stderr,
			"WARNING: hankaku KANA exists AT LINE %d\n",lineno);
	    } else {
		if (c == EOF) return;
		if (c == '\\') {
		    if (in_ch(1) >= '0' && ch <= '7') {
			for (c = i = 0; i < 3; i++) {
			    c = c * 8 + ch - '0';
			    if (in_ch(1) < '0' || ch > '7') break;
			}
			goto NEXT;
		    } else if (ch == 'X' || ch == 'x') {
			for (c = i = 0; i < 2; i++) {
			    in_ch(1);
			    if ((j = hex(ch)) < 0) break;
			    c = c * 16 + j;
			}
			if (i < 2) goto NEXT;
		    } else {
			if (ch == 'n')	    c = '\n';
			else if (ch == 'f') c = '\f';
			else if (ch == 't') c = '\t';
			else if (ch == 'r') c = '\r';
			else if (ch == 'b') c = '\b';
			else		    c = ch;
		    }
		}
	    }
	    in_ch(1);
NEXT:
	    if ((++cnt) & 1)	pc = c;
	    else		put_word (TO_XWORD(pc, c));
	}

	if (cnt & 1)  {put_word (TO_XWORD(pc, 0)); cnt++;}

	in_ch(0);
	if (isdigit(ch)) {
	    i = 0;
	    do {i = i * 10 + (ch - '0'); in_ch(0);} while (isdigit(ch));
	    if ((i -= cnt / 2) > 0) do {put_word(0);} while (--i);
	}
	skip_space();
}

/*----------------------------------------------------------------------*/
put_img()				/* put image data `...`		*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	i, c, j, cnt, kcnt, pc;
	WORD		n;
	WORD		bcnt;
	WORD		byte;
#define	C_ON		0x2223
#define	C_OFF		0x2222
#define	A_OFF		0x2e

	in_ch(1);
	byte = bcnt = cnt = 0;
	while ( (c = ch) != '`' ) {
	    if (KANJI(c)) {
		in_ch(1);
		n = ((c << 8) & 0xff00) | (ch &0xff);
		c = chgtojis(n);

		if (c == C_ON) {
		    byte = (byte << 1) | 1;
		} else if (c == C_OFF) {
		    byte = byte << 1;
		} else {
skip_char:
		    in_ch(1);
		    continue;
		}
	    } else {
		if (isalnum(c)) {
		    byte = (byte << 1) | 1;
		} else if (c == A_OFF) {
		    byte = byte << 1;
		} else {
		    goto skip_char;
		}
	    }

	    in_ch(1);
	    if (((++bcnt) % 8) == 0) {
		if ((++cnt) & 1)	pc = byte;
		else			put_word (TO_XWORD(pc, byte));
		bcnt = byte = 0;
	    }
	}

	if ((bcnt % 8) != 0) {
	    byte <<= (8 - bcnt);
	    if ((++cnt) & 1)	pc = byte;
	    else		put_word (TO_XWORD(pc, byte));
	}
	if (cnt & 1)  {put_word (TO_XWORD(pc, 0)); cnt++;}

	in_ch(0);
#if 0
	if (isdigit(ch)) {
	    i = 0;
	    do {i = i * 10 + (ch - '0'); in_ch(0);} while (isdigit(ch));
	    if ((i -= cnt / 2) > 0) do {put_word(0);} while (--i);
	}
#endif
	skip_space();
}

/*----------------------------------------------------------------------*/
put_bmap()				/* put bmap data		*/
/*----------------------------------------------------------------------*/
{
	WORD		i, c, j, cnt, pc;
	WORD		n;
	LONG		v;
	LONG		size;
	LONG		psz;
	LONG		compac, planes, pixbits, rowbytes;
	WORD		left, top, right, bottom;
#define	CBMP_SIZE	(4 + 4 + 2 + 2 + 8)

	WORD	pixelbits;	/* 1, 2, 4, 8 */
	WORD	pixelmask;	/* 1, 3, 15, 255 */
	WORD	shiftcount;	/* 何ビット読み込んだかを数える(<8) */
	WORD	widthcount;	/* 何ビット読み込んだかを数える(<rowbytes*8) */
	WORD	buffer;
	WORD	imgonly;

	/* $CBMP(compac planes pixbits rowbyte rect data) */
	/* $CBMPIMG(compac planes pixbits rowbyte rect data) */
	do { in_ch(1); } while(isspace(ch));
	if (ch != 'C') goto EEXIT;
	in_ch(1);
	if (ch != 'B') goto EEXIT;
	in_ch(1);
	if (ch != 'M') goto EEXIT;
	in_ch(1);
	if (ch != 'P') {
EEXIT:		errexit ("Must $CBMP or $CBMP_IMG", NULL);
	}
	in_ch(1);
	imgonly = (ch == 'I') ? 1 : 0;

	for(; ch != '('; ) in_ch(1);

	do { in_ch(0); } while(isspace(ch));
	get_value(&compac);

	do { in_ch(0); } while(isspace(ch));
	get_value(&planes);

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	pixbits = v;

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	rowbytes = v;

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	left = v;

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	top = v;

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	right = v;

	do { in_ch(0); } while(isspace(ch));
	get_value(&v);
	bottom = v;

	if (planes != 1 || compac != 0 ||(rowbytes & 1))
	    errexit ("Illegal CBMP format", NULL);

	switch (pixbits) {
		case	0x0101:
			pixelbits = 1;
			pixelmask = 1;
			shiftcount = widthcount = 0;
			break;
		case	0x0202:
			pixelbits = 2;
			pixelmask = 3;
			shiftcount = widthcount = 0;
			break;
		case	0x0404:
			pixelbits = 4;
			pixelmask = 15;
			shiftcount = widthcount = 0;
			break;
		case	0x0808:
			pixelbits = 8;
			pixelmask = 255;
			shiftcount = widthcount = 0;
			break;
		default:
			errexit ("Illegal CBMP format", NULL);
	}

	size = CBMP_SIZE + (psz = rowbytes * (bottom - top)) + 4 * planes;

	if (imgonly == 0) {

		put_long(compac);
		put_long(planes);
		put_word(pixbits);
		put_word(rowbytes);

		put_word(left);		put_word(top);
		put_word(right);	put_word(bottom);

		v = CBMP_SIZE + 4;
		put_long(v);
	}

	in_ch(1);
	cnt = 0;
	while ( (c = ch) != ')' ) {
		if (widthcount >=(right - left))
			c = 0;
		else {
		    if (KANJI(c)) {
			in_ch(1);
			n = ((c << 8) & 0xff00) | (ch & 0xff);
			c = chgtojis(n);

			if (c == C_ON) {
			    c = 0xff;
			} else if (c == C_OFF) {
			    c = 0;
			} else {
skip_char:
			    in_ch(1);
			    continue;
			}
		    } else {
			if (isalpha(c)) {
			    c = 0xff;
			} else if (isxdigit(c)) {
			    if ('0' <= c && c <= '9') c = c - '0';
			    else if ('a' <= c && c <= 'f') c = c - 'a' + 10;
			    else c = c - 'A' + 10;
			} else if (c == A_OFF) {
			    c = 0;
			} else {
			    goto skip_char;
			}
		    }
		    in_ch(1);
		}
		if (shiftcount == 0) {
			buffer = 0;
			shiftcount = 8;
		}
		shiftcount -= pixelbits;
		buffer |= ((c & pixelmask) << shiftcount);
		if ((++widthcount) >= rowbytes * 8 / pixelbits)
			widthcount = 0;
		if (shiftcount)
			continue;
		c = buffer;

		if ((++cnt) & 1)
			pc = c;
		else
			put_word (TO_XWORD(pc, c));
	}
	
	if (shiftcount == 0)
		;
	else if ((++cnt) & 1)
		pc = buffer;
	else
		put_word (TO_XWORD(pc, buffer));
	
	if (cnt & 1) {
		put_word (TO_XWORD(pc, 0));
		cnt++;
	}
	/* データがプレーンのサイズより小さければ 0 で埋める */
	for (; cnt < psz; cnt += 2) put_word(0);

	in_ch(0);
	skip_space();
}

/*----------------------------------------------------------------------*/
make_index()					/* make resource index	*/
/*----------------------------------------------------------------------*/
{
	LONG	ty, attr;

	next_ch();
	get_value (&ty);
	if (ty <= 0 || ty > 0x7fff) errexit ("Illegal DATA Type", bufp);

	skip_space();
	get_value(&attr);			/* optional att */

	if (tycnt) {				/* set index */
	    dp->npos = (BYTE *) &(dp->dix[idno]) - dps;
	    dp = (DTYP_INX *) &(dp->dix[idno]);
	}

	dp->npos = 0L;
	dp->typ = XCLONG(ty);
	dp->attr = XCLONG(attr);
	dp->nd = 0;
	idno = 0;
	tycnt++;
}

/*----------------------------------------------------------------------*/
make_id()					/* make ID record	*/
/*----------------------------------------------------------------------*/
{
	LONG	id, opt, x;

	next_ch();			/* get id number */
	get_value (&id);
	if (id < 0 || id > 0x7fff) errexit ("Illegal DATA ID", bufp);

	dp->dix[idno].id = XCLONG(id);
	dp->dix[idno].pos = id_base = ftell(tmpf);
	dp->dix[idno].size = 0;

	skip_space();
	get_value (&opt);		/* get optional info */
	dp->dix[idno].info[0] = XCLONG(opt);
	skip_space();
	get_value (&opt);		/* get optional info */
	dp->dix[idno].info[1] = XCLONG(opt);
	dp->nd = ++idno;
	dcnt = 0;
	sym_level++;
}
/*----------------------------------------------------------------------*/
fin_make_id()					/* make ID record	*/
/*----------------------------------------------------------------------*/
{
	dp->dix[idno - 1].size = XCLONG(dcnt);
}
/*----------------------------------------------------------------------*/
make_data()					/* make data file main	*/
/*----------------------------------------------------------------------*/
{
	FAST WORD	i, n, seq;
	LONG		v, l;
	DBOX_HDR	*hp;

	/* seq = 0:	to be INDEX (initial)
		 1:	to be DNUM
		 2:	to be DATA
		 3:	to be DATA-END (INDEX or DNUM)	*/


	dps = (BYTE *)malloc (INDEXSZ);		/* memory for data indexes */
	max_sym = 0;
	sym_level = 0;
	init_symbol();
	pass2 = 0;
	lstaddr = 0;

AGAIN:
#if 1
	symbol[0].lv  = LV_WORD;	/* .BASE を初期値にする*/
	symbol[2].val  = 1;	/* .ALIGN を初期値にする*/
#endif
	dp = (DTYP_INX *)(dps + sizeof(DBOX_HDR));

	tycnt = 0;
	sym_level = 0;
	lineno = 1;
#if 0
	seq = (noheader)? 2 : 0;
#else
	if (noheader) {
		seq = 2;
		dcnt = 0;		/*追加*/
	} else
		seq = 0;
#endif
	dbufp = dbuf;

	for (next_ch(); ch != EOF; ) {

	    if (isalnum(ch) || ch == '"' || ch == '\'' || ch == '-' ||
						ch == '`' || ch == '$') {
		if (seq < 2) errexit ("Missing INDEX or DNUM", NULL);
		seq = 3;
		if (ch == '"')		put_kstring ();
		else if (ch == '\'')	put_string ();
		else if (ch == '`')	put_img ();
		else if (ch == '$')	put_bmap ();
		else {
			switch (get_value(&v)) {
			case LV_BYTE: put_byte (v); break;
			case LV_WORD: put_word (v); break;
			case LV_LONG: put_long (v); break;
			default: errexit ("Missing Value", NULL);
			}
		}

	    } else if (ch == '.') {
		put_symbol();

	    } else if (ch == '%') {
		if (seq == 1 || seq == 2 || noheader)
				errexit ("Missing DNUM or DATA", NULL); 
		if (seq == 3) fin_make_id ();
		seq = 1;
		make_index();

	    } else if (ch == '#') {
		{				/* .ALIGN */
			W	align;

			align = symbol[2].val - 1;	/* bit mask*/
			if (align < 0)
				align = 0;
			while (dcnt & align)
				put_byte(0);
		}
		if (seq == 0 || seq == 2 || noheader)
				errexit ("Missing INDEX or DATA", NULL);
		if (seq == 3) fin_make_id ();
		seq = 2;
		make_id();

	    } else errexit ("Illegal data format", NULL);

	    skip_space();
	}

	if (seq != 3) errexit ("Missing DATA", NULL);
	fin_make_id ();

	/* make header */
	if (!noheader) {
	    hp = (DBOX_HDR *)dps;
	    hp->ntyp = XCWORD(tycnt);
	    n = (BYTE *) &(dp->dix[idno]) - dps;
	    hp->ixsize = XCWORD(n);
	    for (i = 0; i < 6; i++) hp->resv[i] = 0;

	    dp = (DTYP_INX *) (dps + sizeof(DBOX_HDR));
	    do {
		l = dp->npos;
		dp->npos = XCLONG(l);
		i = dp->nd;
		dp->nd = XCLONG(i);
		while (--i >= 0) {
		    v = dp->dix[i].pos + n;
		    dp->dix[i].pos = XCLONG(v);
		}
		dp = (DTYP_INX *)(l + dps);
	    } while (l);
	}

	if (pass2++ == 0) {
	    rewind(tmpf);
	    rewind(infile);
	    if (lstfile) {
		lstaddr = /* n */ 0;
		if (!noheader) dump_header(n);
	    }
	    goto AGAIN;
	}

	/* output header */
	if (!noheader && !source) {
	    if (fwrite (dps, 1, n, oufile) != n) eexit ("File write error");
	}

	/* output data */
	rewind (tmpf);
	if ( source > 0 ) {
	    fprintf(oufile, "/* DATABOX Data */\n");
	    fprintf(oufile, "unsigned %s\tdatabox_data[] = {\n",
				( source == 2 )? "short": "long");
	    seq = 0;
	    while ( (i = fgetc(tmpf)) != EOF ) {
		if ( (seq % 16) > 0 )	fprintf(oufile, ", ");
		else if ( seq == 0 )	fprintf(oufile, "\t");
		else { seq = 0;		fprintf(oufile, ",\n\t"); }
		v = i;
		while ( (++seq % source) > 0 ) {
			v <<= 8;
			if ( i != EOF && (i = fgetc(tmpf)) != EOF ) v |= i;
		}
		if ( source == 2 )	fprintf(oufile, "0x%04x", CWORD(v));
		else			fprintf(oufile, "0x%08x", CLONG(v));
	    }
	    fprintf(oufile, "\n};\n");
	} else {
		while  ((i = fgetc(tmpf)) != EOF) fputc(i, oufile);
	}
}
/*----------------------------------------------------------------------*/
WORD	*xdump(n)
/*----------------------------------------------------------------------*/
WORD	n;
{
	WORD	*p;
static	LONG	adr = 0;
	WORD	c;

	fprintf (lstfile, "=%04x: ", adr);
	for (p = (WORD *)&dps[adr]; n > 0; n -= 2, adr += 2) {
		c = *p++;
		c = XCWORD(c);
		fprintf (lstfile, "%04x ", ((LONG)c) & 0xffff);
	}
	fprintf (lstfile, "\n");
	return (p);
}
/*----------------------------------------------------------------------*/
dump_header(sz)
/*----------------------------------------------------------------------*/
WORD	sz;
{
	WORD	*p;
	LONG	n;

	fprintf (lstfile, "**** HEADER PART ***\n");
	p = xdump (sizeof(DBOX_HDR));
	sz -= sizeof(DBOX_HDR);

	while (sz > 0) {
	     n = ((DTYP_INX *)p)->nd;
	     n = XCLONG(n);
	     xdump (sizeof(DTYP_INX) - sizeof(DNUM_INX));
	     sz -= sizeof(DTYP_INX) - sizeof(DNUM_INX);
	     while (n--) {
		p = xdump (sizeof(DNUM_INX));
		if ((sz -= sizeof(DNUM_INX)) <= 0) break;
	     }
	}
	fprintf (lstfile, "**** DATA PART ***\n");
}
/*----------------------------------------------------------------------*/
usage()
/*----------------------------------------------------------------------*/
{
printf ("Usage: databox [-b][-n][-l][-s[a]] [input_file] [output_file]\n");
printf ("       -b: Big Endian, -n: No header, -l: listing\n");
printf ("       -s: Source Out, a: alignment type is long\n");
exit(0);
}
/*----------------------------------------------------------------------*/
xexit ()
/*----------------------------------------------------------------------*/
{
#ifdef	WIN
	if (oufile && ofn[0]) unlink ((char *)ofn);
	if (lstfile)  unlink ((char *)ifn);
#else
	if (oufile && ofn[0]) unlink (ofn);
	if (lstfile)  unlink (ifn);
#endif
	exit(1);
}
/*----------------------------------------------------------------------*/
eexit (mess)
/*----------------------------------------------------------------------*/
BYTE	*mess;
{
	fprintf (stderr,"ERROR: %s\n",mess);
	xexit();
}
/*----------------------------------------------------------------------*/
errexit (mess, err)
/*----------------------------------------------------------------------*/
BYTE	*mess;
BYTE	*err;
{
	BYTE	c[4];

	if (err == NULL) {
	    c[0] = ch; c[1] = 0; c[2] = 0;
	    if (ch == EOF) c[0] = '$';
	    else if (KANJI(ch)) {in_ch(0); c[1] = ch;}
	    err = c;
	}

	fprintf (stderr,"ERROR: %s AT '%s' LINE %d\n",mess, err, lineno);
	xexit();
}
#ifdef	WIN
/*----------------------------------------------------------------------*/
UH	sjistojis (UH sj)           /* SJIS -> JIS (MS-DOS or Win Only) */
/*----------------------------------------------------------------------*/
{
	UB	sc1, sc2;

	sc1 = ((sj >> 8) & 0xff);
	sc2 = ( sj       & 0xff);
	sc1 -= (sc1 < 0xa0) ? 0x71: 0xb1;
	sc1 = sc1 + sc1 + 1;
	if (sc2 > 0x7f) sc2--;
	if (sc2 >= 0x9e) {
		sc1++;
		sc2 -= 0x7d;
	} else {
		sc2 -= 0x1f;
	}
	return (sc1 << 8) | sc2;
}
#endif
