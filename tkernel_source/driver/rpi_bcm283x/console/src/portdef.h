/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/03/01.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/07/30.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)portdef.h	Console/Low-level serial I/O driver (BCM283x) 2016/08/29
 *
 *	Serial line low-level driver system-dependent port definition(BCM283x)
 */

#include <rpi/bcm283x.h>

/* Port definition */
IMPORT	SC_FUNC	ScFuncPL011;
IMPORT	SC_FUNC	ScFuncMINIUART;

#define	N_PORTS		2

LOCAL  SC_DEFS	PortDefs[N_PORTS] = {
	/* Port #0: UART0 */
	{
		&ScFuncPL011,
		{	UART0_BASE,			/* I/O base address */
			4,					/* iostep */
			IV_IRQ(INTERRUPT_VC_UART)	/* Interrupt Vector */
		},
		0,						/* fctl register value */
		{0, 0, 0},				/* dt[3]: Not used */
		{0, 3, 0, 0, 115200},	/* default mode */
		{0, 0, 1, 1, 0, 0, 0},	/* default flow control */
		{0, 0, 0},				/* default flow status */
	},

	/* Port #1: UART1 */
	{
		&ScFuncMINIUART,
		{	UART1_BASE,			/* I/O base address */
			4,					/* iostep */
			IV_IRQ(INTERRUPT_AUX)	/* Interrupt Vector */
		},
		0,						/* fctl register value */
		{0, 0, 0},				/* dt[3]: Not used */
		{0, 3, 0, 0, 115200},	/* default mode */
		{0, 0, 1, 1, 0, 0, 0},	/* default flow control */
		{0, 0, 0},				/* default flow status */
	}
};

#define	INIT_AUXPORT(sup)		/* Nothing */
#define	START_AUXPORT(sup)		/* Nothing */


/*----------------------------------------------------------------------
#|History of "portdef.c"
#|=======================
#|* 2016/07/30	It's made by making reference to "ns16450.c" for [tef_em1d].
#|* 2016/08/29	Port#1(UART1)の定義追加。
#|
*/
