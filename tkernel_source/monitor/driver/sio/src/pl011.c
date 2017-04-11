/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/02/28.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/03/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)pl011.c (rpi_bcm283x) 2016/08/29
 *
 *       ARM UART(PL011) serial port I/O
 */

#include <tmonitor.h>

/*
 * serial port hardware configuration definition
 */
typedef struct {
	UW	iob;		/* I/O base address */
} DEFSIO;

#if 0
#  define IOSTEP		/* I/O address separation */
#  define CLOCK			/* input clock (Hz) */

/* ----------------------------------------------------------------------- */
#elif _RPI_BCM283x_
#  include <rpi/bcm283x.h>
   LOCAL const DEFSIO	DefSIO[1] = {
			{ UART0_BASE },
   };
#  define IOSTEP	4
#  define _CLOCK	UART0_CLOCK		/* 3MHz */

#define	_IOADR(n)		( IOB + (n) * IOSTEP )
#include <rpi/pl011.h>				/* Common definition */
#endif

#define	N_DEFSIO	( sizeof(DefSIO) / sizeof(DEFSIO) )

/* ------------------------------------------------------------------------ */

#define	IOB	( scb->info )		/* I/O base address */

#if !defined(IN)
#define IN(x)		in_w(x)
#define OUT(x, y)	out_w((x), (y))
#endif

/* ------------------------------------------------------------------------ */

/*
 * Power on RS-232C driver IC
 */
#define	RSDRV_PWON(siocb)		/* no operation */

/*
 * serial port I/O
 */
LOCAL void putSIO_pl011( SIOCB *scb, UB c )
{
	RSDRV_PWON(scb);

	/* wait until transmission is ready. */
	while ((IN(regFR) & FR_TXFF) != 0);

	/* write transmission data */
	OUT(regDR, c);

	/* wait until the completion of transmission */
	while ((IN(regFR) & FR_BUSY) != 0);
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
LOCAL W getSIO_pl011(SIOCB *scb, W tmo )
{
	W	sts, c = 0;

	RSDRV_PWON();

	tmo *= 1000/20;		/* convert tmo to 20 usec units */

	/* receive as much data as possible in the receive buffer */
	while (scb->iptr - scb->optr < SIO_RCVBUFSZ) {

		/* is there data in FIFO? */
		if ((sts = IN(regFR)) & FR_RXFE) {
			if (scb->iptr != scb->optr) break;  /* already received */
			if (tmo-- <= 0) break;		    /* timeout */
			waitUsec(20);
			continue;
		}

		/* receive data input */
		c = IN(regDR);

		/* error check */
		if (c & ~0xFF) continue;			/* recive error ? */

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
EXPORT ER initSIO_pl011(SIOCB *scb, const CFGSIO *csio, W speed)
{
	UH	div;
	UW	tmp;

	if ( (UW)csio->info >= N_DEFSIO ) return E_PAR;

	/* select the target port */
	scb->info = DefSIO[csio->info].iob;

	/* communicatin speed default value */
	div = SC_LINE_SPEED(speed);

	/* initialize serial controller */
	OUT(regCR, 0);						/* UART stop */

	/* GPIOピンの選択 */
	tmp = IN(GPFSEL1);
	tmp &= ~(7<<(3*4));					// GPIO14 clear
	tmp |= (4<<(3*4));					// GPIO14 set ALT0(TXD0)
	tmp &= ~(7<<(3*5));					// GPIO15 clear
	tmp |= (4<<(3*5));					// GPIO15 set ALT0(RXD0)
	OUT(GPFSEL1, tmp);

	/* ボーレートの設定 */
	OUT(regFBRD, 0xff);		/* to keep the following from happening, Div = 0 */
	OUT(regIBRD, div >> 8);			/* 整数部(Q8) communication speed */
	OUT(regFBRD, (div & 0xff)>>2);	/* 小数部(Q8→Q6) */

	OUT(regIFLS, IFLS_RxIF_1by2|IFLS_TxIF_1by2);		/* FIFOレベルの設定 */
	OUT(regLCRH, LCRH_8bitData|LCRH_FEN|LCRH_NoParity);	/* 8bit,Noﾊﾟﾘﾃｨ,Stop1 */

	OUT(regCR, CR_RTS|CR_RXE|CR_TXE|CR_UARTEN);	/* UART start */

	/* I/O function default */
	scb->put = putSIO_pl011;
	scb->get = getSIO_pl011;

	return E_OK;
}


/*----------------------------------------------------------------------
#|History of "pl011.c"
#|====================
#|* 2016/03/02	It's made by making reference to "ns16550.c".
#|* 2016/08/29	初期化時にGPIOピンの選択処理を追加
#|
*/
