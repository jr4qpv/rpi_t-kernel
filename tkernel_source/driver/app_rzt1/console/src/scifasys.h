/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2016/07/12.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sh_scifsys.h	Console/Low-level serial I/O driver
 *
 *	Serial line low level driver (for RZT1 SICFA)
 *			app_rzt1 system-dependent definition
 */


/*
 *	Post-interrupt processing
 */
Inline	void	end_inthdr(SC_DEFS *scdefs)
{
	/* Unnecessary to execute anything */
	EndOfInt(scdefs->c.intvec);
}

/*
 *	Release an interrupt handler
 */
Inline	void	delete_inthdr(SC_DEFS *scdefs)
{
	ER	err;
	INTVEC iv_brif, iv_rxif, iv_txif, iv_drif;

	iv_brif = scdefs->c.intvec;			/* BRIF割込みコード */
	iv_rxif = scdefs->c.intvec + 1;		/* RXIF割込みコード */
	iv_txif = scdefs->c.intvec + 2;		/* TXIF割込みコード */
	iv_drif = scdefs->c.intvec + 3;		/* DRIF割込みコード */

	/* Release an interrupt handler */
	err = tk_def_int(iv_brif, NULL);
	err |= tk_def_int(iv_rxif, NULL);
	err |= tk_def_int(iv_txif, NULL);
	err |= tk_def_int(iv_drif, NULL);

	if (err >= E_OK) {
		/* Disable interrupt */
		DisableInt(iv_brif);
		DisableInt(iv_rxif);
		DisableInt(iv_txif);
		DisableInt(iv_drif);
	}
}

/*
 *	Register the interrupt handler
 */
Inline	ER	regist_inthdr(SC_DEFS *scdefs, void *sio_inthdr_rx, void *sio_inthdr_tx)
{
	T_DINT	dint;
	ER	err;
	INTVEC iv_brif, iv_rxif, iv_txif, iv_drif;

	iv_brif = scdefs->c.intvec;			/* BRIF割込みコード */
	iv_rxif = scdefs->c.intvec + 1;		/* RXIF割込みコード */
	iv_txif = scdefs->c.intvec + 2;		/* TXIF割込みコード */
	iv_drif = scdefs->c.intvec + 3;		/* DRIF割込みコード */

	// 割り込みハンドラ定義
	dint.intatr = TA_HLNG;
	dint.inthdr = sio_inthdr_rx;
	err = tk_def_int(iv_brif, &dint);
	err |= tk_def_int(iv_rxif, &dint);

	//dint.intatr = TA_HLNG;
	dint.inthdr = sio_inthdr_tx;
	err |= tk_def_int(iv_txif, &dint);
	err |= tk_def_int(iv_drif, &dint);
	if (err >= E_OK) {
		/* 割り込み許可 & 割り込みレベル設定 */
		EnableInt(iv_brif, scdefs->c.iostep);
		EnableInt(iv_rxif, scdefs->c.iostep);
		EnableInt(iv_txif, scdefs->c.iostep);
		EnableInt(iv_drif, scdefs->c.iostep);
	}
	else {
		tk_def_int(iv_brif, NULL);
		tk_def_int(iv_rxif, NULL);
		tk_def_int(iv_txif, NULL);
		tk_def_int(iv_drif, NULL);
	}

	return err;
}

/*
 *	Suspend/Resume
 */
Inline	void	sio_susres(LINE_INFO *li, W resume)
{
	/* Execute nothing */
}


/*----------------------------------------------------------------------
#|History of "scifasys.h"
#|=======================
#|* 2016/07/12	[app_rzt1]用に、[tef_em1d]の"ns16450sys.h"を参考に作成。
#|
*/
