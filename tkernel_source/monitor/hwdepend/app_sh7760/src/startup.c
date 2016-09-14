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
 *    Modified by T.Yokobayashi at 2016/01/26.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)startup.c () 2016/01/26
 *
 *       system boot processing
 */

/*
 *[Note]
 *・リセット後、"reset.S"の「reset_entry」から、本ﾌｧｲﾙ内の｢procReset()｣が
 *  呼び出され、初期化処理を実行する。
 *・procReset()では、resetSystem()関数の呼び出し、μ秒ﾙｰﾌﾟｶｳﾝﾀの初期化、
 *  ｼﾘｱﾙﾎﾟｰﾄの初期化、その他初期化処理を行う。
 */

#include "hwdepend.h"

/* No support for the progress report yet */
#define	DispProgress(n)		/* nop */

/*
 * debug port speed
 *       default setting is LO:38400bps, and HI:115200bps. But
 *       if you need a different set value, then define the following macros in {cpu}/{machine}/setup_xxx.h :
 *       LO_BAUD_RATE, and HI_BAUD_RATE.
 */
#ifndef	LO_BAUD_RATE
#  define LO_BAUD_RATE	 38400
#endif
#ifndef	HI_BAUD_RATE
#  define HI_BAUD_RATE	115200
//#  define HI_BAUD_RATE	38400					////// test
#endif

/*
 * initial processing after reset
 */
EXPORT void procReset( void )
{
	const MEMSEG	*mp;
	W	i;
	W	speed;

	DispProgress(0x01);

	/* system basic set up */
	resetSystem(0);
	DispProgress(0x06);

	/* setting up the initial count for micro-wait */
	setupWaitUsec();
	DispProgress(0x07);

	/* initialize console serial port */
#if SW_BHI == 0
	speed = HI_BAUD_RATE; /* HI speed is fixed. */
#else
	speed = ( (DipSw & SW_BHI) != 0 )? HI_BAUD_RATE: LO_BAUD_RATE;
#endif
	initSIO(getConPort(), speed);		/* シリアル通信ポートの初期化 */
	DispProgress(0x08);

#if 0
	/* オープニングメッセージ表示 */
	DSP_S("\n\n");
	DSP_S(Title[0]);
	DSP_S(", DipSw="); DSP_02X(getDipSw()); DSP_LF;
#endif

	/* initialize hardware (peripherals) */
	initHardware();
	DispProgress(0x0d);

	/* memory clear is not done to save time when automatic reboot is under way. */
	if ( bootSelect() == BS_MONITOR ) {
		cpuLED(LED_MEMCLR);

		/* all memory clear (save the monitor area) */
		for ( i = 1;; ++i ) {
			mp = MemArea(MSA_OS|MSA_ERAM, i);
			if ( mp == NULL ) break;

			/* MSA_OS 又は MSA_ERAM 属性の領域を零クリア */
			waitMsec(0);				/* WDTトリガ(dog_food) */
			memset((void*)mp->top, 0, mp->end - mp->top);
		}
	}
	cpuLED(LED_POWERUP);
	DispProgress(0x0e);

	/* initialize break processing */
///	initBreak();	///// see cmdsrv/armv6/break.c /////


	/* Invoking user reset initialization routine */
///	callUserResetInit();	///// see cmdsrv/armv6/misc.c /////
	DispProgress(0x0f);

	waitMsec(0);						/* WDTトリガ(dog_food) */

#if 0	///////////////////// for debug ///
	DSP_S("\n>>>>>"); waitMsec(500);
	while (1) { cpuLED(0x02); waitMsec(100); cpuLED(0x00); waitMsec(100); }
#endif	/////////////////////
}


/*----------------------------------------------------------------------*/
#if 0
#|【startup.c 変更履歴】
#|□2015/12/02	[app_sh7760]用に、[tef_em1d]の"startup.c"から作成。
#|  ・基本的には、できるだけそのままで使う。
#|
#endif
