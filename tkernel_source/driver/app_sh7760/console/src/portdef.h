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
 *    Modified by T.Yokobayashi at 2015/12/24.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)portdef.h	(Console/Low-level serial I/O driver) 2016/03/17
 *
 *	Serial line low-level driver system-dependent port definition(app_sh7760)
 */

/* Port definition */
IMPORT	SC_FUNC	ScFuncSCIF;

#define	N_PORTS		1

LOCAL  SC_DEFS	PortDefs[N_PORTS] = {
	/* Port #0: UART0 */
	{
		&ScFuncSCIF,
		{	0xfe600000,			/* SCIF0 I/O base address */
			12,					/* iostep(SH_SCIFでは、ハード割り込みレベル) */
			IV_SCIF_0			/* Interrupt Vector */
		},
		0,						/* fctl register value */
		{0, 0, 0},				/* dt[3]: Not used */
#if 1
		{0, 3, 0, 0, 115200},	/* default mode */
		{0, 0, 1, 1, 0, 0, 0},	/* default flow control */
#else
		{1, 3, 0, 0, 115200},	/* TERM標準 mode(odd,8bit,stop-1)  */
///		{1, 3, 0, 0, 38400},	/* TERM標準 mode(odd,8bit,stop-1)  */
		{0, 0, 0, 0, 0, 1, 1},	/* default flow control(XON/OFF control) */
#endif
		{0, 0, 0},				/* default flow status */
	}
};

#define	INIT_AUXPORT(sup)		/* Nothing */
#define	START_AUXPORT(sup)		/* Nothing */


/*----------------------------------------------------------------------*/
#if 0
#|History of "line_drv.c"
#|=======================
#|* 2015/12/24	[app_sh7760]用に、[tef_em1d]の"portdef.h"を参考に作成。
#|
#endif
