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
 *    Modified by T.Yokobayashi at 2015/12/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)waitusec.c () 2015/12/10
 *
 *       SH7760: micro wait
 */

/*
 *[Note]
 *・CPUの命令ループにて待ちを行う。
 *  待ちループ回数は、初期化時にハードタイマ(TMU#0)で実測し最適値を求める。
 */
 
#include "sysdepend.h"
#include <misc/renesas/sh7760.h>

#define	TMU_CLK	(Pck_CLK * 10)			/* [1/1000 MHz] */


#if 0									// SCInfoの領域はKernelが利用してる //
#define	delay64us	(SCInfo.loop64us)	/* UW:wait for 64 microsec */
#else
LOCAL	UW	delay64us;		 			// wait for 64 microsec
#endif


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
 * （msec=0の時は、WDTﾄﾘｶﾞを実行するのみ)
 */
EXPORT	void	waitMsec(UW msec)
{
	WDT_dog_food();						// WDTトリガ

	while (msec-- > 0) {
		waitUsec(1000);					// 1m秒待ち
		WDT_dog_food();					// WDTトリガ
	}

	return;
}

/* ------------------------------------------------------------------------ */

/*
 * setting up the initial count for micro-wait()
 */
EXPORT	void	setupWaitUsec(void)
{
	UW	t0, t1, t2;

#define	MAX_CNT		(Ick_CLK * 64 / 1)		// 1 Clockで命令実行時のﾙｰﾌﾟ数
#define	MIN_CNT		(Ick_CLK * 64 / 128)	// 128 Clockで命令実行時のﾙｰﾌﾟ数

	/* use TMU#0 timer, and assume clock is Pck/4 */
	TMU.TSTR.BIT.STR0 = 0;				// TMU#0 Timer stop
	TMU0.TCR.WORD = 0x0000;				// 割込しない, 立上りｴｯｼﾞ, Pck/4
	TMU0.TCOR = 0xffffffff;				// maximun count
	TMU0.TCNT = 0xffffffff;				// count clear

	TMU.TSTR.BIT.STR0 = 1;				// TMU#0 Timer start (down counter)

	delay64us = 64;
	waitUsec(1000);						// wait for a while until things settle down
										// (安定待ち)
	t0 = TMU0.TCNT;						// get Timer count
	waitUsec(1000);
	t1 = TMU0.TCNT;						// get Timer count
	waitUsec(3000);
	t2 = TMU0.TCNT;						// get Timer count

	TMU.TSTR.BIT.STR0 = 0;				// TMU#0 Timer stop
	
	t2 = t1 - t2;						// count for 3000 times
	t1 = t0 - t1;						// count for 1000 times
	t2 -= t1;							// count for 2000 times

	/*
	 * calculate the count for 64 microsec
	 *
	 *                    2000 loops x timer clock [MHz] x 64 [microsec]
	 *	delay64us = ------------------------------------------------
	 *	                                 t2
	 *
	 * considering the representation of TMU_CLK (1/1000MHz unit), and setting of pre scaler,
	 * it can be written down as follows.
	 *
	 *                    2 loops x TMU_CLK [1/1000MHz] x (64/4) [microsec]
	 *	delay64us = -------------------------------------------
	 *	                                 t2
	 *
	 */
	delay64us = (t2 == 0) ? MAX_CNT : ((2 * TMU_CLK * 16) / t2);
	if (delay64us > MAX_CNT) delay64us = MAX_CNT;
	else if (delay64us < MIN_CNT) delay64us = MIN_CNT;

	return;
}


/*----------------------------------------------------------------------*/
#if 0
#|【waitusec.c 変更履歴】
#|□2015/12/02	[app_sh7760]用に、[tef_em1d]用の"waitusec.c"を参考に作成。
#|
#endif
