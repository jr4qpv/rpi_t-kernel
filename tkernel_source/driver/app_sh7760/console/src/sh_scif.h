/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2015 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2015/12/24.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sh_scif.h	Console/Low-level serial I/O driver
 *
 *	General definition of serial controller (SH7760 SCIF)
 */



#if 0	/*-----------------------------------------------------------------*/
//////     まだエラーがある!! //////

//#include <misc/sh7760.h>

/* Individual definition is required for "_IOB" */
/*
	シリアルコントローラ (SCIF) の定義
*/
#define	SCSMRx	_IOB(SCSMR.WORD)	/* シリアルモード				(H, R/W) */
#define	SCBRRx	_IOB(SCBRR.BYTE)	/* ビットレート 				(B, R/W) */
#define	SCSCRx	_IOB(SCSCR.WORD)	/* シリアルコントロール			(H, R/W) */
#define	SCFTDRx	_IOB(SCFTDR)		/* トランスミットFIFOデータ		(B, W)   */
#define	SCFSRx	_IOB(SCFSR.WORD)	/* シリアルステータス			(H, R/W) */
#define	SCFRDRx	_IOB(SCFRDR)		/* レシーブFIFOデータ			(B, R)   */
#define	SCFCRx	_IOB(SCFCR.WORD)	/* FIFOコントロール				(H, R/W) */
#define	SCTFDRx	_IOB(SCTFDR.WORD)	/* トランスミットFIFOデータ数	(H, R)   */
#define	SCRFDRx	_IOB(SCRFDR)		/* レシーブFIFOデータ数			(H, R)   */
#define	SCSPTRx	_IOB(SCSPTR.WORD)	/* シリアルポートデータ			(H, R/W) */
#define	SCLSRx	_IOB(SCLSR.WORD)	/* ラインステータス				(H, R/W) */
#define	SCRERx	_IOB(SCRER.WORD)	/* シリアルエラー				(H, R)   */

// I/O入出力マクロ
#define	InB(ix)			(ix)
#define	InH(ix)			(ix)
#define	OutB(ix, dt)	(ix = (dt))
#define	OutH(ix, dt)	(ix = (dt))

/* I/O port define */
#define	_IOB(xx)	((volatile struct st_scif *)(scdefs->c.iobase)->(xx))


#else	/*-----------------------------------------------------------------*/
/* Individual definition is required for "_IOADR(n)" */
/*
	シリアルコントローラ (SCIF) の定義
*/
#define	SCSMRx	_IOADR(0x00)	/* シリアルモード				(H, R/W)	*/
#define	SCBRRx	_IOADR(0x04)	/* ビットレート 				(B, R/W)	*/
#define	SCSCRx	_IOADR(0x08)	/* シリアルコントロール			(H, R/W)	*/
#define	SCFTDRx	_IOADR(0x0c)	/* トランスミットFIFOデータ		(B, W)		*/
#define	SCFSRx	_IOADR(0x10)	/* シリアルステータス			(H, R/W)	*/
#define	SCFRDRx	_IOADR(0x14)	/* レシーブFIFOデータ			(B, R)		*/
#define	SCFCRx	_IOADR(0x18)	/* FIFOコントロール				(H, R/W)	*/
#define	SCTFDRx	_IOADR(0x1c)	/* トランスミットFIFOデータ数	(H, R)		*/
#define	SCRFDRx	_IOADR(0x20)	/* レシーブFIFOデータ数			(H, R)		*/
#define	SCSPTRx	_IOADR(0x24)	/* シリアルポートデータ			(H, R/W)	*/
#define	SCLSRx	_IOADR(0x28)	/* ラインステータス				(H, R/W)	*/
#define	SCRERx	_IOADR(0x2c)	/* シリアルエラー				(H, R)		*/

// I/O入出力マクロ
#define	InB(ix)			in_b((ix))
#define	InH(ix)			in_h((ix))
#define	OutB(ix, dt)	out_b((ix), (dt))
#define	OutH(ix, dt)	out_h((ix), (dt))

/* I/O port address */
#define	_IOADR(n)	(scdefs->c.iobase + (n))
//#define	_IOADR(n)	(0xfe600000 + (n))

#endif	/*-----------------------------------------------------------------*/


// SCSMRx
#define	SCSMRx_CA		0x80	// Communication mode - 0:Async 1:Clock sync
#define	SCSMRx_CHR		0x40	// Character length - 0:8bit 1:7bit
#define	SCSMRx_PE		0x20	// Parity enable - 0:Dis 1:En
#define	SCSMRx_OE		0x10	// Parity mode - 0:Even 1:Odd
#define	SCSMRx_STOP		0x08	// Stop bit length - 0:1bit 1:2bit
#define	SCSMRx_CKS_1	0x00	// Clock select - Pck /  1
#define	SCSMRx_CKS_4	0x01	// Clock select - Pck /  4
#define	SCSMRx_CKS_16	0x02	// Clock select - Pck / 16
#define	SCSMRx_CKS_64	0x03	// Clock select - Pck / 64

// SCSCRx
#define	SCSCRx_TIE		0x80	// Tx interrupt enable - 0:Dis 1:En
#define	SCSCRx_RIE		0x40	// Rx interrupt enable - 0:Dis 1:En
#define	SCSCRx_TE		0x20	// Tx enable - 0:Dis 1:En
#define	SCSCRx_RE		0x10	// Rx enable - 0:Dis 1:En
#define	SCSCRx_REIE		0x08	// Rx error interrupt - 0:Dis 1:En
///#define	SCSCRx_CKE	0x01	// IrDA clock enable 00:Dis 01:En

// SCFSRx
#define	SCFSRx_ER		0x80	// Rx error - 0:none 1:error
#define	SCFSRx_TEND		0x40	// Tx end - 0:busy 1:completed
#define	SCFSRx_TDFE		0x20	// Tx FIFO empty - 0:filled 1:empty
#define	SCFSRx_BRK		0x10	// Break detect - 0:none 1:detected
#define	SCFSRx_FER		0x08	// Framing error - 0:none 1:error
#define	SCFSRx_PER		0x04	// Parity error - 0:none 1:error
#define	SCFSRx_RDF		0x02	// Rx FIFO full - 0:empty 1:filled
#define	SCFSRx_DR		0x01	// Rx ready - 0:data rcvd. 1:none

#define	SCFSRx_ERR_MASK	(SCFSRx_ER | SCFSRx_BRK | SCFSRx_FER | SCFSRx_PER)

// SCFCRx
#define	SCFCRx_RTRG_1	0x00	// Rx FIFO trigger - 1byte
#define	SCFCRx_RTRG_16	0x40	// Rx FIFO trigger - 16byte
#define	SCFCRx_RTRG_64	0x80	// Rx FIFO trigger - 64byte
#define	SCFCRx_RTRG_96	0xc0	// Rx FIFO trigger - 96byte
#define	SCFCRx_TTRG_64	0x00	// Tx FIFO trigger - 64byte
#define	SCFCRx_TTRG_32	0x10	// Tx FIFO trigger - 96byte
#define	SCFCRx_TTRG_4	0x20	// Tx FIFO trigger -124byte
#define	SCFCRx_TTRG_0	0x30	// Tx FIFO trigger -128byte
#define	SCFCRx_MCE		0x08	// Modem control enable - 0:Dis 1:En
#define	SCFCRx_TFRST	0x04	// Tx FIFO reset - 0:Dis 1:En
#define	SCFCRx_RFRST	0x02	// Rx FIFO reset - 0:Dis 1:En
#define	SCFCRx_LOOP		0x01	// Loopback test - 0:Dis 1:En

#define	SCFCRx_TTRG_SZ	64		// トリガ設定数


// SCTFDRx
#define	SCTFDRx_MASK	0x00ff	// 有効なbit位置
#define	SCTFDRx_SIZE	128		// FIFOサイズ

// SCRFDRx
#define	SCRFDRx_MASK	0x00ff	// 有効なbit位置
#define	SCRFDRx_SIZE	128		// FIFO数サイズ


/*----------------------------------------------------------------------*/
#if 0
#|【sh_scif.c 変更履歴】
#|□2015/12/24	[app_sh7760]用に、[tef_em1d]の"ns16450.h"を参考に作成。
#|
#endif
