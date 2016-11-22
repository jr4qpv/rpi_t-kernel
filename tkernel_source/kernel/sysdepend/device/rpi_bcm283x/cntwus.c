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
 *    Modified by T.Yokobayashi at 2016/07/21.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)cntwus.c (kernel/device) 2016/07/21
 *
 *	Calculate the value of required count for loop-wait for a given period (in micro seconds)
 */

#include <basic.h>
#include <sys/sysinfo.h>
#include <tk/syslib.h>
#include "tkdev_conf.h"

#include <rpi/bcm283x.h>

/*
 * Loop wait
 *	disable the inline expansion caused by compiler optimization to obtain accurate
 *      measurement.
 */
__attribute__ ((noinline))
LOCAL void WaitLoop( UW count )
{
	Asm("	_loop:	subs	%0, %0, #1	\n"
	"		bhi	_loop		"
		: "=r"(count)
		: "0"(count + 1)
	);
}

/*
 * WaitUsec()  calculate the loop count for a given time (in microseconds)
 *	interrupt-disabled state is assumed.
 */
EXPORT void CountWaitUsec( void )
{
	UW	t0, t1, t2;
	UW	cnt;
	UW	d;

	WaitLoop(100);		/* wait for a little (just in case) */

	/* measurement */
	t0 = in_w(SYST_CLO);
	WaitLoop(1001);
	t1 = in_w(SYST_CLO);
	WaitLoop(21001);
	t2 = in_w(SYST_CLO);

	/* the time for 20000 loops is calculated excluding the
           overhead for the rest of measurement time. */
	cnt = (t2 - t1) - (t1 - t0);

	/* Calculate the loop count that spends 64 microseconds
	 *                  20000 loops
	 *  loop64us = ---------------------- * 64usec
	 *              cnt * (1 / TIN_CLK)
	 *  TIN_CLK = input clock to the timer [Hz]
	 */
	d = SYST_CLOCK;							// カウント周波数[Hz]
	SCInfo.loop64us = d / (cnt * (50/2)) * (64/2);
}


/*----------------------------------------------------------------------
#|History of "cntwus.c"
#|=======================
#|* 2016/07/21	[rpi_bcc283x]用に、[tef_em1d]用の"cntwus.c"を参考に作成。
#|
*/
