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
 *    Modified by T.Yokobayashi at 2016/07/25.
 *
 *----------------------------------------------------------------------
 */

/*
 *	pl011.c	Console/Low-level serial I/O driver
 *
 *	Serial line low-level driver (for UART/PL011)
 */

#include "line_drv.h"
#include <sys/sysinfo.h>
#include <misc/rpi/bcm283x.h>

/*
 *	Define the serial controller operation function groups
 */
LOCAL ER	pl011_in(LINE_INFO *li, UB *buf, W len, W *alen, W tmout);
LOCAL ER	pl011_out(LINE_INFO *li, UB *buf, W len, W *alen, W tmout);
LOCAL ER	pl011_ctl(LINE_INFO *li, W kind, UW *arg);
LOCAL void	pl011_up(LINE_INFO *li);
LOCAL void	pl011_down(LINE_INFO *li);

EXPORT	SC_FUNC	ScFuncPL011 = {
	pl011_in, pl011_out, pl011_ctl, pl011_up, pl011_down,
};

/*
 *	Define the serial controller ("PL011")
 */
#include <misc/rpi/pl011.h>		/* Common definition */
#include "pl011sys.h"			/* System-dependent definition */

/* Clock */
#define	_CLOCK		UART_CLK

/* I/O port address */
#define	_IOADR(n)	(scdefs->c.iobase + (n) * scdefs->c.iostep)

/*
 *	Bit-unit access to serial controller register
 */
Inline void clr_screg( W port, UW m )
{
	OutW(port, InW(port) & ~m);
}
Inline void set_screg( W port, UW m )
{
	OutW(port, InW(port) | m);
}
Inline void set_screg2( W port, UW m, UW v )
{
	OutW(port, (InW(port) & ~m) | v);
}

/*
 *	"RTS" and "Break" signal operations
 */
#define ON_RTS		set_screg(regCR, CR_RTS|CR_OUT2)   /* Turn on RTS */
#define OFF_RTS		set_screg2(regCR, CR_RTS, CR_OUT2) /* Turn off RTS */
#define	ON_BREAK	set_screg(regLCRH, LCRH_BRK)	/* Break send */
#define	OFF_BREAK	clr_screg(regLCRH, LCRH_BRK)	/* Break end */

/*
 *	Send/Receive interrupt control
 */

#define	ENB_RXINT(li)	OutW(regIMSC, (li->enbint == 0) ? \
					0x0000 : (IMSC_Recv))
#define	ENB_TXRXINT(li)	OutW(regIMSC, (li->enbint == 0) ? \
					0x0000 : (IMSC_Send|IMSC_Recv))
#define	DIS_TXRXINT(li)	OutW(regIMSC, 0x0000)

/*
 *	Forcible setting of the control line
 */
LOCAL	ER	line_ctl( LINE_INFO *li, UW cmd )
{
	SC_DEFS	*scdefs = &li->scdefs;
	UW	line;
	ER	err = E_OK;

	line = cmd & (CR_RTS|CR_DTR);

	switch ( cmd & 0xfffffffc ) {
	  case RSCTL_ON:
		set_screg(regCR, line | CR_OUT2);
		break;
	  case RSCTL_OFF:
		set_screg2(regCR, line, CR_OUT2);
		break;
	  case RSCTL_SET:
		set_screg2(regCR, CR_RTS|CR_DTR, line | CR_OUT2);
		break;
	  default:
		err = E_PAR;
	}

	return err;
}

/*
 *	Initialize the serial controller (mode setting)
 */
LOCAL	ER	init_sio( LINE_INFO *li, RsMode mode )
{
	SC_DEFS	*scdefs = &li->scdefs;
	UH	divcnt;
	UW	lctl, fctl, mctl;
	W	i;
	ER	err = E_OK;

	/* Parameter check */
	if ( mode.parity == 3
	  || mode.stopbits == 3
	  || mode.baud < 50 || mode.baud > SC_LINE_SPEED(1) ) return E_PAR;

	/* Communication speed setting value */
	divcnt = SC_LINE_SPEED(mode.baud);

	/* Line mode */
	lctl = 0x00;
	switch ( mode.datalen ) {
		case 0: lctl |= LCRH_5bitData; break;	/* 5bits data */
		case 1: lctl |= LCRH_6bitData; break;	/* 6bits data */
		case 2: lctl |= LCRH_7bitData; break;	/* 7bits data */
		case 3: lctl |= LCRH_8bitData; break;	/* 8bits data */
	}
	switch ( mode.stopbits ) {
		case 1: return E_NOSPT;					/* 1.5 stop bits */
		case 2: lctl |= LCRH_STP2; break;		/* 2 stop bits */
	}
	switch ( mode.parity ) {
	  case 1: lctl |= LCRH_OddParity;  break;	/* Odd parity */
	  case 2: lctl |= LCRH_EvenParity; break;	/* Even parity */
	}

	/* FIFO mode*/
	lctl |= LCRH_FEN;							/* Enable FIFOs */
	fctl = FIFO_Enable|IFLS_RxIF_1by2|IFLS_TxIF_1by2;	/* FIFO level select */

	/* Modem mode*/
	mctl = ( li->suspend == 0 )? CR_RTS|CR_DTR|CR_OUT2: CR_OUT2;

	/* Disable the interrupt */
	li->enbint = 0;
	DIS_TXRXINT(li);

	/* Wait for the send buffer to become empty for confirmation */
	for ( i = 0; i < FIFO_SIZE; ++i ) {
		if ( (InW(regFR) & FR_BUSY) == 0 ) break;
		tk_dly_tsk(20);
	}

	/* Clear the pending interrupt */
	OutW(regICR, ICR_ALL);
///	InW(SC_LSTS);		/* Clear the "IS_LSTS" */
///	InW(SC_INTS);		/* Clear the "IS_SND" */
///	InW(SC_MSTS);		/* Clear the "IS_MSTS" */

	/* Read and discard the data remaining in the receive-buffer */
	for ( i = 0; i < FIFO_SIZE; ++i ) {
		InW(regDR);						/* Clear receive FIFO */
	}

	OutW(regCR, 0);						/* UART stop */

	if ( fctl != 0 ) {
		/* Check the presence and absence of FIFO */
///		if ( (InW(SC_INTS) & IS_FIFO) == 0 ) {
///			fctl = 0;  /* There is no FIFO */
///		}
	}
	scdefs->fctl = fctl;

	/* Communication speed over 19200 bps is disabled without FIFO */
	if ( fctl == 0 && mode.baud > 19200 ) err = E_NOSPT;

	/* Return to RS flow status */
	if ( li->flowsts.rsoff ) mctl &= ~CR_RTS;  /* RTS OFF */

	/* Initialize the serial controller */
	OutW(regFBRD, 0xff);		/* to keep the following from happening, Div = 0 */
	OutW(regIBRD, divcnt >> 8);			/* 整数部(Q8) Communication speed */
	OutW(regFBRD, (divcnt & 0xff)>>2);	/* 小数部(Q8→Q6) */
	OutW(regIFLS, (fctl == 0) ? 0 : (fctl & (IFLS_RXIFLSEL | IFLS_TXIFLSEL)));
										/* Set FIFO level select */
	OutW(regLCRH, lctl);				/* Line mode */

	OutW(regCR, mctl|CR_RXE|CR_TXE|CR_UARTEN);	/* Set Modem mode, UART start */
	li->msts = InW(regCR);				/* Initilize the modem status */

	if ( err == E_OK && li->suspend == 0 ) {
		li->enbint = 1;
		ENB_RXINT(li);		/* Enable the rcv interrupt only */
	}

	return err;
}

/*
 *	Interrupt handler
 */
LOCAL	void	sio_inthdr( UINT dintno )
{
	LINE_INFO	*li;
	SC_DEFS		*scdefs;
	UB		c;
	UW		lsts, fctl, mis, d;
	UW		ptr, nptr;
	W		i, n, cnt;

	/* Search the port informartion */
	for (li = &LineInfo[0], i = nPorts; ; li++) {
		if (--i < 0) return;
		if (li->scdefs.c.intvec == dintno) {
			if (li->scdefs.fn != &ScFuncPL011) return;
			break;
		}
	}
	scdefs = &li->scdefs;
	fctl = scdefs->fctl;		/* FIFO configuration information */

   /* Execute the processing while the interrupt cause exists */

  while( CheckInt(scdefs->c.intvec) ) {
   mis=InW(regMIS);

   if(mis & (MIS_RXMIS|MIS_RTMIS)) {	/*** 受信関係割り込み? ***/
	/* Receive processing : repeat until there is no receive data :
		Limit the maximum number to prevent the infinite loop
		when card is pulled out */

	for ( cnt = 0; cnt < FIFO_SIZE; cnt++ ) {
		lsts = InW(regFR);
		if ((lsts & FR_RXFE) != 0) break;	/* 受信FIFOが空? */

		d = InW(regDR);					/* 受信FIFOからデータ読み出し */

		if ( (d & DR_RxERR) != 0 ) {
			UW s;

			/* Receive error*/
			s = d & DR_RxERR;
			li->lsts |= s;		/* Record the error status */
			li->lstshist |= s;

			/* Generate event when the receive-buffer is empty */
			if ( li->in_wptr == li->in_rptr ) {
				tk_set_flg(li->flg, FLG_IN_NORM);
			}

			if ( fctl != 0 && (d & DR_OE) != 0 ) {
				/* If overrun error occurs when using FIFO,
				   the all receive FIFO shall be cleared. */
///				OutW(SC_FCTL, fctl|FC_RXCLR);

				/* /// オーバラン検出時の処理を記述 */
			}

			continue;
		}

		/* Fetch the receive data */
		c = (UB)d;

		/* Call the external function */
		if ( li->extfn != NULL ) {
			if ( (*li->extfn)(li->extpar, c) ) continue;
		}

		/* Send "XON/XOFF" flow control  */
		if ( li->flow.sxflow ) {
			if ( c == XOFF ) {
				li->flow.rcvxoff = 1;
				continue;
			}
			if ( c == XON
			  || (li->flow.rcvxoff && li->flow.xonany) ) {
				li->flow.rcvxoff = 0;
				continue;
			}
		}

		/* Record and discard the error status
					when the receive buffer is full  */
		ptr = li->in_wptr;
		nptr = PTRMASK(li, ptr + 1);
		if ( nptr == li->in_rptr ) {
			li->lsts |= FR_RxOV;	/* Record the error status */
			li->lstshist |= FR_RxOV;
			continue;
		}

		/* Store the receive data to the receive-buffer */
		li->in_buf[ptr] = c;
		li->in_wptr = nptr;

		/* Generate the event when a receive-buffer is empty */
		if ( ptr == li->in_rptr ) tk_set_flg(li->flg, FLG_IN_NORM);

		/* Receive error control */
		if ( (li->flow.rsflow || li->flow.rxflow)
		  && IN_BUF_REMAIN(li) < XOFF_MARGIN ) {
			/* RS flow control */
			if ( li->flow.rsflow && li->flowsts.rsoff == 0 ) {
				li->flowsts.rsoff = 1;
				OFF_RTS;
			}
			/* "XON/XOFF" flow control */
			if ( li->flow.rxflow && li->flowsts.sndxoff == 0 ) {
				li->flowsts.sndxoff = 1;
				li->flowsts.reqchar = XOFF;
			}
		}
	}
   }

   if(mis & MIS_TXMIS) {				/*** 送信関係割り込み? ***/
	/* Send processing*/
	lsts = InW(regFR);
	li->msts = InW(regCR);
///?????	li->lsts = lsts;

	if ( (lsts & FR_TXFF) == 0				/* Tx Ready */
	  && li->flow.rcvxoff == 0				/* XOFF */
	  && (li->flow.csflow == 0 || (lsts & FR_CTS) != 0)	/* CS ON */
	) {
		cnt = n = li->ou_wptr - li->ou_rptr;
		/* The number of characters to send */
		if ( li->flowsts.reqchar != 0 ) n++; /* Send the "XON/XOFF" */
		if ( fctl != 0 ) {
			if ( n > FIFO_SIZE ) n = FIFO_SIZE;
				/* Limit to the FIFO buffer size */
		} else {
			if ( n >  1 ) n = 1;	/* There is no FIFO */
		}

		if ( n > 0 ) {			/* There is the send data */
			if ( li->flowsts.reqchar != 0 ) {
				/* Send the "XON / XOFF" */
				OutW(regDR, li->flowsts.reqchar);
				li->flowsts.reqchar = 0;
				n--;
			}
///			cnt -= n;	/* Remainig number of chars to send */

			ENB_TXRXINT(li); /* Enable the snd/rcv interrupts */

			/* Send the data */
			while ( --n >= 0 ) {
				lsts = InW(regFR);
				if (lsts & FR_TXFF) break;	/* Tx Full? */

				OutW(regDR, li->ou_buf[
					OU_PTRMASK(li, li->ou_rptr++)]);
				cnt--;	/* Remainig number of chars to send */
			}
		} else {
			/* Completion of sending or No sending */
			ENB_RXINT(li);	/* Enable the rcv interrupt only */
		}
		/* Send (completion)-notification event occurs */
		if (cnt < li->ou_cnt) {
			tk_set_flg(li->flg, FLG_OU_NORM);
			li->ou_cnt = 0;
		}
	} else {
		/* Disable the snd interrupt
			when the send is unavailable in "Tx Ready" status */
		if ((lsts & FR_TXFE) != 0) ENB_RXINT(li);
	}
   }

   /* Interrupt clear */
   OutW(regICR, mis);
  }

	/* Interrupt post-processing */
	end_inthdr(scdefs);
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
		DIS_TXRXINT(li);

		/* Release the interrupt handler */
		delete_inthdr(scdefs, sio_inthdr);
	}

	/* Save the new configuration */
	scdefs->c = *conf;
	li->mode = mode;

	if ( scdefs->c.iostep > 0 ) {

		/* Disable the interrupt to the target port */
		li->enbint = 0;
		DIS_TXRXINT(li);

		/* Register the interrupt handler */
		err = regist_inthdr(scdefs, sio_inthdr);
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
 *	Set the line status
 */
LOCAL	RsStat	make_line_stat( LINE_INFO *li, UW lsts )
{
	RsStat	stat;
static	RsStat	stat0 = {0};

	stat = stat0;
	if ( (lsts & FR_RxOV) != 0 )	stat.BE = 1;
	if ( (lsts & DR_FE) != 0 )		stat.FE = 1;
	if ( (lsts & DR_OE) != 0 )		stat.OE = 1;
	if ( (lsts & DR_PE) != 0 )		stat.PE = 1;
	stat.XF = li->flow.rcvxoff;
	if ( (lsts & DR_BE) != 0 )		stat.BD = 1;
	if ( (lsts & FR_DSR) != 0 )		stat.DR = 1;
	if ( (lsts & FR_DCD) != 0 )		stat.CD = 1;
	if ( (lsts & FR_CTS) != 0 )		stat.CS = 1;
///	if ( (li->msts & MS_RI) != 0 )	stat.CI = 1;

	return stat;
}

/*
 *	Send the initial one character.
 *	It shall be called in the interrupt-disabled status."True" shall be
 *	returned when it can be sent.
 *	It shall be sent from the send buffer when "c < 0".
 *	In addition, it shall be sent to FIFO buffer size when using FIFO.
 */
LOCAL	BOOL	send_1st_char( LINE_INFO *li, W c )
{
	SC_DEFS		*scdefs = &li->scdefs;
///	UW		lsts, s;
	UW		lsts;
	W		n;

	/* Unable to send during a "XOFF" */
	if ( li->flow.rcvxoff != 0 ) return FALSE;

	/* Get the line status and the modem status */
	lsts = InW(regFR);
	li->msts = InW(regCR);

	/* Record the line error */
///	if ( (s = lsts & LS_RxERR) != 0 ) {
///		li->lsts |= s;
///		li->lstshist |= s;
///	}

	/* Unable to send when "Tx Not Ready" status or "CS OFF" status */
	if ( (lsts & FR_TXFF) != 0
	  || (li->flow.csflow != 0 && (lsts & FR_CTS) == 0) ) return FALSE;

	if ( c < 0 ) {
		n = li->ou_wptr - li->ou_rptr;	/* Number of chars to send */
		if ( n <= 0 ) return FALSE;	/* There is no data to send */
		if ( scdefs->fctl != 0 ) {
			/* When FIFO is used */
			if ( n > FIFO_SIZE ) n = FIFO_SIZE;
						/* Limit to FIFO buffer size */
		} else {
			n = 1;	/* There is no FIFO */
		}
		c = li->ou_buf[OU_PTRMASK(li, li->ou_rptr++)];
	} else {
		n = 1;
	}

	/* Send */
	ENB_TXRXINT(li);		/* Enable the snd/rcv interrupts */
	OutW(regDR, c);
	while ( --n > 0 ) {
		lsts = InW(regFR);
		if (lsts & FR_TXFF) break;		/* Tx Full? */

		OutW(regDR, li->ou_buf[OU_PTRMASK(li, li->ou_rptr++)]);
	}
	if (n == 0)
		li->ou_cnt = 0;					/* 送信完了 */

	return TRUE;
}

/*
 *	One character input
 */
LOCAL	WRTN	recv_one_char( LINE_INFO *li, W tmout )
{
	SC_DEFS	*scdefs = &li->scdefs;
	UB	c;
	UINT	flgptn;
	UW	imask;
	ER	er;

	/* Wait for receiving while the receive buffer is empty */
	while ( li->in_wptr == li->in_rptr ) {

		if ( tmout == 0 ) return RTN_NONE; /* There is no wait */

		/* Wait for receiving or line status change */
		er = tk_wai_flg(li->flg, FLG_IN_WAIPTN,
				TWF_ORW | TWF_BITCLR, &flgptn, tmout);
		if ( er == E_TMOUT ) return RTN_TMOUT;
		if ( er < E_OK || (flgptn & FLG_IN_ABORT) ) return RTN_ABORT;
		if ( li->lsts != 0 ) return RTN_ERR; /* Receive error*/
	}

	/* Fetch the receive data */
	c = li->in_buf[li->in_rptr];
	li->in_rptr = PTRMASK(li, li->in_rptr + 1);

	/* Receive flow control */
	if ( (li->flowsts.rsoff || li->flowsts.sndxoff)
	   && IN_BUF_REMAIN(li) > XON_MARGIN ) {
		/* It is a receive-stop status.
			However, the receive-buffer has some empty space */
		if ( li->flowsts.rsoff ) {
			/* "RS" flow control */
			DI(imask);
			ON_RTS;
			li->flowsts.rsoff = 0;
			EI(imask);
		}
		if ( li->flowsts.sndxoff ) {
			/* "XOFF" flow control */
			DI(imask);
			if ( !send_1st_char(li, XON) ) {
				li->flowsts.reqchar = XON; 
					/* "XON" reservation for sending */
			}
			li->flowsts.sndxoff = 0;
			EI(imask);
		}
	}

	return c;
}

/*
 *	Input from serial port
 */
LOCAL	ER	pl011_in( LINE_INFO *li, UB *buf, W len, W *alen, W tmout )
{
	SC_DEFS		*scdefs = &li->scdefs;
	W		c;
	UW		rsz = 0;
	RsErr		err;

	if ( scdefs->c.iostep <= 0 ) return E_NOMDA; 
				/* Hardware configuration is not set yet */

	/* Execute the special processing
			to avoid deadlock at the time of polling */
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

	if ( len <= 0 ) {
		/* Check the number of the received bytes */
		rsz = IN_BUF_SIZE(li);
	} else {
		li->lsts = 0;  /* Clear the error status */

		/* Receive the data */
		for ( rsz = 0; rsz < len; rsz++ ) {

			c = recv_one_char(li, tmout);
			if ( c < RTN_OK ) {
				if ( c == RTN_NONE ) {
					err.w = ( rsz > 0 )? E_OK: E_BUSY;
				} else {
					err.s = make_line_stat(li, li->lsts);
					err.c.ErrorClass = MERCD(E_IO);
					if ( c == RTN_TMOUT ) err.c.Timout = 1;
					if ( c == RTN_ABORT ) err.c.Aborted= 1;
				}
				break;
			}
			buf[rsz] = c;
		}
	}

	/* Clear the "ABORT" flag */
	tk_clr_flg(li->flg, ~(FLG_IN_ABORT | FLG_OU_ABORT));

	/* Release the lock */
	consMUnlock(&li->lock, IN_LOCK);

EEXIT:

	*alen = rsz;  /* The number of the actually received bytes */
	return err.w;
}

/*
 *	Output to the serial port
 */
LOCAL	ER	pl011_out( LINE_INFO *li, UB *buf, W len, W *alen, W tmout )
{
	SC_DEFS		*scdefs = &li->scdefs;
	RsErr		err;
	W		sz, n;
	RTN		r;
	UW		wptr, ptr, imask;
	UINT		flgptn;
	ER		er;

	if ( scdefs->c.iostep <= 0 ) return E_NOMDA;
				/* Hardware configuration is not set yet */

	*alen = 0;

	/* Console output processing lock */
	if ( consMLock(&li->lock, OU_LOCK) < E_OK ) {
		err.s = make_line_stat(li, 0);
		err.c.ErrorClass = MERCD(E_IO);
		err.c.Aborted = 1;
		goto EEXIT;
	}

	err.w = E_OK;

	/*
	 * An output handler is read by the interrupt handler, so the page-out
	 * shall not be executed.
	 * Therefore, the resident memory shall be used as the output buffer
	 * instead of using the user-specified buffer "buffer", and the output
	 * data shall be copied.
	 */

	for (wptr = li->ou_wptr; len > 0; ) {

		/* Copy the send data to the output buffer */
		sz = li->ou_rptr + OUBUFSZ - li->ou_wptr;
		if (sz > len) sz = len;
		n = OUBUFSZ - (ptr = OU_PTRMASK(li, li->ou_wptr));
		if (n > sz) n = sz;
		if (n > 0)  MEMCPY(&li->ou_buf[ptr], buf, n);
		if (sz > n) MEMCPY(&li->ou_buf[0], &buf[n], sz - n);
		buf += sz;
		len -= sz;

		DI(imask);
		if (len <= 0) {
			/* All the send data is copied to the output buffer.
				Therefore, notification shall be received
				when it is completed */
			n = 1;
			if (tmout == 0) n = 0;
				/* Do not wait for the completion of sending */

		} else {
			/* The all send-data can not be copied to the output
				buffer, so notification shall be received when
				an empty space is available */
			if ((n = OUBUFSZ - len) < 20) n = 20;
		}
		li->ou_cnt = n;
		li->ou_wptr += sz;

		/* While no sending is executed, the one character shall be
			first sent to cause the send interrupt  */
///		if ((InW(regIMSC) & IMSC_Send) == 0) send_1st_char(li, -1);
		if ((InW(regFR) & FR_TXFF) == 0) send_1st_char(li, -1);

		EI(imask);

		/* Wait-for-completion of sending */
		for (r = RTN_OK; li->ou_cnt != 0; ) {
			ptr = li->ou_rptr;
			er = tk_wai_flg(li->flg, FLG_OU_WAIPTN,
					TWF_ORW | TWF_BITCLR, &flgptn, tmout);
			if ( er == E_TMOUT ) {
				/* Time-out error shall occur only when even
					one character has not been sent from
					the previous time. */
				if ( ptr == li->ou_rptr ) {
					r = RTN_TMOUT;
					break;
				}
			} else if ( er < E_OK || (flgptn & FLG_OU_ABORT) ) {
				r = RTN_ABORT;
				break;
			}
		}
		if ( r < RTN_OK ) {
			err.s = make_line_stat(li, 0);
			err.c.ErrorClass = MERCD(E_IO);
			if ( r == RTN_TMOUT ) err.c.Timout  = 1;
			if ( r == RTN_ABORT ) err.c.Aborted = 1;
			DI(imask);
			li->ou_wptr = li->ou_rptr;	/* Stop sending */
			EI(imask);
			break;
		}
	}

	*alen = li->ou_wptr - wptr;	/* Actual already sent byte */
	li->ou_cnt = 0;

	/* Clear the "ABORT" flag */
	tk_clr_flg(li->flg, ~(FLG_IN_ABORT | FLG_OU_ABORT));

	/* Release the lock */
	consMUnlock(&li->lock, OU_LOCK);

EEXIT:

	return err.w;
}

/*
 *	Control the serial port
 */
LOCAL	ER	pl011_ctl( LINE_INFO *li, W kind, UW *arg )
{
	SC_DEFS		*scdefs = &li->scdefs;
	ER		err = E_OK;
	UW		imask;
	W		n;

	if ( !(kind == DN_RS16450 || kind == -DN_RS16450)
		&& scdefs->c.iostep <= 0 ) return E_NOMDA;
				/* Hardware configuration is not set yet */

	/* Lock is unnecessary */

	switch ( kind ) {
	  case RS_ABORT:	/* Abort (release wait) */
		tk_set_flg(li->flg, FLG_IN_ABORT | FLG_OU_ABORT); /* Release */
		break;

	  case RS_SUSPEND:	/* Transit to the suspend status  */
	  case RS_RESUME:	/* Return from the suspend status */
		/* Debug port shall not transit to the "suspend"
						status in debug mode */
		if ( !(isDebugPort(li) && _isDebugMode()) ) {
			if (kind == RS_RESUME) sio_susres(li, 1);

			/* Reset controller */
			li->suspend = (kind == RS_SUSPEND) ? 1 : 0;
			err = setHwConf(li, &li->scdefs.c, li->mode, FALSE);

			if (kind == RS_SUSPEND) sio_susres(li, 0);
		}
		break;

	  case DN_RSMODE:	/* Set the communication mode */
		err = init_sio(li, *(RsMode*)arg);
		if ( err == E_OK ) {
			/* Initialize the serial line management information */
			DI(imask);
			li->in_rptr = li->in_wptr = 0;
			li->ou_rptr = li->ou_wptr = 0;
			li->ou_cnt = li->ou_len = 0;
			li->flow = li->scdefs.flow;
			li->flowsts = li->scdefs.flowsts;
			li->lsts = li->lstshist = 0;
			li->mode = *(RsMode*)arg;
			EI(imask);
		}
		break;

	  case -DN_RSMODE:	/* Get the communication mode */
		*(RsMode*)arg = li->mode;
		break;

	  case DN_RSFLOW:	/* Set the flow control */
		li->flow = *(RsFlow*)arg;
		break;

	  case -DN_RSFLOW:	/* Set and get the flow control */
		*(RsFlow*)arg = li->flow;
		break;

	  case -DN_RSSTAT:	/* Get the line status */
		*(RsStat*)arg = make_line_stat(li, li->lstshist);
		li->lstshist = 0;
		break;

	  case DN_RSBREAK:	/* Send break signal */
		if ( (n = *arg) > 0 ) {
			DI(imask);  ON_BREAK;  EI(imask);
			tk_dly_tsk(n);	/* Wait */
			DI(imask);  OFF_BREAK; EI(imask);
		}
		break;

	  case RS_RCVBUFSZ:	/* Set the receive-buffer */
		if ( (n = *arg) < MIN_INBUFSZ ) {
			err = E_PAR;
			break;
		}
		if ( n != li->in_bufsz ) {
			UB *new, *old;
			new = Malloc(n);
			if ( new != NULL ) {
				DI(imask);
				old = li->in_buf;
				li->in_buf = new;
				li->in_rptr = li->in_wptr = 0;
				li->in_bufsz = n;
				EI(imask);
				Free(old);
			} else {
				err = E_NOMEM;
			}
		}
		break;

	  case -RS_RCVBUFSZ:	/* Get the receive-buffer size */
		*arg = li->in_bufsz;
		break;

	  case RS_LINECTL:	/* Set the "ON/OFF" of control line */
		err = line_ctl(li, *arg);
		break;

	  case RS_EXTFUNC:	/* Set the external processing function */
		if ( li->extfn != NULL && (FUNCP)arg[0] != NULL ) {
			err = E_OBJ;  /* Already set */
		} else {
			DI(imask);
			li->extfn  = (FUNCP)arg[0];
			li->extpar = arg[1];
			EI(imask);
		}
		break;

	  case DN_RS16450:	/* Set the hardware configuration(for 16450) */
		err = setHwConf(li, (RsHwConf_16450*)arg, li->mode, FALSE);
		break;

	  case -DN_RS16450:	/* Get the hardware configuration(for 16450) */
		*(RsHwConf_16450*)arg = scdefs->c;
		break;

	  default:
		err = E_PAR;
	}

	return err;
}

/*
 *	Exit the serial port
 */
LOCAL	void	pl011_down( LINE_INFO *li )
{
	SC_DEFS		*scdefs = &li->scdefs;

	if ( scdefs->c.iostep > 0 ) {
		li->enbint = 0;
		DIS_TXRXINT(li);
		OutW(regCR, 0);					/* UART stop */
	}
}

/*
 *	Start up the serial port
 */
LOCAL	void	pl011_up( LINE_INFO *li )
{
	li->suspend = 0;
	li->flow = li->scdefs.flow;
	setHwConf(li, &li->scdefs.c, li->scdefs.mode, TRUE);
}


/*----------------------------------------------------------------------
#|History of "pl011.c"
#|=======================
#|* 2016/07/25	It's made by making reference to "ns16450.c" for [tef_em1d].
#|* 2016/07/30	sio_inthdr()はEXPORT→LOCALに変更
#|
*/
