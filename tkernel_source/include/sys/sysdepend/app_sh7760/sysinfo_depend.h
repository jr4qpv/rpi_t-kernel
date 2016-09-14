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
 *
 *    Modified by T.Yokobayashi at 2015/11/26.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sysinfo_depend.h (sys/SH7760)
 *
 *	System common information 
 *
 *   0x8c000000 +-----------------------+
 *              |Vector table of        | (EXPEVT / 0x20 * 4) + 0x8c000000
 *              |exception codes and    | (INTEVT / 0x20 * 4) + 0x8c000000
 *              |interrupt factors      |
 *   0x8c000200 +-----------------------+
 *              |Default handler        |
 *   0x8c000204 +-----------------------+
 *              |TLB miss exception     | VBR+0x400 exception handler
 *              |handler                |
 *   0x8c000208 +-----------------------+
 *              |TRAPA vector table     | TRA + 0x8c000000
 *              |(TRAPA 0x70 - 0x7f)    |
 *   0x8c000280 +-----------------------+
 *              |System common info.    |
 *   0x8c000300 +-----------------------+
 *              |EIT stack              |
 *   0x8c000600 +-----------------------+
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
 * System common information 
 */
typedef struct {
	VP	ramtop;		/* RAM free space top */
	VP	ramend;		/* RAM free space end */
	UB	*sysconf;	/* SYSCONF top */
	UB	*devconf;	/* DEVCONF top */
	FP	chkmem;		/* Monitor memory check function */
	UH	Ick;		/* SH7760 CPU clock (MHz) */
	UH	Pck;		/* SH7760 peripheral clock (1/100 MHz) */
	UH	Bck;		/* Bus clock (1/100 MHz) */
	UH	dck;		/* DCK clock (1/100 MHz) */
	UW	loop64us;	/* Loop count per 64 micro sec */
	BootMode bm;	/* Boot mode */
	UB	bootdev[8];	/* Boot device name */
	VW	rsv[21];	/* Reserved (always 0) */
} SysCommonInfo;

/*
 * System common area 
 */
#define N_INTVEC	128
#define N_TRAVEC	30
#define N_INTSTACK	192
typedef struct {
	FP		intvec[N_INTVEC];		/* EIT vector */
	FP		defaulthdr;				/* Default handler */
	FP		tlbmisshdr;				/* TLB miss exception handler */
	FP		travec[N_TRAVEC];		/* TRAPA vector */
	SysCommonInfo	scinfo;			/* System common information */
	UW		intstack[N_INTSTACK];	/* Interrupt stack area */
} SysCommonArea;

#define SCArea	( (SysCommonArea*)0x8c000000 )
#define SCInfo	( SCArea->scinfo )

#endif /* _in_asm_source_ */


/*
 * Vector table addresses 
 */
#define	VECTBL		0x8c000000	/* ベクターテーブル先頭 */
#define	DEFAULTHDR	0x8c000200	/* デフォルトハンドラ */
#define	TLBMISSHDR	0x8c000204	/* TLB ミス例外ハンドラ */

///#define	EITVEC(n)	( ((n) / 0x20) * 4 + VECTBL )


/*
 * 例外ベクタ番号
 */
#define	EIT_RESET		0x00		/* ﾊﾟﾜｰｵﾝﾘｾｯﾄ */
#define	EIT_MRESET		0x01		/* ﾏﾆｭｱﾙﾘｾｯﾄ */
#define	EIT_TLBMISS_R	0x02		/* TLB無効例外(読み出し) */
#define	EIT_TLBMISS_W	0x03		/* TLB無効例外(書き込み) */
#define	EIT_INIPAGE_W	0x04		/* 初期ﾍﾟｰｼﾞ書き込み例外 */
#define	EIT_TLBPROT_R	0x05		/* TLB保護例外(読み出し) */
#define	EIT_TLBPROT_W	0x06		/* TLB保護例外(書き込み) */
#define	EIT_CPUADER_R	0x07		/* CPUｱﾄﾞﾚｽｴﾗｰ(読み出し) */
#define	EIT_CPUADER_W	0x08		/* CPUｱﾄﾞﾚｽｴﾗｰ(書き込み) */
#define	EIT_FPUEXP		0x09		/* FPU例外 */
#define	EIT_TLBHIT		0x0a		/* 命令/ﾃﾞｰﾀTLB多重ﾋｯﾄ例外 */
#define	EIT_TRAPA		0x0b		/* 無条件ﾄﾗｯﾌﾟ(TRAPA 命令) */
#define	EIT_INSTILL		0x0c		/* 一般不当命令例外 */
#define	EIT_SLTILL		0x0d		/* ｽﾛｯﾄ不当命令 */
#define	EIT_NMI			0x0e		/* ﾉﾝﾏｽｶﾌﾞﾙ割込み(NMI) */
#define	EIT_USRBREAK	0x0f		/* ﾕｰｻﾞﾌﾞﾚｰｸﾎﾟｲﾝﾄﾄﾗｯﾌﾟ */

#define	EIT_DEFAULT		0x80		/* ﾃﾞﾌｫﾙﾄﾊﾝﾄﾞﾗ */
#define	EIT_TLBMISS		0x81		/* TLBﾐｽ例外ﾊﾝﾄﾞﾗ */
#define	EIT_MONITOR		0x90		/* T-Monitor ｻｰﾋﾞｽｺｰﾙ */




#ifdef __cplusplus
}
#endif
#endif /* __SYS_SYSINFO_DEPEND_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|【sysinfo_depend.h 変更履歴】
#|□2015/11/26	V1.02.04の"include/sys/sysdepend/std_sh7760/"から、
#|  [app_sh7760]用にコピー
#|□2015/11/26	T-Kernel V1.02.04 Software Package「SH7760 実装仕様書」
#|  imp_sh7760.txt の3.5項を参照してコメントを日本語化
#|
#endif
