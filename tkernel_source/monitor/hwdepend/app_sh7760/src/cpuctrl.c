/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2015/12/12.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cpuctrl.c () 2016/01/26
 *
 *       SH7760(SH-4) CPU control
 */

#include "sysdepend.h"
#include <tk/syslib.h>			/* in_w()参照 */
#include <tk/sysdef.h>			/* CCR関係参照 */

#if 0	////////////////////////////
/*
 * Location of the 1st level page table
 */
EXPORT UW* const	TopPageTable = (UW*)PAGETBL_BASE;
#endif	////////////////////////////


#if 0	////// cpuctrl2.Sへ移動 /////////
/* ------------------------------------------------------------------------ */
/*
 *       cache control
 *       acts on the whole address space.
 */

/*
 * turn on cache
 */
EXPORT void EnableCache( void )
{
	setCacheMMU(ENB_CACHEMMU);
}

/*
 * turn off cache
 */
EXPORT void DisableCache( void )
{
	// MMU can NOT be turned off with this CPU.
	setCacheMMU(DIS_CACHEONLY);
}
#endif	///////////////////////////////////////////////


/* ------------------------------------------------------------------------ */
/*
 *      processing on monitor entry
 */

/*
 * entry
 *       info, return value is meaningless
 */
EXPORT W enterMonitor( UW info )
{
	/* cache and MMU is flushed */
	FlushCache();							/* キャッシュをフラッシュ */

///	setCacheMMU(ENB_CACHEMMU);
	return 0;
}

/*
 * exit
 *       info is the cache and MMU mode
 *       return value is meaningless
 */
EXPORT W leaveMonitor( UW info )
{
  #define	CCR_ENABLE	(CCR_ICE|CCR_OCE)

	/* restore cache && MMU to the original state. */
	if ((in_w(CCR) ^ info) & CCR_ENABLE) {	/* 現在キャッシュ状態から変更? */
		if (info & CCR_ENABLE)
			EnableCache();					/* キャッシュ有効 */
		else
			DisableCache();					/* キャッシュ無効 */
	}

///	setCacheMMU(info);
	return 0;
}


/* ------------------------------------------------------------------------ */
#if 0
#|【cpuctrl.c 変更履歴】
#|□2015/12/12	[app_sh7760]用に、新規作成。
#|  [tef_em1d]用の"cpuctrl.c"を参考に作成。
#|□2016/01/26	EnableCache(),DisableCache()関数を削除(cpuctrl2.Sへ移動)
#|
#endif
