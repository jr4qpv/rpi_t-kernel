/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(c) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)remote.c (libmisc) 2016/02/19
 *  メンテポート リモート制御 routine
 *
 * ［Note］
 *  ・ﾘﾓｰﾄ制御の仕様は｢VW15-3015 IOP-5ﾓﾆﾀｰ機能仕様書｣5.2項を参照。
 */

#include <basic.h>
#include <t2ex/errno.h>					/* for t2ex/string.h */
#include <t2ex/string.h>				/* for memset() */
///#include <sys/consio.h>
#include <misc/libmisc.h>



/********( 定数の定義 )********/
#define	RMT_CHK_COUNT		250			// 250×10m秒=2.5秒間


typedef	unsigned char	uchar;
typedef	unsigned short	ushort;
typedef	unsigned long	ulong;
////	.EXPORT	__REMOTE_INIT, __REMOTE_CHECK, __REMOTE_RX, __REMOTE_TX
////	.EXPORT	__IOBYTE_CHG




/************************************************
;    remote work area
;************************************************/
typedef struct t_rmtwk {
	uchar	rmt_chk;					// ﾘﾓｰﾄ受信停止検出ｶｳﾝﾀ
	char	rfu01;
	char	rx_mode;
	char	tx_mode;

	char	mon_dat;
	char	mon_cmd;

	char	tx_data[2];

	short	mon_seg;					// 現在は未使用(mon_baseに移行)
	short	mon_offs;
	short	mon_seg0;					// ﾁｪｯｸｻﾑ計算用
	short	mon_sum;

	long	mon_base;
	long	mon_cnt;

	long	rx_data_addr;
	long	tx_data_addr;
	long	rx_data_cnt;
	long	tx_data_cnt;
} T_RMTWK;

T_RMTWK	rmtwk[2];						// 0:作業領域の先頭(RS-232C用)
										// 1:CH#0通信用の作業領域


/************************************************
     remote routine code
 ************************************************/

/*================================================
;|
;|□_remote_init        《リモート制御の初期化》
;|  ［形式］#include <misc/libmisc.h>
;|          int _remote_init(void);
;|  ［引数］なし
;|  ［戻値］｢0｣固定
;|  ［Note］・割込み禁止状態で呼ばれる事
;|
 ================================================*/
EXPORT int _remote_init(void)
{
	int i;

	/*-------( ﾘﾓｰﾄ作業領域の初期化 )-------*/
	for (i=0; i<2; i++)
		memset(&rmtwk[i], 0, sizeof(T_RMTWK));

	return 0;
}


/*================================================
;|
;|□_remote_check       《リモート受信停止のチェック》
;|  ［形式］#include <misc/libmisc.h>
;|          int _remote_check(char mode);
;|  ［引数］mode…処理ﾓｰﾄﾞ(bit0:呼出し側IOBYTE)
;|  ［戻値］≧0：正常
;|          =-1：ﾘﾓｰﾄ受信停止を検出し、ﾘﾓｰﾄﾓｰﾄﾞを解除した
;|  ［Note］・割込み禁止状態で呼ばれる事
;|          ・本関数を定期的(10m秒毎)に呼出す事で、2.5秒間(250回)リモート受信
;|            が停止していたら、自動的にリモートモードを強制解除する。
;|
 ================================================*/
EXPORT int _remote_check(char mode)
{
	T_RMTWK	*wkp;

	wkp = &rmtwk[mode & 1];				// 作業領域の選択(bit0)

	/* rx_modeのチェック */
	if (wkp->rx_mode == 0)
		return 0;						// ﾘﾓｰﾄ処理中でないので無視(戻値｢0｣)

	/* rmt_chkのチェック */
	wkp->rmt_chk--;
	if (wkp->rmt_chk == 0) {
		wkp->rx_mode = 0;				// rx_modeをキャンセル
		return -1;						// リモート受信停止検出
	}

	return 0;
}


/*================================================
;|
;|□_remote_rx          《リモート制御の受信処理》
;|  ［形式］#include <misc/libmisc.h>
;|          int _remote_rx(int c, char mode);
;|  ［引数］c   …受信文字
;|          mode…処理ﾓｰﾄﾞ(bit0:呼出し側IOBYTE)
;|  ［戻値］≧0：通常文字（上位では｢戻値｣を通常文字として受信処理）
;|          =-1：ﾘﾓｰﾄ処理（上位では無視）
;|          =-2：ﾘﾓｰﾄ送信開始要求（上位では送信起動）
;|          =-3：送信文字制限解除（上位では送信制限の解除処理）
;|  ［Note］・割込み禁止状態で呼ばれる事
;|          ・ﾘﾓｰﾄ制御の仕様は｢VW15-3015 IOP-5ﾓﾆﾀｰ機能仕様書｣5.2項を参照
;|
 ================================================*/

/*-------( CMD=01 : ﾎｯﾄﾌﾞﾚｰｸ･･･CCP起動 )-------*/
LOCAL void rx_cmd01_proc(void)
{
#if 0	////////
	MOV	#CCP_EVF_NO,R4
	TRAPA	#GET_EVF
	CMP/EQ	#0,R0			;0 ?
	BF	REMOTE_RX_RET1		;ccpが起動中なら無視(to return)

;	( IOBYTEの引き込み処理 )
	MOV	R5,R4			;R4←IOBYTE指定
	STS	PR,R1			;save PR
	BSR	__IOBYTE_CHG		;(遅延分岐),IOBYTEの切換え処理
	NOP
	LDS	R1,PR			;restore PR

;	( CCP task wakeup )
	MOV	#REPLACE_EVF,R4
	SHLL8	R4
	ADD	#CCP_EVF_NO,R4		;replace
	MOV	#CCP_HOT_MODE,R5	;ﾎｯﾄﾓｰﾄﾞでの起動ﾌﾗｸﾞｾｯﾄ
	TRAPA	#SET_EVF
#endif	///////////
}

/*------( CMD=02 : ｽﾘｰﾌﾟ )------*/
LOCAL void rx_cmd02_proc(void)
{
#if 0	///////////////
;	( APが実行中か? )
	MOV	#AP_CHECK_SVC,R0
	TRAPA	#AP_BIOS		;AP起動ﾌﾗｸﾞ ?
	TST	R0,R0
	BF	?0010			;AP実行中なら再起動へ

;	( CCPが起動中か? )
	MOV	#CCP_EVF_NO,R4
	TRAPA	#GET_EVF
	CMP/EQ	#0,R0			;0 ?
	BF	?0010			;ccpが起動中なら再起動へ

;	( 無視してreturn )
	BRA	REMOTE_RX_RET1		;(遅延分岐)to return
	NOP

;	( AP_TASKを初期ﾀｽｸとするｶｰﾈﾙの初期化 )
?0010:	MOV	#CCP_SLEEP_MODE,R0	;CCPは起動しない
	TRAPA	#MON_START		;jump $MON_START
					;もうここへは復帰してこない
#endif	///////////
}

/*-------( CMD=13 : ｺｰﾙﾄﾞﾌﾞﾚｰｸ )-------*/
LOCAL void rx_cmd13_proc(void)
{
#if 0	///////////////
	MOV	#AP_CHECK_SVC,R0
	TRAPA	#AP_BIOS		;AP起動ﾌﾗｸﾞ ?
	TST	R0,R0
	BT	?0001			;AP実行中でないなら無視する

;	( IOBYTEの引き込み処理 )
	MOV	R5,R4			;R4←IOBYTE指定
	STS	PR,R1			;save PR
	BSR	__IOBYTE_CHG		;(遅延分岐),IOBYTEの切換え処理
	NOP
	LDS	R1,PR			;restore PR

;	( AP_TASKを初期ﾀｽｸとするｶｰﾈﾙの初期化 )
	MOV	#CCP_COLD_MODE,R0	;CCPをｺｰﾙﾄﾞﾓｰﾄﾞで起動
	TRAPA	#MON_START		;jump $MON_START
					;もうここへは復帰してこない
?0001:
	BRA	REMOTE_RX_RET1		;(遅延分岐)to return
	NOP
#endif	///////////
}

/*-------( CMD=09 : ﾌﾟﾛｸﾞﾗﾑ実行 )-------*/
LOCAL void rx_cmd09_proc(void)
{
#if 0	///////////////
;	( AP wakeup )
	MOV	#AP_EXECUTE_SVC,R0
	TRAPA	#AP_BIOS
#endif	//////////////
}

#if 0	///////////////////
;================================================
;○__IOBYTE_CHG　　　　 《IOBYTEの切換え処理》
;　［入力］R4(B)…IOBYTE指定(=0:RS-232C側, =1:CH#0側)
;　［出力］なし
;  ［Note］・割込み禁止状態で呼ばれる事
;  ［破壊］R0,R4,R5,PR
;================================================
__IOBYTE_CHG:
	MOV.L	#__sysid,R5		;R5←__sysid address
	MOV	R4,R0
	MOV.L	@R5,R4			;R4←(__sysid)
	TST	#0x01,R0		;IOBYTE指定?
	BF	?0001

;	( IOBYTE='0' )
	MOV.L	#~IOBYTE_BIT,R0
	BRA	?0005			;(遅延分岐)
	AND	R0,R4			;IOBYTE←0

;	( IOBYTE='1' )
?0001:	MOV.L	#IOBYTE_BIT,R0
	OR	R0,R4			;IOBYTE←1
?0005:	MOV.L	R4,@R5			;(__sysid)←R4

;	( 全てのｲﾍﾞﾝﾄをｾｯﾄ )
	MOV	#CON_EVF_NO,R4
	MOV	#CON_EVENT_ALL,R5	;全てのｲﾍﾞﾝﾄをｾｯﾄ
	TRAPA	#SET_EVF

	RTS				;(遅延分岐)
	NOP
#endif	/////////////////////////


/*-------( CMD=11 : IOBYTEをRS232C側へ切換え )-------*/
LOCAL void rx_cmd11_proc(void)
{
#if 0	///////////////
;	( IOBYTEの切換え )
	MOV	#0,R4			;R4←IOBYTE指定(RS-232C側)
	STS	PR,R1			;save PR
	BSR	__IOBYTE_CHG		;(遅延分岐),IOBYTEの切換え処理
	NOP
	LDS	R1,PR			;restore PR
#endif	//////////////
}

/*-------( CMD=12 : IOBYTEをCH#0側へ切換え )-------*/
LOCAL void rx_cmd12_proc(void)
{
#if 0	///////////////
;	( IOBYTEの切換え )
	MOV	#1,R4			;R4←IOBYTE指定(CH#0側)
	STS	PR,R1			;save PR
	BSR	__IOBYTE_CHG		;(遅延分岐),IOBYTEの切換え処理
	NOP
	LDS	R1,PR			;restore PR
#endif	//////////////
}


/*=======( rx_mode 1 decode )=======*/
LOCAL int rx_mode1_decode(T_RMTWK *wkp)
{
	short	data;
	ulong	addr;

	switch (wkp->mon_cmd) {				// ﾘﾓｰﾄ制御ｺﾏﾝﾄﾞの解析
	  case 0x01:						/*---( CMD=01 : ﾎｯﾄﾌﾞﾚｰｸ )---*/
		wkp->rx_mode = 0;				// (rx_mode)←0
		rx_cmd01_proc();				// CCP起動処理
		break;

	  case 0x02:						/*---( CMD=02 : ｽﾘｰﾌﾟ )---*/
		wkp->mon_base = _tpa;			// (mon_base)←TAPｱﾄﾞﾚｽ
		wkp->mon_offs = 0;				// (mon_offs)←0
		wkp->rx_mode = 0;				// (rx_mode)←0
		rx_cmd02_proc();				// ｽﾘｰﾌﾟ処理 ※ﾘﾀｰﾝしてこない事もある
		break;

	  case 0x13:						/*---( CMD=13 : ｺｰﾙﾄﾞﾌﾞﾚｰｸ )---*/
		wkp->rx_mode = 0;				// (rx_mode)←0
		rx_cmd13_proc();				// ｽﾘｰﾌﾟ処理 ※ﾘﾀｰﾝしてこない事もある
		break;

	  case 0x06:						/*---( CMD=06 : ﾒﾓﾘﾗｲﾄ )---*/
		/* ﾃﾞｰﾀ受信 */
		wkp->mon_sum = 0;				// (mon_sum)←ｻﾑﾁｪｯｸﾃﾞｰﾀのｸﾘｱ
		addr = wkp->mon_base;			// get mon_base
		addr += (ushort)wkp->mon_offs;	// ask mon_base + mon_offs
		wkp->rx_data_addr = addr;		// (rx_data_addr)←addr
		wkp->rx_data_cnt = wkp->mon_cnt;// (rx_data_cnt)←(mon_cnt)
		wkp->rx_mode = 4;				// (rx_mode)←4
		return -1;						// 上位では無視

	  case 0x07:						/*---( CMD=07 : ﾒﾓﾘﾘｰﾄﾞ )---*/
	  	/* ﾃﾞｰﾀ送信 */
		addr = wkp->mon_base;			// get mon_base
		addr += (ushort)wkp->mon_offs;	// ask mon_base + mon_offs
		wkp->tx_data_addr = addr;		// (tx_data_addr)←addr
		wkp->tx_data_cnt = wkp->mon_cnt;// (tx_data_cnt)←(mon_cnt)
		wkp->tx_mode = 1;				// (tx_mode)←1
		wkp->rx_mode = 0;				// (rx_mode)←0
		return -2;						// 上位では送信開始要求

	  case 0x09:						/*---( CMD=09 : ﾌﾟﾛｸﾞﾗﾑ実行 )---*/
		rx_cmd09_proc();				// AP起動
		break;

	  case 0x11:						/*---( CMD=11 : IOBYTEをRS232C側へ切換え )---*/
		rx_cmd11_proc();				// IOBYTEの切換(RS-232C側)
		break;

	  case 0x12:						/*---( CMD=12 : IOBYTEをCH#0側へ切換え )---*/
		rx_cmd12_proc();				// IOBYTEの切換(CH#0側)
		break;

	  case 0x0b:						/*---( CMD=0B : ﾚｼﾞｽﾀ設定 )---*/
		break;							/* 無視 */

	  case 0x0c:						/*---( CMD=0C : ﾊﾞｲﾄ入力 )---*/
	  case 0x0d:						/*---( CMD=0D : ﾜｰﾄﾞ入力 )---*/
	  case 0x10:						/*---( CMD=10 : CBA入力 )---*/
	  case 0x16:						/*---( CMD=16 : ﾁｪｯｸｻﾑﾘｰﾄﾞ )---*/
		addr = wkp->mon_base;			// get mon_base
		addr += (ushort)wkp->mon_offs;	// ask mon_base + mon_offs

		if (wkp->mon_cmd == 0x0c)
			data = *((uchar *)addr);	// 8bit読み出しﾃﾞｰﾀ
		else if (wkp->mon_cmd == 0x0d)
			data = *((short *)addr);	// 16bit読み出しﾃﾞｰﾀ
		else if (wkp->mon_cmd == 0x16)
			data = wkp->mon_sum;		// data←(mon_sum)
		else
			data = 0;					// CBA=0(dummyﾃﾞｰﾀ)

		wkp->tx_data[0] = (char)data;		// L
		wkp->tx_data[1] = (char)(data>>8);	// H

		wkp->tx_data_addr = (long)&(wkp->tx_data[0]);	// (tx_data_addr)←tx_data addr.
		wkp->tx_data_cnt = 2;			// (tx_data_cnt)←2
		wkp->tx_mode = 1;				// (tx_mode)←1
		wkp->rx_mode = 0;				// (rx_mode)←0
		return -2;						// 上位では送信開始要求

	  case 0x03:						/* (CMD=03),ｵﾌｾｯﾄ設定 */
	  case 0x04:						/* (CMD=04),ｾｸﾞﾒﾝﾄ設定 */
	  case 0x05:						/* (CMD=05),下位ｻｲｽﾞ設定 */
	  case 0x08:						/* (CMD=08),上位ｻｲｽﾞ設定 */
	  case 0x0e:						/* (CMD=0E),ﾊﾞｲﾄ出力 */
	  case 0x0f:						/* (CMD=0F),ﾜｰﾄﾞ出力 */
		/* (rx_mode)←2 をセット */
		wkp->rx_mode = 2		;		// (rx_mode)←2
		return -1;						// 上位では無視
	}

	wkp->rx_mode = 0;					// (rx_mode)←0
	return -1;							// 上位では無視
}


/*=======( rx_mode 3 decode )=======*/
LOCAL int rx_mode3_decode(T_RMTWK *wkp, short data)
{
	ulong	addr;

	switch (wkp->mon_cmd) {				// (mon_cmd) ?
	  case 0x03:						/*---( CMD=03 : ｵﾌｾｯﾄ設定 )---*/
		wkp->mon_offs = data;
		break;

	  case 0x04:						/*---( CMD=04 : ｾｸﾞﾒﾝﾄ設定 )---*/
		wkp->mon_seg0 = data;			// (mon_seg0)←data …ｻﾑﾁｪｯｸ計算用

		addr = data<<16;
		if (addr == 0)					// ｾｸﾞﾒﾝﾄ設定が｢0｣か?
			addr = APA_ADDR;			// APAの先頭ｱﾄﾞﾚｽを設定
		wkp->mon_base = addr;
		break;

	  case 0x05:						/*---( CMD=05 : 下位ｻｲｽﾞ設定 )---*/
		wkp->mon_cnt = (ushort)data;	// (mon_cnt)←0:data

		/* ｻﾑﾁｪｯｸﾃﾞｰﾀの計算 */
		data += wkp->mon_offs;			// (mon_offs)の加算
		data += wkp->mon_seg0;			// (mon_seg0)の加算
		wkp->mon_sum = data;			// (mon_sum)に格納
		break;

	  case 0x08:						/*---( CMD=08 : 上位ｻｲｽﾞ設定 )---*/
		addr = data<<16;
		addr += (ushort)(wkp->mon_cnt);	// 上位ｻｲｽﾞ:下位ｻｲｽﾞを合成
		wkp->mon_cnt = addr;

		/* ｻﾑﾁｪｯｸﾃﾞｰﾀの計算 */
		///MOV.W	@(MON_SUM-WKTOP,R8),R0	;R0←(MON_SUM)
		///MOV.W	R0,@(MON_SUM-WKTOP,R8)	;(MON_SUM)←R0
		break;

	  case 0x0a:						/*---( CMD=0A : 1ﾊﾞｲﾄ書き込み )---*/
		addr = wkp->mon_base;			// get mon_base
		addr += (ushort)wkp->mon_offs;	// ask mon_base + mon_offs
		addr += (uchar)data;

		(*(char *)addr) = data>>8;		// 書き込み
		break;

	  case 0x0e:						/*---( CMD=0E : ﾊﾞｲﾄ出力 )---*/
		addr = wkp->mon_base;			// get mon_base
		addr += (ushort)wkp->mon_offs;	// ask mon_base + mon_offs

		(*(char *)addr) = (char)data;	// 書き込み
		break;

	  case 0x0f:						/*---( CMD=0F : ﾜｰﾄﾞ出力 )---*/
		addr = wkp->mon_base;			// get mon_base
		addr += (ushort)wkp->mon_offs;	// ask mon_base + mon_offs

		(*(short *)addr) = data;		// 書き込み
		break;
	}

	wkp->rx_mode = 0;					// (rx_mode)←0
	return -1;							// 上位では無視
}


EXPORT int _remote_rx(int c, char mode)
{
	char	rxmode;
	ulong	addr;
	T_RMTWK	*wkp;

	wkp = &rmtwk[mode & 1];				// 作業領域の選択(bit0)
	rxmode = wkp->rx_mode;

	if (rxmode == 0) {					// rx_mode=0 ?
		if (c == 0x00) {
			/*=======( リモートコマンド受信 )=======*/
			wkp->rmt_chk = RMT_CHK_COUNT;
			wkp->rx_mode = 1;			// rx_mode←1
			return -1;					// 上位では無視
		}
		else if (c == 0x01) {
			/*=======( 送信継続コマンド受信 )=======*/
			return -3;					// 上位では送信文字制限解除処理
		}
		else if (c == 0x14) {			// DC4, CTRL+T 受信?
			wkp->mon_cmd = 0x01;		// ホットブレーク 処理へ差し替え
			goto RX_CMD01;
		}
		else {
			return c;					// 通常文字受信
		}
	}
	else {								// rx_mode<>0 の時
		/*******( rx_mode decode )********/
		wkp->rmt_chk = RMT_CHK_COUNT;

		if (rxmode == 1) {				// rx_mode=1 ?
			/*=======( MODE 1  : 0 receive )=======*/
			wkp->mon_cmd = c;			// (mon_cmd)←受信データ
RX_CMD01:
			return rx_mode1_decode(wkp);
		}
		else if (rxmode == 2) {			// rx_mdoe=2 ?
			/*=======( MODE 2  : para 1 wait )=======*/
			wkp->mon_dat = c;			// (mon_dat)←受信ﾃﾞｰﾀ(L)
			wkp->rx_mode = 3;			// (rx_mode)←3
			return -1;					// 上位では無視
		}
		else if (rxmode == 3) {			// rx_mode=3 ?
			/*=======( MODE 3  : para 2 wait )=======*/
			c = (c<<8) + (uchar)(wkp->mon_dat);	// H, Lﾃﾞｰﾀ(mon_dat)
			return rx_mode3_decode(wkp, c);
		}
		else if (rxmode == 4) {
			/*=======( MODE 4  : data rx )=======*/
			addr = wkp->rx_data_addr;
			(*(char *)addr) = c;		// 書き込み
			addr++;
			wkp->rx_data_addr = addr;	// 書き込みｱﾄﾞﾚｽの更新

			wkp->mon_sum += (uchar)c;	// ｻﾑﾁｪｯｸﾃﾞｰﾀの更新

			wkp->rx_data_cnt--;			// ｶｳﾝﾄの更新
			if (wkp->rx_data_cnt == 0)	// 終了?
				wkp->rx_mode = 0;		// (rx_mode)←0
			return -1;					// 上位では無視
		}
		else {
			wkp->rx_mode = 0;			// (rx_mode)←0
			return -1;					// 上位では無視
		}
	}
}


/*================================================
;|
;|□_remote_tx          《リモート制御の送信処理》
;|  ［形式］#include <misc/libmisc.h>
;|          int _remote_tx(char mode);
;|  ［引数］mode…処理ﾓｰﾄﾞ(bit0:呼出し側IOBYTE)
;|  ［戻値］≧0：ﾘﾓｰﾄ文字（上位ではR0の文字を送出する）
;|          =-1：通常処理（上位では通常文字を処理する）
;|  ［Note］・割込み禁止状態で呼ばれる事
;|          ・ﾘﾓｰﾄ制御の仕様は｢VW15-3015 IOP-5ﾓﾆﾀｰ機能仕様書｣5.2項を参照
;|
 ================================================*/
EXPORT int _remote_tx(char mode)
{
	char	txmode;
	int		c;
	ulong	addr;
	T_RMTWK	*wkp;

	wkp = &rmtwk[mode & 1];				// 作業領域の選択(bit0)
	txmode = wkp->tx_mode;

	/******( TX_MODE decode )******/
	switch (txmode) {
	  case 0x00:
		return -1;						// 通常処理

	  case 0x01:
		/*=======( １の送出 )=======*/
		wkp->mon_sum = 0;				// ｻﾑﾁｪｯｸﾃﾞｰﾀのｸﾘｱ
		wkp->tx_mode = 2;				// (tx_mode)←2
		return 1;						// '1'文字送信へ

	  case 0x02:
		/*=======( データの送出 )=======*/
		addr = wkp->tx_data_addr;
		c = (*(uchar *)addr);			// 送信ﾃﾞｰﾀの読み出し
		addr++;
		wkp->tx_data_addr = addr;		// ｱﾄﾞﾚｽの更新

		wkp->mon_sum += (uchar)c;		// ｻﾑﾁｪｯｸﾃﾞｰﾀの更新
		wkp->tx_data_cnt--;				// ｶｳﾝﾄの更新
		if (wkp->tx_data_cnt == 0)		// 終了?
			wkp->tx_mode = 0;			// (tx_mode)←0
		return (uchar)c;				// 送出文字
	}

	wkp->tx_mode = 0;					// (tx_mode)←0
	return -1;							// 通常処理
}


/*----------------------------------------------------------------------
#|History of "remote.c"
#|=====================
#|* 2016/02/18	新規作成(by T.Yokobayashi)
#|  "sh_std/bios/remote.src"を参考にc言語に移植
#|
*/
