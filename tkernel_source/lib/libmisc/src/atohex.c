/*
 *----------------------------------------------------------------------
 *    T-Kernel Stoftware Library
 *
 *    Copyright(C) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)atohex.c (libmisc) 2016/02/11
 *  atohex関係ﾗｲﾌﾞﾗﾘｰ
 */

#include <basic.h>
#include <misc/libmisc.h>

typedef	unsigned char	uchar;
typedef	unsigned long	ulong;



/*
        extract hexadecimal value
*/
static	ulong getHexVal(const uchar *p)
{
	int c;
	ulong v;

	for (v = 0; ((c = *p) >= '0' && c <= '9') ||
		(c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); p++) {
		if (v >= 0x10000000) break;	// overflow
		v <<= 4;
		v += (c >= 'a' ? (c - 'a' + 10) :
			     (c >= 'A' ? (c - 'A' + 10) : ( c - '0')));
	}
	return v;
}


/*================================================
;|
;|□atochex             《16進数文字列を8ﾋﾞｯﾄ数値(char)に変換する》
;|  ［形式］#include <misc/libmisc.h>
;|          char atochex(const char *s);
;|  ［引数］s  …入力文字列
;|  ［戻値］変換後の値
;|  ［Note］・不正な入力文字を検出したところで変換を中断する。
;|            最初の文字が不正の場合は｢0｣を戻す。
;|
 ================================================*/
char atochex(const char *s)
{
	return (char)getHexVal(s);
}


/*================================================
;|
;|□atoshex             《16進数文字列を16ﾋﾞｯﾄ数値(short)に変換する》
;|  ［形式］#include <misc/libmisc.h>
;|          short atoshex(const char *s);
;|  ［引数］s  …入力文字列
;|  ［戻値］変換後の値
;|  ［Note］・不正な入力文字を検出したところで変換を中断する。
;|            最初の文字が不正の場合は｢0｣を戻す。
;|
 ================================================*/
short atoshex(const char *s)
{
	return (short)getHexVal(s);
}


/*================================================
;|
;|□atolhex             《16進数文字列を32ﾋﾞｯﾄ数値(long)に変換する》
;|  ［形式］#include <misc/libmisc.h>
;|          long atolhex(const char *s);
;|  ［引数］s  …入力文字列
;|  ［戻値］変換後の値
;|  ［Note］・不正な入力文字を検出したところで変換を中断する。
;|            最初の文字が不正の場合は｢0｣を戻す。
;|
 ================================================*/
long atolhex(const char *s)
{
	return (long)getHexVal(s);
}



/*----------------------------------------------------------------------
#|History of "atohex.c"
#|=====================
#|* 2016/02/11	新規作成(by T.Yokobayashi)
#|  "sh_std/lib/atohex.c"を参考に移植
#|
*/
