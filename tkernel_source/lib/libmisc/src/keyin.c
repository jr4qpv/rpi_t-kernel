/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)keyin.c (libmisc) 2016/02/19
 *  KEY入力操作関係ﾗｲﾌﾞﾗﾘｰ
 */

#include <basic.h>
#include <misc/libmisc.h>


/*================================================
;|
;|□kbhit_esc           《ESCキーが押されたかチェックする》
;|  ［形式］#include <misc/libmisc.h>
;|          int kbhit_esc(void);
;|  ［引数］なし
;|  ［戻値］=1:押された, =0:押されてない
;|  ［Note］・
;|
 ================================================*/
int kbhit_esc(void)
{
	if (kbhit()) {
		if (getch() == ESC_CODE)
			return 1;						/* ESCが押された */
	}
	return 0;								/* ESCが押されてない */
}


/*================================================
;|
;|□kbhit_spc           《スペースキーが押されたかチェックする》
;|  ［形式］#include <misc/libmisc.h>
;|          int kbhit_spc(void);
;|  ［引数］なし
;|  ［戻値］=1:押された, =0:押されてない
;|  ［Note］・
;|
 ================================================*/
int kbhit_spc(void)
{
	if (kbhit()) {
		if (getch() == ' ')
			return 1;						/* SPCが押された */
	}
	return 0;								/* SPCが押されてない */
}



#if 0	//////////////////////////////////////////////////
/*================================================
;|
;|□getche_yn　　　　　 《'Y'又は'N'の文字の１文字入力》
;|　［形式］#include <sh_std.h>
;|　　　　　char getche_yn(char c);
;|　［引数］c…デフォルト入力文字('Y','N')
;|　［戻値］'Y','N','y','n'の文字を大文字に変換して戻します
;|　［Note］・CR入力の時はデフォルト入力文字 c で指定された文字が戻される。
;|　　　　　・'Y','N'以外の文字は無視し、'Y','N'の文字はｴｺｰﾊﾞｯｸされる。
;|
 ================================================*/
char getche_yn(char c)
{
	char key;

	c = toupper(c);
	if ((c == 'Y') || (c == 'N')) {
		putch(c);
		disp_left(1);
	}

	while (1) {
		key = getch();	/* toupperはﾏｸﾛで2回評価されるので変数へ代入が必要 */

		switch (toupper(key)) {
		case 'Y':
			putch('Y');
			return 'Y';

		case 'N':
		case ESC_CODE:
			putch('N');
			return 'N';

		case CR_CODE:
			if ((c == 'Y') || (c == 'N')) {
			    putch(c);
			    return c;
			}
		}
	}
}


/*================================================
;|
;|□nextpage_yn　　　　 《｢Next Page｣の問い合わせをする》
;|　［形式］#include <sh_std.h>
;|　　　　　char nextpage_yn(char c);
;|　［引数］c…デフォルト入力文字('Y','N')
;|　［戻値］'Y','N','y','n'の文字を大文字に変換して戻します
;|　［Note］・"Next page ?(Y/N) : "を表示し、'Y'/'N'の文字入力をする。
;|　　　　　・CR入力の時はデフォルト入力文字 c で指定された文字が戻される。
;|　　　　　・'Y','N'以外の文字は無視し、'Y','N'の文字はｴｺｰﾊﾞｯｸされる。
;|
 ================================================*/
char nextpage_yn(char c)
{
	cputs("Next page ?(Y/N) : ");
	c = getche_yn(c);
	disp_nl(1);

	return c;
}
#endif	////////////////////////////


/*----------------------------------------------------------------------
#|History of "atohex.c"
#|=====================
#|* 2016/02/11	New created.(By T.Yokobayashi)
#|  "sh_std/lib/keyin.c"を流用
#|
*/
