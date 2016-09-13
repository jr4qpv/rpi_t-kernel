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
 *    Modified by T.Yokobayashi at 2016/07/21.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)power.c (BCM283x) 2016/07/21
 *	Power-Saving Function
 */

#include "sysmgr.h"

/*
 * Switch to power-saving mode
 */
EXPORT void low_pow( void )
{
#ifdef	CPU_ARMv6
	Asm("mcr p15, 0, %0, cr7, c0, 4":: "r"(0));		// Wait For Interrupt
#else	/* CPU_ARMv7以降 */
	Asm("wfi");
#endif
}

/*
 * Move to suspend mode
 */
EXPORT void off_pow( void )
{
#ifdef	CPU_ARMv6
	Asm("mcr p15, 0, %0, cr7, c0, 4":: "r"(0));		// Wiai For Interrupt
#else	/* CPU_ARMv7以降 */
	Asm("wfi");
#endif
}


/*----------------------------------------------------------------------
#|History of "power.c"
#|=====================
#|* 2016/07/21	[rpi_bcm283x]用に、[tef_em1d]用の"power.c"を参考に作成。
#|
*/
