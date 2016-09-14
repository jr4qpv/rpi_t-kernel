/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2015 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2015/12/25.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sh_scifsys.h	Console/Low-level serial I/O driver
 *
 *	Serial line low level driver (for SH7760 SICF)
 *			app-sh7760 system-dependent definition
 */


/*
 *	Post-interrupt processing
 */
Inline	void	end_inthdr(SC_DEFS *scdefs)
{
	/* Unnecessary to execute anything */
}

/*
 *	Release an interrupt handler
 */
Inline	void	delete_inthdr(SC_DEFS *scdefs)
{
	ER	err;
	INTVEC iv_eri, iv_rxi, iv_bri, iv_txi;

	iv_eri = scdefs->c.intvec;			/* ERI割込みコード */
	iv_rxi = scdefs->c.intvec + 0x20;	/* RXI割込みコード */
	iv_bri = scdefs->c.intvec + 0x40;	/* BRI割込みコード */
	iv_txi = scdefs->c.intvec + 0x60;	/* TXI割込みコード */

	/* Release an interrupt handler */
	err = tk_def_int(iv_eri, NULL);
	err |= tk_def_int(iv_rxi, NULL);
	err |= tk_def_int(iv_bri, NULL);
	err |= tk_def_int(iv_txi, NULL);

	if (err >= E_OK) {
		/* Disable interrupt */
		DisableInt(scdefs->c.intvec);
	}
}

/*
 *	Register the interrupt handler
 */
Inline	ER	regist_inthdr(SC_DEFS *scdefs, void *sio_inthdr_rx, void *sio_inthdr_tx)
{
	T_DINT	dint;
	ER	err;
	INTVEC iv_eri, iv_rxi, iv_bri, iv_txi;

	iv_eri = scdefs->c.intvec;			/* ERI割込みコード */
	iv_rxi = scdefs->c.intvec + 0x20;	/* RXI割込みコード */
	iv_bri = scdefs->c.intvec + 0x40;	/* BRI割込みコード */
	iv_txi = scdefs->c.intvec + 0x60;	/* TXI割込みコード */

	// 割り込みハンドラ定義
	dint.intatr = TA_HLNG;
	dint.inthdr = sio_inthdr_rx;
	err = tk_def_int(iv_eri, &dint);
	err |= tk_def_int(iv_rxi, &dint);
	err |= tk_def_int(iv_bri, &dint);

	//dint.intatr = TA_HLNG;
	dint.inthdr = sio_inthdr_tx;
	err |= tk_def_int(iv_txi, &dint);
	if (err >= E_OK) {
		/* 割り込み許可 */
		EnableInt(scdefs->c.intvec, scdefs->c.iostep);	// 割込みレベル

#ifdef CPU_SH7760
		switch (scdefs->c.iobase) {
		  case 0xfe600000:				// SCIF0 ?
			out_w(INTMSKCLR00, 0x00000f00);	// SCIF0割り込みマスククリア
			break;
		  case 0xfe610000:				// SCIF1 ?
			out_w(INTMSKCLR00, 0x000000f0);	// SCIF1割り込みマスククリア
			break;
		  case 0xfe620000:				// SCIF2 ?
			out_w(INTMSKCLR00, 0x0000000f);	// SCIF2割り込みマスククリア
			break;
		}
#endif
	}
	else {
		tk_def_int(iv_eri, NULL);
		tk_def_int(iv_rxi, NULL);
		tk_def_int(iv_bri, NULL);
		tk_def_int(iv_txi, NULL);
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


/*----------------------------------------------------------------------*/
#if 0
#|【sh_scifsys.h 変更履歴】
#|□2015/12/25	[app_sh7760]用に、[tef_em1d]の"ns16450sys.h"を参考に作成。
#|
#endif
