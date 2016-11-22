/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/03/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)eitent.S (monitor/hwdepend) 2016/09/08
 *
 *      BCM283x (ARM1176JFZ-S/Cortex-A7) exception branch handling
 */

#define	_in_asm_source_

#include <machine.h>
#include <tk/sysdef.h>
#include <rpi/bcm283x.h>
#include <sys/sysinfo.h>

#include "setup_bcm283x.h"

#define	base(n)		( (n) & 0xfffff000 )
#define	offs(n)		( (n) & 0x00000fff )

#define	base1(n)	( (n) & 0xffffff00 )
#define	offs1(n)	( (n) & 0x000000ff )


// see <sys/sysdepend/rpi_bcm283x/sysinfo_depend.h>
#define	N_INTVEC	256

/*
 *       EIT entry
 */

	.section EITBRA, "ax"
	.arm
	.org	0x00000000
	b	startup_entry		// 00 : reset
	b	undef_entry		// 04 : undefined instruction exception
	b	svc_entry		// 08 : supervisor call (SVC)
	b	iabort_entry		// 0C : prefetch abort
	b	dabort_entry		// 10 : data abort
	nop				// 14 : (reserved)
	b	irq_entry		// 18 : interrupt
	.org	0x0000001c		// 1C : fast interrupt

/*
 * fast interrupt
 *       calls the handler defined at FIQ interrupt vector unconditionally.
 *       no saving of registers to stack is performed.
 *       the content of R12_fiq(ip) register is overwritten.
 */
fiq_entry:
	ldr	ip, =base(EIT_VECTBL)
	ldr	ip, [ip, #offs(EITVEC(EIT_FIQ))]
	bx	ip

/*
 * interrupt
 *       ACPU interrupt mask status register of Interrupt controller (ARMCTRL_IC_BASE)
 *      Judge the priority of interrupts using (ARM_IRQ_PEND0,1,2),
 *       the highest interrupt's handler is called by jumping into it.
 *       Interrupt priority is descending order of interrupt factor (INT 0-95) , and INT 95 (IRQ 95) is highest.
 *       INT 0 (IRQ 0) has the lowest priority.
 *       If there is no cause of the interrupt, the handler of INT 95 (IRQ95) is called.
 *		+---------------+
 *	sp  ->	|R3		|
 *		|R12=ip		|
 *              |R14=lr         | <- return address from interrupt
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
irq_entry:
	sub	lr, lr, #4		// return address adjustment
	srsdb	sp!, #PSR_IRQ		// save registers
	stmfd	sp!, {r3, ip}

	ldr	lr, =base1(ARMCTRL_IC_BASE)
  l_irq_pend:
	ldr	ip, =EITVEC(EIT_IRQ(31))

	ldr	r3, [lr, #offs1(ARM_IRQ_PEND1)]
	cmp	r3, #0
	bne	l_irq_br

	add	ip, ip, #32*4
	ldr	r3, [lr, #offs1(ARM_IRQ_PEND2)]
	cmp	r3, #0
	bne	l_irq_br

	add	ip, ip, #32*4
	ldr	r3, [lr, #offs1(ARM_IRQ_PEND0)]
	tst	r3, #0x00000300		// bit9,8(irq_pend) ?
	bne	l_irq_pend		// pend1,2チェック
	bic	r3, r3, #0x001F8000	// bit20～15は無視
	cmp	r3, #0
	bne	l_irq_br

//	タイミングで取りこぼしがあるようなので再チェック
	ldr	r3, [lr, #offs1(ARM_IRQ_PEND0)]
	tst	r3, #0x00000300		// bit9,8(irq_pend) ?
	bne	l_irq_pend		// pend1,2チェック
	bic	r3, r3, #0x001F8000	// bit20～15は無視
	cmp	r3, #0
	bne	l_irq_br

#if 1
  #ifdef __MonDebugWK
	ldr	ip, =__MonDebugWK + __INFO_ILLIRQ
	ldr	r3, [ip]
	add	r3, r3, #1
	str	r3, [ip]		// ここを通った回数を記録しておく
  #endif

//	要因のない割り込みなので無視してリターン
	ldmfd	sp!, {r3, ip}
	rfefd	sp!
#else
	ldr	ip, =EITVEC(EIT_IRQ(95))	// 不正トラップとする
#endif

  l_irq_br:
	clzne	r3, r3
	ldr	lr, [ip, -r3, lsl #2]!
	cmp	lr, #0
	bxne	lr
	b	default_entry

/*
 * undefined instruction
 *		+---------------+
 *	sp  ->	|R12=ip		|
 *              |R14=lr         | <- the return address, i.e., the next address that follows the undefined instruction
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
undef_entry:
	srsdb	sp!, #PSR_UND		// save registers
	stmfd	sp!, {ip}

	ldr	ip, =base(EITVEC(EIT_UNDEF))
	ldr	lr, [ip, #offs(EITVEC(EIT_UNDEF))]!
	cmp	lr, #0
	bxne	lr
	b	default_entry

/*
 * supervisor call(SVC)
 *      the valid range of supervisor call number is 0-255 (N_INTVEC - 1).
 *       if an out of range value is given, treat it as SVC 0, and invokes the default handler.
 *		+---------------+
 *	sp  ->	|R12=ip		|
 *              |R14=lr         | <- return address: the address that follows the SVC instruction
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
svc_entry:
	srsdb	sp!, #PSR_SVC		// save registers
	stmfd	sp!, {ip}

	mrs	ip, spsr
	tst	ip, #PSR_T
	ldrneh	ip, [lr, #-2]		// Thumb instruction
	ldreq	ip, [lr, #-4]		// ARM instruction
	bicne	lr, ip, #0xff00
	biceq	lr, ip, #0xff000000
	cmp	lr, #N_INTVEC		// lr = software interrupt number
	movge	lr, #0

	ldr	ip, =EIT_VECTBL
	ldr	lr, [ip, lr, lsl #2]!
	cmp	lr, #0
	bxne	lr
	b	default_entry

/*
 * prefetch abort
 *       in the case of debug event, debug abort (instruction) handler is called.
 *       Otherwise, prefetch abort handler is called.
 *		+---------------+
 *	sp  ->	|R12=ip		|
 *              |R14=lr         | <- return address: the address of aborted instruction
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
iabort_entry:
	sub	lr, lr, #4		// return address adjustment
	srsdb	sp!, #PSR_ABT		// save registers
	stmfd	sp!, {ip}

	mrc	p15, 0, ip, c5, c0, 1	// IFSR
	tst	ip, #0x400		// FS[4]
	and	ip, ip, #0x00f		// FS[3:0]
	cmpeq	ip, #FSR_DebugEvent

	ldr	ip, =base(EITVEC(EIT_IABORT))
	ldrne	lr, [ip, #offs(EITVEC(EIT_IABORT))]!
	ldreq	lr, [ip, #offs(EITVEC(EIT_IDEBUG))]!
	cmp	lr, #0
	bxne	lr
	b	default_entry

/*
 * data abort
 *       in the case of debug event, debug abort (data) handler is called.
 *       Otherwise, data abort handler is called.
 *		+---------------+
 *	sp  ->	|R12=ip		|
 *              |R14=lr         | <- return address: the address of aborted instruction
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
dabort_entry:
	sub	lr, lr, #8		// return address adjustment
	srsdb	sp!, #PSR_ABT		// save registers
	stmfd	sp!, {ip}

	mrc	p15, 0, ip, c5, c0, 0	// DFSR
	tst	ip, #0x400		// FS[4]
	and	ip, ip, #0x00f		// FS[3:0]
	cmpeq	ip, #FSR_DebugEvent

	ldr	ip, =base(EITVEC(EIT_DABORT))
	ldrne	lr, [ip, #offs(EITVEC(EIT_DABORT))]!
	ldreq	lr, [ip, #offs(EITVEC(EIT_DDEBUG))]!
	cmp	lr, #0
	bxne	lr
	b	default_entry

/*
 * default handler
 *       stack contains the details of the generated exception.
 *      registers upon handler invocation
 *       ip = address of the vector table for the raised exception
 *       lr = indeterminate
 */
default_entry:
	ldr	lr, =base(EITVEC(EIT_DEFAULT))
	ldr	lr, [lr, #offs(EITVEC(EIT_DEFAULT))]
	bx	lr

	.pool


/*----------------------------------------------------------------------
#|History of "eitent.S"
#|=====================
#|* 2016/03/04	[rpi_bcm283x]用に、[tef_em1d]用の"eitent.S"を参考に作成。
#|
*/
