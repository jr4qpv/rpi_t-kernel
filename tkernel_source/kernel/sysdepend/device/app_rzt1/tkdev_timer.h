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
 *    Modified by T.Yokobayashi at 2016/06/08.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)tkdev_timer.h (RZT1) 2016/07/14
 *	Hardware-Dependent Timer Processing
 */

#ifndef _TKDEV_TIMER_
#define _TKDEV_TIMER_

#include <tk/syslib.h>
#include <sys/sysinfo.h>
#include "tkdev_conf.h"

#include <misc/renesas/rzt1.h>

/*
 * Range of settable period  (microseconds)
 */
#define	MIN_TIMER_PERIOD	10
#define	MAX_TIMER_PERIOD	27000

/*
 * Setting up timer
 */
Inline void init_hw_timer( void )
{
	UH	n;
	UW	imask;
	UD	d;

	DI(imask);

	/* Disable interrupt */
	DisableInt(VECNO_TIMER);				// CMI0 割り込みイネーブルクリア

	/* stop timer */
    CMT.CMSTR0.BIT.STR0 = 0U;

	/* choose clock */
    CMT0.CMCR.WORD = 0x0041;				// CKS PCLKD/32, CMIE=1
	CMT0.CMCNT = 0;

	/* set counter */
	d = TIMER_PERIOD * PCLKD_CLOCK;			// TIMER_PERIOD[μsec]
	n = (d / (32 * 1000000)) - 1;

	CMT0.CMCOR = n;

	/* start timer count */
    CMT.CMSTR0.BIT.STR0 = 1U;


#if 0
    /* Cancel CMT stop state in LPC */
    MSTP(CMT0) = 0U;
#endif



	EI(imask);
}


/*
 * Timer start processing
 *	Initialize timer, and start periodic timer interrupt.
 */
Inline void start_hw_timer( void )
{
IMPORT	void	timer_handler_startup( void );
//IMPORT	void	int21_interrupt_entry( void );		/////////

	/* set up timer */
	init_hw_timer();

	/* define interrupt handler */
//	VIC.VAD21.LONG = (unsigned long)int21_interrupt_entry;	////////
	VIC.VAD21.LONG = (unsigned long)timer_handler_startup;
	define_inthdr(VECNO_TIMER, timer_handler_startup);

	/* enable timer interrupt */
	SetIntMode(VECNO_TIMER, IM_EDGE);		// エッジ割り込みモード
	ClearInt(VECNO_TIMER);					// エッジ検出フラグクリア
	EnableInt(VECNO_TIMER, TIMER_INTLEVEL);	// 割り込み許可
}

/*
 * Clear timer interrupt
 *	clear timer interrupt. Depending on hardware, we have to clear
 *	the request of the timer interrupt at the beginning of the
 *      timer handler, or clear it at the end.
 *	clear_hw_timer_interrupt() is called at the beginning of the
 *	timer interrupt handler.
 *	end_of_hw_timer_interrupt() is called at the end.
 *	Either one of them, or both are used according hardware requirements.
 */
Inline void clear_hw_timer_interrupt( void )
{
#if 1

#if	TIMER_INTLIB_USE
	ClearInt(VECNO_TIMER);			// CMI0 edge interrupt flag clear
#else
	VIC.PIC0.LONG = 0x00200000UL;	/* Clear the interrupt source CMI0 */
#endif

#else
	/* Mask the current interrupt to allow multiple interrupts */
	out_w(IT0_IDS1, IRQM(IRQ_TIMER));

	/* Clear timer interrupt */
	out_w(IT0_IIR, IRQM(IRQ_TIMER));
#endif
}
Inline void end_of_hw_timer_interrupt( void )
{
#if 1

#if	TIMER_INTLIB_USE
	EndOfInt(VECNO_TIMER);
#else
	VIC.HVA0.LONG = 0x00000000UL;		/* Dummy write */
#endif

#else
	/* Enable the current interrupt */
	out_w(IT0_IEN1, IRQM(IRQ_TIMER));
#endif
}

/*
 * Timer stop processing
 *	stop timer
 *	called during system shutdown
 */
Inline void terminate_hw_timer( void )
{
	UW	imask;

	/* disable timer interrupt */
	DisableInt(VECNO_TIMER);
	DI(imask);

#if 1
    /* Stop CMT0 count */
    CMT.CMSTR0.BIT.STR0 = 0U;
#else
	/* stop timer */
	out_w(TI_OP, 0);

	/* stop clock */
	out_w(GCLKCTRL3, in_w(GCLKCTRL3) & ~TI0_TIN_GCK);
#endif

	EI(imask);
}

/*
 * Obtain the elapsed time (nanoseconds) from the last timer interrupt
 * To compensate for the possibility that the timer interrupt may have
 * occurred during the interval when the interrupt was disabled,
 * we calculate the time in the following range:
 *		0 <= elapsed time < TIMER_PERIOD * 2
 */
Inline UINT get_hw_timer_nsec( void )
{
#if 1
	return 0;								// kari
#else
	UW	ofs, max, ovf, imask, d;

	DI(imask);

	d = in_w(DIVTIMTIN);

	max = in_w(TI_SET);
	do {
		ovf = in_w(IT0_RAW1) & IRQM(IRQ_TIMER);
		ofs = in_w(TI_RCR);
	} while ( ovf != (in_w(IT0_RAW1) & IRQM(IRQ_TIMER)) );
	if ( ovf != 0 ) ofs += max + 1;

	EI(imask);

	return ofs * 1000000000LL / TIN_CLK(d);
#endif
}

/*
 * Similar to the function as above, but returns value in microseconds.
 */
Inline UINT get_hw_timer_usec( void )
{
#if 1
	return 0;								// kari
#else
	UW	ofs, max, ovf, imask, d;

	DI(imask);

	d = in_w(DIVTIMTIN);

	max = in_w(TI_SET);
	do {
		ovf = in_w(IT0_RAW1) & IRQM(IRQ_TIMER);
		ofs = in_w(TI_RCR);
	} while ( ovf != (in_w(IT0_RAW1) & IRQM(IRQ_TIMER)) );
	if ( ovf != 0 ) ofs += max + 1;

	EI(imask);

	return (ofs + 1) * 1000000LL / TIN_CLK(d);
#endif
}

#endif /* _TKDEV_TIMER_ */


/*----------------------------------------------------------------------
#|History of "tkdev_timer.c"
#|=======================
#|* 2016/06/08	[app_rzt1]用に、[tef_em1d]用の"tkdev_timer.h"を参考に作成。
#|* 2016/07/05	MAX_TIMER_PERIODを50000→27000に変更。CMTﾀｲﾏの32分周時の限界値
#|
*/
