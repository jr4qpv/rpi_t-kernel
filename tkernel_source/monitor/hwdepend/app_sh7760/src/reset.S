/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2015 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2015/11/28.
 *
 *----------------------------------------------------------------------
 */

/*
 *	reset.S () 2015/11/28
 *
 *       SH7760(SH-4) : initial setting after a reset.
 */

/*
 *[Note]
 *・CPUリセット後、最初に｢startup_entry｣からBL=1,キャッシュオフで実行。
 *・基本的なシステムレジスタの設定後、初期診断の｢init_diag_start」処理に分岐。
 *  （分岐と同時に、キャッシュオン状態となる）
 *・初期診断後に、｢reset_entry｣に戻ってきて、C言語の｢procReset()｣を呼び出し、
 *  T-Monitorに分岐(起動)する。
 */
#define	_in_asm_source_

#include <machine.h>		/* 機種名、他 */
#include <tk/sysdef.h>		/* see <tk/sysdepend/app_sh7760/sysdef_depend.h> */
#include <sys/sysinfo.h>	/* see <sys/sysdepend/app_sh7760/sysinfo_depend.h> */
#include <sh4/sh7760io.h>	/* IOﾚｼﾞｽﾀ定義 */
#include <misc/cw_data.h>	/* ﾓｰﾙｽ符号ﾃﾞｰﾀ */

#include "setup_sh7760.h"	/* 基板固有定数 */



/*----------------------------------------------------------------------
        T-Monitor reset boot processing
----------------------------------------------------------------------*/
	.section .startup, "ax"

/*
 *  リセット初期化処理 (ｷｬｯｼｭ無効, BL=1)
 */
	.balign	4
	.globl	startup_entry
	.type	startup_entry, %function
startup_entry:				//*** ﾘｾｯﾄ処理 ***
					// MD=1,特権ﾓｰﾄﾞ
					// BL=1,例外要求ﾌﾞﾛｯｸ
					// RB=1,ﾚｼﾞｽﾀﾊﾞﾝｸﾋﾞｯﾄ
					// R0…例外ｺｰﾄﾞ
/*
 * system i/o initialize
 */

/*-------( CPGの設定 )-------*/
#if (CPG_CHANGE == 1)			//*** 発信ｸﾛｯｸ変更時のみ ***
	mov.l	L1_WTCSR, r7		// r7←WTCSR address
	mov.w	W1_WTCSR_CHANGE, r0	// TME=0
	mov.w	r0, @r7			// (WTCSR)←r0

	mov.l	L1_WTCNT, r7		// r7←WTCNT address
	mov.w	W1_WTCNT_INIT, r0
	mov.w	r0, @R7			// (WTCNT)←0,WDTｶｳﾝﾄｸﾘｱ

	mov.l	L1_FRQCR, r7		// r7←FRQCR address
	mov.w	W1_FRQCR_CHANGE, r0	// TME=0
	mov.w	r0, @r7			// (FRQCR)←R0
#endif
	
#if (CPG_CHANGE == 2)			//*** 発信ｸﾛｯｸ=2変更時(IOP5)のみ ***
	mov.l	L1_WTCSR, r7		// r7←WTCSR address
	mov.w	W1_WTCSR_CHANGE, r0	// TME=0
	mov.w	r0, @r7			// (WTCSR)←r0

	mov.l	L1_WTCNT, r7		// r7←WTCNT address
	mov.w	W1_WTCNT_INIT, r0
	mov.w	r0, @R7			// (WTCNT)←0,WDTｶｳﾝﾄｸﾘｱ

	mov.l	L1_FRQCR, r7		// r7←FRQCR address
	mov.w	W1_FRQCR_CHANGE, r0	// TME=0
	mov.w	r0, @r7			// (FRQCR)←R0

	mov.l	L1_DCKDR00, r7		// r7←address
	mov.l	L1_DCKDR_CHANGE, r0	// INIT DATA
	mov.l	r0, @r7			// (address)←R0
#endif					//***[CPG_CHANGE END]***************

	bra	bsc_initialize		//(遅延分岐), 次へ
	nop

	.balign 2
W1_WTCSR_CHANGE:	.word	WTCSR_CHANGE
W1_WTCNT_INIT:		.word	WTCNT_INIT
W1_FRQCR_CHANGE:	.word	FRQCR_CHANGE

	.balign 4
L1_WTCSR:		.long	IOBASE_C0 + WTCSR_C0	// WTCSR address
L1_WTCNT:		.long	IOBASE_C0 + WTCNT_C0	// WTCNT address
L1_FRQCR:		.long	IOBASE_C0 + FRQCR_C0	// FRQCR address
L1_DCKDR00:		.long	DCKDR00_ADDR		// DCKDR00 address

L1_DCKDR_CHANGE:	.long	DCKDR_CHANGE		// INIT DATA


/*-------( BSCの初期化 )-------*/
bsc_initialize:
	mov.l	L2_IOBASE_80, r0
	ldc	r0, gbr			// GBR←IOBASE address

	mov.l	L2_BCR1_INIT, r0
	mov.l	r0, @(BCR1_80, gbr)	// BCR1の設定(ｴﾘｱ機能)

	mov.w	W2_BCR2_BOOT, r0	// BOOT時のﾊﾞｽ幅設定
	mov.w	r0, @(BCR2_80, gbr)	// BCR2の設定(ﾊﾞｽ幅)

	mov.w	W2_BCR3_INIT, r0
	mov.w	r0, @(BCR3_80, gbr)	// BCR3の設定

	mov.l	L2_BCR4_INIT, r0
	mov.l	L2_BCR4_ADDR, r7
	mov.l	r0, @r7			// BCR4の設定(非同期入力)

	mov.l	L2_WCR1_INIT, r0
	mov.l	r0, @(WCR1_80,gbr)	// WCR1の設定(ｱｲﾄﾞﾙｽﾃｰﾄ)

	mov.l	L2_WCR2_INIT, r0
	mov.l	r0, @(WCR2_80, gbr)	// WCR2の設定(ｳｪｲﾄｽﾃｰﾄ)

	mov.l	L2_WCR3_INIT, r0
	mov.l	r0, @(WCR3_80, gbr)	// WCR3の設定


//	( SDRAMの設定 )		<<SH7760ﾊｰﾄﾞｳｪｱﾏﾆｭｱﾙ p10-82参照>>
	mov.l	L2_MCR_INIT, r7
	mov.l	L2_BFFFFFFF, r0
	and	r7, r0			// MRSET=0(全ﾊﾞﾝｸﾌﾟﾘﾁｬｰｼﾞ)
	mov.l	r0, @(MCR_80, gbr)	// MCRの設定

	mov.l	L2_SDMR3_INIT, r0
	mov.b	r0, @r0			// SDMRの設定

	mov.w	W2_RTCNT_INIT, r0
	mov.w	r0, @(RTCNT_80, gbr)	// RTCNTの設定(ｸﾘｱ)

	mov.w	W2_RTCOR_INIT1, r0	// 全ﾊﾞﾝｸﾌﾟﾘﾁｬｰｼﾞの為に短く1に設定
	mov.w	r0, @(RTCOR_80, gbr)	// RTCORの設定(ﾘﾌﾚｯｼｭ周期)

	mov.w	W2_RTCSR_INIT, r0
	mov.w	r0, @(RTCSR_80, gbr)	// RTCSRの設定(ﾘﾌﾚｯｼｭｺﾝﾄﾛｰﾙ)

	mov	#100, r0		// ｵｰﾄﾘﾌﾚｯｼｭ8回以上の期間待ち
1:	nop
	nop
	nop
	add	#-1, r0
	tst	r0, r0
	bf	1b			// to loop

	mov.w	W2_RTCOR_INIT, r0	// 通常値設定
	mov.w	r0, @(RTCOR_80, gbr)	// RTCORの設定(ﾘﾌﾚｯｼｭ周期)

	mov	r7, r0			// MCR_INIT(MRSET=1,ﾓｰﾄﾞﾚｼﾞｽﾀ設定)
	mov.l	r0, @(MCR_80, gbr)	// MCRの設定

	mov.l	L2_SDMR3_INIT, r0
	mov.b	r0, @r0			// SDMRの設定

	bra	ioport_initialize	//(遅延分岐), 次へ
	nop

	.balign 2
W2_BCR2_BOOT:		.word	BCR2_BOOT
W2_BCR3_INIT:		.word	BCR3_INIT
W2_RTCNT_INIT:		.word	RTCNT_INIT
W2_RTCOR_INIT:		.word	RTCOR_INIT
W2_RTCOR_INIT1:	.word	RTCOR_INIT1
W2_RTCSR_INIT:		.word	RTCSR_INIT

	.balign 4
L2_IOBASE_80:		.long	IOBASE_80
L2_BCR4_ADDR:		.long	BCR4_ADDR

L2_BCR1_INIT:		.long	BCR1_INIT
L2_BCR4_INIT:		.long	BCR4_INIT
L2_WCR1_INIT:		.long	WCR1_INIT
L2_WCR2_INIT:		.long	WCR2_INIT
L2_WCR3_INIT:		.long	WCR3_INIT
L2_MCR_INIT:		.long	MCR_INIT
L2_BFFFFFFF:		.long	0xBFFFFFFF
L2_SDMR3_INIT:		.long	SDMR3_INIT


//-------( I/Oﾎﾟｰﾄの初期化 )-------
ioport_initialize:
	mov.l	L5_IOBASE_80, r0
	ldc	r0, gbr			// gbr←IOBASE address

	mov.w	W5_GPIOIC_INIT, r0
	mov.w	r0, @(GPIOIC_80, gbr)	// GPIOICの設定

//	----------------------------
	mov.l	L5_IOBASE_140, r0
	ldc	r0, gbr			// gbr←IOBASE address

	mov	#PCDR_INIT, r0
	mov.b	r0, @(PCDR_140, gbr)	// PCDRの設定

	mov	#PDDR_INIT, r0
	mov.b	r0, @(PDDR_140, gbr)	// PDDRの設定

	mov	#PEDR_INIT, r0
	mov.b	r0, @(PEDR_140, gbr)	// PEDRの設定
	
	mov	#PGDR_INIT, r0
	mov.b	r0, @(PGDR_140, gbr)	// PGDRの設定

	mov	#PHDR_INIT, r0
	mov.b	r0, @(PHDR_140, gbr)	// PHDRの設定

	mov	#PJDR_INIT, r0
	mov.b	r0, @(PJDR_140, gbr)	// PJDRの設定

//	- - - - - - -
	mov.w	W5_PCCR_INIT, r0
	mov.w	r0, @(PCCR_140, gbr)	// PCCRの設定

	mov.w	W5_PDCR_INIT, r0
	mov.w	r0, @(PDCR_140, gbr)	// PDCRの設定

	mov.w	W5_PECR_INIT, r0
	mov.w	r0, @(PECR_140, gbr)	// PECRの設定

	mov.w	W5_PGCR_INIT, r0
	mov.w	r0, @(PGCR_140, gbr)	// PGCRの設定

	mov.w	W5_PHCR_INIT, r0
	mov.w	r0, @(PHCR_140,gbr)	// PHCRの設定

	mov.w	W5_PJCR_INIT, r0
	mov.w	r0, @(PJCR_140,gbr)	// PJCRの設定

//	( シリアルポートレジスタ０ )
	mov.l	L5_SCIF0_IOBASE, r0
	ldc	r0, gbr			// gbr←IOBASE address

	mov.w	W5_SCSPTR0_INIT, r0	// CLK='H'
	mov.w	r0, @(SCSPTR_X, gbr)

//	( シリアルポートレジスタ１ )
	mov.l	L5_SCIF1_IOBASE, r0
	ldc	r0, gbr			// gbr←IOBASE address

	mov.w	W5_SCSPTR1_INIT, r0	// CLK='H'
	mov.w	r0, @(SCSPTR_X, gbr)

//	( シリアルポートレジスタ２ )
	mov.l	L5_SCIF2_IOBASE, r0
	ldc	r0, gbr			// GBR←IOBASE address

	mov.w	W5_SCSPTR2_INIT, r0
	mov.w	r0, @(SCSPTR_X, gbr)

	bra	wdt_initialize		//(遅延分岐), 次へ
	nop

	.balign 2
W5_GPIOIC_INIT:	.word	GPIOIC_INIT
W5_PCCR_INIT:		.word	PCCR_INIT
W5_PDCR_INIT:		.word	PDCR_INIT
W5_PECR_INIT:		.word	PECR_INIT
W5_PGCR_INIT:		.word	PGCR_INIT
W5_PHCR_INIT:		.word	PHCR_INIT
W5_PJCR_INIT:		.word	PJCR_INIT
W5_SCSPTR0_INIT:	.word	SCSPTR0_INIT
W5_SCSPTR1_INIT:	.word	SCSPTR1_INIT
W5_SCSPTR2_INIT:	.word	SCSPTR2_INIT

	.balign 4
L5_IOBASE_80:		.long	IOBASE_80
L5_IOBASE_140:		.long	IOBASE_140
L5_SCIF0_IOBASE:	.long	SCIFn_IOBASE(0)
L5_SCIF1_IOBASE:	.long	SCIFn_IOBASE(1)
L5_SCIF2_IOBASE:	.long	SCIFn_IOBASE(2)


//-------( ＷＤＴの初期化…停止 )-------
wdt_initialize:
	mov.l	L3_IOBASE_C0, r0
	ldc	r0, gbr			// GBR←IOBASE address

	mov.w	W3_WTCSR_INIT, r0
	mov.w	r0, @(WTCSR_C0, gbr)

	mov.w	W3_WTCNT_INIT, r0
	mov.w	r0, @(WTCNT_C0, gbr)	// WDTｶｳﾝﾄｸﾘｱ

	bra	intc_initialize		//(遅延分岐), 次へ
	nop

	.balign 2
W3_WTCSR_INIT:		.word	WTCSR_INIT
W3_WTCNT_INIT:		.word	WTCNT_INIT

	.balign 4
L3_IOBASE_C0:		.long	IOBASE_C0


//-------( INTCの初期化 )-------	///// C言語の方に移動するか？///
intc_initialize:
	mov.l	L4_ICR, r7		// r7←IO address
	mov.w	W4_ICR_INIT, r0
	mov.w	r0, @r7			// CCRの設定

	mov.l	L4_INTMSK00, r7		// r7←IO address
	mov.l	L4_INTMSK00_INIT, r0
	mov.l	r0, @r7			// INTMSK00の設定

	mov.l	L4_INTMSK04, r7		// r7←IO address
	mov.l	L4_INTMSK04_INIT, r0
	mov.l	r0, @r7			// INTMSK04の設定

//	( 割り込みマスクの解除 )
//	mov.l	L4_INTMSKCLR00, r7	// r7←IO address
//	mov.l	L4_INTMSKCLR00_INIT, r0
//	mov.l	r0, @r7			// INTMSK00の設定
//
//	mov.l	L4_INTMSKCLR04, r7	// r7←IO address
//	mov.l	L4_INTMSKCLR04_INIT, r0
//	mov.l	r0, @r7			// INTMSK04の設定

	bra	cash_initialize		//(遅延分岐), 次へ
	nop

	.balign 2
W4_ICR_INIT:		.word	ICR_INIT

	.balign 4
L4_ICR:			.long	ICR		// see <tk/sysdef_depend.h>
L4_INTMSK00:		.long	INTMSK00	// see <tk/sysdef_depend.h>
L4_INTMSK04:		.long	INTMSK04	// see <tk/sysdef_depend.h>
//L4_INTMSKCLR00:		.long	INTMSKCLR00	// see <tk/sysdef_depend.h>
//L4_INTMSKCLR04:		.long	INTMSKCLR04	// see <tk/sysdef_depend.h>

L4_INTMSK00_INIT:	.long	INTMSK00_INIT
L4_INTMSK04_INIT:	.long	INTMSK04_INIT
//L4_INTMSKCLR00_INIT:	.long	INTMSKCLR00_INIT
//L4_INTMSKCLR04_INIT:	.long	INTMSKCLR04_INIT


//-------( キャッシュの設定 )-------
cash_initialize:
#if (BOOT_CASH_ON == 1)
	mov.l	L6_CCR, r7		// r7←IO address
	mov.l	L6_CCR_INIT, r0		// ｷｬｯｼｭON
	mov.l	r0, @r7			// CCRの設定(ｷｬｯｼｭ制御)
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
#endif

	mov.l	L6_INIT_DIAG_START, r7	// キャッシュ有効領域へ
	jmp	@r7			//(遅延分岐), キャシュ有効領域に分岐
	nop				// 初期診断後、後方｢reset_entry｣へ戻る


	.balign 4
L6_CCR:			.long	CCR	// see <tk/sysdef_depend.h>
L6_CCR_INIT:		.long	CCR_INIT
L6_INIT_DIAG_START:	.long	init_diag_start



/*----------------------------------------------------------------------
        Reset processing (ｷｬｯｼｭ有効, BL=1)
----------------------------------------------------------------------*/
	.text
	.balign	4
	.globl	reset_entry
	.type	reset_entry, %function
reset_entry:				// form "initdiag.S"
// SVC mode, interrupt disabled
	mov.l	L_SR_SVC_MODE, r0	// MD=1, RB=1, BL=1, FD=1
	ldc	r0, sr			// sr←r0

	mov.l	L_VBR_INIT, r0
	ldc	r0, vbr			// vbr 初期化

// Switch to T-Monitor stack
	mov.l	L__stack_bottom, SP

///	// not in effect: cache (D/I), program-flow prediction, High-Vector, VIC
///	// in effect: Force AP, TEX remap, Subpage AP
///		.DSB	r0
///		mrc	p15, 0, r0, cr1, cr0, 0
///		ldr	r1, =~0x01003f84
///		and	r0, r0, r1
///		ldr	r1, =0x30800000
///		orr	r0, r0, r1
///		mcr	p15, 0, r0, cr1, cr0, 0

///		.ISB	r0
///		mcr	p15, 0, r0, cr8, cr7, 0		// I/D TLB invalidate
///		.DSB	r0
///		bl	Csym(FlushCache)		// Clean/invalidate I/D cache

///	// Turn on VFP
///		mrc	p15, 0, r0, cr1, cr0, 2
///		orr	r0, r0, #0x00f00000	// VFP(CP11,CP10) enable
///		bic	r0, r0, #0xc0000000	// Should be Zero
///		mcr	p15, 0, r0, cr1, cr0, 2
///		.ISB	r0			// Flush Prefetch buffer

	// initialize data area
	mov.l	L__data_org, r1
	mov.l	L__data_start, r2
	mov.l	L_edata, r3

1:	cmp/hs	r3, r2			// r2≧r3 ?
	bt	9f			// to end

	mov.b	@r1+, r0		// copy (r2)←(r1)
	mov.b	r0, @r2
	bra	1b			//(遅延分岐),to loop
	add	#1,r2
9:
	mov.l	L_DOG_FOOD_ADDR, r7	// r0←DOG_FOOD_ADDR
	mov.b	@r7, r7			// WDTﾄﾘｶﾞ(dog_food)

	// clear bss and stack area
	mov.l	L__bss_start, r2
	mov.l	L__stack_bottom, r3
	mov	#0, r0

1:	cmp/hs	r3, r2			// r2≧r3 ?
	bt	9f			// to end

	mov.b	r0, @r2			// clear (r2)
	bra	1b			//(遅延分岐),to loop
	add	#1,r2
9:
	mov.l	L_DOG_FOOD_ADDR, r7	// r0←DOG_FOOD_ADDR
	mov.b	@r7, r7			// WDTﾄﾘｶﾞ(dog_food)

//----- reset processing -----
	mov.l	L_procReset, r0
	jsr	@r0			//(遅延分岐),Ｃ関数｢procReset()｣ｺｰﾙ
	nop
			// VECTBL領域は、上記 porcReset()内で初期化される。

//----- clear registers & initialize stack pointer -----
	mov.l	L_SR_SVC_MODE, r0	// MD=1, RB=1, BL=1, FD=1
	ldc	r0, sr			// sr←r0

	mov.l	L__stack_bottom, SP	// ｽﾀｯｸﾎﾟｲﾝﾀ(r15) 初期化
	mov.l	L__isp_bottom, ISP	// 例外ｽﾀｯｸﾎﾟｲﾝﾀ(ISP=r6) 初期化

	mov	#0, r0
	mov	r0, MDR			// 動作ﾓｰﾄﾞﾚｼﾞｽﾀ(MDR=r2) 初期化

/////	mov.l	L__stack_bottom, SST	// ｼｽﾃﾑｽﾀｯｸﾄｯﾌﾟ(SST=r3) 初期化 	////?????

// return to SVC mode
	mova	resetLoop, r0
	ldc	r0, spc
	mov.l	L_SR_SVC_RET, r0	// RB=0, BL=0, FD=1
	ldc	r0, ssr
	rte				//(遅延分岐), resetLoopへジャンプ
	nop

// enter monitor by means of SVC #0 instruction (SVC mode)
	.balign 4
resetLoop:
	//(↓↓ SR.RB=0, SR.BL=0, FD=1 ↓↓)
	trapa	#EIT_DEFAULT		// ﾃﾞﾌｫﾙﾄﾊﾝﾄﾞﾗを呼出しT-Monitorに入る
	bra	resetLoop		// return will cause another reset
	nop


#if 0	////////////////////////
///	debug_led_loop			/* ///// for debug ///// */

9:	mov	#2, r0			// TM_PUTCHAR
	mov	#'A', r4
	trapa	#EIT_MONITOR		//////////

	rfcr_wait1 500
	bra	9b
	nop
#endif	///////////////////////


	.balign 2

	.balign 4
L_DOG_FOOD_ADDR:	.long	DOG_FOOD_ADDR
L_SR_SVC_MODE:		.long	(SR_MD | SR_RB | SR_BL | SR_FD | SR_I(15))
L_SR_SVC_RET:		.long	(SR_MD | SR_FD | SR_I(15))	// RB=0, BL=0, FD=1
L_VBR_INIT:		.long	__eitbra_start
L__stack_bottom:	.long	__stack_bottom
L__data_org:		.long	__data_org
L__data_start:		.long	__data_start
L_edata:		.long	_edata
L__bss_start:		.long	__bss_start
L__isp_bottom:		.long	__isp_bottm		// ISP 初期値

	.globl	Csym(procReset)	/* (startup.c) */
L_procReset:		.long	Csym(procReset)



/*----------------------------------------------------------------------
	赤LED点滅処理 (致命的なエラー検出時用,ﾓｰﾙｽ符号で点滅)
----------------------------------------------------------------------*/
	.balign 4
	.globl	Csym(_fatal_trap_led00)
	.type	Csym(_fatal_trap_led00), %function

	.globl	Csym(_fatal_trap_led)
	.type	Csym(_fatal_trap_led), %function
Csym(_fatal_trap_led):			// r4…LED点滅ﾊﾟﾀｰﾝ("misc/cw_data.h"参照)
	tst	r4, r4			// r4=0 ?
	bf	_fatal_trap_led_loop

Csym(_fatal_trap_led00):
	mov.l	L_fatal_led_default, r4	// ﾃﾞﾌｫﾙﾄの点滅ﾊﾟﾀｰﾝ

//-------( LED符号点滅処理 )-------
_fatal_trap_led_loop:			// r4…LED点滅ﾊﾟﾀｰﾝ(保持用)
	mov	r4, r3			// r3←LED点滅ﾊﾟﾀｰﾝ(作業用)
	led_gbr_set			// gbr←LED port gbr
	led_all_off			// 赤&緑LED消灯

00:	mov	r3, r0
	and	#3, r0			// 下位2bit抽出
	cmp/eq	#0, r0			// r0=0 ?
	bt	50f			// 字間の待ちへ
	cmp/eq	#1, r0
	bt	01f			// 短点へ

//	( 長点 点灯 )
	led_red_on			// 赤LED点灯
	rfcr_wait1 (FATAL_LED_UNIT*3)	// 長点の時間待ち
	bra	05f			//(遅延分岐)
	nop

//	( 短点 点灯 )
01:	led_red_on			// 赤LED点灯
	rfcr_wait1 (FATAL_LED_UNIT)	// 短点の時間待ち

05:	led_red_off			// 赤LED消灯

//	( 符号間の時間待ち )
	rfcr_wait1 (FATAL_LED_UNIT)	// 短点の時間待ち

	bra	00b			//(遅延分岐)
	shlr2	r3			// 次の符号へ

//-------( 字間の時間待ち )-------
50:	rfcr_wait1 (FATAL_LED_UNIT*7)	// 字間の時間待ち

	bra	_fatal_trap_led_loop	//(遅延分岐)
	nop

	.balign 4
L_fatal_led_default:	.long	CW_DATA_DEFAULT		/* ････････ */


/*----------------------------------------------------------------------*/
#if 0
#|【reset.S 変更履歴】
#|□2015/11/28	 [app_sh7760]用に新規作成
#|  ・ [tef_em1d]用の"reset.S"、IOPﾓﾆﾀの"BOOT.SRC","BOOT7760.SRC"を参考に作成｡
#|
#endif
