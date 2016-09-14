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
 *	cpu_task.h (SH7760)
 *	CPU-Dependent Task Start Processing
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

IMPORT TCB	*dsp_ctxtsk;		/* Task including DSP context */
IMPORT TCB	*fpu_ctxtsk;		/* Task including FPU context */

/*
 * System stack configuration at task startup
 */
typedef struct {
	struct _rng0_3 {		/* RNG 0-3 common */
		VW	mach;
		VW	macl;
		VP	pr;
		VP	gbr;
		VW	r[15];		/* bank0 R0-R7, R8-R14 */
		VW	bank1_r1;
		VW	bank1_r0;
		VP	spc;
		UW	ssr;
		UW	mdr;		/* bank1 R2 */
	} rng0_3;
	struct _rng1_3 {	/* Only RNG 1-3 */
		VP	usp;
	} rng1_3;
} SStackFrame;

#define SizeofSS0	( sizeof(SStackFrame) - sizeof(VP) )	/* RNG 0 */
#define SizeofSS1_3	( sizeof(SStackFrame) )			/* RNG 1-3 */

/*
 * User stack configuration at task startup (only RNG 1-3)
 */
typedef struct {
	/* Empty */
} UStackFrame;

/*
 * Size of system stack area destroyed by 'make_dormant()'
 * In other words, the size of area required to write by 'setup_context().'
 */
#define DORMANT_STACK_SIZE	( sizeof(VW) * 4 )	/* To 'spc' */

/*
 * Size of area kept for special use from system stack
 */
#if USE_SH3_DSP
#define RESERVE_SSTACK(tskatr)	\
		( ( ((tskatr) & TA_COP0) != 0 )? sizeof(T_COP0REGS): 0 )
#elif TA_FPU
#define RESERVE_SSTACK(tskatr)	\
		( ( ((tskatr) & TA_FPU) != 0 )? sizeof(T_COP0REGS): 0 )
#else
#define RESERVE_SSTACK(tskatr)	0
#endif

/*
 * Initial value for task startup
 */
#if USE_MMU
#define INIT_SR(rng)	( ( (rng) == 3 )? 0: SR_MD )
#else
#define INIT_SR(rng)	( SR_MD )
#endif

#define INIT_MDR(rng)	( MDR_PPL(rng) | MDR_CPL(rng) )

#define INIT_DSP_DSR	0		/* DSP register initial value */
#define INIT_FPSCR	0x000c0000	/* FPU register initial value */

/*
 * Switch Task Space
 */
Inline void change_space( VP uatb, INT lsid )
{
	*(_UW*)TTB  = (UW)uatb;
	*(_UW*)PTEH = (UW)lsid;
}

/*
 * Create stack frame for task startup
 *	Call from 'make_dormant()'
 */
Inline void setup_context( TCB *tcb )
{
	SStackFrame	*ssp;
	T_COP0REGS	*cp0;	/* SH3:DSP */
	W		rng;
	UW		sr;

	rng = (tcb->tskatr & TA_RNG3) >> 8;
	cp0 = tcb->isstack;
	ssp = (SStackFrame*)((UB*)(tcb->isstack) - (UB*)((rng==0)? SizeofSS0:SizeofSS1_3));

	sr = INIT_SR(rng);

	if ( (tcb->tskatr & TA_COP0) != 0 ) {
#if USE_SH3_DSP
		/* DSP context initialization */
		cp0->dsr = INIT_DSP_DSR;
		sr |= SR_DSP;
#endif
#if TA_FPU
		/* FPU context initialization */
		cp0->fpscr = INIT_FPSCR;
#endif
	}

	/* CPU context initialization */
	ssp->rng0_3.mdr = INIT_MDR(rng);	/* Initial MDR */
	ssp->rng0_3.ssr = sr;			/* Initial SR */
	ssp->rng0_3.spc = tcb->task;		/* Task startup address */
	tcb->tskctxb.ssp = ssp;			/* System stack */

	if ( rng > 0 ) {
		ssp->rng1_3.usp = tcb->istack;	/* User stack */
	}
}

/*
 * Set task startup code
 *	Called by 'tk_sta_tsk()' processing.
 */
Inline void setup_stacd( TCB *tcb, INT stacd )
{
	SStackFrame	*ssp = tcb->tskctxb.ssp;

	ssp->rng0_3.r[4] = stacd;
	ssp->rng0_3.r[5] = (VW)tcb->exinf;
}

/*
 * Delete task contexts
 */
Inline void cleanup_context( TCB *tcb )
{
#if USE_SH3_DSP
	/* If the current DSP context belongs to 'tcb,'
	   Disable the DSP context */
	if ( dsp_ctxtsk == tcb ) {
		dsp_ctxtsk = NULL;
	}
#endif
#if TA_FPU
	/* If the current FPU context belongs to 'tcb,'
	   Disable the FPU context */
	if ( fpu_ctxtsk == tcb ) {
		fpu_ctxtsk = NULL;
	}
#endif
}

#endif /* _CPU_TASK_ */
