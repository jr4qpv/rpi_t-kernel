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
 *    Modified by T.Yokobayashi at 2015/11/26.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)asm_depend.h (tk/SH7760)
 *
 *	Assembler Macro for SH7760
 */

#ifndef __TK_ASM_DEPEND_H__
#define __TK_ASM_DEPEND_H__

/*
 * 例外・割込時のスタック状態
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
 * マクロ
 *      INT_ENTER   例外・割込エントリー処理
 *                  上記のように例外スタックへ保存する。
 *      INT_RETURN  例外・割込リターン処理
 *                  上記のような例外スタックから復帰してリターン(RTE)する。
 *
 *      拡張SVC のようにタスク独立部とならない場合は、例外スタックを使い続け
 *      ることはできないため、ディスパッチが許可(割込許可)される前に、システム
 *      スタックへ移動する必要がある。
 *
 *      MOVE_ISP_SSP 例外スタックからシステムスタックへ移動
 *      MOVE_SSP_ISP システムスタックから例外スタックへ戻す
 */

/*
 * 例外・割込エントリー共通処理
 */
 .macro	INT_ENTER name

	mov	#3, r7					// CPL > 0 ならシステムスタックへ切替る
	tst	r7, MDR
	bt	l_nochg_\name

	mov.l	SP, @-ISP			// ユーザースタック保存
	mov	SST, SP					// システムスタックへ切替

  l_nochg_\name:
	mov.l	MDR, @-ISP			// 動作モード保存
	shll16	MDR					// 動作モード更新

	stc.l	ssr, @-ISP			// SSR 保存
	stc.l	spc, @-ISP			// SPC 保存
	mov.l	r0, @-ISP			// R0 保存

	stc	sr, r0					// 割込禁止SR.I = 15
	or	#SR_I(15), r0
	ldc	r0, sr

 .endm

/*
 * 例外・割込リターン共通処理
 * SR.BL=1, SR.RB=1 で実行すること
 */
 .macro	INT_RETURN name

	mov.l	@ISP+, r0			// R0 復帰
	ldc.l	@ISP+, spc			// SPC 復帰
	ldc.l	@ISP+, ssr			// SSR 復帰
	mov.l	@ISP+, MDR			// MDR 復帰

	mov 	#3, r7				// PPL > 0 ならユーザスタックへ切替る
	tst	r7, MDR
	bt	l_norst_\name

	mov.l	@ISP+, SP			// ユーザースタックへ切替

  l_norst_\name:
	rte
	nop

 .endm

/*
 * TRAPA リターン共通処理
 * SR.BL=1, SR.RB=1 で実行すること
 * SR.FD が常に1 となるようにSSR を復帰する。
 */
 .macro	CALL_RETURN name

	mov.l	@(2*4, ISP), r0		// SSR
	mov.l	l_srfd_\name, r7
	or	r7, r0					// SSR.FD = 1
	ldc	r0, ssr					// SSR 復帰

	mov.l	@ISP+, r0			// R0 復帰
	ldc.l	@ISP+, spc			// SPC 復帰
	add	#4, ISP					// SSR 分の読み飛ばし(4byte)
	mov.l	@ISP+, MDR			// MDR 復帰

	mov	#3, r7					// PPL > 0 ならユーザスタックへ切替る
	tst	r7, MDR
	bt	l_norst_\name

	mov.l	@ISP+, SP			// ユーザースタックへ切替
            
  l_norst_\name:
	rte
	nop
		.balign 4
  l_srfd_\name:	.long	SR_FD

 .endm

/*
 * ISP -> SSP 移動
 * SR.BL=0, SR.RB=1 で実行すること
 */
 .macro	MOVE_ISP_SSP name

	mov.l	@(3*4,ISP), r0		// MDR
	tst	#3, r0					// USP が保存されているか確認
	bt	l_nousp1_\name			// PPL > 0 ならUSP が保存されている

	mov.l	@(4*4,ISP), r0		// USP
	mov.l	r0, @-SP

  l_nousp1_\name:
	mov.l	@(3*4,ISP), r0		// MDR
	mov.l	r0, @-SP
	mov.l	@(2*4,ISP), r0		// SSR
	mov.l	r0, @-SP
	mov.l	@(1*4,ISP), r0		// SPC
	mov.l	r0, @-SP
	mov.l	@(0*4,ISP), r0		// R0
	mov.l	r0, @-SP

	bt	l_nousp2_\name			// 例外スタックから捨てる
	add	#1*4, ISP

  l_nousp2_\name:
	add	#4*4, ISP

 .endm

/*
 * SSP -> ISP 移動
 * SR.BL=0, SR.RB=1 で実行すること
 */
 .macro	MOVE_SSP_ISP name

	mov.l	@(3*4,SP), r0		// MDR
	tst	#3, r0					// USP が保存されているか確認
	bt	l_nousp3_\name			// PPL > 0 ならUSP が保存されている

	mov.l	@(4*4,SP), r0		// USP
	mov.l	r0, @-ISP

  l_nousp3_\name:
	mov.l	@(3*4,SP), r0		// MDR
	mov.l	r0, @-ISP
	mov.l	@(2*4,SP), r0		// SSR
	mov.l	r0, @-ISP
	mov.l	@(1*4,SP), r0		// SPC
	mov.l	r0, @-ISP
	mov.l	@(0*4,SP), r0		// R0
	mov.l	r0, @-ISP

	bt	l_nousp4_\name			// システムスタックから捨てる
	add	#1*4, SP

  l_nousp4_\name:
	add	#4*4, SP

 .endm

/*
 * タスク例外ハンドラのエントリー
 *
 *
 *              +---------------+
 *  USP(R15) -> | save SR       | ハンドラからの戻り時に復帰する SR
 *      +4      | retadr        | ハンドラからの戻りアドレス
 *      +8      | texcd         | 例外コード
 *              +---------------+
 *
 *      ユーザーモードで実行される場合があるため、
 *      特権命令が使用できない点に注意。
 *      スタックは 4 バイトアライメントのため、
 *      double(8 バイト)ごとのアクセスができない点に注意。
 */
 .macro	TEXHDR_ENTRY texhdr

	mov.l	r4, @-SP			// R4 保存
	mov.l	@(3*4, SP), r4		// R4 = texcd
	mov.l	r0, @(3*4, SP)		// R0 保存

	stc.l	gbr,  @-SP			// その他レジスタ保存
	sts.l	pr,   @-SP
	sts.l	mach, @-SP
	sts.l	macl, @-SP
	mov.l	r1,   @-SP
	mov.l	r2,   @-SP
	mov.l	r3,   @-SP

	mov.l	r5,   @-SP
	mov.l	r6,   @-SP
	mov.l	r7,   @-SP

	sts.l	fpscr, @-SP			// FPU レジスタの保存
	sts.l	fpul,  @-SP
	mov	#0, r0
	lds	r0, fpscr
	frchg						// FPSCR.FR=1 SZ=0
	fmov.s	fr15, @-SP
	fmov.s	fr14, @-SP
	fmov.s	fr13, @-SP
	fmov.s	fr12, @-SP
	fmov.s	fr11, @-SP
	fmov.s	fr10, @-SP
	fmov.s	fr9,  @-SP
	fmov.s	fr8,  @-SP
	fmov.s	fr7,  @-SP
	fmov.s	fr6,  @-SP
	fmov.s	fr5,  @-SP
	fmov.s	fr4,  @-SP
	fmov.s	fr3,  @-SP
	fmov.s	fr2,  @-SP
	fmov.s	fr1,  @-SP
	fmov.s	fr0,  @-SP
	frchg						// FPSCR.FR=0
	fmov.s	fr11, @-SP
	fmov.s	fr10, @-SP
	fmov.s	fr9,  @-SP
	fmov.s	fr8,  @-SP
	fmov.s	fr7,  @-SP
	fmov.s	fr6,  @-SP
	fmov.s	fr5,  @-SP
	fmov.s	fr4,  @-SP
	fmov.s	fr3,  @-SP
	fmov.s	fr2,  @-SP
	fmov.s	fr1,  @-SP
	fmov.s	fr0,  @-SP

	mov.l	l_fpscr_\texhdr, r0
	lds.l	@r0+, fpscr			// FPSCR 初期値設定

	mov.l	l_\texhdr, r0
	jsr	@r0						// call texhdr(texcd)
	nop

	mov	#0, r0
	lds	r0, fpscr				// FPSCR.FR=0 SZ=0
	fmov.s	@SP+, fr0
	fmov.s	@SP+, fr1
	fmov.s	@SP+, fr2
	fmov.s	@SP+, fr3
	fmov.s	@SP+, fr4
	fmov.s	@SP+, fr5
	fmov.s	@SP+, fr6
	fmov.s	@SP+, fr7
	fmov.s	@SP+, fr8
	fmov.s	@SP+, fr9
	fmov.s	@SP+, fr10
	fmov.s	@SP+, fr11
	frchg						// FPSCR.FR=1
	fmov.s	@SP+, fr0
	fmov.s	@SP+, fr1
	fmov.s	@SP+, fr2
	fmov.s	@SP+, fr3
	fmov.s	@SP+, fr4
	fmov.s	@SP+, fr5
	fmov.s	@SP+, fr6
	fmov.s	@SP+, fr7
	fmov.s	@SP+, fr8
	fmov.s	@SP+, fr9
	fmov.s	@SP+, fr10
	fmov.s	@SP+, fr11
	fmov.s	@SP+, fr12
	fmov.s	@SP+, fr13
	fmov.s	@SP+, fr14
	fmov.s	@SP+, fr15
	lds.l	@SP+, fpul
	lds.l	@SP+, fpscr

	mov.l	@SP+, r7			// レジスタ復帰
	mov.l	@SP+, r6
	mov.l	@SP+, r5
	mov.l	@SP+, r3
	mov.l	@SP+, r2
	mov.l	@SP+, r1
	lds.l	@SP+, macl
	lds.l	@SP+, mach
	lds.l	@SP+, pr
	ldc.l	@SP+, gbr
	mov.l	@SP+, r4

	mov.l	@SP+, r0			// SR の復帰
	trapa	#TRAP_LOADSR		// R0 -> SR

	mov.l	@SP+, r0			// ハンドラからの戻りアドレス
	jmp	@r0
	mov.l	@SP+, r0			// R0 復帰

			.balign 4
  l_\texhdr:		.long	\texhdr		// ハンドラ本体アドレス
  l_fpscr_\texhdr:	.long	__fpscr_values + 4

 .endm


#endif /* __TK_ASM_DEPEND_H__ */

#if 0
#|【asm_depend.h 変更履歴】
#|□2015/11/26	V1.02.04の"include/sys/sysdepend/std_sh7760/"から、
#|  [app_sh7760]用にコピー
#|□2015/11/26	T-Kernel V1.02.04 Software Package「SH7760 実装仕様書」
#|  imp_sh7760.txt の3.5項を参照してコメントを日本語化
#|
#endif
