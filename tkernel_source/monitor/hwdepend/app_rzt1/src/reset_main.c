/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright(C) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Created by T.Yokobayashi at 2016/04/22.
 * 
 *----------------------------------------------------------------------
 */

/*
 *	@(#)reset_main.c (RZT1) 2016/06/05
 *
 *       startup reset main routine
 */

#include <machine.h>
#include <misc/renesas/rzt1.h>
#include <misc/renesas/rzt1/r_port.h>


extern void startup_entry(void);


/*
        RZ/T1 initialize routines
*/
static void	rzt1_init(void)
{
	//--- システムクロックコントロールレジスタの設定(p233) ---
	SYSTEM.SCKCR.BIT.SERICK = 0;			/* ｼﾘｱﾙｸﾛｯｸSERICLK=150MHz */


	//--- 低消費電力機能の設定(p260) ---
	SYSTEM.PRCR.LONG = 0x0000a502;			// 低消費電力機能ﾚｼﾞｽﾀ書込許可

	// 周辺モジュール機能を有効化
	SYSTEM.MSTPCRA.BIT.MSTPCRA1 = 0;		// CWTW#0  ﾓｼﾞｭｰﾙｽﾄｯﾌﾟ解除
	SYSTEM.MSTPCRA.BIT.MSTPCRA4 = 0;		// CWT#0   ﾓｼﾞｭｰﾙｽﾄｯﾌﾟ解除
	SYSTEM.MSTPCRB.BIT.MSTPCRB7 = 0;		// SCIFA#2 ﾓｼﾞｭｰﾙｽﾄｯﾌﾟ解除

	SYSTEM.PRCR.LONG = 0x0000a500;			// 低消費電力機能ﾚｼﾞｽﾀ書込禁止


	//--- マルチファンクションピンコントローラの設定(p619) ---
	MPC.PWPR.BIT.B0WI = 0;					// PFSWE書き込み許可
	MPC.PWPR.BIT.PFSWE = 1;					// PFSﾚｼﾞｽﾀ書き込み許可

	MPC.P91PFS.BIT.PSEL = 0b001011;			// TXD2(P91)
	MPC.P92PFS.BIT.PSEL = 0b001011;			// RXD2(P92)

	MPC.PWPR.BIT.B0WI = 0;					// PFSWE書き込み許可
	MPC.PWPR.BIT.PFSWE = 0;					// PFSﾚｼﾞｽﾀ書き込み禁止

	PORT9.PMR.BIT.B1 = 1;					// TXD2(P91)
	PORT9.PMR.BIT.B2 = 1;					// RXD2(P92)


	//--- ＩＯポートの設定(p605) ---
    /* Set PF7 to Output port pin (Using LED0) */
    PORTF.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize port settings
    PORTF.PMR.BIT.B7 = PORT_MODE_GENERAL;

    PORTF.PODR.BIT.B7 = PORT_OUTPUT_LOW;    // Set output level to Low
    PORTF.PDR.BIT.B7 = PORT_DIRECTION_OUTPUT;  // Set port direction to output

    /* Set P56 to Output port pin (Using LED1) */
    PORT5.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize port settings
    PORT5.PMR.BIT.B6 = PORT_MODE_GENERAL;

    PORT5.PODR.BIT.B6 = PORT_OUTPUT_LOW;    // Set output level to Low
    PORT5.PDR.BIT.B6 = PORT_DIRECTION_OUTPUT;  // Set port direction to output

    /* Set P77 to Output port pin (Using LED2) */
    PORT7.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize port settings
    PORT7.PMR.BIT.B7 = PORT_MODE_GENERAL;

    PORT7.PODR.BIT.B7 = PORT_OUTPUT_LOW;    // Set output level to Low
    PORT7.PDR.BIT.B7 = PORT_DIRECTION_OUTPUT;  // Set port direction to output

    /* Set PA0 to Output port pin (Using LED3) */
    PORTA.PDR.BIT.B0 = PORT_DIRECTION_HIZ;  // Initialize port settings
    PORTA.PMR.BIT.B0 = PORT_MODE_GENERAL;

    PORTA.PODR.BIT.B0 = PORT_OUTPUT_LOW;    // Set output level to Low
    PORTA.PDR.BIT.B0 = PORT_DIRECTION_OUTPUT;  // Set port direction to output


	return;
}



/*
 * startup reset main
 */
void reset_main (void)
{
	/* rz/t1 module register initialize */
	rzt1_init();						// RZ/T1 初期設定

    /* Jump to startup_entry */
	startup_entry();					// see "reset.S"
}


/*----------------------------------------------------------------------
#|History of "reset_main.c"
#|=========================
#|* 2016/04/22	new created.(by T.Yokobayashi)
#|
*/
