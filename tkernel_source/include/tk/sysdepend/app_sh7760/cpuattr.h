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
 *	@(#)cpuattr.h (tk/SH7760)
 *
 *	Type Specific Function
 */

#ifndef __TK_CPUATTR_H__
#define __TK_CPUATTR_H__

/*
 * Global pointer support
 *   0: No global pointer support
 */
#define TA_GP		0		/* No global pointer support */

/*
 * Using FPU (depend on CPU)
 *   0: not using FPU
 *   TA_COPn(n = 0-3): using FPU
 */
#define TA_FPU		TA_COP0

#endif /* __TK_CPUATTR_H__ */
