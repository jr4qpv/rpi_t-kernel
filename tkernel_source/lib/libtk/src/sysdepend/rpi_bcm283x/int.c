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
 *    Modified by T.Yokobayashi at 2016/07/23.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)int.c (BCM283x) 2016/07/23
 *
 *	BCM283x interrupt controller management
 *
 *	intvec passed to the functions for interrupt controller must
 *      be within the valid ranges of IRQ and GPIO interrupt.
 *	If a value outside the valid range is passed, the subsequent
 *      correct behavior of the system is not guaranteed.
 */

#include <basic.h>
#include <tk/syslib.h>
#include <tk/sysdef.h>
#include <rpi/bcm283x.h>


#define	AINTNO(intvec)	( ((intvec) - IV_IRQ(0)) / 32 )		/* AINT number */
#define	BITNO(intvec)	( ((intvec) - IV_IRQ(0)) % 32 )		/* bit number */

/*
 * Interrupt controller
 *	register size W
 */
#define	AINT(n)		( ARMCTRL_IC_BASE + (n) )	/* ARM Interrupt controller */

#define	IT0_IEN0	AINT(0x0010)	/* RW ACPU Enable IRQs 1 */
#define	IT0_IEN1	AINT(0x0014)	/* RW ACPU Enable IRQs 2 */
#define	IT0_IEN2	AINT(0x0018)	/* RW ACPU Enable Basic IRQs */
#define	IT0_IDS0	AINT(0x001c)	/* -W ACPU Disable IRQs 1 */
#define	IT0_IDS1	AINT(0x0020)	/* -W ACPU Disable IRQs 2 */
#define	IT0_IDS2	AINT(0x0024)	/* -W ACPU Disable Basic IRQs */
#define	IT0_RAW0	AINT(0x0004)	/* R- ACPU interrupt Raw status (Pend1) */
#define	IT0_RAW1	AINT(0x0008)	/* R- ACPU interrupt Raw status (Pend2) */
#define	IT0_RAW2	AINT(0x0000)	/* R- ACPU interrupt Raw status (Basic) */

LOCAL const struct aint_reg {
	UINT	IT0_IEN;	/* RW ACPU enable interrupt state */
	UINT	IT0_IDS;	/* -W ACPU disable interrupt */
	UINT	IT0_RAW;	/* R- ACPU interrupt Raw status */
} AINT[3] = {
	{ IT0_IEN0, IT0_IDS0, IT0_RAW0 },
	{ IT0_IEN1, IT0_IDS1, IT0_RAW1 },
	{ IT0_IEN2, IT0_IDS2, IT0_RAW2 }
};


/* ------------------------------------------------------------------------ */

/*
 * Disable / Enable interrupt
 */
Inline UW _disint( void )
{
	UW	imask;
	Asm("	mrs	%0, cpsr	\n"
	"	orr	ip, %0, %1	\n"
	"	msr	cpsr_xc, ip	"
		: "=r"(imask)
		: "i"(PSR_DI)
		: "ip" );
	return imask;
}
Inline void _enaint( UW imask )
{
	Asm("msr cpsr_xc, %0":: "r"(imask));
}
#define	_DI(imask)	( imask = _disint() )
#define	_EI(imask)	( _enaint(imask) )

/*
 * Set interrupt mode.
 *	Set the interrupt mode specified by `intvec' to the mode given
 *  	by `mode'. If an illegal mode is given, subsequent correct behavior
 *      of the system is not guaranteed.
 *
 *	The case of IRQ
 *	mode := IM_ENA | IM_INV
 *	or	IM_DIS
 *
 *	The case of GPIO
 *	mode := IM_ENA | IM_LEVEL | (IM_HI || IM_LOW) | IM_ASYN
 *	 or IM_ENA | IM_EDGE | (IM_HI || IM_LOW || IM_BOTH) | IM_ASYN
 *	 or IM_DIS
 *
 *	If IM_ENA is specified, the mode setting is done, and
 *      interrupt is disabled (DisableInt) and the interrupt pin is
 *      asserted.
 *      If IM_DIS is specified, interrupt pin is disasserted.
 *      disasserted interrupt pin doesn't generate interrupt even if
 *      it is enabled (EnabledInt).
 *      In the initial state, pins are dis-asserted (IM_DIS).
 *
 *	The initial status of the following interrupt controllers
 *	that manage GPIO interrupt is such that
 *      as if IM_ENA had been specified and interrupts are enabled
 * 	(EnableInt).
 *
 *		IRQ26	GIO6 Interrupt (GPIO port  96--111)
 *		IRQ27	GIO7 Interrupt (GPIO port 112--127)
 *		IRQ50	GIO0 Interrupt (GPIO port   0-- 15)
 *		IRQ51	GIO1 Interrupt (GPIO port  16-- 31)
 *		IRQ52	GIO2 Interrupt (GPIO port  32-- 47)
 *		IRQ53	GIO3 Interrupt (GPIO port  48-- 63)
 *		IRQ79	GIO4 Interrupt (GPIO port  64-- 79)
 *		IRQ80	GIO5 Interrupt (GPIO port  80-- 95)
 */
EXPORT void SetIntMode( INTVEC intvec, UINT mode )
{

	if ((intvec < IV_IRQ(0)) || (intvec > IV_IRQ(MAXIRQNUM)))
		return;							// ベクタ番号が不正の時は無視

#if 0	///////////////////
	UW	m = 1 << BITNO(intvec);
	UINT	imask;

	if ( intvec < IV_GPIO(0) ) {
		/* IRQ */
		const struct aint_reg	*r = AINT + AINTNO(intvec);

		out_w(r->IT0_IDS,  m);	/* disable interrupt */
		out_w(r->IT0_IDSS, m);	/* disassert interrupt pin */
		if ( (mode & IM_ENA) == 0 ) return;

		if ( (mode & IM_INV) == 0 ) out_w(r->IT_PINV_CLR, m);
		else			    out_w(r->IT_PINV_SET, m);

		out_w(r->IT0_IENS, m);	/* assert interrupt pin */
	} else {
		/* GPIO */
		UINT	gb = GIOB[GIONO(intvec)];
		UINT	n;
		UW	mm, mv;

		_DI(imask);
		out_w(GIO_IDS(gb), m);			    /* disable interrupt */
		out_w(GIO_IIA(gb), in_w(GIO_IIA(gb)) & ~m); /* disassert interrupt pin */
		_EI(imask);
		if ( (mode & IM_ENA) == 0 ) return;

		n = (intvec - IV_GPIO(0)) % 8 * 4;
		mm = 0xf << n;
		mv = ((mode >> 8) & 0xf) << n;
		n = (intvec - IV_GPIO(0)) % 32 / 8;
		_DI(imask);
		out_w(GIO_IDT(n, gb), (in_w(GIO_IDT(n, gb)) & ~mm) | mv);
		out_w(GIO_IIA(gb), in_w(GIO_IIA(gb)) | m); /* assert interrupt pin */
		_EI(imask);
	}
#endif	/////////////////
}

/*
 * Enable interrupt
 *	enable interrupt specified by `intvec'
 */
EXPORT void EnableInt( INTVEC intvec )
{
	UW	m = 1 << BITNO(intvec);

	if ((intvec < IV_IRQ(0)) || (intvec > IV_IRQ(MAXIRQNUM)))
		return;							// ベクタ番号が不正の時は無視

	const struct aint_reg	*r = AINT + AINTNO(intvec);
	out_w(r->IT0_IEN, m);
}

/*
 * Disable interrupt
 *	disable interrupt specified by `intvec'
 */
EXPORT void DisableInt( INTVEC intvec )
{
	UW	m = 1 << BITNO(intvec);

	if ((intvec < IV_IRQ(0)) || (intvec > IV_IRQ(MAXIRQNUM)))
		return;							// ベクタ番号が不正の時は無視

	const struct aint_reg	*r = AINT + AINTNO(intvec);
	out_w(r->IT0_IDS, m);
}

/*
 * Clear the request for interrupt
 *	clear the request of interrupt specified by `intvec'.
 *	We need to clear the requests only in the case of edge-trigger interrupts.
 */
EXPORT void ClearInt( INTVEC intvec )
{

	if ((intvec < IV_IRQ(0)) || (intvec > IV_IRQ(MAXIRQNUM)))
		return;							// ベクタ番号が不正の時は無視

#if 0	/////////////
	UW	m = 1 << BITNO(intvec);

	if ( intvec < IV_IRQ(32 ) ) {
		/* IRQ 0-31  no relevant targets */

	} else if ( intvec < IV_IRQ(64) ) {
		/* IRQ 32--63 */
		out_w(IT0_IIR, m);

	} else if ( intvec < IV_GPIO(0) ) {
		/* IRQ 64-95 no relevant targets */

	} else {
		/* GPIO */
		UINT	gb = GIOB[GIONO(intvec)];
		out_w(GIO_IIR(gb), m);
	}
#endif	/////////////////
}

/*
 * Check the existence of interrupt request
 *	check the existence of request for interrupt specified by intvec
 *	If there is, TRUE (non-zero value) is returned
 *
 *	The existence of the request of interrupt is checked by
 *      'raw' status register.
 */
EXPORT BOOL CheckInt( INTVEC intvec )
{
	UW	sts;

	if ((intvec < IV_IRQ(0)) || (intvec > IV_IRQ(MAXIRQNUM)))
		return 0;						// ベクタ番号が不正の時は無視

	const struct aint_reg	*r = AINT + AINTNO(intvec);
	sts = in_w(r->IT0_RAW);

	return (sts >> BITNO(intvec)) & 1;
}


/* ------------------------------------------------------------------------
#|History of "int.c"
#|==================
#|* 2016/07/23	It's copied from "../tef_em1d/" and it's modified.
#|
*/
