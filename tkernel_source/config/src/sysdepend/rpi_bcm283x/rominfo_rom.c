/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/03/02.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/07/19.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)rominfo_rom.c (rpi_bcm283x) 2016/08/18
 *
 *	ROM information for ROM kernel
 */

#include <basic.h>
#include <sys/rominfo.h>

IMPORT UB SYSCONF[], DEVCONF[];

/* Kernel address */
#define RI_KERNEL_START	(FP)0x00030000	/* Kernel start address */
#define RI_SYSCONF	(UB*)SYSCONF	/* SYSCONF top */
#define RI_DEVCONF	(UB*)DEVCONF	/* DEVCONF top */

/* User definition */
#define RI_USERAREA_TOP	(void*)0x20000000	/* RAM user area top */
#define RI_USERINIT	(FP)NULL	/* User initialization program */
#define RI_RESETINIT	(FP)NULL	/* Reset initialization program */

/* ROM disk */
#define RI_RDSK_TYPE	(UW)0		/* ROM disk type */
#define RI_RDSK_BLOCK	(UW)0		/* ROM disk block size */
#define RI_RDSK_START	(UW)0		/* ROM disk start address */
#define RI_RDSK_END	(UW)0		/* ROM disk end address */


/* ROM information */
RomInfo rominfo = {
	RI_KERNEL_START,	/* Kernel startup address */
	RI_SYSCONF,		/* SYSCONF top */
	RI_DEVCONF,		/* DEVCONF top */
	RI_USERAREA_TOP,	/* RAM user area top */
	RI_USERINIT,		/* User initialization program address */
	RI_RESETINIT,		/* Reset initialization program address */
	{0},			/* Reserved (always 0) */

	RI_RDSK_TYPE,		/* ROM disk type */
	RI_RDSK_BLOCK,		/* ROM disk block size */
	RI_RDSK_START,		/* ROM disk start address */
	RI_RDSK_END,		/* ROM disk end address */
	{0}			/* Reserved (always 0) */
};


/*----------------------------------------------------------------------
#|History of "rominfo_rom.c"
#|==========================
#|* 2016/07/19	It's copied from "../tef_em1d/" and it's modified.
#|* 2016/08/18	Kernelのスタート番地を0x00030000に変更
#|
*/
