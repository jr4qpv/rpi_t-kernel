/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2011/09/08.
 *    Modified by T-Engine Forum at 2013/03/04.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/03/07.
 *
 *----------------------------------------------------------------------
 */

/*
 *	system.c
 *
 *       @(#)system-related processing () 2016/04/16
 *
 *       target: BCM283x
 */

#include "sysdepend.h"
#include <misc/rpi/bcm283x.h>

EXPORT	UW	DipSw;		/* DipSw status */

/* hardware dependent functions */
IMPORT	UW	DipSwStatus(void);
IMPORT	void	usbPower(BOOL power);
IMPORT	void	powerOff(void);
IMPORT	void	resetStart(void);

/* interrupt entry point (eitent.S) */
IMPORT	void	_gio0Hdr(void);
IMPORT	void	_gio1Hdr(void);
IMPORT	void	_gio2Hdr(void);
IMPORT	void	_gio3Hdr(void);
IMPORT	void	_gio4Hdr(void);
IMPORT	void	_gio5Hdr(void);
IMPORT	void	_gio6Hdr(void);
IMPORT	void	_gio7Hdr(void);

/* default handler (cmdsvc) */
IMPORT	void	_defaultHdr(void);

/* macros for manipulating cache/MMU/PMIC */
#define	EnbCacheMMU(x)	setCacheMMU(ENB_CACHEMMU)
#define	DisCacheMMU(x)	setCacheMMU(ENB_MMUONLY) /* MMU can't be turned off */

/* ------------------------------------------------------------------------ */

IMPORT	char	__loadaddr;	/* monitor load address */
IMPORT	MEMSEG	NoMemSeg[];	/* memory unused area */
IMPORT	W	N_NoMemSeg;



/*
        power management controller (DA9052) handler routines
*/
#define	SPIPol	0x0092

/* initialize SPI for PMIC communication */
LOCAL	void	pmicInit(void)
{
#if 0	///////////////
	out_w(SPn_MODE(SP0), 0x2700);		// 8bit, CS0, Master, CPU mode
	out_w(SPn_TIECS(SP0), 0x000f);		// CS0: follow the specification by SPn_POL
	out_w(SPn_POL(SP0), SPIPol);
	out_w(SPn_ENCLR(SP0), ~0);		// interrupt disable

	out_w(SPn_CONTROL(SP0), 0x0100);	// start reset
	waitUsec(10);
	out_w(SPn_CONTROL(SP0), 0x0000);	// release reset
	out_w(SPn_CONTROL2(SP0), 0x0000);
#endif	//////////////

	return;
}


/* read PMIC register */
EXPORT	W	pmicRead(W reg)
{
#if 1
	return 0;
#else	//////////////////
	W	dat;

	pmicCSassert(TRUE);			// CS assert

	out_w(SPn_FFCLR(SP0), ~0);		// status flag is cleared
	out_w(SPn_TX_DATA(SP0), (reg << 1) | 1);        // send register number
	out_w(SPn_CONTROL(SP0), 0x0009);	// send start
	pmicWait();

	out_w(SPn_FFCLR(SP0), ~0);		// status flag is cleared
	out_w(SPn_CONTROL(SP0), 0x0005);	// start receive
	pmicWait();
	dat = in_w(SPn_RX_DATA(SP0));		// data received

	pmicCSassert(FALSE);			// CS de-assert

	return dat;
#endif	////////////////
}

/* write PMIC register */
EXPORT	void	pmicWrite(W reg, W dat)
{
#if 0	///////////////////
	pmicCSassert(TRUE);			// CS assert

	out_w(SPn_FFCLR(SP0), ~0);		// status flag is cleared
	out_w(SPn_TX_DATA(SP0), reg << 1);	// send register number
	out_w(SPn_CONTROL(SP0), 0x0009);	// send start
	pmicWait();

	out_w(SPn_FFCLR(SP0), ~0);		// status flag is cleared
	out_w(SPn_TX_DATA(SP0), dat);		// send data
	out_w(SPn_CONTROL(SP0), 0x0009);	// send start
	pmicWait();

	pmicCSassert(FALSE);			// CS de-assert
#endif

	return;
}

/* ------------------------------------------------------------------------ */

/* basic system set up (performed during reset, and Disk Boot) */
EXPORT	void	resetSystem(W boot)
{
	MEMSEG	*mp;
	UW	i, va;

	/* obtain DipSw status */
	if (!boot) DipSw = DipSwStatus();

#if 0
	setCacheMMU(DIS_CACHEMMU);			/* MMU turned off */
#else
	DisCacheMMU();
#endif

#if 0	//////////////////////////
	/* set up interrupt controller */
	out_w(IT0_IDS0, ~0);		// CPU: all interrupts disabled
	out_w(IT0_IDS1, ~0);
	out_w(IT0_IDS2, ~0);
	out_w(IT0_IIR, ~0);
	out_w(IT3_IPI0_CLR, 0x0000003f);
	out_w(IT3_IDS0, ~0);		// DSP: all interrupts disabled
	out_w(IT3_IDS1, ~0);
	out_w(IT3_IDS2, ~0);
	out_w(IT3_IIR, ~0);
	out_w(IT0_IPI3_CLR, 0x0000003f);
	out_w(IT0_FID, 0x00000001);	// CPU: FIQ disabled
	out_w(GIO_IIA(GIO_L), 0);	// GPIO: interrupt disabled
	out_w(GIO_IIA(GIO_H), 0);
	out_w(GIO_IIA(GIO_HH), 0);
	out_w(GIO_IIA(GIO_HHH), 0);
	out_w(GIO_GSW(GIO_L), 0);	// GPIO: FIQ interrupt disabled
	out_w(GIO_GSW(GIO_H), 0);
	out_w(GIO_GSW(GIO_HH), 0);
	out_w(GIO_GSW(GIO_HHH), 0);
	out_w(IT0_LIIR, 0x0000000f);	// internal interrupt disabled
	out_w(IT_PINV_CLR0, ~0);	// inhibit interrupt polarity inversion
	out_w(IT_PINV_CLR1, ~0);
	out_w(IT_PINV_CLR2, ~0);
	out_w(IT0_IEN0, 0x0c000000);	// CPU: GPIO interrupt enabled
	out_w(IT0_IEN1, 0x003c0000);
	out_w(IT0_IEN2, 0x00018000);
#endif	///////////////////////////////

	/* power on controller initialization */
	pmicInit();

	/* USB power on */
	usbPower(TRUE);

	/* clear system common area (vector table, and SysInfo) */
	memset(&SCInfo, 0, sizeof(SysCommonInfo));
	memset(SCArea, 0, sizeof(SysCommonArea));

	/* if monitor is loaded into RAM, exclude the RAM area */
	mp = MemArea(MSA_OS, 1);
	va = (UW)&__loadaddr;
	if (va >= mp->top && va < mp->end) mp->end = va;

	/* exclude the area where ROM disk data is stored */
	va = (UW)ROMInfo->userarea;
	if (va >= mp->top && va < mp->end) mp->end = va;

	/* initialize system common information (SysInfo) */
	SCInfo.ramtop = (void*)mp->top;
	if (va < mp->top || va > mp->end) va = mp->end;
	SCInfo.ramend = (void*)va;

	/* set up EIT vectors */
	// we do not need _defaultHdr absolutely, but just in case set it up
	SCArea->intvec[EIT_DEFAULT]	= _defaultHdr;	// default handler
	SCArea->intvec[EIT_UNDEF]	= _defaultHdr;	// undefined instruction
	SCArea->intvec[SWI_MONITOR]	= _defaultHdr;	// SWI - monitor SVC
///	SCArea->intvec[EIT_IRQ(26)]	= _gio6Hdr;	// GPIO branch
///	SCArea->intvec[EIT_IRQ(27)]	= _gio7Hdr;
///	SCArea->intvec[EIT_IRQ(50)]	= _gio0Hdr;
///	SCArea->intvec[EIT_IRQ(51)]	= _gio1Hdr;
///	SCArea->intvec[EIT_IRQ(52)]	= _gio2Hdr;
///	SCArea->intvec[EIT_IRQ(53)]	= _gio3Hdr;
///	SCArea->intvec[EIT_IRQ(79)]	= _gio4Hdr;
///	SCArea->intvec[EIT_IRQ(80)]	= _gio5Hdr;
///	SCArea->intvec[EIT_GPIO(8)]	= _defaultHdr;	// abort switch

	/* set up initial page table */
	for (i = 0; i < N_MemSeg; ++i) {
		mp = &MemSeg[i];
		if (!mp->pa) continue;

		/* FlashROM has already been mapped, and so do not touch it */
		if (mp->attr == MSA_FROM) continue;

		/* set up in unit of section (1MB) */
		for ( va  = (mp->top & 0xfff00000);
		      va != ((mp->end + 0x000fffff) & 0xfff00000);
		      va += 0x00100000 ) {
			TopPageTable[va / 0x00100000] =
				((mp->pa & 0xfff00000) + va) |
				 (mp->pa & 0x000fffff);
		}
	}

	for (i = 0; i < N_NoMemSeg; ++i) {
		mp = &NoMemSeg[i];

		/* set up in unit of section (1MB) */
		for ( va  = (mp->top & 0xfff00000);
		      va != ((mp->end + 0x000fffff) & 0xfff00000);
		      va += 0x00100000 ) {
			TopPageTable[va / 0x00100000] = 0;
		}
	}

	DSB();
	Asm("mcr p15, 0, %0, cr8, c7, 0":: "r"(0));	// I/D TLB invalidate(TLBIALL)
	Asm("mcr p15, 0, %0, cr7, c5, 6":: "r"(0));	// invalidate BTC(BPIALL)
	DSB();
	ISB();

	EnbCacheMMU();

	return;
}

/* ------------------------------------------------------------------------ */

/*
        system termination: reset / system power off
        reset   0       power off
                -1      reboot
                0xaa55  halt boot and power off
*/
EXPORT	void	sysExit(W reset)
{
	DisCacheMMU();

	// after this point, delay such as waitUsec() spends more time than the number indicates //

	/* LED off */
	cpuLED(0x00);

	/* all interrupts disabled */
#if 0	///////////////////
	out_w(IT0_IDS0, ~0);		// CPU: all interrupts disabled
	out_w(IT0_IDS1, ~0);
	out_w(IT0_IDS2, ~0);
	out_w(IT3_IPI0_CLR, 0x0000003f);
	out_w(IT3_IDS0, ~0);		// DSP: all interrupts disabled
	out_w(IT3_IDS1, ~0);
	out_w(IT3_IDS2, ~0);
	out_w(IT0_IPI3_CLR, 0x0000003f);
	out_w(IT0_FID, 0x00000001);	// FIQ disabled
	out_w(IT0_LIIR, 0x0000000f);	// internal interrupt disabled
#endif	//////////////////////

	/* power on controller initialization */
	pmicInit();

	/* USB power off */
	usbPower(FALSE);

	if (reset >= 0) powerOff();

	/* reset start */
	resetStart();
}


/* ------------------------------------------------------------------------
#|History of "system.c"
#|=====================
#|* 2016/03/04	It's copied from "../tef_em1d/" and it's modified.
#|
*/
