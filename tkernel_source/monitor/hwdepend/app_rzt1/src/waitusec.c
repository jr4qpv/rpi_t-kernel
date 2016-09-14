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
 *    Modified by T.Yokobayashi at 2016/04/08.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)waitusec.c () 2016/04/22
 *
 *       RZT1: micro wait
 */

#include "sysdepend.h"
#include <misc/renesas/rzt1.h>

LOCAL	UW	delay64us;		 // wait for 64 microsec

/*
 * wait for nanoseconds
 */
EXPORT	void	waitNsec(_UW nsec)
{
	for (nsec = nsec * delay64us / 64000; nsec > 0; nsec--);

	return;
}

/*
 * wait for microseconds
 */
EXPORT	void	waitUsec(_UW usec)
{
	for (usec = usec * delay64us / 64; usec > 0; usec--);

	return;
}

/*
 * wait for milliseconds
 */
EXPORT	void	waitMsec(UW msec)
{
	while (msec-- > 0) waitUsec(1000);

	return;
}

/* ------------------------------------------------------------------------ */

/*
 * setting up the initial count for micro-wait()
 */
EXPORT	void	setupWaitUsec(void)
{
	UW	t0, t1, t2;

#define	MAX_CNT		((CPUCLK_CLK/1000) * 64 / 1)	// 1 Clockで命令実行時のﾙｰﾌﾟ数
#define	MIN_CNT		((CPUCLK_CLK/1000) * 64 / 256)	// 256 Clockで命令実行時のﾙｰﾌﾟ数

	/* use CMTW#0 Timer, and assume clock is PCLKD(75MHz) */
	CMTW0.CMWSTR.BIT.STR = 0;				// カウント停止
	CMTW0.CMWCR.WORD = 0x2000;				// ｸﾘｱしない,32bit,PCLKD/8
	CMTW0.CMWIOR.WORD = 0x0000;
	CMTW0.CMWCNT = 0;						// カウンタクリア
	CMTW0.CMWCOR = 0xffffffff;
	CMTW0.CMWSTR.BIT.STR = 1;				// カウント動作開始

	delay64us = 64;
	waitUsec(1000);				// wait for a while until things settle down

	t0 = CMTW0.CMWCNT;
	waitUsec(1000);
	t1 = CMTW0.CMWCNT;
	waitUsec(3000);
	t2 = CMTW0.CMWCNT;

	CMTW0.CMWSTR.BIT.STR = 0;				// カウント停止

	t2 -= t1;	// count for 3000 times
	t1 -= t0;	// count for 1000 times
	t2 -= t1;	// count for 2000 times

	/*
	 * calculate the count for 64 microsec
	 *
	 *              2000 loops x timer clock [MHz] x 64 [microsec]
	 *	delay64us = ------------------------------------------------
	 *	                                 t2
	 *
	 * considering the representation of SYST_CLK (1/1000MHz unit), and setting of pre scaler,
	 * it can be written down as follows.
	 *
	 *              2 loops x PCLKD_CLK/8 [1/1000MHz] x 64 [microsec]
	 *	delay64us = ---------------------------------------------------
	 *	                                 t2
	 *
	 */
	delay64us = (t2 == 0) ? MAX_CNT : ((2 * PCLKD_CLK/8 * 64) / t2);
	if (delay64us > MAX_CNT) delay64us = MAX_CNT;
	else if (delay64us < MIN_CNT) delay64us = MIN_CNT;

	return;
}


/*----------------------------------------------------------------------
#|History of "waitusec.c"
#|=======================
#|* 2016/04/08	[app_rzt1]用に、[tef_em1d]用の"waitusec.c"を参考に作成。
#|
*/
