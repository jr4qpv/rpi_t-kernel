/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/03/11.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/04/20.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)launch.S () 2016/04/20
 *
 *	Start RAM-kernel by BD command of T-Monitor.
 *
 *	BD command of T-Monitor requires that the kernel start address
 *	resides within the boundaries of ROM address space.
 *	Therefore, we put a simple stub on ROM that simply jumps to
 *	the actual RAM-kernel address.
 */

#include <machine.h>
#include <tk/asm.h>

	.text
	.balign	4
	.globl	START
	.type	START, %function
START:
//	b	START			//// for debug /////
//	ldr	pc, =0x00006000		// RAM-kernel address
	ldr	pc, =0x00008000		// RAM-kernel address ///// kari //////


/*----------------------------------------------------------------------
#|History of "launch.S"
#|======================
#|* 2016/04/20	It's copied from "../tef_em1d/" and it's modified.
#|
*/
