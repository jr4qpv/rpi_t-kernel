/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 * 	@(#)filedir.c (libmisc) 2016/02/19
 * 	FDIR関係ﾗｲﾌﾞﾗﾘｰ
 */

#include <basic.h>
#include <misc/libmisc.h>
#include <misc/lowlevel.h>


/*******( 定数の定義 )*******/
#define	FDATE_CHAR_SIZE		16		/* Ex. 2016-01-06 15:30 */



/*================================================
;|
;|□mcpy_fdate          《パックされたファイル日付文字データをメモリ格納する》
;|  ［形式］#include <misc/libmisc.h>
;|          int mcpy_fdate(char *buff, long fdate);
;|  ［引数］buff …格納先のポインタ(17byte以上の領域が必要)
;|          fdata…パックされたファイル日付データ
;|  ［戻値］出力した文字数
;|  ［Note］・メモリに出力する
;|
=================================================*/
int mcpy_fdate(char *buff, long fdate)
{
	return csprintf(buff, "%04d-%02d-%02d %02d:%02d", 
				(short)((fdate >> 25) & 0x7f) + 1980,		/* 年 */
				(short)((fdate >> 21) & 0xf),				/* 月 */
				(short)((fdate >> 16) & 0x1f),				/* 日 */
				(short)((fdate >> 11) & 0x1f),				/* 時 */
				(short)((fdate >> 5) & 0x3f) );				/* 分 */
}


/*================================================
;|
;|□disp_fdate          《パックされたファイル日付データを表示する》
;|  ［形式］#include <misc/libmisc.h>
;|         void disp_fdate(char disp, long fdate);
;|  ［引数］fdata…パックされたファイル日付データ
;|          disp …表示指定(0:しない,1:標準出力,2:ｴﾗｰ出力)
;|  ［戻値］なし
;|  ［Note］・表示後に改行は行わない
;|
=================================================*/
void disp_fdate(char disp, long fdate)
{
	char buff[FDATE_CHAR_SIZE + 2];

	mcpy_fdate(buff, fdate);
	cxprintf(disp, "%s", buff);
}


#if 0	/////////////////
/*================================================
;|
;|□mcpy_fileinf　　　　《ファイル情報をメモリへ書き込む》
;|　［形式］#include <sh_std.h>
;|　　　　　void mcpy_fileinf(char *buff, struct FCB *finf);
;|　［引数］buff…メモリのポインタ(42Byte以上の領域が必要)
;|　　　　　finf…FCBのポインタ
;|　［戻値］なし
;|　［Note］・改行文字は出力しない
;|
=================================================*/
void mcpy_fileinf(char *buff, struct FCB *finf)
{
	short i;
	long ldata;
	char nrmfn[MAXFNAME];

	ldata = ledn_ldl(finf->fsize);
	get_nrmfname(finf->fname, nrmfn);	/* 正規化したﾌｧｲﾙ名の獲得 */
	i = csprintf(buff, "%-14s%10d ", nrmfn, ldata); /*ﾌｧｲﾙ名, ﾌｧｲﾙｻｲｽﾞ*/

	ldata = xtrct_word(finf->fdate, finf->ftime);
	mcpy_fdate(&buff[i], ldata);

	return;
}
#endif	/////////////////


/*================================================
;|
;|□disp_fileinf        《ファイル情報を表示する》
;|  ［形式］#include <misc/libmisc.h>
;|          void disp_fileinf(char disp, T_FCB *finf);
;|  ［引数］finf…FCBのポインタ
;|          disp…表示指定(0:しない,1:標準出力,2:ｴﾗｰ出力)
;|  ［戻値］なし
;|  ［Note］・表示後に改行は行わない
;|
=================================================*/
void disp_fileinf(char disp, T_FCB *finf)
{
	long ldata;

	ldata = LD_ULONG(finf->fsize);
	cxprintf(disp, "%-14s%10d ", finf->fname, ldata);	/* ﾌｧｲﾙ名, ﾌｧｲﾙｻｲｽﾞ */

	ldata = (finf->fdate)<<16 | (USHORT)(finf->ftime);
	disp_fdate(disp, ldata);				/* タイムスタンプの表示 */

	return;
}


/*================================================
;|
;|□file_directory      《ファイル名一覧を表示する》
;|  ［形式］#include <misc/libmisc.h>
;|          int file_directory(char *path, char disp, char wopt);
;|  ［引数］path…パス名
;|          disp…削除メッセージ(0:しない,1:標準出力,2:ｴﾗｰ出力)
;|          wopt…ワイド一覧形式で表示(0:しない, 1:する)
;|  ［戻値］表示したﾌｧｲﾙ個数
;|  ［Note］・disp=0を指定する事で、ﾌｧｲﾙの存在個数を獲得できる。
;|
=================================================*/
int file_directory(char *path, char disp, char wopt)
{
	int rc, cnt;
	long size;
	T_FCB buf;

	cnt = 0;
	size = 0;
	rc = findfirst(path, &buf, 0);			/* ファイル名の検索(初回) */
	while (rc == 0) {
///		if (disp) {
///			if (kbhit_esc()) {				/* 中断? */
///				return cnt;
///			}
///		}

		cnt++;
		size += LD_ULONG(buf.fsize);		/* ﾘﾄﾙｴﾝﾃﾞｨｱﾝで獲得 */

		disp_fileinf(disp, &buf);
		cxprintf(disp, "\n");

		rc = findnext(&buf);
	};

	if (cnt)
		cxprintf(disp, "%6d files %11d byte exists.\n", cnt, size);
	else
		cxprintf(disp, "file not found.\n");

///	set_ferrno(0);
	return cnt;
}


/*----------------------------------------------------------------------
#|History of "filedir.c"
#|======================
#|* 2016/02/17	新規作成(by T.Yokobayashi)
#|  "sh_std/lib/fdir.c"を流用
#|
*/
