/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Added by T-Engine Forum at 2012/11/27.
 *    Modified by T-Engine Forum at 2013/02/28.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/02/10.
 *
 *----------------------------------------------------------------------
 */

/*
 *  @(#)cprintf.c (libmisc)  2016/02/19
 *  ｺﾝﾊﾟｸﾄ版 printf関数
 *
 *	printf() / sprintf() routine using t-monitor function.
 *
 *	- Unsupported specifiers: floating point, long long and others.
 *		Coversion:	 a, A, e, E, f, F, g, G, n
 *		Size qualifier:  hh, ll, j, z, t, L
 *	- No limitation of output string length.
 *	- Minimize stack usage.
 *		Depending on available stack size, define OUTBUF_SZ by
 *		appropriate value.
 */

#include <basic.h>
#include <misc/libmisc.h>

#define	CPRINTF_SELECT	1		/* =1:tm_printf()流用, =0:従来cprintf流用 */

//extern	int	tm_putchar(int c);			////// debug //////
//extern	int	tm_putstring(uchar *s);		////// debug //////


/*=============================================================
;|
;|□cprintf,csprintf,ceprintf  《ｺﾝﾊﾟｸﾄ版 printf ライブラリ》
;|  ［機能］簡易型の printf ライブラリ
;|  ［形式］#include <misc/libmisc.h>
;|          int cprintf(char *format, ...);
;|          int ceprintf(char *format, ...);
;|          int cxprintf(char disp, char *format, ...);
;|          int csprintf(char *d, char *format, ...);
;|  ［引数］printf関数に準ずる
;|          使用できるパラメータ : %-0wl(duxXcs)
;|  ［戻値］printf関数に準ずる
;|  ［Note］・一部制限があるが標準ライブラリのprintf関数を使用するより
;|            プログラムサイズが小さい。
;|          ・各関数の出力先を下記に示す。
;|                cprintf    …標準出力
;|                csprintf   …メモリ
;|                ceprintf   …エラー出力(ﾒﾝﾃﾎﾟｰﾄ)
;|          ・cxprintfのdispには出力先(0:しない,1:標準出力,2:ｴﾗｰ出力)を指定。
;|
 =============================================================*/

#if CPRINTF_SELECT
/*===================================================================
	tm_printf()流用版
 ====================================================================*/

/* Output Buffer size in stack */
#define	OUTBUF_SZ	0

typedef	unsigned char	uchar;


/* Output function */
typedef	struct {
	short	len;		/* Total output length */
	short	cnt;		/* Buffer counts */
	uchar	*bufp;		/* Buffer pointer for csprintf */
} OutPar;
typedef	void	(*OutFn)(uchar *str, int len, OutPar *par);

/*
 *	Output integer value
 */
static	uchar	*outint(uchar *ep, unsigned long val, uchar base)
{
static const uchar  digits[] = "0123456789abcdef0123456789ABCDEF";
	uchar	caps;

	caps = (base & 0x40) >> 2;		/* 'a' or 'A' */
	for (base &= 0x3F; val >= base; val /= base) {
		*--ep = digits[(val % base) + caps];
	}
	*--ep = digits[val + caps];
	return ep;				/* buffer top pointer */
}

/*
 *	Output with format (limitted version)
 */
static	void	_vsprintf(OutFn ostr, OutPar *par,
					const uchar *fmt, va_list ap)
{
#define	MAX_DIGITS	14
	unsigned long	v;
	short	wid, prec, n;
	uchar	*fms, *cbs, *cbe, cbuf[MAX_DIGITS];
	uchar	c, base, flg, sign, qual;

/* flg */
#define	F_LEFT		0x01
#define	F_PLUS		0x02
#define	F_SPACE		0x04
#define	F_PREFIX	0x08
#define	F_ZERO		0x10

	for (fms = NULL; (c = *fmt++) != '\0'; ) {

		if (c != '%') {	/* Fixed string */
			if (fms == NULL) fms = (uchar*)fmt - 1;
			continue;
		}

		/* Output fix string */
		if (fms != NULL) {
			(*ostr)(fms, fmt - fms - 1, par);
			fms = NULL;
		}

		/* Get flags */
		for (flg = 0; ; ) {
			switch (c = *fmt++) {
			case '-': flg |= F_LEFT;	continue;
			case '+': flg |= F_PLUS;	continue;
			case ' ': flg |= F_SPACE;	continue;
			case '#': flg |= F_PREFIX;	continue;
			case '0': flg |= F_ZERO;	continue;
			}
			break;
		}

		/* Get field width */
		if (c == '*') {
			wid = va_arg(ap, int);
			if (wid < 0) {
				wid = -wid;
				flg |= F_LEFT;
			}
			c = *fmt++;
		} else {
			for (wid = 0; c >= '0' && c <= '9'; c = *fmt++)
				wid = wid * 10 + c - '0';
		}

		/* Get precision */
		prec = -1;
		if (c == '.') {
			c = *fmt++;
			if (c == '*') {
				prec = va_arg(ap, int);
				if (prec < 0) prec = 0;
				c = *fmt++;
			} else {
				for (prec = 0;c >= '0' && c <= '9';c = *fmt++)
					prec = prec * 10 + c - '0';
			}
			flg &= ~F_ZERO;		/* No ZERO padding */
		}

		/* Get qualifier */
		qual = 0;
		if (c == 'h' || c == 'l') {
			qual = c;
			c = *fmt++;
		}

		/* Format items */
		base = 10;
		sign = 0;
		cbe = &cbuf[MAX_DIGITS];	/* buffer end pointer */

		switch (c) {
		case 'i':
		case 'd':
		case 'u':
		case 'X':
		case 'x':
		case 'o':
			if (qual == 'l') {
				v = va_arg(ap, unsigned long);
			} else {
				v = va_arg(ap, unsigned int);
				if (qual == 'h') {
					v = (c == 'i' || c == 'd') ?
						(short)v :(unsigned short)v;
				}
			}
			switch (c) {
			case 'i':
			case 'd':
				if ((long)v < 0) {
					v = - (long)v;
					sign = '-';
				} else if ((flg & F_PLUS) != 0) {
					sign = '+';
				} else if ((flg & F_SPACE) != 0) {
					sign = ' ';
				} else {
					break;
				}
				wid--;		/* for sign */
			case 'u':
				break;
			case 'X':
				base += 0x40;	/* base = 16 + 0x40 */
			case 'x':
				base += 8;	/* base = 16 */
			case 'o':
				base -= 2;	/* base = 8 */
				if ((flg & F_PREFIX) != 0 && v != 0) {
					wid -= (base == 8) ? 1 : 2;
					base |= 0x80;
				}
				break;
			}
			/* Note: None outputs when v == 0 && prec == 0 */
			cbs = (v == 0 && prec == 0) ?
						cbe : outint(cbe, v, base);
			break;
		case 'p':
			v = (unsigned long)va_arg(ap, void *);
			if (v != 0) {
				base = 16 | 0x80;
				wid -= 2;
			}
			cbs = outint(cbe, v, base);
			break;
		case 's':
			cbe = cbs = va_arg(ap, uchar *);
			if (prec < 0) {
				while (*cbe != '\0') cbe++;
			} else {
				while (--prec >= 0 && *cbe != '\0') cbe++;
			}
			break;
		case 'c':
			cbs = cbe;
			*--cbs = (uchar)va_arg(ap, int);
			prec = 0;
			break;
		case '\0':
			fmt--;
			continue;
		default:
			/* Output as fixed string */
			fms = (uchar*)fmt - 1;
			continue;
		}

		n = cbe - cbs;				/* item length */
		if ((prec -= n) > 0) n += prec;
		wid -= n;				/* pad length */

		/* Output preceding spaces */
		if ((flg & (F_LEFT | F_ZERO)) == 0 ) {
			while (--wid >= 0) (*ostr)((uchar*)" ", 1, par);
		}

		/* Output sign */
		if (sign != 0) {
			(*ostr)(&sign, 1, par);
		}

		/* Output prefix "0x", "0X" or "0" */
		if ((base & 0x80) != 0) {
			(*ostr)((uchar*)"0", 1, par);
			if ((base & 0x10) != 0) {
				(*ostr)((base & 0x40) ?
					(uchar*)"X" : (uchar*)"x", 1, par);
			}
		}

		/* Output preceding zeros for precision or padding */
		if ((n = prec) <= 0) {
			if ((flg & (F_LEFT | F_ZERO)) == F_ZERO ) {
				n = wid;
				wid = 0;
			}
		}
		while (--n >= 0) (*ostr)((uchar*)"0", 1, par);

		/* Output item string */
		(*ostr)(cbs, cbe - cbs, par);

		/* Output tailing spaces */
		while (--wid >= 0) (*ostr)((uchar*)" ", 1, par);
	}

	/* Output last fix string */
	if (fms != NULL) {
		(*ostr)(fms, fmt - fms - 1, par);
	}
#if	OUTBUF_SZ > 0
	/* Flush output */
	(*ostr)(NULL, 0, par);
#endif
}


/*---------------------------------------------------------
 *	Output to console
 */
static	void	out_cons(uchar *str, int len,  OutPar *par)
{
#if	OUTBUF_SZ == 0
	/* Direct output to console */
	par->len += len;
	while (--len >= 0) cputchar(*str++);		/* 標準コンソール出力 */
#else
	/* Buffered output to console */
	if (str == NULL) {	/* Flush */
		if (par->cnt > 0) {
			par->bufp[par->cnt] = '\0';
			cputstring(par->bufp);				/* 標準コンソール出力 */
			par->cnt = 0;
		}
	} else {
		par->len += len;
		while (--len >= 0) {
			if (par->cnt >= OUTBUF_SZ - 1) {
				par->bufp[par->cnt] = '\0';
				cputstring(par->bufp);			/* 標準コンソール出力 */
				par->cnt = 0;
			}
			par->bufp[par->cnt++] = *str++;
		}
	}
#endif
}

int	cvprintf(const char *format, va_list ap)
{
#if	OUTBUF_SZ == 0
	short	len = 0;
	_vsprintf(out_cons, (OutPar*)&len, (const uchar *)format, ap);
	return len;
#else
	char	obuf[OUTBUF_SZ];
	OutPar	par;

	par.len = par.cnt = 0;
	par.bufp = obuf;
	_vsprintf(out_cons, (OutPar*)&par, (const uchar *)format, ap);
	va_end(ap);
	return par.len;
#endif	
}

int	cprintf(const char *format, ...)
{
	va_list	ap;
	int	len;

	va_start(ap, format);
	len = cvprintf(format, ap);
	va_end(ap);

	return len;
}


/*---------------------------------------------------------
 *	Output to error console
 */
static	void	out_err(uchar *str, int len,  OutPar *par)
{
#if	OUTBUF_SZ == 0
	/* Direct output to console */
	par->len += len;
	while (--len >= 0) eputchar(*str++);		/* エラーコンソール出力 */
#else
	/* Buffered output to console */
	if (str == NULL) {	/* Flush */
		if (par->cnt > 0) {
			par->bufp[par->cnt] = '\0';
			eputstring(par->bufp);				/* エラーコンソール出力 */
			par->cnt = 0;
		}
	} else {
		par->len += len;
		while (--len >= 0) {
			if (par->cnt >= OUTBUF_SZ - 1) {
				par->bufp[par->cnt] = '\0';
				eputstring(par->bufp);			/* エラーコンソール出力 */
				par->cnt = 0;
			}
			par->bufp[par->cnt++] = *str++;
		}
	}
#endif
}

int	cevprintf(const char *format, va_list ap)
{
#if	OUTBUF_SZ == 0
	short	len = 0;
	_vsprintf(out_err, (OutPar*)&len, (const uchar *)format, ap);
	return len;
#else
	char	obuf[OUTBUF_SZ];
	OutPar	par;

	par.len = par.cnt = 0;
	par.bufp = obuf;
	_vsprintf(out_err, (OutPar*)&par, (const uchar *)format, ap);
	va_end(ap);
	return par.len;
#endif	
}

int	ceprintf(const char *format, ...)
{
	va_list	ap;
	int	len;

	va_start(ap, format);
	len = cevprintf(format, ap);
	va_end(ap);

	return len;
}


/*---------------------------------------------------------
 *	Output select (dispは出力先指定 0:しない,1:標準出力,2:ｴﾗｰ出力)
 */
int cxprintf(char disp, const char *format, ...)
{
	va_list	ap;
	int	len;

	va_start(ap, format);
	switch (disp) {
	case 1:
		len = cvprintf(format, ap);		/* 標準コンソール出力 */
		break;
	case 2:
		len = cevprintf(format, ap);	/* エラーコンソール出力 */
		break;
	default:							/* 出力しない */
		len = 0;
		break;
	}
	va_end(ap);

	return len;
}


/*---------------------------------------------------------
 *	Output to buffer
 */
static	void	out_buf(uchar *str, int len, OutPar *par)
{
	par->len += len;
	while (--len >= 0) *(par->bufp)++ = *str++;
}

int	cvsprintf(char *str, const char *format, va_list ap)
{
	OutPar	par;

	par.len = 0;
	par.bufp = (uchar*)str;
	_vsprintf(out_buf, &par, (const uchar *)format, ap);
	str[par.len] = '\0';
	return par.len;
}

int	csprintf(char *str, const char *format, ...)
{
	va_list	ap;
	int	len;

	va_start(ap, format);
	len = cvsprintf(str, format, ap);
	va_end(ap);

	return len;
}




#else	/* CPRINTF_SELECT */
/*===================================================================
	従来cprintf()流用版
 ====================================================================*/
typedef	unsigned char	uchar;
typedef	unsigned short	ushort;
typedef	unsigned int	uint;
typedef	unsigned long	ulong;


/*******( CPWの定義 …cprintf work変数 )*******/
struct CPW {
	char  out_flag ;		/* 出力先フラグ  0:*out_p0, 1:stdout, 2:errout */
	char  f, z, l ;			/* 書式指定フラグ */
	char  cbuff[12] ;		/* 数値文字列バッファ */
	short w ;
	short char_num ;		/* プリント文で出力した文字数 */
	uchar *out_p ;
};



/* １文字出力（バッファ付き） */
static void put_char(struct CPW *wkp, char c)
{
	switch (wkp->out_flag) {
	case 0:
	    /* *out_p 出力の場合 */
		if (!(*wkp->out_p++ = c))
			return;
		wkp->char_num++ ;
		return;
	case 1:
		/* stdout 出力の場合 */
		if (!c) return ;
		if (c=='\n') cputchar('\r') ;
		cputchar(c) ;
		wkp->char_num++ ;
		return;
	case 2:
		/* errout 出力の場合 */
		if (!c) return ;
		if (c=='\n') eputchar('\r') ;
		eputchar(c) ;
		wkp->char_num++ ;
		return;
	}
}


/* 数字チェック */
static int isdigit(char c)
{
    return '0'<=c && c<='9' ;
}


/* 数値（ulong型）を文字列（10進数）に変換する */
static int p_lnum(char *buff, ulong data)
{
    int i ;

    i = 0 ;
    do {
		*buff-- = (char)(data % 10 + '0') ;
		data /= 10 ;
		i++ ;
    } while (data) ;
    return i ;
}


/* 数値（ulong型）を文字列（16進数）に変換する */
static int p_lhex(char *buff, ulong data, const char *code)
{
    int i ;

    i = 0 ;
    do {
		*buff-- = code[data & 0xF] ;
		data >>= 4 ;
		i++ ;
    } while (data) ;
    return i ;
}


static void print_sub(struct CPW *wkp, char *buff, int size)
{
    char spc ;
    int  i, sign ;

    if (*buff == '-' && wkp->z) {
		put_char(wkp, '-') ;
		buff++ ;
		size-- ;
		sign = 1 ;
    }
    else
		sign = 0 ;

    spc = (char)(wkp->z ? '0' : ' ') ;
    if(!wkp->f)
	for(i=0 ; i<(wkp->w-size-sign) ; i++)
	    put_char(wkp, spc) ;

    for(i=0 ; i<size ; i++)
		put_char(wkp, *buff++) ;

    if(wkp->f)
	for(i=0 ; i<(wkp->w-size-sign) ; i++)
	    put_char(wkp, ' ') ;
}


/* 符号付き10進出力(long) */
static void print_ld(struct CPW *wkp, long data)
{
    int  sign, size ;

    if(data<0) {
		sign = 1 ;
		data = -data ;
    }
    else
		sign = 0 ;

    size = p_lnum(&wkp->cbuff[11], data) ;

    if(sign)
		wkp->cbuff[11-size++] = '-' ;

    print_sub(wkp, &wkp->cbuff[12-size], size) ;
}


/* 符号なし10進出力(long) */
static void print_lu(struct CPW *wkp, ulong data)
{
    int  size ;

    size = p_lnum(&wkp->cbuff[11], data) ;
    print_sub(wkp, &wkp->cbuff[12-size], size) ;
}


/* 16進出力（小文字）(long) */
static void print_lx(struct CPW *wkp, ulong data)
{
    static const char code[] = "0123456789abcdef" ;
    int  size ;

    size = p_lhex(&wkp->cbuff[11], data, code) ;
    print_sub(wkp, &wkp->cbuff[12-size], size) ;
}


/* 16進出力（大文字）(long) */
static void print_lX(struct CPW *wkp, ulong data)
{
    static const char code[] = "0123456789ABCDEF" ;
    int  size ;

    size = p_lhex(&wkp->cbuff[11], data, code) ;
    print_sub(wkp, &wkp->cbuff[12-size], size) ;
}


/* １文字出力 */
static void print_c(struct CPW *wkp, char c)
{
    print_sub(wkp, (char *)&c, 1) ;
}


/* 文字列出力 */
static void print_s(struct CPW *wkp, uchar *str)
{
    int size ;

	if (str) {
    	for(size=0 ; str[size] ; size++) ;	/* size←文字数を求める */
    	print_sub(wkp, (char *)str, size) ;
	}
	else {
		print_sub(wkp, "(null)", 6);		/* strがNULLの時 */
	}
}


static int cprintf_main(struct CPW *wkp, const uchar *p, va_list ap)
{
    uchar c ;

    if (*p == 0) return 0 ;
    wkp->char_num = 0 ;		/* 出力文字数 <- 0 */
    for(;;) {
	switch(c = *p++) {
	    case '\0' :
			put_char(wkp, 0) ;
			return(wkp->char_num) ;
	    case '%'  :
			wkp->f = wkp->z = wkp->l = 0 ;
			wkp->w = 0 ;
			c = *p ;
			if (c == '%') { put_char(wkp, c); break ; }
			if (c == '-') wkp->f = 1, c = *++p ;
			if (c == '0') wkp->z = 1, c = *++p ;
			if (isdigit(c)) {
			    wkp->w = c - '0' ;
			    while(isdigit(c = *++p))
				wkp->w = wkp->w * 10 + c - '0' ;
			}
			if (c == 'l') wkp->l = 1, c = *++p ;
			switch(c) {
			    case 'd' :
					print_ld(wkp, va_arg(ap, long)) ;	break ;
			    case 'u' :
					print_lu(wkp, va_arg(ap, ulong)) ;	break ;
			    case 'x' :
					print_lx(wkp, va_arg(ap, ulong)) ;	break ;
			    case 'X' :
					print_lX(wkp, va_arg(ap, ulong)) ;	break ;
			    case 'c' :
					print_c(wkp, va_arg(ap, long)) ;	break ;
			    case 's' :
					print_s(wkp, va_arg(ap, uchar *)) ;	break ;
			    default  :
					continue ;
			}

			p++ ;
			break ;
	    default   :
			put_char(wkp, c) ;
			break ;
		}
    }
}



int csprintf(char *d, const char *format, ...)
{
	va_list ap;
	int len;
	struct CPW wk;							/* 作業用変数 */

	wk.out_flag = 0 ;						/* string */
	wk.out_p = (uchar *)d ;
	va_start(ap, format);
	len = cprintf_main(&wk, (uchar *)format, ap) ;
	va_end(ap);

	return len;
}

int cprintf(const char *format, ...)
{
	va_list ap;
	int len;
	struct CPW wk;							/* 作業用変数 */

	wk.out_flag = 1 ;						/* stdout */
	va_start(ap, format);
	len = cprintf_main(&wk, (uchar *)format, ap) ;
	va_end(ap);

	return len;
}

int ceprintf(const char *format, ...)
{
	va_list ap;
	int len;
	struct CPW wk;							/* 作業用変数 */

	wk.out_flag = 2 ;						/* errout */
	va_start(ap, format);
	len = cprintf_main(&wk, (uchar *)format, ap) ;
	va_end(ap);

	return len;
}

int cxprintf(char disp, const char *format, ...)
{
	va_list ap;
	int len;
	struct CPW wk;							/* 作業用変数 */

	switch (disp) {
	  case 1:	wk.out_flag = 1 ; break;	/* stdout */
	  case 2:	wk.out_flag = 2 ; break;	/* errout */
	  default:	return 0;					/* 表示しない */
	}
	va_start(ap, format);
	len = cprintf_main(&wk, (uchar *)format, ap) ;
	va_end(ap);

	return len;
}

#endif	/* CPRINTF_SELECT */


/*----------------------------------------------------------------------
#|History of "cprintf.c"
#|======================
#|* 2016/02/05	新規作成(by T.Yokobayashi)
#|  "sh_std/lib/cprintf.c"を流用
#|* 2016/02/06	可変個引数の扱いをva_listに変更。
#|* 2016/02/10	"{V2}/lib/libtm/src/tm_printf.c"から流用し、CPRINTF_SELECTで
#|  切り換えられるようにし、従来版のソースは念の為残しておく。
#|
*/
