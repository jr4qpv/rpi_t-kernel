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
 *    Modified by T.Yokobayashi at 2016/02/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *	help.h
 *
 *       help message definitions
 */

/*
 * help message definitions
 */
typedef struct help	HELP;
struct help {
	void (*prfn)( const HELP* );	/* display function */
	const UB	*msg;		/* message */
};

/*
 * display help message
 */
IMPORT void printHelp( const HELP *help );

/*
 * help message data
 */
IMPORT	const HELP	helpALL;
IMPORT	const HELP	helpD, helpDB, helpDH, helpDW;
IMPORT	const HELP	helpM, helpMB, helpMH, helpMW;
IMPORT	const HELP	helpF, helpFB, helpFH, helpFW;
IMPORT	const HELP	helpSC, helpSCB, helpSCH, helpSCW;
IMPORT	const HELP	helpCMP, helpMOV;
IMPORT	const HELP	helpIB, helpIH, helpIW;
IMPORT	const HELP	helpOB, helpOH, helpOW;
IMPORT	const HELP	helpDA, helpR, helpB, helpBC;
IMPORT	const HELP	helpG, helpS, helpN, helpBTR, helpLO;
IMPORT	const HELP	helpRD, helpWD, helpID, helpBD;
IMPORT	const HELP	helpKILL, helpWROM, helpFLLO;
IMPORT	const HELP	helpH, helpEX;
IMPORT	const HELP	helpSYSI;


/*----------------------------------------------------------------------*/
#if 0
#|【help.h 改版履歴】
#|□2015/12/17	[app_sh7760]用に、[tef_em1d]の"help.h"から作成。
#|□2016/02/03	｢helpSYSI｣の定義に対応。
#|
#endif
