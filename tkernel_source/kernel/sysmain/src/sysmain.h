/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2011/09/08.
 *    Modified by T-Engine Forum at 2011/07/29.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysmain.h (sysmain)
 *	Kernel Main
 */

#ifndef _SYSMAIN_
#define _SYSMAIN_

#include <basic.h>
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

/* Boot message */
#define BOOT_MESSAGE \
	"\n" \
	"T-Kernel Version 2.02.00\n" \
	"\n\0"

/*
 Display the progress of start processing (sysinit)
 *	Display the value specified by 'n' on display or LED.
 *	Some platform may not support this function.
 */
IMPORT void DispProgress( W n );

/*
 * User main
 */
IMPORT INT usermain( void );

#endif /* _SYSMAIN_ */
