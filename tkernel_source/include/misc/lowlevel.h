/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(c) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)lowlevel.h (libmisc) 2012/02/19
 */

#ifndef __LOWLEVEL_H_DEF
#define __LOWLEVEL_H_DEF 	1

#ifndef  __FCB_STRUCT_DEF
#define  __FCB_STRUCT_DEF	1
typedef struct t_fcb {					/*** ファイル制御ブロック ***/
	/* OSが使用(合計21byte)…FDSK管理用,findfirst/findnext時のﾜｰｸ領域 */
	short dummy[2];						/* 0xA01E, 0x0009 (BRA +40H) */
	char id[4];							/* "FDSK" */

	short fsrev;						/* ﾌｧｲﾙｼｽﾃﾑ･ﾚﾋﾞｼﾞｮﾝ */
	short link;							/* ﾘﾝｸ･ｾｸﾀ番号(=0:end) */
	short seqno;						/* ﾌｧｲﾙ内連続番号 */
	short topsct;						/* 先頭ｾｸﾀ番号 */
	void *paddr;						/* ﾃﾞｰﾀ先頭物理ｱﾄﾞﾚｽ */
	char dummy1;

	/* ファイル情報(合計22byte) */
	char fattr;							/* 属性 */
	short ftime;						/* 時刻 */
	short fdate;						/* 日付 */
	unsigned char fsize[4];				/* ファイルサイズ(ﾘﾄﾙｴﾝﾃﾞｨｱﾝ) */
	char fname[13];						/* ファイル名 */

	char dummy2[1];						/* 合計44byte */
} T_FCB;
#endif	/* __FCB_STRUCT_DEF */


/********( 低水準入出力ｲﾝﾀﾌｪｰｽ関係 )********/
/* ﾌｧｲﾙ番号 */
#define STDIN	0						/* 標準入力(ｺﾝｿｰﾙ) */
#define STDOUT	1						/* 標準出力(ｺﾝｿｰﾙ) */
#define STDERR	2						/* 標準ｴﾗｰ出力(ｺﾝｿｰﾙ) */

/* ﾌｧｲﾙのﾌﾗｸﾞ */
#define O_RDONLY	0x0001				/* 読み込み専用 */
#define O_WRONLY	0x0002				/* 書き込み専用 */
#define O_RDWR		0x0004				/* 読み書き両用 */
#define O_CREAT		0x0008				/* 新規作成(ﾌｧｲﾙがない時) */
#define O_TRUNC		0x0010				/* 新規作成 */
#define O_APPEND	0x0020				/* 追加作成 */

#define SEEK_SET	0					/* ファイルの先頭 */
#define	SEEK_CUR	1					/* 現在位置 */
#define	SEEK_END	2					/* ファイルの終わり */


#ifdef __cplusplus
extern "C" {
#endif

extern short fdskfile_check(char *path);

extern int open(char *path, int mode);
extern int close(int fileno);
extern int read(int fileno, char *buf, unsigned int count);
extern int write(int fileno, char *buf, unsigned int count);
extern long lseek(int fileno, long offset, int base);
extern long get_fdate(int fileno);
extern int set_fdate(int fileno, long fdate);
extern long _tell(int fileno);
extern long filelength(int fileno);

extern short unlink(char *path);
extern short findfirst(char *path, T_FCB *buff, short attr);
extern short findnext(T_FCB *buff);
extern long file_exist(char *path);
extern long disk_get_free(char *path);


/* ----- filedir.c ----- */
extern int mcpy_fdate(char *buff, long fdate);
extern void disp_fdate(char disp, long fdate);
extern void disp_fileinf(char disp, T_FCB *finf);
extern int file_directory(char *path, char disp, char wopt);



#ifdef __cplusplus
}
#endif

#endif	/* __LOWLEVEL_H_DEF */



/*----------------------------------------------------------------------
#|History of "lowlevel.h"
#|=======================
#|* 2016/02/17	新規作成(T.Yokobayashi)
#|  "sh_std/lib/lowlevel.h"を流用
#|
*/
