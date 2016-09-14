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
 *	cpu_insn.h (SH7760)
 *	CPU-Dependent Operation
 */

#ifndef _CPU_INSN_
#define _CPU_INSN_

#include <sys/sysinfo.h>

/* ------------------------------------------------------------------------ */
/*
 *	Control register operation
 */

/*
 * Get SR
 */
Inline UINT getSR( void )
{
	UINT	sr;
	Asm("stc sr, %0" : "=r"(sr));
	return sr;
}

/* ------------------------------------------------------------------------ */
/*
 *	EIT-related
 */

/*
 * Monitor use vector number
 */
#define VECNO_TRAPA	( 0x02c / 4 )	/* TRAPA exception handler */
#define VECNO_BREAK	( 0x03c / 4 )	/* User break point handler */
#define VECNO_DEFAULT	( 0x200 / 4 )	/* Default handler */
#define VECNO_TLBMISS	( 0x204 / 4 )	/* TLB miss exception handler */
#define VECNO_MONITOR	( 0x240 / 4 )	/* Monitor service call */

/*
 * For saving monitor exception handler
 */
typedef struct monhdr {
	FP	trapa_hdr;
	FP	break_hdr;
	FP	monitor_hdr;
	FP	default_hdr;
	FP	tlbmiss_hdr;
} MONHDR;

/* For saving monitor exception handler */
IMPORT MONHDR	SaveMonHdr;

/*
 * Set interrupt handler
 */
Inline void define_inthdr( INT vecno, FP inthdr )
{
	SCArea->intvec[vecno] = inthdr;
}

IMPORT VW	*int_stack_top;	/* EIT stack top */

/*
 * If it is the task-independent part, TRUE
 *	If EIT stack is consumed, the task independent part
 */
Inline BOOL isTaskIndependent( void )
{
	VW	*isp;
	Asm("stc r6_bank, %0" : "=r"(isp));
	return ( isp != int_stack_top )? TRUE: FALSE;
}

/*
 * Move to/Restore task independent part
 */
Inline void EnterTaskIndependent( void )
{
	VW	*isp;
	Asm("stc r6_bank, %0": "=r"(isp));
	Asm("ldc %0, r6_bank":: "r"(isp - 2));
}
Inline void LeaveTaskIndependent( void )
{
	VW	*isp;
	Asm("stc r6_bank, %0": "=r"(isp));
	Asm("ldc %0, r6_bank":: "r"(isp + 2));
}

/* ------------------------------------------------------------------------ */

#endif /* _CPU_INSN_ */
