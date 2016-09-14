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


#include <machine.h>

#if _Csym == 0
#define	START	_start
#else
#define	START	start
#endif

	.text
	.globl	START
	.type	START, @function
	.balign	2
START:
	mov.l	L_Startup, r0
	jmp	@r0
	nop

		.balign	4
L_Startup:	.long	Csym(_C_startup)
