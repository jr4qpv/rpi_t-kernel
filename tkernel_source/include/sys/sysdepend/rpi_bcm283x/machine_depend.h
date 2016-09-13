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
 *    Modified by T.Yokobayashi at 2016/03/01.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)machine_depend.h (sys/BCM283x) 2016/08/31
 *
 *	Machine type definition
 */

#ifndef __SYS_MACHINE_DEPEND_H__
#define __SYS_MACHINE_DEPEND_H__

/*
 * CPU_xxxx		CPU type
 * ALLOW_MISALIGN	1 if access to misalignment data is allowed
 * BIGENDIAN		1 if big endian
 * VIRTUAL_ADDRESS	1 if virtual memory
 * ALLOCA_NOSPT		1 if alloca() is not supported
 */

/* ----- Raspberry Pi BCM283x definition ----- */
#undef _RPI_BCM283x_

#define _RPI_BCM283x_		1

#if (TYPE_RPI == 1)				/*** BCM2835 ***/
#define CPU_ARM1176			1
#define CPU_ARMv6			1
#define CPU_BCM2835			1

#elif (TYPE_RPI == 2)			/*** BCM2836 ***/
#define CPU_CORTEX_A7		1
#define CPU_ARMv7			1
#define CPU_BCM2836			1

#elif (TYPE_RPI == 3)			/*** BCM2837(RPI2と同じ扱いにする) ***/
#define CPU_CORTEX_A53		1
#define CPU_ARMv7			1	/* 32bitモードで実行 */
#define CPU_BCM2837			1
#else
#error TYPE_RPI undefine error.
#endif

#define CPU_ARM				1
#define RPI_BCM283x			1
#define ALLOW_MISALIGN		0
#define BIGENDIAN			0
#define VIRTUAL_ADDRESS		1
#define ALLOCA_NOSPT		0

#ifndef _Csym
#define _Csym				0
#endif


/* --- etc ----- */
#define	_TEMPOLALY_DELETE_	0		/* 暫定的な削除コード */
#define	_TEMPOLALY_INSERT_	1		/* 暫定的な追加コード */


#endif /* __SYS_MACHINE_DEPEND_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|History of "machine_depend.h"
#|=============================
#|* 2016/03/01	"../tef_em1d/"から、[rpi_bcm283x]用にコピー
#|* 2016/03/01	_TEF_EM1D_ → _RPI_BCM283x_ に変更
#|* 2016/03/01	CPU_ARM1176 → CPU_ARM1167 又は CPU_CORTEX_A7 に変更
#|* 2016/03/01	CPU_ARMv6 → CPU_ARMv6 又は CPU_ARMv7 に変更
#|* 2016/03/01	TEF_EM1D → RPI_BCM283x に変更
#|* 2016/03/04	_TEMPOLALY_DELETE_,_TEMPOLALY_INSERT_ 定義の追加
#|* 2016/04/12	｢TYPE_RPI｣を導入し、Raspberry機種を選択
#|* 2016/04/13	CPU_BCM2835 又は CPU_BCM2836 定義の追加
#|* 2016/08/20	Raspberry機種判断は｢TYPE_RPI｣を参照し、TYPE_RPI=3に対応。
#|* 2016/08/24	TYPE_RPI=3時、CPU_CORTEX_A53,CPU_BCM2837定義追加
#|
#endif
