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
 *    Modified by T.Yokobayashi at 2015/11/24.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)cpudepend.h () 2016/01/21
 *
 *       SH4-related definitions
 */

#ifndef __MONITOR_SH4_CPUDEPEND_H__
#define __MONITOR_SH4_CPUDEPEND_H__

#include <machine.h>

/*
 * monitor stack area
 * stack area is from     &__stack_top to &__stack_bottom
 *       initial stack pointer = &__stack_bottom
 */
IMPORT UB	__stack_top, __stack_bottom;

/*
 * first level page table
 */
//////IMPORT UW* const	TopPageTable;	/* location of page table */
//////#define	N_PageTableEntry	0x1000	/* number of entries */

/*
 * address conversion to non-cached and cached area
 *      in the case of SH7760, all address have the same cache mode, and hence
 *       return as it is.
 */
#define NOCACHE_ADDR(p)	( (VP)((UW)(p) | 0x20000000U) )
#define CACHE_ADDR(p)	( (VP)((UW)(p) & ~0x20000000U) )

/*
 * I/O port access functions
 */
#if 0	///↓ include/tk/sysdepend/app_sh7760/syslib_depend.h に記述ある ↓///
Inline void out_w( INT port, UW data )
{
	*(_UW*)port = data;
}
Inline void out_h( INT port, UH data )
{
	*(_UH*)port = data;
}
Inline void out_b( INT port, UB data )
{
	*(_UB*)port = data;
}

Inline UW in_w( INT port )
{
	return *(_UW*)port;
}
Inline UH in_h( INT port )
{
	return *(_UH*)port;
}
Inline UB in_b( INT port )
{
	return *(_UB*)port;
}
#endif	////↑↑↑/////


/*
 * value of control register (r1) of system control coprocessor cp15
 */
#if CPU_SH7760
#define	MASK_CACHEMMU	(0x00000000)		/* ///kari V,I,R,S,C,A,M (B = 0)   */
#define	VALID_CACHEMMU	(0x00000000)		/* ///kari B = 0		   */
#define	DIS_CACHEMMU	(0x00000000)		/* ///kari I=0,R=0,S=0,C=0,A=0,M=0 */
#define	DIS_CACHEONLY	(0x00000000)		/* ///kari I=0,R=0,S=0,C=0,A=0,M=1 */
#define	ENB_CACHEMMU	(0x00000000)		/* ///kari I=1,R=0,S=0,C=1,A=1,M=1 */
#define	ENB_MMUONLY		(0x00000000)		/* ///kari I=0,R=0,S=0,C=0,A=1,M=1 */
#endif

///#if CPU_ARM1176
///#define	MASK_CACHEMMU	(0xFFFFCC78)		/* V,I,R,S,C,A,M (B = 0)   */
///#define	VALID_CACHEMMU	(0x3307)		/* B = 0		   */
///#define	DIS_CACHEMMU	(0x0000)		/* I=0,R=0,S=0,C=0,A=0,M=0 */
///#define	DIS_CACHEONLY	(0x0001)		/* I=0,R=0,S=0,C=0,A=0,M=1 */
///#define	ENB_CACHEMMU	(0x1007)		/* I=1,R=0,S=0,C=1,A=1,M=1 */
///#define	ENB_MMUONLY	(0x0003)		/* I=0,R=0,S=0,C=0,A=1,M=1 */
///#endif



/*
 * references registers under monitor control
 *       references the value of registers at the time of monitor entry.
 */
IMPORT UW getCurPC( void );		/* PC register */
IMPORT UW getCurSR( void );		/* SR register */

///IMPORT UW getCP15( W reg, W opcd );	/* CP15 register CRn: reg, Op2: opcd */
///IMPORT UW getCurPCX( void );	/* PC register (raw value) */
///IMPORT UW getCurCPSR( void );	/* CPSR register */


#endif /* __MONITOR_SH4_CPUDEPEND_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|【cpudepend.h 変更履歴】
#|□2015/11/24	V2.02.04の"../arm/"からコピーし、
#|  [app_sh7760]用に修正
#|□2016/01/21	NOCACHE_ADDR,CACHE_ADDRのマクロを、
#|  "/kernel/sysdepend/device/std_sh7760/cache_info.h"を参考に記述追加。
#|
#endif
