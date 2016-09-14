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
 *    Modified by T.Yokobayashi at 2016/06/16.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)cpu_insn.h (RZT1) 2016/06/16
 *	RZT1 Dependent Operation
 */

#ifndef _CPU_INSN_
#define _CPU_INSN_

#include <sys/sysinfo.h>


/* ------------------------------------------------------------------------ */
/*
 *	Control register operation
 */

/*
 * Get CPSR
 */
Inline UINT getCPSR( void )
{
	UINT	cpsr;
	Asm("mrs %0, cpsr": "=r"(cpsr));
	return cpsr;
}

/*
 * TLB disable
 */
Inline void PurgeTLB( void )
{
#ifdef CPU_CORTEX_R4
	/* Cortex-R4 not define */
#else /* Cortex-A */
	Asm("mcr p15, 0, %0, cr8, c7, 0":: "r"(0));
	DSB(); ISB();
#endif
}

/* ------------------------------------------------------------------------ */
/*
 *	EIT-related
 */

/*
 * Vector numbers used by the T-Monitor
 */
#define VECNO_DEFAULT	EIT_DEFAULT	/* default handler */
#define VECNO_IDEBUG	EIT_IDEBUG	/* debug abort instruction */
#define VECNO_DDEBUG	EIT_DDEBUG	/* debug abort data */
#define VECNO_MONITOR	SWI_MONITOR	/* monitor service call */
//#define VECNO_ABORTSW	EIT_GPIO(8)	/* abort switch */

//#define VECNO_GIO0	EIT_IRQ(50)	/* Generic handler for GPIO interrupt */
//#define VECNO_GIO1	EIT_IRQ(51)
//#define VECNO_GIO2	EIT_IRQ(52)
//#define VECNO_GIO3	EIT_IRQ(53)
//#define VECNO_GIO4	EIT_IRQ(79)
//#define VECNO_GIO5	EIT_IRQ(80)
//#define VECNO_GIO6	EIT_IRQ(26)
//#define VECNO_GIO7	EIT_IRQ(27)

/*
 * To save monitor exception handler
 */
typedef struct monhdr {
	FP	default_hdr;		/* default handler */
	FP	idebug_hdr;		/* debug abort instruction */
	FP	ddebug_hdr;		/* debug abort data */
	FP	monitor_hdr;		/* monitor service call */
	FP	abortsw_hdr;		/* abort swiitch  */
	FP	gio_hdr[8];		/* Generic handler for GPIO interrupt */
} MONHDR;

/* For saving monitor exception handler */
IMPORT MONHDR	SaveMonHdr;

/*
 * Set interrupt handler
 */
Inline void define_inthdr( INT vecno, FP inthdr )
{
	SCArea->intvec[vecno] = inthdr;

///	if (vecno == 21) {						//// CMI0 interrupt address /////
///		VIC.VAD21.LONG = (unsigned long)inthdr;
///	}
}

/*
 * If it is the task-independent part, TRUE
 */
Inline BOOL isTaskIndependent( void )
{
	return ( SCInfo.taskindp > 0 )? TRUE: FALSE;
}

/*
 * Move to/Restore task independent part
 */
Inline void EnterTaskIndependent( void )
{
	SCInfo.taskindp++;
}
Inline void LeaveTaskIndependent( void )
{
	SCInfo.taskindp--;
}

/* ------------------------------------------------------------------------ */

#endif /* _CPU_INSN_ */


/*----------------------------------------------------------------------
#|History of "cpu_insh.h"
#|=======================
#|* 2016/06/16	[app_rzt1]用に、[tef_em1d]用の"cpu_insn.h"を参考に作成。
#|
*/
