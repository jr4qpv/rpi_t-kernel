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
 *	@(#)prreg.c (libtk/SH7760)
 *
 *	Display task register 
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <sys/misc.h>

/*
 * Uses prfn to display the contents of gr, er, and cr.
 * prfn must be a printf compatible function.
 */
EXPORT W PrintTaskRegister( int (*prfn)( const char *format, ... ),
				T_REGS *gr, T_EIT *er, T_CREGS *cr )
{
/*
 *	PC: 12345678 SR: 12345678 MDR:12345678
 *	R0: 12345678 R1: 12345678 R2: 12345678 R3: 12345678
 *	R4: 12345678 R5: 12345678 R6: 12345678 R7: 12345678
 *	R8: 12345678 R9: 12345678 R10:12345678 R11:12345678
 *	R12:12345678 R13:12345678 R14:12345678
 *	MAC:12345678 12345678     GBR:12345678 PR: 12345678
 *	USP:12345678 SSP:12345678 LSID:1234   UATB:12345678
 */
	(*prfn)("PC: %08x SR: %08x MDR:%08x\n",
		(UW)er->pc, er->sr, er->mdr);
	(*prfn)("R0: %08x R1: %08x R2: %08x R3: %08x\n",
		gr->r[0], gr->r[1], gr->r[2], gr->r[3]);
	(*prfn)("R4: %08x R5: %08x R6: %08x R7: %08x\n",
		gr->r[4], gr->r[5], gr->r[6], gr->r[7]);
	(*prfn)("R8: %08x R9: %08x R10:%08x R11:%08x\n",
		gr->r[8], gr->r[9], gr->r[10], gr->r[11]);
	(*prfn)("R12:%08x R13:%08x R14:%08x\n",
		gr->r[12], gr->r[13], gr->r[14]);
	(*prfn)("MAC:%08x %08x     GBR:%08x PR: %08x\n",
		gr->mach, gr->macl, gr->gbr, gr->pr);
	(*prfn)("USP:%08x SSP:%08x LSID:%-4d   UATB:%08x\n",
		(UW)cr->usp, (UW)cr->ssp, cr->lsid, cr->uatb);
	return 7;  /* Number of display rows */
}
