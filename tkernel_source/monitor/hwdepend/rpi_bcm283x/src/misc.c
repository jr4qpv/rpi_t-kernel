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
 *    Modified by T.Yokobayashi at 2016/08/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)misc.c (BCM283x) 2016/08/29
 *
 */

#include "sysdepend.h"

/*
 * obtain switch status
 */
EXPORT UW getDipSw( void )
{
	return DipSw;
}

/*
 * Obtain boot selection information
 */
EXPORT W bootSelect( void )
{
	if ( (DipSw & (SW_MON|SW_ABT)) != 0 ) return BS_MONITOR;

	return BS_AUTO;
}

/*
 * obtain the console port number
 */
EXPORT W getConPort( void )
{
	return ConPort_Number;
}


/*----------------------------------------------------------------------
#|History of "misc.c"
#|======================
#|* 2016/03/10	It's copied from "../tef_em1d/" and it's modified.
#|* 2016/08/29	getConPort()の戻値は setup_bcm283x.h で指定の値を返す
#|
*/
