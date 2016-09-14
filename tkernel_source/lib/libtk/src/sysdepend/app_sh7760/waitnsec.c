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
 *	@(#)waitnsec.c (libtk/SH7760)
 *
 *	Busy loop wait time in nanoseconds 
 */

#include <basic.h>
#include <sys/sysinfo.h>

EXPORT void WaitNsec( UINT nsec )
{
	UW	count = nsec * SCInfo.loop64us / 64000U;

	Asm("	loop:	tst	%0, %0	\n"
	"		bf/s	loop	\n"
	"		add	#-1, %0	"
		: "=r"(count)
		: "0"(count)
	);
}
