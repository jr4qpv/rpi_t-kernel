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
 *	@(#)segment_depend.h (sys/SH7760)
 *
 *	Segment management system dependent definitions (SH7760)
 */

#ifndef __SYS_SEGMENT_DEPEND_H__
#define __SYS_SEGMENT_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Access level definitions */
typedef enum {
	MapUser		= 0x02,
	MapSystem	= 0x00,
	MapRead		= 0x00,
	MapWrite	= 0x01,
	MapExecute	= 0x00
} MapModeLevel;

/*
 * MapMemory() attr
 */
#define MM_USER		0x40U	/* User */
#define MM_SYSTEM	0x00U	/* System */
#define MM_READ		0x00U	/* Read */
#define MM_WRITE	0x20U	/* Write */
#define MM_EXECUTE	0x00U	/* Execute */
#define MM_CDIS		0x08U	/* Cache disabled */

#ifdef __cplusplus
}
#endif
#endif /* __SYS_SEGMENT_DEPEND_H__ */
