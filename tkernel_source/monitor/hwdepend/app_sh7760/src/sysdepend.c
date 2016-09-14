/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2015-2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2015/12/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sysdepend.c () 2015/12/02
 *
 *       system-depended routine
 *
 *       target: App Board/SH7760(SH-4)
 */

#include "sysdepend.h"
#include <misc/sh7760.h>

////#include <tmonitor.h>		////////////
///#include <tm/tmonitor.h>	///////////
////#include <tk/sysdef.h>	///////////////////






#if 0	////// C言語からは cpuLED()で代用できるので削除 //////
/*
 *   LED access routine
 *  (IOP5:#5=GREEN,PJ#4=RED)
 */

/* 赤LED点灯 */
EXPORT	void	LED_red_on(void)
{
	PFC.PJDR.BIT.PJ4DT = 1;				// 赤LED点灯
	PFC.PJDR.BIT.PJ5DT = 0;				// 緑LED消灯
	return;
}

/* 赤LED消灯 */
EXPORT	void	LED_red_off(void)
{
	PFC.PJDR.BIT.PJ4DT = 0;				// 赤LED消灯
	return;
}

/* 赤LED反転 */
EXPORT	void	LED_red_inv(void)
{
	PFC.PJDR.BIT.PJ4DT ^= 1;			// 赤LED反転(xor)
	PFC.PJDR.BIT.PJ5DT = 0;				// 緑LED消灯
	return;
}

/* 緑LED点灯 */
EXPORT	void	LED_green_on(void)
{
	PFC.PJDR.BIT.PJ5DT = 1;				// 緑LED点灯
	PFC.PJDR.BIT.PJ4DT = 0;				// 赤LED消灯
	return;
}

/* 緑LED消灯 */
EXPORT	void	LED_green_off(void)
{
	PFC.PJDR.BIT.PJ5DT = 0;				// 緑LED点灯
	return;
}

/* 緑LED反転 */
EXPORT	void	LED_green_inv(void)
{
	PFC.PJDR.BIT.PJ5DT ^= 1;			// 緑LED反転(xor)
	PFC.PJDR.BIT.PJ4DT = 0;				// 赤LED消灯
	return;
}

/* 赤&緑LED消灯 */
EXPORT	void	LED_all_off(void)
{
	PFC.PJDR.BIT.PJ4DT = 0;				// 赤LED消灯
	PFC.PJDR.BIT.PJ5DT = 0;				// 緑LED点灯
	return;
}
#endif	//////////////////////////


/*================================================
;|
;|□WDT_dog_food                《WDTトリガ》
;|  ［書式］#include "sysdepend.h"
;|  ［入力］なし
;|  ［出力］なし
;|  ［Note］・WDTをトリガする。
;|          ・マクロにより実現(/include/sys/sysdepend/app_sh7760/machine_depend.h)
;|
 ================================================*/
#if 0									// マクロに変更
EXPORT	char	WDT_dog_food(void)
{
	volatile char *addr;

	addr = (char *)DOG_FOOD_ADDR;
	return (*addr);						// WDTトリガ
}
#endif


/*================================================
;|
;|□mon_wait_1ms                《約1m秒の待ち》
;|  ［書式］#include "sysdepend.h"
;|  ［入力］count…待ち回数[ms]
;|  ［出力］なし
;|  ［Note］・ﾘﾌﾚｯｼｭｶｳﾝﾀを利用して待ちﾀｲﾐﾝｸﾞを計る。
;|          ・待ち同時にWDTをﾄﾘｶﾞする。
;|
 ================================================*/
#define	RFCR_1MS_COUNT	64				// 15.6μ×64＝1m秒

EXPORT	void mon_wait_1ms(W count)
{
	UH c0, c1, tmp;

	for ( ; count > 0; count--) {
		//	( 1m秒待ち )
		c0 = BSC.RFCR;					// W:現在のﾘﾌﾚｯｼｭｶｳﾝﾀを獲得
		do {
			WDT_dog_food();				// WDTトリガ

			c1 = BSC.RFCR;				// W:現在のﾘﾌﾚｯｼｭｶｳﾝﾀを獲得
			tmp = (c1 - c0) & 0x03FF;	// ｶｳﾝﾀの差分をﾏｽｸ
		} while (tmp < RFCR_1MS_COUNT);	// 約1m秒待ち
	}

	return;
}


/*----------------------------------------------------------------------*/
#if 0
#|【sysdepend.c 変更履歴】
#|□2015/12/02	[app_sh7760]用に、新規作成。
#|
#endif
