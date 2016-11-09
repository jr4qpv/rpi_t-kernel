/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(c) 2015-2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2016/01/26.
 *
 *----------------------------------------------------------------------
 */

/*
 *	monhdr.S
 *
 *       Monitor handler (SH-4,SH7760)
 */
#define	_in_asm_source_

#include <machine.h>
#include <tk/asm.h>		/* see <tk/sysdepend/app_sh7760/asm_depend.h> */
#include <sys/sysinfo.h>
#include "cpudep.h"

#include "register.h"


/*
 * Monitor entry (registered as default handler)
 *
 *            +-------+
 * new ISP -> | R0    |
 *            | SPC   |
 *            | SSR   |
 *            | MDR   |
 *            | USP   | USP はシステムスタックへの切替があった場合のみ保存
 *            +-------+
 * old ISP ->
 *
 */
	.text
	.balign	4
	.globl	Csym(_defaultHdr)
	.type	Csym(_defaultHdr), %function
Csym(_defaultHdr):			// RB=1, BL=1, IM=15
					// r0…ﾍﾞｸﾀｰｲﾝﾃﾞｯｸｽ+要因
// save register
//      regStack[0-15]          r0 .. r15
//              [16-19]         mach, macl, gbr, pr
//              [20,21]         Entry sr, pc
//              [22-24]         vbr, dbr, sgr
//              [25-31]         r0b .. r6b
//              [32]            ccr
//              [33-48]         fr0 .. fr15
//              [49-64]         xf0 .. xf15
//              [65,66]         fpul, fpscr

        // 例外ﾍﾞｸﾀｲﾝﾃﾞｯｸｽ情報をISPに退避
	mov.l	r0, @-ISP		// push 例外ﾍﾞｸﾀｲﾝﾃﾞｯｸｽ情報

        //===== regStackにﾚｼﾞｽﾀ状態を退避 =====
	mov.l	L_SR_FD, r7
	stc	sr, r0
	tst	r7, r0			// SR.FD ?
	bf	10f			// SR.FD=1の時はFPU関連退避しない

	//--- FPUレジスタの退避 ---
	mov.l	L_regStack_btm, r7

        // save fpul, fpscr
	sts.l	fpscr, @-r7
	sts.l	fpul, @-r7

        // save xf0 .. xf15
	mov	#0, r0
	lds	r0, fpscr
	frchg				// FPSCR.FR=1に設定

	fmov.s	fr15, @-r7
	fmov.s	fr14, @-r7
	fmov.s	fr13, @-r7
	fmov.s	fr12, @-r7
	fmov.s	fr11, @-r7
	fmov.s	fr10, @-r7
	fmov.s	fr9, @-r7
	fmov.s	fr8, @-r7

	fmov.s	fr7, @-r7
	fmov.s	fr6, @-r7
	fmov.s	fr5, @-r7
	fmov.s	fr4, @-r7
	fmov.s	fr3, @-r7
	fmov.s	fr2, @-r7
	fmov.s	fr1, @-r7
	fmov.s	fr0, @-r7

        // save fr0 .. fr15
	frchg				// FPSCR.FR=0に設定

	fmov.s	fr15, @-r7
	fmov.s	fr14, @-r7
	fmov.s	fr13, @-r7
	fmov.s	fr12, @-r7
	fmov.s	fr11, @-r7
	fmov.s	fr10, @-r7
	fmov.s	fr9, @-r7
	fmov.s	fr8, @-r7

	fmov.s	fr7, @-r7
	fmov.s	fr6, @-r7
	fmov.s	fr5, @-r7
	fmov.s	fr4, @-r7
	fmov.s	fr3, @-r7
	fmov.s	fr2, @-r7
	fmov.s	fr1, @-r7
	bra	11f			//(遅延分岐)
	fmov.s	fr0, @-r7

	//--- 一般レジスタの退避 ---
10:	mov.l	L_regStack_ftop, r7

11:	// save CCR
	mov.l	L_CCR, r0
	mov.l	@r0, r0
	mov.l	r0, @-r7		// CCR

        // save r0b .. r6b
	mov.l	r6, @-r7		// ISP
	mov.l	r5, @-r7
	mov.l	r4, @-r7
	mov.l	r3, @-r7		// SST
	mov.l	@(12+4, ISP), r0	// get MDR
	mov.l	r0, @-r7		// r2b
	mov.l	r1, @-r7
	mov.l	@(0+4, ISP), r0		// get R0
	mov.l	r0, @-r7		// r0b

        // save vbr, dbr, sgr
	stc.l	sgr, @-r7
	stc.l	dbr, @-r7
	stc.l	vbr, @-r7

        // save the status on entry sr, pc(ssr, spc)
	mov.l	@(4+4, ISP), r0		// get SPC
	mov.l	r0, @-r7		// pc
	mov.l	@(8+4, ISP), r0		// get SSR
	mov.l	r0, @-r7		// sr

        // save mach, macl, gbr, pr
	sts.l	pr, @-r7
	stc.l	gbr, @-r7
	sts.l	macl, @-r7
	sts.l	mach, @-r7

        // save r0 .. r15
	mov.l	r15, @-r7		// SP
	mov.l	r14, @-r7
	mov.l	r13, @-r7
	mov.l	r12, @-r7
	mov.l	r11, @-r7
	mov.l	r10, @-r7
	mov.l	r9, @-r7
	mov.l	r8, @-r7

	stc.l	r7_bank, @-r7
	stc.l	r6_bank, @-r7
	stc.l	r5_bank, @-r7
	stc.l	r4_bank, @-r7
	stc.l	r3_bank, @-r7
	stc.l	r2_bank, @-r7
	stc.l	r1_bank, @-r7
	stc.l	r0_bank, @-r7

        // set up stack exclusively used for monitor (SVC mode)
	// (SPが __stack_top～__stack_bottom の範囲にあるかをﾁｪｯｸ)
	mov.l	L__stack_bottom, r7	// stack exclusively used for monitor
	cmp/hi	r7, SP			// SP＞r7(btm) ?, using monitor stack?
	bt	15f			// SPが__stack_bottm超えてるのでSP切換
	mov.l	L__stack_top, r0
	cmp/hs	r0, SP			// SP≧r0(top) ?
	bt	16f			// SPが__stack_top以上なのでそのまま

15:	// (SPを切換え)
	mov	r7, SP			// switch to monitor stack

        // monitor entry: r4 = interrupt/exception vector number : SVC mode
16:	mov.l	@ISP+, r0		// pop 例外ﾍﾞｸﾀｰｲﾝﾃﾞｯｸｽ情報
	shlr2	r0
	ldc	r0, r4_bank		// r4←ﾍﾞｸﾀ番号(vec)

	stc.l	sr, @-SP		// save SR (RB,BL,FD情報含む)

	mov.l	L_SR_RB_BL_NOT, r7
	stc	sr, r0
	and	r7, r0			// clear SR.RB=0, SR.BL=0
	ldc	r0, sr
	// (↓↓ SR.RB=0, SR.BL=0 ↓↓)

	//***** Call entMonitor(vec) *****
	mov.l	L_entMonitor, r0	// r4…ﾍﾞｸﾀ番号(vec)
	jsr	@r0			//(遅延分岐),call entMonitor(vec)
	nop

	ldc.l	@SP+, sr		// restore SR (RB,BL,FD情報含む)
	// (↓↓ SR.RB=1, SR.BL=1 ↓↓)

        //===== regStackからﾚｼﾞｽﾀ状態を復帰 ======
	mov.l	L_SR_FD, r7
	stc	sr, r0
	tst	r7, r0			// SR.FD ?
	bf	20f			// SR.FD=1の時はFPU関連退避しない

	//--- FPUレジスタの復帰 ---
	mov.l	L_regStack_ftop, r7	// FPU退避領域の先頭

        // restore fr0 .. fr15
	mov	#0, r0
	lds	r0, fpscr		// FPSCR.FR=0に設定

	fmov.s	@r7+, fr0
	fmov.s	@r7+, fr1
	fmov.s	@r7+, fr2
	fmov.s	@r7+, fr3
	fmov.s	@r7+, fr4
	fmov.s	@r7+, fr5
	fmov.s	@r7+, fr6
	fmov.s	@r7+, fr7

	fmov.s	@r7+, fr8
	fmov.s	@r7+, fr9
	fmov.s	@r7+, fr10
	fmov.s	@r7+, fr11
	fmov.s	@r7+, fr12
	fmov.s	@r7+, fr13
	fmov.s	@r7+, fr14
	fmov.s	@r7+, fr15

        // restore xf0 .. xf15
	frchg				// FPSCR.FR=1に設定

	fmov.s	@r7+, fr0
	fmov.s	@r7+, fr1
	fmov.s	@r7+, fr2
	fmov.s	@r7+, fr3
	fmov.s	@r7+, fr4
	fmov.s	@r7+, fr5
	fmov.s	@r7+, fr6
	fmov.s	@r7+, fr7

	fmov.s	@r7+, fr8
	fmov.s	@r7+, fr9
	fmov.s	@r7+, fr10
	fmov.s	@r7+, fr11
	fmov.s	@r7+, fr12
	fmov.s	@r7+, fr13
	fmov.s	@r7+, fr14
	fmov.s	@r7+, fr15

        // restore fpul, fpscr
	lds.l	@r7+, fpul
	lds.l	@r7+, fpscr

	//--- 一般レジスタの復帰 ---
20:	mov.l	L_regStack_top, r7

        // restore r0 .. r15
	ldc.l	@r7+, r0_bank
	ldc.l	@r7+, r1_bank
	ldc.l	@r7+, r2_bank
	ldc.l	@r7+, r3_bank
	ldc.l	@r7+, r4_bank
	ldc.l	@r7+, r5_bank
	ldc.l	@r7+, r6_bank
	ldc.l	@r7+, r7_bank

	mov.l	@r7+, r8
	mov.l	@r7+, r9
	mov.l	@r7+, r10
	mov.l	@r7+, r11
	mov.l	@r7+, r12
	mov.l	@r7+, r13
	mov.l	@r7+, r14
	mov.l	@r7+, r15		// SP

        // restore mach, macl, gbr, pr
	lds.l	@r7+, mach
	lds.l	@r7+, macl
	ldc.l	@r7+, gbr
	lds.l	@r7+, pr

        // restore return sr, pc (ssr, spc)
	mov.l	@r7+, r0		// ssr
	mov.l	r0, @(8, ISP)		// set SSR
	mov.l	@r7+, r0		// spc
	mov.l	r0, @(4, ISP)		// set SPC

        // restore vbr, dbr, sgr
	add	#4, r7			// skip restore vbr, 安全の為
	ldc.l	@r7+, dbr
	add	#4, r7			// skip sgr restore, 命令がない

        // restore r0b .. r6b
	mov.l	@r7+, r0		// r0b
	mov.l	r0, @(0, ISP)		// set R0
	mov.l	@r7+, r1
	mov.l	@r7+, r2		// r2b
	mov.l	r2, @(12, ISP)		// set MDR
	mov.l	@r7+, r3
	mov.l	@r7+, r4
	mov.l	@r7+, r5
	add	#8, r7			// skip restore ISP(r6b),CCR ﾓﾆﾀ管理

        // do we have request for forcible termination of processe(es)?
	mov.l	L_killProcReq, r7
	mov.l	@r7, r0			// r0←(killProcReq)
	tst	r0, r0			// r0=0 ?
	bt	no_kill			// no request
	mov	#0, r0
	mov.l	r0, @r7			// clear forcible termination request

        // restore to the state on entry completely, and then push lr_svc
#if 1	//////////// kari //////////////
	mov.l	9001f, r0
	jmp	@r0
	mov	#0x09, r4

	.balign 4
	.globl	Csym(_fatal_trap_led)
9001:	.long	Csym(_fatal_trap_led)
#else	///////////////////////////////
	stmfd	sp!, {r1, ip}		// sp -> lr_svc, ip
	mov	ip, r2			// restore ip
	ldmdb	r7, {r0-r7}		// r0 .. r7
#endif	////////////////////////////////

	trapa	#TRAP_KILLPROC		// forcible termination of processes request
	nop				// do not return
	nop

  no_kill:
        // return from monitor
	INT_RETURN			// 例外ﾘﾀｰﾝ処理, see "tk/asm_depend.h"


	.balign 4
L_SR_FD:	.long	SR_FD
L_SR_RB_BL_NOT:	.long	~(SR_RB | SR_BL)
L__stack_top:	.long	__stack_top
L__stack_bottom: .long	__stack_bottom
L_regStack_top:	.long	Csym(regStack)		// see "sh4/register.c"
L_regStack_btm:	.long	Csym(regStack) + (N_regStack * 4)
L_regStack_ftop: .long	Csym(regStack) + ((N_regStack - N_regStack_FPU) * 4)
L_CCR:		.long	CCR			// see "tk/sysdef_depend.h"
L_entMonitor:	.long	Csym(entMonitor)	// see "sh4/monent.c"
L_killProcReq:	.long	Csym(killProcReq)	// W:see "cmdsvc/command.c"



/* ------------------------------------------------------------------------ */

/*
 * calling an external program
 * W callExtProg( FP entry )
 */
	.text
	.balign	4
	.globl	Csym(callExtProg)
	.type	Csym(callExtProg), %function
Csym(callExtProg):
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

	jsr	@r4				// call entry(void)
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


/*----------------------------------------------------------------------*/
#if 0
#|【monhdr.S 変更履歴】
#|□2015/12/10	[app_sh7760]用に、新規作成。
#|  [tef_em1d]用の"../armv6/monhdr.S"を参考に作成。
#|□2016/01/26	CCRﾚｼﾞｽﾀの退避処理を追加。
#|
#endif
