/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2015 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2015/11/28.
 *
 *----------------------------------------------------------------------
 */

/*
 *	setup_sh7760.h () 2015/11/28
 *
 *       SH7760(SH-4) configuration information
 *
 *       this file included from assembly source files, too.
 */

#ifndef __MONITOR_SETUP_SH7760_H__
#define	__MONITOR_SETUP_SH7760_H__


/*******************************************************************************
 *   IOP-5 constant define
 * ※基板固有な情報を本ファイルに集約する
 *******************************************************************************/
///MON_REVISION	.DEFINE		"Rev1.26b 2015/04/07"

#define	BOOT_MESSAGE	1		/* 0:表示しない, 1:する */


///IOP5_VERSION	.DEFINE		"0"	;0:試作(JCB09B30～),1:量産(JCB?)※現在不使用
/////;DBLC_ON	.ASSIGNA	2	;CLK倍速設定(1:1倍[6倍], 2:2倍[12倍], 3:3倍  ?)

#define	BOOT_CASH_ON		1	// ﾌﾞｰﾄ時ｷｬｯｼｭ設定(0:OFF,1:ON)
///LCA_CFG		.ASSIGNA	0	;LCAｺﾝﾌｨｸﾞﾚｰｼｮﾝ(0:しない,1:MON,2:BOOT)
///BROM_CHK	.ASSIGNA	1	;BROMのｻﾑﾁｪｯｸ(1:する,0:しない)
///IRAM_USE	.ASSIGNA	1	;内蔵RAM使用(0:しない,1:する)
///IWDT_USE	.ASSIGNA	0	;内蔵WDT使用(0:しない,1:する)
#define	CPG_CHANGE			2	/* ｸﾛｯｸ発信回路変更 (0:しない,1:する,2:するIOP5用) */
///FMP_USE		.ASSIGNA	1	;FMP使用(0:しない,1:する)
///FMEM_TYPE	.DEFINE		"1"	;FMEM種類(0:MBM29LV800T,
///					;	  1:SST39VF800A/SST39VF1601)

///MON_REVCODE	.EQU	H'30001262	;revision code(32BIT),SH4だけど互換'3'
///CKIO_CLOCK	.EQU	64		;CKIOｸﾛｯｸ周波数(MHz)
///P_CLOCK		.EQU	32		;周辺ｸﾛｯｸ周波数(MHz)


/*-------( ﾒﾓﾘｰの番地割当て )-------*/
///BROM_TOP	.EQU	H'A0000000	;CS0ｴﾘｱ BOOT-ROM  TOP ADDRESS
///BROM_CNT	.EQU	H'00010000	;       BOOT-ROM  BYTE SIZE(BOOT部のみ)
///MON_TOP		.EQU	H'A0010000	;内蔵   MON-ROM   TOP ADDRESS
///MON_CNT		.EQU	H'00030000	;       MON-ROM   BYTE SIZE(MONｾｸﾀ)
///FMEM_MON_OFFS	.EQU	H'00010000	;CS0ｴﾘｱ MON-AREA  OFFSET ADDRESS

///CS1_TOP		.EQU	H'A4000000	;CS1ｴﾘｱ (LCA_uNET2_IOB)

#define	DOG_FOOD_ADDR	0xA8000000		// CS2ｴﾘｱ (WDT)

///DRAM_TOP	.EQU	H'AC000000	;CS3ｴﾘｱ DATA-RAM  TOP ADDRESS(NonCash)
/////;DRAM_TOP	.EQU	H'CC000000	;///CS3ｴﾘｱ DATA-RAM  TOP ADDRESS(CashAble:forTEST!!)
///DRAM_CNT	.EQU	H'00800000	;       DATA-RAM  BYTE SIZE
/////;///DRAM_CNT	.EQU	H'00200000	;///       DATA-RAM  BYTE SIZE(old iop)
///CS4_TOP		.EQU	H'B0000000	;CS4ｴﾘｱ 予備(LCA_uNET2_BANK)
///CS4_CNT		.EQU	H'00000000	;

///FMEM_TOP	.EQU	H'B4000000	;CS5ｴﾘｱ FLASH-MEM TOP ADDRESS ※BOOT時
///FMEM_CNT	.EQU	H'00100000	;       FLASH-MEM BYTE SIZE
///OPT_CS		.EQU	H'B4000000	;CS5ｴﾘｱ DDA基板ｱﾄﾞﾚｽ          ※通常時

///LCA_PORT_TOP	.EQU	H'B8000000	;CS6ｴﾘｱ LCA_REG

///NTBL_TOP	.EQU	H'CC6E0000	;NODE-TABLE TOP ADDRESS

///;※SH4不具合の為、ｷｬｯｼｭ制御のｲﾝﾃﾞｯｸｽﾓｰﾄﾞ(A25=1)を利用した擬似的な高速RAM
///IRAM_TOP	.EQU	H'0E5F8000	;INTERNAL-RAM TOP ADDRESS
///IRAM_CNT	.EQU	H'00004000	;INTERNAL-RAM BYTE SIZE(16Kbyte)
///IRAM_USER_TOP	.EQU	H'00000000	;INTERNAL-RAM(USER) TOP ADDRESS
///IRAM_USER_CNT	.EQU	H'00000000	;INTERNAL-RAM(USER) BYTE SIZE

/* ※○-------( CPG初期値 )-------*/
#define	WTCSR_CHANGE		0xA504			// ※???!!!(要修正) TME=0,CKS=1/512(240MHz,546μs)

#define	FRQCR_CHANGE		0x0C13			// (TEST:PLL2OFF)CPU=192M,BUS=64M,IO=32M
// #define	FRQCR_CHANGE		0x0E13			// CPU=192M,BUS=64M,IO=32M
#define	DCKDR_CHANGE		0x000D			// ｸﾛｯｸ分周ﾚｼﾞ:PLL3=ON,DCK=OUT,DIV=1/1

//※○-------( BSC初期値 )-------
///;<iop4設定：参考用>-------------
///; BCR1_INIT	.EQU	B'0000000000001000	;ｴﾘｱ機能,ARE2=通常,AREA3=SDRAM
///; BCR2_BOOT	.EQU	B'0010101010100000	;ﾊﾞｽ幅16(BOOT時:AREA5=16bit)
///; BCR2_INIT	.EQU	B'0010011010100000	;ﾊﾞｽ幅16(通常時:AREA5=8bit)
///; WCR1_INIT	.EQU	B'1011011101010001	;ｱｲﾄﾞﾙｽﾃｰﾄ(ｴﾘｱ6,4=3,他=1)
///; WCR2_INIT	.EQU	B'1001011001000101	;ｳｪｲﾄｽﾃｰﾄ(ｴﾘｱ6=4,ｴﾘｱ5=6,ｴﾘｱ4=4)
///;										;        (ｴﾘｱ3=2,ｴﾘｱ2=0,ｴﾘｱ0=6)
///; MCR_INIT	.EQU	B'0101000100100100	;SDRAM設定(TPC=2,RCD=2,TRWL=1)
///;-------------------------------		;(TRAS=3,RASD=0,RFSH=1,RMODE=0)
//<iop5設定>
#define	BCR1_INIT		0x00000008			// ｴﾘｱ機能,ARE0~2,5,6=通常,ARE3=SDRAM32bit
#define	BCR2_BOOT		0b0010101011101100	// ﾊﾞｽ幅(BOOT時:AREA5=16bit,AREA1&3=32bit)
///BCR2_INIT	.EQU	B'0010011011101100	// ﾊﾞｽ幅(通常時:AREA5=8bit ,AREA1&3=32bit)
//                    CSn:_0_6_5_4_3_2_1_X
#define	BCR3_INIT		0b0000000000000000	// ｾｯﾄｱｯﾌﾟ&ﾎｰﾙﾄﾞ
#define	BCR4_INIT		0x00000000			// CSﾎｰﾙﾄﾞｻｲｸﾙ
#define	WCR1_INIT		0x23131111			// ｱｲﾄﾞﾙｽﾃｰﾄ(ｴﾘｱ6,4=3,他=1)

#define	WCR2_INIT		0x92484064			// ｳｪｲﾄｽﾃｰﾄ(ｴﾘｱ6=4->6,ｴﾘｱ5=6,ｴﾘｱ4=4->6)
//											//         (ｴﾘｱ3=2,ｴﾘｱ2=0,ｴﾘｱ1=1,ｴﾘｱ0=6)

#define	WCR3_INIT		0x00000000			// MPX&SRAM(SRAMﾊﾞｰｽﾄ長=8)
#define	MCR_INIT		0x48090194			// SDRAM設定[ MRSET=1,TRC=1(3),
//											// TPC=1(2),RCD=1(2),TRWL=0(1),TRAS=0(4),
//											// SZ=3(32bit),AMX=2,RFSH=1,RMODE=0 ]

#define	SDMR3_INIT		0xFF940000 + (0x0023*4)	// CASﾚﾃﾝｼ=2,ﾊﾞｰｽﾄﾀｲﾌﾟ=ｼｰｹﾝｼｬﾙ,
//													// ﾊﾞｰｽﾄ長=8 (ｴﾘｱ3,32BITﾊﾞｽ)
						
#define	RTCSR_INIT		0b1010010100001000	// ﾘﾌﾚｯｼｭｺﾝﾄﾛｰﾙ(CKIO/4)
#define	RTCNT_INIT		0b1010010100000000	// ﾘﾌﾚｯｼｭｶｳﾝﾀ=0
#define	RTCOR_INIT		0b1010010111111010	// ﾘﾌﾚｯｼｭ周期=200(64MHz,15.6μ秒:通常時)
#define	RTCOR_INIT1	0b1010010100010100	// ﾘﾌﾚｯｼｭ周期=20(BOOT時全ﾊﾞﾝｸ ﾌﾟﾘﾁｬｰｼﾞ用)

//※○-------( PFC初期値 )-------
#define	GPIOIC_INIT	0b0000000000000000	// GPIO割込ｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ
//								// P-CR設定:00周辺,01出力,10入力p-up無し,11入力p-up
#define	PCCR_INIT		0b1010101010101010	// DIPSW(Pupｵﾌ入力)
#define	PDCR_INIT		0b1111111111111111	// LCA予備(Pupｵﾝ入力)
#define	PECR_INIT		0b0010000010001000	// ROM-CN:PTE6,PTE3,PTE1
#define	PGCR_INIT		0b1100000100000001	// SCIF#0(mente),#1(LCA-DEF)'12/03/06修正
#define	PHCR_INIT		0b1100000000111111	// SCIF#2 &USB(Pupｵﾝ入力)'13/02/04修正
#define	PJCR_INIT		0b1111010100110000	//
//								// PJ5&4(LEDout),PJ3->2(DONEin),PJ1(DCKout)

//※○-------( I/Oﾎﾟｰﾄ初期値 )-------
#define	PCDR_INIT		0b00000000			//
#define	PDDR_INIT		0b00000000			//
#define	PEDR_INIT		0b00000000			//
#define	PJDR_INIT		0b00010000			// 赤LEDのみ点灯
#define	PGDR_INIT		0b00010001			// CCLK(G4)='H',DIN(G0)='H'
#define	PHDR_INIT		0b00100000			// RTS2='H'

#define	SCSPTR0_INIT	0x0000				// 通信ﾎﾟｰﾄ信号
#define	SCSPTR1_INIT	0x000C				// SCK='H'
#define	SCSPTR2_INIT	0x0000				// 通信ﾎﾟｰﾄ信号

//※○-------( ｷｬｼｭ初期値 )-------
///#define	CCR_INIT		0x80000929			// IC=ON,OC=ON,ｷｬｼｭｸﾘｱ,ｲﾝﾃﾞｯｸｽ,RAMﾓｰﾄﾞ
///#define	CCR_INIT		0x80000989			// IC=ON,OC=ON,ｷｬｼｭｸﾘｱ,ｲﾝﾃﾞｯｸｽ
#define	CCR_INIT		0x8000098D			// IC=ON,OC=ON,ｷｬｼｭｸﾘｱ,ｲﾝﾃﾞｯｸｽ,P1=CB


//-------( WDT初期値 )-------
#define	WTCSR_INIT			0xA567			// TME=0,WDT,ﾏﾆｭｱﾙﾘｾｯﾄ,CKS=111
#define	WTCNT_INIT			0x5A00			// 



///;※○-------( INTC初期値 )-------
#define	ICR_INIT		0x0000				// NMIの立下りｴｯｼﾞ,IRL3～0ｴﾝｺｰﾄﾞ
#define	INTMSK00_INIT	0xF3FF7FFF			// 全割込禁止
#define	INTMSK04_INIT	0x00FFFFFF			// 全割込禁止
#define	INTMSKCLR00_INIT 0x00000F00			// SCIF0割込許可
#define	INTMSKCLR04_INIT 0x00000000			//



///;-------( TMU初期値 )-------
///TOCR_INIT	.EQU	B'00000000		;TCOE=0(TCLKは外部ｸﾛｯｸ入力)





#if 0	///////////// setup_em1d512.h ///////////////
#define	EITENT_BASE	0x70000000	// address for exception branch processing
#define	PAGETBL_BASE	0x30000000	// address of the first level table page.
#endif	//////////////////////////////////////////

/*
 * clock value
 */
#define	PLL1_CLK	192000000				/* [Hz]:192MHz(16M X 12) */

#define	Ick_CLK		192						/* [MHz]:CPU clock(192MHz) */
#define	Pck_CLK		3200					/* [1/100 MHz]:周辺 clock(32MHz) */
#define	Bck_CLK		6400					/* [1/100 MHz]:バス clock(64MHz) */
#define	dck_CLK		6400					/* [1/100 MHz]:外部 clock(64MHz) */

/*
 * assignment to DipSw (switches)
 */
#define	SW_ABT		0x0100				/* Abort SW */
#define	SW_MON		0x0080				/* Monitor Boot (IOP5:Dipsw No8 on) */
#define	SW_BHI		0x0000				/* fix HI_BAUD_RATE */

/*
 * LED display (two bits, 2 bits)
 */
#define	LED_POWERUP	0x02				/* Power-on (green led on) */
#define	LED_MEMCLR	0x01				/* Memory clear (red led on) */



//-------( 致命的異常LED表示の割当て )-------
#define	RFCR_1MS_COUNT	64				// 15.6μ×64＝1m秒

#define	FATAL_LED_UNIT	250				// FATAL_LED点滅基本時間(250m秒)

///CPU_ERR_LED	.EQU	CW_DATA_C	;"-･-･"	点滅ﾊﾟﾀｰﾝ(Cpu)
///BROM_ERR_LED	.EQU	CW_DATA_O	;"---"	点滅ﾊﾟﾀｰﾝ(rOm)
///FROM_ERR_LED	.EQU	CW_DATA_F	;"･･-･"	点滅ﾊﾟﾀｰﾝ(From)
///LCAI_ERR_LED	.EQU	CW_DATA_L	;"･-･･"	点滅ﾊﾟﾀｰﾝ(Lca init)
///LCAD_ERR_LED	.EQU	CW_DATA_D	;"-･･"	点滅ﾊﾟﾀｰﾝ(lca Done)
///LCAR_ERR_LED	.EQU	CW_DATA_R	;"･-･"	点滅ﾊﾟﾀｰﾝ(lcaﾚｼﾞｽﾀ ﾘｰﾄﾞ/ﾗｲﾄ)
///IRAM_ERR_LED	.EQU	CW_DATA_I	;"･･"	点滅ﾊﾟﾀｰﾝ(Iram)
///DRAM_ERR_LED	.EQU	CW_DATA_A	;"･-"	点滅ﾊﾟﾀｰﾝ(rAm)
///MONSUM_ERR_LED	.EQU	CW_DATA_M	;"--"	点滅ﾊﾟﾀｰﾝ(Mon sum)
///RAMCOPY_ERR_LED	.EQU	CW_DATA_B	;"-･･･"	点滅ﾊﾟﾀｰﾝ(ramCopy)
///FATAL_ERR_LED	.EQU	CW_DATA_S	;"･･･"	点滅ﾊﾟﾀｰﾝ(fatal)
///NUM_ERR_LED	.EQU	CW_DATA_N	;"-･"	点滅ﾊﾟﾀｰﾝ(ﾉｰﾄﾞ番号情報不正)








#ifdef _in_asm_source_

/*******( LEDの操作マクロ for IOP5:PJ#4=RED,#5=GREEN )*******/
 .macro led_gbr_set						// gbr←IOBASE_140
	mov.l	1070f, r0					// r0←IOBASE_140
	ldc		r0, gbr						// gbr←IOBASE address

	bra		1009f						//(遅延分岐), 終了へ
	nop

			.balign 4
  1070:		.long	IOBASE_140

  1009:									// end
 .endm

 .macro led_red_on						// gbr…IOBASE_140
	mov		#PJDR_140, r0				// r0←PJDR offset
	or.b	#0x10, @(r0, gbr)			// 赤LED点灯
	and.b	#0xDF, @(r0, gbr)			// 緑LED消灯
 .endm

 .macro led_red_off						// gbr…IOBASE_140
	mov		#PJDR_140, r0				// r0←PJDR offset
	and.b	#0xEF, @(r0, gbr)			// 赤LED消灯
 .endm

 .macro led_red_inv						// gbr…IOBASE_140
	mov		#PJDR_140, r0				// r0←PJDR offset
	xor.b	#0x10, @(r0,gbr)			// 赤LED反転
	and.b	#0xDF, @(r0,GBR)			// 緑LED消灯
 .endm

 .macro led_green_on					// gbr…IOBASE_140
	mov		#PJDR_140, r0				// r0←PJDR offset
	or.b	#0x20, @(r0, gbr)			// 緑LED点灯
	and.b	#0xEF, @(r0, gbr)			// 赤LED消灯
 .endm

 .macro led_green_off					// gbr…IOBASE_140
	mov		#PJDR_140, r0				// r0←PJDR offset
	and.b	#0xDF, @(r0, gbr)			// 緑LED消灯
 .endm

 .macro led_green_inv					// gbr…IOBASE_140
	mov		#PJDR_140, r0				// r0←PJDR offset
	xor.b	#0x20, @(r0, gbr)			// 緑LED反転
	and.b	#0xEF, @(r0, gbr)			// 赤LED消灯
 .endm

 .macro led_all_off						// gbr…IOBASE_140
	mov		#PJDR_140, r0				// r0←PJDR offset
	and.b	#0xCF, @(r0, gbr)			// 赤&緑LED消灯
 .endm


/*================================================
 * rfcr_wait1  《約1m秒の待ち マクロ》
 *  ［入力］count…待ち回数
 *  ［出力］なし
 *  ［Note］・ﾘﾌﾚｯｼｭｶｳﾝﾀを利用して待ちﾀｲﾐﾝｸﾞを計る。
 *          ・待ち同時にWDTをﾄﾘｶﾞする。
 *  ［破壊］r0,r1,r2,r7
 *================================================*/
 .macro rfcr_wait1 count

	mov.l	1070f, r2					// r2←count

//	( 1m秒待ち )
1001:									// (r2…待ち回数)
	mov.l	1071f, r0					// r0←RFCR_ADDR
	mov.w	@r0, r1						// r1←現在のﾘﾌﾚｯｼｭｶｳﾝﾀを保持

1002:
	mov.l	1071f, r0					// r0←RFCR_ADDR
	mov.w	@r0, r0						// 現在のﾘﾌﾚｯｼｭｶｳﾝﾀを獲得
	sub	r1, r0							// r0←r0-r1
	mov.w	1060f, r7					// r7←0x03FF (ﾏｽｸﾃﾞｰﾀ)
	and	r7, r0
	mov.w	1061f, r7					// r7←RFCR_1MS_COUNT, 比較ﾃﾞｰﾀ(1m秒)
	cmp/hi	r7, r0						// r0＞r7
	bf		1002b						// 約1m秒待ち

	mov.l	1072f, r0					// r0←DOG_FOOD_ADDR
	mov.b	@r0, r0						// WDTﾄﾘｶﾞ

	dt		r2							// R2-1
	bf		1001b

	bra		1009f						//(遅延分岐),終了へ
	nop

		.balign 2
  1060:		.word	0x03FF
  1061:		.word	RFCR_1MS_COUNT

		.balign 4
  1070:		.long	\count
  1071:		.long	RFCR_ADDR
  1072:		.long	DOG_FOOD_ADDR

  1009:									// end
 .endm


 .macro debug_led_loop
  1099:
    led_gbr_set
	led_green_inv
	rfcr_wait1 250				// 0.25秒待ち
	bra		1099b				// to loop
	nop
 .endm

#endif	/* _in_asm_source_ */

#endif /* __MONITOR_SETUP_SH7760_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|History of "setup_sh7760.h"
#|===========================
#|* 2015/11/28	 [app_sh7760]用に新規作成 
#|  ・ [tef_em1d]用の setup_em1d512.h, IOPモニタの _iop5.def を参考に作成。
#|
#endif
