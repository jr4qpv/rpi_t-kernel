/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(c) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)filebios.c (libmisc) 2016/02/19
 *	FILE基本入出力関係ﾗｲﾌﾞﾗﾘｰ
 *
 *	［Note］
 *		・ﾌｧｲﾙBIOS仕様は｢VW15-3015 IOP-5ﾓﾆﾀｰ機能仕様書｣5.3項を参照。
 */

#include <basic.h>
///#include <t2ex/ctype.h>				/* for isprint() */
#include <t2ex/errno.h>					/* for t2ex/string.h */
#include <t2ex/string.h>				/* for memcpy(),memset() */
#include <tk/tkernel.h>
#include <misc/libmisc.h>
#include "filebios.h"


///extern short _ferrno;
short _ferrno;				////////


//typedef	unsigned char	uchar;
typedef	unsigned short	ushort;
//typedef	unsigned long	ulong;



/*** FRB(File Request Block) structure define ***/
typedef struct t_frb {
	short SI;							/* (16):SI(ﾌｧﾝｸｼｮﾝｺｰﾄﾞ) */
	short AX;							/* (16):AX */
	short BX;							/* (16):BX */
	short CX;							/* (16):CX */
	short DX;							/* (16):DX */
	short ES;							/* (16):ES */
	short DS;							/* (16):DS */
	short FG;							/* (16):flag */
} T_FRB;
///extern volatile T_FRB _frb;
#define _frb	(*(volatile T_FRB *)FRB_ADDR)		/* ////// */


/*******( 定数の定義 )*******/
#define	REQ_CODE			2			/* file request code */
#define	TIMEOUT_DEFAULT		500			/* 5.0秒 [10ms単位] */
#define	INITCHECK_TIME		20			/* 最初の20m秒間は1m秒で高速ﾁｪｯｸ */
//#define	BIG_ENDIAN			1		/* BIGエンディアン時有効にする */


/*******( 共通変数の定義 )*******/
static short timeout_count = TIMEOUT_DEFAULT;	/* ﾀｲﾑｱｳﾄ検出時間の保持変数[10ms] */



/*------------------------------------------------
;
;□file_request         《ファイル処理を要求する》
;  ［形式］int file_request(T_FRB *frbp, char axchk);
;  ［引数］frbp …処理するFRBのﾎﾟｲﾝﾀ
;          fcbp …FCBﾃﾞｰﾀﾊﾞｯﾌｧのﾎﾟｲﾝﾀ(_findfirst, _findnextで使用)
;          axchk…戻値AXのﾁｪｯｸ(=0:しない, =1:する)
;  ［戻値］＝0:正常, =-1:エラー
;  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;
 ------------------------------------------------*/
static int file_request(T_FRB *frbp, char axchk)
{
	short tim;

///	get_sem(__FILE_REQ_SEM_NO);			/*>>> セマフォーの獲得 >>>>>>*/

	/*------( FRBのｺﾋﾟｰ )------*/
	memcpy((void *)&_frb, (void *)frbp, sizeof(T_FRB));

	/*------( リクエストの送出 )------*/
	_frb.FG = 0;
#ifdef BIG_ENDIAN
	swap_memw(&_frb, 8);				/* ビッグエンディアンの処理 */
#endif
	putch(REQ_CODE);					/* file request code */

	/*------( 処理完了待ち )------*/
	for (tim=-INITCHECK_TIME; tim<timeout_count; tim++) {
		if (tim < 0)
			tk_dly_tsk(1);				/* 最初の間は、1mSでﾁｪｯｸ */
		else
			tk_dly_tsk(10);

		if (_frb.FG) {					/* FG=0 ? */
#ifdef BIG_ENDIAN
			swap_memw(&_frb, 8);		/* ビッグエンディアンの処理 */
#endif

			/* ( FRBのｺﾋﾟｰ )*/
			memcpy((void *)frbp, (void *)&_frb, sizeof(T_FRB));
///			free_sem(__FILE_REQ_SEM_NO);/*<<< セマフォーの解放 <<<<<<<*/

			if (axchk) {
				if (frbp->AX == 0) {	/* AX=0 ? */
					return 0;			/* 正常 */
				}
				else {
					_ferrno = frbp->AX;	/* ｴﾗｰｺｰﾄﾞ */
					return -1;			/* 異常 */
				}
			}
			else {
				return 0;				/* 正常 */
			}
		}
	}

	/*------( タイムアウト処理 )------*/
///	free_sem(__FILE_REQ_SEM_NO);		/*<<< セマフォーの解放 <<<<<<<*/
	_ferrno = 0x7f;						/* ﾀｲﾑｱｳﾄ */
	return -1;							/* 異常 */
}


/*------------------------------------------------
;|
;|□_file_init          《BIOSの初期化》
;|  ［形式］#include "filebios.h"
;|          int _file_init(long timo);
;|  ［引数］timo…ﾀｲﾑｱｳﾄ時間指定[1mS単位で指定]（=0:の時はﾃﾞﾌｫﾙﾄ値）
;|  ［戻値］＝0:正常, ≠0:エラー
;|  ［Note］・
;|
 ------------------------------------------------*/
////static int _file_init(long timo)
int _file_init(long timo)		/////
{
	/*******( 初回起動時の初期化処理 )*******/
///	if (get_sysid() & __FIRST_START_BIT) {	/* 初回ﾌﾗｸﾞ? */
		_ferrno = 0;						/* ｴﾗｰｺｰﾄﾞのｸﾘｱ */
///	}

	/*******( 毎回起動時の初期化処理 )*******/
	/* タイムアウト監視時間の設定 */
	if ((timo < 10) || (timo > 30000)) {	/* それらしい値か? */
		timeout_count = TIMEOUT_DEFAULT;	/* デフォルト値 */
	}
	else {
		timeout_count = timo / 10;			/* 10mS単位に換算 */
	}

	return 0;
}


/*================================================
;|
;|□_file_creatnew     《新しいファイルを作成する》
;|  ［形式］#include "filebios.h"
;|          int _file_creatnew(char *path);
;|  ［引数］path…ファイルネームのポインタ
;|  ［戻値］≧0:ﾌｧｲﾙﾊﾝﾄﾞﾙ, =-1:エラー
;|  ［Note］・新しいﾌｧｲﾙを作成する。すでにﾌｧｲﾙが存在する場合はｴﾗｰとなる。
;|          ・ファイルは常にバイナリーモードでR/Wモードでオープンされる。
;|          ・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x03:ﾒﾃﾞｨｱが無い      ,0x04:ｵｰﾌﾟﾝするﾌｧｲﾙ名が多すぎる
;|                0x05:ｱｸｾｽが否定された ,0x50:ﾌｧｲﾙがすでに存在する
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
////static int _file_creatnew(char *path)
int _file_creatnew(char *path)		//////
{
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*------( FRBの設定 )------*/
	lfrb.SI = 8;						/* SI←8(新しいファイルの作成) */
	lfrb.DX = (short)((long)path);		/* DX←path(下位16bit) */
	lfrb.DS = (short)((long)path>>16);	/* DS←path(上位16bit) */
	lfrb.CX = 0;						/* CX←0 */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(8, _ferrno));
		return -1;						/* 異常 */
	}

	return lfrb.BX;						/* BX(ファイルハンドル) */
}


/*================================================
;|
;|□_file_unlink        《ファイルを削除する》
;|  ［形式］#include "filebios.h"
;|          int _file_unlink(char *path);
;|  ［引数］path…ファイルネームのポインタ
;|  ［戻値］＝0:正常, =-1:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|            0x02:ﾌｧｲﾙが見つからない ,0x03:ﾒﾃﾞｨｱがみつからない
;|            0x05:ｱｸｾｽが拒否された   ,0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
////static int _file_unlink(char *path)
int _file_unlink(char *path)		//////
{
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*------( FRBの設定 )------*/
	lfrb.SI = 9;						/* SI←9(ファイル削除) */
	lfrb.DX = (short)((long)path);		/* DX←path(下位16bit) */
	lfrb.DS = (short)((long)path>>16);	/* DS←path(上位16bit) */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(9, _ferrno));
		return -1;						/* 異常 */
	}

	return 0;							/* 正常 */
}


/*================================================
;|
;|□_file_open          《ﾌｧｲﾙをｵｰﾌﾟﾝする》
;|  ［形式］#include "filebios.h"
;|          int _file_open(char *path, int oflag);
;|  ［引数］path…ﾌｧｲﾙ名のﾎﾟｲﾝﾀ
;|          oflag …BIOS_O_RD(0),BIOS_O_WR(1),BIOS_O_RDWR(2)
;|  ［戻値］≧0:ﾌｧｲﾙﾊﾝﾄﾞﾙ, =-1:エラー
;|  ［Note］・常にバイナリーモードでオープンされる。
;|          ・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x01:無効なﾌｧﾝｸｼｮﾝ   ,0x02:ﾌｧｲﾙ名が見つからない
;|                0x03:ﾒﾃﾞｨｱが無い     ,0x04:ｵｰﾌﾟﾝするﾌｧｲﾙ名が多すぎる
;|                0x05:ｱｸｾｽが否定された,0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
/////static int _file_open(char *path, int oflag)
int _file_open(char *path, int oflag)		///////
{
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*------( FRBの設定 )------*/
	lfrb.SI = 1;						/* SI←1(ファイルオープン) */
	lfrb.AX = oflag;					/* AX←oflag */
	lfrb.DX = (short)((long)path);		/* DX←path(下位16bit) */
	lfrb.DS = (short)((long)path>>16);	/* DS←path(上位16bit) */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(1, _ferrno));
		return -1;						/* 異常 */
	}

	return lfrb.BX;						/* BX(ファイルハンドル) */
}


/*================================================
;|
;|□_file_close         《ファイルをクローズする》
;|  ［形式］#include "filebios.h"
;|          int _file_close(int fd);
;|  ［引数］fd…ファイルハンドル
;|  ［戻値］＝0:正常, =-1:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x06:無効なハンドル   ,0x07:クローズ処理エラー
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
/////static int _file_close(int fd)
int _file_close(int fd)			///////
{
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*------( FRBの設定 )------*/
	lfrb.SI = 2;						/* SI←2(ファイルクローズ) */
	lfrb.BX = (short)fd;				/* BX←fd */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(2, _ferrno));
		return -1;						/* 異常 */
	}

	return 0;							/* 正常 */
}


/*================================================
;|
;|□_file_lseek         《ファイルポインタを移動する》
;|  ［形式］#include "filebios.h"
;|          long _file_lseek(int fd, long offset, int ptrname);
;|  ［引数］fd…ファイルハンドル
;|          offset…移動するバイト数
;|          ptrname…移動する基点 BIOS_SEEK_SET(0),BIOS_SEEK_CUR(1),
;|                                BIOS_SEEK_END(2)
;|  ［戻値］≧0:新しいポインタの位置オフセット, =-1L:エラー
;|  ［Note］・BIOS_SEEK_SET:ﾌｧｲﾙの先頭,BIOS_SEEK_CUR:現在位置,
;|            BIOS_SEEK_END:ﾌｧｲﾙの終り
;|          ・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x01:無効なﾌｧﾝｸｼｮﾝ   ,0x05:ｱｸｾｽが否定された
;|                0x06:無効なﾊﾝﾄﾞﾙ     ,0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
/////static long _file_lseek(int fd, long offset, int ptrname)
long _file_lseek(int fd, long offset, int ptrname)		//////
{
	long ltmp;
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*------( FRBの設定 )------*/
	lfrb.SI = 10;						/* SI←10(ファイルポインタの移動) */
	lfrb.BX = fd;						/* BX←fd */
	lfrb.AX = (short)ptrname;			/* AX←ptrname */
	lfrb.DX = (short)offset;			/* DX←offset(下位16bit) */
	lfrb.CX = (short)(offset>>16);		/* CX←offset(上位16bit) */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(10, _ferrno));
		return -1L;						/* 異常 */
	}

	ltmp = (lfrb.DX<<16) + (ushort)lfrb.BX;	/* DX:BX(新しいﾛｹｰｼｮﾝ) */
	return ltmp;
}


/*================================================
;|
;|□_file_findfirst     《ファイル名の検索(初回)》
;|  ［形式］#include "filebios.h"
;|          int _file_findfirst(char *path, T_FCB *buff, int attr);
;|  ［引数］path…ファイルネームのポインタ
;|          buff…データバッファーの先頭アドレス
;|          attr…属性
;|  ［戻値］＝0:正常, =-1:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|            0x03:ﾒﾃﾞｨｱがみつからない,0x12:一致するﾌｧｲﾙがない
;|            0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
////static int _file_findfirst(char *path, T_FCB *buff, int attr)
int _file_findfirst(char *path, T_FCB *buff, int attr)	/////////
{
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	memset(buff, 0, sizeof(T_FCB));		/* ﾃﾞｰﾀﾊﾞｯﾌｧｰのｸﾘｱ */

	/*------( FRBの設定 )------*/
	lfrb.SI = 13;						/* SI←13(ファイル名の検索初回) */
	lfrb.CX = (short)attr;				/* CX←attr */
	lfrb.DX = (short)((long)path);		/* DX←path(下位16bit) */
	lfrb.DS = (short)((long)path>>16);	/* DS←path(上位16bit) */
	lfrb.BX = (short)((long)buff);		/* BX←buff(下位16bit) */
	lfrb.ES = (short)((long)buff>>16);	/* ES←buff(上位16bit) */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(13, _ferrno));
		return -1;						/* 異常 */
	}

	return 0;							/* 正常 */
}


/*================================================
;|
;|□_file_findnext      《ファイル名の検索(2回目以降)》
;|  ［形式］#include "filebios.h"
;|          int _file_findnext(T_FCB *buff);
;|  ［引数］buff…データバッファーの先頭アドレス
;|  ［戻値］＝0:正常, =-1:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x03:ﾒﾃﾞｨｱがみつからない,0x12:一致するﾌｧｲﾙがない
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
/////static int _file_findnext(T_FCB *buff)
int _file_findnext(T_FCB *buff)	///////
{
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*------( FRBの設定 )------*/
	lfrb.SI = 14;						/* SI←14(ファイル名の検索次回) */
	lfrb.BX = (short)((long)buff);		/* BX←buff(下位16bit) */
	lfrb.ES = (short)((long)buff>>16);	/* ES←buff(上位16bit) */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(14, _ferrno));
		return -1;						/* 異常 */
	}

	return 0;						/* 正常 */
}


/*================================================
;|
;|□_file_read          《ファイルから読み込みを行なう》
;|  ［形式］#include "filebios.h"
;|          int _file_read(int fd, void *buf, unsigned short n);
;|  ［引数］fd …ファイルハンドル
;|          buf…バッファーの先頭アドレス
;|          n  …バッファーのバイト数
;|  ［戻値］バッファーに読み込んだバイト数(=-1:エラー)
;|  ［Note］・ファイルエンドでは｢0｣を返します。
;|          ・メンテナンスポート使用のときはデータ転送に時間がかかるので、
;|            1024byteを越えるデータ長を指定しない事。
;|          ・ｾｸﾞﾒﾝﾄ境界を越える場合は転送を2回に分ける(TERMの8086版に対応)。
;|          ・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x05:ｱｸｾｽが否定された    ,0x06:無効なハンドル
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
////static int _file_read(int fd, void *buf, unsigned short n)
int _file_read(int fd, void *buf, unsigned short n)		//////
{
	long segchk;
	unsigned short rest, count;
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*-----( 8086CPUの為のｾｸﾞﾒﾝﾄ境界を越えるかのﾁｪｯｸ )-----*/
	segchk = (long)buf + n;
	if ((segchk & 0xFFFF0000) != ((long)buf & 0xFFFF0000)) {
		rest = segchk & 0x0000FFFF;		/* ｾｸﾞﾒﾝﾄ境界を越えるﾊﾞｲﾄ数 */
		segchk = segchk & 0xFFFF0000;	/* 2回目起動のｱﾄﾞﾚｽ */
	}
	else
		rest = 0;						/* ｾｸﾞﾒﾝﾄ境界を越えない */

	/*------( FRBの設定 )------*/
	lfrb.SI = 3;						/* SI←3(ファイルリード) */
	lfrb.BX = (short)fd;				/* BX←fd */
	lfrb.CX = n - rest;					/* CX←n */
	lfrb.DX = (short)((long)buf);		/* DX←buf(下位16bit) */
	lfrb.DS = (short)((long)buf>>16);	/* DS←buf(上位16bit) */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(3, _ferrno));
		return -1;						/* 異常 */
	}

	count = lfrb.BX;					/* BX(読み出したバイト数) */
	if (count != (n - rest))
		return count;

	/*=====( ｾｸﾞﾒﾝﾄ境界を越える時は２回に分ける )=====*/
	if (rest) {
		/*------( FRBの設定 )------*/
		lfrb.SI = 3;					/* SI←3(ファイルリード) */
		lfrb.BX = (short)fd;			/* BX←fd */
		lfrb.CX = rest;					/* CX←n */
		lfrb.DX = (short)segchk;		/* DX←segchk(下位16bit) */
		lfrb.DS = (short)(segchk>>16);	/* DS←segchk(上位16bit) */

		/*------( ファイル要求 )------*/
		if (file_request(&lfrb, 1)) {
///			bios_errlog(BIOSERR_FILEBIOS, xtrct_word(0x100+3, _ferrno));
			return -1;					/* 異常 */
		}

		count += (unsigned short)lfrb.BX;	/* BX(読み出したバイト数) */
	}

	return count;
}


/*================================================
;|
;|□_file_write         《ファイルへの書き込みを行なう》
;|  ［形式］#include "filebios.h"
;|          int _file_write(int fd, void *buf, unsigned short n);
;|  ［引数］fd…ファイルハンドル
;|          buf…バッファーの先頭アドレス
;|          n…バッファーのバイト数(=0の時はﾌｧｲﾙﾎﾟｲﾝﾀを最後に移動)
;|  ［戻値］バッファーに読み込んだバイト数(=-1:エラー)
;|  ［Note］・メンテナンスポートを使用したときはデータ転送に時間がかかるので、
;|            1024byteを越えるデータ長を指定しない事。
;|          ・ｾｸﾞﾒﾝﾄ境界を越える場合は転送を2回に分ける(TERMの8086版に対応)。
;|          ・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x05:ｱｸｾｽが否定された    ,0x06:無効なハンドル
;|                0x07:書き込み処理ｴﾗｰ     ,0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
/////static int _file_write(int fd, void *buf, unsigned short n)
int _file_write(int fd, void *buf, unsigned short n)	//////
{
	long segchk;
	unsigned short rest, count;
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*-----( 8086CPUの為のｾｸﾞﾒﾝﾄ境界を越えるかのﾁｪｯｸ )-----*/
	segchk = (long)buf + n;
	if ((segchk & 0xFFFF0000) != ((long)buf & 0xFFFF0000)) {
		rest = segchk & 0x0000FFFF;		/* ｾｸﾞﾒﾝﾄ境界を越えるﾊﾞｲﾄ数 */
		segchk = segchk & 0xFFFF0000;	/* 2回目起動のｱﾄﾞﾚｽ */
	}
	else
		rest = 0;						/* ｾｸﾞﾒﾝﾄ境界を越えない */

	/*------( FRBの設定 )------*/
	lfrb.SI = 4;						/* SI←4(ファイルライト) */
	lfrb.BX = (short)fd;				/* BX←fd */
	lfrb.CX = n - rest;					/* CX←n */
	lfrb.DX = (short)((long)buf);		/* DX←buf(下位16bit) */
	lfrb.DS = (short)((long)buf>>16);	/* DS←buf(上位16bit) */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(4, _ferrno));
		return -1;						/* 異常 */
	}

	count = (unsigned short)lfrb.BX;	/* BX(書き込んだバイト数) */
	if (count != (n - rest))
		return count;

	/*=====( ｾｸﾞﾒﾝﾄ境界を越える時は２回に分ける )=====*/
	if (rest) {
		/*------( FRBの設定 )------*/
		lfrb.SI = 4;					/* SI←4(ファイルライト) */
		lfrb.BX = (short)fd;			/* BX←fd */
		lfrb.CX = rest;					/* CX←n */
		lfrb.DX = (short)(segchk);		/* DX←segchk(下位16bit) */
		lfrb.DS = (short)(segchk>>16);	/* DS←segchk(上位16bit) */

		/*------( ファイル要求 )------*/
		if (file_request(&lfrb, 1)) {
///			bios_errlog(BIOSERR_FILEBIOS, xtrct_word(0x100+4, _ferrno));
			return -1;					/* 異常 */
		}

		count += (unsigned short)lfrb.BX;	/* BX(書き込んだバイト数) */
	}

	return count;
}


/*================================================
;|
;|□_file_get_fdate     《ファイルの日付けを獲得する》
;|  ［形式］#include "filebios.h"
;|          long _file_get_fdate(int fd);
;|  ［引数］fd…ファイルハンドル
;|  ［戻値］パックされた日付けデータ, =-1L:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x06:無効なﾊﾝﾄﾞﾙ   ,0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
/////static long _file_get_fdate(int fd)
long _file_get_fdate(int fd)	//////
{
	long ltmp;
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*------( FRBの設定 )------*/
	lfrb.SI = 15;						/* SI←15(ファイルの日付を得る) */
	lfrb.BX = (short)fd;				/* BX←fd */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(15, _ferrno));
		return -1L;						/* 異常 */
	}

	ltmp = (lfrb.DX<<16) + (ushort)lfrb.CX;	/* DX:CX(日付,時刻) */
	return ltmp;
}


/*================================================
;|
;|□_file_set_fdate     《ファイルの日付けを設定する》
;|  ［形式］#include "filebios.h"
;|          int _file_set_fdate(int fd, long fdate);
;|  ［引数］fd…ファイルハンドル
;|  ［戻値］＝0:正常, =-1:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x05:ｱｸｾｽが否定された    ,0x06:無効なハンドル
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
////static int _file_set_fdate(int fd, long fdate)
int _file_set_fdate(int fd, long fdate)	/////
{
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*------( FRBの設定 )------*/
	lfrb.SI = 16;						/* SI←16(ファイルの日付を設定する) */
	lfrb.BX = (short)fd;				/* BX←fd */
	lfrb.CX = (short)(fdate);			/* CX←fdate(下位16bit,時刻) */
	lfrb.DX = (short)(fdate>>16);		/* DX←fdate(上位16bit,日付) */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 1)) {
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(16, _ferrno));
		return -1;						/* 異常 */
	}

	return 0;							/* 正常 */
}


/*================================================
;|
;|□_file_get_free      《空きスペースを得る》
;|  ［形式］#include "filebios.h"
;|          long _file_get_free(void);
;|  ［引数］なし
;|  ［戻値］空きバイト数, =-1L:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
///static long _file_get_free(void)
long _file_get_free(void)			/////////////////
{
	T_FRB lfrb;							/* ﾛｰｶﾙFRB */

	/*------( FRBの設定 )------*/
	lfrb.SI = 7;						/* SI←7(空きスペースを得る) */

	/*------( ファイル要求 )------*/
	if (file_request(&lfrb, 0)) {		/* AXﾁｪｯｸしない */
///		bios_errlog(BIOSERR_FILEBIOS, xtrct_word(7, _ferrno));
		return -1L;						/* 異常 */
	}

	return (lfrb.AX * lfrb.BX * lfrb.CX);	/* 空きバイト数 */
}



#if 0	/////////////////////////
/*******( BCTの定義 )*******/
#define	FUNC_MAX	18
struct BCT {
	struct BCT0 hed;	/* ﾍｯﾀﾞｰ部 */
	void *funcp[FUNC_MAX];
};
const struct BCT filebios_bct = {
	FUNC_MAX,			/* function max */
	0,					/* RFU */
	0,					/* RFU */

	_file_init,			/* #0(初期化) */
	_file_open,			/* #1 */
	_file_close,		/* #2 */
	_file_read,			/* #3 */
	_file_write,		/* #4 */
	0,					/* #5(未定義) */
	0,					/* #6(未定義) */
	_file_get_free,		/* #7(空きｽﾍﾟｰｽを得る) */

	_file_creatnew,		/* #8 */
	_file_unlink,		/* #9 */
	_file_lseek,		/* #10 */
	0,					/* #11(ﾌｧｲﾙ属性を得る) */
	0,					/* #12(ﾌｧｲﾙ属性を設定する) */
	_file_findfirst,	/* #13 */
	_file_findnext,		/* #14 */
	_file_get_fdate,	/* #15 */

	_file_set_fdate,	/* #16 */
	0					/* #17(ｲﾆｼｬﾗｲｽﾞ) */
};
#endif	//////////////////////////////


/*----------------------------------------------------------------------
#|History of "filebios.c"
#|=======================
#|* 2016/02/17	新規作成(by T.Yokobayashi)
#|  "sh_std/bios/filebios.c"を流用
#|* 2016/02/18	ｵﾘｼﾞﾅﾙでは､_findfirst,_findnext時に、file_request()内で、
#|  FCBﾃﾞｰﾀを、APA領域(_fcb_buff)にコピー処理していたが削除した。
#|  ※ｾｸﾞﾒﾝﾄ境界にこのﾊﾞｯﾌｧｰがあるとOS86へのﾌｧｲﾙ要求ﾌｧﾝｸｼｮﾝで不具合が発生
#|    する対策だったが、最新のTERMを使っていれば問題ないと判断。
#|
*/
