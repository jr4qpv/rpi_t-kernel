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
 *	@(#)tmsvc.h (libtm/SH7760)
 *
 *	T-Engine/SH7760 definitions
 *
 *	* Used by assembler 
 */

#include <tk/sysdef.h>

/*
 * T-Monitor service call 
 */
.macro _TMCALL func, fno
	.text
	.balign	2
	.globl	Csym(\func)
	.type	Csym(\func), @function
Csym(\func):
  .if \fno < 128
	mov	#\fno, r0
	trapa	#TRAP_MONITOR
	rts
	nop
  .else
	mov.l	fno_\func, r0
	trapa	#TRAP_MONITOR
	rts
	nop
		.balign	4
    fno_\func:	.long	\fno
  .endif
.endm

#define TMCALL(func, fno)	_TMCALL func, fno
