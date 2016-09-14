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
 *
 *    Modified by T.Yokobayashi at 2015/12/14.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cpu_support.S (SH7760)
 *	Device-Dependent CPU Operation
 */

#define	_in_asm_source_

#include <machine.h>
#include <tk/errno.h>
#include <tk/sysdef.h>
#include <tk/asm.h>
#include <sys/sysinfo.h>

#include "config.h"
#include "cpu_conf.h"
#include "isysconf.h"
#include "tkdev_conf.h"
#include "offset.h"

#define	MMU_RegBase	0xff000000	/* Head of MMU-related register */

/* ------------------------------------------------------------------------ */
/*
 * Dispatcher
 *	dispatch_to_schedtsk:
 *		Throw away the current contexts and forcibly dispatch to
 *		'schedtsk.'
 *		Called directly by jump (jmp) but do not return.
 *		Called on the undefined stack state (undefined 'ssp').
 *		Called on the interrupt disable state.
 *	dispatch_entry:
 *		Normal dispatch processing.
 *		Called by 'trapa TRAP_DISPATCH.'
 *	_ret_int_dispatch:
 *		Called when dispatch is required by 'ret_int().'
 *
 *	Contexts to save
 *	Save registers except for SSP to a stack. Save SSP to TCB.
 *
 *		   High Address	+---------------+
 *				| USP		| Only RNG 1-3 
 *				+---------------+
 *				| MDR (R2_BANK1)| RNG0-3 shared
 *				| SSR		|
 *				| SPC		| Saved by interrupt entry
 *				| R0_BANK1	| routines until here.
 *				+---------------+
 *				| R1_BANK1	|
 *				| R8-14		|
 *				| R0-7_BANK0	|
 *				| GBR		|
 *				| PR		|
 *				| MACL		|
 *	Saved to TCB  SSP =>	| MACH		|
 *		    Low Address	+---------------+
 *
 */

	/* Temporal stack used when 'dispatch_to_schedtsk' is called
	*/
#define	TMP_STACK_SZ	(4*1024)
#define	TMP_STACK_TOP	(tmp_stack + TMP_STACK_SZ)
	.lcomm	tmp_stack, TMP_STACK_SZ

	.text
	.balign	2
	.globl	Csym(dispatch_to_schedtsk)
	.type	Csym(dispatch_to_schedtsk), @function
	.globl	Csym(dispatch_entry)
	.type	Csym(dispatch_entry), @function

Csym(dispatch_to_schedtsk):
	/* During interrupt disable SR.I=15 BL=0 RB=0 */
	mov.l	L_tmp_stack_top, SP		// Set temporal stack

	mov.l	L_dispatch_disabled, r0		// Dispatch disable
	mov	#1, r1
	mov.l	r1, @r0

	mov.l	L_ctxtsk, r10			// R10 = &ctxtsk
	mov	#0, r0
#if USE_DBGSPT
	mov.l	@r10, r8
#endif
	mov.l	r0, @r10			// ctxtsk = NULL

	mov.l	L_EnableIntSR, r0		// Interrupt enable, Register bank switch
	ldc	r0, sr
	bra	l_dispatch0
	nop

Csym(dispatch_entry):
	/* During interrupt disable SR.I=15 BL=1 RB=1 */
	mov.l	@(2*4, ISP), r0
	or	#SR_I(15), r0
	xor	#SR_I(15), r0
	mov.l	r0, @(2*4, ISP)			// SSR compensation (I=0)

	mov.l	L_DisableIntSR, r0		// Free exception block
	ldc	r0, sr

_ret_int_dispatch:
	/* During interrupt disable SR.I=15 BL=0 RB=1 */

	/* Move from interrupt stack to system stack */
	MOVE_ISP_SSP dispatch
	mov.l	r1, @-SP			// R1_BANK1 save

	mov.l	L_dispatch_disabled, r0		// Dispatch disable
	mov	#1, r1
	mov.l	r1, @r0
	mov.l	L_EnableIntSR, r0		// Interrupt enable
	ldc	r0, sr

	mov.l	r14, @-SP			// Context save
	mov.l	r13, @-SP
	mov.l	r12, @-SP
	mov.l	r11, @-SP
	mov.l	r10, @-SP
	mov.l	r9,  @-SP
	mov.l	r8,  @-SP
	stc.l	r7_bank, @-SP
	stc.l	r6_bank, @-SP
	stc.l	r5_bank, @-SP
	stc.l	r4_bank, @-SP
	stc.l	r3_bank, @-SP
	stc.l	r2_bank, @-SP
	stc.l	r1_bank, @-SP
	stc.l	r0_bank, @-SP
	stc.l	gbr,  @-SP
	sts.l	pr,   @-SP
	sts.l	macl, @-SP
	sts.l	mach, @-SP

	mov.l	L_ctxtsk, r10			// R10 = &ctxtsk
	mov.l	@r10, r8
	mov.l	L_tskctxb_ssp, r0
	mov.l	SP, @(r0, r8)			// Save SSP to TCB

  l_dispatch0:
	/* During interrupt enable SR.I=0 BL=0 RB=1 */
#if USE_DBGSPT
	mov.l	L_hook_stop_jmp, r0		// Hook processing
	mov.l	@r0, r0
	jmp	@r0
	nop
  ret_hook_stop:
#endif

	mov	#0, r0
	mov.l	r0, @r10			// ctxtsk = NULL

	mov.l	L_schedtsk, r11			// R11 = &schedtsk
	mov.l	L_lowpow_discnt, r12		// R12 = &lowpow_discnt
	mov.l	L_low_pow, r13			// R13 = &low_pow()

  l_dispatch1:
#ifdef WDT_DOG_FOOD_ADDR
	mov.l	L_WDT_DOG_FOOD_ADDR, r0		// see "sys/machine_depend.h"
	mov.b	@r0, r0				// WDTﾄﾘｶﾞ
#endif
	mov.l	L_DisableIntSR, r0		// Interrupt disable
	ldc	r0, sr

	mov.l	@r11, r8			// R8 = schedtsk
	tst	r8, r8				// Is there 'schedtsk'?
	bf	l_dispatch2

	/* Because there is no task that should be executed,
	   move to the power saving mode */
	mov.l	@r12, r0			// Is 'low_pow' disabled?
	tst	r0, r0
	bf	l_dispatch11
	mov.l	L_low_pow_SR, r0		// SR setting value for 'low_pow' call
	ldc	r0, sr
	jsr	@r13				// call low_pow()
	nop
  l_dispatch11:

	mov.l	L_EnableIntSR, r0		// Interrupt enable
	ldc	r0, sr
	bra	l_dispatch1
	nop

  l_dispatch2:					// Switch to 'schedtsk'
	/* During interrupt disable SR.I=15 BL=0 RB=1 SH3:DSP=1 */
	mov.l	r8, @r10			// ctxtsk = schedtsk
	mov.l	L_tskctxb_ssp, r0
	mov.l	@(r0, r8), SP			// Restore SSP from TCB
	mov.l	L_tcb_isstack, r0
	mov.l	@(r0, r8), SST			// Restore SST from TCB

	/* Switch Task Space */
	mov.l	L_MMU_RegBase, r12
	mov.l	L_tskctxb_uatb, r0		// TTB = uatb
	mov.l	@(r0, r8), r9
	mov.l	r9, @(TTB - MMU_RegBase, r12)
	mov.l	L_tskctxb_lsid, r0		// PTEH = lsid
	mov.l	@(r0, r8), r9
	mov.l	r9, @(PTEH - MMU_RegBase, r12)

#if USE_DBGSPT
	mov.l	L_hook_exec_jmp, r0		// Hook processing
	mov.l	@r0, r0
	jmp	@r0
	nop
  ret_hook_exec:
#endif

#if TA_FPU
	/* Switch FPU context */
	mov.l	L_fpu_ctxtsk, r12		// R12 = &fpu_ctxtsk
	mov.l	@r12, r9			// R9 = fpu_ctxtsk;
	cmp/eq	r8, r9				// If fpu_ctxtsk == ctxtsk
	bt/s	l_same_fpuctx			// Do not need to switch FPU context

	mov	#22*4+1, r0
	ldc	SP, gbr
	or.b	#SR_FD >> 8, @(r0, gbr)		// SSR.FD = 1 on stack

  l_same_fpuctx:
#endif /* TA_FPU */

#if USE_SH3_DSP
	/* Switch DSP context */
	mov.l	L_tcb_tskatr, r0		// Use DSP?
	mov.l	L_TA_COP0, r13
	mov.l	@(r0, r8), r0
	tst	r0, r13
	bt	l_noswdsp

	mov.l	L_dsp_ctxtsk, r12		// R12 = &dsp_ctxtsk
	mov.l	@r12, r9			// R9 = dsp_ctxtsk
	cmp/eq	r8, r9				// If fpu_ctxtsk == ctxtsk,
	bt	l_noswdsp			// do not need to switch DSP context

	mov	r4, r13				// R4_BANK1 save
	tst	r9, r9				// If dsp_ctxtsk == NULL,
	bt	l_nosavedsp			// do not need to save DSP context

	mov.l	L_tcb_isstack, r0		// DSP context save 
	mov.l	@(r0, r9), r4
	add	#COP0_REGSZ, r4
	stc.l	mod, @-r4
	stc.l	re,  @-r4
	stc.l	rs,  @-r4
	movs.l	y1,  @-r4
	movs.l	y0,  @-r4
	movs.l	x1,  @-r4
	movs.l	x0,  @-r4
	movs.l	m1,  @-r4
	movs.l	m0,  @-r4
	movs.w	a1g, @-r4
	movs.w	a0g, @-r4
	movs.l	a1,  @-r4
	movs.l	a0,  @-r4
	sts.l	dsr, @-r4

  l_nosavedsp:
	mov	SST, r4				// DSP context restore 
	lds.l	@r4+, dsr
	movs.l	@r4+, a0
	movs.l	@r4+, a1
	movs.w	@r4+, a0g
	movs.w	@r4+, a1g
	movs.l	@r4+, m0
	movs.l	@r4+, m1
	movs.l	@r4+, x0
	movs.l	@r4+, x1
	movs.l	@r4+, y0
	movs.l	@r4+, y1
	ldc.l	@r4+, rs
	ldc.l	@r4+, re
	ldc.l	@r4+, mod

	mov.l	r8, @r12			// dsp_ctxtsk = ctxtsk
	mov	r13, r4				// R4_BANK1 restore

  l_noswdsp:
#endif /* USE_SH3_DSP */

#if !USE_MDR_DCT
	mov.l	@(TCB_reqdct, r8), r0		// DCT request
#endif

	lds.l	@SP+, mach			// Context restore 
	lds.l	@SP+, macl
	lds.l	@SP+, pr
	ldc.l	@SP+, gbr
	ldc.l	@SP+, r0_bank
	ldc.l	@SP+, r1_bank
	ldc.l	@SP+, r2_bank
	ldc.l	@SP+, r3_bank
	ldc.l	@SP+, r4_bank
	ldc.l	@SP+, r5_bank
	ldc.l	@SP+, r6_bank
	ldc.l	@SP+, r7_bank
	mov.l	@SP+, r8
	mov.l	@SP+, r9
	mov.l	@SP+, r10
	mov.l	@SP+, r11
	mov.l	@SP+, r12
	mov.l	@SP+, r13
	mov.l	@SP+, r14

#if !USE_MDR_DCT
	cmp/eq	#1, r0				// If DCT request is enabled, SR.T=1
#endif

	mov.l	L_dispatch_disabled, r0		// Dispatch enable 
	mov	#0, r1
	mov.l	r1, @r0

#if !USE_MDR_DCT
	bf	l_nodct_dispatch

	mov.l	@(4*4, SP), r0			// MDR
	tst	#MDR_CPL(3), r0			// If the protected level is 0, DCT disable
	bt	l_nodct_dispatch

	bra	dct_startup			// To DCT processing
	mov.l	@SP+, r1			// R1_BANK1 restore

  l_nodct_dispatch:
#endif
	mov.l	@SP+, r1			// R1_BANK1 restore
	/* Move from system stack to interrupt stack */
	MOVE_SSP_ISP dispatch

	mov.l	L_BlockIntSR, r0		// Exception block
	ldc	r0, sr
	INT_RETURN dispatch

			.balign	4
  L_tmp_stack_top:	.long	TMP_STACK_TOP

  L_EnableIntSR:	.long	SR_MD | SR_FD | SR_RB | SR_I(0)
  L_DisableIntSR:	.long	SR_MD | SR_FD | SR_RB | SR_I(15)
  L_BlockIntSR:		.long	SR_MD | SR_FD | SR_RB | SR_I(15) | SR_BL
  L_low_pow_SR:		.long	SR_MD | SR_FD | SR_I(15)

  L_dispatch_disabled:	.long	Csym(dispatch_disabled)
  L_schedtsk:		.long	Csym(schedtsk)
  L_ctxtsk:		.long	Csym(ctxtsk)
  L_tskctxb_ssp:	.long	TCB_tskctxb + CTXB_ssp
  L_tskctxb_uatb:	.long	TCB_tskctxb + CTXB_uatb
  L_tskctxb_lsid:	.long	TCB_tskctxb + CTXB_lsid
  L_tcb_isstack:	.long	TCB_isstack
  L_MMU_RegBase:	.long	MMU_RegBase

  L_lowpow_discnt:	.long	Csym(lowpow_discnt)
  L_low_pow:		.long	Csym(low_pow)

#if USE_SH3_DSP
  L_dsp_ctxtsk:		.long	Csym(dsp_ctxtsk)
  L_tcb_tskatr:		.long	TCB_tskatr
  L_TA_COP0:		.long	TA_COP0
#endif
#if TA_FPU
  L_fpu_ctxtsk:		.long	Csym(fpu_ctxtsk)
#endif

#ifdef WDT_DOG_FOOD_ADDR
  L_WDT_DOG_FOOD_ADDR:	.long	WDT_DOG_FOOD_ADDR
#endif


#if USE_DBGSPT
/*
 * Task dispatcher hook routine call
 *	void stop( ID tskid, INT lsid, UINT tskstat )
 *	void exec( ID tskid, INT lsid )
 */
	.text
	.balign	2
hook_stop:
	tst	r8, r8			// r8 = ctxtsk
	bt	l_notask

	mov.l	L_hook_SR, r0		// SR setting value for hook call
	stc	sr, r9			// SR save
	ldc	r0, sr
	/* SR.I=0 BL=0 RB=0 */

	mov.l	L_hook_stopfn, r1
	mov.l	@r1, r1

	mov.l	L_tcb_state, r0
	mov.b	@(r0, r8), r6
	shll	r6			// tskstat
	mov.l	L_tskctxb_lsid2, r0
	mov.l	@(r0, r8), r5		// lsid
	mov.l	L_tcb_tskid, r0
	jsr	@r1			// call stop(tskid, lsid, tskstat)
	mov.l	@(r0, r8), r4		// tskid

	ldc	r9, sr			// SR restore

  l_notask:
	bra	ret_hook_stop
	nop

hook_exec:
	mov.l	L_hook_SR, r0		// SR setting value for hook call
	stc	sr, r9			// SR save
	ldc	r0, sr
	/* SR.I=0 BL=0 RB=0 */

	mov.l	L_hook_execfn, r1
	mov.l	@r1, r1

	mov.l	L_tskctxb_lsid2, r0	// r8 = ctxtsk
	mov.l	@(r0, r8), r5		// lsid
	mov.l	L_tcb_tskid, r0
	jsr	@r1			// call exec(tskid, lsid)
	mov.l	@(r0, r8), r4		// tskid

	ldc	r9, sr			// SR restore
	bra	ret_hook_exec
	nop

/*
 * Set/Free task dispatcher hook routine
 */
	.text
	.balign	2
	.globl	Csym(hook_dsp)
	.type	Csym(hook_dsp), @function
Csym(hook_dsp):
	mov.l	L_hook_exec_jmp, r0
	mov.l	L_hook_stop_jmp, r1
	mov.l	L_hook_exec, r2
	mov.l	L_hook_stop, r3
	mov.l	r2, @r0
	rts
	mov.l	r3, @r1

	.globl	Csym(unhook_dsp)
	.type	Csym(unhook_dsp), @function
Csym(unhook_dsp):
	mov.l	L_hook_exec_jmp, r0
	mov.l	L_hook_stop_jmp, r1
	mov.l	L_ret_hook_exec, r2
	mov.l	L_ret_hook_stop, r3
	mov.l	r2, @r0
	rts
	mov.l	r3, @r1

			.balign	4
  L_hook_SR:		.long	SR_MD | SR_I(0) | SR_FD
  L_tcb_tskid:		.long	TCB_tskid
  L_tcb_state:		.long	TCB_state
  L_tskctxb_lsid2:	.long	TCB_tskctxb + CTXB_lsid

  L_hook_exec_jmp:	.long	hook_exec_jmp
  L_hook_stop_jmp:	.long	hook_stop_jmp

  L_hook_exec:		.long	hook_exec
  L_hook_stop:		.long	hook_stop
  L_ret_hook_exec:	.long	ret_hook_exec
  L_ret_hook_stop:	.long	ret_hook_stop

  L_hook_execfn:	.long	Csym(hook_execfn)
  L_hook_stopfn:	.long	Csym(hook_stopfn)

			.data
			.balign	4
  hook_exec_jmp:	.long	ret_hook_exec
  hook_stop_jmp:	.long	ret_hook_stop

#endif /* USE_DBGSPT */

/* ------------------------------------------------------------------------ */
#if USE_SH3_DSP
/*
 * Save DSP context
 *	void save_dspctx( T_COP0REGS *save_area )
 */
	.text
	.balign	2
	.globl	Csym(save_dspctx)
	.type	Csym(save_dspctx), @function
Csym(save_dspctx):
	stc	sr, r0			// SR save
	mov.l	S_SR_DSP, r1
	or	r0, r1
	ldc	r1, sr			// SR.DSP = 1

	add	#COP0_REGSZ, r4
	stc.l	mod, @-r4		// DSP context save 
	stc.l	re,  @-r4
	stc.l	rs,  @-r4
	movs.l	y1,  @-r4
	movs.l	y0,  @-r4
	movs.l	x1,  @-r4
	movs.l	x0,  @-r4
	movs.l	m1,  @-r4
	movs.l	m0,  @-r4
	movs.w	a1g, @-r4
	movs.w	a0g, @-r4
	movs.l	a1,  @-r4
	movs.l	a0,  @-r4
	sts.l	dsr, @-r4

	ldc	r0, sr			// SR restore
	rts
	nop

			.balign	4
  S_SR_DSP:		.long	SR_DSP
#endif /* USE_SH3_DSP */
/* ------------------------------------------------------------------------ */
#if TA_FPU
/*
 * FPU suppress exception
 *	FPU context switch
 *	   High Address	+---------------+
 *			| USP		| Only RNG 1-3 
 *			+---------------+
 *			| MDR (R2_BANK1)| RNG0-3 shared
 *			| SSR		|
 *			| SPC		|
 *		ISP =>	| R0_BANK1	|
 *	    Low Address	+---------------+
 */
	.text
	.balign	2
	.globl	Csym(unavailable_cop)
	.type	Csym(unavailable_cop), @function
Csym(unavailable_cop):
	/* During interrupt disable SR.I=15 BL=1 RB=1 */
	mov.l	r4, @-ISP		// R4_BANK1 save

	mov.l	U_DisableIntSR, r7	// Free exception block/Enable FPU
	ldc	r7, sr			// SR.BL=0 RB=1 FD=0

	mov.l	r8, @-SP		// Save register for work
	mov.l	r9, @-SP

	mov.l	U_ctxtsk, r8
	mov.l	@r8, r8			// R8 = ctxtsk
	tst	r8, r8			// If ctxtsk == NULL, invalid use of FPU
	bt	l_goto_defaulthdr
	mov.l	@(TCB_tskatr, r8), r9
	mov.l	U_TA_FPU, r4		// If TA_FPU attribute is not specified
	tst	r4, r9			// Invalid use of FPU
	bt	l_goto_defaulthdr
	/* Do not destroy R0_BANK1 until here */

	mov.l	U_fpu_ctxtsk, r9	// R9 = &fpu_ctxtsk
	mov.l	@r9, r4			// R4 = fpu_ctxtsk
	cmp/eq	r4, r8			// If fpu_ctxtsk == ctxtsk,
	bt	l_nosw_fpu		// do not need to switch FPU context
	tst	r4, r4			// If fpu_ctxtsk == NULL,
	bt	l_nosave_fpu		// do not need to save FPU context

	mov.l	U_tcb_isstack, r0	// Save FPU context
	mov.l	@(r0, r4), r4		// Location for saving R4 = FPU context
	mov.l	U_COP0_REGSZ, r0
	add	r0, r4
	sts.l	fpscr, @-r4
	sts.l	fpul, @-r4
	mov.l	U_FPSCR_FR0_SZ1, r0
	lds	r0, fpscr		// FPSCR.FR=0 SZ=1
	fmov	xd14, @-r4
	fmov	xd12, @-r4
	fmov	xd10, @-r4
	fmov	xd8,  @-r4
	fmov	xd6,  @-r4
	fmov	xd4,  @-r4
	fmov	xd2,  @-r4
	fmov	xd0,  @-r4
	fmov	dr14, @-r4
	fmov	dr12, @-r4
	fmov	dr10, @-r4
	fmov	dr8,  @-r4
	fmov	dr6,  @-r4
	fmov	dr4,  @-r4
	fmov	dr2,  @-r4
	fmov	dr0,  @-r4

  l_nosave_fpu:
	mov.l	U_tcb_isstack, r0	// Restore FPU context
	mov.l	@(r0, r8), r4		// Location for R4 = FPU context
	mov.l	U_FPSCR_FR0_SZ1, r0
	lds	r0, fpscr		// FPSCR.FR=0 SZ=1
	fmov	@r4+, dr0
	fmov	@r4+, dr2
	fmov	@r4+, dr4
	fmov	@r4+, dr6
	fmov	@r4+, dr8
	fmov	@r4+, dr10
	fmov	@r4+, dr12
	fmov	@r4+, dr14
	fmov	@r4+, xd0
	fmov	@r4+, xd2
	fmov	@r4+, xd4
	fmov	@r4+, xd6
	fmov	@r4+, xd8
	fmov	@r4+, xd10
	fmov	@r4+, xd12
	fmov	@r4+, xd14
	lds.l	@r4+, fpul
	lds.l	@r4+, fpscr

	mov.l	r8, @r9			// fpu_ctxtsk = ctxtsk

  l_nosw_fpu:
	mov.l	@(3*4, ISP), r4
	mov.l	U_NOT_SR_FD, r0
	and	r0, r4			// FPU enable
	mov.l	r4, @(3*4, ISP)		// SSR.FD = 0 on stack

	mov.l	@SP+, r9		// Restore register for work
	mov.l	@SP+, r8

	mov.l	U_BlockIntSR, r0	// Exception block
	ldc	r0, sr
	mov.l	@ISP+, r4		// R4_BANK1 restore
	INT_RETURN unavailable_cop

  l_goto_defaulthdr:			// R0_BANK1 is when entering handler 
	mov.l	@(3*4, ISP), r4		// Same state
	mov.l	U_BlockIntSR, r8
	or	r8, r4			// Value for R4 = SR restorer

	mov.l	@SP+, r9		// Restore register for work
	mov.l	@SP+, r8

	ldc	r4, sr			// SR restore
	mov.l	@ISP+, r4		// R4_BANK1 restore

	mov.l	U_DEFAULTHDR, r7
	mov.l	@r7, r7
	jmp	@r7			// To default handler
	nop

/*
 * Save FPU context
 *	void save_fpuctx( T_COP0REGS *save_area )
 *
 *	'save_area' is aligned by 4 bytes.
 *	Note that this is not to be accessed by double (8 bytes).
 */
	.text
	.balign 2
	.globl	Csym(save_fpuctx)
	.type	Csym(save_fpuctx), @function
Csym(save_fpuctx):
	mov.l	U_NOT_SR_FD, r0
	stc	sr, r1			// R1 = SR save
	and	r1, r0
	ldc	r0, sr			// SR.FD=0  FPU enable
	sts	fpscr, r2		// R2 = FPSCR save

	mov.l	U_COP0_REGSZ, r0
	add	r0, r4
	sts.l	fpscr, @-r4
	sts.l	fpul, @-r4
	mov.l	U_FPSCR_FR1_SZ0, r0
	lds	r0, fpscr		// FPSCR.FR=1 SZ=0
	fmov.s	fr15, @-r4
	fmov.s	fr14, @-r4
	fmov.s	fr13, @-r4
	fmov.s	fr12, @-r4
	fmov.s	fr11, @-r4
	fmov.s	fr10, @-r4
	fmov.s	fr9,  @-r4
	fmov.s	fr8,  @-r4
	fmov.s	fr7,  @-r4
	fmov.s	fr6,  @-r4
	fmov.s	fr5,  @-r4
	fmov.s	fr4,  @-r4
	fmov.s	fr3,  @-r4
	fmov.s	fr2,  @-r4
	fmov.s	fr1,  @-r4
	fmov.s	fr0,  @-r4
	frchg				// FPSCR.FR=0
	fmov.s	fr15, @-r4
	fmov.s	fr14, @-r4
	fmov.s	fr13, @-r4
	fmov.s	fr12, @-r4
	fmov.s	fr11, @-r4
	fmov.s	fr10, @-r4
	fmov.s	fr9,  @-r4
	fmov.s	fr8,  @-r4
	fmov.s	fr7,  @-r4
	fmov.s	fr6,  @-r4
	fmov.s	fr5,  @-r4
	fmov.s	fr4,  @-r4
	fmov.s	fr3,  @-r4
	fmov.s	fr2,  @-r4
	fmov.s	fr1,  @-r4
	fmov.s	fr0,  @-r4

	lds	r2, fpscr		// FPSCR restore
	ldc	r1, sr			// SR restore
	rts
	nop

			.balign	4
  U_DisableIntSR:	.long	SR_MD | SR_RB | SR_I(15)
  U_BlockIntSR:		.long	SR_MD | SR_RB | SR_BL | SR_FD | SR_I(15)
  U_NOT_SR_FD:		.long	~SR_FD
  U_FPSCR_FR0_SZ1:	.long	FPSCR_SZ
  U_FPSCR_FR1_SZ0:	.long	FPSCR_FR
  U_TA_FPU:		.long	TA_FPU
  U_ctxtsk:		.long	Csym(ctxtsk)
  U_fpu_ctxtsk:		.long	Csym(fpu_ctxtsk)
  U_tcb_isstack:	.long	TCB_isstack
  U_COP0_REGSZ:		.long	COP0_REGSZ
  U_DEFAULTHDR:		.long	DEFAULTHDR

#endif /* TA_FPU */
/* ------------------------------------------------------------------------ */
/*
 * High level programming language routine for interrupt handler
 *	Called by interrupt entry routine on the state saved in the interrupt 
 *	stack as shown below.
 *	   High Address	+---------------+
 *			| USP		| Only RNG 1-3 
 *			+---------------+
 *			| MDR (R2_BANK1)| RNG0-3 common
 *			| SSR		|
 *			| SPC		|
 *		ISP =>	| R0_BANK1	|
 *	    Low Address	+---------------+
 *
 *	Also, 'vector indexe + factor' are saved in R0_BANK1.
 *	Factor	0:Exception  1:TRAPA  2:Interrupt
 */
	.text
	.balign	2
	.globl	Csym(defaulthdr_startup)
	.type	Csym(defaulthdr_startup), @function
	.globl	Csym(inthdr_startup)
	.type	Csym(inthdr_startup), @function
Csym(defaulthdr_startup):
	and	#3, r0
	mov.l	I_defaulthdr_vecidx, r7
	or	r7, r0

Csym(inthdr_startup):
	/* During interrupt disable SR.I=15 BL=1 RB=1 */
	mov.l	r8, @-ISP		// Temporarily save R8-R9
	mov.l	r9, @-ISP

	/* Get EIT information */
	mov.l	I_FF000020, r7
	tst	#2, r0
	bf	l_interrupt		// Exception/TRAPA
	mov.l	@(EXPEVT - 0xff000020, r7), r8	// R8 = EXPEVT
	mov.l	@(TRA    - 0xff000020, r7), r9	// R9 = TRA
	bra	l_inthdr1
	nop
  l_interrupt:					// Interrupt
	mov.l	@(INTEVT - 0xff000020, r7), r8	// R8 = INTEVT
  l_inthdr1:					// R9 = unknown


	mov.l	I_DisableIntSR, r7	// Free exception block, Switch register bank
	ldc	r7, sr

	/* During interrupt disable SR.I=15 BL=0 RB=0 */
	stc.l	r1_bank, @-SP		// Register save
	mov.l	r7,   @-SP
	mov.l	r6,   @-SP
	mov.l	r5,   @-SP
	mov.l	r4,   @-SP
	mov.l	r3,   @-SP
	mov.l	r2,   @-SP
	mov.l	r1,   @-SP
	mov.l	r0,   @-SP
	stc.l	gbr,  @-SP
	sts.l	pr,   @-SP
	sts.l	macl, @-SP
	sts.l	mach, @-SP

	mov	r8, r4			// EIT information
	mov	r9, r5

	stc	r6_bank, r6		// ISP == R6_BANK1
	mov.l	@r6+, r9		// R8,R9 restore
	mov.l	@r6+, r8
	ldc	r6, r6_bank

#if USE_DBGSPT
	mov.l	I_hook_ienter_jmp, r3
	mov.l	@r3, r3
	jmp	@r3
	nop
  ret_hook_ienter:
#endif

	stc	r0_bank, r0		// R0 = vector indexe + factor
	mov.l	I_hll_inthdr, r1
	shlr2	r0
	shll2	r0
	mov.l	@(r0, r1), r2		// call hll_inthdr[n](dintno, info)
	jsr	@r2
	nop

#if USE_DBGSPT
	mov.l	I_hook_ileave_jmp, r3
	mov.l	@r3, r3
	jmp	@r3
	nop
  ret_hook_ileave:
#endif

	mov.l	I_DisableIntSR, r0	// Interrupt disable
	ldc	r0, sr

	/* During interrupt disable SR.I=15 BL=0 RB=0 */
	lds.l	@SP+, mach		// Register restore
	lds.l	@SP+, macl
	lds.l	@SP+, pr
	ldc.l	@SP+, gbr
	mov.l	@SP+, r0
	mov.l	@SP+, r1
	mov.l	@SP+, r2
	mov.l	@SP+, r3
	mov.l	@SP+, r4
	mov.l	@SP+, r5
	mov.l	@SP+, r6
	mov.l	@SP+, r7
	ldc.l	@SP+, r1_bank

	trapa	#TRAP_RETINT		// tk_ret_int()

			.balign	4
  I_defaulthdr_vecidx:	.long	DEFAULTHDR - VECTBL
  I_hll_inthdr:		.long	Csym(hll_inthdr)
  I_DisableIntSR:	.long	SR_MD | SR_FD | SR_I(15)
  I_FF000020:		.long	0xff000020

#if USE_DBGSPT
/*
 * Interrupt handler hook routine call
 */
	.text
	.balign	2
hook_ienter:
	mov.l	I_hook_ienterfn, r3
	mov.l	@r3, r3

	mov.l	r4, @-SP	// Save dintno, info
	mov.l	r5, @-SP

	jsr	@r3		// call enter(dintno, info)
	nop

	mov.l	@(0*4, SP), r5	// r5 restore
	mov.l	@(1*4, SP), r4	// r4 restore

	bra	ret_hook_ienter
	nop

hook_ileave:
	mov.l	I_hook_ileavefn, r3
	mov.l	@r3, r3

	mov.l	@SP+, r5	// info
	jsr	@r3		// call leave(dintno, info)
	mov.l	@SP+, r4	// dintno

	bra	ret_hook_ileave
	nop

/*
 * Set/Free interrupt handler hook routine
 */
	.text
	.balign	2
	.globl	Csym(hook_int)
	.type	Csym(hook_int), @function
Csym(hook_int):
	mov.l	I_hook_ienter_jmp, r0
	mov.l	I_hook_ileave_jmp, r1
	mov.l	I_hook_ienter, r2
	mov.l	I_hook_ileave, r3
	mov.l	r2, @r0
	rts
	mov.l	r3, @r1

	.globl	Csym(unhook_int)
	.type	Csym(unhook_int), @function
Csym(unhook_int):
	mov.l	I_hook_ienter_jmp, r0
	mov.l	I_hook_ileave_jmp, r1
	mov.l	I_ret_hook_ienter, r2
	mov.l	I_ret_hook_ileave, r3
	mov.l	r2, @r0
	rts
	mov.l	r3, @r1

			.balign	4
  I_hook_ienter_jmp:	.long	hook_ienter_jmp
  I_hook_ileave_jmp:	.long	hook_ileave_jmp

  I_hook_ienter:	.long	hook_ienter
  I_hook_ileave:	.long	hook_ileave
  I_ret_hook_ienter:	.long	ret_hook_ienter
  I_ret_hook_ileave:	.long	ret_hook_ileave

  I_hook_ienterfn:	.long	Csym(hook_ienterfn)
  I_hook_ileavefn:	.long	Csym(hook_ileavefn)

			.data
			.balign	4
  hook_ienter_jmp:	.long	ret_hook_ienter
  hook_ileave_jmp:	.long	ret_hook_ileave

#endif /* USE_DBGSPT */

/*
 * tk_ret_int() processing
 *	When called, the interrupt stack is configured as shown below.
 *	   High Address	+---------------+
 *			| USP		| Only RNG 1-3 
 *			+---------------+
 *			| MDR (R2_BANK1)| RNG0-3 common
 *			| SSR		|
 *			| SPC		|
 *			| R0_BANK1	| When the interrupt occurs, save
 *			+---------------+ until here
 *			| MDR (R2_BANK1)| Save by 'trapa TRAP_RETINT'
 *			| SSR		|
 *			| SPC		|
 *		ISP =>	| R0_BANK1	|
 *	    Low Address	+---------------+
 */
	.text
	.balign	2
	.globl	Csym(_tk_ret_int)
	.type	Csym(_tk_ret_int), @function
Csym(_tk_ret_int):
	/* During interrupt disable SR.I=15 BL=1 RB=1 */
	add	#(4*4), ISP		// Trash parts saved by 'trapa TRAP_RETINT'

	mov.l	R_DisableIntSR, r0	// Free exception block
	ldc	r0, sr

	/* During interrupt disable SR.I=15 BL=1 RB=1 */
	mov.l	R_int_stack_top, r0		// Is it a nesting interrupt?
	mov.l	@r0, r0
	add	#-(5*4), r0
	cmp/ge	r0, ISP				// ISP >= r0
	bf	l_nodispatch

	mov.l	@(2*4, ISP), r0			// SR when EIT occurs
	tst	#SR_I(15), r0			// Is it an exception during interrupt disable?
	bf	l_nodispatch

	mov.l	R_dispatch_disabled, r0		// Is it during dispatch disable?
	mov.l	@r0, r0
	tst	r0, r0
	bf	l_nodispatch

	mov.l	r4, @-SP
	mov.l	R_ctxtsk, r0			// Is dispatch required?
	mov.l	R_schedtsk, r4
	mov.l	@r0, r0
	mov.l	@r4, r4
	cmp/eq	r0, r4
	mov.l	@SP+, r4
	bt	l_nodispatch

	mov.l	R_ret_int_dispatch, r0	// To dispatch processing
	jmp	@r0
	nop

  l_nodispatch:				// Dispatch not required
	mov.l	R_BlockIntSR, r0
	ldc	r0, sr
	INT_RETURN ret_int

			.balign	4
  R_int_stack_top:	.long	Csym(int_stack_top)
  R_dispatch_disabled:	.long	Csym(dispatch_disabled)
  R_ctxtsk:		.long	Csym(ctxtsk)
  R_schedtsk:		.long	Csym(schedtsk)
  R_ret_int_dispatch:	.long	_ret_int_dispatch
  R_DisableIntSR:	.long	SR_MD | SR_FD | SR_RB | SR_I(15)
  R_BlockIntSR:		.long	SR_MD | SR_FD | SR_RB | SR_I(15) | SR_BL

/* ------------------------------------------------------------------------ */

/*
 * Unsupported system call
 */
	.text
	.balign	2
	.globl	Csym(no_support)
	.type	Csym(no_support), @function
Csym(no_support):
	mov.l	N_E_RSFN, r0
	rts
	nop

		.balign	4
  N_E_RSFN:	.long	E_RSFN

/*
 * System call entry table
 */
	.text
	.balign	4
_svctbl:
	.int	Csym(no_support)
#define	_tk_ret_int	no_support
#include <sys/svc/tksvctbl.h>
#undef	_tk_ret_int

/*
 * System call entry
 *	Do not need to save the temporary register.
 *	The compiler saves the permanent register.
 *	   High Address	+---------------+
 *			| USP		| Only RNG 1-3
 *			+---------------+
 *			| MDR (R2_BANK1)| RNG0-3 common
 *			| SSR		|
 *			| SPC		|
 *	     ISP/SSP =>	| R0_BANK1	|
 *	    Low Address	+---------------+
 */
	.text
	.balign	2
	.globl	Csym(call_entry)
	.type	Csym(call_entry), @function
Csym(call_entry):
	/* During interrupt disable SR.I=15 BL=1 RB=1 */
	mov.l	C_DisableIntSR, r0	// Free exception block
	ldc	r0, sr

	/* Move from interrupt stack to system stack */
	MOVE_ISP_SSP call_entry

	mov.l	r8, @-SP		// R8 save

	mov.l	C_CallerSRMask, r8	// Register bank switch
	mov.l	@(3*4, SP), r0		// SSR
	and	r8, r0
	mov.l	C_EnableIntSR, r8
	or	r8, r0
	ldc	r0, sr
	/* The interrupt disable state keeps the caller's state SR.I=? BL=0 RB=0 */

	mov	r0, r8			// Save function code to R8
	sts.l	pr, @-SP		// PR save 
	mov.l	r14, @-SP		// FP save
	mov	SP, r14

#if USE_DBGSPT
	mov.l	C_hook_enter_jmp, r1	// Hook processing
	mov.l	@r1, r1
	jmp	@r1
	nop
  ret_hook_enter:
#endif

	cmp/pz	r8			// R0_BANK0 : Function code
	bt	l_esvc_function

	/* T-Kernel system call */
	mov	r14, r3
	add	#(7*4), r3
	mov.l	@(6*4, r14), r0		// Saved MDR
	and	#3, r0			// R0 = Caller's protected level

	mov.l	C_TSVCLimit, r2		// Call protected level limit
	mov.l	@r2, r2
	cmp/hi	r2, r0
	bt	l_oacv_err

	cmp/pl	r0
	bf	l_nousp
	mov.l	@r3, r3			// R3 = User stack pointer
  l_nousp:				// R3 = Argument position on stack

	swap.w	r8, r2
	exts.w	r2, r2			// R2 = Function code
	mov.l	C_TFN_limit, r0
	cmp/gt	r0, r2
	bt	l_illegal_svc

	mov	r8, r0
	shlr8	r0
	and	#0xff, r0		// R0 = Number of arguments
	cmp/eq	#5, r0
	bf	l_nocopy
	mov.l	@r3, r1			// Copy fifth argument
	mov.l	r1, @-SP
  l_nocopy:

	mov.l	C_svctbl, r0
	shll2	r2
	mov.l	@(r0, r2), r0
	jsr	@r0			// T-Kernel system call
	nop

  l_retsvc:
#if USE_DBGSPT
	mov.l	C_hook_leave_jmp, r1	// Hook processing
	mov.l	@r1, r1
	jmp	@r1
	nop
  ret_hook_leave:
#endif

	mov	r14, SP			// Register restore
	mov.l	@SP+, r14
	lds.l	@SP+, pr
	mov.l	@SP+, r8

	mov.l	C_DisableIntSR, r1	// Interrupt disable/Register bank switch RB=1
	ldc	r1, sr

#if !USE_MDR_DCT
	mov.l	@(3*4, SP), r0		// MDR on stack
	tst	#MDR_CPL(3), r0		// If it is a call from protected level 0,
	bt	l_nodct_svc		// do not process DCT

	mov.l	C_ctxtsk, r0		// Is there a DCT request?
	mov.l	@r0, r0
	mov.l	@(TCB_reqdct, r0), r0
	cmp/eq	#1, r0
	bf	l_nodct_svc

	mov.l	C_dispatch_disabled, r0	// If it is dispatch disable,
	mov.l	@r0, r0			// do not process DCT
	tst	r0, r0
	bf	l_nodct_svc

	mov.l	C_int_stack_top, r0	// If it is a call from the task independent part,
	mov.l	@r0, r0			// do not process DCT
	cmp/eq	ISP, r0			// (cannot not happen, but to make sure)
	bf	l_nodct_svc

	bra	dct_startup		// To DCT processing
	nop

  l_nodct_svc:
#endif
	MOVE_SSP_ISP call_entry		// Move from system stack to interrupt stack
	mov.l	C_BlockIntSR, r0	// Exception block
	ldc	r0, sr
	CALL_RETURN call_entry


  l_esvc_function:
	/* Extended SVC */
	mov.l	C_svc_ientry, r1
	jsr	@r1			// svc_ientry(pk_para, fncd)
	mov	r8, r5			// R5 = Function code

	bra	l_retsvc
	nop


  l_illegal_svc:
	mov.l	C_E_RSFN, r0
	bra	l_retsvc
	nop

  l_oacv_err:
	mov.l	C_E_OACV, r0
	bra	l_retsvc
	nop

			.balign	4
  C_EnableIntSR:	.long	SR_MD | SR_I(0)
  C_DisableIntSR:	.long	SR_MD | SR_FD | SR_I(15) | SR_RB
  C_BlockIntSR:		.long	SR_MD | SR_FD | SR_I(15) | SR_RB | SR_BL
  C_CallerSRMask:	.long	SR_I(15) | SR_FD

  C_TFN_limit:		.long	N_TFN + 0xffff8000
  C_svctbl:		.long	_svctbl - 0xfffe0000	// 0xffff8000 << 2

  C_TSVCLimit:		.long	Csym(svc_call_limit)
  C_svc_ientry:		.long	Csym(svc_ientry)

#if !USE_MDR_DCT
  C_ctxtsk:		.long	Csym(ctxtsk)
  C_dispatch_disabled:	.long	Csym(dispatch_disabled)
  C_int_stack_top:	.long	Csym(int_stack_top)
#endif

  C_E_RSFN:		.long	E_RSFN
  C_E_OACV:		.long	E_OACV

#if USE_DBGSPT

/*
 * System call/Extended SVC hook routine call
 *	VP enter( FN fncd, TD_CALINF *calinf, ... )
 *	void leave( FN fncd, INT ret, VP exinf )
 *
 *	typedef struct td_calinf {
 *		VP	ssp;	System stack pointer
 *		VP	pr;	PR register when calling
 *		VP	r14;	Flame pointer when calling
 *	} TD_CALINF;
 */
	.text
	.balign	2
hook_enter:
	mov.l	r13, @-SP		// Save register for work
	mov.l	r4, @-SP		// Save argument 
	mov.l	r5, @-SP
	mov.l	r6, @-SP
	mov.l	r7, @-SP
	mov	SP, r13			// Save stack position

	mov	r14, r3
	mov.l	@r14, r2		// r2 = Frame pointer when calling
	add	#(4*4), r3		// r3 = ssp
	mov.l	r2, @-SP		// Create TD_CALINF
	sts.l	pr, @-SP
	mov.l	r3, @-SP
	mov	SP, r2			// r2 = &TD_CALINF

	cmp/pz	r8			// r8 = fncd function code
	bt	l_nocopy_para		// Extended SVC

	mov	r14, r3
	add	#(7*4), r3
	mov.l	@(6*4, r14), r0		// Saved MDR
	and	#3, r0			// Caller's protected level
	cmp/pl	r0
	bf	l_nousp_para
	mov.l	@r3, r3			// User stack pointer
  l_nousp_para:				// r3 = Argument position on stack

	mov	r8, r0			// fncd
	shlr8	r0
	and	#0xff, r0		// Number of arguments
	cmp/eq	#5, r0
	bt	l_copy_para5
	cmp/eq	#4, r0
	bt	l_copy_para4
	cmp/eq	#3, r0
	bt	l_copy_para3
	bra	l_nocopy_para
	nop
  l_copy_para5:
	mov.l	@r3, r1
	mov.l	r1, @-SP		// Fifth argument
  l_copy_para4:
	mov.l	r7, @-SP		// Fourth argument
  l_copy_para3:
	mov.l	r6, @-SP		// Third argument

  l_nocopy_para:
	mov.l	C_hook_enterfn, r1
	mov.l	@r1, r1
	mov	r5, r7			// Second argument
	mov	r4, r6			// First argument
	mov	r2, r5			// TD_CALINF
	jsr	@r1			// exinf = enter(fncd, calinf, ...)
	mov	r8, r4			// fcnd

	mov	r13, SP			// Return stack position
	mov.l	@SP+, r7		// Restore argument
	mov.l	@SP+, r6
	mov.l	@SP+, r5
	mov.l	@SP+, r4
	mov.l	@SP, r13		// Restore register for work
	bra	ret_hook_enter
	mov.l	r0, @SP			// Save 'lr_svc'

hook_leave:
	mov	r0, r5			// r5 = ret
	mov	#0, r6			// r6 = exinf = 0

	mov	r8, r0			// Function code
	cmp/pz	r0			// If r0 >= 0, extended SVC
	bt	l_hl1
	shlr8	r0
	and	#0xff, r0		// Parameter number
	add	#-4, r0
	cmp/pz	r0
	bt	l_hl2
  l_hl1:
	mov	#0, r0
  l_hl2:
	shll2	r0
	add	SP, r0			// Whether 'hook_enter' is executed
	cmp/hi	r0, r14			// Check by stack usage
	bf	l_hl3
					// 'hook_enter' is executed
	mov.l	@r0, r6			// 'exinf' retrieve
  l_hl3:

	mov.l	C_hook_leavefn, r1
	mov.l	@r1, r1
	mov	r8, r4			// fncd
	jsr	@r1			// call leave(fncd, ret, exinf)
	mov	r5, r8			// r8 = ret save

	bra	ret_hook_leave
	mov	r8, r0			// r0 = ret restore

/*
 * Set/Free system call/extended SVC hook routine 
 */
	.text
	.balign	2
	.globl	Csym(hook_svc)
	.type	Csym(hook_svc), @function
Csym(hook_svc):
	mov.l	C_hook_enter_jmp, r0
	mov.l	C_hook_leave_jmp, r1
	mov.l	C_hook_enter, r2
	mov.l	C_hook_leave, r3
	mov.l	r2, @r0
	rts
	mov.l	r3, @r1

	.globl	Csym(unhook_svc)
	.type	Csym(unhook_svc), @function
Csym(unhook_svc):
	mov.l	C_hook_enter_jmp, r0
	mov.l	C_hook_leave_jmp, r1
	mov.l	C_ret_hook_enter, r2
	mov.l	C_ret_hook_leave, r3
	mov.l	r2, @r0
	rts
	mov.l	r3, @r1

			.balign	4
  C_hook_enter_jmp:	.long	hook_enter_jmp
  C_hook_leave_jmp:	.long	hook_leave_jmp

  C_hook_enter:		.long	hook_enter
  C_hook_leave:		.long	hook_leave
  C_ret_hook_enter:	.long	ret_hook_enter
  C_ret_hook_leave:	.long	ret_hook_leave

  C_hook_enterfn:	.long	Csym(hook_enterfn)
  C_hook_leavefn:	.long	Csym(hook_leavefn)

			.data
			.balign	4
  hook_enter_jmp:	.long	ret_hook_enter
  hook_leave_jmp:	.long	ret_hook_leave

#endif /* USE_DBGSPT */

/* ------------------------------------------------------------------------ */

#if USE_DBGSPT
/*
 * Debugger support function service call entry table
 */
	.text
	.balign	4
_tdsvctbl:
	.int	Csym(no_support)
#include <sys/svc/tdsvctbl.h>

/*
 * Debugger support function service call entry
 */
	.text
	.balign	2
	.globl	Csym(call_dbgspt)
	.type	Csym(call_dbgspt), @function
Csym(call_dbgspt):
	/* During interrupt disable SR.I=15 BL=1 RB=1 */
	mov.l	B_DisableIntSR, r0	// Free exception block
	ldc	r0, sr

	/* Move from interrupt stack to system stack */
	MOVE_ISP_SSP call_dbgspt

	mov.l	r4, @-SP		// R4_BANK1 save
	mov.l	B_EnableIntSR, r4	// Register bank switch
	mov.l	@(3*4, SP), r0		// SSR
	and	#SR_I(15), r0
	or	r4, r0
	mov.l	@SP+, r4		// R4_BANK1 restore
	ldc	r0, sr
	/* The interrupt disable state keeps the caller's state SR.I=? BL=0 RB=0 */

	sts.l	pr, @-SP		// PR save 
	mov.l	r14, @-SP		// FP save
	mov	SP, r14

	mov	r0, r1			// Save function code to R1

	mov.l	@(5*4, r14), r0		// Saved MDR
	and	#3, r0			// R0 = Caller's protected level

	mov.l	B_TSVCLimit, r2		// Call protected level limit
	mov.l	@r2, r2
	cmp/hi	r2, r0
	bt	b_oacv_err

	swap.w	r1, r2
	exts.w	r2, r2			// R2 = Function code
	mov.l	B_TDFN_limit, r0
	cmp/gt	r0, r2
	bt	b_illegal_svc

	mov.l	B_tdsvctbl, r0
	shll2	r2
	mov.l	@(r0, r2), r0
	jsr	@r0			// T-Kernel/DS service call
	nop

  b_retsvc:
	mov	r14, SP
	mov.l	@SP+, r14
	lds.l	@SP+, pr

	mov.l	B_DisableIntSR, r1	// Interrupt disable RB=1
	ldc	r1, sr
	MOVE_SSP_ISP call_dbgspt	// Move from system stack to interrupt stack
	mov.l	B_BlockIntSR, r0	// Exception block
	ldc	r0, sr
	CALL_RETURN call_dbgspt


  b_illegal_svc:
	mov.l	B_E_RSFN, r0
	bra	b_retsvc
	nop

  b_oacv_err:
	mov.l	B_E_OACV, r0
	bra	b_retsvc
	nop

			.balign	4
  B_BlockIntSR:		.long	SR_MD | SR_FD | SR_I(15) | SR_RB | SR_BL
  B_DisableIntSR:	.long	SR_MD | SR_FD | SR_I(15) | SR_RB
  B_EnableIntSR:	.long	SR_MD | SR_FD | SR_I(0)

  B_TDFN_limit:		.long	N_TDFN + 0xffff8000
  B_tdsvctbl:		.long	_tdsvctbl - 0xfffe0000	// 0xffff8000 << 2

  B_TSVCLimit:		.long	Csym(svc_call_limit)

  B_E_RSFN:		.long	E_RSFN
  B_E_OACV:		.long	E_OACV

#endif /* USE_DBGSPT */

/* ------------------------------------------------------------------------ */
/*
 * High level programming language routine for timer handler
 */
	.text
	.balign	2
	.globl	Csym(timer_handler_startup)
	.type	Csym(timer_handler_startup), @function
Csym(timer_handler_startup):
	/* During interrupt disable SR.I=15 BL=1 RB=1 */
	mov.l	T_EnableIntSR, r7	// Free exception block, Switch register bank
	ldc	r7, sr

	/* Multiplexed interrupt enable SR.I=Timer priority BL=0 RB=0 */
	mov.l	r7, @-SP		// Register save
	mov.l	r6, @-SP
	mov.l	r5, @-SP
	mov.l	r4, @-SP
	mov.l	r3, @-SP
	mov.l	r2, @-SP
	mov.l	r1, @-SP
	mov.l	r0, @-SP
	stc.l	gbr,  @-SP
	sts.l	pr,   @-SP
	sts.l	macl, @-SP
	sts.l	mach, @-SP

	mov.l	T_timer_handler, r0
	jsr	@r0			// call timer_handler()
	nop

	lds.l	@SP+, mach		// Register restore
	lds.l	@SP+, macl
	lds.l	@SP+, pr
	ldc.l	@SP+, gbr
	mov.l	@SP+, r0
	mov.l	@SP+, r1
	mov.l	@SP+, r2
	mov.l	@SP+, r3
	mov.l	@SP+, r4
	mov.l	@SP+, r5
	mov.l	@SP+, r6
	mov.l	@SP+, r7

	trapa	#TRAP_RETINT		// tk_ret_int()

			.balign	4
  T_EnableIntSR:	.long	SR_MD | SR_I(TIMER_INTLEVEL) | SR_FD
  T_timer_handler:	.long	Csym(timer_handler)

/* ------------------------------------------------------------------------ */
/*
 * Delayed context trap (DCT)
 *	Task exception handler startup
 *	When called, the interrupt stack is configured as shown below.
 *	(Use MDR DCT)
 *	   High Address	+---------------+
 *			| USP		|
 *			| MDR (R2_BANK1)|
 *			| SSR		|
 *			| SPC		|
 *			| R0_BANK1	|
 *			+---------------+
 *			| MDR (R2_BANK1)| Save by 'trapa TRAP_RETINT'
 *			| SSR		|
 *			| SPC		|
 *		ISP =>	| R0_BANK1	|
 *	    Low Address	+---------------+
 */
	.text
	.balign	2
#if USE_MDR_DCT
	.globl	Csym(dct_startup)
	.type	Csym(dct_startup), @function
Csym(dct_startup):
	/* During interrupt disable SR.I=15 BL=1 RB=1 */
	add	#(4*4), ISP		// Trash parts saved by 'trapa TRAP_RETINT'
	mov.l	D_DisableIntSR_RB, r7	// Free exception block
	ldc	r7, sr
	MOVE_ISP_SSP dct_startup
#else
dct_startup:
#endif
	/* During interrupt disable SR.I=15 BL=0 RB=1 */
	mov.l	D_DisableIntSR, r0	// Register bank switch (RB=0)
	ldc	r0, sr

	/* During interrupt disable SR.I=15 BL=0 RB=0 */
	mov.l	r7, @-SP		// Register save
	mov.l	r6, @-SP
	mov.l	r5, @-SP
	mov.l	r4, @-SP
	mov.l	r3, @-SP
	mov.l	r2, @-SP
	mov.l	r1, @-SP
	mov.l	r0, @-SP
	stc.l	gbr,  @-SP
	sts.l	pr,   @-SP
	sts.l	macl, @-SP
	sts.l	mach, @-SP

	mov.l	D_setup_texhdr, r0
	mov	SP, r4
	jsr	@r0			// call setup_texhdr(ISP)
	add	#12*4, r4		// ISP equivalent position
	/* Return on interrupt enable state */

	lds.l	@SP+, mach		// Register restore
	lds.l	@SP+, macl
	lds.l	@SP+, pr
	ldc.l	@SP+, gbr
	mov.l	@SP+, r0
	mov.l	@SP+, r1
	mov.l	@SP+, r2
	mov.l	@SP+, r3
	mov.l	@SP+, r4
	mov.l	@SP+, r5
	mov.l	@SP+, r6

	mov.l	D_DisableIntSR_RB, r7	// Interrupt disable/Register bank switch (RB=1)
	ldc	r7, sr
	/* During interrupt disable SR.I=15 BL=0 RB=1 */

	ldc.l	@SP+, r7_bank

	MOVE_SSP_ISP dct_startup
	mov.l	D_BlockIntSR, r0	// Exception block
	ldc	r0, sr
	CALL_RETURN dct_startup

			.balign	4
  D_DisableIntSR_RB:	.long	SR_MD | SR_FD | SR_I(15) | SR_RB
  D_DisableIntSR:	.long	SR_MD | SR_FD | SR_I(15)
  D_BlockIntSR:		.long	SR_MD | SR_FD | SR_I(15) | SR_RB | SR_BL
  D_setup_texhdr:	.long	Csym(setup_texhdr)

/* ------------------------------------------------------------------------ */
/*
 * SR register load function
 *	Function for writing to the SR register on the user mode.
 *	Set the value to write to SR on R0_BANK0,
 *	and then call by 'trapa TRAP_LOADSR.'
 *	However, cannot change the following (not be changed):
 *	  MD	Privilege mode
 *	  RB	Register bank
 *	  BL	Exception block
 *	  DSP	DSP mode (SH3)
 *	  I	Interrupt mask
 *	  Reservation bit
 */

	.text
	.balign	2
	.globl	Csym(load_SR)
	.type	Csym(load_SR), @function
Csym(load_SR):
	/* SR.I=15 BL=1 RB=1 */
	mov.l	r1, @-ISP		// R1 save

	mov.l	L_SR_Mask, r7		// R7 = Unchangeable bit mask
	mov.l	@(3*4, ISP), r1		// R1 = Current SR
	stc	r0_bank, r0		// R0 = Write only SR

	and	r7, r1
	not	r7, r7
	and	r7, r0
	or	r0, r1			// R1 = R1 & R7 | R0 & ~R7

	mov.l	r1, @(3*4, ISP)		// Save SR after change

	mov.l	@ISP+, r1		// R1 restore
	INT_RETURN load_SR		// Update SR by returning from TRAP

		.balign	4
  L_SR_Mask:	.long	0x8fff7c0c | SR_MD | SR_RB | SR_BL | SR_FD | SR_I(15)


/*----------------------------------------------------------------------*/
#if 0
#|【cpu_support.S 改版履歴】
#|□2015/12/19	[app_sh7760]用に、{V1.02.04}の"sh7760/cpu_support.S"から作成。
#|□2015/12/19	dispatch時にWDTﾄﾘｶﾞ処理を追加(WDT_DOG_FOOD_ADDR)
#|
#endif
