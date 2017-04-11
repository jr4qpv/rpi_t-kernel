/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)init_libmisc.c (libmisc) 2016/09/12
 *  libmisc初期化ﾗｲﾌﾞﾗﾘｰ
 */

#include <basic.h>
#include <tm/tmonitor.h>
#include <misc/libmisc.h>

#if 0	//////////
extern int _INIT_LOWLEVEL(void);
#endif	/////////


/*** 共通変数 ***/
int _libmisc_std_portno;




/*================================================
;|
;|□init_libmisc        《libmiscライブラリーの初期化》
;|  ［形式］#include <misc/libmisc.h>
;|          int init_libmisc(void);
;|  ［引数］なし
;|  ［戻値］=0:正常終了
;|  ［Note］・libmiscライブラリーを利用する前に、本ルーチンをコールしておく。
;|
 ================================================*/
int init_libmisc(void)
{
	/* get T-Monitor port number */
	_libmisc_std_portno = tm_extsvc(TMEF_PORTNO, 0, 0, 0);


#if 0	////////////////////////////
	_INIT_LOWLEVEL();				/////////////
#endif	////////////////////////////


	return 0;
}


/*----------------------------------------------------------------------
#|History of "init_libmisc.c"
#|===========================
#|* 2016/09/12	New created.(By T.Yokobayashi)
#|
*/
