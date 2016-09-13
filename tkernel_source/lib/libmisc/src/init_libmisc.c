/*
 *----------------------------------------------------------------------
 *    T-Kernel Stoftware Library
 *
 *    Copyright(C) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)init_libmisc.c (libmisc) 2016/09/12
 *  libmisc初期化ﾗｲﾌﾞﾗﾘｰ
 */

#include <basic.h>
#include <misc/libmisc.h>



extern int _INIT_LOWLEVEL(void);	////////



/*================================================
;|
;|□init_libmisc        《libmiscライブラリーの初期化》
;|  ［形式］#include <misc/libmisc.h>
;|          int init_libmisc(void);
;|  ［引数］なし
;|  ［戻値］=0:正常終了
;|  ［Note］・
;|
 ================================================*/
int init_libmisc(void)
{
////////////////////////////
	_INIT_LOWLEVEL();				/////////////
////////////////////////////


	return 0;
}


/*----------------------------------------------------------------------
#|History of "init_libmisc.c"
#|===========================
#|* 2016/09/12	新規作成(by T.Yokobayashi)
#|
*/
