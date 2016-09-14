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
 *    Modified by T.Yokobayashi at 2016/02/18.
 *
 *----------------------------------------------------------------------
 */

/*
 *	line_drv.h	Console/Low level serial I/O driver
 *
 *		Low-level driver common definition : System-independent
 */

#include <basic.h>
#include <device/serialio.h>
#include <device/rs.h>
#include <tk/tkernel.h>
#include <tk/util.h>
#include <sys/imalloc.h>
#include <sys/util.h>
#include <libstr.h>

#define	DEF_INBUFSZ	2048		/* 標準受信バッファサイズ */
#define	MIN_INBUFSZ	256			/* 最低受信バッファサイズ */
#define OUBUFSZ		512			/* 送信バッファサイズ：固定 */

/* エラー情報 */
typedef union {
	RsError	c;
	RsStat	s;
	ER	w;
} RsErr;

/* フロー制御状態 */
typedef	struct {
	UB	reqchar:8;				/* 送信要求文字 */
	UW	rsoff:1;				/* RS = OFF で受信停止状態 */
	UW	sndxoff:1;				/* XOFF を送信して受信停止状態 */
} FlowState;

/* コントローラ操作関数群 */
struct _L_INFO;
typedef struct {
	ER	(*in)(struct _L_INFO *li, UB *buf, W len, W *alen, W tmout);
	ER	(*out)(struct _L_INFO *li, UB *buf, W len, W *alen, W tmout);
	ER	(*ctl)(struct _L_INFO *li, W kind, UW *arg);
	void	(*up)(struct _L_INFO *li);
	void	(*down)(struct _L_INFO *li);
} SC_FUNC;

/* コントローラ情報 */
typedef struct {
	SC_FUNC	*fn;				/* コントローラ操作関数群 */
	RsHwConf_16450	c;			/* ポート HW コンフィグレーション */
	UB	fctl;					/* ポート HW レジスタ情報 */
	UB	dt[3];					/* ポート HW その他の情報 */
	RsMode	mode;				/* Default line mode */
	RsFlow	flow;				/* Default flow control mode */
	FlowState flowsts;			/* Default flow control state */
} SC_DEFS;

/* シリアル回線管理情報 */
typedef struct _L_INFO {
	SC_DEFS	scdefs;				/* コントローラ操作関数定義 */
	UW	suspend:1;				/* サスペンド状態のとき 1 */
	UW	enbint:1;				/* 割り込み許可状態のとき 1 */

	FastMLock lock;				/* 入出力用ロック */
	ID	flg;					/* 入出力割り込み待ち用フラグ */

	RsMode	mode;				/* 回線モード */
	RsFlow	flow;				/* フロー制御モード */
	FlowState flowsts;			/* フロー制御状態 */

	FUNCP	extfn;				/* 外部関数 */
	UW	extpar;					/* 外部関数パラメータ */
	UB	lsts;					/* 現在の回線エラー状態 */
	UB	lstshist;				/* 回線エラー履歴状態 */
	UB	msts;					/* 現在のモデム状態 */

	UW	in_rptr;				/* 入力バッファ読み出しポインタ */
	UW	in_wptr;				/* 入力バッファ書き込みポインタ */
	UW	in_bufsz;				/* 入力バッファサイズ */
	UB	*in_buf;				/* 入力バッファエリアの先頭 */

	UW	ou_rptr;				/* 出力バッファ読み出しポインタ */
	UW	ou_wptr;				/* 出力バッファ書き込みポインタ */

	UW	ou_cnt;					/* 出力データカウンタ */
	UW	ou_len;					/* 出力データァサイズ */
	UB	ou_buf[OUBUFSZ];		/* 出力バッファ */
} LINE_INFO;

#define	XOFF_MARGIN	64			/* XOFF を送信する残りサイズ */
#define	XON_MARGIN	128			/* XON を送信する残りサイズ */

/* 送信バッファポインタマスク */
#define	OU_PTRMASK(p, ptr)	((ptr) % OUBUFSZ)

/* 受信バッファポインタマスク */
#define	PTRMASK(p, ptr)		((ptr) % ((p)->in_bufsz))

/* 受信バッファの残りサイズ */
#define	IN_BUF_REMAIN(p)	((p->in_rptr >= p->in_wptr) ?		\
				 (p->in_rptr - p->in_wptr) :		\
				 (p->in_rptr + p->in_bufsz - p->in_wptr))

/* 受信バッファの受信済みサイズ */
#define	IN_BUF_SIZE(p)		((p->in_wptr >= p->in_rptr) ?		\
				 (p->in_wptr - p->in_rptr) :		\
				 (p->in_wptr + p->in_bufsz - p->in_rptr))

/* 制御コード */
#define	XOFF		('S'-'@')
#define	XON		('Q'-'@')

/* リターンコード */
typedef enum {
	RTN_OK		= 0,
	RTN_NONE	= -1,
	RTN_TMOUT	= -2,
	RTN_ABORT	= -3,
	RTN_ERR		= -4
} RTN;
typedef	W	WRTN;

/* 制御フラグのパターン */
#define	OU_LOCK		17
#define	IN_LOCK		16
#define	FLG_OU_ABORT	(1 << 3)
#define	FLG_OU_NORM	(1 << 2)
#define	FLG_IN_ABORT	(1 << 1)
#define	FLG_IN_NORM	(1 << 0)
#define	FLG_OU_WAIPTN	(FLG_OU_NORM | FLG_OU_ABORT)
#define	FLG_IN_WAIPTN	(FLG_IN_NORM | FLG_IN_ABORT)

/* メモリー獲得／解放 */
#define	Malloc(len)	(void*)Imalloc(len)
#define	Free(ptr)	Ifree((void*)(ptr))

/* シリアル回線管理情報 */
IMPORT	W		nPorts;
IMPORT	LINE_INFO	*LineInfo;
IMPORT	W		DebugPort;

/* デバッグポートであれば True */
#define	isDebugPort(li)		( (li) == &LineInfo[DebugPort] )

/* ロック */
IMPORT	ER	consMLock(FastMLock *lock, INT no);
IMPORT	ER	consMUnlock(FastMLock *lock, INT no);
IMPORT	ER	consCreateMLock(FastMLock *lock, UB *name);
IMPORT	ER	consDeleteMLock(FastMLock *lock);


/*----------------------------------------------------------------------
#|History of "line_drv.h"
#|=======================
#|* 2016/07/08	[app_rzt1]用に、[tef_em1d]用の"line_drv.h"を参考に作成。
#|* 2016/07/08	ｺﾒﾝﾄ日本語化など修正。
*/
