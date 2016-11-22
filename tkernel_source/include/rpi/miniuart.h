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
 *    Modified by T.Yokobayashi at 2016/08/27.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)miniuart.h Console/Low-level serial I/O driver (BCM283x) 2016/09/07
 *
 *	General definition of serial controller (mini UART)
 */

/* Individual definition is required for "_IOADR(n)" and "_CLOCK (Hz)" */

/* I/O port address */
#define	regAUXIRQ	_IOADR(0)	/* 00:Auxiliary Interrupt status (R) */
#define	regAUXENB	_IOADR(1)	/* 04:Auxiliary enables (RW) */

#define	regMU_IO	_IOADR(16)	/* 40:Mini Uart I/O Data (RW) */
#define	regMU_IER	_IOADR(17)	/* 44:Mini Uart Interrupt Enable (RW) */
#define	regMU_IIR	_IOADR(18)	/* 48:Mini Uart Interrupt Identify (RW) */
#define	regMU_LCR	_IOADR(19)	/* 4C:Mini Uart Line Control (RW) */
#define	regMU_MCR	_IOADR(20)	/* 50:Mini Uart Modem Control (RW) */
#define	regMU_LSR	_IOADR(21)	/* 54:Mini Uart Line Status (R) */
#define	regMU_MSR	_IOADR(22)	/* 58:Mini Uart Modem Status (R) */
#define	regMU_SCRATCH _IOADR(23)	/* 5C:Mini Uart Scratch (RW) */
#define	regMU_CNTL	_IOADR(24)	/* 60:Mini Uart Control (RW) */
#define	regMU_STAT	_IOADR(25)	/* 64:Mini Uart Extra Status (R) */
#define	regMU_BAUD	_IOADR(26)	/* 68:Mini Uart Baudrate (RW) */



/* Communication speed -> frequency dividing count value */
//#define	MU_LINE_SPEED(bps)	( ((_CLOCK) / 8 / (bps)) - 1 )
#define	MU_LINE_SPEED(bps)	 ( (((_CLOCK)/8) + ((bps)/2)) / bps - 1 ) /* 四捨五入 */


/* regAUXIRQ (R), Auxilialy Interrupt status register */
#define	AUXIRQ_SPI2IRQ	0x0004	/* SPI2 interrupt pending */
#define	AUXIRQ_SPI1IRQ	0x0002	/* SPI1 interrupt pending */
#define	AUXIRQ_MUIRQ	0x0001	/* mini UART interrupt pending */


/* regAUXENB (RW), Auxilialy enables register */
#define	AUXENB_SPI2ENB	0x0004	/* SPI2 module enable */
#define	AUXENB_SPI1ENB	0x0002	/* SPI1 module enable */
#define	AUXENB_MUENB	0x0001	/* mini UART module enable */


/* regMU_IO (RW), I/O register */


/* regMU_IER (RW), Interrupt Enable register */
#define	IM_MSTS		0x00		/* 0x08, unsupported */
#define	IM_LSTS		0x00		/* 0x04, unsupported */
#define	IM_SND		0x02		/* Send-enabled interrupt */
#define	IM_RCV		0x01		/* Receive-data-enabled interrupt */


/* regMU_IIR (RW), Interrupt Identify register */
#define	IS_FIFO		0xC0		/* FIFO during use */

#define	IS_RCV		0x04		/* ID=2 rcv-data-enabled interrupt */
#define	IS_SND		0x02		/* ID=1 send-enabled interrupt  */
#define	IS_PEND		0x01		/* Interrupt pending */

#define	FC_TXCLR	0x04		/* Send FIFO clear  */
#define	FC_RXCLR	0x02		/* Receive FIFO clear*/


/* regMU_LCR (RW), Line control register */
#define	LC_DLAB		0x80		/* Frequency dividing counter access */
#define	LC_SBRK		0x40		/* Break sending */

#define	LC_BLEN		0x03		/* The number of data bits(bit 1 must be set) */


/* regMU_MCR (RW), Modem control register */
#define	MC_OUT2		0x00		/* 0x08, unsupported */

#define	MC_RTS		0x02		/* Request To Send */
#define	MC_DTR		0x00		/* 0x01, unsupported */


/* regMU_LSR (R), Line status register */
#define	LS_TSRE		0x40		/* Send shift register is empty */
#define	LS_THRE		0x20		/* Send hold register is empty*/

#define	LS_OERR		0x02		/* Overrun error */
#define	LS_DRDY		0x01		/* There is the receive data */
/* Additional definition for driver internal processing */
#define	LS_RXOV		0x80		/* Receive-buffer overflow */

#define	LS_RxERR	(LS_OERR)

/* regMU_MSR (R), Modem status register*/
#define	MS_CS		0x10		/* Clear To Send */


/* regMU_SCRATCH (RW), Scratch register*/


/* regMU_CNTL (RW), Extra Control register*/
#define	CNTL_CTSLV	0x80		/* CTS assert level */
#define	CNTL_RTSLV	0x40		/* RTS assert level */
#define	CNTL_RTSAT	0x30		/* RTS AUTO flow level */
#define	CNTL_ENCTS	0x08		/* Enable transmit Auto flow-control using CTS */
#define	CNTL_ENRTS	0x04		/* Enable receive Auto flow-control using RTS */
#define	CNTL_TXENB	0x02		/* Transmitter enable */
#define	CNTL_RXENB	0x01		/* Receiver enable */


/* regMU_STAT (R), Extra Status register*/
#define	STAT_TXFIFO	0x0F000000	/* Transmit FIFO fill level */
#define	STAT_RXFIFO	0x000F0000	/* Receive FIFO fill level */
#define	STAT_TXDONE	0x00000100	/* Transmitter done */
#define	STAT_CTS	0x00000080	/* CTS line */
#define	STAT_RTS	0x00000040	/* RTS status */
#define	STAT_TXFFUL	0x00000020	/* Transmit FIFO full */
#define	STAT_OERR	0x00000010	/* Receiver overrun(MU_LSR-bit1) */
#define	STAT_TXIDL	0x00000008	/* Transmitter idle */
#define	STAT_RXIDL	0x00000004	/* Recceiver idle */
#define	STAT_TXAVL	0x00000002	/* Space available */
#define	STAT_RXAVL	0x00000001	/* Symbol available */


/* regMU_BAUD (RW), Baudrate register */


#define	FC_FIFO		IS_FIFO		/* Enable FIFO */
#define	FIFO_SIZE	8			/* FIFO size */


/*----------------------------------------------------------------------
#|History of "miniuart.h"
#|=======================
#|* 2016/08/27	It's made by making reference to "ns16450.h" for [tef_em1d].
#|* 2016/09/06	LC_BLEN 0x01→0x03 bit1は必ず1にする事(正誤表に記述あり)
#|
*/
