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
 *	chkplv.c (SH7760)
 *	Call Protected Level Test
 */

#include "sysmgr.h"

IMPORT INT	svc_call_limit;	/* SVC protected level (T-Kernel/OS) */

Inline INT PPL( void )
{
	UW	mdr;
	Asm("stc r2_bank, %0": "=r"(mdr));
	return (mdr >> 16) & 3;
}

/*
 * Check protected level of extended SVC caller
 */
EXPORT ER ChkCallPLevel( void )
{
	return ( PPL() > svc_call_limit )? E_OACV: E_OK;
}
