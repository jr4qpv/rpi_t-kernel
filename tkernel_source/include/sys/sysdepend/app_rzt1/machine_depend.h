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
 *    Modified by T.Yokobayashi at 2016/03/14.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)machine_depend.h (sys/RZT1) 2016/04/14
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

/* ----- Renesas RZ/T1 definition ----- */
#undef _APP_RZT1_

#define _APP_RZT1_			1
#define CPU_CORTEX_R4		1
#define CPU_ARMv7			1
#define CPU_RZT1			1
#define CPU_ARM				1
#define APP_RZT1			1
#define ALLOW_MISALIGN		0
#define BIGENDIAN			0
#define VIRTUAL_ADDRESS		0
#define ALLOCA_NOSPT		0

#ifndef _Csym
#define _Csym				0
#endif


/* --- etc ----- */
#define	_TEMPOLALY_DELETE_	0		/* 暫定的な削除コード */
#define	_TEMPOLALY_INSERT_	1		/* 暫定的な追加コード */


#endif /* __SYS_MACHINE_DEPEND_H__ */


/*----------------------------------------------------------------------
#|History of "machine_depend.h"
#|=============================
#|* 2016/03/14	"../tef_em1d/"から、[app_rzt1]用にコピー
#|* 2016/03/14	_TEF_EM1D_ → _APP_RZT1_ に変更
#|* 2016/03/14	CPU_ARM1176 → CPU_CORTEX_R4 に変更
#|* 2016/03/14	CPU_ARMv6 → CPU_ARMv7 に変更
#|* 2016/03/14	TEF_EM1D → APP_RZT1 に変更
#|* 2016/03/14	_TEMPOLALY_DELETE_,_TEMPOLALY_INSERT_ 定義の追加
#|* 2016/04/14	VIRTUAL_ADDRESS 1→0 に変更
#|* 2016/04/13	CPU_RZT1 定義の追加
#|
*/
