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
 *	@(#)asmstartup.S (crttk)
 *
 *	Driver/manager startup 
 */

#include <machine.h>
#include <tk/asm.h>

	.text
	.balign	4
	.globl	Csym(_C_startup)
	.type	Csym(_C_startup), @function
Csym(_C_startup):
	cmp/pz	r4
	bf	l_noinit	// Initialize is performed only when ac >= 0

	mov.l	DATA_ORG, r1	// When data area does not need to be moved,
	mov.l	DATA_START, r2	// OS loads
	cmp/eq	r1, r2
	bt	l_noinit

        mov.l   DATA_END, r3
        bra     l1		// initialize data area
        nop
  copydata:
	mov.l	@r1+, r0
	mov.l	r0, @r2
	add	#4, r2
  l1:
	cmp/hi	r2, r3
	bt	copydata

	mov.l	BSS_START, r2	// clear bss area
	mov.l	BSS_END, r3
	bra	l2
	mov	#0, r0
  clrbss:
	mov.l	r0, @-r3
  l2:
	cmp/hi	r2, r3
	bt	clrbss

  l_noinit:
	mov.l	L_Startup, r0
	jmp	@r0
	nop

		.balign	4
DATA_ORG:	.long	Csym(__data_org)
DATA_START:	.long	Csym(__data_start)
DATA_END:	.long	Csym(_edata)
BSS_START:	.long	Csym(__bss_start)
BSS_END:	.long	Csym(_end)
L_Startup:	.long	Csym(_P_startup)
