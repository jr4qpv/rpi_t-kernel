/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/03/01.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/07/25.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)pl011.h Console/Low-level serial I/O driver (BCM283x) 2016/07/30
 *
 *	General definition of serial controller (UART/PL011)
 */

/* Individual definition is required for "_IOADR(n)" and "_CLOCK (Hz)" */

/* I/O port address */
#define	regDR		_IOADR(0)	/* 00:data register          (RW) */
#define	regRSRECR	_IOADR(1)	/* 04:recive statas/error clear register (RW) */

#define	regFR		_IOADR(6)	/* 18:flag register          (RW) */

#define	regILPR		_IOADR(8)	/* 20:not in use     */
#define	regIBRD		_IOADR(9)	/* 24:integer baud rate divisor (RW) */
#define	regFBRD		_IOADR(10)	/* 28:fractional baud rate divisor (RW) */
#define	regLCRH		_IOADR(11)	/* 2c:line control register (RW) */

#define	regCR		_IOADR(12)	/* 30:control register (RW) */
#define	regIFLS		_IOADR(13)	/* 34:interupt FIFO level select (RW) */
#define	regIMSC		_IOADR(14)	/* 38:interupt mask set clear (RW) */
#define	regRIS		_IOADR(15)	/* 3c:raw interupt status (RW) */

#define	regMIS		_IOADR(16)	/* 40:mask interupt  status (RW) */
#define	regICR		_IOADR(17)	/* 44:interupt clear register (RW) */
#define	regDMACR	_IOADR(18)	/* 48:DMA control register (RW) */

#define	regITCR		_IOADR(32)	/* 80:test control register (RW) */
#define	regITIP		_IOADR(33)	/* 84:integration test input reg (RW) */
#define	regITOP		_IOADR(34)	/* 88:integration test output reg (RW) */
#define	regTDR		_IOADR(35)	/* 8c:test data reg (RW) */


/* Communication speed -> divided counter value.(ask Q8) */
#define	SC_LINE_SPEED(bps)	( ((_CLOCK/16*0x100) + (bps/2)) / (bps) )



/* regDR (RW), data register */
#define	DR_OE		0x0800		/* Overrun error */
#define	DR_BE		0x0400		/* Break error */
#define	DR_PE		0x0200		/* Parity error */
#define	DR_FE		0x0100		/* Framing error */
#define	DR_DATA		0x00FF		/* Receive/Transmit data */

#define	DR_RxERR	(DR_OE|DR_BE|DR_PE|DR_FE)


/* regRSRECR (RW), receive status register/error clear register */
#define	RSRECR_OE	0x0008		/* Overrun error */
#define	RSRECR_BE	0x0004		/* Break error */
#define	RSRECR_PE	0x0002		/* Parity error */
#define	RSRECR_FE	0x0001		/* Framing error */


/* regFR (RW), Flag register */
#define	FR_RI		0x0000		/* 0x0100, unsupported */

#define	FR_TXFE		0x0080		/* transmit FIFO empty */
#define	FR_RXFF		0x0040		/* receive FIFO full */
#define	FR_TXFF		0x0020		/* transmit FIFO full */
#define	FR_RXFE		0x0010		/* receive FIFO empty */
#define	FR_BUSY		0x0008		/* UART busy */
#define	FR_DCD		0x0004		/* unsupported */
#define	FR_DSR		0x0002		/* unsupported */
#define	FR_CTS		0x0001		/* clear to send */

/* Additional definition for driver internal processing */
#define	FR_RxOV		0x8000		/* Receive-buffer overflow */


/* regLCRH (RW), Line control register */
#define	LCRH_SPS	0x80		/* Stic parity select(RO) */
#define	LCRH_WLEN	0x60		/* Word length */
#define	LCRH_FEN	0x10		/* Enable FIFOs */
#define	LCRH_STP2	0x08		/* Two stop bits select */
#define	LCRH_EPS	0x04		/* Even parity select */
#define	LCRH_PEN	0x02		/* Parity enable */
#define	LCRH_BRK	0x01		/* Send break */

#define	LCRH_NoParity	(0x00)
#define	LCRH_EvenParity	(LCRH_PEN|LCRH_EPS)
#define	LCRH_OddParity	(LCRH_PEN)

#define	LCRH_8bitData	(0x60)
#define	LCRH_7bitData	(0x40)
#define	LCRH_6bitData	(0x20)
#define	LCRH_5bitData	(0x00)


/* regCR (RW), Control register */
#define	CR_CTSEN	0x8000		/* CTS hardware flow control enable */
#define	CR_RTSEN	0x4000		/* RTS hardware flow control enable */
#define	CR_OUT2		0x0000		/* 0x2000, unsupported(RO) */
#define	CR_OUT1		0x0000		/* 0x1000, unsupported(RO) */
#define	CR_RTS		0x0800		/* Request to send */
#define	CR_DTR		0x0000		/* 0x0400, unsupported(RO) */
#define	CR_RXE		0x0200		/* Receive enable */
#define	CR_TXE		0x0100		/* Transmit enable */
#define	CR_LBE		0x0080		/* Loopback enable */

#define	CR_SIRLP	0x0000		/* 0x0004, unsupported */
#define	CR_SIREN	0x0000		/* 0x0002, unsupported */
#define	CR_UARTEN	0x0001		/* UART enable */


/* regIFLS (RW), FIFO level select register */
								/* IFLS_RXIFPSEL unsupported */
								/* IFLS_TXIFPSEL unsupported */
#define	IFLS_RXIFLSEL	0x0038	/* Receive interrupt FIFO level select */
#define	IFLS_TXIFLSEL	0x0007	/* Transmit interrupt FIFO level select */

#define	IFLS_RxIF_1by8	0x0000	/* Rx FIFO becomes 1/8 full */
#define	IFLS_RxIF_1by4	0x0008	/* Rx FIFO becomes 1/4 full */
#define	IFLS_RxIF_1by2	0x0010	/* Rx FIFO becomes 1/2 full */
#define	IFLS_RxIF_3by4	0x0018	/* Rx FIFO becomes 3/4 full */
#define	IFLS_RxIF_7by8	0x0020	/* Rx FIFO becomes 7/8 full */

#define	IFLS_TxIF_1by8	0x0000	/* Tx FIFO becomes 1/8 full */
#define	IFLS_TxIF_1by4	0x0001	/* Tx FIFO becomes 1/4 full */
#define	IFLS_TxIF_1by2	0x0002	/* Tx FIFO becomes 1/2 full */
#define	IFLS_TxIF_3by4	0x0003	/* Tx FIFO becomes 3/4 full */
#define	IFLS_TxIF_7by8	0x0004	/* Tx FIFO becomes 7/8 full */

/* Additional definition for driver internal processing */
#define	FIFO_Enable		0x8000	/* fctl FIFO use flag */
#define	FIFO_SIZE		16		/* FIFO size */


/* regIMSC (RW), interrupt mask set/clear register */
#define	IMSC_OEIM	0x0400		/* Overrun error interrupt mask */
#define	IMSC_BEIM	0x0200		/* Break error interrupt mask */
#define	IMSC_PEIM	0x0100		/* Parity error interrupt mask */
#define	IMSC_FEIM	0x0080		/* Framing error interrupt mask */
#define	IMSC_RTIM	0x0040		/* Recive timeout interrupt mask */
#define	IMSC_TXIM	0x0020		/* Transmit interrupt mask */
#define	IMSC_RXIM	0x0010		/* Receive interrupt mask */
								/* DSRMIM unsupported */
								/* DCDMIM unsupported */
#define	IMSC_CTSMIM	0x0002		/* UARTCTS modem interrupt mask */
								/* RIMIM unsupported */

#define	IMSC_Recv	(IMSC_RXIM|IMSC_RTIM)
#define	IMSC_Send	(IMSC_TXIM)


/* regRIS (RW), raw interrupt status register */
#define	RIS_OERIS	0x0400		/* Overrun error interrupt status */
#define	RIS_BERIS	0x0200		/* Break error interrupt status */
#define	RIS_PERIS	0x0100		/* Parity error interrupt status */
#define	RIS_FERIS	0x0080		/* Framing error interrupt status */
#define	RIS_RTRIS	0x0040		/* Recive timeout interrupt status */
#define	RIS_TXRIS	0x0020		/* Transmit interrupt status */
#define	RIS_RXRIS	0x0010		/* Receive interrupt status */
								/* DSRRMIS unsupported */
								/* DCDRMIS unsupported */
#define	RIS_CTSRMIS	0x0002		/* UARTCTS modem interrupt status */
								/* RIRMIS unsupported */


/* regMIS (RW), masked interrupt status register */
#define	MIS_OEMIS	0x0400		/* Overrun error masked interrupt status */
#define	MIS_BEMIS	0x0200		/* Break error masked interrupt status */
#define	MIS_PEMIS	0x0100		/* Parity error masked interrupt status */
#define	MIS_FEMIS	0x0080		/* Framing error masked interrupt status */
#define	MIS_RTMIS	0x0040		/* Recive timeout masked interrupt status */
#define	MIS_TXMIS	0x0020		/* Transmit masked interrupt status */
#define	MIS_RXMIS	0x0010		/* Receive masked interrupt status */
								/* DSRMMIS unsupported */
								/* DCDMMIS unsupported */
#define	MIS_CTSMMIS	0x0002		/* UARTCTS modem masked interrupt status */
								/* RIMMIS unsupported */


/* regICR (RW), interrupt clear register */
#define	ICR_OEIC	0x0400		/* Overrun error interrupt clear */
#define	ICR_BEIC	0x0200		/* Break error interrupt clear */
#define	ICR_PEIC	0x0100		/* Parity error interrupt clear */
#define	ICR_FEIC	0x0080		/* Framing error interrupt clear */
#define	ICR_RTIC	0x0040		/* Recive timeout interrupt clear */
#define	ICR_TXIC	0x0020		/* Transmit interrupt clear */
#define	ICR_RXIC	0x0010		/* Receive masked interrupt status */
								/* DSRMIC unsupported */
								/* DCDMIC unsupported */
#define	ICR_CTSMIC	0x0002		/* UARTCTS modem masked interrupt clear */
								/* RIMIC unsupported */

#define	ICR_ALL		0x07F2		/* all bits */


/* regITCR (RW), Test Control register */
#define	ITCR_ITIP1	0x0002		/* Test FIFO enable */
#define	ITCR_ITIP0	0x0001		/* Integration test enable */


/* regITIP (RW), Test Control register */
#define	ITIP_ITIP3	0x0004		/* Reads return, UARTCTS primary input */
#define	ITIP_ITIP0	0x0001		/* Reads return, UARTRTX primary input */


/* regTDR (RW), test data register */


/*----------------------------------------------------------------------
#|History of "pl011.h"
#|=======================
#|* 2016/07/25	It's made by making reference to "ns16450.h" for [tef_em1d].
#|
*/
