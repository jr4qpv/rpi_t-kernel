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
 *	@(#)getsvcenv.h (libtk/SH7760)
 *
 *	Get extended SVC call environment 
 */

Inline UW getsvcenv( void )
{
	UW	mdr;
	Asm("stc r2_bank, %0": "=r"(mdr));
	return mdr;
}
