/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(c) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)file.c (libmisc) 2016/02/20
 *  ﾌｧｲﾙ操作関係関数
 */

#include <basic.h>
#include <misc/libmisc.h>
#include <misc/lowlevel.h>


/*******( 定数の定義 )*******/
#define DEBUG		1
#define TRANS_SIZE	0x400



/*================================================
;|
;|□file_check          《ファイルの存在(サイズ)をチェックする》
;|  ［形式］#include <misc/libmisc.h>
;|          long file_check(char *fname);
;|  ［引数］fname…ファイル名
;|  ［戻値］ﾌｧｲﾙﾊﾞｲﾄｻｲｽﾞ(=-1:異常)
;|  ［Note］・
;|
 ================================================*/
long file_check(char *fname)
{
	return file_exist(fname);
}


/*================================================
;|
;|□file_read           《ファイルの内容をメモリーへ読み込む》
;|  ［形式］#include <misc/libmisc.h>
;|          long file_read(char *fname, void *addr, long limsz);
;|  ［引数］fname…読み込むファイル名
;|          addr…読み込み先のメモリアドレス
;|          limsz…読み込み最大バイト数
;|  ［戻値］≧0:正常(ﾌｧｲﾙｻｲｽﾞ), ＜0:異常
;|  ［Note］・ファイルサイズがlimszを越えている場合はファイルの読み込みを行な
;|            わないで異常終了(-2)する。
;|          ・異常時の戻り値
;|              -1: file_existエラー
;|              -2: ﾌｧｲﾙｻｲｽﾞが大きすぎる
;|              -3: openエラー
;|              -4: readエラー
;|
 ================================================*/
long file_read(char *fname, void *addr, long limsz)
{
	int handle, ret_code;
	long fsize, len;

	/*-----( ファイルサイズのチェック )-----*/
	if ((fsize = file_check(fname)) == -1) {	/* ﾌｧｲﾙのｻｲｽﾞの獲得 */
#if DEBUG
		cprintf("\nfile not found!! fname=%s\n", fname);
#endif
		return -1;
	}
	if (fsize > limsz) {
		return -2;							/* ｴﾗｰｺｰﾄﾞのｾｯﾄ */
	}

	/*-----( ファイルの読み込み )-----*/
	if ((handle = open(fname, O_RDONLY)) < 0) {
#if DEBUG
		cprintf("\nopen error!! fname=%s\n", fname);
#endif
		return -3;
	}

#if DEBUG
	cprintf("file read(%s[", fname);
	disp_fdate(1, get_fdate(handle));
	cprintf(" %d].....", fsize);
#endif

	len = fsize;
	while (len > 0) {
		if (len >= TRANS_SIZE)
			ret_code = read(handle, addr, TRANS_SIZE);
		else
			ret_code = read(handle, addr, len);

		if (ret_code > 0) {
#if DEBUG
//			disp_hexw(len & 0xfe00); disp_left(4);
#endif
			addr = (void *)((unsigned)addr + TRANS_SIZE);
			len -= TRANS_SIZE;
		}
		else if (ret_code == 0) {
			break;							/* 正常終了 */
		}
		else {
			close(handle);
			return -4;
		}
    }
	close(handle);

#if DEBUG
	cprintf("complete.\n");
#endif

	return fsize;							/* 正常終了 */
}


#if 0	/////////////////
/*================================================
;|
;|□file_write_wd　　　 《論理メモリーの内容をファイルへ書き込む(日付指定)》
;|　［形式］#include <sh_std.h>
;|　　　　　short file_write_wd(char *fname, void *addr, long size,long fdate);
;|　［引数］fname…書き込み先のファイル名
;|　　　　　addr …書き込むメモリアドレス
;|　　　　　size …書き込むバイトサイズ
;|　　　　　fdate…ファイル日付情報（日付・時刻）
;|　［戻値］0:正常, ＜0:異常
;|　［Note］・fdateが-1の時には、指定の日付情報を書き込まない。
;|　　　　　・異常時の戻り値
;|　　　　　　　-1: openエラー
;|　　　　　　　-2: writeエラー
;|　　　　　　　-3: set_fdateエラー
;|
 ================================================*/
short file_write_wd(char *fname, void *addr, long size, long fdate)
{
	short fdsk;
	int handle, ret_code;

	if (fdate != -1)
		fdsk = fdskfile_check(fname);

	/*-----( 出力ファイルのオープン )-----*/
	if ((handle = open(fname, O_TRUNC)) < 0) {
		return -1;
	}
	if (fdate != -1) {
		if (fdsk)
			set_fdate(handle, fdate);		/* 日付のコピー */
	}

#if DEBUG
	disp_str("file write(");
	disp_str(fname);
	disp_str(").....");
#endif

	/*-----( ファイル書き込み )-----*/
	while (size > 0) {
		if (size >= TRANS_SIZE)
		    ret_code = write(handle, (char *)addr, TRANS_SIZE);
		else
			ret_code = write(handle, (char *)addr, size);

		if (ret_code > 0) {
#if DEBUG
			disp_hexw(size & 0xfe00); disp_left(4);
#endif
			addr = (void *)((unsigned)addr + TRANS_SIZE);
			size -= TRANS_SIZE;
		}
		else {
			close(handle);
			return -2;
		}
	}

	/*-----( 出力ファイルのクローズ )-----*/
	close(handle);
	if (fdate != -1) {
		if (!fdsk) {
			if ((handle = open(fname, O_RDONLY)) < 0)
				return -3;					/* 日付のコピーに失敗 */
			set_fdate(handle, fdate);		/* 日付のコピー */
			close(handle);
		}
	}

#if DEBUG
	disp_str("complete.\n");
#endif
	return 0;								/* 正常終了 */
}


/*================================================
;|
;|□file_write　　　　　《論理メモリーの内容をファイルへ書き込む》
;|　［形式］#include <sh_std.h>
;|　　　　　short file_write(char *fname, void *addr, long size);
;|　［引数］fname…書き込み先のファイル名
;|　　　　　addr…書き込むメモリアドレス
;|　　　　　size…書き込むバイトサイズ
;|　［戻値］0:正常, ＜0:異常
;|　［Note］・異常時の戻り値
;|　　　　　　　-1: openエラー
;|　　　　　　　-2: writeエラー
;|
 ================================================*/
short file_write(char *fname, void *addr, long size)
{
	return file_write_wd(fname, addr, size, -1);
}


/*================================================
;|
;|□remove　　　　　　　《ファイルを削除する》
;|　［形式］#include <sh_std.h>
;|　　　　　short remove(char *fname);
;|　［引数］fname…ファイルネームのポインタ
;|　［戻値］＝0:正常, =-1:エラー
;|　［Note］・本関数は､基本ﾌｧｲﾙ関数の｢unlink｣を呼び出す。
;|　　　　　・ｴﾗｰを検出した場合の _ferrno は｢unlink｣を参照の事。
;|
 ================================================*/
short remove(char *fname)
{
	return unlink(fname);
}


/*================================================
;|
;|□filebios_timeout_set《FILE_BIOSのタイムアウト時間の変更》
;|　［形式］#include <sh_std.h>
;|　　　　　int filebios_timeout_set(long timo);
;|　［引数］timo…ﾀｲﾑｱｳﾄ時間指定[1mS単位で指定]（=0:の時はﾃﾞﾌｫﾙﾄ値）
;|　［戻値］＝0:正常, ≠0:エラー
;|　［Note］・
;|
 ================================================*/
int filebios_timeout_set(long timo)
{
	return trapa_svc(__FILE_BIOS, 0, timo);		/* FILE_BIOSの初期化 */
}
#endif	/////////////////



/*----------------------------------------------------------------------
#|History of "file.c"
#|===================
#|* 2016/02/19	新規作成(by T.Yokobayashi)
#|  "sh_std/lib/file.c"を流用
#|
*/
