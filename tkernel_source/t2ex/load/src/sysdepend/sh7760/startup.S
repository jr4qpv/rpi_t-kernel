/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/04.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/01/05.
 *
 *----------------------------------------------------------------------
 */
/*
 * This software package is available for use, modification, 
 * and redistribution in accordance with the terms of the attached 
 * T-License 2.x.
 * If you want to redistribute the source code, you need to attach 
 * the T-License 2.x document.
 * There's no obligation to publish the content, and no obligation 
 * to disclose it to the TRON Forum if you have modified the 
 * software package.
 * You can also distribute the modified source code. In this case, 
 * please register the modification to T-Kernel traceability service.
 * People can know the history of modifications by the service, 
 * and can be sure that the version you have inherited some 
 * modification of a particular version or not.
 *
 *    http://trace.tron.org/tk/?lang=en
 *    http://trace.tron.org/tk/?lang=ja
 *
 * As per the provisions of the T-License 2.x, TRON Forum ensures that 
 * the portion of the software that is copyrighted by Ken Sakamura or 
 * the TRON Forum does not infringe the copyrights of a third party.
 * However, it does not make any warranty other than this.
 * DISCLAIMER: TRON Forum and Ken Sakamura shall not be held
 * responsible for any consequences or damages caused directly or
 * indirectly by the use of this software package.
 *
 * The source codes in bsd_source.tar.gz in this software package are 
 * derived from NetBSD or OpenBSD and not covered under T-License 2.x.
 * They need to be changed or redistributed according to the 
 * representation of each source header.
 */

/*
 *	startup.S
 *
 *       T2EX: program load functions
 *       program module startup (SH7760)
 */

#define _in_asm_source_
#include <machine.h>
#include <tk/asm.h>

/*
 * System program startup
 * ER pmCallStartup( INT ac, UB *av[], FP entry )
 */
	.text
	.balign	4
	.globl	Csym(pmCallStartup)
Csym(pmCallStartup):
	sts.l	pr, @-SP			// save registers
	stc.l	gbr, @-SP
	sts.l	mach, @-SP			// 不要 ?
	sts.l	macl, @-SP			// 不要 ?
	mov.l	r14, @-SP
	mov.l	r13, @-SP
	mov.l	r12, @-SP
	mov.l	r11, @-SP
	mov.l	r10, @-SP
	mov.l	r9, @-SP
	mov.l	r8, @-SP

	jsr	@r6				// call entry(ac, av)
	nop

	mov.l	@SP+, r8			// restore registers
	mov.l	@SP+, r9
	mov.l	@SP+, r10
	mov.l	@SP+, r11
	mov.l	@SP+, r12
	mov.l	@SP+, r13
	mov.l	@SP+, r14
	lds.l	@SP+, macl			// 不要 ?
	lds.l	@SP+, mach			// 不要 ?
	ldc.l	@SP+, gbr
	lds.l	@SP+, pr

	rts
	nop


/* ------------------------------------------------------------------------ */
#if 0
#|【startup.S 変更履歴】
#|□2016/01/05	../tef_em1d/から、 [app_sh7760]用にコピーし、修正。
#|
#endif
