/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)filebios.c (libmisc) 2016/02/19
 *	低水準入出力ｲﾝﾀﾌｪｰｽﾙｰﾁﾝ
 */

#include <basic.h>
///#include <t2ex/ctype.h>				/* for isprint() */
#include <t2ex/errno.h>					/* for t2ex/string.h */
#include <t2ex/string.h>				/* for strcmp() */
///#include <tk/tkernel.h>
#include "filebios.h"
#include <misc/libmisc.h>
#include <misc/lowlevel.h>


#define	DEBUG		0


struct FLB {
	char mode;							/* ｵｰﾌﾟﾝﾓｰﾄﾞ */
	char fdsk;							/* =1の時FDSK使用 */
	short handle;						/* ﾊﾝﾄﾞﾙ */
};


/*******( 定数定義 )*******/
#define FLMIN	0						/* 最小のﾌｧｲﾙ番号 */
#define FLMAX	32						/* ﾌｧｲﾙ数の最大値(SH-3,4時) */



/*******( 共通変数 )*******/
static struct FLB flmod[FLMAX];			/* ｵｰﾌﾟﾝしたﾌｧｲﾙのﾓｰﾄﾞ等設定場所 */



/*------------------------------------------------
;
;□curdrv_trim          《ﾊﾟｽ名からｶﾚﾝﾄﾄﾞﾗｲﾌﾞ指定を削除する》
;  ［形式］char *curdrv_trim(char *path);
;  ［引数］path…ﾌｧｲﾙ名
;  ［戻値］処理後のﾌｧｲﾙ名
;  ［Note］・ファイル名から"@:"のドライブ指定を削除する。
;
 ------------------------------------------------*/
static char *curdrv_trim(char *path)
{
	if (path[1] == ':') {
		if (path[0] == '@')
			return &path[2];
	}
	return path;
}


/*------------------------------------------------
;
;□fileno_check         《ﾌｧｲﾙ番号をﾁｪｯｸし管理ﾃｰﾌﾞﾙのﾎﾟｲﾝﾀを獲得》
;  ［形式］struct FLB *fileno_check(int fileno);
;  ［引数］fileno…ファイル番号
;  ［戻値］管理テーブルのポインタ(＝NULL:エラー)
;  ［Note］・
;
 ------------------------------------------------*/
static struct FLB *fileno_check(int fileno)
{
	struct FLB *p;

	if ((fileno<FLMIN) || (fileno>FLMAX)) {	/* ﾌｧｲﾙ番号の範囲ﾁｪｯｸ */
		set_ferrno(0x06);					/* 無効なハンドル */
		return NULL;
	}

	p = &flmod[fileno];
	if (p->mode == 0) {
		set_ferrno(0x06);					/* 無効なハンドル */
		return NULL;
	}

	return p;
}



/*================================================
;|
;|□fdskfile_check      《Flash-Diskのファイルか判定する》
;|  ［形式］#include <misc/lowlevel.h>
;|          int fdskfile_check(char *path);
;|  ［引数］path…ﾌｧｲﾙ名
;|  ［戻値］＝1:Flash-Diskのﾌｧｲﾙ, ＝0:その他のﾌｧｲﾙ
;|  ［Note］・ﾄﾞﾗｲﾌﾞ指定がない時はFlash-Diskとみなす。
;|
 ================================================*/
short fdskfile_check(char *path)
{
#if 0	//////////
	if (get_sysid() & __FLASH_ABLE_BIT) {		/* Flashが有効? */
		return (get_drvno(path) < 0) ? 1 : 0;	/* 未指定？ */
	}
#endif	//////////

	return 0;
}


/*================================================
;|
;|□_INIT_LOWLEVEL      《LOWLEVEL関数の初期化》
;|  ［形式］int _INIT_LOWLEVEL(void);
;|  ［引数］なし
;|  ［戻値］＝0:正常, ≠0:エラー
;|  ［Note］・C言語のｽﾀｰﾄｱｯﾌﾟ関数からｺｰﾙされる。
;|
 ================================================*/
int _INIT_LOWLEVEL(void)
{
	int i;
	struct FLB *p;

	for (i=0; i<=FLMAX; i++) {
		p = &flmod[i];

		p->mode = 0;
	}

	return 0;
}


/*================================================
;|
;|□open                《ファイルをオープンする(低水準入出力)》
;|  ［形式］#include <misc/lowlevel.h>
;|          int open(char *path, int mode);
;|  ［引数］path…ﾌｧｲﾙ名
;|          mode…ﾌｧｲﾙのﾓｰﾄﾞ
;|  ［戻値］≧0:ﾌｧｲﾙ番号, ＝-1:ｴﾗｰ
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x01:無効なﾌｧﾝｸｼｮﾝ    ,0x02:ﾌｧｲﾙ名が見つからない
;|                0x03:ﾒﾃﾞｨｱが無い      ,0x04:ｵｰﾌﾟﾝするﾌｧｲﾙ名が多すぎる
;|                0x05:ｱｸｾｽが否定された ,0x50:ﾌｧｲﾙがすでに存在する
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
int open(char *path, int mode)
{
	int i;
	int fileno;
	short fd, fdsk_flag, oflag;

	if (strcmp(path, "stdin") == 0) {		/*** 標準入力ﾌｧｲﾙ ***/
		if ((mode & O_RDONLY) == 0)
			return -1;
		flmod[STDIN].mode = mode;
		return STDIN;
	}
	else if (strcmp(path, "stdout") == 0) {	/*** 標準出力ﾌｧｲﾙ ***/
		if ((mode & O_WRONLY) == 0)
			return -1;
		flmod[STDOUT].mode = mode;
		return STDOUT;
	}
	else if (strcmp(path, "stderr") == 0) {	/*** 標準ｴﾗｰ出力ﾌｧｲﾙ ***/
		if ((mode & O_WRONLY) == 0)
			return -1;
		flmod[STDERR].mode = mode;
		return STDERR;
	}
	else {
		;
	}

	/*=======( 空きFLBの検索 )=======*/
	fileno = -1;
	for (i=STDERR+1; i<=FLMAX; i++) {
		if (flmod[i].mode == 0) {
			fileno = i;
			break;
		}
	}
	if (fileno < 0) {
		set_ferrno(0x04);					/* ｵｰﾌﾟﾝするﾌｧｲﾙ名が多すぎる */
		return -1;							/* 空きFLBが無い */
	}
	fdsk_flag = fdskfile_check(path);

	/*=======( 通常のファイルのオープン )=======*/
	if (mode & (O_TRUNC | O_CREAT)) {
		if (mode & O_TRUNC) {				/* すでに存在する場合は破棄 */
			/* ファイルの削除 */
			if (fdsk_flag)
				fdskbios_unlink(path);
			else
				filebios_unlink(curdrv_trim(path));
		}
											/* 次のO_CREATの処理へ継続 */
		if (fdsk_flag)
			fd = fdskbios_creatnew(path);
		else
			fd = filebios_creatnew(curdrv_trim(path));
	}
	else if (mode & (O_RDONLY | O_WRONLY | O_RDWR)) {
		switch (mode & (O_RDONLY | O_WRONLY | O_RDWR)) {
		case O_RDONLY:
			oflag = BIOS_O_RD; break;
		case O_WRONLY:
			oflag = BIOS_O_WR; break;
		case O_RDWR:
			oflag = BIOS_O_RDWR; break;
		default:
			set_ferrno(0x01);				/* 無効なﾌｧﾝｸｼｮﾝ */
			return -1;						/* ｴﾗｰ */
		}

		if (fdsk_flag)
			fd = fdskbios_open(path, oflag);
		else
			fd = filebios_open(curdrv_trim(path), oflag);
	}
	else {
		set_ferrno(0x01);					/* 無効なﾌｧﾝｸｼｮﾝ */
		return -1;							/* ｴﾗｰ */
	}

	if (fd < 0) {
#if DEBUG
		ceprintf("open(%s)=>%d, fdsk_flag=%d, ferrno=%04X\n",
				path, fd, fdsk_flag, get_ferrno());
#endif
		return -1;							/* ｴﾗｰ */
	}

	if (mode & O_APPEND) {
		/* //// 未サポート //// */
	}

	flmod[fileno].mode = mode;				/* オープンフラグ */
	flmod[fileno].fdsk = fdsk_flag;			/* FDSK使用フラグ */
	flmod[fileno].handle = fd;				/* ハンドル */

	return fileno;							/* ﾌｧｲﾙ番号 */
}


/*================================================
;|
;|□close               《ファイルをクローズする(低水準入出力)》
;|  ［形式］#include <misc/lowlevel.h>
;|          int close(int fileno);
;|  ［引数］fileno…ﾌｧｲﾙ番号
;|  ［戻値］=0:正常, ＝-1:ｴﾗｰ
;|  ［Note］・標準ファイルはクローズできない。
;|          ・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x06:無効なハンドル   ,0x07:クローズ処理エラー
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
int close(int fileno)
{
	short rc;
	struct FLB *p;

	if ((p = fileno_check(fileno)) == NULL)
		return -1;

	if (fileno > STDERR) {
		/*=====( 通常ファイルのクローズ )======*/
		if (p->fdsk)
			rc = fdskbios_close(p->handle);
		else
			rc = filebios_close(p->handle);

		p->mode = 0;						/* ﾌｧｲﾙのﾓｰﾄﾞﾘｾｯﾄ */
	}
	else {
		/*=====( 標準ファイルのクローズ )======*/
		rc = 0;
	}

	return rc;
}


/*================================================
;|
;|□read                《ファイルの読み込み(低水準入出力)》
;|  ［形式］#include <misc/lowlevel.h>
;|          int read(int fileno, char *buf, unsigned int count);
;|  ［引数］fileno…ﾌｧｲﾙ番号
;|          buf   …転送先ﾊﾞｯﾌｧｱﾄﾞﾚｽ
;|          count …読み込み文字数
;|  ［戻値］≧0:実際に読み込んだ文字数, ＝-1:ｴﾗｰ
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x05:ｱｸｾｽが否定された    ,0x06:無効なハンドル
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
int read(int fileno, char *buf, unsigned int count)
{
	short rc, i;
	struct FLB *p;

	if ((p = fileno_check(fileno)) == NULL)
		return -1;

	if (fileno > STDERR) {
		/*=====( 通常ファイルの読み込み )======*/
		if (p->fdsk)
			rc = fdskbios_read(p->handle, buf, count);
		else
			rc = filebios_read(p->handle, buf, count);

		return rc;
	}
	else {
		/*=====( 標準ファイルの読み込み )======*/
		if (p->mode & O_RDONLY || p->mode & O_RDWR) {
			for (i=count; i>0; i--) {
				*buf = getch();				/* 1文字入力 */
				if (*buf == CR_CODE)			/* 改行文字列の置き換え */
					*buf = LF_CODE;
				buf++;
			}
			return count;
		}
		else
			return -1;
	}
}


/*================================================
;|
;|□write               《ファイルへ書き出し(低水準入出力)》
;|  ［形式］#include <misc/lowlevel.h>
;|          int write(int fileno, char *buf, unsigned int count);
;|  ［引数］fileno…ﾌｧｲﾙ番号
;|          buf   …転送先ﾊﾞｯﾌｧｱﾄﾞﾚｽ
;|          count …書き出し文字数
;|  ［戻値］≧0:実際に書き出した文字数, ＝-1:ｴﾗｰ
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x05:ｱｸｾｽが否定された    ,0x06:無効なハンドル
;|                0x07:書き込み処理ｴﾗｰ     ,0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
int write(int fileno, char *buf, unsigned int count)
{
	short rc, i;
	struct FLB *p;
	char c;

	if ((p = fileno_check(fileno)) == NULL)
		return -1;

	if (fileno > STDERR) {
		/*=====( 通常ファイルの書き出し )======*/
		if (p->mode & O_RDONLY)
			return -1;

		if (p->fdsk)
			rc = fdskbios_write(p->handle, buf, count);
		else
			rc = filebios_write(p->handle, buf, count);

		return rc;
	}
	else {
		/*=====( 標準ファイルの書き出し )======*/
		if (p->mode & O_WRONLY || p->mode & O_RDWR) {
			if (fileno == STDERR) {			/*** エラー出力 ***/
				for (i=count; i>0; i--) {
					c = *(buf++);
					if (c == '\n')
						_putSIO('\r');
					_putSIO(c);
				}
			}
			else {							/*** 標準出力 ***/
				for (i=count; i>0; i--) {
					c = *(buf++);
					if (c == '\n')
						putch('\r');
					putch(c);
				}
			}
			return count;
		}
		else
			return -1;
	}
}


/*================================================
;|
;|□lseek               《ファイル位置の設定(低水準入出力)》
;|  ［形式］#include <misc/lowlevel.h>
;|          long lseek(int fileno, long offset, int base);
;|  ［引数］fileno…ﾌｧｲﾙ番号
;|          offset…ﾌｧｲﾙ位置
;|          base  …ｵﾌｾｯﾄの起点
;|  ［戻値］≧0:ﾌｧｲﾙ先頭からのｵﾌｾｯﾄ, ＝-1:ｴﾗｰ
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x01:無効なﾌｧﾝｸｼｮﾝ   ,0x05:ｱｸｾｽが否定された
;|                0x06:無効なﾊﾝﾄﾞﾙ     ,0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
long lseek(int fileno, long offset, int base)
{
	long rc;
	struct FLB *p;

	if ((p = fileno_check(fileno)) == NULL)
		return -1;

	if (fileno > STDERR) {
		/*=====( 通常ファイルの位置設定 )======*/
		if (p->fdsk)
			rc = fdskbios_lseek(p->handle, offset, base);
		else
			rc = filebios_lseek(p->handle, offset, base);

		return rc;
	}
	else {
		/*=====( 標準ファイルの位置設定 )======*/
		return -1;
	}
}


/*================================================
;|
;|□get_fdate           《ファイルの日付けを獲得する》
;|  ［形式］#include <misc/lowlevel.h>
;|          long get_fdate(int fileno);
;|  ［引数］fileno…ﾌｧｲﾙ番号
;|  ［戻値］パックされた日付けデータ, =-1L:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x06:無効なﾊﾝﾄﾞﾙ   ,0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
long get_fdate(int fileno)
{
	long rc;
	struct FLB *p;

	if ((p = fileno_check(fileno)) == NULL)
		return -1;

	if (fileno > STDERR) {
		/*=====( ファイルの日付獲得 )======*/
		if (p->fdsk)
			rc = fdskbios_get_fdate(p->handle);
		else
			rc = filebios_get_fdate(p->handle);

		return rc;
	}
	else {
		return -1L;
	}
}


/*================================================
;|
;|□set_fdate           《ファイルの日付けを設定する》
;|  ［形式］#include <misc/lowlevel.h>
;|          int set_fdate(int fileno, long fdate);
;|  ［引数］fileno…ﾌｧｲﾙ番号
;|          fdate …パックされた日付データ
;|  ［戻値］=0:正常, ＝-1:ｴﾗｰ
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x05:ｱｸｾｽが否定された    ,0x06:無効なハンドル
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
int set_fdate(int fileno, long fdate)
{
	short rc;
	struct FLB *p;

	if ((p = fileno_check(fileno)) == NULL)
		return -1;

	if (fileno > STDERR) {
		/*=====( ファイルの日付設定 )======*/
		if (p->fdsk)
			rc = fdskbios_set_fdate(p->handle, fdate);
		else
			rc = filebios_set_fdate(p->handle, fdate);

		return rc;
	}
	else {
		return -1;
	}
}


/*================================================
;|
;|□tell                《現在のリードライト位置の取得》
;|  ［形式］#include <misc/lowlevel.h>
;|          long _tell(int fileno);
;|  ［引数］fileno…ﾌｧｲﾙ番号
;|  ［戻値］≧0:現在のリードライト位置, ＝-1L:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を ferrno にセットするので
;|            エラーコードの詳細は「lseek」関数を参照の事。
;|
=================================================*/
long tell(int fileno)
{
	if (fileno > STDERR) {
		return lseek(fileno, 0L, SEEK_CUR);
	}
	else {
		return -1L;
	}
}


/*================================================
;|
;|□filelength          《ファイルの大きさの取得》
;|  ［形式］#include <misc/lowlevel.h>
;|          long filelength(int fileno);
;|  ［引数］fileno…ﾌｧｲﾙ番号
;|  ［戻値］ファイルの大きさ(=-1L:エラー)
;|  ［Note］・エラーを検出した場合はエラーの種類を ferrno にセットする。
;|                0x01:無効なﾌｧﾝｸｼｮﾝ   ,0x05:ｱｸｾｽが否定された
;|                0x06:無効なﾊﾝﾄﾞﾙ     ,0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
=================================================*/
long filelength(int fileno)
{
	long pos, size;

	if ((pos = tell(fileno)) == -1) {
		return -1;
	}
	if ((size = lseek(fileno, 0, SEEK_END)) == -1) {
		return -1;
	}
	if (lseek(fileno, pos, SEEK_SET) == -1) {
		return -1;
	}

	return (size);
}


/*================================================
;|
;|□unlink              《ファイルを削除する》
;|  ［形式］#include <misc/lowlevel.h>
;|          short unlink(char *path);
;|  ［引数］path…ファイルネームのポインタ
;|  ［戻値］＝0:正常, =-1:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                0x02:ﾌｧｲﾙが見つからない ,0x03:ﾒﾃﾞｨｱがみつからない
;|                0x05:ｱｸｾｽが拒否された   ,0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
short unlink(char *path)
{
	if (fdskfile_check(path))
		return fdskbios_unlink(path);
	else
		return filebios_unlink(curdrv_trim(path));
}


/*================================================
;|
;|□findfirst            《ファイル名の検索(初回)》
;|  ［形式］#include <misc/lowlevel.h>
;|          short findfirst(char *path, T_FCB *buff, short attr);
;|  ［引数］path…ファイルネームのポインタ
;|          buff…データバッファーの先頭アドレス
;|          attr…属性
;|  ［戻値］＝0:正常, =-1:エラー
;|  ［Note］・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|            H'01:無効なﾌｧﾝｸｼｮﾝ      ,H'05:ｱｸｾｽが拒否された
;|                0x03:ﾒﾃﾞｨｱがみつからない,0x12:一致するﾌｧｲﾙがない
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
short findfirst(char *path, T_FCB *buff, short attr)
{
	if (fdskfile_check(path))
		return fdskbios_findfirst(path, buff, attr);
	else
		return filebios_findfirst(curdrv_trim(path), buff, attr);
}


/*================================================
;|
;|□findnext            《ファイル名の検索(2回目以降)》
;|  ［形式］#include <misc/lowlevel.h>
;|          short findnext(T_FCB *buff);
;|  ［引数］buff…データバッファーの先頭アドレス
;|  ［戻値］＝0:正常, =-1:エラー
;|  ［Note］・buffはfindfirstで獲得したﾃﾞｰﾀﾊﾞｯﾌｧｰ領域をそのまま渡す事。
;|          ・エラーを検出した場合はエラーの種類を _ferrno にセットする。
;|                H'01:無効なﾌｧﾝｸｼｮﾝ      ,H'05:ｱｸｾｽが拒否された
;|                0x03:ﾒﾃﾞｨｱがみつからない,0x12:一致するﾌｧｲﾙがない
;|                0x7F:通信異常(ﾀｲﾑｱｳﾄ)
;|
 ================================================*/
short findnext(T_FCB *buff)
{
	if (memcmp(buff->id, "FDSK", 4) == 0)
		return fdskbios_findnext(buff);
	else
		return filebios_findnext(buff);
}


/*================================================
;|
;|□file_exist          《ファイルの存在(サイズ)をチェック》
;|  ［形式］#include <misc/lowlevel.h>
;|          long file_exist(char *path);
;|  ［引数］path…ファイルネームのポインタ
;|  ［戻値］ファイルの大きさ(=-1L:エラー)
;|  ［Note］・指定のファイルがない時にも｢-1｣を戻す。
;|
 ================================================*/
long file_exist(char *path)
{
	short rc;
	T_FCB _fcb;

	if (fdskfile_check(path))
		rc = fdskbios_findfirst(path, &_fcb, 0);
	else
		rc = filebios_findfirst(curdrv_trim(path), &_fcb, 0);

	if (rc < 0)
		return -1;							/* 指定のファイルがない */

	return LD_ULONG(_fcb.fsize);			/* ファイルサイズ */
}


/*================================================
;|
;|□disk_get_free       《ディスクの空きスペースを得る》
;|  ［形式］#include <misc/lowlevel.h>
;|          long disk_get_free(char *path);
;|  ［引数］path…ファイルパス
;|  ［戻値］空きバイト数(=-1L:エラー)
;|  ［Note］・
;|
 ================================================*/
long disk_get_free(char *path)
{
	if (fdskfile_check(path))
		return fdskbios_get_free();
	else
		return filebios_get_free();
}


/*----------------------------------------------------------------------
#|History of "lowlevel.c"
#|=======================
#|* 2016/02/17	新規作成(by T.Yokobayashi)
#|  "sh_std/lib/lowlevel.c"を流用
#|
*/
