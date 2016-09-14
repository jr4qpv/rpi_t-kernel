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
 *    Modified by T.Yokobayashi at 2015/12/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sio.c () 2015/12/04
 *
 *       serial port I/O
 */

/*
 *[Note]
 *・SIOCBの定義は、/monitor/include/device.hに記述がある。
 *・CFGSIOの定義は、/monitor/include/device.hに記述があり、
 *  config.cで実態は定義。そこに、初期化処理の関数が登録してあり、
 *  間接コールされる。
 */

#include "hwdepend.h"

EXPORT	W	ConPort;	/* console port number */
EXPORT	UW	ConPortBps;	/* console port commnication speed (bps) */

LOCAL	SIOCB	SIO;		/* serial port control block */

/*
 * initialize serial port
 *       port    console port number (0 - )
 *              when it is -1, it means there is no console.
 *       speed   communication speed (bps)
 */
EXPORT ER initSIO( W port, W speed )
{
	const CFGSIO	*cp;
	ER	err;

	if ( port >= N_ConfigSIO ) port = 0; /* invalid value is turned into a default value. */

	memset(&SIO, 0, sizeof(SIO));
	ConPort    = port;
	ConPortBps = speed;

	if ( port < 0 ) return E_OK; /* no console */

	/* initialize hardware */
	cp = &ConfigSIO[port];					// "config.c"で定義してある
	err = (*cp->initsio)(&SIO, cp, speed);	// 初期化関数を間接コール
	if ( err < E_OK ) goto err_ret;

	return E_OK;

err_ret:
	/* if there was an error, treat it as no console */
	memset(&SIO, 0, sizeof(SIO));
	ConPort = -1;
	return err;
}

/*
 * serial port I/O
 */
EXPORT void putSIO( UB c )
{
	if ( SIO.put != NULL ) (*SIO.put)(&SIO, c);
}

/*
 * serial port input (with buffering)
 *       tmo     timeout (milliseconds)
 *              You can not wait forever.
 *       return value       >= 0 : character code
 *                 -1 : timeout
 *       receive error is ignored
 */
EXPORT W getSIO( W tmo )
{
	return ( SIO.get != NULL )? (*SIO.get)(&SIO, tmo): -1;
}


/*----------------------------------------------------------------------*/
#if 0
#|【sio.c 変更履歴】
#|□2015/12/04	[app_sh7760]用に、一部コメントを追記。
#|
#endif
