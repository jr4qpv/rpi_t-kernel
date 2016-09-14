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
 *    Modified by T.Yokobayashi at 2015/12/18.
 *
 *----------------------------------------------------------------------
 */

/*
 *	tkdev_timer.h (SH7760)
 *	Hardware-Dependent Timer Processing
 */

#ifndef _TKDEV_TIMER_
#define _TKDEV_TIMER_

#include <tk/syslib.h>
#include <sys/sysinfo.h>
#include "tkdev_conf.h"

#define	TKernel_Ver		2				/* Ver1=1, Ver2=2 */


#if (TKernel_Ver == 2)					/* T-Kernel Ver2用 */
/*
 * Settable interval range (microsecond)
 */
#define MIN_TIMER_PERIOD	10
#define MAX_TIMER_PERIOD	50000
#else
/*
 * Settable interval range (millisecond)
 */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50
#endif


/*
 * Timer control register setting value
 *	Interrupt enable, 16 dividing
 */
#define TCR0_INIT	( TCR_UNIE | TCR_TPSC_P16 )

/*
 * Set timer
 */
Inline void init_hw_timer( void )
{
	UB	tstr;
	UW	cnt;
#if (TKernel_Ver == 2)					/* T-Kernel Ver2用 */
	UD	dtmp;
#endif

	/* Stop timer */
	tstr = in_b(TSTR) & ~TSTR_STR0;
	out_b(TSTR, tstr);

	/* Set timer mode */
	out_h(TCR0, TCR0_INIT);

	/* Set counter */
#if (TKernel_Ver == 2)					/* T-Kernel Ver2用 */
	dtmp = TIMER_PERIOD * (UW)SCInfo.Pck;
	cnt = (dtmp / (16 * 100)) - 1;
#else
	cnt = TIMER_PERIOD * (UW)SCInfo.Pck * 10 / 16 - 1;
#endif
	out_w(TCOR0, cnt);
	out_w(TCNT0, cnt);

	/* Start timer count */
	out_b(TSTR, tstr | TSTR_STR0);
}

/*
 * Timer start processing
 *	Initialize the timer and start the periodical timer interrupt.
 */
Inline void start_hw_timer( void )
{
IMPORT	void	timer_handler_startup( void );
	UW	ipri;

	/* Set timer */
	init_hw_timer();

	/* Interrupt handler definition */
	define_inthdr(VECNO_TMU0, timer_handler_startup);

	/* Timer interrupt enable */
	ipri = in_h(IPRA);
	ipri = (ipri & 0x0fff) | (TIMER_INTLEVEL << 12);
	out_h(IPRA, ipri);
}

/*
 * Clear timer interrupt
 *	Clear the timer interrupt request. Depending on the type of
 *	hardware, there are two timings for clearing: at the beginning
 *	and the end of the interrupt handler.
 *	'clear_hw_timer_interrupt()' is called at the beginning of the
 *	timer interrupt handler.
 *	'end_of_hw_timer_interrupt()' is called at the end of the timer
 *	interrupt handler.
 *	Use either or both depending on hardware.
 */
Inline void clear_hw_timer_interrupt( void )
{
	/* Clear underflow flag */
	out_h(TCR0, TCR0_INIT);
}
Inline void end_of_hw_timer_interrupt( void )
{
	/* Nothing required to do at this point */
}

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
Inline void terminate_hw_timer( void )
{
	UW	ipri;

	/* Timer interrupt disable */
	ipri = in_h(IPRA);
	ipri = ipri & 0x0fff;
	out_h(IPRA, ipri);
}

/*
 * Get processing time from the previous timer interrupt to the
 * current (nanosecond)
 *	Consider the possibility that the timer interrupt occurred
 *	during the interrupt disable and calculate the processing time
 *	within the following
 *	range: 0 <= Processing time < TIMER_PERIOD * 2
 */
Inline UINT get_hw_timer_nsec( void )
{
	UW	ofs, max;
	UH	unf;
#if (TKernel_Ver == 2)					/* T-Kernel Ver2用 */
	UD	dtmp;
#endif

	max = in_w(TCOR0) + 1;
	do {
		unf = in_h(TCR0) & TCR_UNF;
		ofs = max - in_w(TCNT0);
	} while ( unf != (in_h(TCR0) & TCR_UNF) );
	if ( unf != 0 ) {
		ofs += max;
	}

#if (TKernel_Ver == 2)					/* T-Kernel Ver2用 */
	dtmp = ofs * (100 * 16 * 1000);
	return dtmp / SCInfo.Pck;
#else
	return ofs * (100 * 16) / SCInfo.Pck * 1000;
#endif
}


#if (TKernel_Ver == 2)					/* T-Kernel Ver2用 */
/*
 * Similar to the function as above, but returns value in microseconds.
 */
Inline UINT get_hw_timer_usec( void )
{
	UW	ofs, max;
	UH	unf;
	UD	dtmp;

	max = in_w(TCOR0) + 1;
	do {
		unf = in_h(TCR0) & TCR_UNF;
		ofs = max - in_w(TCNT0);
	} while ( unf != (in_h(TCR0) & TCR_UNF) );
	if ( unf != 0 ) {
		ofs += max;
	}

	dtmp = ofs * (100 * 16 * 1);
	return dtmp / SCInfo.Pck;
}
#endif

#endif /* _TKDEV_TIMER_ */


/*----------------------------------------------------------------------*/
#if 0
#|【tkdev_timer.h 変更履歴】
#|□2015/12/18	[app_sh7760]用に、{V1.02.04}kernel/sysdepend/device/std_sh7760/
#|    から"tkdev_timer.h"をコピーし、get_hw_timer_usec()を追加。
#|□2015/12/19	../tef_em1d/tkdev_timer.hを参考に、MAX_TIMER_PERIOD=50→50000,
#|  MIN_TIMER_PERIOD=1→10に変更。(millisecond→microsecond)
#|□2015/12/22	init_hw_timer()の、ﾀｲﾏｶｳﾝﾄ値の計算で、TIMER_PERIODの単位が、
#|  {V2}では、millisecond→microsecondに変更なってるのに対応。
#|□2015/12/22	get_hw_timer_nsec(),get_hw_timer_usec()の戻値計算に、
#|  明示的にUD(long long)を使う記述にした。(TKernel_Ver==2の箇所)
#|
#endif
