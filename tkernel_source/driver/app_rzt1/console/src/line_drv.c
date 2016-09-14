/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2014/09/10.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/02/18.
 *
 *----------------------------------------------------------------------
 */

/*
 *	line_drv.c	Console/Low-level serial I/O driver
 *
 *	Serial line low-level driver common part  : system-independent
 */

#include "line_drv.h"
#include "svc/ifserialio.h"
#include <tm/tmonitor.h>

/* Register/Deregister subsystem */
IMPORT	ER	con_def_subsys(W svc, W pri, void *svcent, void *brkent);

/* Port definition (system-dependent)*/
#include "portdef.h"

/* シリアル回線管理情報 */
EXPORT	LINE_INFO	*LineInfo;
EXPORT	W		nPorts = N_PORTS;

/*
 *	シリアル入力
 */
LOCAL ER	_serial_in(SERIAL_SERIAL_IN_PARA *p)
{
	/* パラメータチェック */
	if (p->port < 0 || p->port >= nPorts) return E_PAR;

	return (*(LineInfo[p->port].scdefs.fn->in))(
		&LineInfo[p->port], (UB*)p->buf, p->len, p->alen,
		(p->tmout < 0) ? TMO_FEVR : p->tmout);
}

/*
 *	シリアルポート出力
 */
LOCAL ER	_serial_out(SERIAL_SERIAL_OUT_PARA *p)
{
	/* パラメータチェック */
	if (p->port < 0 || p->port >= nPorts) return E_PAR;
	if (p->len <= 0) return E_OK;

	return (*(LineInfo[p->port].scdefs.fn->out))(
		&LineInfo[p->port], (UB*)p->buf, p->len, p->alen,
		(p->tmout < 0) ? TMO_FEVR : p->tmout);
}

/*
 * シリアルポート制御
 */
LOCAL ER	_serial_ctl(SERIAL_SERIAL_CTL_PARA *p)
{
	/* パラメータチェック */
	if (p->port < 0 || p->port >= nPorts) return E_PAR;

	return (*(LineInfo[p->port].scdefs.fn->ctl))
		(&LineInfo[p->port], p->kind, p->arg);
}

/*
 * シリアルI/O 拡張SVCエントリ
 */
LOCAL ER	serial_io_entry(void *para, W fn)
{
	switch(fn) {
	case SERIAL_SERIAL_IN_FN:	return _serial_in(para);
	case SERIAL_SERIAL_OUT_FN:	return _serial_out(para);
	case SERIAL_SERIAL_CTL_FN:	return _serial_ctl(para);
	}
	return E_RSFN;
}

/*
 * シリアルI/O ブレーク関数
 */
LOCAL void	serial_io_break(ID tskid)
{
	tk_dis_wai(tskid, TTW_FLG);
}

/*
 * シリアルインタフェースドライバの起動
 */
EXPORT ER	startup_serial_io(BOOL StartUp)
{
	LINE_INFO	*li;
	W		port, baud;
static	union objname	name = {{ "sio0" }};

	/* 通信速度を tmonitor から取り出して設定 */
	if ((baud = tm_extsvc(0x00, 0, 0, 0)) < 19200) baud = 0;

	/* Initialize the auxiliary port */
	INIT_AUXPORT(StartUp);

	if (!StartUp) {
		/* Deregister the subsystem */
		con_def_subsys(SERIAL_SVC, SERIAL_PRI, NULL, NULL);

		/* Stop the auxiliary port */
		START_AUXPORT(FALSE);

		/* Stop the all serial ports */
		for (port = 0; port < nPorts; port++) {
			li = &LineInfo[port];
			li->scdefs = PortDefs[port];

			/* Stop the serial port */
			(*(li->scdefs.fn->down))(li);

			/* Release the receive buffer */
			if (li->in_buf) Free(li->in_buf);

			/* Delete the event flag for notification of
						the lock and the interrupt */
			if (li->flg > 0) consDeleteMLock(&li->lock);
		}

		/* Release the serial line management information */
		if (LineInfo) Free(LineInfo);

		return E_OK;
	}

	/* Start up the auxiliary port */
	START_AUXPORT(TRUE);

	/* シリアル回線管理情報の初期化 */
	LineInfo = (LINE_INFO *)Malloc(sizeof(LINE_INFO) * nPorts);
	if (! LineInfo) {nPorts = 0; return E_NOMEM;}
	MEMSET(LineInfo, 0, sizeof(LINE_INFO) * nPorts);

	/* シリアルポート起動 */
	for (port = 0; port < nPorts; port++) {
		li = &LineInfo[port];
		li->scdefs = PortDefs[port];

		/* Change baud by that from t-monitor */
		if (baud != 0) li->scdefs.mode.baud = baud;

		name.s[3] = '0' + port;

		/* 受信バッファの確保 */
		li->in_buf = Malloc(li->in_bufsz = DEF_INBUFSZ);
		if (li->in_buf == NULL) return E_NOMEM;

		/* ロックおよび割り込み通知用イベントフラグの生成 */
		li->flg = consCreateMLock(&li->lock, name.s);

		/* RTS/CTSフロー制御: on */
		li->flow.csflow = 1;
		li->flow.rsflow = 1;

		/* 各種ハードウェア設定 */
		(*(li->scdefs.fn->up))(li);
	}

	/* Register the subsystem */
	return con_def_subsys(SERIAL_SVC, SERIAL_PRI,
				&serial_io_entry, &serial_io_break);
}


/*----------------------------------------------------------------------
#|History of "line_drv.c"
#|=======================
#|* 2016/07/08	[app_rzt1]用に、[tef_em1d]用の"line_drv.c"を参考に作成。
#|
*/
