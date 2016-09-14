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
 *    Modified by T.Yokobayashi at 2016/02/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)rominfo_ram.c (SH7760)
 *
 *	ROM information for RAM kernel
 */

#include <basic.h>
#include <sys/rominfo.h>

IMPORT UB SYSCONF[], DEVCONF[];
IMPORT void START();

/* Kernel address */
#define RI_KERNEL_START	(FP)&START	/* Kernel start address */
#define RI_SYSCONF	(UB*)SYSCONF	/* SYSCONF top */
#define RI_DEVCONF	(UB*)DEVCONF	/* DEVCONF top */

/* User definition */
#define RI_USERAREA_TOP	(void*)0x8d000000	/* RAM user area top */
#define RI_USERINIT	(FP)NULL	/* User initialization program */
#define RI_RESETINIT	(FP)NULL	/* Reset initialization program */

/* ROM disk */
#if 0
#define RI_RDSK_TYPE	(UW)1		/* ROM disk type */
#define RI_RDSK_BLOCK	(UW)512		/* ROM disk block size */
#define RI_RDSK_START	(UW)0x80080000	/* ROM disk start address */
#define RI_RDSK_END		(UW)0x80200000	/* ROM disk end address */
#else
#define RI_RDSK_TYPE	(UW)0		/* ROM disk type */
#define RI_RDSK_BLOCK	(UW)0		/* ROM disk block size */
#define RI_RDSK_START	(UW)0		/* ROM disk start address */
#define RI_RDSK_END		(UW)0		/* ROM disk end address */
#endif


/* ROM information */
RomInfo rominfo = {
	RI_KERNEL_START,	/* Kernel startup address */
	RI_SYSCONF,			/* SYSCONF top */
	RI_DEVCONF,			/* DEVCONF top */
	RI_USERAREA_TOP,	/* RAM user area top */
	RI_USERINIT,		/* User initialization program address */
	RI_RESETINIT,		/* Reset initialization program address */
	(void *)0,			/* IO configuration address(ioconf) */
	{0},				/* Reserved (always 0) */

	RI_RDSK_TYPE,		/* ROM disk type */
	RI_RDSK_BLOCK,		/* ROM disk block size */
	RI_RDSK_START,		/* ROM disk start address */
	RI_RDSK_END,		/* ROM disk end address */
	{0}					/* Reserved (always 0) */
};


#if 0
#|【rominfo_ram.c 変更履歴】
#|□2015/12/17	[app_sh7760]用に、[tef_em1d]の"rominfo_ram.c"を参考に作成。
#|□2016/02/03	ioconfの初期化定義を追加、[tef_em1d]では未定義。
#|  （"/sys/sysdepend/app_sh7760/rominfo_depend.h"を参照，今のところ未使用）
#|
#endif
