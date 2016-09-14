/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sh_scif.c (driver) 2016/02/19
 *
 *       シリアル回線低レベルドライバ (SH7760 SCIF 用)
 */

#include "line_drv.h"
#include <tk/syslib.h>
#include <tk/sysdef.h>
#include <sys/sysinfo.h>
//#include <tm/tmonitor.h>		/////// for debug ////////
////#include <misc/sh7760.h>
#include <misc/libmisc.h>

////#define	REMOTE_MODE		0		/* 0 or 1,TERMのﾘﾓｰﾄ通信機能を使う(RS232C側) */


/*
	シリアルコントローラ操作関数群の定義
*/
LOCAL	ER	scif_in(LINE_INFO *li, UB *buf, W len, W *alen, W tmout);
LOCAL	ER	scif_out(LINE_INFO *li, UB *buf, W len, W *alen, W tmout);
LOCAL	ER	scif_ctl(LINE_INFO *li, W kind, UW *arg);
LOCAL	void	scif_up(LINE_INFO *li);
LOCAL	void	scif_down(LINE_INFO *li);

EXPORT SC_FUNC	ScFuncSCIF = {
	scif_in, scif_out, scif_ctl, scif_up, scif_down
};

/*
 *	Define the serial controller (SH7760 "SCIF")
 */
#include "sh_scif.h"			/* Common definition */
#include "sh_scifsys.h"			/* System-dependent definition */



/*
	RTS/CTS 制御の ON/OFF (FIFOトリガの設定も同時に行っている)
*/
LOCAL	void	rscs_ctrl(LINE_INFO *li)
{
	SC_DEFS	*scdefs = &li->scdefs;

	if (li->flow.csflow) {
		OutH(SCFCRx, SCFCRx_RTRG_1 | SCFCRx_TTRG_64 | SCFCRx_MCE);
	} else {
		OutH(SCFCRx, SCFCRx_RTRG_1 | SCFCRx_TTRG_64);
	}
}

/*
	ブレーク信号制御
*/
LOCAL	void	break_ctrl(LINE_INFO *li, W bk)
{
	SC_DEFS	*scdefs = &li->scdefs;

	if (bk) {		// break on
		OutH(SCSCRx, InH(SCSCRx) & ~(SCSCRx_TIE | SCSCRx_TE));// TE off
		OutH(SCSPTRx, (InH(SCSPTRx) & 0xfffc) | 0x0002);	// SPB2IO=1,SPB2DT=0
	} else {		// break off
		OutH(SCSPTRx, InH(SCSPTRx) & 0xfffc);				// SPB2IO=0
		OutH(SCSCRx, InH(SCSCRx) | SCSCRx_TIE | SCSCRx_TE); // TE on
	}
}

/*
	ビットレートレジスタ 設定値計算
*/
LOCAL	void	calc_brr(W baud, W *brr, W *cks)
{
	W	pclk = (W)SCInfo.Pck * 10000;	// SCInfo.Pck:[1/100Mhz]
	W	n, s, f;

	n = 0;
	for (s = 0; s < 4; s++) {
		f = pclk / (1 << (2 * s));		// クロックソース設定
		n = f / (16 * baud);
		n = (n + 1) / 2 - 1;			// 四捨五入してから1を引く
		if (n < 256) break;				// n は 0～255の範囲にあること
	}

	*brr = n;
	*cks = s & 3;
}

/*
	シリアルコントローラの初期化 (モード設定)
*/
LOCAL	 ER	init_sio(LINE_INFO *li, RsMode mode)
{
	SC_DEFS	*scdefs = &li->scdefs;
	UW	imask;
	W	smr, brr, cks;

	// パラメータチェック
	if (mode.parity == 3 || mode.datalen < 2 || mode.stopbits & 1 ||
	    mode.baud < 110 || mode.baud > 115200) return E_PAR;

	// 通信速度設定
	calc_brr(mode.baud, &brr, &cks);

	// 回線モード
	smr = 0;
	if (mode.datalen == 2) smr |= SCSMRx_CHR; // mode.datlen = 2 or 3
	if (mode.stopbits) smr |= SCSMRx_STOP;	// mode.stopbits = 0 or 2
	switch(mode.parity) {
	case 1:	smr |= SCSMRx_PE | SCSMRx_OE;	break;	// odd
	case 2:	smr |= SCSMRx_PE;		break;	// even
	default:	break;						// none
	}

	DI(imask);

#ifdef REMOTE_MODE
	// ﾒﾝﾃﾎﾟｰﾄ remote処理
	_remote_init();
#endif

	/*--- SCIFの初期化 ---*/
	OutH(SCSCRx, 0);					// 割込・送受信禁止

//	OutH(SCSPTRx, InH(SCSPTRx) & 0xef);		// SCPT4 - low level
	OutH(SCSPTRx, InH(SCSPTRx) & 0xfffc);	// 端子を TxD/RxDに

	OutH(SCFCRx, SCFCRx_TFRST | SCFCRx_RFRST); // 送受信FIFOクリア
	OutH(SCSMRx, smr | cks);			// 通信ﾌｫｰﾏｯﾄ,ｸﾛｯｸｿｰｽ設定
	OutB(SCBRRx, brr);					// ビットレート設定
	WaitUsec(15000000 / mode.baud);		// 1bit分確実に待たせる
	rscs_ctrl(li);						// SCFCR設定(FIFO/RSCSフロー設定)
	OutH(SCFSRx, InH(SCFSRx) & 0x0000);	// 割込ステータスクリア
	OutH(SCSCRx, SCSCRx_RIE|SCSCRx_TE|SCSCRx_RE); // 割込・送受信許可

#if 0	//////// for debug ////////////////
	OutH(0xfe600008, 0x30);	/* SCSCR */
	tm_printf("<<< init_sio() debug, brr=%04X, smr=%04X, fcr%04X >>>\n",
		InB(SCBRRx), InH(SCSMRx), InH(SCFCRx));
	tm_monitor();
#endif	//////////////////////////////////
	EI(imask);

	return E_OK;
}

/*
	割り込みハンドラ(受信)
*/
LOCAL	void	sio_inthdr_rx(UINT dintno)
{
	LINE_INFO	*li;
	SC_DEFS		*scdefs;
	W		ssr, len, i, c;
	UW		ptr, nptr;

	/* Search the port informartion */
	for (li = &LineInfo[0], i = nPorts; ; li++) {
		if (--i < 0) return;
		if (li->scdefs.c.intvec == (dintno & 0xff80)) {	// intvecは代表番号
			if (li->scdefs.fn != &ScFuncSCIF) return;
			break;
		}
	}
	scdefs = &li->scdefs;
///	fctl = scdefs->fctl;		/* FIFO configuration information */

	// ブレーク検出
	ssr = InH(SCFSRx);
	if (ssr & SCFSRx_BRK) {
		// ブレークフラグクリア
		OutH(SCFSRx, InH(SCFSRx) & ~SCFSRx_BRK);
		li->lsts     |= SCFSRx_BRK;
		li->lstshist |= SCFSRx_BRK;
	}

	// (エラーの有無に関わらず)受信データ有り
	len = InH(SCRFDRx) & SCRFDRx_MASK;

	// 受信データ長は長くてもFIFOサイズ
	if (len > SCRFDRx_SIZE) len = SCRFDRx_SIZE;

	for (i = 0; i < len; i++) {
		// 受信データとそのステータスを取得し、
		// ステータスフラグをクリア
		c   = InB(SCFRDRx);
		ssr = InH(SCFSRx);
		OutH(SCFSRx, ssr & ~(SCFSRx_ER | SCFSRx_RDF | SCFSRx_DR));

		// 受信データにエラー有り
		if (ssr & SCFSRx_ER) {
			// エラー状態を記録
			ssr &= SCFSRx_ERR_MASK;
			li->lsts |= ssr;
			li->lstshist |= ssr;

			// 受信バッファが空の時はイベント発生
			if ( li->in_wptr == li->in_rptr ) {
				tk_set_flg(li->flg, FLG_IN_NORM);
			}

			continue;	// 受信データは破棄される
		}

#ifdef REMOTE_MODE
		// ﾒﾝﾃﾎﾟｰﾄ remote処理
		if ((c = _remote_rx(c, REMOTE_MODE)) < 0) {
			if (c == -2) {				// 送信開始要求?
				// 送信割込許可
				OutH(SCSCRx, InH(SCSCRx) | SCSCRx_TIE);
			}
			continue;					// 受信データは無視（次のデータへ）
		}
#endif

		// 外部関数が設定されていれば呼び出す
		if (li->extfn != NULL && (*li->extfn)(li->extpar, c)) continue;

		// 送信Xon/Xoffフロー制御
		if (li->flow.sxflow) {
			if (c == XOFF) {
				li->flow.rcvxoff = 1;
				continue;
			}
			if(c == XON || (li->flow.rcvxoff && li->flow.xonany)) {
				li->flow.rcvxoff = 0;
				continue;
			}
		}

		// 受信バッファフルの時はエラー状態を記録して捨てる
		ptr  = li->in_wptr;
		nptr = PTRMASK(li, ptr + 1);
		if (nptr == li->in_rptr) {
			li->lsts |= SCFSRx_RDF;	// エラーコードをこの値で代用
			li->lstshist |= SCFSRx_RDF;
			continue;
		}

		// 受信データを受信バッファに格納
		li->in_buf[ptr] = c;
		li->in_wptr = nptr;

		// 受信バッファが空であった時はイベント発生
		if ( ptr == li->in_rptr ) tk_set_flg(li->flg, FLG_IN_NORM);

		// Xoffフロー制御 - RTS制御はハードウェアで処理される
		if (li->flow.rxflow && IN_BUF_REMAIN(li) < XOFF_MARGIN) {
			if(!li->flowsts.sndxoff) {
				li->flowsts.sndxoff = 1;
				li->flowsts.reqchar = XOFF;
			}
		}
	}
	// 受信割込クリア
	OutH(SCFSRx, InH(SCFSRx) & ~(SCFSRx_ER | SCFSRx_RDF | SCFSRx_DR));
}

/*
	割り込みハンドラ(送信)
*/
LOCAL	void	sio_inthdr_tx(UINT dintno)
{
	LINE_INFO	*li;
	SC_DEFS		*scdefs;
	W		ssr, i, n;
#ifdef REMOTE_MODE
	W		c;
#endif
	UW		ptr;

	/* Search the port informartion */
	for (li = &LineInfo[0], i = nPorts; ; li++) {
		if (--i < 0) return;
		if (li->scdefs.c.intvec == (dintno & 0xff80)) {	// intvecは代表番号
			if (li->scdefs.fn != &ScFuncSCIF) return;
			break;
		}
	}
	scdefs = &li->scdefs;
///	fctl = scdefs->fctl;		/* FIFO configuration information */

	// 送信完了
	ssr = InH(SCFSRx);
	if ((ssr & SCFSRx_TDFE) == 0) goto fin;

#ifdef REMOTE_MODE
	// ﾒﾝﾃﾎﾟｰﾄ remote処理
	if ((c = _remote_tx(REMOTE_MODE)) >= 0) {
		OutB(SCFTDRx, c);				// リモート文字を送信
		goto fin;
	}
#endif

	// この時点でTx Ready, Xoffだけチェックする
	if (li->flow.rcvxoff) goto nosend;

	ptr = li->ou_cnt;
	if ((n = li->ou_len - ptr) < 0) n = 0;	// 送信文字数
	if (li->flowsts.reqchar) n++;		// Xon/Xoff送信
	if (n > SCFCRx_TTRG_SZ) n = SCFCRx_TTRG_SZ;	// トリガサイズに制限

	// Xon/Xoffの送信
	if (li->flowsts.reqchar) {
		OutB(SCFTDRx, li->flowsts.reqchar);
		li->flowsts.reqchar = 0;
		n--;
	}
	if (n > 0) {
		// データ送信
		while(--n >= 0) OutB(SCFTDRx, li->ou_buf[ptr++]);
		li->ou_cnt = ptr;
	} else {
		// 送信完了 or 送信なし → 送信完了イベント発生
		if (ptr >= li->ou_len && li->ou_len > 0)
			tk_set_flg(li->flg, FLG_OU_NORM);
nosend:
		// 送信割込禁止
		OutH(SCSCRx, InH(SCSCRx) & ~SCSCRx_TIE);
	}
fin:
	// 送信割込クリア
	OutH(SCFSRx, InH(SCFSRx) & ~SCFSRx_TDFE);
}

/*
 *	Set the hardware configuration
 */
LOCAL	ER	setHwConf( LINE_INFO *li, RsHwConf_16450 *conf,
						RsMode mode, BOOL startup )
{
	SC_DEFS	*scdefs = &li->scdefs;
	ER	err;

	/* Cancel the current setting when it is already set */
	if ( scdefs->c.iostep > 0 ) {
		/* Disable the interrupt to the target port */
		li->enbint = 0;
		OutH(SCSCRx, 0);				// 割り込み禁止

		/* Release the interrupt handler */
		delete_inthdr(scdefs);
	}

	/* Save the new configuration */
	scdefs->c = *conf;
	li->mode = mode;

	if ( scdefs->c.iostep > 0 ) {

		/* Disable the interrupt to the target port */
		li->enbint = 0;
		OutH(SCSCRx, 0);				// 割り込み禁止

		/* Register the interrupt handler */
		err = regist_inthdr(scdefs, sio_inthdr_rx, sio_inthdr_tx);
		if (err < E_OK) return err;

		/* Initialize the line by the specified communication mode */
		err = init_sio(li, mode);
		if ( err < E_OK ) {
			/* Initialize by the default mode when error occurs */
			li->mode = scdefs->mode;
			err = init_sio(li, li->mode);
			if ( err < E_OK ) return err;
		}
		/* Interrupt is enabled by the init_sio (when "suspend = 0") */
	}

	return E_OK;
}

/*
	回線状態の設定
*/
LOCAL	RsStat	make_line_stat(LINE_INFO *li, UB lsts)
{
	RsStat	stat;

	*(UW *)&stat = 0;

	if (lsts & SCFSRx_RDF) stat.BE = 1;
	if (lsts & SCFSRx_FER) stat.FE = 1;
	if (lsts & SCFSRx_PER) stat.PE = 1;
	//stat.OE = 0;

	stat.XF = li->flow.rcvxoff;
	if (lsts & SCFSRx_BRK) stat.BD = 1;

	// モデム信号を取得できないので、常に送受信可とする
	stat.DR = 1;
	stat.CD = 1;
	stat.CS = 1;
	stat.CI = 0;
	return stat;
}

/*
	最初の1文字の送信(割り込み禁止状態で呼び出す)
	True - 送信に成功
*/
LOCAL	BOOL	send_1st_char(LINE_INFO *li, W c)
{
	SC_DEFS	*scdefs = &li->scdefs;
	W	n, lsts;

	// Xoff中は送信できない
	if (li->flow.rcvxoff) return FALSE;

	// FIFOに空きがなければ送信できない
	lsts = InH(SCFSRx);
	if ((lsts & (SCFSRx_TDFE | SCFSRx_TEND)) == 0) return FALSE;

	// 送信サイズを決定
	if (c < 0) {
		n = li->ou_len - li->ou_cnt;
		if (n <= 0) return FALSE;		// 送信すべきデータが無い
		if (n > SCFCRx_TTRG_SZ) n = SCFCRx_TTRG_SZ;	// トリガサイズに制限
		c = li->ou_buf[li->ou_cnt++];
	} else {
		n = 1;
	}

	// 送信
	OutB(SCFTDRx, c);
	while(--n > 0) OutB(SCFTDRx, li->ou_buf[li->ou_cnt++]);

	// 送信割込クリア
	OutH(SCFSRx, InH(SCFSRx) & ~SCFSRx_TDFE);

	// 送信割込許可
	OutH(SCSCRx, InH(SCSCRx) | SCSCRx_TIE);

	return TRUE;
}

/*
	送信完了待ち
*/
LOCAL	RTN	wait_send_complete(LINE_INFO *li, W tmout)
{
	RTN	rtn = RTN_OK;
	UINT	flgptn;
	UW	ptr;
	UW	imask;
	ER	err;

	// 送信割り込みを発生させるため、最初の1文字を送信する
	DI(imask);
	send_1st_char(li, -1);
	EI(imask);

	// 送信完了待ち
	do {
		ptr = li->ou_cnt;
		err = tk_wai_flg(li->flg, FLG_OU_WAIPTN,
				TWF_ORW | TWF_BITCLR, &flgptn, tmout);
		if (err == E_TMOUT) {
			// 前回から1文字も送信していないときタイムアウトエラー
			if (ptr == li->ou_cnt) {
				rtn = RTN_TMOUT;
				break;
			}
			continue;
		}
		if (err < E_OK || (flgptn & FLG_IN_ABORT)) {
			rtn = RTN_ABORT;
			break;
		}
	} while (li->ou_cnt < li->ou_len);

	li->ou_len = 0;						// 送信データ消去
	return rtn;
}

/*
	1文字入力
*/
LOCAL	WRTN	recv_one_char(LINE_INFO *li, W tmout)
{
	UB	c;
	UINT	flgptn;
	UW	imask;
	ER	err;

	// 受信バッファが空の間は受信待ち
	while(li->in_wptr == li->in_rptr) {

		if (tmout == 0) return RTN_NONE;	 // 待ちなし

		// 受信または回線状態変化を待つ
		err = tk_wai_flg(li->flg, FLG_IN_WAIPTN,
				TWF_ORW | TWF_BITCLR, &flgptn, tmout);
		if ( err == E_TMOUT ) return RTN_TMOUT;
		if ( err < E_OK || (flgptn & FLG_IN_ABORT) ) return RTN_ABORT;
		if ( li->lsts != 0) return RTN_ERR;	// 受信エラー
	}

	// 受信データの取り込み
	c = li->in_buf[li->in_rptr];
	li->in_rptr = PTRMASK(li, li->in_rptr + 1);

	// Xoffフロー制御  - RTS制御はハードウェアが行う
	if (li->flowsts.sndxoff && IN_BUF_REMAIN(li) > XON_MARGIN) {
		DI(imask);
		if (!send_1st_char(li, XON))
			li->flowsts.reqchar = XON;	// 送信予約
		li->flowsts.sndxoff = 0;
		EI(imask);
	}
	return c;
}

/*
	シリアルポート入力
*/
LOCAL	ER	scif_in(LINE_INFO *li, UB *buf, W len, W *alen, W tmout)
{
	SC_DEFS		*scdefs = &li->scdefs;
	W		c;
	UW		rsz = 0;
	RsErr		err;

	if ( scdefs->c.iostep <= 0 ) return E_NOMDA; 
				/* Hardware configuration is not set yet */

	// ポーリングの際におけるデッドロックを防止
	if (tmout == 0 && li->in_wptr == li->in_rptr) {
		err.w = E_BUSY;
		goto EEXIT;
	}

	/* Console input processing lock */
	if ( consMLock(&li->lock, IN_LOCK) < E_OK ) {
		err.s = make_line_stat(li, 0);
		err.c.ErrorClass = MERCD(E_IO);
		err.c.Aborted = 1;
		goto EEXIT;
	}

	err.w = E_OK;

	if (len <= 0) rsz = IN_BUF_SIZE(li);	// 受信済みバイト数を調べる
	else {
		li->lsts = 0;					// エラー状態のクリア

		// データ受信
		for ( rsz = 0; rsz < len; rsz++ ) {

			c = recv_one_char(li, tmout);
			if ( c < RTN_OK ) {
				if ( c == RTN_NONE ) {
					err.w = ( rsz > 0 ) ? E_OK : E_BUSY;
				} else {
					err.s = make_line_stat(li, li->lsts);
					err.c.ErrorClass = MERCD(E_IO);
					if ( c == RTN_TMOUT ) err.c.Timout  = 1;
					if ( c == RTN_ABORT ) err.c.Aborted = 1;
				}
				break;
			}
			buf[rsz] = c;
		}
	}

	// ABORTフラグのクリア
	tk_clr_flg(li->flg, ~(FLG_IN_ABORT | FLG_OU_ABORT));

	/* Release the lock */
	consMUnlock(&li->lock, IN_LOCK);

EEXIT:

	*alen = rsz;						// 実際の受信バイト数
	return err.w;
}

/*
	シリアルポート出力
*/
LOCAL	ER	scif_out(LINE_INFO *li, UB *buf, W len, W *alen, W tmout)
{
	SC_DEFS		*scdefs = &li->scdefs;
	RsErr		err;
	W		sz;
	RTN		r;

	if ( scdefs->c.iostep <= 0 ) return E_NOMDA;
				/* Hardware configuration is not set yet */

	/* Console output processing lock */
	if ( consMLock(&li->lock, OU_LOCK) < E_OK ) {
		err.s = make_line_stat(li, 0);
		err.c.ErrorClass = MERCD(E_IO);
		err.c.Aborted = 1;
		goto EEXIT;
	}

	err.w = E_OK;

	// 出力バッファは割込ハンドラで読み出すため、ページアウト不可。
	// よって、ユーザー指定のバッファ(buf)を直接使用するのではなく、
	// 常駐メモリーを出力バッファとして使用して出力データをコピーする。

	*alen = 0;
	while(len > 0) {
		// 送信データを出力バッファにコピー
		sz = (len < OUBUFSZ) ? len : OUBUFSZ;
		MEMCPY(li->ou_buf, buf, sz);
		buf += sz;
		len -= sz;

		li->ou_cnt = 0;
		li->ou_len = sz;

		// 送信開始 ＆ 完了待ち
		r = wait_send_complete(li, tmout);
		*alen += li->ou_cnt;			// 実際の送信済みバイト
		if (r < RTN_OK) {
			err.s = make_line_stat(li, 0);
			err.c.ErrorClass = MERCD(E_IO);
			if (r == RTN_TMOUT) err.c.Timout  = 1;
			if (r == RTN_ABORT) err.c.Aborted = 1;
			break;
		}
	}
	li->ou_cnt = 0;

	// ABORT フラグのクリア
	tk_clr_flg(li->flg, ~(FLG_IN_ABORT | FLG_OU_ABORT));

	/* Release the lock */
	consMUnlock(&li->lock, OU_LOCK);

EEXIT:

	return err.w;
}

/*
	シリアルポート制御
*/
LOCAL	ER	scif_ctl(LINE_INFO *li, W kind, UW *arg)
{
	SC_DEFS		*scdefs = &li->scdefs;
	ER		err = E_OK;
	UW		imask;
	W		n;

	if ( !(kind == DN_RS16450 || kind == -DN_RS16450)
		&& scdefs->c.iostep <= 0 ) return E_NOMDA;
				/* Hardware configuration is not set yet */

	// ロック不要

	switch ( kind ) {
	case RS_ABORT:						// アボート(待ちの解除)
		tk_set_flg(li->flg, FLG_IN_ABORT | FLG_OU_ABORT); /* Release */
		break;

	case RS_SUSPEND:					// サスペンド状態へ移行
	case RS_RESUME:						// サスペンド状態から復帰
		// SCIFはデバッグポートなので特に何もしない
		break;

	case DN_RSMODE:						// 通信モードの設定
		err = init_sio(li, *(RsMode*)arg);
		if(err == E_OK) {
			// シリアル回線管理情報の初期化
			DI(imask);
			li->in_rptr = li->in_wptr  = 0;
			li->ou_rptr = li->ou_wptr = 0;
			li->ou_cnt = li->ou_len   = 0;
			li->flow = li->scdefs.flow;
			li->flowsts = li->scdefs.flowsts;
			li->lsts = li->lstshist = 0;
			li->mode = *(RsMode*)arg;
			EI(imask);
		}
		break;

	case -DN_RSMODE:					// 通信モードの取得
		*(RsMode*)arg = li->mode;
		break;

	case DN_RSFLOW:						// フロー制御の設定
		// RTS/CTS共にonあるいはoffであること
			if((*(RsFlow*)arg).csflow ^ (*(RsFlow*)arg).rsflow) {
			err = E_PAR;
			break;
		}
		li->flow = *(RsFlow*)arg;
		rscs_ctrl(li);
		break;

	case -DN_RSFLOW:					// フロー制御の設定取得
		*(RsFlow*)arg = li->flow;
		break;

	case -DN_RSSTAT:					// 回線状態の取得
		*(RsStat*)arg = make_line_stat(li, li->lstshist);
		li->lstshist = 0;
		break;

	case DN_RSBREAK:					// ブレーク信号送出
		if ((n = *arg) > 0) {
			DI(imask);	break_ctrl(li, 1);	EI(imask);
			tk_dly_tsk(n);		// 待ち
			DI(imask);	break_ctrl(li, 0);	EI(imask);
		}
		break;

	case RS_RCVBUFSZ:					// 受信バッファ設定
		if ((n = *arg) < MIN_INBUFSZ) {
			err = E_PAR;
			break;
		}
		if(n != li->in_bufsz) {
			UB	*new, *old;
			new = Malloc(n);
			if (new != NULL) {
				DI(imask);
				old = li->in_buf;
				li->in_buf   = new;
				li->in_rptr  = li->in_wptr = 0;
				li->in_bufsz = n;
				EI(imask);
				Free(old);
			} else {
				err = E_NOMEM;
			}
		}
		break;

	case -RS_RCVBUFSZ:					// 受信バッファサイズ取得
		*arg = li->in_bufsz;
		break;

	case RS_EXTFUNC:					// 外部処理関数の設定
		if (li->extfn != NULL && (FUNCP)arg[0] != NULL) {
			err = E_OBJ;				// すでに設定されている
		} else {
			DI(imask);
			li->extfn  = (FUNCP)arg[0];
			li->extpar = arg[1];
			EI(imask);
		}
		break;

	case RS_LINECTL:	// 制御線のON/OFF切り替えは行えない
	case DN_RS16450:	// ハードウエア構成の設定・取得はしない
	case -DN_RS16450:	// ハードウエア構成の設定・取得はしない
		err = E_NOSPT;
		break;

	default:
		err = E_PAR;
		break;
	}
	return	err;
}

/*
	シリアルポートの停止
*/
LOCAL void	scif_down(LINE_INFO *li)
{
	SC_DEFS		*scdefs = &li->scdefs;

	// SCIF割り込みを無効にする
	if ( scdefs->c.iostep > 0 ) {
		OutH(SCSCRx, 0);				// 割り込み禁止
		li->enbint = 0;
		delete_inthdr(scdefs);
	}
}

/*
	シリアルポートの起動
*/
LOCAL void	scif_up(LINE_INFO *li)
{
	li->suspend = 0;
	li->flow = li->scdefs.flow;
	setHwConf(li, &li->scdefs.c, li->scdefs.mode, TRUE);
}


/*----------------------------------------------------------------------*/
#if 0
#|【sh_scif.c 変更履歴】
#|□2015/12/24	[app_sh7760]用に、[tef_em1d]の"ns16450.c"を参考に作成。
#|□2015/12/24	ﾊﾟｰｿﾅﾙﾒﾃﾞｨｱ社のTE-SH7760用ﾄﾞﾗｲﾊﾞｿｰｽ｢scif.c｣を参考に修正。
#|□2016/02/18	｢REMOTE_MODE｣に対応。
#|
#endif
