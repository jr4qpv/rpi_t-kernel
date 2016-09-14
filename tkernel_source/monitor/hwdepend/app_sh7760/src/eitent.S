/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2015-2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2015/11/26.
 *
 *----------------------------------------------------------------------
 */

/*
 *    @(#)eitent.S () 2016/11/26
 *
 *    SH7760(SH-4) 例外・割り込みエントリールーチン
 *
 */

/*
 *[Note]
 *・CPUリセット後には、最初に｢_start｣が実行される。
 *・｢VBRHDR｣セクションの処理は、RAMに移動する事もあるのでﾘﾛｹｰﾀﾌﾞﾙにする事。
 *・例外/割込みテーブルについては、「SH7760 実装仕様書」の3.項を参照の事。
 *・割込例外マクロ詳細は、"tk/sysdepend/app_sh7760/asm_depend.h"を参照。
 */
#define	_in_asm_source_

#include <machine.h>		/* 機種名、他 */
#include <tk/asm.h>		/* see <tk/sysdepend/app_sh7760/asm_depend.h> */
				/* see <tk/sysdepend/app_sh7760/sysdef_depend.h> */
#include <sys/sysinfo.h>	/* see <sys/sysdepend/app_sh7760/sysinfo_depend.h> */



/*----------------------------------------------------------------------
        例外・割込み分岐ルーチン
----------------------------------------------------------------------*/
	.section VBRHDR, "ax"

/* ------------------------------------------------------------------------ */
/*
 * リセット処理(0xA0000000)
 */
	.org 0x00000000
_start:
_reset:					//*** ﾘｾｯﾄ entry(実際は0xA0000000) ***
					// MD=1,特権ﾓｰﾄﾞ
					// BL=1,例外要求ﾌﾞﾛｯｸ
					// RB=1,ﾚｼﾞｽﾀﾊﾞﾝｸﾋﾞｯﾄ
					// EXPEVT=例外ｺｰﾄﾞ
//	( 例外コードの確認 )
	mov.l	L0_EXPEVT, r7		// 例外コード取得
	mov.l	@r7, r7			// r0←(EXPEVT)

	tst	r7, r7			// r7=0x000:ﾊﾟﾜｰｵﾝ ?
	bf	Csym(_exception)	// (その他は、例外ﾊﾝﾄﾞﾗ処理へ分岐)
					// r7=0x020:ﾏﾆｭｱﾙﾘｾｯﾄ(WDT) 含む
					// r7=0x140:命令/ﾃﾞｰﾀTLB多重ﾋｯﾄ例外 含む

//	( ﾘｾｯﾄ処理へ相対分岐 )
	mov.l	L0_startup_entry_diff, r7
	braf	r7			//(遅延分岐)
	nop
_reset90:

	.balign 4
L0_EXPEVT:		.long	EXPEVT
L0_startup_entry_diff:	.long	startup_entry - _reset90	// 相対値


/* ------------------------------------------------------------------------ */
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
 * 例外ハンドラ(VBR + 0x100)
 */
	.org 0x100
	.globl	Csym(_exception)
	.type	Csym(_exception), %function
Csym(_exception):			//*** 一般例外処理 ***
					// MD=1,特権ﾓｰﾄﾞ
					// BL=1,例外要求ﾌﾞﾛｯｸ
					// RB=1,ﾚｼﾞｽﾀﾊﾞﾝｸﾋﾞｯﾄ
					// EXPEVT=例外ｺｰﾄﾞ
					// TRA=TRAPAﾃﾞｰﾀ(TRAPAの時)
	INT_ENTER _exception

	mov.l	L1_EXPEVT, r0		// 例外コード取得
	mov.l	@r0, r0
	mov	#-5, r7
	shad	r7, r0
	shll2	r0			// ベクターテーブルインデックス

	cmp/eq	#EIT_TRAPA*4, r0	// TRAPA 例外コードか?
	bt	_trapa			// TRAPA ハンドラの処理へ

	mov.l	L1_VECTBL, r7		// ハンドラーアドレス取得
	mov.l	@(r0,r7), r7
	tst	r7, r7			// NULL なら未定義
	bf	l_jmpexc

	mov.l	L1_DEFAULT, r7		// デフォルトハンドラ
	mov.l	@r7, r7

l_jmpexc:
	jmp	@r7			// 例外ハンドラへジャンプ
	nop				// r0 = ベクターインデックス+ 要因(0)

/*
 * TRAPA ハンドラ
 */
	.balign 4
_trapa:
	mov.l	L1_TRA, r0		// TRAPA インデックス取得
	mov.l	@r0, r0

	mov.l	L1_TRALIMIT, r7		// 範囲チェック
	cmp/hs	r7, r0			// 有効範囲TRAPA 0～0x9f
	bt	l_jmptrap_default

	mov.l	L1_VECTBL, r7		// ハンドラーアドレス取得
	mov.l	@(r0,r7), r7
	tst	r7, r7			// NULL なら未定義
	bf	l_jmptrap

l_jmptrap_default:
	mov.l   L1_DEFAULT, r7		// デフォルトハンドラ
	mov.l   @r7, r7

l_jmptrap:
	jmp     @r7			// TRAPA ハンドラへジャンプ
	add     #1, r0			// r0 = ベクターインデックス+ 要因(1)

	.balign 4
L1_EXPEVT:	.long	EXPEVT
L1_VECTBL:	.long	VECTBL
L1_DEFAULT:	.long	DEFAULTHDR
L1_TRA:		.long	TRA
L1_TRALIMIT:	.long	0xA0 << 2	// TRAPA 0xA0 以上は有効範囲外


/*
 * 例外・割込リターン共通処理
 * SR.BL=1, SR.RB=1 で実行すること
 */
	.balign 4
	.globl	_int_return_bl

	.globl	Csym(_int_return)
	.type	Csym(_int_return), %function
Csym(_int_return):
	stc	sr, r0
	mov.l	L1_SR_BL_RB, r7
	or	r7, r0			// BL=1, RB=1
	ldc	r0, sr

_int_return_bl:
	INT_RETURN _int_return

	.balign 4
L1_SR_BL_RB:	.long	(SR_RB | SR_BL )	// RB=1, BL=1


/* ------------------------------------------------------------------------ */
/*
 * TLB ミス例外ハンドラ(VBR + 0x400)
 */
	.org 0x400
_tlbmiss:				//*** TLBミス例外処理 ***
					// MD=1,特権ﾓｰﾄﾞ
					// BL=1,例外要求ﾌﾞﾛｯｸ
					// RB=1,ﾚｼﾞｽﾀﾊﾞﾝｸﾋﾞｯﾄ
					// EXPEVT=例外ｺｰﾄﾞ
	mov.l	L2_TLBMISS, r7		// TLB ミス例外ハンドラアドレス取得
	mov.l	@r7, r7
	jmp	@r7			// TLB ミス例外ハンドラへジャンプ
	nop

	.balign 4
L2_TLBMISS:	.long	TLBMISSHDR


/* ------------------------------------------------------------------------ */
/*
 * 割込ハンドラ(VBR + 0x600)
 */
	.org 0x600
_interrupt:				//*** 割り込み処理 ***
					// MD=1,特権ﾓｰﾄﾞ
					// BL=1,例外要求ﾌﾞﾛｯｸ
					// RB=1,ﾚｼﾞｽﾀﾊﾞﾝｸﾋﾞｯﾄ
					// INTEVT=割込ｺｰﾄﾞ
	INT_ENTER _interrupt

	mov.l	L3_INTEVT, r0		// 割込要因取得
	mov.l	@r0, r0
	mov	#-5, r7
	shad	r7, r0
	shll2	r0			// ベクターテーブルインデックス

	mov.l	L3_VECTBL, r7		// ハンドラーアドレス取得
	mov.l	@(r0,r7), r7
	tst	r7, r7			// NULL なら未定義
	bf	l_jmpint

	mov.l	L3_DEFAULT, r7		// デフォルトハンドラ
	mov.l	@r7, r7

l_jmpint:
	jmp	@r7			// 割込ハンドラへジャンプ
	add 	#2, r0			// r0 = ベクターインデックス+ 要因(2)

	.balign 4
L3_INTEVT:	.long	INTEVT
L3_VECTBL:	.long	VECTBL
L3_DEFAULT:	.long	DEFAULTHDR


/*----------------------------------------------------------------------*/
#if 0
#|【eitent.S 変更履歴】
#|□2015/11/26	 [app_sh7760]用に新規作成
#|  T-Kernel V1.02.04 Software Package「SH7760 実装仕様書」imp_sh7760.txt と
#|  [tef_em1d]用のeitent.S を参考に作成。
#|
#endif
