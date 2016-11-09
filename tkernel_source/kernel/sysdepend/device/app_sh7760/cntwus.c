/*
 *----------------------------------------------------------------------
 *    T-Kernel
 *
 *    Copyright (C) 2004-2008 by Ken Sakamura. All rights reserved.
 *    T-Kernel is distributed under the T-License.
 *----------------------------------------------------------------------
 *
 *    Version:   1.02.04
 *    Released by T-Engine Forum(http://www.t-engine.org) at 2008/02/29.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/11/09.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cntwus.c (SH7760)
 *	Loop Count Computation of micro Second Wait 
 */

#include <basic.h>
#include <sys/sysinfo.h>
#include <tk/syslib.h>
#include "tkdev_conf.h"

/*
 * Wait loop
 */
LOCAL volatile void WaitLoop( UW count )
{
	Asm("	10:	tst	%0, %0	\n"
	"		bf/s	10b	\n"
	"		add	#-1, %0	"
		: "=r"(count)
		: "0"(count)
	);
}

/*
 * WaitUsec()  Compute loop count of micro second wait
 *	Suppose that it is executed by interrupt disable.
 */
EXPORT void CountWaitUsec( void )
{
	UW	cnt, sc, ec;
	UB	tstr;

	/* Stop TMU0 */
	tstr = in_b(TSTR) & ~TSTR_STR0;
	out_b(TSTR, tstr);

	/* Set TMU0 */
	out_h(TCR0, TCR_TPSC_P64);	/* Select clock/Disable interrupt */
	out_w(TCOR0, 0xffffffff);	/* Counter initial value */
	out_w(TCNT0, 0xffffffff);	/* Counter initial value */

	/* Start TMU0 count */
	out_b(TSTR, tstr | TSTR_STR0);

	/* Measure time for 110000 loops */
	sc = in_w(TCNT0);
	WaitLoop(110000);
	ec = in_w(TCNT0);
	cnt = sc - ec;

	/* Measure time for 10000 loops */
	sc = in_w(TCNT0);
	WaitLoop(10000);
	ec = in_w(TCNT0);

	/* Delete time for operation except for loops and 
	   compute the net time for 100000 loops */
	cnt -= sc - ec;

	/* Stop TMU0 */
	out_b(TSTR, tstr);

	/* Compute number of loops for 64 micro seconds
	 *			100000 loops
	 *	loop64us = ----------------------- * 64usec
	 *		    cnt * (64 dividing / Pck)
	 *	Pck = Peripheral clock (1/100MHz)
	 */
	SCInfo.loop64us = SCInfo.Pck * 1000 / cnt;
}


/*----------------------------------------------------------------------
#|History of "cntwus.c"
#|======================
#|* 2016/11/09	WaitLoop()の_loopﾗﾍﾞﾙがｲﾝﾗｲﾝ展開されてしまい重複ｴﾗｰとなるので、
#|  ﾃﾝﾎﾟﾗﾘﾗﾍﾞﾙに変更した。
#|
*/
