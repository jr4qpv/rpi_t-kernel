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
 *	@(#)dbgspt_depend.h (tk/SH7760)
 *
 *	T-Kernel Debugger Support System-Dependent Definition (SH7760)
 */

#ifndef __TK_DBGSPT_DEPEND_H__
#define __TK_DBGSPT_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * System call/extension SVC caller information 
 */
typedef struct td_calinf {
	VP	ssp;		/* System stack pointer */
	VP	pr;		/* PR register when calling */
	VP	r14;		/* Frame pointer when calling */
} TD_CALINF;

#ifdef __cplusplus
}
#endif
#endif /* __TK_DBGSPT_DEPEND_H__ */
