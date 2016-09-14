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
 */

/*
 *	@(#)int.c (libtk/SH7760)
 *
 *	Interrupt controller  
 */

#include <basic.h>
#include <tk/syslib.h>
#include <tk/sysdef.h>

typedef struct {
	INTVEC	vec:16;
	INT	shift:16;
	INT	port;
} INTCTBL;

LOCAL INTCTBL intctbl[] = {
	{ IV_TMU0,	12, IPRA },
	{ IV_TMU1,	 8, IPRA },
	{ IV_TMU2,	 4, IPRA },
	{ IV_WDT,	12, IPRB },
	{ IV_REF,	 8, IPRB },
	{ IV_HUDI,	 0, IPRC },
	{ IV_GPIO,	12, IPRC },
	{ IV_DMAC,	 8, IPRC },
	{ IV_HCAN2_0,	28, INTPRI04 },
	{ IV_HCAN2_1,	24, INTPRI04 },
	{ IV_SSI_0,	20, INTPRI04 },
	{ IV_SSI_1,	16, INTPRI04 },
	{ IV_HAC_0,	12, INTPRI04 },
	{ IV_HAC_1,	 8, INTPRI04 },
	{ IV_I2C_0,	 4, INTPRI04 },
	{ IV_I2C_1,	 0, INTPRI04 },
	{ IV_USB,	28, INTPRI08 },
	{ IV_LCDC,	24, INTPRI08 },
	{ IV_DMABRG,	20, INTPRI08 },
	{ IV_SCIF_0,	16, INTPRI08 },
	{ IV_SCIF_1,	12, INTPRI08 },
	{ IV_SCIF_2,	 8, INTPRI08 },
	{ IV_SIM,	 4, INTPRI08 },
	{ IV_HSPI,	 0, INTPRI08 },
	{ IV_MMCIF,	20, INTPRI0C },
	{ IV_MFI,	12, INTPRI0C },
	{ IV_ADC,	 4, INTPRI0C },
	{ IV_CMT,	 0, INTPRI0C },
	{ 0 }
};

/*
 * Interrupt disable/enable
 */
Inline UINT _disint( void )
{
	UINT	imask;

	Asm("	stc	sr, r0	\n"
	"	mov	r0, %0	\n"
	"	or	%1, r0	\n"
	"	ldc	r0, sr	"
		: "=r"(imask)
		: "i"(SR_I(15))
		: "r0" );

	return imask;
}
Inline void _enaint( UINT imask )
{
	Asm("	ldc	%0, sr" :: "r"(imask | SR_FD));
}

#define _DI(imask)	( imask = _disint() )
#define _EI(imask)	( _enaint(imask) )

/*
 * Enable interrupt
 *	Enable the interrupt specified by 'intvec' by setting priority
 *	level to 'level'. '1'-'15' is valid for 'level' and '15' is the
 *	highest priority. If 'level' is invalid value, operation is not
 *	guaranteed.
 *	INTMSK00 register is not changed automatically, so it must be set
 *	if necessary.
 */
EXPORT void EnableInt( INTVEC intvec, INT level )
{
	INTCTBL	*tbl;
	UINT	msk, imask;

	for ( tbl = intctbl; tbl->vec != 0; tbl++ ) {
		if ( tbl->vec != intvec ) {
			continue;
		}

		msk = ~(0x0000000fU << tbl->shift);
		level <<= tbl->shift;

		_DI(imask);
		if ( tbl->port >= IPRA ) {
			out_h(tbl->port, (in_h(tbl->port) & msk) | level);
		} else {
			out_w(tbl->port, (in_w(tbl->port) & msk) | level);
		}
		_EI(imask);

		break;
	}
}

/* Disable interrupt
 *	Disable the interrupt specified by 'intvec' by setting its priority
 *	level to '0'. INTMSK00 register is not changed automatically, so it
 *	must be set if necessary.
 */
EXPORT void DisableInt( INTVEC intvec )
{
	return EnableInt(intvec, 0);
}
