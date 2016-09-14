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
 *	@(#)sysexc_depend.h (sys/SH7760)
 *
 *	System exception processing (SH7760)
 */

#ifndef __SYS_SYSEXC_DEPEND_H__
#define __SYS_SYSEXC_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/* System exception message */
typedef struct {
	W	type;		/* Message type (MS_SYS1) */
	W	size;		/* Message size */
	ID	taskid;		/* Exception generated task ID */
	ID	procid;		/* Exception generated process ID */
	UW	exccode;	/* Exception code (EXPEVT, INTEVT2) */
	UW	excinfo;	/* Exception information (TRA, INTEVT) */
	UW	excaddr;	/* Exception address (TEA) */
} EXCMESG;

#ifdef __cplusplus
}
#endif

#endif /* __SYS_SYSEXC_DEPEND_H__ */
