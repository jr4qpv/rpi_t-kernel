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
 *
 *    Modified by T.Yokobayashi at 2016/04/14.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sysinfo_depend.h (sys/BCM283x) 2016/04/14
 *
 *	System common information
 *
 *   0x10004000 +-----------------------+
 *              |Exception vector table |
 *   0x10004400 +-----------------------+
 *              |System common info.    |
 *   0x10004440 +-----------------------+
 */

#ifndef __SYS_SYSINFO_DEPEND_H__
#define __SYS_SYSINFO_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _in_asm_source_

/*
 * Boot mode/Operation mode
 */
typedef union {
	struct {
		UW	debug:1;	/* When in debug mode 1 */
		UW	fsrcv:1;	/* When in disk repair mode 1 */
		UW	rsv:12;		/* Reserved (always 0) */
		UW	basic:1;	/* When in basic operation mode 1 */
		UW	rsv2:17;	/* Reserved (always 0) */
	} c;
	UW	w;
} BootMode;

#define BM_DEBUG	0x00000001U	/* Debug mode */
#define BM_FSRCV	0x00000002U	/* Disk repair mode */
#define BM_BASIC	0x00004000U	/* Basic operation mode */

/*
 * System shared information
 */
typedef struct {
	void		*ramtop;	/* RAM free space top */
	void		*ramend;	/* RAM free space end */
	UB		*sysconf;	/* SYSCONF top */
	UB		*devconf;	/* DEVCONF top */
	W		taskindp;	/* Task independent context flag */
	UW		taskmode;	/* Task mode flag */
	BootMode	bm;		/* Boot mode */
	UW		loop64us;	/* Loop count per 64 micro sec */
	UB		bootdev[8];	/* Boot device name */
	VW		rsv[6];		/* Reserved (always 0) */
} SysCommonInfo;

/*
 * System common area
 */
#define N_INTVEC	256
typedef struct {
	FP		intvec[N_INTVEC];	/* EIT vector */
	SysCommonInfo	scinfo;			/* System common information */
} SysCommonArea;

#define SCArea	( (SysCommonArea*)0x10004000 )
#define SCInfo	( SCArea->scinfo )

#endif /* _in_asm_source_ */

/* Definitions used by assembler */
#define TASKINDP	(0x10004400 + 4*4)	/* Task independent flag */
#define TASKMODE	(0x10004400 + 5*4)	/* Task mode flag */

/*
 * Vector table address
 */
#define EIT_VECTBL	0x10004000	/* Vector table top */

#define EITVEC(n)	( EIT_VECTBL + (n) * 4 )

#define EIT_DEFAULT	0		/* default handler */
#define EIT_UNDEF	1		/* undefined instruction */
#define EIT_IABORT	2		/* prefetch abort */
#define EIT_DABORT	3		/* data abort */
#define EIT_IDEBUG	29		/* debug abort instruction */
#define EIT_DDEBUG	30		/* debug abort data */
#define EIT_FIQ		31		/* fast interrupt FIQ */
#define EIT_IRQ(n)	(  32 + (n) )	/* interrupt		IRQ  0-127 */
#define EIT_GPIO(n)	( 128 + (n) )	/* GPIO interrupt	port 0-127 */

#ifdef __cplusplus
}
#endif
#endif /* __SYS_SYSINFO_DEPEND_H__ */


/*----------------------------------------------------------------------
#|History of "sysinfo_depend.h"
#|======================
#|* 2016/04/14	It's copied from "../tef_em1d/" and it's modified.
#|
*/
