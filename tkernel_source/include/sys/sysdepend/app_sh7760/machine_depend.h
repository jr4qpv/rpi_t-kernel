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
 *    Modified by T.Yokobayashi at 2015/11/24.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)machine_depend.h (sys/SH7760) 2016/03/17
 *
 *	Machine type definition (system dependent)
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

/* ----- App-Board/SH7760(SH4) definition ----- */
#undef _APP_SH7760_

#define _APP_SH7760_		1
#define CPU_SH4				1
#define CPU_SH7760			1
#define APP_SH7760			1
#define ALLOW_MISALIGN		0
#define BIGENDIAN			0
#define VIRTUAL_ADDRESS		1
#define ALLOCA_NOSPT		0

#ifndef _Csym
//#define _Csym				0
#define _Csym				1		/* for KIPT GCC */
#endif


#define	WDT_DOG_FOOD_ADDR	0xA8000000		/* CS2ｴﾘｱ (WDT) */
#define	WDT_dog_food()	(*(volatile char *)WDT_DOG_FOOD_ADDR)



#endif /* __SYS_MACHINE_DEPEND_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|History of "machine_depend.h"
#|=============================
#|* 2015/11/24	V1.02.04の"include/sys/sysdepend/std_sh7760/"から、
#|  [app_sh7760]用にコピー。(T.Yokobayashi)
#|* 2015/11/24	_STD_SH7760_ → _APP_SH7760_ に修正
#|* 2015/11/25	STD_SH7760 → APP_SH7760 に修正
#|* 2015/12/19	WDT_DOG_FOOD_ADDR定義の追加
#|* 2016/02/03	WDT_dog_food()マクロ定義の追加
#|
#endif
