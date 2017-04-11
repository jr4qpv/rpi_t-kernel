/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)errio.c (libmisc) 2016/11/14
 *  エラー入出力 ﾙｰﾁﾝ（シリアルポート）
 */

#include <basic.h>
#include <device/serialio.h>
#include <device/rs.h>
#include <misc/libmisc.h>


extern int _libmisc_std_portno;



/*================================================
;|
;|□_putSIO             《RS-232Cポートに1Bite出力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int _putSIO(int c);
;|  ［引数］c…出力文字(cの下位バイトのみ有効)
;|  ［戻値］成功すると出力された文字cを返し、エラーの場合はEOFを返す。
;|  ［Note］・ｼﾘｱﾙI/Oﾄﾞﾗｲﾊﾞの｢serial_out｣をｺｰﾙしている。
;|          ・'\n'のｺｰﾄﾞはCR/LFへ変換しない。
;|
 ================================================*/
int _putSIO(int c)
{
	B	buf[1];
	W	alen;

	buf[0] = c;							// 出力文字
	if (serial_out(_libmisc_std_portno, buf, 1, &alen, TMO_FEVR) != 0)
		return EOF;						// エラー

	return (UB)c;						// 正常
}


/*================================================
;|
;|□eputchar            《エラーコンソールに文字を出力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int eputch(int c);
;|  ［引数］c…表示文字
;|  ［戻値］成功すると出力された文字cを返し、エラーの場合はEOFを返す。
;|  ［Note］・ｼﾘｱﾙI/Oﾄﾞﾗｲﾊﾞをｺｰﾙしている。
;|          ・'\n'(LF)のｺｰﾄﾞはCR/LFへ変換する。
;|
 ================================================*/
int eputchar(int c)
{
	if (c == '\n') _putSIO('\r');		// CR/LFへ変換
	return _putSIO(c);
}


/*================================================
;|
;|□eputstring          《 エラーコンソールに文字列を出力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int eputstring(const char *s);
;|  ［引数］str…表示文字列
;|  ［戻値］正常の場合は０、エラーの場合はEOFを返す。
;|  ［Note］・ｼﾘｱﾙI/Oﾄﾞﾗｲﾊﾞをｺｰﾙしている。
;|          ・'\n'(LF)のｺｰﾄﾞはCR/LFへ変換する。
;|
================================================*/
int eputstring(const char *s)
{
	int c;

	while ((c = *s++) != '\0') {
		if (eputchar(c) < 0) return EOF;
	}
	return 0;							// 正常
}


/*================================================
;|
;|□_getSIO             《RS-232Cポートから1Bite入力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int _getSIO(long wait);
;|  ［引数］wait…タイムアウト指定[m秒], <0でタイムアウトしない
;|  ［戻値］入力した文字（タイムアウト時はEOF）
;|  ［Note］・ｼﾘｱﾙI/Oﾄﾞﾗｲﾊﾞの｢serial_in｣をｺｰﾙしている。
;|          ・'\r'(CR)のｺｰﾄﾞ変換はしない。
;|          ・不正時はEOF(-1)を戻す。
;|
 ================================================*/
int _getSIO(long wait)
{
	B	buf[2];							// 1Byte余裕に確保
	W	alen;

	if (serial_in(_libmisc_std_portno, buf, 1, &alen, wait) != 0)	// 正常?
		return EOF;						// エラー

	return (UB)buf[0];					// 入力文字
}


/*================================================
;|
;|□egetchar            《エラーコンソールからｴｺｰﾊﾞｯｸなしで文字を入力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int egetchar(void);
;|  ［引数］なし
;|  ［戻値］入力した文字
;|  ［Note］・ｼﾘｱﾙI/Oﾄﾞﾗｲﾊﾞをｺｰﾙしている。
;|          ・'\r'(CR)のｺｰﾄﾞは'\n'(LF)へ変換する。
;|          ・不正時はEOF(-1)を戻す。
;|
 ================================================*/
int egetchar(void)
{
	int c;

	if ((c = _getSIO(TMO_FEVR)) == EOF)	// 1文字入力
		return EOF;
	if (c == '\r')
		c = '\n';						// CRをLFに変換

	return (UB)c;						// 入力文字
}


/*================================================
;|
;|□ekbhit              《エラー入力が押されたかどうかをﾁｪｯｸする》
;|  ［形式］#include <misc/libmisc.h>
;|          int ekbhit(void);
;|  ［引数］なし
;|  ［戻値］=0:ｷｰ入力なし, ≠0:ﾊﾞｯﾌｧｰに溜まってるﾃﾞｰﾀ数
;|  ［Note］・ｼﾘｱﾙI/Oﾄﾞﾗｲﾊﾞの｢serial_in｣をｺｰﾙしている。
;|
 ================================================*/
int ekbhit(void)
{
	W	alen;

	serial_in(_libmisc_std_portno, NULL, 0, &alen, 0);	// 受信済みバイト数獲得 */

	return alen;							// 受信済バイト数
}


/*================================================
;|
;|□ekbclr              《エラー入力ﾊﾞｯﾌｧｰをｸﾘｱする》
;|  ［形式］#include <misc/libmisc.h>
;|          void ekbclr(void);
;|  ［引数］なし
;|  ［戻値］なし
;|  ［Note］・エラー入力ﾊﾞｯﾌｧｰをｸﾘｱする
;|
 ================================================*/
void ekbclr(void)
{
	int n;

	n = ekbhit();
	for ( ; n>0; n--)
		egetchar();
}


/*================================================
;|
;|□egetstring          《エラーコンソールから文字列を入力する》
;|  ［形式］#include <misc/libmisc.h>
;|          int egetstring(char *s, int n);
;|  ［引数］s  …入力ﾊﾞｯﾌｧｰへのﾎﾟｲﾝﾀ
;|          n  …入力長さ(2以上を指定すること,'\0'を含む数)
;|  ［戻値］入力文字数('\0'は含まない)、エラー時はEOFを返す。
;|  ［Note］・ｼﾘｱﾙI/Oﾄﾞﾗｲﾊﾞをｺｰﾙしている。
;|          ・改行文字CR('\r')又はLF('\n')が入力されるまで読み込み、
;|            sに格納し、エコーバックする。
;|          ・改行文字CR('\r')又はLF('\n')は、s内ではヌル文字('\0')に
;|            置き換え、エコーバックもしない。
;|          ・BSで1文字削除
;|
 ================================================*/
int egetstring(char *s, int n)
{
	int c, i;

	i = 0;
	while (i < n) {
		if ((c = egetchar()) == EOF)
			return EOF;					// エラー
		if (c == '\n')					// CR,LF ?
			break;						// '\n'は格納しない

		if (c == BS_CODE || c == DEL_CODE)  {	// BS,DEL ?
			if (i > 0) {
				s--;
				i--;
				eputchar(BS_CODE);		// BS エコーバック 
				eputchar(' ');
				eputchar(BS_CODE);
			}
			continue;
		}

		eputchar(c);					// エコーバック
		*s = c;
		s++;
		i++;
	}
	eputchar('\n');						// CR+LF エコーバック
	*s = '\0';

	return i;							// 入力文字数
}


/*================================================
;|
;|□egets               《エラーコンソールから文字列を入力する》
;|  ［形式］#include <misc/libmisc.h>
;|          char *egets(char *buf);
;|  ［引数］buf…入力ﾊﾞｯﾌｧｰへのﾎﾟｲﾝﾀ
;|  ［戻値］buf[2]へのﾎﾟｲﾝﾀを返す
;|  ［Note］・ｼﾘｱﾙI/Oﾄﾞﾗｲﾊﾞをｺｰﾙしている。
;|          ・本関数をｺｰﾙする前に､buf[0]には読み込むべき文字列の最大長を
;|            設定しておく(ﾀｰﾐﾈﾀのNULLを含む文字数)。
;|          ・本関数の実行が終わるとbuf[1]には実際に読み込まれた文字数が
;|            設定される。読み込まれた文字はbuf[2]からはじまりNULLで終わる。
;|            従って、bufは最低でも3ﾊﾞｲﾄ以上は確保しておく事。
;|
 ================================================*/
char *egets(char *buf)
{
	int n, len;

	n = (UB)buf[0];							// 入力文字数
	if ((len = egetstring(&buf[2], n)) < 0)
		len = 0;
	buf[1] = len;

	return &buf[2];
}


/*================================================
;|
;|□rsflow_save_setraw  《シリアルポートのRsFlowを退避し、RAWモードに設定》
;|  ［形式］#include <misc/libmisc.h>
;|          void rsflow_save_setraw(void);
;|  ［引数］なし
;|  ［戻値］なし
;|  ［Note］・ﾓﾆﾀｺｰﾙの｢serial_ctl｣をｺｰﾙしている。
;|
 ================================================*/
static RsFlow rsflow_backup;

void rsflow_save_setraw(void)
{
	RsFlow rsflow;

	/* RsFlowの獲得＆退避 */
	serial_ctl(_libmisc_std_portno, -DN_RSFLOW, (UW *)&rsflow);
	rsflow_backup = rsflow;

	/* RAWモードに設定 */
	rsflow.rcvxoff = 0;					/* "XOFF" status/Compulsory change */
	rsflow.csflow = 0;					/* "CTS" control */
	rsflow.rsflow = 0;					/* "RTS" control */
	rsflow.xonany = 0;					/* "XON" by any character */
	rsflow.sxflow = 0;					/* Send "XON/XOFF" control */
	rsflow.rxflow = 0;					/* Receive "XON/XOFF" contorol */

	serial_ctl(_libmisc_std_portno, DN_RSFLOW, (UW *)&rsflow);
}


/*================================================
;|
;|□rsflow_restore      《シリアルポートのRsFlowを復帰》
;|  ［形式］#include <misc/libmisc.h>
;|          void rsflow_restore(void);
;|  ［引数］なし
;|  ［戻値］なし
;|  ［Note］・ﾓﾆﾀｺｰﾙの｢serial_ctl｣をｺｰﾙしている。
;|
 ================================================*/
void rsflow_restore(void)
{
	/* RsFlowの復帰 */
	serial_ctl(_libmisc_std_portno, DN_RSFLOW, (UW *)&rsflow_backup);
}


/*----------------------------------------------------------------------
#|History of "errio.c"
#|=====================
#|* 2016/02/10	New created.(By T.Yokobayashi)
#|  "sh_std/lib/errio.c"を参考に移植
#|
*/
