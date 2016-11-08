/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2015-2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2015/12/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sh_scif.c () 2016/03/17
 *
 *       serial port I/O for SH7760 SCIF
 */

/*
 *[Note]
 *・SH7760 SCIF(シリアルポート)の通信ドライバー
 *
 *注）GCCでは、ﾋﾞｯﾄﾌｨｰﾙﾄﾞのｱｸｾｽ(WORD,BYTEなど)がちゃんと出来ないようなので
 *    利用しないこと！！！
 */

#include <tmonitor.h>

/*
 * serial port hardware configuration definition
 */
typedef struct {
	UW	iob;		/* I/O base address */
} DEFSIO;

#if 0
#  define CLOCK			/* input clock (Hz) */

/* ----------------------------------------------------------------------- */
#elif _APP_SH7760_
#  include <misc/renesas/sh7760.h>
   LOCAL const DEFSIO	DefSIO[3] = {
			{ (UW)&SCIF0 },
			{ (UW)&SCIF1 },
			{ (UW)&SCIF2 }
   };
  #define CLOCK		32000000			/* [Hz]…IOP5:Pck=32MHz */


  /*-------( SCIF初期値 )-------*/
  #define	SCSCR_INIT	0x0000			/* CKE=0 */
  #define	SCSMR_INIT	0x0000			/* T-Kernel標準:調歩同期,8bit,なし,STOP1 */
//  #define	SCSMR_INIT	0x0030			/* Term標準:調歩同期,8bit,ODD,STOP1 */

  /* transmission speed -> divided counter value. */
#if 1
  /* 四捨五入 */
  #define	ASK_BBR(bps)		((((CLOCK/32)+(bps/2)) / (bps)) - 1)
#else
  /* 計算結果が切り捨てだと、115.2Kbpsの時に誤差がでる */
  #define	ASK_BBR(bps)		(CLOCK / (64/2 * (bps)) - 1)
#endif

  /* SCFCR register */
  #define	SC_RE		0x10			/* RE */
  #define	SC_TE		0x20			/* TE */

  /* SCFSR register */
  #define	SC_RDF		0x02			/* ﾚｼｰﾌﾞFIFOﾃﾞｰﾀフル */
  #define	SC_RxERR	0x91			/* ER,BRK,DR=1 */
  #define	SC_DR		0x01			/* DR */
  #define	SC_TDFE		0x20			/* TDFE */
  #define	SC_TEND		0x40			/* TEND */

#endif


/* ------------------------------------------------------------------------ */
#define	N_DEFSIO	( sizeof(DefSIO) / sizeof(DEFSIO) )



/* ------------------------------------------------------------------------ */

/*
 * Power on RS-232C driver IC
 */
#define	RSDRV_PWON(siocb)		/* no operation */


/*
 * serial port I/O
 */
LOCAL void putSIO_scif( SIOCB *scb, UB c )
{
	UH	sts;
	volatile struct st_scif *iobp;

	RSDRV_PWON(scb);

	iobp = (struct st_scif *)scb->info;		// iobp←SCIFベースアドレス

	/* wait until transmission is ready. */
	while (((sts = iobp->SCFSR.WORD) & SC_TDFE) == 0);	// TDFE=1 ?, 書き込み可能まで待つ

	/* write transmission data */
	iobp->SCFTDR = c;						// 文字出力
	sts &= ~(SC_TDFE | SC_TEND);
	iobp->SCFSR.WORD = sts;					// TDFE,TENDｸﾘｱ

	waitMsec(0);							// WDTトリガ

	/* wait until the completion of transmission */
	while ((iobp->SCFSR.WORD & SC_TEND)== 0);	// TEND=1 ?, 送信終了まで待つ
}


/*
 * serial port input
 *       tmo     timeout (milliseconds)
 *              You can not wait forever.
 *       return value       >= 0 : character code
 *                 -1 : timeout
 *       input data using buffer.
 *       receive error is ignored.
 */
LOCAL W getSIO_scif(SIOCB *scb, W tmo )
{
	UH	sts;
	W	c = 0;
	volatile struct st_scif *iobp;

	RSDRV_PWON();

	tmo *= 1000/20;		/* convert tmo to 20 usec units */
	iobp = (struct st_scif *)scb->info;		// iobp←SCIFベースアドレス

	/* receive as much data as possible in the receive buffer */
	while (scb->iptr - scb->optr < SIO_RCVBUFSZ) {
		waitMsec(0);						// WDTトリガ

		/* is there data in FIFO? */
		if ( !((sts = iobp->SCFSR.WORD) & (SC_RDF | SC_RxERR))) {
			if (scb->iptr != scb->optr) break;  /* already received */
			if (tmo-- <= 0) break;		    /* timeout */
			waitUsec(20);					/* 20μ秒待ち */
			continue;
		}

		/* error check */
		if (sts & SC_RxERR) {
			if (sts & SC_DR)
				iobp->SCFRDR;				/* 文字入力(受信ﾃﾞｰﾀ廃却) */

			iobp->SCFSR.WORD &= (~SC_RxERR & 0x00FF);	/* ER,BRK,DRをｸﾘｱ */
			iobp->SCLSR.WORD = 0;			/* ORERをｸﾘｱ */
			continue;
		}

		/* receive data input */
		if (sts & SC_RDF) {
			c = iobp->SCFRDR;				/* 文字入力 */
			iobp->SCFSR.WORD &= (~SC_RDF & 0x00FF);	/* RDFｸﾘｱ */
		}

		/* set data to rcvbuf */
		scb->rcvbuf[scb->iptr++ & SIO_PTRMSK] = c;
	}

	/* return the data in rcvbuf */
	return (scb->iptr == scb->optr)?
			-1 : scb->rcvbuf[scb->optr++ & SIO_PTRMSK];
}


/* ------------------------------------------------------------------------ */

/*
 * initialize serial port
 *       serial port that is supported by the initialization of CFGSIO
 *       speed   communication speed (bps)
 *       initialize the serial port according to the specified parameters and set SIOCB
 *       SIOCB is given in 0-cleared state initially.
 *       Subsequent I/O operations uses the SIOCB.
 *
 *       Only for PC/AT version
 *      if speed = 0, we use the value in biosp->siomode.
 *       But we use only the transmission speed and other settings are ignored.
 *       Efforts were made to be compatible B-right/V, but because of the ignorance of no-speed settings such as data length and stop bit length,
 *       we have reduced functionality.
 */
EXPORT ER initSIO_scif(SIOCB *scb, const CFGSIO *csio, W speed)
{
	UH	div;
	volatile struct st_scif *iobp;

	if ( (UW)csio->info >= N_DEFSIO ) return E_PAR;

	/* select the target port */
	scb->info = DefSIO[csio->info].iob;			/* SCIFベースアドレス情報セット */

	iobp = (struct st_scif *)scb->info;			/* iobp←SCIFベースアドレス */

	/*--- communicatin speed default value ---*/
	div = ASK_BBR(speed);

	/* initialize serial controller */
	iobp->SCSCR.WORD &= 0x0003;			// TE,REを0ｸﾘｱ(TIE,RIEも0)
	iobp->SCFCR.WORD = 0x0006;			// FIFOﾃﾞｰﾀﾘｾｯﾄ(TFRST,RFRSTを1にｾｯﾄ)
	iobp->SCSCR.WORD = SCSCR_INIT;		// SCSCRの設定(ｼﾘｱﾙｺﾝﾄﾛｰﾙ), CKE
	iobp->SCSMR.WORD = SCSMR_INIT;		// SCSMRの設定(ｼﾘｱﾙﾓｰﾄﾞ), 通信ﾌｫｰﾏｯﾄ
	iobp->SCBRR = div;					// SCBRRの設定(ﾋﾞｯﾄﾚｰﾄ), 通信速度

	iobp->SCFCR.WORD = 0;				// TFRST,RFRSTを0ｸﾘｱ

	/* 1ﾋﾞｯﾄ期間経過待ち */
	waitMsec(5);						// 5m秒待ち

	/* 送信,受信許可 */
	iobp->SCSCR.WORD |= (SC_TE | SC_RE);	// 送信,受信許可

	/*--- I/O function default ---*/
	scb->put = putSIO_scif;
	scb->get = getSIO_scif;

	return E_OK;
}


/* ----------------------------------------------------------------------- */
#if 0
#|History of "sh_scif.c"
#|======================
#|* 2015/12/04	[app_sh7760]用に、"ns16550.c"を参考に新規作成。
#|
#endif
