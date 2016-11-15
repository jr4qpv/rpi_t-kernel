/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Stoftware Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)atohex.c (libmisc) 2016/02/11
 *  atohex関係ﾗｲﾌﾞﾗﾘｰ
 */

#include <basic.h>
#include <misc/libmisc.h>


/*
        extract hexadecimal value
*/
static	uint32_t getHexVal(const uint8_t *p)
{
	int c;
	uint32_t v;

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
	return (char)getHexVal((uint8_t *)s);
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
	return (short)getHexVal((uint8_t *)s);
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
	return (long)getHexVal((uint8_t *)s);
}


/*----------------------------------------------------------------------
#|History of "atohex.c"
#|=====================
#|* 2016/02/11	New created.(By T.Yokobayashi)
#|  "sh_std/lib/atohex.c"を参考に移植
#|
*/
