/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/02/28.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/09/08.
 *
 *----------------------------------------------------------------------
 */

/*
 *	inittask_main.c (sysmain)
 *	Initial Task
 */

#include "sysmain.h"
#include "kernel.h"
#include <sys/rominfo.h>
#include <sys/debug.h>

typedef INT	(*MAIN_FP)(INT, UB **);

/* ------------------------------------------------------------------------ */

/*
 * Initial task Main
 *	The available stack size is slightly less than 8KB.
 *	The initial task is the system task,
 *	so it should not be deleted.
 */
EXPORT INT init_task_main( void )
{
	INT	fin;
	MAIN_FP	adr;

	/* Start message */
#if USE_KERNEL_MESSAGE
 #ifdef _TEF_EM1D_
	tm_putstring((UB*)BOOT_MESSAGE);
 #else
extern char const * const TitleAPP[];		/* ブートメッセージ */
	tm_putstring((UB*)"\n");
	tm_putstring((UB*)TitleAPP[0]);
	tm_putstring((UB*)"\n\n");
 #endif
#endif

	fin = 1;
	adr = (MAIN_FP)ROMInfo->userinit;
	if ( adr != NULL ) {
		/* Perform user defined initialization sequence */
		fin = (*adr)(0, NULL);
	}
	if ( fin > 0 ) {
		/* Perform user main */
		fin = usermain();
	}
	if ( adr != NULL ) {
		/* Perform user defined finalization sequence */
		(*adr)(-1, NULL);
	}

	return fin;
}


/*----------------------------------------------------------------------
#|History of "inittask_main.c"
#|============================
#|* 2016/09/08	起動時にTitleAPP[0]の文字列を表示するようにした。
#|
*/
