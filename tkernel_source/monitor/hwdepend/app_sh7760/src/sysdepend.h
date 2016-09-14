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
 *    Modified by T.Yokobayashi at 2015/12/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdepend.h
 *
 *       system-related definitions: SH7760(SH-4) CPUs.
 */

#ifndef __MONITOR_CMDSVC_SYSDEPEND_H__
#define	__MONITOR_CMDSVC_SYSDEPEND_H__

#include "hwdepend.h"
#include <sys/sysinfo.h>
#include <sys/rominfo.h>
#include "setup_sh7760.h"


#ifdef __cplusplus
extern "C" {
#endif


/*** IOP-5基板 独自関数の参照定義 ***/

#if 0	////// C言語からは cpuLED()で代用できるので削除 //////
/* LED操作関係(sysdepend.c) */
IMPORT	void	LED_red_on(void);
IMPORT	void	LED_red_off(void);
IMPORT	void	LED_red_inv(void);
IMPORT	void	LED_green_on(void);
IMPORT	void	LED_green_off(void);
IMPORT	void	LED_green_inv(void);
#endif	//////////////////



/* 時間待ち関係(sysdepend.c) */
IMPORT	void	mon_wait_1ms(W count);




#if 0	/////////////////////////

/*
 * cache and MMU control
 */
IMPORT void setCacheMMU( UW cp15r1 );

/*
 * machine-dependent interrupt processing
 *       info is defined in machine-dependent manner.
 *       return value     0 : it is not the target of processing.
 *               1 : the object is the target of processing (the monitor should continue monitoring)
 *               2 : the object is the target of processing (exiting interrupt handler).
 */
IMPORT W procHwInt( UW info );

#endif	////////////////////////////



#ifdef __cplusplus
}
#endif
#endif /* __MONITOR_CMDSVC_SYSDEPEND_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|【sysdepend.h 改版履歴】
#|□2015/12/01	[app_sh7760]用に、[tef_em1d]用の"sysdepend.h"を参考に作成。
#|
#endif
