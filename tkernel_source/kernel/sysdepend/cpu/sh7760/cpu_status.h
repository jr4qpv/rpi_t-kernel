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
 *	cpu_status.h (SH7760)
 *	CPU-Dependent Definition
 */

#ifndef _CPU_STATUS_
#define _CPU_STATUS_

#include <tk/syslib.h>
#include <tk/sysdef.h>
#include "cpu_insn.h"

/*
 * Start/End critical section
 */
#define BEGIN_CRITICAL_SECTION	{ UINT _sr_ = disint();
#define END_CRITICAL_SECTION	if ( !isDI(_sr_)			\
				  && ctxtsk != schedtsk			\
				  && !isTaskIndependent()		\
				  && !dispatch_disabled ) {		\
					dispatch();			\
				}					\
				enaint(_sr_); }

/*
 * Start/End interrupt disable section
 */
#define BEGIN_DISABLE_INTERRUPT	{ UINT _sr_ = disint();
#define END_DISABLE_INTERRUPT	enaint(_sr_); }

/*
 * Interrupt enable/disable
 */
#define ENABLE_INTERRUPT	{ enaint(0); }
#define DISABLE_INTERRUPT	{ disint(); }

/*
 * Enable interrupt nesting
 *	Enable the interrupt that has a higher priority than 'level.'
 */
#define ENABLE_INTERRUPT_UPTO(level)	{ enaint(SR_I(level)); }

/*
 * Move to/Restore task independent part
 */
#define ENTER_TASK_INDEPENDENT	{ EnterTaskIndependent(); }
#define LEAVE_TASK_INDEPENDENT	{ LeaveTaskIndependent(); }

/* ----------------------------------------------------------------------- */
/*
 *	Check system state
 */

/*
 * When a system call is called from the task independent part, TRUE
 */
#define in_indp()	( isTaskIndependent() || ctxtsk == NULL )

/*
 * When a system call is called during dispatch disable, TRUE
 * Also include the task independent part as during dispatch disable.
 */
#define in_ddsp()	( dispatch_disabled	\
			|| in_indp()		\
			|| isDI(getSR()) )

/*
 * When a system call is called during CPU lock (interrupt disable), TRUE
 * Also include the task independent part as during CPU lock.
 */
#define in_loc()	( isDI(getSR())		\
			|| in_indp() )

/*
 * When a system call is called during executing the quasi task part, TRUE
 * Valid only when in_indp() == FALSE because it is not discriminated from
 * the task independent part.
 */
#define in_qtsk()	( ctxtsk->sysmode > ctxtsk->isysmode )

/* ----------------------------------------------------------------------- */
/*
 *	Task dispatcher startup routine
 */

/*
 * Request for task dispatcher startup
 *	Do nothing at this point because there is no delayed interrupt
 *	function in SH.
 *	Perform dispatcher startup with END_CRITICAL_SECTION.
 */
#define dispatch_request()	/* */

/*
 * Throw away the current task context
 * and forcibly dispatch to the task that should be performed next.
 *	Use at system startup and 'tk_ext_tsk, tk_exd_tsk.'
 */
Inline void force_dispatch( void )
{
IMPORT	void	dispatch_to_schedtsk();

	Asm("	jmp @%0		\n"
	"	nop		"
		:: "r"(&dispatch_to_schedtsk));
}

/*
 * Start task dispatcher
 */
Inline void dispatch( void )
{
	Asm("trapa %0":: "i"(TRAP_DISPATCH));
}

/* ----------------------------------------------------------------------- */
/*
 *	Task exception
 */

/*
 * Task exception handler startup reservation
 */
IMPORT void request_tex( TCB *tcb );

/* ----------------------------------------------------------------------- */

/*
 * Task context block
 */
typedef struct {
	VP	ssp;		/* System stack pointer */
	VP	uatb;		/* Task space page table */
	INT	lsid;		/* Task space ID */
} CTXB;

/*
 * CPU information
 */
IMPORT ATR	available_cop;	/* Enabled coprocessor (TA_COPn) */

#endif /* _CPU_STATUS_ */
