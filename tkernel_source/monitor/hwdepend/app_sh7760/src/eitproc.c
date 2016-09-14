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
 *    Modified by T.Yokobayashi at 2015/12/09.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)eitproc.c () 2016/12/09
 *
 *       EIT processing
 */

/*
 *[Note]
 *・
 */

#include "sysdepend.h"
#include <tk/sysdef.h>

/*
 * vector information
 */
typedef struct vecinfo	VECINFO;
struct vecinfo {
	UW	vec;			/* initial vector numer */
	B	*msg;			/* message */

	/* processing function
	 *           return value  1 : adjust PC by decrementing it by one instruction worth
	 *                         0 : PC needs no adjustment
	 */
	W (*func)( const VECINFO*, UW vec, UW pc, UW cpsr );
};


/* display message */
LOCAL W vf_msg( const VECINFO *vi, UW vec, UW pc, UW cpsr )
{
	B	*msg = vi->msg;
	B	opt;

	if ( msg == NULL ) return 0;

        /* if the first byte of the message is not a letter, treat it as option.
         *       opt = 0 - 037 (the code prior to ' ' )
         *       \020    PC is adjusted to the previous instruction's address
	 */
	opt = 0;
	if ( *msg < ' ' ) opt = *msg++;

	DSP_F5(S,"Exception ", D,vec, S," (", S,msg, CH,')');

	return ( opt & 020 )? 1: 0;
}


#if 0	///////////////////
/* undefined instruction */
LOCAL W vf_undef( const VECINFO *vi, UW vec, UW pc, UW cpsr )
{
	if (cpsr & PSR_T) {
		DSP_F3(S,vi->msg, CH,' ', 04X,*((UH*)(pc - 2)));
	} else {
		DSP_F3(S,vi->msg, CH,' ', 08X,*((UW*)(pc - 4)));
	}
	return 1;
}


/* data abort */
LOCAL W vf_dabort( const VECINFO *vi, UW vec, UW pc, UW cpsr )
{
	DSP_F1(S,vi->msg);
	DSP_F4(S," ADDR: ", 08X,getCP15(6, 0), S," STAT: ", 08X,getCP15(5, 0));
	return 0;
}
#endif	//////////////////


/*
 * vector information table
 *       this has to be filled in the ascending order of the vector number
 */
LOCAL const VECINFO VecInfoTable[] = {
  { EIT_RESET,      "Power on Reset",		vf_msg		},
  { EIT_MRESET,     "Manual Reset",			vf_msg		},
  { EIT_TLBMISS_R,	"TLB invalid(Read)",	vf_msg		},
  { EIT_TLBMISS_W,	"TLB invalid(Write)",	vf_msg		},
  { EIT_INIPAGE_W,	"Initial page write",	vf_msg		},
  { EIT_TLBPROT_R,  "TLB protection(Read)",	vf_msg		},
  { EIT_TLBPROT_W,  "TLB protection(Write)", vf_msg		},
  { EIT_CPUADER_R,	"CPU addr error(Read)",	vf_msg		},
  { EIT_CPUADER_W,	"CPU addr error(Write)", vf_msg		},
  { EIT_FPUEXP,		"FPU exception",		vf_msg		},
  { EIT_TLBHIT,		"TLB multiple hit",		vf_msg		},
  { EIT_TRAPA,		"TRAPA #0x0b Exception", vf_msg		},
  { EIT_INSTILL,	"Instcuction invalid",	vf_msg		},
  { EIT_SLTILL,		"Slot invalid",			vf_msg		},
  { EIT_NMI,		"NMI Exception",		vf_msg		},
  { EIT_USRBREAK,	"User Break Exception",	vf_msg		},

  { EIT_USRBREAK+1,	"Undefined Exception",	vf_msg		},

///  { 0,            "\020" "Undefined SWI",	vf_msg		},
///  { EIT_UNDEF,    "Undefined Instruction",	vf_undef	},
///  { EIT_IABORT,   "Prefetch Abort",		vf_msg		},
///  { EIT_DABORT,   "Data Abort",			vf_dabort	},
///  { EIT_DABORT+1, "\020" "Undefined SWI",	vf_msg		},

///  { EIT_FIQ,               "Undefined FIQ",		vf_msg  },
///  { EIT_IRQ(0),            "Undefined IRQ",		vf_msg  },
///  { EIT_GPIO(0),           "Undefined GPIO-INT",	vf_msg  },
///  { EIT_GPIO(127)+1,"\020" "Undefined SWI",		vf_msg  },

  { N_INTVEC+32, NULL, vf_msg }	/* terminating mark (the last vector number + 1) */
};
#define	N_VECINFO	( sizeof(VecInfoTable) / sizeof(VECINFO) )


/*
 * EIT processing
 *       *  return value   0 : monitor should keep on running
 *                         1 : return from the interrupt handler
 */
EXPORT W procEIT( UW vec )
{
	const VECINFO	*vp;
	UW	pc, sr;
	W	i;

	pc = getCurPC();
	sr = getCurSR();

	/* machine-dependent interrupt processing */
	i = procHwInt(vec);					/* see "config.c" */
	if ( i == 2 ) return 1; /* exit from the interrupt handler immediately */

	if ( i == 0 ) {
		/* other EIT processing */
		for ( i = 1; i < N_VECINFO; ++i ) {
			if ( vec < VecInfoTable[i].vec ) break;
		}
		vp = &VecInfoTable[i-1];
		i = (*vp->func)(vp, vec, pc, sr);
#if 0	/////// kari ///////
		if ( i > 0 ) {
			/* PC is adjusted to the previous instruction's address */
			pc -= ( (sr & PSR_T) != 0 )? 2: 4;
		}
#endif	////////////////////
	}

	DSP_F5(S,"\nPC: ", 08X,pc, S," SR: ", 08X,sr, CH,'\n');

	return 0;
}


/*----------------------------------------------------------------------*/
#if 0
#|【eitproc.c 変更履歴】
#|□2015/12/09	[app_sh7760]用に、[tef_em1d]の"eitproc.c"を参考に作成。
#|
#endif
