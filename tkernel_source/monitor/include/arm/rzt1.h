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
 *    Modified by T.Yokobayashi at 2016/03/14.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)rzt1.h () 2016/04/12
 *
 *       Renesas RZ/T1 (RZT1) register definitions (excerpt)
 *
 *       * this is included from assembler program source files
 */

#ifndef	__MONITOR_ARM_RZT1_H__
#define	__MONITOR_ARM_RZT1_H__

#include <tk/sysdef.h>



















#define	AB0Base			0x2fff0000
#define	AB0_FLASHCOMSET		(AB0Base + 0x0000)
#define	AB0_FLASHCOMLATCH	(AB0Base + 0x0004)
#define	AB0_FLASHCOMADD0	(AB0Base + 0x0010)
#define	AB0_FLASHCOMDATA0	(AB0Base + 0x0014)
#define	AB0_FLASHCOMADD1	(AB0Base + 0x0018)
#define	AB0_FLASHCOMDATA1	(AB0Base + 0x001c)
#define	AB0_FLASHCLKCTRL	(AB0Base + 0x0080)
#define	AB0_FLA_RCLK_DLY	(AB0Base + 0x0084)
#define	AB0_WAIT_STATUS		(AB0Base + 0x0090)
#define	AB0_CSnBASEADD(n)	(AB0Base + 0x0100 + 0x0010 * (n))
#define	AB0_CSnBITCOMP(n)	(AB0Base + 0x0104 + 0x0010 * (n))
#define	AB0_CSnWAITCTRL(n)	(AB0Base + 0x0200 + 0x0020 * (n))
#define	AB0_CSnWAITCTRL_W(n)	(AB0Base + 0x0204 + 0x0020 * (n))
#define	AB0_CSnREADCTRL(n)	(AB0Base + 0x0208 + 0x0020 * (n))
#define	AB0_CSnWAIT_MASK(n)	(AB0Base + 0x020c + 0x0020 * (n))
#define	AB0_CSnCONTROL(n)	(AB0Base + 0x0210 + 0x0020 * (n))
#define	AB0_CSnFLASHRCR(n)	(AB0Base + 0x0214 + 0x0020 * (n))
#define	AB0_CSnFLASHWCR(n)	(AB0Base + 0x0218 + 0x0020 * (n))
#define	AB0_CSnWAITCTRL2(n)	(AB0Base + 0x0300 + 0x0020 * (n))
#define	AB0_CSnWAITCTRL_W2(n)	(AB0Base + 0x0304 + 0x0020 * (n))
#define	AB0_CSnREADCTRL2(n)	(AB0Base + 0x0308 + 0x0020 * (n))
#define	AB0_CSnWAIT_MASK2(n)	(AB0Base + 0x030c + 0x0020 * (n))
#define	AB0_CSnCONTROL2(n)	(AB0Base + 0x0310 + 0x0020 * (n))
#define	AB0_CSnFLASHRCR2(n)	(AB0Base + 0x0314 + 0x0020 * (n))
#define	AB0_CSnFLASHWCR2(n)	(AB0Base + 0x0318 + 0x0020 * (n))

#define	LCDBase			0x40270000
#define	LCD_CONTROL		(LCDBase + 0x0000)
#define	LCD_QOS			(LCDBase + 0x0004)
#define	LCD_DATAREQ		(LCDBase + 0x0008)
#define	LCD_LCDOUT		(LCDBase + 0x0010)
#define	LCD_BUSSEL		(LCDBase + 0x0014)
#define	LCD_STATUS		(LCDBase + 0x0018)
#define	LCD_BACKCOLOR		(LCDBase + 0x001c)
#define	LCD_AREAADR		(LCDBase + 0x0020)
#define	LCD_HOFFSET		(LCDBase + 0x0024)
#define	LCD_IFORMAT		(LCDBase + 0x0028)
#define	LCD_RESIZE		(LCDBase + 0x002c)
#define	LCD_HTOTAL		(LCDBase + 0x0030)
#define	LCD_HAREA		(LCDBase + 0x0034)
#define	LCD_HEDGE1		(LCDBase + 0x0038)
#define	LCD_HEDGE2		(LCDBase + 0x003c)
#define	LCD_VTOTAL		(LCDBase + 0x0040)
#define	LCD_VAREA		(LCDBase + 0x0044)
#define	LCD_VEDGE1		(LCDBase + 0x0048)
#define	LCD_VEDGE2		(LCDBase + 0x004c)
#define	LCD_INTSTATUS		(LCDBase + 0x0060)
#define	LCD_INTRAWSTATUS	(LCDBase + 0x0064)
#define	LCD_INTENSET		(LCDBase + 0x0068)
#define	LCD_INTENCLR		(LCDBase + 0x006c)
#define	LCD_INTFFCLR		(LCDBase + 0x0070)
#define	LCD_FRAMECNT		(LCDBase + 0x0074)

#define	UARTnBase(n)		(0x50000000 + 0x00010000 * (n))
#define	UART0			0x00
#define	UART1			0x01
#define	UART2			0x02
/* omitted */

#define	SDIxBase(x)		(0x50050000 + 0x00010000 * (x))
#define	SDIA			0x00
#define	SDIB			0x01
#define	SDIC			0x04
#define	SDIx_CMD(x)		(SDIxBase(x) + 0x0000)
#define	SDIx_PORT(x)		(SDIxBase(x) + 0x0004)
#define	SDIx_ARG0(x)		(SDIxBase(x) + 0x0008)
#define	SDIx_ARG1(x)		(SDIxBase(x) + 0x000c)
#define	SDIx_STOP(x)		(SDIxBase(x) + 0x0010)
#define	SDIx_SECCNT(x)		(SDIxBase(x) + 0x0014)
#define	SDIx_RSP0(x)		(SDIxBase(x) + 0x0018)
#define	SDIx_RSP1(x)		(SDIxBase(x) + 0x001c)
#define	SDIx_RSP2(x)		(SDIxBase(x) + 0x0020)
#define	SDIx_RSP3(x)		(SDIxBase(x) + 0x0024)
#define	SDIx_RSP4(x)		(SDIxBase(x) + 0x0028)
#define	SDIx_RSP5(x)		(SDIxBase(x) + 0x002c)
#define	SDIx_RSP6(x)		(SDIxBase(x) + 0x0030)
#define	SDIx_RSP7(x)		(SDIxBase(x) + 0x0034)
#define	SDIx_INFO1(x)		(SDIxBase(x) + 0x0038)
#define	SDIx_INFO2(x)		(SDIxBase(x) + 0x003c)
#define	SDIx_INFO1_MASK(x)	(SDIxBase(x) + 0x0040)
#define	SDIx_INFO2_MASK(x)	(SDIxBase(x) + 0x0044)
#define	SDIx_CLK_CTRL(x)	(SDIxBase(x) + 0x0048)
#define	SDIx_SIZE(x)		(SDIxBase(x) + 0x004c)
#define	SDIx_OPTION(x)		(SDIxBase(x) + 0x0050)
#define	SDIx_ERR_STS1(x)	(SDIxBase(x) + 0x0058)
#define	SDIx_ERR_STS2(x)	(SDIxBase(x) + 0x005c)
#define	SDIx_BUF0(x)		(SDIxBase(x) + 0x0060)
#define	SDIx_SDIO_MODE(x)	(SDIxBase(x) + 0x0068)
#define	SDIx_SDIO_INFO1(x)	(SDIxBase(x) + 0x006c)
#define	SDIx_SDIO_INFO1_MASK(x)	(SDIxBase(x) + 0x0070)
#define	SDIx_CC_EXT_MODE(x)	(SDIxBase(x) + 0x01b0)
#define	SDIx_SOFT_RST(x)	(SDIxBase(x) + 0x01c0)
#define	SDIx_VERSION(x)		(SDIxBase(x) + 0x01c4)
#define	SDIx_USER(x)		(SDIxBase(x) + 0x0200)
#define	SDIx_USER2(x)		(SDIxBase(x) + 0x0204)
#define	SDIx_DMA(x)		(SDIxBase(x) + 0x0300)

#define	AB1Base			0x50070000
#define	AB1_ERROR		(AB1Base + 0x0000)
#define	AB1_GENERAL		(AB1Base + 0x0004)
#define	AB1_DEBUG0		(AB1Base + 0x0008)

#define	USBBase			0x60000000
/* omitted */

#define	TimerBase(x)		(0xc0000000 + 0x00000100 * (x))
#define	TI0			0x00
#define	TI1			0x01
#define	TI2			0x02
#define	TI3			0x03
#define	TW0			0x10
#define	TW1			0x11
#define	TW2			0x12
#define	TW3			0x13
#define	TG0			0x20
#define	TG1			0x21
#define	TG2			0x22
#define	TG3			0x23
#define	TG4			0x24
#define	TG5			0x25
#define	Txx_OP(x)		(TimerBase(x) + 0x0000)
#define	Txx_CLR(x)		(TimerBase(x) + 0x0004)
#define	Txx_SET(x)		(TimerBase(x) + 0x0008)
#define	Txx_RCR(x)		(TimerBase(x) + 0x000c)
#define	Txx_SCLR(x)		(TimerBase(x) + 0x0014)

#define	AINTBase		0xc0020000
#define	IT0_IEN0		(AINTBase + 0x0000)
#define	IT0_IEN1		(AINTBase + 0x0004)
#define	IT0_IDS0		(AINTBase + 0x0008)
#define	IT0_IDS1		(AINTBase + 0x000c)
#define	IT0_RAW0		(AINTBase + 0x0010)
#define	IT0_RAW1		(AINTBase + 0x0014)
#define	IT0_MST0		(AINTBase + 0x0018)
#define	IT0_MST1		(AINTBase + 0x001c)
#define	IT0_IIR			(AINTBase + 0x0024)
#define	IT0_IPI3_SET		(AINTBase + 0x003c)
#define	IT3_IPI0_CLR		(AINTBase + 0x005c)
#define	IT0_FIE			(AINTBase + 0x0080)
#define	IT0_FID			(AINTBase + 0x0084)
#define	IT0_IEN2		(AINTBase + 0x0100)
#define	IT0_IDS2		(AINTBase + 0x0104)
#define	IT0_RAW2		(AINTBase + 0x0108)
#define	IT0_MST2		(AINTBase + 0x010c)
#define	IT_PINV_SET0		(AINTBase + 0x0300)
#define	IT_PINV_SET1		(AINTBase + 0x0304)
#define	IT_PINV_SET2		(AINTBase + 0x0308)
#define	IT_PINV_CLR0		(AINTBase + 0x0310)
#define	IT_PINV_CLR1		(AINTBase + 0x0314)
#define	IT_PINV_CLR2		(AINTBase + 0x0318)
#define	IT0_LIIS		(AINTBase + 0x0320)
#define	IT0_LIIR		(AINTBase + 0x0324)
#define	IT3_IEN0		(AINTBase + 0xc000)
#define	IT3_IEN1		(AINTBase + 0xc004)
#define	IT3_IDS0		(AINTBase + 0xc008)
#define	IT3_IDS1		(AINTBase + 0xc00c)
#define	IT3_RAW0		(AINTBase + 0xc010)
#define	IT3_RAW1		(AINTBase + 0xc014)
#define	IT3_MST0		(AINTBase + 0xc018)
#define	IT3_MST1		(AINTBase + 0xc01c)
#define	IT3_IIR			(AINTBase + 0xc024)
#define	IT3_IPI0_SET		(AINTBase + 0xc030)
#define	IT0_IPI3_CLR		(AINTBase + 0xc050)
#define	ID_VBS			(AINTBase + 0xc090)
#define	ID_CLR			(AINTBase + 0xc094)
#define	IT3_IEN2		(AINTBase + 0xc100)
#define	IT3_IDS2		(AINTBase + 0xc104)
#define	IT3_RAW2		(AINTBase + 0xc108)
#define	IT3_MST2		(AINTBase + 0xc10c)

#define	ASINTBase		0xcc010000
#define	IT0_IENS0		(ASINTBase + 0xe200)
#define	IT0_IENS1		(ASINTBase + 0xe204)
#define	IT0_IENS2		(ASINTBase + 0xe208)
#define	IT0_IDSS0		(ASINTBase + 0xe20c)
#define	IT0_IDSS1		(ASINTBase + 0xe210)
#define	IT0_IDSS2		(ASINTBase + 0xe214)

#define	GIOBase(x)		(0xc0050000 + 0x00000040 * (x))
#define	GIO_L			0x00
#define	GIO_H			0x01
#define	GIO_HH			0x02
#define	GIO_HHH			0x08
#define	GIO_E1(x)		(GIOBase(x) + 0x0000)
#define	GIO_E0(x)		(GIOBase(x) + 0x0004)
#define	GIO_EM(x)		(GIOBase(x) + 0x0004)
#define	GIO_OL(x)		(GIOBase(x) + 0x0008)
#define	GIO_OH(x)		(GIOBase(x) + 0x000c)
#define	GIO_I(x)		(GIOBase(x) + 0x0010)
#define	GIO_IIA(x)		(GIOBase(x) + 0x0014)
#define	GIO_IEN(x)		(GIOBase(x) + 0x0018)
#define	GIO_IDS(x)		(GIOBase(x) + 0x001c)
#define	GIO_IIM(x)		(GIOBase(x) + 0x001c)
#define	GIO_RAW(x)		(GIOBase(x) + 0x0020)
#define	GIO_MST(x)		(GIOBase(x) + 0x0024)
#define	GIO_IIR(x)		(GIOBase(x) + 0x0028)
#define	GIO_GSW(x)		(GIOBase(x) + 0x003c)
#define	GIO_IDT0(x)		(GIOBase(x) + 0x0100)
#define	GIO_IDT1(x)		(GIOBase(x) + 0x0104)
#define	GIO_IDT2(x)		(GIOBase(x) + 0x0108)
#define	GIO_IDT3(x)		(GIOBase(x) + 0x010c)
#define	GIO_RAWBL(x)		(GIOBase(x) + 0x0110)
#define	GIO_RAWBH(x)		(GIOBase(x) + 0x0114)
#define	GIO_IRBL(x)		(GIOBase(x) + 0x0118)
#define	GIO_IRBH(x)		(GIOBase(x) + 0x011c)

#define	MEMCBase		0xc00a0000
#define	MEMC_CACHE_MODE		(MEMCBase + 0x0000)
#define	MEMC_DEGFUN		(MEMCBase + 0x0008)
#define	MEMC_INTSTATUS_A	(MEMCBase + 0x0014)
#define	MEMC_INTRAWSTATUS_A	(MEMCBase + 0x0018)
#define	MEMC_INTENSET_A		(MEMCBase + 0x001c)
#define	MEMC_INTENCLR_A		(MEMCBase + 0x0020)
#define	MEMC_INTFFCLR_A		(MEMCBase + 0x0024)
#define	MEMC_ERRMID		(MEMCBase + 0x0068)
#define	MEMC_ERRADR		(MEMCBase + 0x006c)
#define	MEMC_REQSCH		(MEMCBase + 0x1000)
#define	MEMC_DDR_CONFIGF	(MEMCBase + 0x2000)
#define	MEMC_DDR_CONFIGA1	(MEMCBase + 0x2004)
#define	MEMC_DDR_CONFIGA2	(MEMCBase + 0x2008)
#define	MEMC_DDR_CONFIGC1	(MEMCBase + 0x200c)
#define	MEMC_DDR_CONFIGC2	(MEMCBase + 0x2010)
#define	MEMC_DDR_CONFIGR1	(MEMCBase + 0x2014)
#define	MEMC_DDR_CONFIGR2	(MEMCBase + 0x2018)
#define	MEMC_DDR_CONFIGR3	(MEMCBase + 0x201c)
#define	MEMC_DDR_CONFIGT1	(MEMCBase + 0x2020)
#define	MEMC_DDR_CONFIGT2	(MEMCBase + 0x2024)
#define	MEMC_DDR_CONFIGT3	(MEMCBase + 0x2028)
#define	MEMC_DDR_STATE8		(MEMCBase + 0x202c)

#define	PMUBase			0xc0100000
#define	PMU_PC			(PMUBase + 0x0004)
#define	PMU_START		(PMUBase + 0x0008)
#define	PMU_POWER_ON_PC		(PMUBase + 0x0030)
#define	PMU_WDT_COUNT_EN	(PMUBase + 0x0060)
#define	PMU_WDT_COUNT_LMT	(PMUBase + 0x0064)
#define	PMU_INT_HANDLER_PC	(PMUBase + 0x0068)
#define	PMU_PSR			(PMUBase + 0x0070)
#define	PMU_TRIG_STATUS		(PMUBase + 0x0074)
#define	PMU_REGA		(PMUBase + 0x0078)
#define	PMU_REGB		(PMUBase + 0x007c)
#define	PMU_INTSTATUS_A		(PMUBase + 0x0080)
#define	PMU_INTRAWSTATUS_A	(PMUBase + 0x0084)
#define	PMU_INTENSET_A		(PMUBase + 0x0088)
#define	PMU_INTENCLR_A		(PMUBase + 0x008c)
#define	PMU_INTFFCLR_A		(PMUBase + 0x0090)
#define	PMU_PCERR		(PMUBase + 0x00a8)
#define	PMU_CMD_BUF_RAM		(PMUBase + 0x1000)
#define	PMU_CMD_BUF_FF		(PMUBase + 0x2000)

#define	ASMUBase		0xc0110000
#define	RESETCTRL0		(ASMUBase + 0x0000)
#define	RESETREQ0		(ASMUBase + 0x0004)
#define	RESETREQ0ENA		(ASMUBase + 0x0008)
#define	RESETREQ1		(ASMUBase + 0x000c)
#define	RESETREQ1ENA		(ASMUBase + 0x0010)
#define	RESETREQ2		(ASMUBase + 0x0018)
#define	RESETREQ2ENA		(ASMUBase + 0x001c)
#define	WDT_INT_RESET		(ASMUBase + 0x0020)
#define	RESET_PCLK_COUNT	(ASMUBase + 0x0024)
#define	AUTO_MODE_EN		(ASMUBase + 0x007c)
#define	CLK_MODE_SEL		(ASMUBase + 0x0080)
#define	PLL1CTRL0		(ASMUBase + 0x0084)
#define	PLL1CTRL1		(ASMUBase + 0x0088)
#define	PLL2CTRL0		(ASMUBase + 0x008c)
#define	PLL2CTRL1		(ASMUBase + 0x0090)
#define	PLL3CTRL0		(ASMUBase + 0x0094)
#define	PLL3CTRL1		(ASMUBase + 0x0098)
#define	PLLLOCKTIME		(ASMUBase + 0x009c)
#define	AUTO_PLL_STANDBY	(ASMUBase + 0x00a8)
#define	PLLVDDWAIT		(ASMUBase + 0x00b4)
#define	CLKSTOPSIG_ST		(ASMUBase + 0x00c4)
#define	CLK32_STATUS		(ASMUBase + 0x00c8)
#define	POWER_RECORD		(ASMUBase + 0x00cc)
#define	ASMU_INT_STATUS		(ASMUBase + 0x00d0)
#define	ASMU_INT_RAW_STATUS	(ASMUBase + 0x00d4)
#define	ASMU_INT_ENSET		(ASMUBase + 0x00d8)
#define	ASMU_INT_ENCLR		(ASMUBase + 0x00dc)
#define	ASMU_INT_ENMON		(ASMUBase + 0x00e0)
#define	ASMU_INT_CLEAR		(ASMUBase + 0x00e4)
#define	NORMALA_DIV		(ASMUBase + 0x00f0)
#define	NORMALB_DIV		(ASMUBase + 0x00f4)
#define	NORMALC_DIV		(ASMUBase + 0x00f8)
#define	NORMALD_DIV		(ASMUBase + 0x00fc)
#define	ECONOMY_DIV		(ASMUBase + 0x0100)
#define	STANDBY_DIV		(ASMUBase + 0x0104)
#define	POWERON_DIV		(ASMUBase + 0x0108)
#define	DIVSP0SCLK		(ASMUBase + 0x0118)
#define	DIVSP1SCLK		(ASMUBase + 0x011c)
#define	DIVSP2SCLK		(ASMUBase + 0x0120)
#define	DIVMEMCRCLK		(ASMUBase + 0x0128)
#define	DIVCAMSCLK		(ASMUBase + 0x012c)
#define	DIVLCDLCLK		(ASMUBase + 0x0130)
#define	DIVIICSCLK		(ASMUBase + 0x0134)
#define	TI0TIN_SEL		(ASMUBase + 0x0138)
#define	TI1TIN_SEL		(ASMUBase + 0x013c)
#define	TI2TIN_SEL		(ASMUBase + 0x0140)
#define	TI3TIN_SEL		(ASMUBase + 0x0144)
#define	TIGnTIN_SEL		(ASMUBase + 0x0148)
#define	DIVTIMTIN		(ASMUBase + 0x014c)
#define	DIVMWISCLK		(ASMUBase + 0x0150)
#define	DIVDMATCLK		(ASMUBase + 0x0154)
#define	DIVU70SCLK		(ASMUBase + 0x0158)
#define	DIVU71SCLK		(ASMUBase + 0x015c)
#define	DIVU72SCLK		(ASMUBase + 0x0160)
#define	DIVPM0SCLK		(ASMUBase + 0x016c)
#define	DIVPM1SCLK		(ASMUBase + 0x0170)
#define	DIVREFCLK		(ASMUBase + 0x0178)
#define	DIVPWMPWCLK		(ASMUBase + 0x0184)
#define	AHBCLKCTRL0		(ASMUBase + 0x01a0)
#define	AHBCLKCTRL1		(ASMUBase + 0x01a4)
#define	APBCLKCTRL0		(ASMUBase + 0x01a8)
#define	APBCLKCTRL1		(ASMUBase + 0x01ac)
#define	CLKCTRL			(ASMUBase + 0x01b0)
#define	GCLKCTRL0		(ASMUBase + 0x01b4)
#define	GCLKCTRL0ENA		(ASMUBase + 0x01b8)
#define	GCLKCTRL1		(ASMUBase + 0x01bc)
#define	GCLKCTRL1ENA		(ASMUBase + 0x01c0)
#define	GCLKCTRL2		(ASMUBase + 0x01c4)
#define	GCLKCTRL2ENA		(ASMUBase + 0x01c8)
#define	GCLKCTRL3		(ASMUBase + 0x01cc)
#define	GCLKCTRL3ENA		(ASMUBase + 0x01d0)
#define	AUTO_FRQ_CHANGE		(ASMUBase + 0x01dc)
#define	AUTO_FRQ_MASK0		(ASMUBase + 0x01e0)
#define	AUTO_FRQ_MASK1		(ASMUBase + 0x01e4)
#define	DFS_HALFMODE		(ASMUBase + 0x01e8)
#define	FLA_CLK_DLY		(ASMUBase + 0x01f0)
#define	MEMCCLK270_SEL		(ASMUBase + 0x01fc)
#define	ASMU_BGCTRL		(ASMUBase + 0x0208)
#define	QR_ENA			(ASMUBase + 0x0220)
#define	QR_CLKDIV		(ASMUBase + 0x0224)
#define	FAKE_MODE		(ASMUBase + 0x0238)
#define	POWERSW_STATUS		(ASMUBase + 0x023c)
#define	POWERSW_ENA		(ASMUBase + 0x0240)
#define	L1_POWERSW		(ASMUBase + 0x0244)
#define	ACPU_POWERSW		(ASMUBase + 0x0248)
#define	ADSP_POWERSW		(ASMUBase + 0x024c)
#define	ACPU_BUB		(ASMUBase + 0x0254)
#define	ADSP_BUB		(ASMUBase + 0x0258)
#define	POWERSW_ACTRL_EN	(ASMUBase + 0x025c)
#define	LOG1SW_ACTRL		(ASMUBase + 0x0260)
#define	ADSPSW_ACTRL		(ASMUBase + 0x0264)
#define	L1_BUZ			(ASMUBase + 0x0268)
#define	L1_BUZ2			(ASMUBase + 0x026c)
#define	ACPUBUFTYPE		(ASMUBase + 0x0288)
#define	ADSPUBUFTYPE		(ASMUBase + 0x028c)
#define	HXBBUFTYPE		(ASMUBase + 0x0290)
#define	STATUS_RECORD(n)	(ASMUBase + 0x0320 + 0x004 *(n))
#define	ACPU_INIT		(ASMUBase + 0x0360)
#define	AB1_U70WAITCTRL		(ASMUBase + 0x03c0)
#define	AB1_U71WAITCTRL		(ASMUBase + 0x03c4)
#define	AB1_U72WAITCTRL		(ASMUBase + 0x03c8)
#define	AB1_IIC2WAITCTRL	(ASMUBase + 0x03cc)
#define	AB1_IICWAITCTRL		(ASMUBase + 0x03d0)
#define	AB1_U70READCTRL		(ASMUBase + 0x03d4)
#define	AB1_U71READCTRL		(ASMUBase + 0x03d8)
#define	AB1_U72READCTRL		(ASMUBase + 0x03dc)
#define	AB1_IIC2READCTRL	(ASMUBase + 0x03e0)
#define	AB1_IICREADCTRL		(ASMUBase + 0x03e4)
#define	AB1_SDIBWAITCTRL	(ASMUBase + 0x03e8)
#define	AB1_SDIBREADCTRL	(ASMUBase + 0x03ec)
#define	AB1_SDICWAITCTRL	(ASMUBase + 0x03f0)
#define	AB1_SDICREADCTRL	(ASMUBase + 0x03f4)
#define	FLASHCLK_CTRL		(ASMUBase + 0x0494)
#define	L2_POWERSW_BUZ		(ASMUBase + 0x0500)
#define	LOG2SW_ACTRLEN		(ASMUBase + 0x0504)
#define	LOG2SW_ACTRL		(ASMUBase + 0x0508)
#define	L3_POWERSW_BUZ		(ASMUBase + 0x050c)
#define	LOG3SW_ACTRLEN		(ASMUBase + 0x0510)
#define	LOG3SW_ACTRL		(ASMUBase + 0x0514)
#define	PLL_STATUS		(ASMUBase + 0x0520)
#define	IO_L0_LM_BUZ		(ASMUBase + 0x0814)
#define	RESETREQ3		(ASMUBase + 0x083c)
#define	RESETREQ3ENA		(ASMUBase + 0x0840)
#define	APBCLKCTRL2		(ASMUBase + 0x0848)
#define	GCLKCTRL4		(ASMUBase + 0x084c)
#define	GCLKCTRL4ENA		(ASMUBase + 0x0850)
#define	AUTO_FRQ_MASK3		(ASMUBase + 0x0860)
#define	DFS_FIFOMODE		(ASMUBase + 0x0864)
#define	DFS_FIFO_REQMASK	(ASMUBase + 0x0868)
#define	LCD_FIFOTHRESHOLD	(ASMUBase + 0x086c)
#define	CAM_FIFOTHRESHOLD	(ASMUBase + 0x0870)
#define	CAM_SAFE_RESET		(ASMUBase + 0x0878)
#define	DTV_SAFE_RESET		(ASMUBase + 0x0880)
#define	USB_SAFE_RESET		(ASMUBase + 0x0884)
#define	CLKCTRL1		(ASMUBase + 0x088c)
#define	AB1_SDIAWAITCTRL	(ASMUBase + 0x0890)
#define	AB1_SDIAREADCTRL	(ASMUBase + 0x0894)
#define	MEMC_HAND_SHAKE_FAKE	(ASMUBase + 0x08a0)
#define	SEL_BIGWEST		(ASMUBase + 0x08b8)

#define	SPnBase(n)		(0xc0120000 + 0x00010000 * (n))
#define	SP0			0x00
#define	SP1			0x01
#define	SPn_MODE(n)		(SPnBase(n) + 0x0000)
#define	SPn_POL(n)		(SPnBase(n) + 0x0004)
#define	SPn_CONTROL(n)		(SPnBase(n) + 0x0008)
#define	SPn_TX_DATA(n)		(SPnBase(n) + 0x0010)
#define	SPn_RX_DATA(n)		(SPnBase(n) + 0x0014)
#define	SPn_STATUS(n)		(SPnBase(n) + 0x0018)
#define	SPn_RAW_STATUS(n)	(SPnBase(n) + 0x001c)
#define	SPn_ENSET(n)		(SPnBase(n) + 0x0020)
#define	SPn_ENCLR(n)		(SPnBase(n) + 0x0024)
#define	SPn_FFCLR(n)		(SPnBase(n) + 0x0028)
#define	SPn_CONTROL2(n)		(SPnBase(n) + 0x0034)
#define	SPn_TIECS(n)		(SPnBase(n) + 0x0038)

#define	CHGREGBase		0xc0140000
#define	CHG_BOOT_MODE		(CHGREGBase + 0x0000)
#define	CHG_L1_HOLD		(CHGREGBase + 0x0004)
#define	CHG_LSI_REVISION	(CHGREGBase + 0x0010)
#define	CHG_CTRL_SDINT		(CHGREGBase + 0x0104)
#define	CHG_CTRL_AB0_BOOT	(CHGREGBase + 0x0108)
#define	CHG_CTRL_OSC		(CHGREGBase + 0x0110)
#define	CHG_PINSEL_G(n)		(CHGREGBase + 0x0200 + 0x0004 * ((n) / 16))
#define	CHG_PINSEL_SP0		(CHGREGBase + 0x0280)
#define	CHG_PINSEL_DTV		(CHGREGBase + 0x0284)
#define	CHG_PINSEL_SD0		(CHGREGBase + 0x0288)
#define	CHG_PINSEL_SD1		(CHGREGBase + 0x028c)
#define	CHG_PINSEL_IIC2		(CHGREGBase + 0x0290)
#define	CHG_PINSEL_REFCLKO	(CHGREGBase + 0x0294)
#define	CHG_PULL_G(n)		(CHGREGBase + 0x0300 + 0x0004 * ((n) / 8))
#define	CHG_PULL(n)		(CHGREGBase + 0x0380 + 0x0004 * (n))
#define	CHG_DRIVE(n)		(CHGREGBase + 0x0400 + 0x0004 * (n))

#define	IICBase(x)		(0x50030000 + 0x00010000 * (x))
#define	IIC			0x01
#define	IIC2			0x00
#define	IIC_IIC(n)		(IICBase(n) + 0x0000)
#define	IIC_IICC(n)		(IICBase(n) + 0x0008)
#define	IIC_SVA(n)		(IICBase(n) + 0x000c)
#define	IIC_IICCL(n)		(IICBase(n) + 0x0010)
#define	IIC_IICSE(n)		(IICBase(n) + 0x001c)
#define	IIC_IICF(n)		(IICBase(n) + 0x0028)

#endif /*__MONITOR_ARM_RZT1_H__*/


/*----------------------------------------------------------------------*/
#if 0
#|History of "rzt1.h"
#|======================
#|* 2016/02/29	"em1d512.h"から、 [app_rzt1]用にコピーし修正。
#|
#endif
