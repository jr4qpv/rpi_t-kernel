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
 *    Modified by T.Yokobayashi at 2015/12/17.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)launch.S (sh7760) 2016/11/10
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
	.globl	Csym(START)
	.type	Csym(START), %function
Csym(START):
	mov.l	10f, r0
	jmp	@r0			//(遅延分岐)
	nop

	.balign 4
10:	.long	0x8c002000		// RAM-kernel address


/*----------------------------------------------------------------------
#|History of "launch.S"
#|======================
#|* 2015/12/17	[app_sh7760]用に、[tef_em1d]の"launch.S"を参考に作成。
#|* 2016/11/10	KPIT GCCに対応の為、STARTﾗﾍﾞﾙにCsymを適用。
#|
*/
