/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)libmisc.h (libmisc) 2016/11/14
 *
 *	Misc library for T-Kernel
 *
 */

#ifndef	__LIBMISC_H__
#define __LIBMISC_H__

#include <basic.h>
#include <stdarg.h>				/* for va_list */
////kari////#include <t2ex/stdint.h>
////#include <stdtype.h>

#ifdef __cplusplus
extern "C" {
#endif


/*** Misc標準の型定義 ***/
#ifndef _MISC_TYPEDEF
#define _MISC_TYPEDEF
/* もし以下を定義したくない時は、本inludeの前に｢_MISC_TYPEDEF｣を定義しておく */
typedef	unsigned char	BYTE;		/* unsigned 8bit integer */
typedef unsigned short	USHORT;		/* unsinged 16bit integer */
typedef	unsigned long	ULONG;		/* unsinged 32bit integer */
#endif	/* _MISC_TYPEDEF */


#if 0	////////////////
/********( 定数の定義 )********/
#define	APA_ADDR	0x8c800000			// APAアドレス	///////////
#define	FRB_ADDR	0x8c800400			// FRBアドレス	///////////
#define	_tpa		0x8c800800			// TPAアドレス	///////////
#endif	//////////////////


/*------( ｷｰｺｰﾄﾞの定義 )------*/
#define	BS_CODE		0x08
#define	LF_CODE		0x0A
#define	CR_CODE		0x0D
#define	CLS_CODE	0x1A
#define	ESC_CODE	0x1B
#define	DEL_CODE	0x7F

#ifndef	NULL
#define	NULL           ((void *)0)
#endif	/* NULL */

#ifndef	EOF
#define EOF		(-1)					/* File termination */
#endif	/* EOF */


#define	debug_trap()	Asm("trapa   #0x0b")

///#define	LD_WORD(ptr)		(WORD)(((WORD)*((BYTE*)(ptr)+1)<<8)|(WORD)*(BYTE*)(ptr))
#define	LD_ULONG(ptr)		(ULONG)(((ULONG)*((BYTE*)(ptr)+3)<<24)|((ULONG)*((BYTE*)(ptr)+2)<<16)|((USHORT)*((BYTE*)(ptr)+1)<<8)|*(BYTE*)(ptr))
///#define	ST_WORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8)
///#define	ST_ULONG(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8); *((BYTE*)(ptr)+2)=(BYTE)((DWORD)(val)>>16); *((BYTE*)(ptr)+3)=(BYTE)((DWORD)(val)>>24)


/* ----- initialize library ----- */
extern int init_libmisc(void);

/* ----- conio.c ----- */
extern int putch(int c);
extern int cputchar(int c);
extern int cputstring(const char *s);
extern int getch(void);
extern int cgetchar(void);
extern int kbhit(void);
extern void kbclr(void);
extern int cgetstring(char *s, int n);
extern char *cgets(char *buf);

/* ----- errio.c ----- */
extern int _putSIO(int c);
extern int eputchar(int c);
extern int eputstring(const char *s);
extern int _getSIO(long timo);
extern int egetchar(void);
extern int ekbhit(void);
extern void ekbclr(void);
extern int egetstring(char *s, int n);
extern char *egets(char *buf);
extern void rsflow_save_setraw(void);
extern void rsflow_restore(void);

/* ----- cprintf.c ----- */
extern int cvsprintf(char *str, const char *format, va_list ap);
extern int cvprintf(const char *format, va_list ap);
extern int cevprintf(const char *format, va_list ap);

extern int csprintf(char *d, const char *format, ...);
extern int cprintf(const char *format, ...);
extern int ceprintf(const char *format, ...);
extern int cxprintf(char disp, const char *format, ...);

#if 0	///////////////////////////////////
/* ----- keyin.c ----- */
extern int kbhit_esc(void);


/* ----- disp_mem.c ----- */
extern uint32_t mem_dumpx(uint32_t addr, long count, int size, char disp);
extern uint32_t mem_dump(uint32_t addr, long count, int size);
extern uint32_t mem_set(uint32_t addr, int size);

/* ----- cursor.c ----- */
extern void locate(int x, int y);
extern void cursor_off(void);
extern void cursor_on(void);
extern void cls(void);

/* ----- atohex.c ----- */
extern char atochex(const char *s);
extern short atoshex(const char *s);
extern long atolhex(const char *s);

/* ----- remote.c ----- */
extern int _remote_init(void);
extern int _remote_check(char mode);
extern int _remote_rx(int c, char mode);
extern int _remote_tx(char mode);





/* ----- load.c ----- */
////extern  ER doLoading( W proto, UW addr, UW range[3] );
extern  int doLoading( W proto, UW addr, UW range[3] );

/* load option (LOAD commands, etc.) */
#define	P_XMODEM	0x20		/* XMODEM */
#define	P_TEXT		0x10		/* no protocol */
#define	P_SFORM		0x02		/* S-Format */
#define	P_MEMIMG	0x01		/* memory image */


/* ----- file.c ----- */
extern long file_read(char *fname, void *addr, long limsz);

#endif	//////////////////////


#ifdef __cplusplus
}
#endif
#endif /* __LIBMISC_H__ */


/*----------------------------------------------------------------------
#|History of "libmisc.h"
#|======================
#|* 2016/02/06	New created.(By T.Yokobayashi)
#|
*/
