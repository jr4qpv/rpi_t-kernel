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
 *	@(#)call.h (libtk)
 *
 *	Kernel function call
 */

#include <basic.h>
#include <tk/syscall.h>
#include <sys/commarea.h>

#if TA_GP

IMPORT INT _CallKernelFunc( INT p1, INT p2, FP func, void *gp );

#define CallKernelFunc(func, p1, p2)	\
	_CallKernelFunc((INT)(p1), (INT)(p2), (FP)(func), __CommArea->gp)

#else /* TA_GP */

#define CallKernelFunc(func, p1, p2)	(*func)(p1, p2)

#endif /* TA_GP */
