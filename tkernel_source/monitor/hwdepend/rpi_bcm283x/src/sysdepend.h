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
 *    Modified by T.Yokobayashi at 2016/03/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdepend.h
 *
 *       system-related definitions: ARM CPUs.
 */

#ifndef __MONITOR_CMDSVC_SYSDEPEND_H__
#define	__MONITOR_CMDSVC_SYSDEPEND_H__

#include "hwdepend.h"
#include <sys/sysinfo.h>
#include <sys/rominfo.h>
#include "setup_bcm283x.h"

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

#endif /* __MONITOR_CMDSVC_SYSDEPEND_H__ */


/*----------------------------------------------------------------------
#|History of "sysdepend.h"
#|========================
#|* 2016/03/02	[rpi_bcm283x]用に、[tef_em1d]用の"sysdepend.h"を参考に作成。
#|
*/
