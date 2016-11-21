/*
	mkbtf.c		BTRON �ե���������ץ����

	(C) Copyright 1997-2008 by Personal Media Corporation
*/
#include	<stdio.h>
#include	<string.h>
#include	<strings.h>
#include	<stdlib.h>

typedef	int		INT;
typedef	int		LONG;
typedef	unsigned char	BYTE;
typedef	short		WORD;
typedef	unsigned short	UWORD;
typedef	unsigned short	TCODE;
typedef	void		VOID;
typedef	INT		(* FNP)();

#define	LOCAL		static

#define	Malloc(n)	malloc(n)
#define	Free(n)		free(n)

#define	P		fprintf
#define	sE		stderr

#define FNAME_MAX	20
#ifdef WIN
#define FOPEN_MODE_WRITE "wb"
#define NATIVE2TC	sjtotcs
#else
#define FOPEN_MODE_WRITE "w"
#define NATIVE2TC	etotcs
#endif

/*======================================================================
	����ǥ������Ѵ�
======================================================================*/
LOCAL	INT	BigEndian;

VOID	TestEndian()
{
	union {
		BYTE	b[2];
		UWORD	w;
	} x;

	x.w = 0xAA55;
	BigEndian = (x.b[0] == 0x55) ? 0 : 1;
}
UWORD	CnvW(UWORD d)
{
	if (BigEndian == 0) return d;
	return ((d << 8) & 0xff00) | ((d >> 8) & 0xff);
}
LONG	CnvL(LONG d)
{
	if (BigEndian == 0) return d;
	return (CnvW(d & 0xffff) << 16) | CnvW((d >> 16) & 0xffff);
}
static	TCODE	_asctotc[128] = {
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 
	0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F, 
	0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 
	0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
	0x2121, /* 20 SP */	0x212a, /* 21 �� */	0x2149, /* 22 �� */
	0x2174, /* 23 �� */	0x2170, /* 24 �� */	/*0x2173,*/
							0x2121,	 /* 25 �� */
	0x2175, /* 26 �� */	0x2147, /* 27 �� */	0x214a, /* 28 �� */
	0x214b, /* 29 �� */	0x2176, /* 2A �� */	0x215c, /* 2B �� */
	0x2124, /* 2C �� */	0x215d, /* 2D �� */	0x2125, /* 2E �� */
	0x213f, /* 2F �� */
	0x2330, /* 30 �� */	0x2331, /* 31 �� */	0x2332, /* 32 �� */
	0x2333, /* 33 �� */	0x2334, /* 34 �� */	0x2335, /* 35 �� */
	0x2336, /* 36 �� */	0x2337, /* 37 �� */	0x2338, /* 38 �� */
	0x2339, /* 39 �� */	0x2127, /* 3A �� */	0x2128, /* 3B �� */
	0x2163, /* 3C �� */	0x2161, /* 3D �� */	0x2164, /* 3E �� */
	0x2129, /* 3F �� */
	0x2177, /* 40 �� */	0x2341, /* 41 �� */	0x2342, /* 42 �� */
	0x2343, /* 43 �� */	0x2344, /* 44 �� */	0x2345, /* 45 �� */
	0x2346, /* 46 �� */	0x2347, /* 47 �� */	0x2348, /* 48 �� */
	0x2349, /* 49 �� */	0x234a, /* 4A �� */	0x234b, /* 4B �� */
	0x234c, /* 4C �� */	0x234d, /* 4D �� */	0x234e, /* 4E �� */
	0x234f, /* 4F �� */
	0x2350, /* 50 �� */	0x2351, /* 51 �� */	0x2352, /* 52 �� */
	0x2353, /* 53 �� */	0x2354, /* 54 �� */	0x2355, /* 55 �� */
	0x2356, /* 56 �� */	0x2357, /* 57 �� */	0x2358, /* 58 �� */
	0x2359, /* 59 �� */	0x235a, /* 5A �� */	0x214e, /* 5B �� */
	0x216f, /* 5C �� */	0x214f, /* 5D �� */	0x2130, /* 5E �� */
	0x2132, /* 5F �� */
	0x2146, /* 60 �� */	0x2361, /* 61 �� */	0x2362, /* 62 �� */
	0x2363, /* 63 �� */	0x2364, /* 64 �� */	0x2365, /* 65 �� */
	0x2366, /* 66 �� */	0x2367, /* 67 �� */	0x2368, /* 68 �� */
	0x2369, /* 69 �� */	0x236a, /* 6A �� */	0x236b, /* 6B �� */
	0x236c, /* 6C �� */	0x236d, /* 6D �� */	0x236e, /* 6E �� */
	0x236f, /* 6F ��*/
	0x2370, /* 70 �� */	0x2371, /* 71 �� */	0x2372, /* 72 �� */
	0x2373, /* 73 �� */	0x2374, /* 74 �� */	0x2375, /* 75 �� */
	0x2376, /* 76 �� */	0x2377, /* 77 �� */	0x2378, /* 78 �� */
	0x2379, /* 79 �� */	0x237a, /* 7A �� */	0x2150, /* 7B �� */
	0x2143, /* 7C �� */	0x2151, /* 7D �� */	0x2141, /* 7E �� */
	0x007F	/* 7F	 */
};
#ifdef	WIN
/*======================================================================
	�������Ѵ�: SJIS -> TCODE
======================================================================*/
INT	sjtotc(TCODE *tc, BYTE *sjis)
{
	UWORD	sc1, sc2;
	TCODE	c;

	if ((sc1 = *sjis++) < 0x80) {
		if (sc1 == '\0') return 0;
		if (sc1 != NULL) {
			c = _asctotc[sc1 & 0x7f];	/* Ⱦ�ѱѿ�ʸ�� */
			*tc = CnvW(c);
		}
		return (1);
	}
	if (sc1 >= 0xa0 && sc1 < 0xe0)
		return (-1);			/* Ⱦ�Ѥ���ʸ�� */

	sc1 -= (sc1 < 0xa0) ? 0x71 : 0xb1;	/* ����ʸ�� */
	sc1 = sc1 + sc1 + 1;
	if ((sc2 = *sjis) > 0x7f) sc2--;
	if (sc2 >= 0x9e) {
		sc1++;
		sc2 -= 0x7d;
	} else {
		sc2 -= 0x1f;
	}
	if (tc != NULL) *tc = (sc1 << 8) | (BYTE)sc2;
	return(2);
}
INT	sjtotcs(TCODE* tcstr, BYTE* sjisstr, INT maxlen, INT* rest)
{
	INT	tlen, len;

	for (tlen = 0; tlen < maxlen; tlen++) {
		if ((len = sjtotc(tcstr, sjisstr)) <= 0) break;
		sjisstr += len;
		tcstr++;
	}
	while (tlen++ < maxlen) *tcstr++ = 0;
	if (rest) *rest = *sjisstr != '\0';
	return	(len < 0) ? -1 : 0;
}
#else
/*======================================================================
	�������Ѵ�: EUC -> TCODE
======================================================================*/
INT	etotc(TCODE *tc, BYTE *euc)
{
	INT	ec;
	TCODE	c;

	if ((ec = *euc++) & 0x80) {
		if ((*euc & 0x80) == 0) return -1;
		c = ((ec << 8) | (*euc & 0xff)) & 0x7f7f;
		*tc = CnvW(c);
		return 2;
	}
	if (ec == 0) return 0;
	c = _asctotc[ec & 0x7f];
	*tc = CnvW(c);
	return 1;
}
INT	etotcs(TCODE* tcstr, BYTE* eucstr, INT maxlen, INT* rest)
{
	INT	tlen, len;

	for (tlen = 0; tlen < maxlen; tlen++) {
		if ((len = etotc(tcstr, eucstr)) <= 0) break;
		eucstr += len;
		tcstr++;
	}
	while (tlen++ < maxlen) *tcstr++ = 0;
	if (rest) *rest = *eucstr != '\0';
	return	(len < 0) ? -1 : 0;
}
#endif

#define TK_EXTEND_FNM_MARK 0xA121       /* ���̥ޡ��� */
#define L_EXTEND_FNM    34              /* �ե�����̾�κ���Ĺ(ʸ����) */

/**
 * ASCII���ϰϤǹ��������ʸ�����TC�˥ѥå������Ѵ����롣
 * @param tcname ������Хåե���maxlen�������ʤ����Τ�0�������롣
 * @param str ����ʸ����ASCII���ϰϤǤʤ���Фʤ�ʤ���
 * @param maxlen tcname ��Ĺ����
 * @param rest NULL�Ǥʤ������ͤ��᤹��str���Τ��Ѵ��Ǥ������ 0��
 * @return �Ѵ���̤�ʸ�������Ѵ��˼��Ԥ�������-1��
 */
LOCAL INT packAsciiFileName(TCODE* tcname, BYTE* str, INT maxlen, INT* rest)
{
        INT i, j;
        INT len;
        BYTE a1, a2;

        if (3 <= maxlen) {
                tcname[0] = _asctotc['U'];
                tcname[1] = _asctotc['X'];
                tcname[2] = TK_EXTEND_FNM_MARK;
        }

        /* 2 ʸ������ D ������˥ѥå� */
        for ( i = 3, j = 0;
              i < maxlen && (a1 = str[j]);
              i++, j += 2 ) {
                if ((a2 = str[j + 1])) {
                        /* �ϰϥ����å� */
                        if ((a1 < 0x20 || 0x7e < a1) ||
                            (a2 < 0x20 || 0x7e < a2)) {
                                return -1;
                        }
                        tcname[i] = ((a1 + 0x60) << 8) | (0x00ff & (a2 + 0x60));
                } else {
                        /* �Ǹ�� 1 ʸ����;�ä��� A ������ */
                        if (a1 < 0x20 || 0x7e < a1) {
                                return -1;
                        }
                        tcname[i++] = _asctotc [a1];
                        ++j;
                        break;
                }
        }
        len = i;

        while (i < maxlen)
                tcname[i++] = 0;
        if (rest) *rest = str[j] != '\0';
        return len;
}
/*======================================================================
	���̽񤭹���

	ñ��ʥ��饤�ɼ��������ˤ�밵�̥ץ����
	�������� �ΣΣΣ�	³��(�Ρܣ�)�Х��Ȥ���ϡʣ�����Ρ棰��
	�̣̣̣� �УУУ� �УУУ� �УУУ�
				�ХХ������ΰ��֤���(�̡ܣ�)�Х��Ȥ����
				�ʣ�����̡棲��
	�������� �̣̣̣� �̣̣̣� �УУУ� �УУУ� �УУУ�
				�ХХ������ΰ��֤���(�̡ܣ�)�Х��Ȥ����
				�ʣ�������̡棱����
======================================================================*/
#define	PBITS		12
#define	MATCH_MAX	256
#define	MATCH_POS	((1 << PBITS) - 2)
#define	UNMATCH_MAX	32
#define	BUFSZ		(1 << (PBITS + 1))
#define	BUFPOS(n)	((n) & (BUFSZ - 1))

LOCAL	FNP	WriteBytesFn;		/* �Х��Ƚ񤭹��ߴؿ�		*/
LOCAL	BYTE	wbuf[BUFSZ];		/* ���ϥ�󥰥Хåե�		*/
LOCAL	INT	tpos;			/* ���ϥХåե������Ƭ����	*/
LOCAL	INT	rpos;			/* ���ϥХåե�����ɤ߹��߰���	*/
LOCAL	INT	wpos;			/* ���ϥХåե���ν񤭹��߰���	*/
LOCAL	BYTE	que[UNMATCH_MAX + 4];	/* ���ϥ��塼			*/
LOCAL	INT	qpos;			/* ���ϥ��塼����		*/
#define	HASH_SZ	1024
LOCAL	WORD	Hash_s[HASH_SZ];	/* �ϥå���ơ��֥����Ƭ��	*/
LOCAL	WORD	Hash_e[HASH_SZ];	/* �ϥå���ơ��֥�ʺǸ��	*/
LOCAL	WORD	Hnxt[BUFSZ];		/* �ϥå���ꥹ�ȥơ��֥�	*/
LOCAL	INT	Hdmy;			/* ���ߡ��ϥå��奫�����	*/

/*
	�ϥå��女���ɤ����� (Ϣ³���� 3 �Х��Ȥ����)
*/
LOCAL	INT	MkHash(INT pos)
{
	return ((wbuf[pos] + (wbuf[BUFPOS(pos + 1)] << 1) +
			(wbuf[BUFPOS(pos + 2)] << 1)) & (HASH_SZ - 1));
}
/*
	�񤭹��ߤν����
*/
LOCAL	VOID	CompWriteInit(FNP wfn)
{
	WriteBytesFn = wfn;
	bzero(wbuf, MATCH_POS);
	tpos = 0;
	rpos = wpos = MATCH_POS;
	qpos = 0;
	Hdmy = MATCH_POS;

	/* �ϥå���ơ��֥�Υ��ꥢ */
	bzero(Hash_s, sizeof(Hash_s));
	bzero(Hash_e, sizeof(Hash_e));
	bzero(Hnxt, sizeof(Hnxt));
}
/*
	���̽���

	wbuf:	tpos	--> ���ϺѤߥǡ���  (tpos = rpos - MATCH_POS)
		rpos	--> ���ϳ��ϥǡ���
		wpos	--> ���Ͻ�λ�ǡ���
*/
LOCAL	INT	CompOut(INT fin)
{
	INT	i, k, cnt, er;
	INT	mlen;
	INT	mpos;

	for (er = 0;;) {
		/* rpos �� + MATCH_MAX (wpos) �Ȱ��פ�������ʸ�����
				tpos �� rpos (�� wpos) ���ϰϤǸ��Ĥ��� */
		/* �Ĥ�Х��ȿ� */
		cnt = BUFPOS(wpos + BUFSZ - rpos);

		/* ��λ�Ǥʤ��Ȥ��ϡ��ϥå�������������ݾڤ��뤿��ˡ�
			�Ĥ�Х��ȿ� �� - 3 �Ȥ��롣����ˡ�����ΰ��פ򸫤Ĥ�
			�뤿��ˡ��Ĥ�Х��ȿ� > MATCH_MAX �λ��Τ߽������� */
		if (fin == 0) {
			if ((cnt -= 3) <= MATCH_MAX) break;
		}

		mlen = 0;
		if (cnt > 3) {
			if (cnt > MATCH_MAX) cnt = MATCH_MAX;
			for (i = Hash_s[MkHash(rpos)]; --i >= 0; i = Hnxt[i]) {
				if (wbuf[BUFPOS(i + mlen)] !=
					wbuf[BUFPOS(rpos + mlen)]) continue;
				for (k = 0; k < cnt; k++) {
					if (wbuf[BUFPOS(i + k)] !=
					    wbuf[BUFPOS(rpos + k)]) break;
				}
				if (k <= mlen) continue;
				mpos = rpos - i;
				if ((mlen = k) >= MATCH_MAX) break;
			}
		}
		if (--mlen >= 2) {	/* ����ʸ���󤢤� */
			if (qpos) que[0] = qpos - 1;
			else	  qpos--;

			if (mpos < 0)	mpos += BUFSZ;
			if ((i = mlen) >= 15)
				que[++qpos] = 0xf0 | ((i >> 4) & 0xf);

			mpos |= (i & 0xf) << PBITS;
			que[++qpos] = (BYTE)(mpos >> 8);
			que[++qpos] = (BYTE)mpos;

			/* que[] ��񤭽Ф� */
			if (er = (*WriteBytesFn)(que, qpos + 1)) break;
			qpos = 0;

		} else if (cnt > 0) {	/* ����ʸ����ʤ� */
			que[++qpos] = wbuf[rpos];
			if (qpos >= UNMATCH_MAX) {	/* que[] ��񤭽Ф� */
				que[0] = qpos - 1;
				if (er = (*WriteBytesFn)(que, qpos+ 1)) break;
				qpos = 0;
			}
			mlen = 0;
		}

		/* rpos, tpos �ι��� */
		while (mlen-- >= 0) {
			i = tpos++;
			if (Hdmy > 0) Hdmy--;	/* ̤�����ʤΤǺ�����ʤ� */
			else {
				/* Hash ����κ�� : ��Ƭ����ȥ�ΤϤ� */
				k = MkHash(i);
				if (Hash_s[k] == tpos) {
					if ((Hash_s[k] = Hnxt[i]) == 0)
							Hash_e[k] = 0;
					Hnxt[i] = 0;
				}
			}
			tpos = BUFPOS(tpos);

			/* Hash  �ؤ��ɲá��Ǹ���ɲä���
				��λ�λ��κǸ�� 2 �Х��ȤΥϥå��������
				�Ȥʤ뤬���Ѥ���ʤ�����̵�뤹�� */
			k = MkHash(i = rpos++);
			Hnxt[i] = 0;
			if (i = Hash_e[k])	Hnxt[i - 1] = rpos;
			else			Hash_s[k] = rpos;
			Hash_e[k] = rpos;
			rpos = BUFPOS(rpos);
		}

		if (fin) {	/* ��λ���Ǹ�ޤǽ������� */
			if (rpos != wpos) continue;
			if (qpos) {	/* que[] ��񤭽Ф� */
				que[0] = qpos - 1;
				er = (*WriteBytesFn)(que, qpos + 1);
				qpos = 0;
			}
			break;
		}
	}
	return er;
}
/*
	���̽񤭹���
*/
LOCAL	INT	CompWrite(BYTE *buf, INT len, INT comp)
{
	INT	er, n;

	if (comp == 0) return (*WriteBytesFn)(buf, len);

	if (len <= 0) return CompOut(1); /* �Хåե���Υǡ����������ݤ��Ф� */

	/* �Хåե���Υǡ�����񤭹��� */
	er = 0;
	while (len > 0) {
		if ((n = tpos) == wpos) { /* �Хåե��Υǡ������ݤ��Ф� */
			if (er = CompOut(0)) break;
		} else {		  /* �Хåե��˥ǡ����򥳥ԡ����� */
			if (n < wpos) n = BUFSZ;
			if ((n -= wpos) > len) n = len;
			memcpy(&wbuf[wpos], buf, n);
			buf += n;
			len -= n;
			wpos = BUFPOS(wpos + n);
		}
	}
	return er;
}
/*======================================================================
	���������֥ե��������:

	�ԣ��ĥ쥳���ɤΣ��쥳���ɤǡ��ͥåȥ���ե������ɽ�����롣

	AR_HEAD(slen = 0xFFFF)	4 bytes
	AR_FILE			144 bytes

		AR_REC		8   bytes
		��쥳���ɥǡ�����AR_REC.len bytes��
		AR_REC		8   bytes
		��쥳���ɥǡ�����AR_REC.len bytes��
		������

	AR_HEAD(slen = 0xFFFF)	4 bytes
	AR_FILE			144 bytes
		::
		::

	AR_HEAD	(slen = 0)	4 bytes
======================================================================*/

/* �ե������������¤ (F_STATE) */
typedef struct {
	UWORD	f_type; 	/* �ե����륿���ס���ͭ�ԥ��������⡼�� */
	UWORD	f_atype;	/* ���ץꥱ������󡦥�����		*/
	TCODE	f_owner[14];	/* �ե������ͭ��̾(����ʸ��)�ܣ�	*/
	TCODE	f_group[14];	/* ��ͭ���롼��̾(����ʸ��)  �ܣ�	*/
	WORD	f_grpacc;	/* ���롼�ס�����������٥�		*/
	WORD	f_pubacc;	/* ���̥���������٥�			*/
	WORD	f_nlink;	/* �ޤ�Ǥ����󥯿�			*/
	WORD	f_index;	/* ����ǥå�����٥�			*/
	LONG	f_size; 	/* �ե��������Х��ȿ� 		*/
	LONG	f_nblk; 	/* ����������֥�å��� 		*/
	LONG	f_nrec;		/* ��쥳���ɿ� 			*/
	LONG	f_ltime;	/* �ե��������¸����(����)		*/
	LONG	f_atime;	/* �ǿ��Υ�����������			*/
	LONG	f_mtime;	/* �ǿ��ι�������			*/
	LONG	f_ctime;	/* �ե�����κ�������			*/
} F_STATE;

/* ���������֥ե�����إå���
	�ե������ֹ�ϡ����������֥ǡ�����Υե�����򼨤����Ū���ֹ�	*/
typedef	struct	{
	UWORD	tadid;			/* = AR_TADID			*/
	UWORD	slen;			/* = 0xFFFF or 0 (��λ)		*/
} AR_HEAD;

typedef	struct	{
	LONG	len;			/* �ʲ������Τ�Ĺ��		*/
	BYTE	attr;			/* ���̷���			*/
	BYTE	subid;			/* = 0xFF			*/
	UWORD	fnum;			/* �ե������ֹ�			*/
	TCODE	name[FNAME_MAX];	/* �ե�����̾			*/
	F_STATE	stat;			/* F_STATE ������		*/
} AR_FILE;

#define	AR_TADID	0xFFFD		/* ������ԣ��ģɣ�		*/
#define	AR_SLEN		0xFFFF		/* �顼����������		*/
#define	AR_SUBID	0xFF		/* ���֣ɣ�(����)		*/

#define	AR_NOCOMPAC	0x0		/* �󰵽�			*/
#define AR_COMPAC1	0x80		/* ����������			*/

/* ���������֥쥳���ɥ�إå� */
typedef	struct	{
	UWORD	type;			/* �쥳���ɥ�����		*/
	UWORD	styp;			/* ���֥�����			*/
	LONG	len;			/* �쥳���ɥǡ�����Ĺ�� 	*/
} AR_REC;

/* ���������֥�󥯥ǡ�������󥯥쥳���ɤλ��ϰʲ��Υǡ���������	*/
typedef	struct	{
	UWORD	fnum;			/* ���Ȥ���ե������ֹ�		*/
	UWORD	atr[5];			/* ���°��			*/
} AR_LINK;

LOCAL	INT	atype = 0;
LOCAL	INT	verbose = 0;
LOCAL	INT	compac = AR_NOCOMPAC;
LOCAL	INT	ux_longname = 0;
LOCAL	INT	fnum = 0;
LOCAL	INT	olen = 0;
LOCAL	INT	bcnt = 0;
LOCAL	INT	fcnt = 0;

LOCAL	BYTE	ofn[256] = "btf.tmp";
LOCAL	BYTE	bfn[256] = {0};
LOCAL	BYTE	ifn[256];
LOCAL	BYTE	dbuf[4096];

LOCAL	FILE	*ofp;
LOCAL	FILE	*ifp;

typedef	struct	_flist {
	struct	_flist	*next;
	UWORD		fnum;
	BYTE		name[2];
} FLIST;
LOCAL	FLIST	*fltop = NULL;
LOCAL	FLIST	*flend = NULL;

/*
	����ˡ
*/
VOID	usage()
{
	P(sE, "mkbtf [option] <obj> ...\n");
	P(sE, "      <obj> rtype.stype.fname -- Data Rec\n");
	P(sE, "            @[stype,]atr1.a2.a3.a4.a5.btfname -- Link Rec\n");
	P(sE, "                   (btfname MUST be No Compaction)\n");
	P(sE, "            N -- None Record\n");
	P(sE, "      -o*   Output File Path\n");
	P(sE, "      -a#   Application Type\n");
	P(sE, "      -t*   Btron File Name(%% --> Space)\n");
	P(sE, "      -c    Compaction\n");
	P(sE, "      -u    use Unixemu packed file name\n");
	P(sE, "      -v    Verbose\n");
	exit(0);
}
/*
	���顼��λ
*/
VOID	erexit(INT er, BYTE *s)
{
	switch(er) {
	case -1:	P(sE, "** Can't Create %s\n", s);	break;
	case -2:	P(sE, "** Can't Write %s\n", s);	break;
	case -3:	P(sE, "** Illegal Format %s\n", s);	break;
	case -4:	P(sE, "** Can't Read %s\n", s);		break;
	case -5:	P(sE, "** Illegal RecType %s\n", s);	break;
	case -6:	P(sE, "** Not Btron File %s\n", s);	break;
	case -7:	P(sE, "** Nest File Must be NoCompac %s\n", s); break;
	case -8:	P(sE, "** Illegal Record Data %s\n", s); break;
	case -9:	P(sE, "** Next File Too Many %s\n", s); break;
	}
	if (ifp) fclose(ifp);
	if (ofp) {fclose(ofp); unlink((char *)ofn);}
	exit(-1);
}
/*
	�ȡ������ɽ��
*/
VOID	prTotal(INT len, INT tlen)
{
	if (verbose) {
		P(sE, "..Total %d [%d] bytes", len, tlen);
		if (compac && tlen != 0) {
			P(sE, "(%d.%d%%)", (len * 100) / tlen,
				((len * 100) % tlen * 100) / tlen);
		}
		P(sE, "\n");
	}
}
/*
	�ե�����ؤν񤭹���
*/
INT	wrfile(BYTE *buf, INT len)
{
	if (len <= 0) fflush(ofp);
	else {
		if (fwrite(buf, 1, len, ofp) != len) erexit(-2, ofn);
		olen += len;
		bcnt += len;
	}
	return 0;
}
/*
	�ե���������ΰ��֤ؤν񤭹���
*/
INT	pos_wrfile(BYTE *buf, INT len, INT pos)
{
	INT	cpos;

	cpos = ftell(ofp);
	fseek(ofp, pos, 0);
	wrfile(buf, len);
	bcnt -= len;
	fseek(ofp, cpos, 0);
}
/*
	�ե����뤫����ɤ߹���
*/
INT	rdfile(BYTE *buf, INT len)
{
	if (fread(buf, 1, len, ifp) != len) erexit(-4, ifn);
	return 0;
}
/*
	�ͥ��ȥե�����ν���
*/
VOID	NestFile(UWORD fn)
{
	INT	tlen, n, len, clen;
	INT	arf_lenpos;
	INT	rc;
	AR_HEAD	arh;
	AR_FILE	arf;
	AR_REC	arr;
	AR_LINK	arl;
	INT	i, nf;
#define	MAX_NF		2048
	struct {UWORD old, new;} fnumtab[MAX_NF];

	if (verbose) P(sE, "FILE: [%d] %s\n", fn, ifn);

	for (nf = 0; ;fcnt++) {
		/* AR_HEAD ���ɤ߹��� �� �����å� */
		rdfile((BYTE*)&arh, sizeof(arh));
		if (arh.tadid != CnvW(AR_TADID)) erexit(-6, ifn);
		if (arh.slen == 0) break;
		if (arh.slen != CnvW(AR_SLEN)) erexit(-6, ifn);

		/* AR_FILE ���ɤ߹��� �� �����å� */
		rdfile((BYTE*)&arf, sizeof(arf));
		if (arf.subid != AR_SUBID) erexit(-6, ifn);
		if (arf.attr != AR_NOCOMPAC) erexit(-7, ifn);

		/* ���Τ�Ĺ�� */
		tlen = CnvL(arf.len);
		n = CnvW(arf.fnum);
		if (nf == 0) {		/* ��Ƭ */
			fnumtab[nf].old = n;
			fnumtab[nf++].new = fn;
			n = fn;
		} else {		/* �ޤޤ��ե����� */
			for (i = 0; i < nf; i++) {
				if (fnumtab[i].old == n) break;
			}
			if (i >= nf) {
				if (nf >= MAX_NF) erexit(-9, ifn);
				fnumtab[nf].old = n;
				fnumtab[nf++].new = ++fnum;
			}
			n = fnumtab[i].new;
			if (verbose) P(sE, "++FILE: [%d]\n", n);
		}
		arf.fnum = CnvW(n);
		arf.attr = compac;

		/* AR_HEAD, AR_FILE �ν񤭹��� */
		wrfile((BYTE*)&arh, sizeof(arh));
		arf_lenpos = ftell(ofp);
		wrfile((BYTE*)&arf, sizeof(arf));

		/* ���̽񤭹��߳��� */
		CompWriteInit(wrfile);

		/* �쥳���ɽ񤭹��� */
		olen = 0;
		tlen -= sizeof(AR_FILE) - sizeof(LONG);
		for (rc = 0, clen = tlen; clen > 0; rc++) {
			/* AR_REC���ɤ߹��ߡ��񤭹��� */
			rdfile((BYTE*)&arr, sizeof(arr));
			CompWrite((BYTE*)&arr, sizeof(arr), compac);
			len = CnvL(arr.len);
			clen -= sizeof(AR_REC) + len;
			if (clen < 0) erexit(-8, ifn);
			if (CnvW(arr.type) == 0) {	/* ��󥯥쥳���� */
				if (len != sizeof(AR_LINK)) erexit(-8, ifn);
				/* �ե����ֹ���ѹ� */
				rdfile((BYTE*)&arl, sizeof(arl));
				n = CnvW(arl.fnum);
				for (i = 0; i < nf; i++) {
					if (fnumtab[i].old == n) break;
				}
				if (i >= nf) {
					if (nf >= MAX_NF) erexit(-9, ifn);
					fnumtab[nf].old = n;
					fnumtab[nf++].new = ++fnum;
				}
				n = fnumtab[i].new;
				arl.fnum = CnvW(n);
				if (verbose) {
					P(sE, "..[%d] atr:", rc);
					for (n = 0; n < 5; n++)
						P(sE,"0x%x.",CnvW(arl.atr[n]));
					P(sE, "==> [%d]\n", CnvW(arl.fnum));
				}
				CompWrite((BYTE*)&arl, len, compac);
			} else {	/* �ǡ����쥳���� */
				if (verbose) {
				    P(sE, "..[%d] rt:%2d st:0x%04x len:%d\n",
					rc, CnvW(arr.type),CnvW(arr.styp),len);
				}
				/* �쥳���ɥǡ����ν񤭹��� */
				for (;len > 0;len -= n) {
					if ((n = sizeof(dbuf)) > len) n = len;
					rdfile(dbuf, n);
					CompWrite(dbuf, n, compac);
				}
			}
		}
		/* ���̽񤭹��߽�λ */
		CompWrite(NULL, 0, compac);

		/* �ȡ�����ɽ�� */
		prTotal(olen, tlen);

		/* AR_FILE.len �ν񤭹��� */
		len = CnvL(olen + sizeof(AR_FILE) - sizeof(LONG));
		pos_wrfile((BYTE*)&len, sizeof(LONG), arf_lenpos);
	}
}
/*
	�ᥤ��
*/
int	main(INT ac, BYTE **av)
{
	INT	an, tlen, n, len;
	INT	rest;
	INT	ftype = 0;
	INT	arf_lenpos;
	INT	rc;
	BYTE	*p;
	AR_HEAD	arh;
	AR_FILE	arf;
	AR_REC	arr;
	AR_LINK	arl;
	FLIST	*flp;

	/* ����ǥ�����Υƥ��� */
	TestEndian();

	/* ���ץ����μ��Ф� */
	for (an = 1; an < ac; an++) {
		p = av[an];
		if (*p++ != '-') break;
		switch(*p++) {
		case 'o':	strcpy((char *)ofn, (char *)p);	break;
		case 'a':	atype = strtol(p, NULL, 0);	break;
		case 'v':	verbose = 1;			break;
		case 'c':	compac = AR_COMPAC1;		break;
		case 'u':	ux_longname = 1;		break;
		case 't':	strcpy((char *)bfn, (char *)p);	break;
		case 'f':	ftype = strtol(p, NULL, 0);	break;
		default:	usage();
		}
	}
	if (an >= ac) usage();

	/* ���ϥե�����Υ����ץ� */
	if (bfn[0] == 0) strcpy((char *)bfn, (char *)ofn);
	if (!(ofp = fopen((char *)ofn, FOPEN_MODE_WRITE))) erexit(-1, ofn);

	bcnt = 0;
	fcnt = 1;

	/* AR_HEAD �ν񤭹��� */
	arh.tadid = CnvW(AR_TADID);
	arh.slen = CnvW(AR_SLEN);
	wrfile((BYTE*)&arh, sizeof(arh));

	/* AR_FILE �ν񤭹��� */
	bzero(&arf, sizeof(arf));
	arf.len = CnvL(0);
	arf.attr = compac;
	arf.subid = AR_SUBID;
	arf.fnum = CnvW(++fnum);
	NATIVE2TC(arf.name, bfn, FNAME_MAX, &rest);
	if (rest) {
                if (ux_longname) {
                        TCODE tfn[FNAME_MAX];
                        if (packAsciiFileName(tfn, bfn, FNAME_MAX, &rest) == -1) {
                                P(sE, "warning: file name too long(%d): `%s'.\n",
                                  FNAME_MAX, bfn);
                        } else {
                                if (rest) {
                                        P(sE, "warning: file name too long(%d): `%s'.\n",
                                          L_EXTEND_FNM, bfn);
                                }
                                memcpy(arf.name, tfn, FNAME_MAX * sizeof(TCODE));
                        }
                } else {
                        P(sE, "warning: file name too long(%d): `%s'.\n",
                          FNAME_MAX, bfn);
                }
        }
	arf.stat.f_type = CnvW(ftype);
	arf.stat.f_atype = CnvW(atype);
	arf_lenpos = ftell(ofp);
	wrfile((BYTE*)&arf, sizeof(arf));

	if (verbose) P(sE, "FILE: [%d] %s (atyp:0x%x) %s\n",
				fnum, bfn, atype, compac ? "COMPAC" : " ");

	/* ���̽񤭹��߳��� */
	CompWriteInit(wrfile);

	/* �쥳���ɤν񤭹��� */
	for (olen = tlen = rc = 0; an < ac; an++, rc++) {
		p = av[an];
		if (*p == '@') {	/* ��󥯥쥳���� */
			/* °���μ��Ф� */
			arr.styp = 0;
			for (n = 0; n < 5; n++) {
				UWORD v = strtol(++p, (char **)&p, 0);
				if (n == 0 && *p == ',') {
					arr.styp = v; /* �쥳���ɥ��֥����� */
					v = strtol(++p, (char **)&p, 0);
				}
				if (*p != '.') erexit(-3, av[an]);
				arl.atr[n] = CnvW(v);
			}
			/* �ե������¸�ߥ����å� */
			if (!(ifp = fopen((char *)++p, "r"))) erexit(-4, p);
			fclose(ifp);

			arl.fnum = CnvW(++fnum);
			if (verbose) {
				P(sE, "..[%d] st:0x%04x atr:", rc, arr.styp);
				for (n = 0; n < 5; n++)
					P(sE,"0x%x.", CnvW(arl.atr[n]));
				P(sE, "==> [%d] %s\n", fnum, p);
			}
			/* FLIST �ؤ���Ͽ */
			flp = (FLIST*)Malloc(strlen((char *)p) + sizeof(FLIST));
			strcpy((char *)flp->name, (char *)p);
			flp->fnum = fnum;
			if (flend) flend->next = flp;
			else	   fltop = flp;
			flend = flp;
			flp->next = NULL;

			/* AR_REC��AR_LINK�ν񤭹��� */
			tlen += sizeof(arr) + sizeof(arl);
			arr.type = CnvW(0);
			arr.styp = CnvW(arr.styp);
			arr.len = CnvL(sizeof(arl));
			CompWrite((BYTE*)&arr, sizeof(arr), compac);
			CompWrite((BYTE*)&arl, sizeof(arl), compac);

		} else if (*p != 'N') {		/* �ǡ����쥳���� */
			/* �쥳���ɥ��������μ��Ф� */
			arr.type = strtol(p, (char **)&p, 0);
			if (*p != '.') erexit(-3, av[an]);
			arr.styp = strtol(++p, (char **)&p, 0);
			if (*p != '.') erexit(-3, av[an]);
			if (arr.type == 0 || arr.type > 31) erexit(-5, av[an]);

			/* �ե�����Υ����ץ� */
			strcpy((char *)ifn, (char *)++p);
			if (!(ifp = fopen((char *)ifn, "r"))) erexit(-4, ifn);

			/* �ե����륵�����μ��Ф� */
			fseek(ifp, 0, 2);
			len = ftell(ifp);
			fseek(ifp, 0, 0);

			if (verbose)
				P(sE, "..[%d] rt:%2d st:0x%04x len:%d (%s)\n",
					rc, arr.type, arr.styp, len, ifn);

			/* AR_REC�ν񤭹��� */
			tlen += sizeof(arr) + len;
			arr.type = CnvW(arr.type);
			arr.styp = CnvW(arr.styp);
			arr.len = CnvL(len);
			CompWrite((BYTE*)&arr, sizeof(arr), compac);

			/* �쥳���ɥǡ����ν񤭹��� */
			for (; len > 0; len -= n) {
				if ((n = sizeof(dbuf)) > len) n = len;
				rdfile(dbuf, n);
				CompWrite(dbuf, n, compac);
			}
			fclose(ifp);
		}
	}
	/* ���̽񤭹��߽�λ */
	CompWrite(NULL, 0, compac);

	/* �ȡ�����ɽ�� */
	prTotal(olen, tlen);

	/* AR_FILE.len �ν񤭹��� */
	len = CnvL(olen + sizeof(AR_FILE) - sizeof(LONG));
	pos_wrfile((BYTE*)&len, sizeof(LONG), arf_lenpos);

	/* �ͥ��Ȥ����ե���������� */
	while (flp = fltop) {
		fltop = fltop->next;
		/* �ե�����Υ����ץ� */
		strcpy((char *)ifn, (char *)flp->name);
		if (!(ifp = fopen((char *)ifn, "r"))) erexit(-4, ifn);
		/* �ͥ��ȥե�����Υޡ��� */
		NestFile(flp->fnum);
		fclose(ifp);
		Free(flp);
	}

	/* AR_HEAD (�Ǹ�)�ν񤭹��� */
	arh.tadid = CnvW(AR_TADID);
	arh.slen = CnvW(0);
	wrfile((BYTE*)&arh, sizeof(arh));
	fclose(ofp);

	if (verbose) P(sE, "OUTFILE: %s (Total %d files, %d bytes)\n",
						ofn, fcnt, bcnt);

	return 0;
}
