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
 *    Modified by T.Yokobayashi at 2016/03/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)reset.S (BCM283x) 2016/08/29
 *
 *      BCM283x: initial setting after a reset.
 */

#define	_in_asm_source_

#include <machine.h>
#include <tk/sysdef.h>
#include <rpi/bcm283x.h>
#include <sys/sysinfo.h>

#include "setup_bcm283x.h"

/*
 * macro for setting up registers
 */
.macro out_w reg, val
  .ifnes "\val", ""	// when val is empty, do nothing.
	ldr	r0, =\reg
	ldr	r1, =\val
	str	r1, [r0]
  .endif
.endm

.macro setup_param	// r0: address of parameter string.
0:			// * r0, r2, and r3 are going to be clobbered.
	ldmia	r0!, {r2, r3}
	cmp	r2, #0
	strne	r3, [r2]
	bne	0b
.endm

.macro	wait_nsec	// r0: wait time (nsec)
			// * Assume one step is 4 ns @ (500MHz)
	lsr	r0, r0, #2
0:
	subs	r0, r0, #1
	bne	0b
.endm

/*
 * memory barrier macros
 */
.macro _mov reg, val
  .ifnes "\reg", "\val"
	mov	\reg, \val
  .endif
.endm
.macro .ISB reg, val=#0
#ifdef CPU_ARMv6
	_mov	\reg, \val
	mcr	p15, 0, \reg, cr7, c5, 4
#else	/* CPU_ARMv7以降 */
	isb
#endif
.endm
.macro .DSB reg, val=#0
#ifdef CPU_ARMv6
	_mov	\reg, \val
	mcr	p15, 0, \reg, cr7, c10, 4
#else	/* CPU_ARMv7以降 */
	dsb
#endif
.endm
.macro .DMB reg, val=#0
#ifdef CPU_ARMv6
	_mov	\reg, \val
	mcr	p15, 0, \reg, cr7, c10, 5
#else	/* CPU_ARMv7以降 */
	dmb
#endif
.endm

/*----------------------------------------------------------------------
        T-Monitor boot processing
----------------------------------------------------------------------*/
	.section .startup, "ax"
	.balign	4
	.globl	startup_entry
	.type	startup_entry, %function
	.org	0x00000000
startup_entry:

#if CPU_BCM2835				// BCM2835 ?
// SVC mode, FIQ/IRQ interrupt disabled
	mov	r0, #(PSR_SVC | PSR_I | PSR_F)
	msr	cpsr_fsxc, r0
#else					// BCM2836 or BCM2837
	/* Disable IRQ & FIQ interrupt */
	cpsid	if

	/* Check for HYP mode */
	mrs	r0, cpsr_all
	and	r0, r0, #0x1F
	cmp	r0, #0x1A		// Hyp mode ?
	bne	continueBoot

	/* Get out of HYP mode */	// TODO: HYP stub to allow HVC call
	ldr	r1, =continueBoot
	msr	ELR_hyp, r1
	mrs	r1, cpsr_all
	bic	r1, r1, #0x1F
	orr	r1, r1, #PSR_SVC
	msr	SPSR_hyp, r1
	eret				// jump continueBoot

continueBoot: /* Continue with bootup */
	/* check CPU ID--MPIDR */
	mrc	p15, 0, r0, cr0, cr0, 5	// c0,0,c0,5:MPIDR read
	and	r0, r0, #3		// get CPU ID
	cmp	r0, #0
	beq	cpu00_start
cpu_sleep:
	wfe				// sleep
	b	cpu_sleep
cpu00_start:
#endif

// use On-Chip SRAM as stack area
	ldr	sp, =StartupEntry_stk

// setup JTAG pin
	ldr	r1, =GPFSEL0
	ldr	r0, [r1]
	bic	r0, r0, #7 << (3*4)	// GPIO04 clear
	orr	r0, r0, #2 << (3*4)	// GPIO04 set ALT5(ARM_TDI)
	str	r0, [r1]

	ldr	r1, =GPFSEL2
	ldr	r0, [r1]
	bic	r0, r0, #7 << (3*2)	// GPIO22 clear
	orr	r0, r0, #3 << (3*2)	// GPIO22 set ALT4(ARM_TRST)
	bic	r0, r0, #7 << (3*4)	// GPIO24 clear
	orr	r0, r0, #3 << (3*4)	// GPIO24 set ALT4(ARM_TDO)
	bic	r0, r0, #7 << (3*5)	// GPIO25 clear
	orr	r0, r0, #3 << (3*5)	// GPIO25 set ALT4(ARM_TCK)
	bic	r0, r0, #7 << (3*7)	// GPIO27 clear
	orr	r0, r0, #3 << (3*7)	// GPIO27 set ALT4(ARM_TMS)
	str	r0, [r1]

// setup LED pin
#if TYPE_RPI < 3			/*** RPI1 or RPI2 ? ***/
	/* PWR Red LED (GPIO35) */
	ldr	r1, =GPFSEL3
	ldr	r0, [r1]
	bic	r0, r0, #7 << (3*5)	// GPIO35 clear
	orr	r0, r0, #1 << (3*5)	// GPIO35 set OUTPUT
	str	r0, [r1]

	/* ACT Green LED (GPIO47) */
	ldr	r1, =GPFSEL4
	ldr	r0, [r1]
	bic	r0, r0, #7 << (3*7)	// GPIO47 clear
	orr	r0, r0, #1 << (3*7)	// GPIO47 set OUTPUT
	str	r0, [r1]
#else					/*** RPI3 ***/
	/* GPIO17(pin11) */
	ldr	r1, =GPFSEL1
	ldr	r0, [r1]
	bic	r0, r0, #7 << (3*7)	// GPIO17 clear
	orr	r0, r0, #1 << (3*7)	// GPIO17 set OUTPUT
	str	r0, [r1]
#endif

#ifdef __DEBUG_BUILD__	// for debug (test pin GPIO21 initialize)
	ldr	r1, =GPFSEL2
	ldr	r0, [r1]
	bic	r0, r0, #7 << (3*1)	// GPIO21 clear
	orr	r0, r0, #1 << (3*1)	// GPIO21 set OUTPUT
	str	r0, [r1]

# if 0
debugdebug_loop:
	ldr	r1, =GPSET0
	mov	r0, #1 << 21		// GPIO21 set
	str	r0, [r1]

	ldr	r1, =GPCLR0
	mov	r0, #1 << 21		// GPIO21 clear
	str	r0, [r1]
	b	debugdebug_loop		// 無限ループ
#endif
#endif

// not in effect: MMU, cache (D/I), program-flow prediction, High-Vector, VIC
// in effect: Force AP, TEX remap, Subpage AP
	.DSB	r0
	mrc	p15, 0, r0, cr1, cr0, 0	// c1,0,c0,0:SCTLR read
#ifdef CPU_ARMv6
	ldr	r1, =~0x01003f85
	and	r0, r0, r1
	ldr	r1, =0x30800000
	orr	r0, r0, r1
#else	/* CPU_ARMv7以降 */
	ldr	r1, =~0x00003c05
	and	r0, r0, r1
	ldr	r1, =0x30000000
	orr	r0, r0, r1
#endif
	mcr	p15, 0, r0, cr1, cr0, 0	// write SCTLR

// creation of temporary page table
	ldr	r0, =PAGETBL_BASE
	ldr	r1, =0x00000000
	ldr	r2, =0x00000402	// Kernel/RW, Strongly-order
tmptable_loop:
	orr	r3, r1, r2
	str	r3, [r0], #4
	adds	r1, r1, #0x00100000
	bne	tmptable_loop

#if 1
// 先頭の1Mセクション(0x00000000 - 0x00100000)は、モニタコード実行領域

	ldr	r0, =PAGETBL_BASE
	ldr	r1, =0x00000000
	ldr	r2, =0x0000940e	// Kernel/RO, Normal WB/WA
	orr	r3, r1, r2
	str	r3, [r0], #4
#else
// Mapping of FlashROM area (0x70000000 - 0x72000000 --> 0x00000000 -)
	ldr	r0, =(PAGETBL_BASE + (0x700 << 2))
	ldr	r1, =0x00000000
	ldr	r2, =0x0000940e	// Kernel/RO, Normal WB/WA
flashtable_loop:
	orr	r3, r1, r2
	str	r3, [r0], #4
	adds	r1, r1, #0x00100000
	cmp	r1, #0x02000000
	bne	flashtable_loop
#endif

// initialization of CP15
	ldr	r0, =0x00000004
	mcr	p15, 0, r0, cr2, cr0, 2		// TTBCR
	ldr	r0, =(PAGETBL_BASE + 0x09)	// WB/WA, no-shared, cachable
	mcr	p15, 0, r0, cr2, cr0, 1		// TTBR1
	mcr	p15, 0, r0, cr2, cr0, 0		// TTBR0
	ldr	r0, =EITENT_BASE
	mcr	p15, 0, r0, cr12, cr0, 0	// VBAR
	ldr	r0, =0x000a8aa4
	mcr	p15, 0, r0, cr10, cr2, 0 	// PRRR
	ldr	r0, =0x44e048e0
	mcr	p15, 0, r0, cr10, cr2, 1	// NMRR
	ldr	r0, =0x55555555			// All client
	mcr	p15, 0, r0, cr3, cr0, 0		// Domain access

// MMU enable
	.DSB	r0
	mcr	p15, 0, r0, cr8, cr7, 0		// I/D TLB invalidate(TLBIALL)
	mcr	p15, 0, r0, cr7, cr5, 6		// invalidate BTC(BPIALL)
	.DSB	r0
	.ISB	r0
	mrc	p15, 0, r0, cr1, cr0, 0
	orr	r0, r0, #0x00000001		// SCTLR, MMU on
	mcr	p15, 0, r0, cr1, cr0, 0
	.ISB	r0

// perform reset processing
	ldr	pc, =reset_entry

	.pool

/*----------------------------------------------------------------------
        Reset processing
----------------------------------------------------------------------*/
	.text
	.balign	4
	.globl	reset_entry
	.type	reset_entry, %function
reset_entry:

// SVC mode, FIQ/IRQ interrupt disabled
	mov	r0, #(PSR_SVC | PSR_I | PSR_F)
	msr	cpsr_fsxc, r0

// Switch to T-Monitor stack
	ldr	sp, =__stack_bottom

// not in effect: cache (D/I), program-flow prediction, High-Vector, VIC
// in effect: Force AP, TEX remap, Subpage AP
	.DSB	r0
	mrc	p15, 0, r0, cr1, cr0, 0	// c1,0,c0,0:SCTLR read
#ifdef CPU_ARMv6
	ldr	r1, =~0x01003f84
	and	r0, r0, r1
	ldr	r1, =0x30800000
	orr	r0, r0, r1
#else	/* CPU_ARMv7以降 */
	ldr	r1, =~0x00003c04
	and	r0, r0, r1
	ldr	r1, =0x30000000
	orr	r0, r0, r1
#endif
	mcr	p15, 0, r0, cr1, cr0, 0	// SCTLR write

	.ISB	r0
	mcr	p15, 0, r0, cr8, cr7, 0	// I/D TLB invalidate(TLBIALL)
	.DSB	r0
	bl	Csym(FlushCache)	// Clean/invalidate I/D cache

// Turn on VFP
	mrc	p15, 0, r0, cr1, cr0, 2	// c1,0,c0,2:CPACR read
	orr	r0, r0, #0x00f00000	// VFP(CP11,CP10) enable
	bic	r0, r0, #0xc0000000	// Should be Zero
	mcr	p15, 0, r0, cr1, cr0, 2
	.ISB	r0			// Flush Prefetch buffer

// initialize monitor debug work area
#ifdef __MonDebugWK
	ldr	r2, =__MonDebugWK
	ldr	r3, =StartupEntry_stk
	ldr	r4, =0
	ldr	r5, =0
	ldr	r6, =0
	ldr	r7, =0
debugwk_loop:
	stmia	r2!, {r4-r7}	// clear in units of 16 bytes
	cmp	r2, r3
	blo	debugwk_loop

	/* CTR キャッシュタイプ情報チェック */
	ldr	r2, =__MonDebugWK
	mrc	p15, 0, r1, cr0, cr0, 1	// CTR read
	str	r1, [r2, #__INFO_CTR]

  #if defined(CPU_ARMv7)
	/* CCSIDR キャッシュ情報チェック */
	mov	r0, #0x2		// キャッシュ選択レベル1
	mcr	p15, 2, r0, cr0, cr0, 0	// write CSSELR
	mrc	p15, 1, r1, cr0, cr0, 0	// CCSIDR read
	str	r1, [r2, #__INFO_CCSIDR1]

	mov	r0, #0x0		// キャッシュ選択レベル0
	mcr	p15, 2, r0, cr0, cr0, 0	// write CSSELR
	mrc	p15, 1, r1, cr0, cr0, 0	// CCSIDR read
	str	r1, [r2, #__INFO_CCSIDR0]
  #endif
#endif

// initialize data area
	ldr	r1, =__data_org
	ldr	r2, =__data_start
	ldr	r3, =_edata
data_loop:
	ldmia	r1!, {r4-r7}	// copy in unit of 16 bytes
	stmia	r2!, {r4-r7}
	cmp	r2, r3
	blo	data_loop

// clear bss and stack area
	ldr	r2, =__bss_start
	ldr	r3, =__stack_bottom
	ldr	r4, =0
	ldr	r5, =0
	ldr	r6, =0
	ldr	r7, =0
bss_loop:
	stmia	r2!, {r4-r7}	// clear in units of 16 bytes
	cmp	r2, r3
	blo	bss_loop

// reset processing
	bl	procReset		// see "startup.c"

// clear registers & initialize stack pointer
	ldr	r7, =__stack_top	// since it is 0-cleared, why not use it.

	ldmia	r7, {r8-r12,sp,lr}^	// usr: r8-r12, sp, lr

	mov	r0, #(PSR_FIQ | PSR_I | PSR_F)
	msr	cpsr_fsxc, r0
	ldmia	r7, {r8-r12,sp,lr}	// fiq: r8-r12, sp, lr, spsr
	msr	spsr_fsxc, lr
	ldr	sp, =__stack_top + 32

	mov	r0, #(PSR_IRQ | PSR_I | PSR_F)
	msr	cpsr_fsxc, r0
	ldr	sp, =__stack_top + 16	// irq: sp, lr, spsr
	mov	lr, #0
	msr	spsr_fsxc, lr

	mov	r0, #(PSR_ABT | PSR_I | PSR_F)
	msr	cpsr_fsxc, r0
	ldr	sp, =__stack_top + 64	// abt: sp, lr, spsr
	mov	lr, #0
	msr	spsr_fsxc, lr

	mov	r0, #(PSR_UND | PSR_I | PSR_F)
	msr	cpsr_fsxc, R0
	ldr	sp, =__stack_top + 48	// und: sp, lr, spsr
	mov	lr, #0
	msr	spsr_fsxc, lr

        // clear VFP
	mov	r0, #0x40000000		// EX=0,EN=1,SBZ/other flags = 0
	fmxr	fpexc, r0

	mov	r1, #0x00000000		// SBZ/other flags = 0
	fmxr	fpscr, r1

	fldmiad	r7, {d0-d15}		// zero clear

        // return to SVC mode
	mov	r0, #(PSR_SVC | PSR_I | PSR_F)
	msr	cpsr_fsxc, r0

	ldmia	r7, {r0-r7}		// r0-r7

// enter monitor by means of SVC #0 instruction (SVC mode)
  resetLoop:
	svc	#0
	b	resetLoop		// return will cause another reset

	.pool


/*----------------------------------------------------------------------
#|History of "reset.S"
#|====================
#|* 2016/03/02	It's made by making reference to "reset.S" for [tef_em1d].
#|* 2016/04/05	JTAG pin の初期化処理を追加
#|    GPIO22(ALT4):ARM_TRST, GPIO04(ALT5):ARM_TDI, GPIO27(ALT4):ARM_TMS,
#|    GPIO25(ALT4):ARM_TCK, GPIO24(ALT4):ARM_TDO
#|* 2016/04/16	UART pin の初期化処理を追加
#|    GPIO14(ALT0):TXD, GPIO15(ALT0):RXD
#|* 2016/04/18	LED pin の初期化処理を追加
#|    GPIO35(OUTPUT):PWR, GPIO47(OUTPUT):ACT
#|* 2016/04/18	CPU_ARMv7以降の時は、ISB,DSB,DMB命令を使うように修正。
#|* 2016/04/19	CPU_BCM2835以外では、Hypﾓｰﾄﾞの時はSVCﾓｰﾄﾞに切り換える処理追加
#|  (Raspberry Pi2では、ﾌﾞｰﾄローダから飛んできた時にHypモードになってる対策）
#|* 2016/08/29	TYPE_RPI=3の時、GPIO17をLED用に出力に初期化。
#|
*/
