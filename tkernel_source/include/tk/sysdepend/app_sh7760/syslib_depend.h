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
 *	@(#)syslib_depend.h (tk/SH7760)
 *
 *	T-Kernel/SM SH7760 Library
 */

#ifndef __TK_SYSLIB_DEPEND_H__
#define __TK_SYSLIB_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CPU interrupt control
 *	'intsts' is the value of SR register in CPU
 *	disint()  Set SR.I = 15 and return the original SR to the return value.
 *	enaint()  Set SR.I = intsts.I. Do not change except for SR.I.
 *		  Return the original SR to the return value.
 */
IMPORT UINT disint( void );
IMPORT UINT enaint( UINT intsts );
#define DI(intsts)	( (intsts) = disint() )
#define EI(intsts)	( enaint(intsts) )
#define isDI(intsts)	( ((intsts) & 0x00f0U) != 0 )

/*
 * Enable the higher level of interrupt than intlevel (0-15)
 *	Set 'intlevel' to SR.I in CPU.
 *	Return the original SR to the return value.
 *	Use when enabling a multiplexed interrupt in interrupt handler.
 */
#define SetIntLevel(intlevel)	( enaint((intlevel) << 4) )

/*
 * Request the interrupt level (1-15) by the value of INTEVT (0x200-0x3c0)
 */
#define IntLevel(intevt)	( 0x1f - ((intevt) >> 5) )

/*
 * Interrupt vector
 *	Interrupt vector is the value of the interrupt factor code INTVEC 2
 */
typedef UINT	INTVEC;

/* Convert to the interrupt definition number */
#define DINTNO(intvec)	(intvec)

/*
 * Interrupt vector(INTVEC) : Typical exception code
 *	Only these typical exception codes can be used with
 *	an INTC operation functions.
 */
#define IV_TMU0		0x400	/* Timer ch.0 */
#define IV_TMU1		0x420	/* Timer ch.1 */
#define IV_TMU2		0x440	/* Timer ch.2 */
#define IV_WDT		0x560	/* WDT */
#define IV_REF		0x580	/* Memory refresh */
#define IV_HUDI		0x600	/* HUDI */
#define IV_GPIO		0x620	/* I/O port */
#define IV_DMAC		0x640	/* DMA */
#define IV_HCAN2_0	0x900	/* HCAN */
#define IV_HCAN2_1	0x920	/* HCAN */
#define IV_SSI_0	0x940	/* SSI */
#define IV_SSI_1	0x960	/* SSI */
#define IV_HAC_0	0x980	/* HAC */
#define IV_HAC_1	0x9a0	/* HAC */
#define IV_I2C_0	0x9c0	/* Inter IC Bus */
#define IV_I2C_1	0x9e0	/* Inter IC Bus */
#define IV_USB		0xa00	/* USB host */
#define IV_LCDC		0xa20	/* LCD controller */
#define IV_DMABRG	0xa80	/* DMA */
#define IV_SCIF_0	0x880	/* SCIF */
#define IV_SCIF_1	0xb00	/* SCIF */
#define IV_SCIF_2	0xb80	/* SCIF */
#define IV_SIM		0xc00	/* SIM */
#define IV_HSPI		0xc80	/* HSPI */
#define IV_MMCIF	0xd00	/* MMCIF */
#define IV_MFI		0xe80	/* MFI */
#define IV_ADC		0xf80	/* A/D */
#define IV_CMT		0xfa0	/* CMT */

/*
 * Conversion between IRL interrupt and interrupt vector.
 *  (*)IRL shows interrupt level('1'-'15'), not /IRL[3:0] signal value.
 */
#define IV_IRL(irl)	(0x3e0 - (0x20 * (irl)))
#define IRLNO(vec)	((0x3e0 - (vec)) / 0x20)

/*
 * Enable interrupt
 *	Set priority level to 'level' regarding interrupt specified by
 *	'intvec'.
 *	'1'-'15' is valid for 'level' and '15' is the highest level.
 *	If 'level' is invalid value, operation is not guaranteed.
 *	The INTMSK00 register is not changed  automatically, so it must be
 *	set if necessary.
 */
IMPORT void EnableInt( INTVEC intvec, INT level );

/*
 * Disable interrupt
 *	Disable interrupt specified by 'intvec' (i.e. set level 0).
 *	The INTMSK00 register is not changed  automatically, so it must be set
 *	if necessary.
 */
IMPORT void DisableInt( INTVEC intvec );

/*
 * Issue EOI(End Of Interrupt)
 */
#define EndOfInt(intvec)

/* ------------------------------------------------------------------------ */

/*
 * I/O port access
 *	Only memory mapped I/O for SH
 */
Inline void out_w( INT port, UW data )
{
	Asm("mov.l %0, @%1":: "r"(data), "r"(port));
}
Inline void out_h( INT port, UH data )
{
	Asm("mov.w %0, @%1":: "r"(data), "r"(port));
}
Inline void out_b( INT port, UB data )
{
	Asm("mov.b %0, @%1":: "r"(data), "r"(port));
}

Inline UW in_w( INT port )
{
	UW	data;
	Asm("mov.l @%1, %0": "=r"(data): "r"(port));
	return data;
}
Inline UH in_h( INT port )
{
	UH	data;
	/* Compensation for PC card I/O */
	if ( ((UINT)port & 0xfffc0000U) == 0xb8240000U
	  || ((UINT)port & 0xff3c0000U) == 0xb6240000U ) {
		port -= 0x40000;
	}
	Asm("mov.w @%1, %0": "=r"(data): "r"(port));
	return data;
}
Inline UB in_b( INT port )
{
	UB	data;
	Asm("mov.b @%1, %0": "=r"(data): "r"(port));
	return data;
}

#ifdef __cplusplus
}
#endif
#endif /* __TK_SYSLIB_DEPEND_H__ */
