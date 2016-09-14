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
 *	@(#)sysdef_depend.h (tk/SH7760)
 *
 *	Definition about SH7760
 *
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_H__
#define __TK_SYSDEF_DEPEND_H__

/*
 * 用途指定レジスタ
 */
#define	SP	r15			/* スタックポインタ */

/*
 * BANK1 レジスタ
 *      R0     作業用                   タスク付属(With task)
 *      R1     予約                       〃
 *      R2 MDR 動作モードレジスタ         〃
 *      R3 SST システムスタックトップ     〃
 *      R4     作業用                   タスク独立(Task independent)
 *      R5     予約                       〃
 *      R6 ISP 例外スタックポインタ       〃
 *      R7     例外作業用                 〃     SR.BL=1 の時のみ使用可
 */
#define	MDR	r2			/* 動作モードレジスタ */
#define	SST	r3			/* システムスタックトップ */
#define	ISP	r6			/* 例外スタックポインタ */

/*
 * MDR register
 */
#define MDR_DCT		0x80000000		/* Delayed context trap request */
#define MDR_PPL(n)	( (n) << 16 )	/* Previous protection level (0-3) */
#define MDR_CPL(n)	( (n) )			/* Current protection level (0-3) */

/*
 * SR レジスタ
 */
#define	SR_MD		0x40000000		/* 特権モード */
#define	SR_RB		0x20000000		/* レジスタバンク */
#define	SR_BL		0x10000000		/* 割込ブロック */
#define	SR_FD		0x00008000		/* FPU 使用禁止(SH4) */
#define	SR_I(n)		( (n) << 4 )	/* 割込マスクレベル(0～15) */

/*
 * FPU ステータス/コントロールレジスタ
 */
#define	FPSCR_FR	0x00200000		/* FPU レジスタバンク */
#define	FPSCR_SZ	0x00100000		/* 転送サイズモード */
#define	FPSCR_PR	0x00080000		/* 精度モード */
#define	FPSCR_DN	0x00040000		/* 非正規化モード */
#define	FPSCR_C(n)	((n) << 12)		/* 例外要因(6bit) */
#define	FPSCR_E(n)	((n) << 7)		/* 例外許可(5bit) */
#define	FPSCR_F(n)	((n) << 2)		/* 例外フラグ(5bit) */
#define	FPSCR_RM(n)	(n)				/* 丸めモード(2bit) */

					/* FPU 例外 */
#define	FPE_I		0x01			/* 不正確 */
#define	FPE_U		0x02			/* アンダーフロー */
#define	FPE_O		0x04			/* オーバーフロー */
#define	FPE_Z		0x08			/* ゼロ除算 */
#define	FPE_V		0x10			/* 無効演算 */
#define	FPE_E		0x20			/* FPU エラー */

/*
 * 例外処理レジスタ
 */
#define	TRA			0xff000020		/* W:TRAPA 例外 */
#define	EXPEVT		0xff000024		/* W:例外事象 */
#define	INTEVT		0xff000028		/* W:割込事象 */

/*
 * MMU register
 */
#define PTEH		0xff000000		/* W:Page table entry high */
#define PTEL		0xff000004		/* W:Page table entry low */
#define PTEA		0xff000034		/* W:Page table entry assistance */
#define TTB			0xff000008		/* W:Page table base */
#define TEA			0xff00000c		/* W:TLB exception address */
#define MMUCR		0xff000010		/* W:MMU control */

#define MMU_AT		0x00000001		/* MMU enable */
#define MMU_TF		0x00000004		/* TLB flush */
#define MMU_SV		0x00000100		/* Single virtual memory mode */
#define MMU_SQMD	0x00000200		/* Store queue mode */

/*
 * Cache register
 */
#define CCR			0xff00001c		/* W:Cache control */
#define QACR0		0xff000038		/* W:Queue address control 0 */
#define QACR1		0xff00003c		/* W:Queue address control 1 */

#define CCR_OCE		0x00000001		/* OC enable */
#define CCR_WT		0x00000002		/* Write through enable (U0 P0 P3) */
#define CCR_CB		0x00000004		/* Copy back enable (P1) */
#define CCR_OCI		0x00000008		/* OC invalidation */
#define CCR_ORA		0x00000020		/* OC RAM enable */
#define CCR_OIX		0x00000080		/* OC index enable */
#define CCR_ICE		0x00000100		/* IC enable */
#define CCR_ICI		0x00000800		/* IC invalidation */
#define CCR_IIX		0x00008000		/* IC index enable */
#define CCR_EMODE	0x80000000		/* Cache double mode */

/*
 * Interrupt controller
 */
#define ICR			0xffd00000		/* H:Interrupt control */
#define IPRA		0xffd00004		/* H:Interrupt priority A */
#define IPRB		0xffd00008		/* H:Interrupt priority B */
#define IPRC		0xffd0000c		/* H:Interrupt priority C */
#define IPRD		0xffd00010		/* H:Interrupt priority D */
#define INTPRI00	0xfe080000		/* W:Interrupt priority level 00 */
#define INTPRI04	0xfe080004		/* W:Interrupt priority level 04 */
#define INTPRI08	0xfe080008		/* W:Interrupt priority level 08 */
#define INTPRI0C	0xfe08000C		/* W:Interrupt priority level 0C */
#define INTREQ00	0xfe080020		/* W:Interrupt factor 00 */
#define INTREQ04	0xfe080024		/* W:Interrupt factor 04 */
#define INTMSK00	0xfe080040		/* W:Interrupt mask 00 */
#define INTMSK04	0xfe080044		/* W:Interrupt mask 04 */
#define INTMSKCLR00	0xfe080060		/* W:Interrupt mask clear 00 */
#define INTMSKCLR04	0xfe080064		/* W:Interrupt mask clear 04 */

/* ------------------------------------------------------------------------ */

/*
 * T-Monitor 用TRAPA 番号
 */
#define	TRAP_MONITOR	0x90		/* T-Monitor サービスコール */

/*
 * T-Kernel 用TRAPA 番号
 */
#define	TRAP_SVC		0x91		/* T-Kernel システムコール・拡張SVC */
#define	TRAP_RETINT		0x92		/* tk_ret_int() システムコール */
#define	TRAP_DISPATCH	0x93		/* タスクディスパッチャ呼出 */
									/*  0x94 予約 */
#define	TRAP_LOADSR		0x95		/* SR レジスタロード機能 */
#define	TRAP_DEBUG		0x96		/* デバッガサポート機能 */

/*
 * Extension用TRAPA 番号
 */
#define	TRAP_KILLPROC	0x97		/* プロセス強制終了要求 */

#endif /* __TK_SYSDEF_DEPEND_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|【sysdef_depend.h 変更履歴】
#|□2015/11/26	V1.02.04の"include/tk/sysdepend/std_sh7760/"から、
#|  [app_sh7760]用にコピー
#|□2015/11/26	T-Kernel V1.02.04 Software Package「SH7760 実装仕様書」
#|  imp_sh7760.txt の3.5項を参照してコメントを日本語化
#|
#endif
