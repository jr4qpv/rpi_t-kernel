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
 *    Modified by T.Yokobayashi at 2015/11/25.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sh7760io.h () 2015/11/25
 *
 *       RENESAS SH7760(SH-4) register definitions (excerpt)
 *
 *       * this is included from assembler program source files
 *       * 【Note】
 *         8ﾋﾞｯﾄｲﾐﾃﾞｴｲﾄ転送命令でもｵﾌｾｯﾄ値を代入できるように、
 *         ｵﾌｾｯﾄ値を｢0x7F｣以内としている。
 *
 */

#ifndef	__MONITOR_SH_SH7760IO_H__
#define	__MONITOR_SH_SH7760IO_H__

/*******( H'FF000000番地～のIO定義 )*******/
#define	IOBASE_00		0xFF000000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ */

/*	( CCN ) */
#define	PTEH_00			0x00			/* (L):ﾍﾟｰｼﾞﾃｰﾌﾞﾙｴﾝﾄﾘ上位ﾚｼﾞｽﾀ */
#define	PTEL_00			0x04			/* (L):ﾍﾟｰｼﾞﾃｰﾌﾞﾙｴﾝﾄﾘ下位ﾚｼﾞｽﾀ */
#define	TTB_00			0x08			/* (L):変換ﾃｰﾌﾞﾙﾍﾞｰｽﾚｼﾞｽﾀ */
#define	TEA_00			0x0C			/* (L):TLB例外ｱﾄﾞﾚｽﾚｼﾞｽﾀ */
#define	MMUCR_00		0x10			/* (L):MMU制御ﾚｼﾞｽﾀ */
#define	BASRA_00		0x14			/* (B):ﾌﾞﾚｰｸASIDﾚｼﾞｽﾀA */
#define	BASRB_00		0x18			/* (B):ﾌﾞﾚｰｸASIDﾚｼﾞｽﾀB */
#define	CCR_00			0x1C			/* (L):ｷｬｯｼｭ制御ﾚｼﾞｽﾀ2 */
#define	TRA_00			0x20			/* (L):TRAPA例外ﾚｼﾞｽﾀ */
#define	EXPEVT_00		0x24			/* (L):例外事象ﾚｼﾞｽﾀ */
#define	INTEVT_00		0x28			/* (L):割込み事象ﾚｼﾞｽﾀ */
#define	PTEA_00			0x34			/* (L): */
#define	QACR0_00		0x38			/* (L): */
#define	QACR1_00		0x3C			/* (L): */


#define	TRA_ADDR		(IOBASE_00 + TRA_00)
#define	EXPEVT_ADDR		(IOBASE_00 + EXPEVT_00)
#define	INTEVT_ADDR		(IOBASE_00 + INTEVT_00)



/*******( H'FF200000番地～のIO定義 )*******/
#define	IOBASE_20		0xFF200000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ */

/*	( UBC ) */
#define	BARA_20			0x00			/* (L):ﾌﾞﾚｰｸｱﾄﾞﾚｽﾚｼﾞｽﾀA */
#define	BAMRA_20		0x04			/* (B):ﾌﾞﾚｰｸｱﾄﾞﾚｽﾏｽﾀﾚｼﾞｽﾀA */
#define	BBRA_20			0x08			/* (W):ﾌﾞﾚｰｸﾊﾞｽｻｲｸﾙﾚｼﾞｽﾀA */
#define	BARB_20			0x0C			/* (L):ﾌﾞﾚｰｸｱﾄﾞﾚｽﾚｼﾞｽﾀB */
#define	BAMRB_20		0x10			/* (B):ﾌﾞﾚｰｸｱﾄﾞﾚｽﾏｽﾀﾚｼﾞｽﾀB */
#define	BBRB_20			0x14			/* (W):ﾌﾞﾚｰｸﾊﾞｽｻｲｸﾙﾚｼﾞｽﾀB */
#define	BDRB_20			0x18			/*	(L):ﾌﾞﾚｰｸﾃﾞｰﾀﾚｼﾞｽﾀB */
#define	BDMRB_20		0x1C			/* (L):ﾌﾞﾚｰｸﾃﾞｰﾀﾏｽﾀﾚｼﾞｽﾀB */
#define	BRCR_20			0x20			/* (W):ﾌﾞﾚｰｸｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ */


/*******( H'FF800000番地～のIO定義 )*******/
#define	IOBASE_80		0xFF800000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ */

/*	( BSC ) */
#define	BCR1_80			0x00			/* (L):ﾊﾞｽｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ1 */
#define	BCR2_80			0x04			/* (W):ﾊﾞｽｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ2 */
#define	BCR3_80			0x50			/* (W):ﾊﾞｽｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ3 */
#define	WCR1_80			0x08			/* (L):ｳｪｲﾄｽﾃｰﾄｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ1 */
#define	WCR2_80			0x0C			/* (L):ｳｪｲﾄｽﾃｰﾄｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ2 */
#define	WCR3_80			0x10			/* (L):ｳｪｲﾄｽﾃｰﾄｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ3 */
#define	MCR_80			0x14			/* (L):個別ﾒﾓﾘｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ */
#define	PCR_80			0x18			/* (W):PCMCIAｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ */
#define	RTCSR_80		0x1C			/* (W):ﾘﾌﾚｯｼｭﾀｲﾏｺﾝﾄﾛｰﾙ/ｽﾃｰﾀｽﾚｼﾞｽﾀ */
#define	RTCNT_80		0x20			/* (W):ﾘﾌﾚｯｼｭﾀｲﾏｶｳﾝﾀ */
#define	RTCOR_80		0x24			/* (W):ﾘﾌﾚｯｼｭﾀｲﾑｺﾝｽﾀﾝﾄｶｳﾝﾀ */
#define	RFCR_80			0x28			/* (W):ﾘﾌﾚｯｼｭｶｳﾝﾄﾚｼﾞｽﾀ */

/*	( PFC ) */
#define	GPIOIC_80		0x48			/* (W): */


#define	BCR4_ADDR		0xFE0A00F0		/* (L):ﾊﾞｽｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ4 */

#define	SDMR2_ADDR		0xFF900000		/* (B):SDRAMﾓｰﾄﾞﾚｼﾞｽﾀ(ｴﾘｱ2) */
#define	SDMR3_ADDR		0xFF940000		/* (B):SDRAMﾓｰﾄﾞﾚｼﾞｽﾀ(ｴﾘｱ3) */

#define	RFCR_ADDR		(IOBASE_80 + RFCR_80)


/*******( H'FE400000番地～のIO定義 )*******/
#define	IOBASE_140		0xFE400000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ */

/*	( PFC ) */
#define	PACR_140		0x00			/* (W): */
#define	PBCR_140		0x04			/* (W): */
#define	PCCR_140		0x08			/* (W): */
#define	PDCR_140		0x0C			/* (W): */
#define	PECR_140		0x10			/* (W): */
#define	PFCR_140		0x14			/* (W): */
#define	PGCR_140		0x18			/* (W): */
#define	PHCR_140		0x1C			/* (W): */
#define	PJCR_140		0x20			/* (W): */
#define	PKCR_140		0x24			/* (W): */

#define	PADR_140		0x40			/* (B): */
#define	PBDR_140		0x44			/* (B): */
#define	PCDR_140		0x48			/* (B): */
#define	PDDR_140		0x4C			/* (B): */
#define	PEDR_140		0x50			/* (B): */
#define	PFDR_140		0x54			/* (B): */
#define	PGDR_140		0x58			/* (B): */
#define	PHDR_140		0x5C			/* (B): */
#define	PJDR_140		0x60			/* (B): */
#define	PKDR_140		0x64			/* (B): */


/*******( H'FFA00000番地～のIO定義 )*******/
#define	IOBASE_A0		0xFFA00000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ */

/*	( DMAC ) */
#define	SAR0_A0			0x00			/* (L):DMAｿｰｽｱﾄﾞﾚｽﾚｼﾞｽﾀ0 */
#define	DAR0_A0			0x04			/* (L):DMAﾃﾞｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾚｼﾞｽﾀ0 */
#define	DMATCR0_A0		0x08			/* (L):DMAﾄﾗﾝｽﾌｧｶｳﾝﾄﾚｼﾞｽﾀ0 */
#define	CHCR0_A0		0x0C			/* (L):DMAﾁｬﾈﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ0 */
#define	SAR1_A0			0x10			/* (L):DMAｿｰｽｱﾄﾞﾚｽﾚｼﾞｽﾀ1 */
#define	DAR1_A0			0x14			/* (L):DMAﾃﾞｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾚｼﾞｽﾀ1 */
#define	DMATCR1_A0		0x18			/* (L):DMAﾄﾗﾝｽﾌｧｶｳﾝﾄﾚｼﾞｽﾀ1 */
#define	CHCR1_A0		0x1C			/* (L):DMAﾁｬﾈﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ1 */
#define	SAR2_A0			0x20			/* (L):DMAｿｰｽｱﾄﾞﾚｽﾚｼﾞｽﾀ2 */
#define	DAR2_A0			0x24			/* (L):DMAﾃﾞｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾚｼﾞｽﾀ2 */
#define	DMATCR2_A0		0x28			/* (L):DMAﾄﾗﾝｽﾌｧｶｳﾝﾄﾚｼﾞｽﾀ2 */
#define	CHCR2_A0		0x2C			/* (L):DMAﾁｬﾈﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ2 */
#define	SAR3_A0			0x30			/* (L):DMAｿｰｽｱﾄﾞﾚｽﾚｼﾞｽﾀ3 */
#define	DAR3_A0			0x34			/* (L):DMAﾃﾞｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾚｼﾞｽﾀ3 */
#define	DMATCR3_A0		0x38			/* (L):DMAﾄﾗﾝｽﾌｧｶｳﾝﾄﾚｼﾞｽﾀ3 */
#define	CHCR3_A0		0x3C			/* (L):DMAﾁｬﾈﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ3 */
#define	DMAOR_A0		0x40			/* (L):DMAｵﾍﾟﾚｰｼｮﾝﾚｼﾞｽﾀ */
#define	SAR4_A0			0x50			/* (L):DMAｿｰｽｱﾄﾞﾚｽﾚｼﾞｽﾀ4 */
#define	DAR4_A0			0x54			/* (L):DMAﾃﾞｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾚｼﾞｽﾀ4 */
#define	DMATCR4_A0		0x58			/* (L):DMAﾄﾗﾝｽﾌｧｶｳﾝﾄﾚｼﾞｽﾀ4 */
#define	CHCR4_A0		0x5C			/* (L):DMAﾁｬﾈﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ4 */

#define	SAR5_A0			0x60			/* (L):DMAｿｰｽｱﾄﾞﾚｽﾚｼﾞｽﾀ4 */
#define	DAR5_A0			0x64			/* (L):DMAﾃﾞｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾚｼﾞｽﾀ4 */
#define	DMATCR5_A0		0x68			/* (L):DMAﾄﾗﾝｽﾌｧｶｳﾝﾄﾚｼﾞｽﾀ4 */
#define	CHCR5_A0		0x6C			/* (L):DMAﾁｬﾈﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ4 */
#define	SAR6_A0			0x70			/* (L):DMAｿｰｽｱﾄﾞﾚｽﾚｼﾞｽﾀ4 */
#define	DAR6_A0			0x74			/* (L):DMAﾃﾞｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾚｼﾞｽﾀ4 */
#define	DMATCR6_A0		0x78			/* (L):DMAﾄﾗﾝｽﾌｧｶｳﾝﾄﾚｼﾞｽﾀ4 */
#define	CHCR6_A0		0x7C			/* (L):DMAﾁｬﾈﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ4 */


/*******( H'FFA00080番地～のIO定義 )*******/
#define	IOBASE_A0H		0xFFA00080		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ */

#define	SAR7_A0H		0x00			/* (L):DMAｿｰｽｱﾄﾞﾚｽﾚｼﾞｽﾀ4 */
#define	DAR7_A0H		0x04			/* (L):DMAﾃﾞｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾚｼﾞｽﾀ4 */
#define	DMATCR7_A0H		0x08			/* (L):DMAﾄﾗﾝｽﾌｧｶｳﾝﾄﾚｼﾞｽﾀ4 */
#define	CHCR7_A0H		0x0C			/* (L):DMAﾁｬﾈﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ4 */


/*******( H'FFC00000番地～のIO定義 )*******/
#define	IOBASE_C0		0xFFC00000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ */

/*	( CPG ) */
#define	FRQCR_C0		0x00			/* (W):周波数制御ﾚｼﾞｽﾀ */
/*	(低消費) */
#define	STBCR_C0		0x04			/* (B):ｽﾀﾝﾊﾞｲｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ */
/*	(WDT) */
#define	WTCNT_C0		0x08			/* (B):ｳｫｯﾁﾄﾞｯｸﾞﾀｲﾏｶｳﾝﾀ(R:8,W:16) */
#define	WTCSR_C0		0x0C			/* (B):ｳｫｯﾁﾄﾞｯｸﾞﾀｲﾏｺﾝﾄﾛｰﾙ/ｽﾃｰﾀｽ(R:8,W:16) */
/*	(低消費) */
#define	STBCR2_C0		0x10			/* (B):ｽﾀﾝﾊﾞｲｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ2 */

/*	(CPG   :ロングアドレス) */
#define	DCKDR00_ADDR	0xFE0A0020		/* (L):ｸﾛｯｸ分周ﾚｼﾞｽﾀ */
#define	MCKCR00_ADDR	0xFE0A0024		/* (L):ﾓｼﾞｭｰﾙｸﾛｯｸ ｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ */

/*	(低消費:ロングアドレス) */
#define	CLKSTP00_ADDR	0xFE0A0000		/* (L): */
// #define	CLKSTPCLR00_ADDR	0xFE0A0008		/* (L):[SH7750の値] */
#define	CLKSTPCLR00_ADDR	0xFE0A0010	/* (L): */



/*******( H'FFD00000番地～のIO定義 )*******/
#define	IOBASE_D0		0xFFD00000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ */

/*	( INTC ) */
#define	ICR_D0			0x00			/* (W):割込みｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ0 */
#define	IPRA_D0			0x04			/* (W):割込み優先ﾚﾍﾞﾙ設定ﾚｼﾞｽﾀA */
#define	IPRB_D0			0x08			/* (W):割込み優先ﾚﾍﾞﾙ設定ﾚｼﾞｽﾀB */
#define	IPRC_D0			0x0C			/* (W):割込み優先ﾚﾍﾞﾙ設定ﾚｼﾞｽﾀC */
#define	IPRD_D0			0x10			/* (W):割込み優先ﾚﾍﾞﾙ設定ﾚｼﾞｽﾀD */

#define	INTPRI00_ADDR	0xFE080000		/* (L):割込優先ﾚﾍﾞﾙ設定ﾚｼﾞｽﾀ00 */
#define	INTPRI04_ADDR	0xFE080004		/* (L):割込優先ﾚﾍﾞﾙ設定ﾚｼﾞｽﾀ04 */
#define	INTPRI08_ADDR	0xFE080008		/* (L):割込優先ﾚﾍﾞﾙ設定ﾚｼﾞｽﾀ08 */
#define	INTPRI0C_ADDR	0xFE08000C		/* (L):割込優先ﾚﾍﾞﾙ設定ﾚｼﾞｽﾀ0C */
#define	INTREQ00_ADDR	0xFE080020		/* (L):割込要因ﾚｼﾞｽﾀ00 */
#define	INTREQ04_ADDR	0xFE080024		/* (L):割込要因ﾚｼﾞｽﾀ04 */
#define	INTMSK00_ADDR	0xFE080040		/* (L):割込ﾏｽｸﾚｼﾞｽﾀ00 */
#define	INTMSK04_ADDR	0xFE080044		/* (L):割込ﾏｽｸﾚｼﾞｽﾀ04 */
#define	INTMSKCLR00_ADDR	0xFE080060	/* (L):割込ﾏｽｸｸﾘｱﾚｼﾞｽﾀ00 */
#define	INTMSKCLR04_ADDR	0xFE080064	/* (L):割込ﾏｽｸｸﾘｱﾚｼﾞｽﾀ04 */


/*******( H'FFD80000番地～のIO定義 )*******/
#define	IOBASE_D8		0xFFD80000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ */

/*	( TMU ) */
#define	TOCR_D8			0x00			/* (B):ﾀｲﾏｱｳﾄﾌﾟｯﾄｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ */
#define	TSTR_D8			0x04			/* (B):ﾀｲﾏｽﾀｰﾄﾚｼﾞｽﾀ */
#define	TCOR0_D8		0x08			/* (L):ﾀｲﾏｺﾝｽﾀﾝﾄﾚｼﾞｽﾀ0 */
#define	TCNT0_D8		0x0C			/* (L):ﾀｲﾏｶｳﾝﾀ0 */
#define	TCR0_D8			0x10			/* (W):ﾀｲﾏｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ0 */
#define	TCOR1_D8		0x14			/* (L):ﾀｲﾏｺﾝｽﾀﾝﾄﾚｼﾞｽﾀ1 */
#define	TCNT1_D8		0x18			/* (L):ﾀｲﾏｶｳﾝﾀ1 */
#define	TCR1_D8			0x1C			/* (W):ﾀｲﾏｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ1 */
#define	TCOR2_D8		0x20			/* (L):ﾀｲﾏｺﾝｽﾀﾝﾄﾚｼﾞｽﾀ2 */
#define	TCNT2_D8		0x24			/* (L):ﾀｲﾏｶｳﾝﾀ2 */
#define	TCR2_D8			0x28			/* (W):ﾀｲﾏｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ2 */
#define	TCPR2_D8		0x2C			/* (L):ｲﾝﾌﾟｯﾄｷｬﾌﾟﾁｬﾚｼﾞｽﾀ2 */


/*******( H'FE60 0000番地～のIO定義 )*******/
////#define	IOBASE_160		0xFE600000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ(SCIF#0) */
////#define	IOBASE_161		0xFE610000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ(SCIF#1) */
////#define	IOBASE_162		0xFE620000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ(SCIF#2) */

#define	SCIFn_IOBASE(n)	(0xFE600000 + 0x00010000 * (n))		/* ﾍﾞｰｽ */
#define	SCIF0			0				/* n=0, 0xFE600000, SCIF#0 */
#define	SCIF1			1				/* n=1, 0xFE610000, SCIF#1 */
#define	SCIF2			2				/* n=2, 0xFE620000, SCIF#2 */

/*	( SCIF ) */
#define	SCSMR_X			0x00			/* (W):ｼﾘｱﾙﾓｰﾄﾞﾚｼﾞｽﾀ */
#define	SCBRR_X			0x04			/* (B):ﾋﾞｯﾄﾚｰﾄﾚｼﾞｽﾀ */
#define	SCSCR_X			0x08			/* (W):ｼﾘｱﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ */
#define	SCFTDR_X		0x0C			/* (B):ﾄﾗﾝｽﾐｯﾄFIFOﾃﾞｰﾀﾚｼﾞｽﾀ */
#define	SCFSR_X			0x10			/* (W):ｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀ */
#define	SCFRDR_X		0x14			/* (B):ﾚｼｰﾌﾞFIFOﾃﾞｰﾀﾚｼﾞｽﾀ */
#define	SCFCR_X			0x18			/* (W):FIFOｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ */
#define	SCTFDR_X		0x1C			/* (W):ﾄﾗﾝｽﾐｯﾄFIFOﾃﾞｰﾀ数ﾚｼﾞｽﾀ */
#define	SCRFDR_X		0x20			/* (W):ﾚｼｰﾌﾞ  FIFOﾃﾞｰﾀ数ﾚｼﾞｽﾀ */
#define	SCSPTR_X		0x24			/* (W):ｼﾘｱﾙﾎﾟｰﾄﾚｼﾞｽﾀ */
#define	SCLSR_X			0x28			/* (W):ﾗｲﾝｽﾃｰﾀｽﾚｼﾞｽﾀ */
#define	SCRER_X			0x2C			/* (W):ｼﾘｱﾙｴﾗｰｽﾚｼﾞｽﾀ */


/*******( H'FFF00000番地～のIO定義 )*******/
#define	IOBASE_F0		0xFFF00000		/* 内蔵周辺ﾓｼﾞｭｰﾙﾍﾞｰｽ */

/*	( H-UDI ) */
#define	SDIR_F0			0x00			/* (W):ｲﾝｽﾄﾗｸｼｮﾝﾚｼﾞｽﾀ */
#define	SDDR_F0			0x08			/* (L): */
#define	SDINT_F0		0x14			/* (W): */


#endif /*__MONITOR_SH_SH7760IO_H__*/


/*----------------------------------------------------------------------*/
#if 0
#|【sh7760io.h 変更履歴】
#|□2015/11/25	V1.02.04の"../arm/em1d512.h"からコピーし、
#|  [app_sh7760]用に修正。（IOPモニターの"sh7760io.ha"から修正）
#|
#endif
