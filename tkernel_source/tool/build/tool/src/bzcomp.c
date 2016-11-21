/*
 *	bzcomp.c  09-03-23
 *
 *	実行プログラムの圧縮/展開
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

/* レコード圧縮処理 (comp.c) */
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

LOCAL	ENDIAN	OutEndian;	/* 出力先のエンディアン */
LOCAL	ENDIAN	CpuEndian;	/* このプログラムを実行している
				   プロセッサのエンディアン */

/* エンディアン変換 */
inline CEW( UW x )
{
	return ( CpuEndian != OutEndian )? swapW(x): x;
}

/* エンディアンチェック */
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
LOCAL	BOOL	SysProc;	/* システムプロセス */
LOCAL	BOOL	PlainData;	/* データのみ (Bz ヘッダーなし) */

/*
	圧縮レコード書き込み
*/
#define	MAGIC1	CEW(BzMAGIC1)	/* マジックナンバ1 */
#define	MAGIC1S	CEW(BzMAGIC1S)	/* マジックナンバ1 (システムプロセス) */
#define	MAGIC2	CEW(BzMAGIC2)	/* マジックナンバ2 */
#define	MAGIC3	CEW(BzMAGIC3)	/* マジックナンバ3 */

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

	/* ８バイト単位に切り上げ */
	wBufLen = (sz + 7) & ~7;

	/* バッファ獲得 */
	wBuf = malloc(wBufLen * 2);
	if (wBuf == NULL) {
		P("%s: No Memory\n", fn);
		er = -1;
		goto EEXIT;
	}
	rBuf = wBuf + wBufLen;

	/* レコード一括読み込み */
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
		/* 圧縮レコードヘッダ作成 */
		bhd = (Bz_HDR *)wBuf;
		bhd->magic1 = ( SysProc )? MAGIC1S: MAGIC1;
		bhd->magic2 = MAGIC2;
		bhd->magic3 = MAGIC3;
		bhd->expsz = CEW(sz);
		wLen += sizeof(Bz_HDR);
	}

	/* 圧縮書き込み初期化 */
	CompWriteInit(WriteFn);

	/* 圧縮処理(一括) */
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

	/* 圧縮レコード書き込み */
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
	展開レコード書き込み
*/
LOCAL	W	UnCompRec(char *fn, W fd, W sz)
{
	W	er, ofs;
	B	*rBuf;
	Bz_HDR	*bhd;

	wBuf = NULL;

	/* バッファ獲得 */
	rBuf = malloc(sz);
	if (rBuf == NULL) {
		P("%s: No Memory\n", fn);
		er = -1;
		goto EEXIT;
	}

	/* レコード一括読み込み */
	if (lseek(fd, 0, SEEK_SET) < 0
	   || read(fd, rBuf, sz) != sz) {
		P("%s: Can't Read (%s)\n", fn, strerror(errno));
		goto EEXIT;
	}

	if (PlainData) {	/* 圧縮レコードヘッダ無し: 10倍サイズとする */
		wLen = sz * 10;	
		ofs = 0;
	} else {		/* 圧縮レコードヘッダチェック */
		bhd = (Bz_HDR *)rBuf;
		if (! ((bhd->magic1 == MAGIC1 || bhd->magic1 == MAGIC1S)
			&& bhd->magic2 == MAGIC2 && bhd->magic3 == MAGIC3) ) {
			P("%s: Not Compressed\n", fn);
			goto EEXIT;
		}
		wLen = CEW(bhd->expsz);
		ofs = sizeof(Bz_HDR);
	}

	/* バッファ獲得 */
	wBuf = malloc(wLen);
	if (wBuf == NULL) {
		P("%s: No Memory\n", fn);
		er = -1;
		goto EEXIT;
	}

	/* 圧縮展開処理(一括) */
	er = UnCompBuf((UB*)wBuf, (UB*)rBuf + ofs, wLen, sz - ofs);
	if (er != 0) {
		if (PlainData && er > 0) {
			wLen -= er;	/* 展開後の実サイズ */
		} else {
			P("%s: Can't UnCompress\n", fn);
			goto EEXIT;
		}
	}

	/* 展開レコード書き込み */
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
	メイン処理
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

	/* オプションの取り出し */
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
