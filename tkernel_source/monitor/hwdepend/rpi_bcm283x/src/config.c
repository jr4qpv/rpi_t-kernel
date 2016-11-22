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
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/03/10.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)config.c (BCM283x) 2016/09/07
 *
 *       system-related processing / system configuration information
 *
 *       target: BCM283x
 */

#include "sysdepend.h"
#include <rpi/bcm283x.h>

/* used device driver */
IMPORT	ER	initSIO_pl011(SIOCB *, const CFGSIO *, W speed);
IMPORT	ER	initSIO_miniuart(SIOCB *, const CFGSIO *, W speed);
IMPORT	ER	initMemDisk(DISKCB *, const CFGDISK *);

/* memory region definition */
EXPORT	MEMSEG	MemSeg[] = {
#ifdef CPU_BCM2835						/* BCM2835 ? */
	// FROM, 1MB (※0番地は含まないようにする事※)
	{0x00000100, 0x00100000, MSA_FROM,	PGA_RO|PGA_C |0x00000000},
	// SDRAM, 511Mbyte
	{0x00100000, 0x20000000, MSA_RAM,	PGA_RW|PGA_C},
	// I/O
	{0x20000000, 0x21000000, MSA_IO,	PGA_RW|PGA_D |PGA_S|PGA_XN},
#else									/* BCM2836  */
	// FROM, 1MB (※0番地は含まないようにする事※)
	{0x00000100, 0x00100000, MSA_FROM,	PGA_RO|PGA_C |0x00000000},
	// SDRAM, 1007Mbyte
	{0x00100000, 0x3f000000, MSA_RAM,	PGA_RW|PGA_C},
	// I/O
  #if 1
	{0x3e000000, 0x40000000, MSA_IO,	PGA_RW|PGA_D |PGA_S|PGA_XN},
	{0x40000000, 0x40100000, MSA_IO,	PGA_RW|PGA_D |PGA_S|PGA_XN},
  #else		/*///debug ストロング・オーダにしてみる ///*/
	{0x3e000000, 0x40000000, MSA_IO,	PGA_RW|PGA_S|PGA_XN},
	{0x40000000, 0x40100000, MSA_IO,	PGA_RW|PGA_S|PGA_XN},
  #endif
#endif

	{0x00008000, 0x00030000, MSA_MON,	0},
	{0x00100000, 0x02000000, MSA_RDA,	0},
	{0x10006000, 0x20000000, MSA_OS,	0},
};

EXPORT	W	N_MemSeg = sizeof(MemSeg) / sizeof(MEMSEG);

/* unused memory region definition */
EXPORT	MEMSEG	NoMemSeg[] = {
#ifdef CPU_BCM2835						/* BCM2835 ? */
	{0x21000000, 0xf0000000, 0,		0},
#else									/* BCM2836 */
	{0x40000000, 0xf0000000, 0,		0},
#endif
};

EXPORT	W	N_NoMemSeg = sizeof(NoMemSeg) / sizeof(MEMSEG);

/*
 * serial port configuration definition
 *       list in the order of port number
 */
EXPORT	const	CFGSIO	ConfigSIO[] = {
	{initSIO_pl011, 0},
	{initSIO_miniuart, 0},
};

EXPORT	const W	N_ConfigSIO = sizeof(ConfigSIO) / sizeof(CFGSIO);


/*
 * disk drive configuration definition
 *	list in the order of port number
 */
EXPORT	const CFGDISK	ConfigDisk[] = {
	{"rda",	DA_RONLY,	initMemDisk,	0},	// FlashROM
};

EXPORT	const W	N_ConfigDisk = sizeof(ConfigDisk) / sizeof(CFGDISK);

/* boot information */
EXPORT	const UH	BootSignature = 0xe382;		// signature
EXPORT	UB *	const PBootAddr = (UB *)0x30200000;	// primary boot loader address
 

/* ------------------------------------------------------------------------ */

IMPORT	W	pmicRead(W reg);
IMPORT	W	pmicWrite(W reg, W dat);
#define	pmicDelay(x)	waitUsec(4)	// about 16msec
#define	USBPowerOn	0xe0		// GPIO13(OD), High * power is supplied to A connector only
#define	USBPowerOff	0xe0		// GPIO13(OD), High

/* obtain DipSw status */
EXPORT	UW	DipSwStatus(void)
{
	UW	d;

#if 1
  #if FORCED_MON
	d = SW_MON;							// t-monitorを強制起動する
  #else
	d = 0;								// t-kernelを自動起動する
  #endif
#else
        /* read data from read port */
	d = IICGPIORead(0xd9);

        /* unnecessary bits are masked and then invert logic. */
	d = (d ^ SW_MON) & SW_MON;

        /* check abort switch */
	if (in_w(GIO_I(GIO_L)) & 0x00000100) d |= SW_ABT;
#endif

	return d;
}

/* USB power control */
EXPORT	void	usbPower(BOOL power)
{
#if 0	///////////////
	pmicWrite(27, (pmicRead(27) & 0x0f) |
		  		(power ? USBPowerOn : USBPowerOff));
	pmicDelay();
#endif	///////////////
}

/* power off */
EXPORT	void	powerOff(void)
{
#if 0	////////////
	W	i;

	for (i = 10; i < 14; i++) pmicWrite(i, 0xff);	// IRQ_MASK_A-D (mask)
	pmicDelay();

	for (i = 5 ; i < 9; i++) pmicWrite(i, 0xff);	// EVENT_A-D (clear)
	pmicDelay();

	while (1) {
		pmicWrite(15, 0x60);	// DEEP_SLEEP
		pmicDelay();
	}
#endif	///////////////////////
}

/* reset start*/
EXPORT	void	resetStart(void)
{
	while (1) {
#if 0	////////////////////////
                /* reset */
		pmicWrite(15, 0xac);		// SHUTDOWN
		pmicDelay();
#endif	///////////////////////
	}
}

/* initialize hardware peripherals (executed only during reset) */
EXPORT	void	initHardware(void)
{
#if 0	//////////////
        /* enable abort switch interrupt */
	out_w(GIO_IDT1(GIO_L), 0x00000008);	// asynchronous leading-edge high interrupt
	out_w(GIO_IIR(GIO_L), 0x00000100);
	out_w(GIO_IIA(GIO_L), 0x00000100);
	out_w(GIO_IEN(GIO_L), 0x00000100);
#endif	//////////////

	return;
}

/* LED on/off */
/* bit1=err[赤], bit0=run[緑] 割当推奨 */
EXPORT	void	cpuLED(UW v)
{
	UB	m, d, r, c;
	UW tmp;

	m = ~((v >> 16) | 0xf0);			// mask (0:unmodified 1:modify)
	d = ((v >>  0) & 0x0f);				// set value (0:off 1:on) :正論理
//	d = ~((v >>  0) | 0xf0);			// set value (0:on 1:off) :負論理

#if TYPE_RPI < 3						/*** RPI1 or RPI2 ***/
	/* PWR Red LED (GPIO35) 状態入力 */
	tmp = in_w(GPLEV1);
	r = (tmp >> 3) & 1	;				// get GPIO35

	/* ACT Green LED (GPIO47) 状態入力 */
	r <<= 1;
	if ((tmp >> 15) & 1)				// check GPIO47
		r |= 1;
#else									/*** RPI3 ***/
	/* GPIO17 状態入力 */
	tmp = in_w(GPLEV0);					// get GPIO17
	r = (tmp >> 17) & 1;
#endif

	c = (r ^ d) & m;					// modify flag (0:unmodified 1:modify)

#if TYPE_RPI < 3						/*** RPI1 or RPI2 ***/
	/* GPIO47, ACT green LED 出力 */
	if (c & 0x01) {						// bit0変化?
		if (r & 0x01)
			out_w(GPCLR1, 1<<15);		// clear GPIO47
		else
			out_w(GPSET1, 1<<15);		// set GPIO47
	}

	/* GPIO35, PWR red LED 出力 */
	if (c & 0x02) {						// bit1変化?
		if (r & 0x02)
			out_w(GPCLR1, 1<<3);		// clear GPIO35
		else
			out_w(GPSET1, 1<<3);		// set GPIO35
	}
#else									/*** RPI3 ***/
	/* GPIO17 LED 出力 */
	if (c & 0x01) {						// bit0変化?
		if (r & 0x01)
			out_w(GPCLR0, 1<<17);		// clear GPIO17
		else
			out_w(GPSET0, 1<<17);		// set GPIO17
	}
#endif
}

/*
 * machine-dependent interrupt processing
 *       vec     interrupt vector number
 *       return value    0: unsupported target
 *               1: for the supported target, processing was performed. (monitor still continues)
 *               2: for the supported target, proceesing was performed (interrupt handler is exited)
 */
EXPORT	W	procHwInt(UW vec)
{
	/* IRQ interrupt? */
	if ((vec >= EIT_IRQ(0)) && (vec <= EIT_IRQ(MAXIRQNUM))) {
		return 0;
	}

#if 0	////////////////////
        /* only abort switch (GPIO(P8)) is supported */
	if (vec != EIT_GPIO(8)) return 0;

        /* clear interrupt */
	out_w(GIO_IIR(GIO_L), 0x00000100);

	DSP_S("Abort Switch (SW1) Pressed");
#endif	////////////////////
	return 1;
}


/*----------------------------------------------------------------------
#|History of "config.c"
#|======================
#|* 2016/03/10	It's copied from "../tef_em1d/" and it's modified.
#|* 2016/08/27	RPI3の時は、cpuLED()はGPIO17に出力する。
#| (Raspberry Pi 3では、ACT,PWRのLEDはGPIOで操作できない為）
#|
*/
