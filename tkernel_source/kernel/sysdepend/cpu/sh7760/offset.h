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
 *    Modified by T.Yokobayashi at 2015/12/22.
 *
 *----------------------------------------------------------------------
 */

/*
 *	offset.h (SH7760)
 *	TCB Offset Definition
 */

#ifndef _OFFSET_
#define _OFFSET_

/* Adjust offset of TCB member variables in offset.h for cpu_support.S */
#include "config.h"

/*	TCB.wrdvno	*/
#if defined(NUM_PORID)
#define TCBSZ_POR	(4)	/* = sizeof(RNO) */
#else
#define TCBSZ_POR	(0)
#endif

/*	TCB.mtxlist	*/
#if defined(NUM_MTXID)
#define TCBSZ_MTX	(4)	/* = sizeof(MTXCB*) */
#else
#define TCBSZ_MTX	(0)
#endif

/*	TCB.winfo.xxx	*/
#if defined(NUM_PORID)
#define TCBSZ_WINFO	(16)
#else
#if defined(NUM_FLGID)
#define TCBSZ_WINFO	(12)
#else
#if defined(NUM_MBFID)||defined(NUM_MPLID)
#define TCBSZ_WINFO	(8)
#else
#if defined(NUM_SEMID)||defined(NUM_MBXID)||defined(NUM_MPFID)
#define TCBSZ_WINFO	(4)
#else
#define TCBSZ_WINFO	(0)
#endif
#endif
#endif
#endif

#define _ALIGN_CPU(x)	(((x)+3)&0xFFFFFFFC)	/* SH7760 : 32 bit CPU	*/


/*----------------------------------------------------------------------*/
/*  machine dependent data      see "/kernel/src/task.h"                */
/*----------------------------------------------------------------------*/
#define	TCB_winfo	(68)		/* tskque - wercd		*/
#define	TCB_wtmeb	_ALIGN_CPU(TCB_winfo+TCBSZ_WINFO)
#define	TCBsz_wtmeb2istack	(28+TCBSZ_MTX+TCBSZ_POR+76)
					/* wtmeb - istack		*/
#define TCBSZ_GP	(0)		/* No global pointer support	*/

/*----------------------------------------------------------------------*/
/*  offset data in TCB                                                  */
/*----------------------------------------------------------------------*/
#define TCB_isstack	(TCB_wtmeb+TCBsz_wtmeb2istack)
#define TCB_tskctxb	_ALIGN_CPU(TCB_isstack+4+TCBSZ_GP)

#define TCB_tskid	8
#define TCB_tskatr	16
#define TCB_state	43
#define TCB_reqdct	36
#define CTXB_lsid	8
#define TA_FPU		4096
#define CTXB_ssp	0
#define CTXB_uatb	4
#define TA_COP0		4096
#define COP0_REGSZ	136

#endif /* _OFFSET_ */


/*----------------------------------------------------------------------*/
#if 0
#|【offset.h 変更履歴】
#|□2015/12/22	[app_sh7760]用に、{V1.02.04}の"sh7760/offset.h"から作成。
#|□2015/12/22	｢TCBsz_wtmeb2istack｣の定義を
#|  (28+TCBSZ_MTX+TCBSZ_POR+60)→(28+TCBSZ_MTX+TCBSZ_POR+76)に修正。
#|  TCB(Task control block)構造体の、slicetime～utimeまでの型が、
#|  RELTIM_U(long long)に{V2}から変更された為に合計で16byte増えている。
#|  ("kernel/src/task.h"を参照)
#|
#endif
