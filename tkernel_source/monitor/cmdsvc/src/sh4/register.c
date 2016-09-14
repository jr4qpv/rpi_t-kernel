/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/03/04.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/01/26.
 *
 *----------------------------------------------------------------------
 */

/*
 *	register.c
 *
 *       Register-related operations (SH-4, SH7760)
 */

/*
 *[Note]
 *・本ファイルを修正したら、"register.h"も見なす事。
 */

#include "../cmdsvc.h"
#include <sys/sysinfo.h>

#include "register.h"


EXPORT	UW	regStack[N_regStack];

/*
        register definition table

        * registers are saved to regStack on entry to the monitor.
           The value in register ID follows the order saved in register stack (below).
           (See eitent.S)
*/
// save register
//      regStack[0-15]          r0 .. r15
//              [16-19]         mach, macl, gbr, pr
//              [20,21]         Entry sr, pc
//              [22-24]         vbr, dbr, sgr
//              [25-31]         r0b .. r6b
//              [32]            ccr
//              [33-48]         fr0 .. fr15
//              [49-64]         xf0 .. xf15
//              [65,66]         fpul, fpscr

#define	L_REGNM		8
typedef	struct {
	UB	name[L_REGNM];					// register name
	UW	id;								// register ID
} REGTAB;

#define	R_GEN		0x001000			// general register
#define	R_CTL		0x002000			// control register
#define	R_FPU		0x004000			// fpu register
#define	R_GRP		0x010000			// register group

#define	R_LF		0x080000			// forced linefeed
#define	R_GAP		0x040000			// empty line

#define	R_ONLY		0x100				// disable setup
#define	SPEC(n)		(0x200 | (n))		// special



#define	N_ACTREGS	(N_regStack)
#define	N_REGS		(N_ACTREGS + 4)

LOCAL	const	REGTAB	regTab[N_REGS] = {
	{"R0      ",	R_GEN + ixR0					},	// 0
	{"R1      ",	R_GEN + ixR1					},	// 1
	{"R2      ",	R_GEN + ixR2					},	// 2
	{"R3      ",	R_GEN + ixR3 + R_LF				},	// 3
	{"R4      ",	R_GEN + ixR4					},	// 4
	{"R5      ",	R_GEN + ixR5					},	// 5
	{"R6      ",	R_GEN + ixR6					},	// 6
	{"R7      ",	R_GEN + ixR7 + R_LF				},	// 7
	{"R8      ",	R_GEN + ixR8					},	// 8
	{"R9      ",	R_GEN + ixR9					},	// 9
	{"R10     ",	R_GEN + ixR10					},	// 10
	{"R11     ",	R_GEN + ixR11 + R_LF			},	// 11
	{"R12     ",	R_GEN + ixR12					},	// 12
	{"R13     ",	R_GEN + ixR13					},	// 13
	{"R14     ",	R_GEN + ixR14					},	// 14
	{"R15/SP  ",	R_GEN + ixR15 + R_LF			},	// 15

	{"MACH    ",	R_GEN + ixMACH + R_GAP			},	// 16
	{"MACL    ",	R_GEN + ixMACL 					},	// 17
	{"GBR     ",	R_GEN + ixGBR 					},	// 18
	{"PR      ",	R_GEN + ixPR + R_LF				},	// 19

	{"SR      ",	R_GEN + ixSR					},	// 20
	{"PC      ",	R_GEN + ixPC + R_LF				},	// 21

	{"VBR     ",	R_CTL + ixVBR + R_ONLY + R_GAP	},	// 22
	{"DBR     ",	R_CTL + ixDBR					},	// 23
	{"SGR     ",	R_CTL + ixSGR + R_ONLY + R_LF	},	// 24

	{"R0B     ",	R_CTL + ixR0B 					},	// 25
	{"R1B     ",	R_CTL + ixR1B					},	// 26
	{"R2B/MDR ",	R_CTL + ixR2B					},	// 27
	{"R3B/SST ",	R_CTL + ixR3B + R_LF			},	// 28
	{"R4B     ",	R_CTL + ixR4B					},	// 29
	{"R5B     ",	R_CTL + ixR5B					},	// 30
	{"R6B/ISP ",	R_CTL + ixR6B + R_ONLY + R_LF	},	// 31

	{"CCR     ",	R_CTL + SPEC(0x00) + R_LF 		},	// 32

	{"FR0     ",	R_FPU + ixFR0 + R_GAP			},	// 33
	{"FR1     ",	R_FPU + ixFR1					},	// 34
	{"FR2     ",	R_FPU + ixFR2					},	// 35
	{"FR3     ",	R_FPU + ixFR3 + R_LF			},	// 36
	{"FR4     ",	R_FPU + ixFR4					},	// 37
	{"FR5     ",	R_FPU + ixFR5					},	// 38
	{"FR6     ",	R_FPU + ixFR6					},	// 39
	{"FR7     ",	R_FPU + ixFR7 + R_LF			},	// 40
	{"FR8     ",	R_FPU + ixFR8					},	// 41
	{"FR9     ",	R_FPU + ixFR9					},	// 42
	{"FR10    ",	R_FPU + ixFR10					},	// 43
	{"FR11    ",	R_FPU + ixFR11 + R_LF			},	// 44
	{"FR12    ",	R_FPU + ixFR12					},	// 45
	{"FR13    ",	R_FPU + ixFR13					},	// 46
	{"FR14    ",	R_FPU + ixFR14					},	// 47
	{"FR15    ",	R_FPU + ixFR15 + R_LF			},	// 48

	{"XF0     ",	R_FPU + ixXF0 + R_GAP			},	// 49
	{"XF1     ",	R_FPU + ixXF1					},	// 50
	{"XF2     ",	R_FPU + ixXF2					},	// 51
	{"XF3     ",	R_FPU + ixXF3 + R_LF			},	// 52
	{"XF4     ",	R_FPU + ixXF4					},	// 53
	{"XF5     ",	R_FPU + ixXF5					},	// 54
	{"XF6     ",	R_FPU + ixXF6					},	// 55
	{"XF7     ",	R_FPU + ixXF7 + R_LF			},	// 56
	{"XF8     ",	R_FPU + ixXF8					},	// 57
	{"XF9     ",	R_FPU + ixXF9					},	// 58
	{"XF10    ",	R_FPU + ixXF10					},	// 59
	{"XF11    ",	R_FPU + ixXF11 + R_LF			},	// 60
	{"XF12    ",	R_FPU + ixXF12					},	// 61
	{"XF13    ",	R_FPU + ixXF13					},	// 62
	{"XF14    ",	R_FPU + ixXF14					},	// 63
	{"XF15    ",	R_FPU + ixXF15 + R_LF			},	// 64

	{"FPUL    ",	R_FPU + ixFPUL					},	// 65
	{"FPSCR   ",	R_FPU + ixFPSCR + R_LF			},	// 66

	{"G       ",	R_GRP|R_GEN						},
	{"C       ",	R_GRP|R_CTL						},
	{"F       ",	R_GRP|R_FPU						},
	{"A       ",	R_GRP|R_GEN|R_CTL|R_FPU			},
};


/*
        Searching register name
*/
EXPORT	W	searchRegister(UB *name, W grp)
{
	W	i, n, a;
	UB	bf[L_REGNM];
	REGTAB	*p;

	if (name[L_REGNM] != ' ') return -1;

	for (p = (REGTAB*)regTab, i = 0; i < N_REGS; p++, i++) {
		for (n = 0; n < L_REGNM; n++) if (p->name[n] == '/') break;
		if (n == L_REGNM) {	// no separator '/' -> a single register name
			if (memcmp(name, p->name, L_REGNM)) continue;
		} else {		// has alias
                        // check the name(s) after the separator
			memset(bf, ' ', sizeof(bf));
			memcpy(bf, p->name + (n + 1), L_REGNM - (n + 1));
			a = memcmp(name, bf, L_REGNM - n);

                        // check the name before the separator
			memset(bf, ' ', sizeof(bf));
			memcpy(bf, p->name, n);
			if (a && memcmp(name, bf, n + 1)) continue;
		}
		if (grp == 0 && (p->id & R_GRP)) break;
		return i;
	}
	return -1;
}


/*
        obtain register value
*/
EXPORT	UW	getRegister(W regno)
{
	W	i;

	i = regTab[regno].id & (R_GRP | 0x3ff);

	// normal register
	if (i < SPEC(0)) return regStack[i & 0xff];

	// special register
	switch(i) {
	case SPEC(0x00):	// CCR
		return regStack[ixCCR];
	}

	// retur 0 on error
	return 0;
}


/*
        Set register value
*/
EXPORT	ER	setRegister(W regno, UW val)
{
	W	i;

	i = regTab[regno].id & (R_GRP | 0x3ff);
	if (i & R_ONLY) return E_RONLY;		// cannot be set

	if (i < SPEC(0)) {	// normal register
		regStack[i & 0xff] = val;
		return 0;
	}

	// special register
	switch(i) {
	case SPEC(0x00):	// CCR
		regStack[ixCCR] = val;
///		regStack[ixCP15R1] &= MASK_CACHEMMU;
///		regStack[ixCP15R1] |= val & VALID_CACHEMMU;
		break;
	default:
		return E_PAR;
	}

	return 0;
}


/*
        List the values of register (group)
                regno < 0 : default group (not specified)
*/
EXPORT	void	dispRegister(W regno)
{
	W	i, j, n, id, rid;

	if (regno >= N_REGS) return;

	id = (regno < 0) ? (R_GRP | R_GEN) : regTab[regno].id;

	for (n = i = 0; i < N_ACTREGS; i++) {
		rid = regTab[i].id;
		if (!(i == regno || ((id & R_GRP) && (rid & id)))) continue;
		if (n != 0 && (rid & R_GAP)) DSP_LF;
		if (n++ & 0x0f) DSP_S("  ");
		for (j = 0; j < L_REGNM; j++) DSP_CH(regTab[i].name[j]);
		DSP_F2(S,": ", 08X,getRegister(i));
		if (rid & R_LF) {DSP_LF; n = 0x10;}
		if ((id & R_GRP) == 0) break;
	}
	if (n & 0x0f) DSP_LF;
}


/*
        obtain SR register value
*/
EXPORT	UW	getCurSR(void)
{
	return regStack[ixSR];
}

/*
        obtain PC register value
*/
EXPORT	UW	getCurPC(void)
{
	return regStack[ixPC];
}

/*
        obtain CCR register value
*/
EXPORT	UW	getCCR(void)
{
#if 1
	return regStack[ixCCR];
#else
	W	i, d;
	LOCAL	const UH reg_op[] = {
		0x1000, 0x2000, 0x2001, 0x2002, 0x3000, 0x5000, 0x5001, 0x6000,
		0x6002, 0xd001,
	};

	d = ((reg & 0x0f) << 12) | (opcd & 0x0fff);

	for (i = 0; i < sizeof(reg_op) / sizeof(UH); i++) {
		if (reg_op[i] == d) return regStack[ixCP15 + i];
	}
	return 0;
#endif
}


/*
        Set PC register value
*/
EXPORT	void	setCurPC(UW val)
{
	regStack[ixPC] = val & ~0x1;
}


#if 0	///////////////////////// 追加したら"cpudep.h"記述必要
EXPORT	void	setCurPCX(UW val)
{
#if 1
        // Thumb Bit is not changed.
	regStack[ixPC] = val & ~0x1;
#else
	if (regStack[ixPC] != val) {
                // Thumb Bit is changed according to the LSB value of PC.
		if (val & 0x3)	regStack[ixCPSR] |= PSR_T;
		else		regStack[ixCPSR] &= ~PSR_T;
		regStack[ixPC] = val & ~0x1;
	}
#endif
}
#endif	////////////////////////


/*
        Set registers for BOOT
*/
EXPORT	void	setUpBoot( void *start, BootInfo *bootinfo )
{
	bootFlag = 1; /* suppress the setting register R0 upon exit of the monitor */

	regStack[ixSR]	= SR_MD | SR_FD | SR_I(15);
	regStack[ixR4]	= (UW)bootinfo;			// R0 boot parameter
	regStack[ixPC]	= (UW)start;			// PC start address
	regStack[ixSP]	= (UW)&__stack_bottom;	// SP monitor stack

	// Cache / Write Buffer not enabled
	regStack[ixCCR] = 0;					// disable cache
#if 0
	// MMU enabled, Cache / Write Buffer not enabled
///	regStack[ixCP15R1] &= MASK_CACHEMMU;
///	regStack[ixCP15R1] |= ENB_MMUONLY;
#endif

	// system initialization processing
	resetSystem(1);
}


/*
        Check whether we can use KILL command
*/
EXPORT	W	isKillValid(void)
{
	// Has TRAP for KILL been define?
	if ( SCArea->intvec[TRAP_KILLPROC] == NULL ) return -1;
	return 0;
}



#if REF_TKOBJECT
/*
        Check whether T-Kernel/DS functions can be executed?
*/
EXPORT	W	isTKDSValid(void)
{
	// Has TRAP for T-Kernel/DS been defined?
	if ( SCArea->intvec[EIT_USRBREAK] == NULL ) return -1;
	return 0;
}


/*
        Display register of tasks
*/
EXPORT W PrintTaskRegister( int (*prfn)( const char *format, ... ),
				T_REGS *gr, T_EIT *er, T_CREGS *cr )
{
/*
 *	PC: 12345678 SR: 12345678 MDR:12345678
 *	R0: 12345678 R1: 12345678 R2: 12345678 R3: 12345678
 *	R4: 12345678 R5: 12345678 R6: 12345678 R7: 12345678
 *	R8: 12345678 R9: 12345678 R10:12345678 R11:12345678
 *	R12:12345678 R13:12345678 R14:12345678
 *	MAC:12345678 12345678     GBR:12345678 PR: 12345678
 *	USP:12345678 SSP:12345678 LSID:1234   UATB:12345678
 *	R0B:12345678 R1B:12345678              R3B:12345678
 */
	(*prfn)("PC: %08x SR: %08x MDR:%08x\n",
		(UW)er->pc, er->cpsr, er->taskmode);

	(*prfn)("R0: %08x R1: %08x R2: %08x R3: %08x\n",
		gr->r[0], gr->r[1], gr->r[2], gr->r[3]);
	(*prfn)("R4: %08x R5: %08x R6: %08x R7: %08x\n",
		gr->r[4], gr->r[5], gr->r[6], gr->r[7]);
	(*prfn)("R8: %08x R9: %08x R10:%08x R11:%08x\n",
		gr->r[8], gr->r[9], gr->r[10], gr->r[11]);
	(*prfn)("R12:%08x R13:%08x R14:%08x\n",
		gr->r[12], gr->r[13], gr->r[14]);
	(*prfn)("MAC:%08x %08x     GBR:%08x PR: %08x\n",
		gr->mach, gr->macl, gr->gbr, gr->pr);

	(*prfn)("USP:%08x SSP:%08x LSID:%-4d   UATB:%08x\n",
		(UW)cr->usp, (UW)cr->ssp, cr->lsid, (UW)cr->uatb);
	(*prfn)("R0B:%08x R1B:%08x             R3B:%08x\n",
		(UW)cr->r0, (UW)cr->r1, (UW)cr->sstop);
	return 8;  /* number of display lines */
}
#endif	/* REF_TKOBJECT */


/*----------------------------------------------------------------------*/
#if 0
#|【register.c 変更履歴】
#|□2015/12/12	[app_sh7760]用に、新規作成。
#|  [tef_em1d]用の"../armv6/register.c"を参考に作成。
#|□2016/01/26	regTabに｢CCR｣の定義を追加。
#|
#endif
