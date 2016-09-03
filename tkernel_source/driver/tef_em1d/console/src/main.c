/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/03/01.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 */

/*
 *	main.c	Console/Low-level serial I/O driver
 *
 *	Main entry  : system-independent
 */

#include <basic.h>
#include <tk/tkernel.h>

/* Console driver */
IMPORT	ER	console_startup( BOOL StartUp );

/* Low-level serial I/O driver */
IMPORT	ER	startup_serial_io( BOOL StartUp );

/*
 *	Register/Deregister the subsystem
 */
EXPORT	ER	con_def_subsys(W svc, W pri, void *svcent, void *brkent)
{
	T_DSSY	dssy;

	dssy.ssyatr    = TA_NULL;
	dssy.ssypri    = pri;
	dssy.svchdr    = (FP)svcent;
	dssy.breakfn   = (FP)brkent;
	dssy.startupfn = NULL;
	dssy.cleanupfn = NULL;
	dssy.eventfn   = NULL;
	dssy.resblksz  = 0;
	if (! svcent) return tk_def_ssy(svc, NULL);	/* Delete only */
	if (brkent) tk_def_ssy(svc, NULL);		/* Delete once */
	return tk_def_ssy(svc, &dssy);			/* Registration */
}

/*
 *	Console initialization/End processing entry
 */
EXPORT	ER	ConsoleIO( INT ac, UB *av[] )
{
	BOOL	start;

	start = (ac >= 0) ? TRUE : FALSE;

	/* Startup/Exit the low-level serial I/O */
	startup_serial_io(start);

	/* Startup/ Exit the console driver */
	console_startup(start);

	return E_OK;
}
