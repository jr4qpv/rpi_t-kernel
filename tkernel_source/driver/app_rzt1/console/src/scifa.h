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
 *	scifa.h	Console/Low-level serial I/O driver
 *
 *	General definition of serial controller (RZT1 SCIFA)
 */


#define	SERICLK		150000000	/* SERICLK[Hz]…150MHz */


// SCSMRx
#define	SCSMRx_CKS_1	0x00	// Clock select - SERICLK /  1
#define	SCSMRx_CKS_4	0x01	// Clock select - SERICLK /  4
#define	SCSMRx_CKS_16	0x02	// Clock select - SERICLK / 16
#define	SCSMRx_CKS_64	0x03	// Clock select - SERICLK / 64

#define	SCSMRx_STOP		0x08	// Stop bit length - 0:1bit 1:2bit
#define	SCSMRx_PM_ODD	0x10	// Parity mode - 0:Even 1:Odd
#define	SCSMRx_PE		0x20	// Parity enable - 0:Dis 1:En
#define	SCSMRx_CHR		0x40	// Character length - 0:8bit 1:7bit
#define	SCSMRx_CM		0x80	// Communication mode - 0:Async 1:Clock sync


// SCSCRx
//#define	SCSCRx_CKE	0x01	// b1-b0 clock enable 00:Dis 01:En
#define	SCSCRx_TEIE		0x04	// Tx end interrupt enable - 0:Dis 1:En
#define	SCSCRx_REIE		0x08	// Rx error interrupt - 0:Dis 1:En
#define	SCSCRx_RE		0x10	// Rx enable - 0:Dis 1:En
#define	SCSCRx_TE		0x20	// Tx enable - 0:Dis 1:En
#define	SCSCRx_RIE		0x40	// Rx interrupt enable - 0:Dis 1:En
#define	SCSCRx_TIE		0x80	// Tx interrupt enable - 0:Dis 1:En


// SCFSRx
#define	SCFSRx_DR		0x01	// Rx ready - 0:data rcvd. 1:none
#define	SCFSRx_RDF		0x02	// Rx FIFO full - 0:empty 1:filled
#define	SCFSRx_PER		0x04	// Parity error - 0:none 1:error
#define	SCFSRx_FER		0x08	// Framing error - 0:none 1:error
#define	SCFSRx_BRK		0x10	// Break detect - 0:none 1:detected
#define	SCFSRx_TDFE		0x20	// Tx FIFO empty - 0:filled 1:empty
#define	SCFSRx_TEND		0x40	// Tx end - 0:busy 1:completed
#define	SCFSRx_ER		0x80	// Rx error - 0:none 1:error

#define	SCFSRx_ERR_MASK	(SCFSRx_ER | SCFSRx_BRK | SCFSRx_FER | SCFSRx_PER)

// SCFCRx
#define	SCFCRx_LOOP		0x01	// Loopback test - 0:Dis 1:En
#define	SCFCRx_RFRST	0x02	// Rx FIFO reset - 0:Dis 1:En
#define	SCFCRx_TFRST	0x04	// Tx FIFO reset - 0:Dis 1:En
#define	SCFCRx_MCE		0x08	// Modem control enable - 0:Dis 1:En
#define	SCFCRx_TTRG_8	0x00	// Tx FIFO trigger - 8(8)byte
#define	SCFCRx_TTRG_4	0x10	// Tx FIFO trigger - 4(12)byte
#define	SCFCRx_TTRG_2	0x20	// Tx FIFO trigger - 2(14)byte
#define	SCFCRx_TTRG_0	0x30	// Tx FIFO trigger - 0(16)byte
#define	SCFCRx_RTRG_1	0x00	// Rx FIFO trigger - 1byte
#define	SCFCRx_RTRG_4	0x40	// Rx FIFO trigger - 4byte
#define	SCFCRx_RTRG_8	0x80	// Rx FIFO trigger - 8byte
#define	SCFCRx_RTRG_14	0xc0	// Rx FIFO trigger - 14byte

#define	SCFCRx_RSTRG_15	0x0000	// RTS active trigger - 15byte
#define	SCFCRx_RSTRG_1	0x0100	// RTS active trigger - 1byte
#define	SCFCRx_RSTRG_4	0x0200	// RTS active trigger - 4byte
#define	SCFCRx_RSTRG_6	0x0300	// RTS active trigger - 6byte
#define	SCFCRx_RSTRG_8	0x0400	// RTS active trigger - 8byte
#define	SCFCRx_RSTRG_10	0x0500	// RTS active trigger - 10byte
#define	SCFCRx_RSTRG_12	0x0600	// RTS active trigger - 12byte
#define	SCFCRx_RSTRG_14	0x0700	// RTS active trigger - 14byte

#define	SCFCRx_TTRG_SZ	8		// トリガ設定数


// SCFDRx
///#define	SCFDRx_T_MASK	0x1f00	// 有効なbit位置
///#define	SCFDRx_T_SIZE	16		// FIFOサイズ

#define	SCFDRx_R_MASK	0x001f	// 有効なbit位置
#define	SCFDRx_R_SIZE	16		// FIFO数サイズ


/*----------------------------------------------------------------------
#|History of "scifa.h"
#|=======================
#|* 2016/07/12	[app_rzt1]用に、[tef_em1d]の"ns16450.h"を参考に作成。
#|
*/
