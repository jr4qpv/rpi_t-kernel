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
 *    Modified by T.Yokobayashi at 2016/03/01.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)setup_bcm283x.h (BCM283x) 2016/09/08
 *
 *       Raspberry Pi (BCM283x) configuration information
 *
 *       this file included from assembly source files, too.
 */

#define	FORCED_MON		0			// モニタ強制起動(=1:する, =0:Auto)


#define	EITENT_BASE		0x00008000	// address for exception branch processing
#define	PAGETBL_BASE	0x10000000	// address of the first level table page.

/*
 * clock value
 */
#define	PLL1_CLK	900000			// 900.000MHz
#define	ACPU_CLK	(PLL1_CLK / 1000)		/* [MHz]:ARM CPU clock(900MHz) */

/*
 * assignment to DipSw (switches)
 */
#define	SW_ABT		0x0100		// Abort SW
#define	SW_MON		0x0020		// Monitor Boot
#define	SW_BHI		0x0000		// fix HI_BAUD_RATE

/*
 * LED display (two bits, 2 bits)
 */
#define	LED_POWERUP	0x01		// Power-on
#define	LED_MEMCLR	0xff		// Memory clear

/*
 * ConPort Number (see misc.c)
 */
#if TYPE_RPI < 3					/*** RPI1 or RPI2 ***/
  #define	ConPort_Number	0		/* UART0(PL011) */
#else								/*** RPI3 ***/
  #define	ConPort_Number	1		/* UART1(mini UART) */
#endif


/*
 * Reset startup use stack area / Monitor debug work area
 */
#define	StartupEntry_stk	0x10000000	/* Startup時のみ利用するｽﾀｯｸ */

#define	__MonDebugSize		0x1000
#define	__MonDebugWK		(StartupEntry_stk - __MonDebugSize)

#define	__INFO_ILLIRQ		0x0000
#define	__INFO_CTR			0x0004
#define	__INFO_CCSIDR0		0x0008
#define	__INFO_CCSIDR1		0x000c



#ifdef _in_asm_source_
/* アセンブラ専用の定義はここに記述 */


#endif	/* _in_asm_source_ */


/*----------------------------------------------------------------------
#|History of "setup_bcm283x.h"
#|============================
#|* 2016/03/01	It's copied from "../tef_em1d/setup_em1d512.h" and it's modified.
#|* 2016/08/30	ConPort_Number定義を追加、RPI3の時はｺﾝｿｰﾙﾎﾟｰﾄは｢1｣とする。
#| (Raspberry Pi 3では、UART0はBluetoothで使用の為,UART1に変更されてる)
#|
*/
