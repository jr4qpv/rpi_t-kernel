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
 *	@(#)setup_rzt1.h () 2016/04/08
 *
 *       Renesas RZ/T1 (RZT1) configuration information
 *
 *       this file included from assembly source files, too.
 */

#define	EITENT_BASE		0x00000000	// address for exception branch processing
//#define	PAGETBL_BASE	0x30000000	// address of the first level table page.

/*
 * clock value
 */
#define	CPUCLK_CLK	600000		// 600.000MHz


#define	PCLKD_CLK	75000		// 75.000MHz






#define	PLL1_CLK	900000		// 900.000MHz
#define	PLL2_CLK	401418		// 401.408MHz	//// ??? //////
#define	PLL3_CLK	229376		// 229.376MHz	//// ??? //////

#define	ACPU_CLK	(PLL1_CLK / 1000)		/* [MHz]:ARM CPU clock(900MHz) */
#define	Txx_CLK		(PLL3_CLK / 8)			///// ? ///////

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





#ifdef _in_asm_source_

/*******( LED for Rpi2:GPIO47=LED1(ACT,Green),GPIO35=LED2(PWR,Red) )*******/

 .macro led_green_on					// GPIO47(LED1,ACT) on
	ldr		r2,=0x3f200020				// GPSET1
	mov		r0,#0x00008000				// set GPIO47
	str		r0,[r2]
 .endm

 .macro led_green_off					// GPIO47(LED1,ACT) off
	ldr		r2,=0x3f20002c				// GPCLR1
	mov		r0,#0x00008000				// clear GPIO47
	str		r0,[r2]
 .endm

 .macro led_green_inv					// GPIO47(LED1,ACT)
	ldr		r2,=0x3f200038				// GPLVL1
	ldr		r0,[r2]
	tst		r0,#0x00008000				// check GPIO47
	bne		1010f
	led_green_on
	b		1019f
1010:
	led_green_off
1019:
 .endm


 .macro led_red_setup					// GPIO35(LED2,PWR) output setup
	ldr		r2,=0x3f20000c				// GPFSEL3
	ldr		r0,[r2]
	bic		r0,r0,#0x00038000			// 下位3bitのみビットクリア
	orr		r0,r0,#0x00008000			// GPIO35 output
	str		r0,[r2]
 .endm

 .macro led_red_on						// GPIO35(LED2,PWR) on
	ldr		r2,=0x3f200020				// GPSET1
	mov		r0,#0x00000008				// set GPIO35
	str		r0,[r2]
 .endm

 .macro led_red_off						// GPIO35(LED2,PWR) off
	ldr		r2,=0x3f20002c				// GPCLR1
	mov		r0,#0x00000008				// set GPIO35
	str		r0,[r2]
 .endm

 .macro led_red_inv						// GPIO35(LED2,PWR)
	ldr		r2,=0x3f200038				// GPLVL1
	ldr		r0,[r2]
	tst		r0,#0x00000008				// check GPIO35
	bne		1010f
	led_red_on
	b		1019f
1010:
	led_red_off
1019:
 .endm


 .macro led_all_off
	led_green_off
	led_red_off
 .endm


 .macro gpio20_setup					// GPIO20 を出力ポートに設定
	ldr		r2,=0x3f200008				// GPFSEL2
	ldr		r0,[r2]
	bic		r0,r0,#7					// 下位3bitのみビットクリア
	orr		r0,r0,#1					// GPIO20 output
	str		r0,[r2]
 .endm

 .macro gpio20_on						// GPIO20 を'H'出力する
	ldr		r2,=0x3f20001c				// GPSET0
	mov		r0,#0x00100000				// set GPIO20
	str		r0,[r2]
 .endm

 .macro gpio20_off						// GPIO20 を'L'出力する
	ldr		r2,=0x3f200028				// GPCLR0
	mov		r0,#0x00100000				// clear GPIO20
	str		r0,[r2]
 .endm

 .macro gpio20_inv						// GPIO20 を反転する
	ldr		r2,=0x3f200034				// GPLVL0
	ldr		r0,[r2]
	tst		r0,#0x00100000				// check GPIO20
	bne		1010f
	gpio20_on							// set GPIO20
	b		1019f
1010:
	gpio20_off							// clear GPIO20
1019:
 .endm



/*================================================
 * syst_wait1  《約1m秒の待ち マクロ》
 *  ［入力］count…待ち回数
 *  ［出力］なし
 *  ［Note］・System Timer(1MHz)を利用して待ちﾀｲﾐﾝｸﾞを計る。
 *  ［破壊］r0,r1,r2,r3
 =================================================*/
 .macro syst_wait1 count
	ldr		r2,=\count					// r2←count
	cmp		r2,#0						// r2=0 ?
	beq		1009f						// r2=0の時は終了へ

//	( 1m秒待ち )
1001:									// (r2…待ち回数)
	ldr		r3,=0x3f003004				// SYST_CLO(ﾌﾘｰﾗﾝｶｳﾝﾀの下位)
	ldr		r1,[r3]						// r1←現在のSYST_CLO ｶｳﾝﾀ値を保持

1002:
	ldr		r3,=0x3f003004				// SYST_CLO(ﾌﾘｰﾗﾝｶｳﾝﾀの下位32bit)
	ldr		r0,[r3]						// r0←現在のｶｳﾝﾀ値を獲得
	sub		r0,r0,r1					// r0←r0-r1
	ldr		r3,=1000
	cmp		r0,r3						// 1msec
	blo		1002b						// r0<1000の時分岐

	subs	r2,r2,#1					// r2-1
	beq		1009f						// r2=0?,時終了へ
	b		1001b						// to loop

	.pool

  1009:									// end
 .endm


 .macro debug_led_loop
///  1099:
///    led_gbr_set
///	led_green_inv
	syst_wait1 250				// 0.25秒待ち
///	bra		1099b				// to loop
///	nop
 .endm

#endif	/* _in_asm_source_ */








/*----------------------------------------------------------------------*/
#if 0
#|History of "setup_rzt1.h"
#|============================
#|* 2016/03/14	It's copied from "../tef_em1d/setup_em1d512.h" and it's modified.
#|
#endif
