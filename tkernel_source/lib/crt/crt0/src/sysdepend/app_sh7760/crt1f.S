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
 *	@(#)crt1f.S (SH)
 *
 *	C startup 
 *
 *	For static links 
 *	shared library loader is not linked.
 */

#include <machine.h>

	.data
	.globl	Csym(__initloading)
	.globl	Csym(__callinit)
	.globl	Csym(__callfini)

			.balign	4
Csym(__initloading):	.int	initloading
Csym(__callinit):	.int	Csym(_init)
Csym(__callfini):	.int	Csym(_fini)

	.text
	.balign	2
initloading:
	/* No action */
	rts
	nop
