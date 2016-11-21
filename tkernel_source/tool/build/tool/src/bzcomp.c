/*
 *	bzcomp.c  09-03-23
 *
 *	�¹ԥץ����ΰ���/Ÿ��
 *	Copyright (C) 1997-2008 by Personal Media Corporation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "typedef.h"
#include "bzcomp.h"

#define P	printf

/* �쥳���ɰ��̽��� (comp.c) */
IMPORT	void	CompWriteInit(FUNCP wfn);
IMPORT	W	CompWrite(VP buf, W len);
IMPORT	W	UnCompBuf(UB *dst, UB *src, W dlen, W slen);

/* ------------------------------------------------------------------------- */

inline UH swapH( UH x )
{
	return (x << 8) | (x >> 8);
}
inline UW swapW( UW x )
{
	return (UW)swapH((UH)x) << 16 | swapH((UH)(x >> 16));
}

typedef enum { little, big } ENDIAN;

LOCAL	ENDIAN	OutEndian;	/* ������Υ���ǥ����� */
LOCAL	ENDIAN	CpuEndian;	/* ���Υץ�����¹Ԥ��Ƥ���
				   �ץ��å��Υ���ǥ����� */

/* ����ǥ������Ѵ� */
inline CEW( UW x )
{
	return ( CpuEndian != OutEndian )? swapW(x): x;
}

/* ����ǥ���������å� */
LOCAL void testendian( void )
{
	union {
		UB	b[2];
		UH	h;
	} x;

	x.h = 0x1234;
	CpuEndian = ( x.b[0] == 0x12 )? big: little;
}

/* ------------------------------------------------------------------------- */

LOCAL	BOOL	Verbose;
LOCAL	BOOL	SysProc;	/* �����ƥ�ץ��� */
LOCAL	BOOL	PlainData;	/* �ǡ����Τ� (Bz �إå����ʤ�) */

/*
	���̥쥳���ɽ񤭹���
*/
#define	MAGIC1	CEW(BzMAGIC1)	/* �ޥ��å��ʥ��1 */
#define	MAGIC1S	CEW(BzMAGIC1S)	/* �ޥ��å��ʥ��1 (�����ƥ�ץ���) */
#define	MAGIC2	CEW(BzMAGIC2)	/* �ޥ��å��ʥ��2 */
#define	MAGIC3	CEW(BzMAGIC3)	/* �ޥ��å��ʥ��3 */

LOCAL	B*	wBuf;
LOCAL	W	wLen;
LOCAL	W	wBufLen;

LOCAL	W	WriteFn(VP buf, W len)
{
	if (wLen + len > wBufLen) return -1000;
	memcpy(&wBuf[wLen], buf, len);
	wLen += len;
	return 0;
}
LOCAL	W	CompRec(char *fn, W fd, W sz)
{
	W	er, rsz;
	B	*rBuf;
	Bz_HDR	*bhd;

	/* ���Х���ñ�̤��ڤ�夲 */
	wBufLen = (sz + 7) & ~7;

	/* �Хåե����� */
	wBuf = malloc(wBufLen * 2);
	if (wBuf == NULL) {
		P("%s: No Memory\n", fn);
		er = -1;
		goto EEXIT;
	}
	rBuf = wBuf + wBufLen;

	/* �쥳���ɰ���ɤ߹��� */
	if (lseek(fd, 0, SEEK_SET) < 0
	 || read(fd, rBuf, sz) != sz) {
		P("%s: Can't Read (%s)\n", fn, strerror(errno));
		goto EEXIT;
	}

	bhd = (Bz_HDR *)rBuf;
	if ( (bhd->magic1 == MAGIC1 || bhd->magic1 == MAGIC1S)
	  && bhd->magic2 == MAGIC2 && bhd->magic3 == MAGIC3 ) {
		P("%s: Already Compressed\n", fn);
		goto EEXIT;
	}

	wLen = 0;

	if (!PlainData) {
		/* ���̥쥳���ɥإå����� */
		bhd = (Bz_HDR *)wBuf;
		bhd->magic1 = ( SysProc )? MAGIC1S: MAGIC1;
		bhd->magic2 = MAGIC2;
		bhd->magic3 = MAGIC3;
		bhd->expsz = CEW(sz);
		wLen += sizeof(Bz_HDR);
	}

	/* ���̽񤭹��߽���� */
	CompWriteInit(WriteFn);

	/* ���̽���(���) */
	er = CompWrite(rBuf, sz);
	if (er >= 0) er = CompWrite(rBuf, 0);
	if (er < 0) {
		if (er == -1000) {
			P("%s: Unchanged\n", fn);
			er = 0;
		} else {
			P("%s: Can't Compress (%d)\n", fn, er);
		}
		goto EEXIT;
	}

	/* ���̥쥳���ɽ񤭹��� */
	if (lseek(fd, 0, SEEK_SET) < 0
	 || write(fd, wBuf, wLen) != wLen) {
		P("%s: Can't Write Rec (%s)\n", fn, strerror(errno));
		goto EEXIT;
	}
	if (ftruncate(fd, wLen) < 0) {
		P("%s: Can't Truncate Rec (%s)\n", fn, strerror(errno));
		goto EEXIT;
	}

	if (Verbose) {
		P ("%s: Compress %d --> %d [%d%%]\n", fn, sz, wLen,
					wLen * 100 / sz);
	}
	er = 0;
EEXIT:
	if (wBuf != NULL) free(wBuf);
	return er;
}
/*
	Ÿ���쥳���ɽ񤭹���
*/
LOCAL	W	UnCompRec(char *fn, W fd, W sz)
{
	W	er, ofs;
	B	*rBuf;
	Bz_HDR	*bhd;

	wBuf = NULL;

	/* �Хåե����� */
	rBuf = malloc(sz);
	if (rBuf == NULL) {
		P("%s: No Memory\n", fn);
		er = -1;
		goto EEXIT;
	}

	/* �쥳���ɰ���ɤ߹��� */
	if (lseek(fd, 0, SEEK_SET) < 0
	   || read(fd, rBuf, sz) != sz) {
		P("%s: Can't Read (%s)\n", fn, strerror(errno));
		goto EEXIT;
	}

	if (PlainData) {	/* ���̥쥳���ɥإå�̵��: 10�ܥ������Ȥ��� */
		wLen = sz * 10;	
		ofs = 0;
	} else {		/* ���̥쥳���ɥإå������å� */
		bhd = (Bz_HDR *)rBuf;
		if (! ((bhd->magic1 == MAGIC1 || bhd->magic1 == MAGIC1S)
			&& bhd->magic2 == MAGIC2 && bhd->magic3 == MAGIC3) ) {
			P("%s: Not Compressed\n", fn);
			goto EEXIT;
		}
		wLen = CEW(bhd->expsz);
		ofs = sizeof(Bz_HDR);
	}

	/* �Хåե����� */
	wBuf = malloc(wLen);
	if (wBuf == NULL) {
		P("%s: No Memory\n", fn);
		er = -1;
		goto EEXIT;
	}

	/* ����Ÿ������(���) */
	er = UnCompBuf((UB*)wBuf, (UB*)rBuf + ofs, wLen, sz - ofs);
	if (er != 0) {
		if (PlainData && er > 0) {
			wLen -= er;	/* Ÿ����μ¥����� */
		} else {
			P("%s: Can't UnCompress\n", fn);
			goto EEXIT;
		}
	}

	/* Ÿ���쥳���ɽ񤭹��� */
	if (lseek(fd, 0, SEEK_SET) < 0
	    || write(fd, wBuf, wLen) != wLen) {
		P("%s: Can't Write Rec (%s)\n", fn, strerror(errno));
		goto EEXIT;
	}
	if (ftruncate(fd, wLen) < 0) {
		P("%s: Can't Truncate Rec (%s)\n", fn, strerror(errno));
		goto EEXIT;
	}

	if (Verbose) {
		P ("%s: UnCompress %d --> %d [%d%%]\n", fn, sz, wLen,
						wLen * 100 / sz);
	}
	er = 0;
EEXIT:
	if (wBuf != NULL) free(wBuf);
	if (rBuf != NULL) free(rBuf);
	return er;
}

/*
	�ᥤ�����
*/
EXPORT	int	main(int ac, char *av[])
{
	W	n, fd, sz, er;
	BOOL	uncomp;
	char	*s;

	Verbose = FALSE;
	SysProc = FALSE;
	uncomp = FALSE;
	OutEndian = little;

	testendian();

	/* ���ץ����μ��Ф� */
	for (av++; --ac > 0; av++) {
		s = *av;
		if (*s++ != '-') break;
		switch (*s++) {
		case 'v':	Verbose = TRUE;		break;
		case 's':	SysProc = TRUE;		break;
		case 'b':	OutEndian = big;	break;
		case 'p':	PlainData = TRUE;	break;
		case 'u':	uncomp = TRUE;		break;
		default:	goto USAGE;
		}
	}
	if (ac < 1) {
USAGE:		P("Usage: bzcomp [-v][-s][-b][-p][-u] file ...\n");
		P("       -v:Verbose\n");
		P("       -s:System Process\n");
		P("       -b:Big Endian\n");
		P("       -p:Plain Data\n");
		P("       -u:Uncompress\n");
		return -1;
	}

	er = 0;
	fd = -1;
	for (n = 0; n < ac; n++) {

		if (fd >= 0) close(fd);
		s = av[n];

		if ((fd = open(s, O_RDWR)) < 0) {
			P("%s: Can't Open (%s)\n", s, strerror(errno));
			continue;
		}

		if ((sz = lseek(fd, 0, SEEK_END)) < 0) {
			P("%s: Can't Seek (%s)\n", s, strerror(errno));
			continue;
		}
		if (uncomp) {
			er = UnCompRec(s, fd, sz);
		} else {
			er = CompRec(s, fd, sz);
		}
		if (er < 0) break;
	}
	if (fd >= 0) close(fd);

	return er;
}
