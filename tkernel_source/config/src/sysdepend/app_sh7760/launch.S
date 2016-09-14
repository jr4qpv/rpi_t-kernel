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
 *	launch.S
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
	mov.l	10f, r0
	jmp	@r0			//(遅延分岐)
	nop

	.balign 4
10:	.long	0x8c002000		// RAM-kernel address


/*----------------------------------------------------------------------*/
#if 0
#|【launch.S 変更履歴】
#|□2015/12/17	[app_sh7760]用に、[tef_em1d]の"launch.S"を参考に作成。
#|
#endif
