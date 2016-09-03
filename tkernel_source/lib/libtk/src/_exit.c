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
 */

/*
 *	@(#)_exit.c (libtk)
 */

#include <basic.h>
#include <tk/syscall.h>

#ifdef __GNUC__
extern void tk_ext_tsk(void) __attribute__ ((noreturn));
#else
extern void tk_ext_tsk(void);
#endif

EXPORT void _exit(int status)
{
	tk_ext_tsk();
}
