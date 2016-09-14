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
 *    Modified by T.Yokobayashi at 2016/01/22.
 *
 *----------------------------------------------------------------------
 */

/*
 *	monent.c
 *
 *       Entry to monitor
 */

#include "../cmdsvc.h"
#include <sys/sysinfo.h>

#include "register.h"


EXPORT W	bootFlag;	/* boot flag */

/*
 * monitor entry processing
 *       vec     exception vector number
 */
EXPORT void entMonitor( UW vec )
{
	UW	v;
///	W	bpflg;
///	UB	*cmd;
///	UW	save_taskmode;				/* //// sh7760では不要？？？///// */

	/* update task mode */
///	save_taskmode = SCInfo.taskmode;
///	SCInfo.taskmode <<= 16;

	/* monitor entry processing (flushing cache, etc.) */
	enterMonitor(0);					// see "hwdepend/cpuctrl.c"

	/* initialize address check data */
	initChkAddr();						// see "cmdsvc/sh4/chkaddr.c"

	/* release breakpoint temporarily */
///	bpflg = resetBreak(vec);			// see "cmdsvc/sh4/break.c"

	bootFlag = 0;

	switch ( vec ) {
	  case EIT_DEFAULT:					/* reset */
		if ( bootSelect() == BS_AUTO ) {
			/* automatic boot */
			if ( bootDisk(NULL) >= E_OK ) break; /* execute boot */
		}
		/* invoke monitor */
		dispTitle();					/* boot message */
		procCommand(NULL, 0);			/* command processing */
		break;

	  case EIT_MONITOR:					/* service call */
		/* Execute SVC: Parameters given are, r0(fn), r4, r5, r6, r7 */
		v = procSVC(getRegister(ixR0), getRegister(ixR4),
			getRegister(ixR5), getRegister(ixR6), getRegister(ixR7));

		/* At boot time, r0 holds the boot parameter,
			so don't change r0 */
		if ( bootFlag == 0 ) setRegister(ixR0, v);	/* result is set to 0 */
		break;

	  case EIT_USRBREAK:				/* debug user break trap */
///		if ( procBreak(bpflg, &cmd) > 0 ) {
///			procCommand(cmd, 0);
///		}
		break;

	  default:	/* unsupported instruction exception, interrupt, or traps */
		if ( procEIT(vec) == 0 ) {
///			stopTrace();				/* stop tracing */
			procCommand(NULL, 0);		/* command processing */
		}
	}

	/* set breakpoint */
///	setupBreak();

	/* monitor exit processing (flushing cache, etc.) */
	leaveMonitor(getCCR());				// see "hwdepend/cpuctrl.c"

	/* restore task mode */
///	SCInfo.taskmode = save_taskmode;

	return; /* returning leads to user program execution */
}


/*----------------------------------------------------------------------*/
#if 0
#|【monent.c 改版履歴】
#|□2015/12/12	[app_sh7760]用に、新規作成。
#|  [tef_em1d]用の"../armv6/monent.c"を参考に作成。
#|
#endif
