/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/01/12.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 */

/*
 *	tkstart.c (T-Kernel/OS)
 *	T-Kernel Startup / Finalization
 */

#include "kernel.h"
#include "timer.h"
#include "task.h"
#include <tm/tmonitor.h>

/*
 * Start initial task
 */
LOCAL void init_task_startup( T_CTSK *ctsk )
{
	ER	ercd;

#if TA_GP
	extern int _gp;
	ercd = _tk_cre_tsk(ctsk, 0,0,0,0,&_gp);
#else
	ercd = _tk_cre_tsk(ctsk);
#endif
	if ( ercd < E_OK ) {
		goto err_exit;
	}

	ercd = _tk_sta_tsk((ID)ercd, 0);
	if ( ercd < E_OK ) {
		goto err_exit;
	}

	return;

err_exit:
#if USE_KERNEL_MESSAGE
	tm_putstring((UB*)"init_task can not started\n");
#endif
	tm_monitor();
}

/*
 * Call module initialization
 */
LOCAL void init_module( ER (*initfunc)( void ), UB *name )
{
	ER	ercd;

	ercd = (*initfunc)();
	if ( ercd < E_OK ) {
#if USE_KERNEL_MESSAGE
		tm_putstring(name);
		tm_putstring((UB*)" : module initialize error\n");
#endif
		tm_monitor();
	}
}
#define InitModule(name)	init_module( name##_initialize, (UB*) #name )

/*
 * Initialize kernel and create/start initial task
 */
EXPORT void t_kernel_main( T_CTSK *inittask )
{
IMPORT	void DispProgress( W n );	/* sysinit */

	DISABLE_INTERRUPT;

	/*
	 * Target-dependent initialization
	 */
	DispProgress(0x20);
	InitModule(cpu);
	InitModule(tkdev);

	/*
	 * Each module initialization
	 */
	DispProgress(0x21);
	InitModule(task);
#ifdef NUM_SEMID
	InitModule(semaphore);
#endif
#ifdef NUM_FLGID
	InitModule(eventflag);
#endif
#ifdef NUM_MBXID
	InitModule(mailbox);
#endif
#ifdef NUM_MBFID
	InitModule(messagebuffer);
#endif
#ifdef NUM_PORID
	InitModule(rendezvous);
#endif
#ifdef NUM_MTXID
	InitModule(mutex);
#endif
#ifdef NUM_MPLID
	InitModule(memorypool);
#endif
#ifdef NUM_MPFID
	InitModule(fix_memorypool);
#endif
#ifdef NUM_CYCID
	InitModule(cyclichandler);
#endif
#ifdef NUM_ALMID
	InitModule(alarmhandler);
#endif
	InitModule(subsystem);
	InitModule(resource_group);
	InitModule(timer);

	/*
	 * Create/start initial task
	 */
	DispProgress(0x22);
	init_task_startup(inittask);

	/*
	 * Dispatch to initial task and start kernel operation.
	 */
	DispProgress(0x23);
	force_dispatch();
	/* No return */
}

/*
 * Finalization
 */
EXPORT void t_kernel_shutdown( void )
{
	timer_shutdown();
	cpu_shutdown();
}

/*
 * Execute finalization and stop
 */
EXPORT void t_kernel_exit( void )
{
	t_kernel_shutdown();
	tkdev_exit();
	/* No return */
}
