/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(c) 2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)filebios.h (libmisc) 2016/02/19
 */

#ifndef __FILEBIOS_H_DEF
#define __FILEBIOS_H_DEF 	1


#ifndef  __FCB_STRUCT_DEF
#define  __FCB_STRUCT_DEF	1
typedef struct t_fcb {					/*** ファイル制御ブロック ***/
	/* OSが使用(合計21byte)…FDSK用 */
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


/********( 基本ﾌｧｲﾙ入出力ｲﾝﾀﾌｪｰｽ関係 )********/
#define BIOS_O_RD			0			/* 読み込みモード */
#define BIOS_O_WR			1			/* 書き込みモード */
#define BIOS_O_RDWR			2			/* 読み書きモード */

#define BIOS_SEEK_SET		0			/* ファイルの先頭 */
#define	BIOS_SEEK_CUR		1			/* 現在位置 */
#define	BIOS_SEEK_END		2			/* ファイルの終わり */


#if 1	///////////// kari ///////////
extern int _file_init(long timo);
extern int _file_creatnew(char *path);
extern int _file_unlink(char *path);
extern int _file_open(char *path, int oflag);
extern int _file_close(int fd);
extern long _file_lseek(int fd, long offset, int ptrname);
extern int _file_findfirst(char *path, T_FCB *buff, int attr);
extern int _file_findnext(T_FCB *buff);
extern int _file_read(int fd, void *buf, unsigned short n);
extern int _file_write(int fd, void *buf, unsigned short n);
extern long _file_get_fdate(int fd);
extern int _file_set_fdate(int fd, long fdate);
extern long _file_get_free(void);


extern short _ferrno;
#define	set_ferrno(code)	{ _ferrno = (code); }



/********( FILEBIOSコールマクロ )********/
#define	filebios_open		_file_open
#define	filebios_creatnew	_file_creatnew
#define	filebios_close		_file_close
#define	filebios_read		_file_read
#define	filebios_write		_file_write
#define	filebios_lseek		_file_lseek
#define	filebios_get_fdate	_file_get_fdate
#define	filebios_set_fdate	_file_set_fdate
#define	filebios_unlink		_file_unlink
#define	filebios_findfirst	_file_findfirst
#define	filebios_findnext	_file_findnext
#define	filebios_get_free	_file_get_free


/********( FDSKBIOSコールマクロ )********/
#define	fdskbios_open		_file_open
#define	fdskbios_creatnew	_file_creatnew
#define	fdskbios_close		_file_close
#define	fdskbios_read		_file_read
#define	fdskbios_write		_file_write
#define	fdskbios_lseek		_file_lseek
#define	fdskbios_get_fdate	_file_get_fdate
#define	fdskbios_set_fdate	_file_set_fdate
#define	fdskbios_unlink		_file_unlink
#define	fdskbios_findfirst	_file_findfirst
#define	fdskbios_findnext	_file_findnext
#define	fdskbios_get_free	_file_get_free




#else	/////////////////////
/********( FILEBIOSコールマクロ )********/
#define	filebios_open(path,mode)	(short)trapa_svc(__FILE_BIOS, __FILE_OPEN_SVC, (char *)path, BIOS_O_RD)

#define	filebios_creatnew(path)		(short)trapa_svc(__FILE_BIOS, __FILE_CREATNEW_SVC, (char *)path)

#define	filebios_close(fd)			(short)trapa_svc(__FILE_BIOS, __FILE_CLOSE_SVC, (short)fd)

#define	filebios_read(fd,buf,n)		(short)trapa_svc(__FILE_BIOS, __FILE_READ_SVC, (short)fd, (void *)buf, (unsigned short)n)

#define	filebios_write(fd,buf,n)	(short)trapa_svc(__FILE_BIOS, __FILE_WRITE_SVC, (short)fd, (void *)buf, (unsigned short)n)

#define	filebios_lseek(fd,offset,ptrname)	(long)trapa_svc(__FILE_BIOS, __FILE_LSEEK_SVC, (short)fd, (long)offset, (short)ptrname)

#define	filebios_get_fdate(fd)		(long)trapa_svc(__FILE_BIOS, __FILE_GET_FDATE_SVC, (short)fd)

#define	filebios_set_fdate(fd,fdate)	(long)trapa_svc(__FILE_BIOS, __FILE_SET_FDATE_SVC, (short)fd, (long)fdate)

#define	filebios_unlink(path)		(short)trapa_svc(__FILE_BIOS, __FILE_UNLINK_SVC, (char *)path)

#define	filebios_findfirst(path,buff,attr)	(short)trapa_svc(__FILE_BIOS, __FILE_FINDFIRST_SVC, (char *)path, (struct FCB *)buff, (short)attr)

#define	filebios_findnext(buff)		(short)trapa_svc(__FILE_BIOS, __FILE_FINDNEXT_SVC, (struct FCB *)buff)

#define	filebios_get_free()			(long)trapa_svc(__FILE_BIOS, __FILE_GET_FREE_SVC)



/********( FDSKBIOSコールマクロ )********/
#define	fdskbios_open(path,mode)	(short)trapa_svc(__FDSK_BIOS, __FILE_OPEN_SVC, (char *)path, BIOS_O_RD)

#define	fdskbios_creatnew(path)		(short)trapa_svc(__FDSK_BIOS, __FILE_CREATNEW_SVC, (char *)path)

#define	fdskbios_close(fd)			(short)trapa_svc(__FDSK_BIOS, __FILE_CLOSE_SVC, (short)fd)

#define	fdskbios_read(fd,buf,n)		(short)trapa_svc(__FDSK_BIOS, __FILE_READ_SVC, (short)fd, (void *)buf, (unsigned short)n)

#define	fdskbios_write(fd,buf,n)	(short)trapa_svc(__FDSK_BIOS, __FILE_WRITE_SVC, (short)fd, (void *)buf, (unsigned short)n)

#define	fdskbios_lseek(fd,offset,ptrname)	(long)trapa_svc(__FDSK_BIOS, __FILE_LSEEK_SVC, (short)fd, (long)offset, (short)ptrname)

#define	fdskbios_get_fdate(fd)		(long)trapa_svc(__FDSK_BIOS, __FILE_GET_FDATE_SVC, (short)fd)

#define	fdskbios_set_fdate(fd,fdate)	(long)trapa_svc(__FDSK_BIOS, __FILE_SET_FDATE_SVC, (short)fd, (long)fdate)

#define	fdskbios_unlink(path)		(short)trapa_svc(__FDSK_BIOS, __FILE_UNLINK_SVC, (char *)path)

#define	fdskbios_findfirst(path,buff,attr)	(short)trapa_svc(__FDSK_BIOS, __FILE_FINDFIRST_SVC, (char *)path, (struct FCB *)buff, (short)attr)

#define	fdskbios_findnext(buff)		(short)trapa_svc(__FDSK_BIOS, __FILE_FINDNEXT_SVC, (struct FCB *)buff)

#define	fdskbios_get_free()			(long)trapa_svc(__FDSK_BIOS, __FILE_GET_FREE_SVC)
#endif	//////////////////////////////




#if 0	////////////////
/*--------------------------------*/
/* Multi-byte word access macros  */

#if _WORD_ACCESS == 1	/* Enable word access to the FAT structure */
#define	LD_WORD(ptr)		(WORD)(*(WORD*)(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(*(DWORD*)(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(WORD*)(BYTE*)(ptr)=(WORD)(val)
#define	ST_DWORD(ptr,val)	*(DWORD*)(BYTE*)(ptr)=(DWORD)(val)
#else					/* Use byte-by-byte access to the FAT structure */
#define	LD_WORD(ptr)		(WORD)(((WORD)*((BYTE*)(ptr)+1)<<8)|(WORD)*(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(((DWORD)*((BYTE*)(ptr)+3)<<24)|((DWORD)*((BYTE*)(ptr)+2)<<16)|((WORD)*((BYTE*)(ptr)+1)<<8)|*(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8)
#define	ST_DWORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8); *((BYTE*)(ptr)+2)=(BYTE)((DWORD)(val)>>16); *((BYTE*)(ptr)+3)=(BYTE)((DWORD)(val)>>24)
#endif
#endif	////////////////



#endif	/* __FILEBIOS_H_DEF */


/*----------------------------------------------------------------------
#|History of "filebios.h"
#|=======================
#|* 2016/02/17	新規作成(By T.Yokobayashi)
#|  "sh_std/bios/filebios.h"を流用
#|
*/
