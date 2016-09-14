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
 *	@(#)disint.S (libtk/SH7760)
 *
 *	Enable/disable external interrupt.
 */

#include <machine.h>
#include <tk/sysdef.h>

/*
 * Disable external interrupt 
 * UINT disint( void )
 *	SR.I = 15; the return value is SR prior to the change.
 */
	.text
	.balign	2
	.globl	Csym(disint)
	.type	Csym(disint), @function
Csym(disint):
	stc	sr, r0
	stc	sr, r1
	or	#SR_I(15), r0
	ldc	r0, sr
	rts
	mov	r1, r0

/*
 * Enable external interrupt (restore to original state)
 * UINT enaint( UINT intsts )
 *	SR.I = msk.I. Only  SR.I changes; all else remains unchanged.
 *	The return value is SR prior to the change.
 */
	.text
	.balign	2
	.globl	Csym(enaint)
	.type	Csym(enaint), @function
Csym(enaint):
	stc	sr, r0
	stc	sr, r1
	xor	r4, r0
	and	#SR_I(15), r0
	xor	r1, r0
#if CPU_SH4
	mov.l	L_SR_FD, r2	// To prevent accidental FD = 0 setting
	or	r2, r0		// Set to FD = 1 at all times
#endif
	ldc	r0, sr
	rts
	mov	r1, r0

#if CPU_SH4
		.balign	4
  L_SR_FD:	.long	SR_FD
#endif
