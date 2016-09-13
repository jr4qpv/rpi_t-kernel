/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *  @(#)conio.c (libmisc) 2016/02/19
 *  ｺﾝｿｰﾙI/O ﾙｰﾁﾝ
 */

#include <basic.h>
#include <t2ex/errno.h>					/* for t2ex/string.h */
#include <t2ex/string.h>				/* for strlen() */
#include <sys/consio.h>
#include <misc/libmisc.h>


//#define	USE_ERRIO_DEBUG		1			/* =1:serialﾄﾞﾗｲﾊﾞを使う(ﾃﾞﾊﾞｯｸ用) */


#ifndef USE_ERRIO_DEBUG
#define	PORT_NO				1			/* 標準ｺﾝｿｰﾙ */
#endif



/*================================================
;|
;|□putch               《標準コンソールに文字を出力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int putch(int c);
;|  ［引数］c…表示文字
;|  ［戻値］成功すると出力された文字cを返し、エラーの場合はEOFを返す。
;|  ［Note］・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの｢console_out｣をｺｰﾙしている。
;|          ・'\n'(LF)のｺｰﾄﾞはCR/LFへ変換しない
;|          ・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの仕様は、ﾊﾟｰｿﾅﾙﾒﾃﾞｨｱ社のSH7760開発ｷｯﾄに含まれる
;|            ｢T-Engine開発キットデバイスドライバ共通説明書｣が参考になる。
;|
 ================================================*/
int putch(int c)
{
#ifndef USE_ERRIO_DEBUG
	B	buf[1];

	buf[0] = c;							// 出力文字

	if (console_out(PORT_NO, buf, 1) != 1)
		return EOF;						// エラー

	return (UB)c;						// 正常
#else
	return eputchar(c);
#endif
}


/*================================================
;|
;|□cputchar            《標準コンソールに文字を出力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int cputchar(int c);
;|  ［引数］c…表示文字
;|  ［戻値］成功すると出力された文字cを返し、エラーの場合はEOFを返す。
;|  ［Note］・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの｢console_out｣をｺｰﾙしている。
;|          ・'\n'(LF)のｺｰﾄﾞはCR/LFへ変換する。
;|          ・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの仕様は、ﾊﾟｰｿﾅﾙﾒﾃﾞｨｱ社のSH7760開発ｷｯﾄに含まれる
;|            ｢T-Engine開発キットデバイスドライバ共通説明書｣が参考になる。
;|
 ================================================*/
int cputchar(int c)
{
#ifndef USE_ERRIO_DEBUG
	B	buf[1];

	buf[0] = c;							// 出力文字

	if (console_out(PORT_NO, buf, 1) != 1)
		return EOF;						// エラー

	return (UB)c;						// 正常
#else
	return eputchar(c);
#endif
}


/*================================================
;|
;|□cputstring          《 標準出力に文字列を出力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int cputstring(const char *s);
;|  ［引数］str…表示文字列
;|  ［戻値］成功すると｢0｣を返し、エラーの場合はEOFを返す。
;|  ［Note］・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの｢console_out｣をｺｰﾙしている。
;|          ・'\n'(LF)のｺｰﾄﾞはCR/LFへ変換する。
;|
 ================================================*/
int cputstring(const char *s)
{
#ifndef USE_ERRIO_DEBUG
	UW len;

	len = strlen(s);					// 文字数
	if (console_out(PORT_NO, (B *)s, len) != 1)
		return EOF;						// エラー

	return 0;							// 正常
#else
	return eputstring(s);
#endif
}


/*================================================
;|
;|□getch               《標準コンソールからｴｺｰﾊﾞｯｸなしで文字を入力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int getch(void);
;|  ［引数］なし
;|  ［戻値］入力した文字
;|  ［Note］・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの｢console_in｣をｺｰﾙしている。
;|          ・'\r'(CR)のｺｰﾄﾞは'\n'(LF)へ変換しない。
;|          ・不正時はEOF(-1)を戻す。
;|          ・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの仕様は、ﾊﾟｰｿﾅﾙﾒﾃﾞｨｱ社のSH7760開発ｷｯﾄに含まれる
;|            ｢T-Engine開発キットデバイスドライバ共通説明書｣が参考になる。
;|
 ================================================*/
int getch(void)
{
#ifndef USE_ERRIO_DEBUG
	B	buf[2];							// 1Byte余裕に確保
	int	c;
	W	echo_mode;

	/* 現在のモードの獲得 */
	echo_mode = console_ctl(PORT_NO, ECHO | GETCTL, 0);

	console_ctl(PORT_NO, ECHO, 0);		// エコーオフ

	if (console_in(PORT_NO, buf, 1) == 1)	// 1文字入力
		c = (UB)buf[0];					// 入力文字
	else
		c = EOF;						// 不正

	/* モードの復旧 */
	console_ctl(PORT_NO, ECHO , echo_mode);

	return c;
#else
	return _getSIO(-1);
#endif
}


/*================================================
;|
;|□cgetchar            《標準コンソールからｴｺｰﾊﾞｯｸなしで文字を入力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int cgetchar(void);
;|  ［引数］なし
;|  ［戻値］入力した文字
;|  ［Note］・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの｢console_in｣をｺｰﾙしている。
;|          ・'\r'(CR)のｺｰﾄﾞは'\n'(LF)へ変換する。
;|          ・不正時はEOF(-1)を戻す。
;|          ・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの仕様は、ﾊﾟｰｿﾅﾙﾒﾃﾞｨｱ社のSH7760開発ｷｯﾄに含まれる
;|            ｢T-Engine開発キットデバイスドライバ共通説明書｣が参考になる。
;|
 ================================================*/
int cgetchar(void)
{
#ifndef USE_ERRIO_DEBUG
	B	buf[2];							// 1Byte余裕に確保
	int c;

	if (console_in(PORT_NO, buf, 1) == 1)	// 1文字入力
		c = (UB)buf[0];					// 入力文字
	else
		c = EOF;						// 不正

	return c;
#else
	return egetchar();
#endif
}


/*================================================
;|
;|□kbhit               《標準入力が押されたかどうかをﾁｪｯｸする》
;|  ［形式］#include <misc/libmisc.h>
;|          int kbhit(void);
;|  ［引数］なし
;|  ［戻値］=0:ｷｰ入力なし, ≠0:ﾊﾞｯﾌｧｰに溜まってるﾃﾞｰﾀ数
;|  ［Note］・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの｢console_in｣をｺｰﾙしている。
;|
 ================================================*/
int kbhit(void)
{
#ifndef USE_ERRIO_DEBUG
	return console_in(PORT_NO, NULL, 0);	// 入力可能ﾊﾞｲﾄ数の獲得
#else
	return ekbhit();
#endif
}


/*================================================
;|
;|□kbclr               《標準入力ﾊﾞｯﾌｧｰをｸﾘｱする》
;|  ［形式］#include <misc/libmisc.h>
;|          void kbclr(void);
;|  ［引数］なし
;|  ［戻値］なし
;|  ［Note］・入力ﾊﾞｯﾌｧｰをｸﾘｱする
;|
 ================================================*/
void kbclr(void)
{
#ifndef USE_ERRIO_DEBUG
	int n;

	n = kbhit();
	for ( ; n>0; n--)
		cgetchar();
#else
	return ekbclr();
#endif
}


/*================================================
;|
;|□cgetstring          《標準入力から文字列を入力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int cgetstring(char *s, int n);
;|  ［引数］s  …入力ﾊﾞｯﾌｧｰへのﾎﾟｲﾝﾀ
;|          n  …長さ(2以上を指定すること)
;|  ［戻値］入力文字数('\0'含む), 中断時(^C入力)は-1を返す。
;|  ［Note］・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞの｢console_in｣をｺｰﾙしている。
;|          ・改行文字CR('\r')又はLF('\n')が入力されるまで読み込み、
;|            sに格納し、エコーバックする。
;|          ・改行文字CR('\r')又はLF('\n')は、s内ではヌル文字('\0')に
;|            置き換え、エコーバックもしない。
;|          ・ｺﾝｿｰﾙﾄﾞﾗｲﾊﾞのEDITﾓｰﾄﾞで1行編集入力を行う。
;|
 ================================================*/
int cgetstring(char *s, int n)
{
#ifndef USE_ERRIO_DEBUG
	int rc;

	rc = console_in(PORT_NO, s, n);
	return rc;
#else
	return egetstring(s, n);
#endif
}


/*================================================
;|
;|□cgets               《標準入力から文字列を入力する》
;|  ［形式］#include <misc/libmisc.h>
;|          char *cgets(char *buf);
;|  ［引数］buf…入力ﾊﾞｯﾌｧｰへのﾎﾟｲﾝﾀ
;|  ［戻値］buf[2]へのﾎﾟｲﾝﾀを返す
;|  ［Note］・ﾓﾆﾀｺｰﾙの｢CON_STR_IN｣をｺｰﾙしている。
;|          ・本関数をｺｰﾙする前に､buf[0]には読み込むべき文字列の最大長を
;|            設定しておく(ﾀｰﾐﾈﾀのNULLを含む文字数)。
;|          ・本関数の実行が終わるとbuf[1]には実際に読み込まれた文字数が
;|            設定される。読み込まれた文字はbuf[2]からはじまりNULLで終わる。
;|            従って、bufは最低でも3ﾊﾞｲﾄ以上は確保しておく事。
;|
 ================================================*/
char *cgets(char *buf)
{
	int n, len;

	n = (UB)buf[0];							// 入力文字数
	if ((len = cgetstring(&buf[2], n)) < 0)
		len = 0;
	buf[1] = len;

	return &buf[2];
}


/*----------------------------------------------------------------------
#|History of "conio.c"
#|=====================
#|* 2016/02/10	新規作成(by T.Yokobayashi)
#|  "sh_std/lib/conio.c"を参考に移植
#|
*/
