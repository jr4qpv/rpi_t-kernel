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
 *	tkdev_conf.h (SH7760)
 *	Target System Configuration 
 */

#ifndef _TKDEV_CONF_
#define _TKDEV_CONF_

/*
 * Timer TMU4
 */
#define VECNO_TMU0	( 0x400 >> 5 )	/* TMU0 interrupt vector number */

#define TSTR		0xffd80004	/* B:Timer start register */
#define TCOR0		0xffd80008	/* W:Timer constant register */
#define TCNT0		0xffd8000c	/* W:Timer counter */
#define TCR0		0xffd80010	/* H:Timer control register */

#define TSTR_STR0	0x01U		/* TMU0 start (TSTR) */
#define TSTR_STR1	0x02U		/* TMU1 start (TSTR) */
#define TSTR_STR2	0x04U		/* TMU2 start (TSTR) */

#define TCR_ICPF	0x0200U		/* Input capture interrupt flag */
#define TCR_UNF		0x0100U		/* Underflow flag */
#define TCR_ICPE	0x00c0U		/* Input capture enable */
#define TCR_UNIE	0x0020U		/* Underflow interrupt enable */
#define TCR_CKEG	0x0018U		/* Clock edge */
#define TCR_TPSC	0x0007U		/* Timer prescaler */
#define TCR_TPSC_P4	0x0000U		/*	Peripheral clock/4 */
#define TCR_TPSC_P16	0x0001		/*	Peripheral clock/16 */
#define TCR_TPSC_P64	0x0002		/*	Peripheral clock/64 */
#define TCR_TPSC_P256	0x0003		/*	Peripheral clock/256 */
#define TCR_TPSC_P1024	0x0004		/*	Peripheral clock/1024 */
#define TCR_TPSC_EXT	0x0007		/*	External clock */

/*
 * Timer interrupt level
 */
#define TIMER_INTLEVEL		8

#endif /* _TKDEV_CONF_ */
