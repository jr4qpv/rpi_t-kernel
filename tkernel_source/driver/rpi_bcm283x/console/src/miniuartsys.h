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
 *    Modified by T.Yokobayashi at 2016/08/30.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)miniuartsys.h	Console/Low-level serial I/O driver 2016/09/05
 *
 *	Serial line low level driver (for mini UART)
 *			BCM283x system-dependent definition
 */

/*
 *	"UART" clock
 */
#define	UART_CLK	(250000000)	/* 250MHz */


/*
 *	I/O input and output macro
 */
#define	OutW(ix, dt)	out_w((ix), (dt))
#define	InW(ix)		in_w((ix))

/*
 *	Post-interrupt processing
 */
Inline	void	end_inthdr(SC_DEFS *scdefs)
{
	/* Unnecessary to execute anything */
}

/*
 *	Release an interrupt handler
 */
Inline	void	delete_inthdr(SC_DEFS *scdefs, void *sio_inthdr)
{
	ER	err;

	/* Release an interrupt handler */
	err = tk_def_int(scdefs->c.intvec, NULL);
	if (err >= E_OK) {
		/* Disable interrupt */
		DisableInt(scdefs->c.intvec);
	}
}

/*
 *	Register the interrupt handler
 */
Inline	ER	regist_inthdr(SC_DEFS *scdefs, void *sio_inthdr)
{
	T_DINT	dint;
	ER	err;

	/* Register the interrupt handler */
	dint.intatr = TA_HLNG;
	dint.inthdr = sio_inthdr;
	err = tk_def_int(scdefs->c.intvec, &dint);
	if (err >= E_OK) {
		/* Enable interrupt */
		SetIntMode(scdefs->c.intvec, IM_ENA);
		EnableInt(scdefs->c.intvec);
	}
	return err;
}

/*
 *	Suspend/Resume
 */
Inline	void	sio_susres(LINE_INFO *li, W resume)
{
	/* Execute nothing */
}


/*----------------------------------------------------------------------
#|History of "miniuartsys.h"
#|==========================
#|* 2016/08/30	It's made by making reference to "ns16450sys.h" for [tef_em1d].
#|
*/
