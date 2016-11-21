/*
	gterm.h		gterm : 定義ヘッダ

	Copyright (C) 1996-2007 by Personal Media Corporation
*/
#include	<sys/param.h>

/* プロトタイプ */
typedef	unsigned char	uchar;
typedef	unsigned short	uhalf;

/* デフォールト回線デバイス */
#if defined(BSD)
#define	DEF_LINE	"/dev/cuaa1"
#elif defined(__linux__)
#define	DEF_LINE	"/dev/ttyS1"
#elif defined(__CYGWIN__)
#define	DEF_LINE	"/dev/com2"
#else
#define	DEF_LINE	"/dev/ttyb"
#endif

/* デフォールトボーレート */
#define	DEF_BAUD	19200

/* デフォールトシェル */
#define	DEF_SHELL	"/bin/bash"

/* デフォールトログファイル名 */
#define	DEF_LOGFN	"gterm.log"

/* load コマンド使用 */
#define USE_LOADCMD	1

#define	LINE_LEN	2048			/* 1 行の文字数		*/

/* 漢字の文字の判断 */
#define issjisk1(c) ((c)>=0x81 && (c)<=0x9f || (c)>=0xe0 && (c) <= 0xfc)
#define issjisk2(c) ((c)>=0x40 && (c)<=0xfc && (c) != 0x7f)
#define iseucknj(c) ((c)>=0xa1 && (c)<=0xfe)

/* gterm.c */
void	Putc(uchar c);
void	Puts(uchar *s);
void	Print(char *fmt, ...);
int	Getc(int f);
int	RSgetc(int f, int tmout);
void	int_break(void);
FILE*	f_open(uchar *path, uchar *mode);
void	get_term(uchar *buf, uchar **pp);

/* devio.c */
int	isk1(int c);
int	isk2(int c);
int	TYinit(int use_stdin, int arg_ign_echo);
void	TYreinit(void);
void	TYflush(void);
void	TYclr(void);
void	TYout(uchar c);
void	TYouts(uchar *s);
uchar	TYin(void);
int	Keytest(uchar c);
int	checkBaud(int baud);
int	SOinit(uchar *line);
int	SOlocalinit(uchar *line);
void	SetCode(int sjis);
int	RSinit(uchar *line, int clen, int par, int sbit, int noflow, int baud);
void	RSclr(void);
int	RSin(void);
void	RSout(uchar c);
void	RSoutx(uchar c);
void	RSwrite(uchar *buf, int len);
void	RSputc(uchar c);
void	RSputs(uchar *s);
void	RSDrain(void);
int	PollRSTY(int tmout);
void	exam_echo(uchar *ptr, int d_size);
void	cur_addr(int l, int c);
int	escape(void);
void	sys_end(int i);
int	do_system(char *s);
void	sleep_ms(int n);
void	sleep_us(int n);

/* load.c */
int	do_load(int kind, uchar *p);

#define	kLOAD	(0x001)		/* .load コマンド */
#define	kFLLO	(0x002)		/* .flload コマンド */
#define	kRECV	(0x004)		/* recv 応答 */
#define	kMEMIMG	(0x100)		/* メモリイメージ形式 */
#define	kXMODEM	(0x200)		/* XMODEM 使用 */

/* sym.c */
void	symload(char *fn, char *p);
void	symeval(char *sym, char *p);

/* conv.c */
void	sjtoeuc(int *ph, int *pl);
void	euctosj(int *ph, int *pl);
