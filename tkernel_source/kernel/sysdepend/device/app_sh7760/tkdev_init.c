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
 */

/*
 *	tkdev_init.c (SH7760)
 *	T-Kernel Device-Dependent Initialization/Finalization
 */

#include "kernel.h"
#include <tk/sysdef.h>
#include <tk/syslib.h>
#include <tm/tmonitor.h>

/*
 * Target system-dependent initialization
 */
EXPORT ER tkdev_initialize( void )
{
	/* Disable all interrupts */
	out_h(IPRA, 0);
	out_h(IPRB, 0);
	out_h(IPRC, 0);
	out_h(IPRD, 0);
	out_w(INTPRI00, 0);
	out_w(INTPRI04, 0);
	out_w(INTPRI08, 0);
	out_w(INTPRI0C, 0);
	out_w(INTMSK00, 0xf3ff7fff);
	out_w(INTMSK04, 0x00ff807f);

	return E_OK;
}

/*
 Target system-dependent finalization
 *	Normally jump to ROM monitor.
 *	No return from this function.
 */
EXPORT void tkdev_exit( void )
{
	disint();
	tm_exit(0);	/* Turn off power and exit */

	/* Not suppose to return from 'tm_exit,' but just in case */
	for ( ;; ) {
		tm_monitor();  /* To T-Monitor */
	}
}
