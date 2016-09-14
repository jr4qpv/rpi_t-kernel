/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(c) 2015-2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2016/01/26.
 *
 *----------------------------------------------------------------------
 */

/*
 *	register.h
 *
 *       Register define header (SH-4, SH7760)
 *
 */

/*
 *[Note]
 *・本ファイルを修正したら、"register.c"も見なす事。
 */

#ifndef __MONITOR_REGISTER_H__
#define	__MONITOR_REGISTER_H__

#define	N_regStack		(32 + 1 + 34)
#define	N_regStack_FPU	(34)


/* regStack index No */
#define	ixR0		0					// R0 index
#define	ixR1		1					// R1 index
#define	ixR2		2					// R2 index
#define	ixR3		3					// R3 index
#define	ixR4		4					// R4 index
#define	ixR5		5					// R5 index
#define	ixR6		6					// R6 index
#define	ixR7		7					// R7 index

#define	ixR8		8					// R8 index
#define	ixR9		9					// R9 index
#define	ixR10		10					// R10 index
#define	ixR11		11					// R11 index
#define	ixR12		12					// R12 index
#define	ixR13		13					// R13 index
#define	ixR14		14					// R14 index
#define	ixR15		15					// R15 index

#define	ixMACH		16					// MACH index
#define	ixMACL		17					// MACL index
#define	ixGBR		18					// GBR index
#define	ixPR		19					// PR index

#define	ixSR		20					// SR index
#define	ixPC		21					// PC index

#define	ixVBR		22					// VBR index
#define	ixDBR		23					// DBR index
#define	ixSGR		24					// SGR index

#define	ixR0B		25					// R0B index
#define	ixR1B		26					// R1B index
#define	ixR2B		27					// R2B index
#define	ixR3B		28					// R3B index
#define	ixR4B		29					// R4B index
#define	ixR5B		30					// R5B index
#define	ixR6B		31					// R6B index

#define	ixCCR		32					// CCR index

#define	ixFR0		33					// FR0 index
#define	ixFR1		34					// FR1 index
#define	ixFR2		35					// FR2 index
#define	ixFR3		36					// FR3 index
#define	ixFR4		37					// FR4 index
#define	ixFR5		38					// FR5 index
#define	ixFR6		39					// FR6 index
#define	ixFR7		40					// FR7 index

#define	ixFR8		41					// FR8 index
#define	ixFR9		42					// FR9 index
#define	ixFR10		43					// FR10 index
#define	ixFR11		44					// FR11 index
#define	ixFR12		45					// FR12 index
#define	ixFR13		46					// FR13 index
#define	ixFR14		47					// FR14 index
#define	ixFR15		48					// FR15 index

#define	ixXF0		49					// XF0 index
#define	ixXF1		50					// XF1 index
#define	ixXF2		51					// XF2 index
#define	ixXF3		52					// XF3 index
#define	ixXF4		53					// XF4 index
#define	ixXF5		54					// XF5 index
#define	ixXF6		55					// XF6 index
#define	ixXF7		56					// XF7 index

#define	ixXF8		57					// XF8 index
#define	ixXF9		58					// XF9 index
#define	ixXF10		59					// XF10 index
#define	ixXF11		60					// XF11 index
#define	ixXF12		61					// XF12 index
#define	ixXF13		62					// XF13 index
#define	ixXF14		63					// XF14 index
#define	ixXF15		64					// XF15 index

#define	ixFPUL		65					// FPUL index
#define	ixFPSCR		66					// FPSCR index


#define	ixSP		ixR15				// SP index
#define	ixMDR		ixR2B				// MDR index



#endif /* __MONITOR_REGISTER_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|【register.h 変更履歴】
#|□2015/12/14	[app_sh7760]用に、新規作成。
#|□2016/01/26	ixCCR定義の追加。
#|
#endif
