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
 *	@(#)cpudef.h (tk/SH7760)
 *
 *	SH7760 dependent definition
 */

#ifndef __TK_CPUDEF_H__
#define __TK_CPUDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * General purpose register		tk_get_reg tk_set_reg
 */
typedef struct t_regs {
	VW	r[15];	/* General purpose register R0-R14 */
	VW	mach;	/* Multiply accumulate register MACH */
	VW	macl;	/* Multiply accumulate register MACL */
	VP	gbr;	/* Global base register GBR */
	VP	pr;	/* Procedure register PR */
} T_REGS;

/*
 * Exception-related register		tk_get_reg tk_set_reg
 */
typedef struct t_eit {
	VP	pc;	/* Program counter */
	UW	sr;	/* Status register */
	UW	mdr;	/* Operation mode register R2_BANK1 */
} T_EIT;

/*
 * Control register			tk_get_reg tk_set_reg
 */
typedef struct t_cregs {
	VP	sstop;	/* System stack pointer initial value R3_BANK1 */
	VP	ssp;	/* System stack pointer R15 */
	VP	usp;	/* User stack pointer R15 */
	VP	uatb;	/* Address of task specific space page table */
	UW	lsid;	/* Task logical space ID */
	VW	r0;	/* R0_BANK1 register */
	VW	r1;	/* R1_BANK1 register */
} T_CREGS;

/*
 * Coprocessor register			tk_get_cpr tk_set_cpr
 */
typedef struct t_cop0regs {
	float	fr[16];	/* FPU BANK 0 register FR0 - 15 */
	float	xf[16];	/* FPU BANK 1 register XF0 - 15 */
	UW	fpul;	/* communication register */
	UW	fpscr;	/* status/control register */
} T_COP0REGS;

typedef union {
	T_COP0REGS	cop0;	/* DSP */
#if 0
	T_COP1REGS	cop1;
	T_COP2REGS	cop2;
	T_COP3REGS	cop3;
#endif
} T_COPREGS;

#ifdef __cplusplus
}
#endif
#endif /* __TK_CPUDEF_H__ */
