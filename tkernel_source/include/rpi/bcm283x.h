/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel 2.0 Software Package
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)bcm283x.h () 2016/11/22
 *
 *       Raspberry Pi (BCM283x) register definitions (excerpt)
 *
 *       * this is included from assembler program source files
 */

#ifndef	__RPI_BCM283x_H__
#define	__RPI_BCM283x_H__

#include <tk/sysdef.h>


#if (TYPE_RPI <= 1)						/*** BCM2835 ? ***/
#include <rpi/mach-bcm2708/platform.h>
#include <rpi/mach-bcm2708/arm_control.h>
#else									/*** BCM2836 or BCM2837 ***/
#include <rpi/mach-bcm2709/platform.h>
#include <rpi/mach-bcm2709/arm_control.h>
#endif


// ARM Interrupt controller [p110]
// UART [p175]
// Timer(ARM side) [p196]



/* General Purpose I/O GPIO [p89] */
#define	GPIOBase		(GPIO_BASE)
#define	GPFSEL0			(GPIOBase + 0x0000)		/* 09-00 */
#define	GPFSEL1			(GPIOBase + 0x0004)		/* 19-10 */
#define	GPFSEL2			(GPIOBase + 0x0008)		/* 29-20 */
#define	GPFSEL3			(GPIOBase + 0x000c)		/* 39-30 */
#define	GPFSEL4			(GPIOBase + 0x0010)		/* 49-40 */
#define	GPFSEL5			(GPIOBase + 0x0014)		/* 53-50 */

#define	GPSET0			(GPIOBase + 0x001c)		/* 31-00 */
#define	GPSET1			(GPIOBase + 0x0020)		/* 53-32 */

#define	GPCLR0			(GPIOBase + 0x0028)		/* 31-00 */
#define	GPCLR1			(GPIOBase + 0x002c)		/* 53-32 */

#define	GPLEV0			(GPIOBase + 0x0034)		/* 31-00 */
#define	GPLEV1			(GPIOBase + 0x0038)		/* 53-32 */


/* System Timer [p172] */
#define	SYSTBase		(ST_BASE)
#define	SYST_CS			(SYSTBase + 0x0000)
#define	SYST_CLO		(SYSTBase + 0x0004)
#define	SYST_CLH		(SYSTBase + 0x0008)
#define	SYST_C0			(SYSTBase + 0x000c)
#define	SYST_C1			(SYSTBase + 0x0010)
#define	SYST_C2			(SYSTBase + 0x0014)
#define	SYST_C3			(SYSTBase + 0x0018)

#define	SYST_CLOCK		1000000					/* 1MHz */



#endif /*__RPI_BCM283x_H__*/


/*----------------------------------------------------------------------
#|History of "bcm283x.h"
#|======================
#|* 2016/07/19	新規作成（By T.Yokobayashi）
#|* 2016/08/20	機種判定は、TYPE_RPIを参照するように変更
#|* 2016/11/22	includeフォルダを"misc/rpi"→"rpi"に変更
#|
*/
