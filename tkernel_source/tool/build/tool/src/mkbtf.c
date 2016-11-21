/*
	mkbtf.c		BTRON ファイル作成プログラム

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
	エンディアン変換
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
	0x2121, /* 20 SP */	0x212a, /* 21 ！ */	0x2149, /* 22 ” */
	0x2174, /* 23 ＃ */	0x2170, /* 24 ＄ */	/*0x2173,*/
							0x2121,	 /* 25 ％ */
	0x2175, /* 26 ＆ */	0x2147, /* 27 ’ */	0x214a, /* 28 （ */
	0x214b, /* 29 ） */	0x2176, /* 2A ＊ */	0x215c, /* 2B ＋ */
	0x2124, /* 2C ， */	0x215d, /* 2D − */	0x2125, /* 2E ． */
	0x213f, /* 2F ／ */
	0x2330, /* 30 ０ */	0x2331, /* 31 １ */	0x2332, /* 32 ２ */
	0x2333, /* 33 ３ */	0x2334, /* 34 ４ */	0x2335, /* 35 ５ */
	0x2336, /* 36 ６ */	0x2337, /* 37 ７ */	0x2338, /* 38 ８ */
	0x2339, /* 39 ９ */	0x2127, /* 3A ： */	0x2128, /* 3B ； */
	0x2163, /* 3C ＜ */	0x2161, /* 3D ＝ */	0x2164, /* 3E ＞ */
	0x2129, /* 3F ？ */
	0x2177, /* 40 ＠ */	0x2341, /* 41 Ａ */	0x2342, /* 42 Ｂ */
	0x2343, /* 43 Ｃ */	0x2344, /* 44 Ｄ */	0x2345, /* 45 Ｅ */
	0x2346, /* 46 Ｆ */	0x2347, /* 47 Ｇ */	0x2348, /* 48 Ｈ */
	0x2349, /* 49 Ｉ */	0x234a, /* 4A Ｊ */	0x234b, /* 4B Ｋ */
	0x234c, /* 4C Ｌ */	0x234d, /* 4D Ｍ */	0x234e, /* 4E Ｎ */
	0x234f, /* 4F Ｏ */
	0x2350, /* 50 Ｐ */	0x2351, /* 51 Ｑ */	0x2352, /* 52 Ｒ */
	0x2353, /* 53 Ｓ */	0x2354, /* 54 Ｔ */	0x2355, /* 55 Ｕ */
	0x2356, /* 56 Ｖ */	0x2357, /* 57 Ｗ */	0x2358, /* 58 Ｘ */
	0x2359, /* 59 Ｙ */	0x235a, /* 5A Ｚ */	0x214e, /* 5B ［ */
	0x216f, /* 5C ￥ */	0x214f, /* 5D ］ */	0x2130, /* 5E ＾ */
	0x2132, /* 5F ＿ */
	0x2146, /* 60 ‘ */	0x2361, /* 61 ａ */	0x2362, /* 62 ｂ */
	0x2363, /* 63 ｃ */	0x2364, /* 64 ｄ */	0x2365, /* 65 ｅ */
	0x2366, /* 66 ｆ */	0x2367, /* 67 ｇ */	0x2368, /* 68 ｈ */
	0x2369, /* 69 ｉ */	0x236a, /* 6A ｊ */	0x236b, /* 6B ｋ */
	0x236c, /* 6C ｌ */	0x236d, /* 6D ｍ */	0x236e, /* 6E ｎ */
	0x236f, /* 6F ｏ*/
	0x2370, /* 70 ｐ */	0x2371, /* 71 ｑ */	0x2372, /* 72 ｒ */
	0x2373, /* 73 ｓ */	0x2374, /* 74 ｔ */	0x2375, /* 75 ｕ */
	0x2376, /* 76 ｖ */	0x2377, /* 77 ｗ */	0x2378, /* 78 ｘ */
	0x2379, /* 79 ｙ */	0x237a, /* 7A ｚ */	0x2150, /* 7B ｛ */
	0x2143, /* 7C ｜ */	0x2151, /* 7D ｝ */	0x2141, /* 7E 〜 */
	0x007F	/* 7F	 */
};
#ifdef	WIN
/*======================================================================
	コード変換: SJIS -> TCODE
======================================================================*/
INT	sjtotc(TCODE *tc, BYTE *sjis)
{
	UWORD	sc1, sc2;
	TCODE	c;

	if ((sc1 = *sjis++) < 0x80) {
		if (sc1 == '\0') return 0;
		if (sc1 != NULL) {
			c = _asctotc[sc1 & 0x7f];	/* 半角英数文字 */
			*tc = CnvW(c);
		}
		return (1);
	}
	if (sc1 >= 0xa0 && sc1 < 0xe0)
		return (-1);			/* 半角かな文字 */

	sc1 -= (sc1 < 0xa0) ? 0x71 : 0xb1;	/* 全角文字 */
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
	コード変換: EUC -> TCODE
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

#define TK_EXTEND_FNM_MARK 0xA121       /* 識別マーク */
#define L_EXTEND_FNM    34              /* ファイル名の最大長(文字数) */

/**
 * ASCIIの範囲で構成される文字列をTCにパックして変換する。
 * @param tcname 出力先バッファ。maxlenに満たない場合のみ0で埋められる。
 * @param str 入力文字列。ASCIIの範囲でなければならない。
 * @param maxlen tcname の長さ。
 * @param rest NULLでない場合は値を戻す。str全体を変換できた場合 0。
 * @return 変換結果の文字数。変換に失敗した場合は-1。
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

        /* 2 文字ずつ D ゾーンにパック */
        for ( i = 3, j = 0;
              i < maxlen && (a1 = str[j]);
              i++, j += 2 ) {
                if ((a2 = str[j + 1])) {
                        /* 範囲チェック */
                        if ((a1 < 0x20 || 0x7e < a1) ||
                            (a2 < 0x20 || 0x7e < a2)) {
                                return -1;
                        }
                        tcname[i] = ((a1 + 0x60) << 8) | (0x00ff & (a2 + 0x60));
                } else {
                        /* 最後の 1 文字が余ったら A ゾーン */
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
	圧縮書き込み

	単純なスライド辞書方式による圧縮プログラム
	０００Ｎ ＮＮＮＮ	続く(Ｎ＋１)バイトを出力（３１≧Ｎ≧０）
	ＬＬＬＬ ＰＰＰＰ ＰＰＰＰ ＰＰＰＰ
				Ｐバイト前の位置から(Ｌ＋１)バイトを出力
				（１４≧Ｌ≧２）
	１１１１ ＬＬＬＬ ＬＬＬＬ ＰＰＰＰ ＰＰＰＰ ＰＰＰＰ
				Ｐバイト前の位置から(Ｌ＋１)バイトを出力
				（２５５≧Ｌ≧１５）
======================================================================*/
#define	PBITS		12
#define	MATCH_MAX	256
#define	MATCH_POS	((1 << PBITS) - 2)
#define	UNMATCH_MAX	32
#define	BUFSZ		(1 << (PBITS + 1))
#define	BUFPOS(n)	((n) & (BUFSZ - 1))

LOCAL	FNP	WriteBytesFn;		/* バイト書き込み関数		*/
LOCAL	BYTE	wbuf[BUFSZ];		/* 出力リングバッファ		*/
LOCAL	INT	tpos;			/* 出力バッファ内の先頭位置	*/
LOCAL	INT	rpos;			/* 出力バッファ内の読み込み位置	*/
LOCAL	INT	wpos;			/* 出力バッファ内の書き込み位置	*/
LOCAL	BYTE	que[UNMATCH_MAX + 4];	/* 出力キュー			*/
LOCAL	INT	qpos;			/* 出力キュー位置		*/
#define	HASH_SZ	1024
LOCAL	WORD	Hash_s[HASH_SZ];	/* ハッシュテーブル（先頭）	*/
LOCAL	WORD	Hash_e[HASH_SZ];	/* ハッシュテーブル（最後）	*/
LOCAL	WORD	Hnxt[BUFSZ];		/* ハッシュリストテーブル	*/
LOCAL	INT	Hdmy;			/* ダミーハッシュカウント	*/

/*
	ハッシュコードの生成 (連続した 3 バイトを使用)
*/
LOCAL	INT	MkHash(INT pos)
{
	return ((wbuf[pos] + (wbuf[BUFPOS(pos + 1)] << 1) +
			(wbuf[BUFPOS(pos + 2)] << 1)) & (HASH_SZ - 1));
}
/*
	書き込みの初期化
*/
LOCAL	VOID	CompWriteInit(FNP wfn)
{
	WriteBytesFn = wfn;
	bzero(wbuf, MATCH_POS);
	tpos = 0;
	rpos = wpos = MATCH_POS;
	qpos = 0;
	Hdmy = MATCH_POS;

	/* ハッシュテーブルのクリア */
	bzero(Hash_s, sizeof(Hash_s));
	bzero(Hash_e, sizeof(Hash_e));
	bzero(Hnxt, sizeof(Hnxt));
}
/*
	圧縮出力

	wbuf:	tpos	--> 出力済みデータ  (tpos = rpos - MATCH_POS)
		rpos	--> 出力開始データ
		wpos	--> 出力終了データ
*/
LOCAL	INT	CompOut(INT fin)
{
	INT	i, k, cnt, er;
	INT	mlen;
	INT	mpos;

	for (er = 0;;) {
		/* rpos 〜 + MATCH_MAX (wpos) と一致する最大の文字列を
				tpos 〜 rpos (〜 wpos) の範囲で見つける */
		/* 残りバイト数 */
		cnt = BUFPOS(wpos + BUFSZ - rpos);

		/* 終了でないときは、ハッシュの正当性を保証するために、
			残りバイト数 を - 3 とする。さらに、最大の一致を見つけ
			るために、残りバイト数 > MATCH_MAX の時のみ処理する */
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
		if (--mlen >= 2) {	/* 一致文字列あり */
			if (qpos) que[0] = qpos - 1;
			else	  qpos--;

			if (mpos < 0)	mpos += BUFSZ;
			if ((i = mlen) >= 15)
				que[++qpos] = 0xf0 | ((i >> 4) & 0xf);

			mpos |= (i & 0xf) << PBITS;
			que[++qpos] = (BYTE)(mpos >> 8);
			que[++qpos] = (BYTE)mpos;

			/* que[] を書き出す */
			if (er = (*WriteBytesFn)(que, qpos + 1)) break;
			qpos = 0;

		} else if (cnt > 0) {	/* 一致文字列なし */
			que[++qpos] = wbuf[rpos];
			if (qpos >= UNMATCH_MAX) {	/* que[] を書き出す */
				que[0] = qpos - 1;
				if (er = (*WriteBytesFn)(que, qpos+ 1)) break;
				qpos = 0;
			}
			mlen = 0;
		}

		/* rpos, tpos の更新 */
		while (mlen-- >= 0) {
			i = tpos++;
			if (Hdmy > 0) Hdmy--;	/* 未生成なので削除しない */
			else {
				/* Hash からの削除 : 先頭エントリのはず */
				k = MkHash(i);
				if (Hash_s[k] == tpos) {
					if ((Hash_s[k] = Hnxt[i]) == 0)
							Hash_e[k] = 0;
					Hnxt[i] = 0;
				}
			}
			tpos = BUFPOS(tpos);

			/* Hash  への追加：最後に追加する
				終了の時の最後の 2 バイトのハッシュは不正
				となるが使用されないため無視する */
			k = MkHash(i = rpos++);
			Hnxt[i] = 0;
			if (i = Hash_e[k])	Hnxt[i - 1] = rpos;
			else			Hash_s[k] = rpos;
			Hash_e[k] = rpos;
			rpos = BUFPOS(rpos);
		}

		if (fin) {	/* 終了：最後まで処理する */
			if (rpos != wpos) continue;
			if (qpos) {	/* que[] を書き出す */
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
	圧縮書き込み
*/
LOCAL	INT	CompWrite(BYTE *buf, INT len, INT comp)
{
	INT	er, n;

	if (comp == 0) return (*WriteBytesFn)(buf, len);

	if (len <= 0) return CompOut(1); /* バッファ内のデータを全部掃き出す */

	/* バッファ内のデータを書き込む */
	er = 0;
	while (len > 0) {
		if ((n = tpos) == wpos) { /* バッファのデータを掃き出す */
			if (er = CompOut(0)) break;
		} else {		  /* バッファにデータをコピーする */
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
	アーカイブファイル形式:

	ＴＡＤレコードの１レコードで、ネットワークファイルを表現する。

	AR_HEAD(slen = 0xFFFF)	4 bytes
	AR_FILE			144 bytes

		AR_REC		8   bytes
		＜レコードデータ：AR_REC.len bytes＞
		AR_REC		8   bytes
		＜レコードデータ：AR_REC.len bytes＞
		：：：

	AR_HEAD(slen = 0xFFFF)	4 bytes
	AR_FILE			144 bytes
		::
		::

	AR_HEAD	(slen = 0)	4 bytes
======================================================================*/

/* ファイル管理情報構造 (F_STATE) */
typedef struct {
	UWORD	f_type; 	/* ファイルタイプ／所有者アクセスモード */
	UWORD	f_atype;	/* アプリケーション・タイプ		*/
	TCODE	f_owner[14];	/* ファイル所有者名(１２文字)＋０	*/
	TCODE	f_group[14];	/* 所有グループ名(１２文字)  ＋０	*/
	WORD	f_grpacc;	/* グループ・アクセスレベル		*/
	WORD	f_pubacc;	/* 一般アクセスレベル			*/
	WORD	f_nlink;	/* 含んでいるリンク数			*/
	WORD	f_index;	/* インデックスレベル			*/
	LONG	f_size; 	/* ファイルの総バイト数 		*/
	LONG	f_nblk; 	/* 総使用論理ブロック数 		*/
	LONG	f_nrec;		/* 総レコード数 			*/
	LONG	f_ltime;	/* ファイルの保存期限(日時)		*/
	LONG	f_atime;	/* 最新のアクセス日時			*/
	LONG	f_mtime;	/* 最新の更新日時			*/
	LONG	f_ctime;	/* ファイルの作成日時			*/
} F_STATE;

/* アーカイブファイルヘッダ：
	ファイル番号は、アーカイブデータ内のファイルを示す一意的な番号	*/
typedef	struct	{
	UWORD	tadid;			/* = AR_TADID			*/
	UWORD	slen;			/* = 0xFFFF or 0 (終了)		*/
} AR_HEAD;

typedef	struct	{
	LONG	len;			/* 以下の全体の長さ		*/
	BYTE	attr;			/* 圧縮形式			*/
	BYTE	subid;			/* = 0xFF			*/
	UWORD	fnum;			/* ファイル番号			*/
	TCODE	name[FNAME_MAX];	/* ファイル名			*/
	F_STATE	stat;			/* F_STATE の内容		*/
} AR_FILE;

#define	AR_TADID	0xFFFD		/* 非公式ＴＡＤＩＤ		*/
#define	AR_SLEN		0xFFFF		/* ラージセグメント		*/
#define	AR_SUBID	0xFF		/* サブＩＤ(固定)		*/

#define	AR_NOCOMPAC	0x0		/* 非圧縮			*/
#define AR_COMPAC1	0x80		/* 圧縮方式１			*/

/* アーカイブレコードルヘッダ */
typedef	struct	{
	UWORD	type;			/* レコードタイプ		*/
	UWORD	styp;			/* サブタイプ			*/
	LONG	len;			/* レコードデータの長さ 	*/
} AR_REC;

/* アーカイブリンクデータ：リンクレコードの時は以下のデータが入る	*/
typedef	struct	{
	UWORD	fnum;			/* 参照するファイル番号		*/
	UWORD	atr[5];			/* リンク属性			*/
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
	使用法
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
	エラー終了
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
	トータルの表示
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
	ファイルへの書き込み
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
	ファイルの前の位置への書き込み
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
	ファイルからの読み込み
*/
INT	rdfile(BYTE *buf, INT len)
{
	if (fread(buf, 1, len, ifp) != len) erexit(-4, ifn);
	return 0;
}
/*
	ネストファイルの出力
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
		/* AR_HEAD の読み込み ＆ チェック */
		rdfile((BYTE*)&arh, sizeof(arh));
		if (arh.tadid != CnvW(AR_TADID)) erexit(-6, ifn);
		if (arh.slen == 0) break;
		if (arh.slen != CnvW(AR_SLEN)) erexit(-6, ifn);

		/* AR_FILE の読み込み ＆ チェック */
		rdfile((BYTE*)&arf, sizeof(arf));
		if (arf.subid != AR_SUBID) erexit(-6, ifn);
		if (arf.attr != AR_NOCOMPAC) erexit(-7, ifn);

		/* 全体の長さ */
		tlen = CnvL(arf.len);
		n = CnvW(arf.fnum);
		if (nf == 0) {		/* 先頭 */
			fnumtab[nf].old = n;
			fnumtab[nf++].new = fn;
			n = fn;
		} else {		/* 含まれるファイル */
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

		/* AR_HEAD, AR_FILE の書き込み */
		wrfile((BYTE*)&arh, sizeof(arh));
		arf_lenpos = ftell(ofp);
		wrfile((BYTE*)&arf, sizeof(arf));

		/* 圧縮書き込み開始 */
		CompWriteInit(wrfile);

		/* レコード書き込み */
		olen = 0;
		tlen -= sizeof(AR_FILE) - sizeof(LONG);
		for (rc = 0, clen = tlen; clen > 0; rc++) {
			/* AR_RECの読み込み／書き込み */
			rdfile((BYTE*)&arr, sizeof(arr));
			CompWrite((BYTE*)&arr, sizeof(arr), compac);
			len = CnvL(arr.len);
			clen -= sizeof(AR_REC) + len;
			if (clen < 0) erexit(-8, ifn);
			if (CnvW(arr.type) == 0) {	/* リンクレコード */
				if (len != sizeof(AR_LINK)) erexit(-8, ifn);
				/* ファイ番号の変更 */
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
			} else {	/* データレコード */
				if (verbose) {
				    P(sE, "..[%d] rt:%2d st:0x%04x len:%d\n",
					rc, CnvW(arr.type),CnvW(arr.styp),len);
				}
				/* レコードデータの書き込み */
				for (;len > 0;len -= n) {
					if ((n = sizeof(dbuf)) > len) n = len;
					rdfile(dbuf, n);
					CompWrite(dbuf, n, compac);
				}
			}
		}
		/* 圧縮書き込み終了 */
		CompWrite(NULL, 0, compac);

		/* トータル表示 */
		prTotal(olen, tlen);

		/* AR_FILE.len の書き込み */
		len = CnvL(olen + sizeof(AR_FILE) - sizeof(LONG));
		pos_wrfile((BYTE*)&len, sizeof(LONG), arf_lenpos);
	}
}
/*
	メイン
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

	/* エンディアンのテスト */
	TestEndian();

	/* オプションの取り出し */
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

	/* 出力ファイルのオープン */
	if (bfn[0] == 0) strcpy((char *)bfn, (char *)ofn);
	if (!(ofp = fopen((char *)ofn, FOPEN_MODE_WRITE))) erexit(-1, ofn);

	bcnt = 0;
	fcnt = 1;

	/* AR_HEAD の書き込み */
	arh.tadid = CnvW(AR_TADID);
	arh.slen = CnvW(AR_SLEN);
	wrfile((BYTE*)&arh, sizeof(arh));

	/* AR_FILE の書き込み */
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

	/* 圧縮書き込み開始 */
	CompWriteInit(wrfile);

	/* レコードの書き込み */
	for (olen = tlen = rc = 0; an < ac; an++, rc++) {
		p = av[an];
		if (*p == '@') {	/* リンクレコード */
			/* 属性の取り出し */
			arr.styp = 0;
			for (n = 0; n < 5; n++) {
				UWORD v = strtol(++p, (char **)&p, 0);
				if (n == 0 && *p == ',') {
					arr.styp = v; /* レコードサブタイプ */
					v = strtol(++p, (char **)&p, 0);
				}
				if (*p != '.') erexit(-3, av[an]);
				arl.atr[n] = CnvW(v);
			}
			/* ファイルの存在チェック */
			if (!(ifp = fopen((char *)++p, "r"))) erexit(-4, p);
			fclose(ifp);

			arl.fnum = CnvW(++fnum);
			if (verbose) {
				P(sE, "..[%d] st:0x%04x atr:", rc, arr.styp);
				for (n = 0; n < 5; n++)
					P(sE,"0x%x.", CnvW(arl.atr[n]));
				P(sE, "==> [%d] %s\n", fnum, p);
			}
			/* FLIST への登録 */
			flp = (FLIST*)Malloc(strlen((char *)p) + sizeof(FLIST));
			strcpy((char *)flp->name, (char *)p);
			flp->fnum = fnum;
			if (flend) flend->next = flp;
			else	   fltop = flp;
			flend = flp;
			flp->next = NULL;

			/* AR_REC、AR_LINKの書き込み */
			tlen += sizeof(arr) + sizeof(arl);
			arr.type = CnvW(0);
			arr.styp = CnvW(arr.styp);
			arr.len = CnvL(sizeof(arl));
			CompWrite((BYTE*)&arr, sizeof(arr), compac);
			CompWrite((BYTE*)&arl, sizeof(arl), compac);

		} else if (*p != 'N') {		/* データレコード */
			/* レコードタイプ等の取り出し */
			arr.type = strtol(p, (char **)&p, 0);
			if (*p != '.') erexit(-3, av[an]);
			arr.styp = strtol(++p, (char **)&p, 0);
			if (*p != '.') erexit(-3, av[an]);
			if (arr.type == 0 || arr.type > 31) erexit(-5, av[an]);

			/* ファイルのオープン */
			strcpy((char *)ifn, (char *)++p);
			if (!(ifp = fopen((char *)ifn, "r"))) erexit(-4, ifn);

			/* ファイルサイズの取り出し */
			fseek(ifp, 0, 2);
			len = ftell(ifp);
			fseek(ifp, 0, 0);

			if (verbose)
				P(sE, "..[%d] rt:%2d st:0x%04x len:%d (%s)\n",
					rc, arr.type, arr.styp, len, ifn);

			/* AR_RECの書き込み */
			tlen += sizeof(arr) + len;
			arr.type = CnvW(arr.type);
			arr.styp = CnvW(arr.styp);
			arr.len = CnvL(len);
			CompWrite((BYTE*)&arr, sizeof(arr), compac);

			/* レコードデータの書き込み */
			for (; len > 0; len -= n) {
				if ((n = sizeof(dbuf)) > len) n = len;
				rdfile(dbuf, n);
				CompWrite(dbuf, n, compac);
			}
			fclose(ifp);
		}
	}
	/* 圧縮書き込み終了 */
	CompWrite(NULL, 0, compac);

	/* トータル表示 */
	prTotal(olen, tlen);

	/* AR_FILE.len の書き込み */
	len = CnvL(olen + sizeof(AR_FILE) - sizeof(LONG));
	pos_wrfile((BYTE*)&len, sizeof(LONG), arf_lenpos);

	/* ネストしたファイルの生成 */
	while (flp = fltop) {
		fltop = fltop->next;
		/* ファイルのオープン */
		strcpy((char *)ifn, (char *)flp->name);
		if (!(ifp = fopen((char *)ifn, "r"))) erexit(-4, ifn);
		/* ネストファイルのマージ */
		NestFile(flp->fnum);
		fclose(ifp);
		Free(flp);
	}

	/* AR_HEAD (最後)の書き込み */
	arh.tadid = CnvW(AR_TADID);
	arh.slen = CnvW(0);
	wrfile((BYTE*)&arh, sizeof(arh));
	fclose(ofp);

	if (verbose) P(sE, "OUTFILE: %s (Total %d files, %d bytes)\n",
						ofn, fcnt, bcnt);

	return 0;
}
