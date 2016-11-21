/*----------------------------------------------------------------------
	comp.h		単純なスライド辞書方式による圧縮プログラム

	000N NNNN	続く (N + 1) バイトを出力 (31 >= N >= 0)

	LLLL PPPP PPPP PPPP
			P (1 〜 4095) バイト前の位置から (L + 1) バイトを出力
			(14 >= L >= 2)
	1111 LLLL LLLL PPPP PPPP PPPP
			P (1 〜 4095) バイト前の位置から (L + 1) バイトを出力
			(255 >= L >= 15)

	VOID	CompWriteInit(FUNCP wfn)		圧縮書き込み初期化

		W (*wfn)(VP buf, W len)
			return	== 0 : 正常
				 < 0 : エラー
			len == 0 の時は Flush する。

	W	CompWrite(VP buf, W len)		圧縮書き込み
			return	== 0 : 正常
				 < 0 : エラー
			len == 0 の時は Flush する。

	W	UnCompBuf(UB *dst, UB *src, W dlen, W slen)	圧縮展開処理
			return	>= 0 : 正常 (dlen の余りバイト数)
				 < 0 : エラー

----------------------------------------------------------------------*/

#include <string.h>
#include <strings.h>
#include "typedef.h"

#define	PBITS		12
#define	MATCH_MAX	256
#define	MATCH_POS	((1 << PBITS) - 2)
#define	UNMATCH_MAX	32

#define	BUFSZ		(1 << (PBITS + 1))
#define	BUFPOS(n)	((n) & (BUFSZ - 1))

LOCAL	FUNCP	WriteBytesFn;		/* バイト書き込み関数		*/

LOCAL	UB	wbuf[BUFSZ];		/* 出力リングバッファ		*/
LOCAL	W	tpos;			/* 出力バッファ内の先頭位置	*/
LOCAL	W	rpos;			/* 出力バッファ内の読み込み位置	*/
LOCAL	W	wpos;			/* 出力バッファ内の書き込み位置	*/

LOCAL	UB	que[UNMATCH_MAX + 4];	/* 出力キュー			*/
LOCAL	W	qpos;			/* 出力キュー位置		*/

#define	HASH_SZ	1024
LOCAL	H	Hash_s[HASH_SZ];	/* ハッシュテーブル（先頭）	*/
LOCAL	H	Hash_e[HASH_SZ];	/* ハッシュテーブル（最後）	*/
LOCAL	H	Hnxt[BUFSZ];		/* ハッシュリストテーブル	*/
LOCAL	W	Hdmy;			/* ダミーハッシュカウント	*/

/*
	ハッシュコードの生成 (連続した 3 バイトを使用)
*/
LOCAL	W	MkHash(W pos)
{
	return ((wbuf[pos] + (wbuf[BUFPOS(pos + 1)] << 1) +
			(wbuf[BUFPOS(pos + 2)] << 1)) & (HASH_SZ - 1));
}
/*
	書き込みの初期化
*/
EXPORT	void	CompWriteInit(FUNCP wfn)
{
	WriteBytesFn = wfn;

	/* 出力バッファの初期化 */
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
LOCAL	W	CompOut(W fin)
{
	W	i, n, cnt, er;
	W	mlen, mpos;

	for (er = 0; rpos != wpos; ) {

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

				for (n = 0; n < cnt; n++) {
					if (wbuf[BUFPOS(i + n)] !=
					    wbuf[BUFPOS(rpos + n)]) break;
				}
				if (n <= mlen) continue;
				mpos = rpos - i;
				if ((mlen = n) >= MATCH_MAX) break;
			}
		}
		if (--mlen >= 2) {	/* 3 文字以上の一致文字列あり */
			if (qpos > 0)	que[0] = qpos - 1;
			else		qpos--;

			if ((n = mpos) < 0) n += BUFSZ;
			if ((i = mlen) >= 15)
				que[++qpos] = 0xf0 | ((i >> 4) & 0xf);

			n |= (i & 0xf) << PBITS;
			que[++qpos] = (UB)(n >> 8);
			que[++qpos] = (UB)n;

			/* que[] を書き出す */
			if (er = (*WriteBytesFn)((VP)que, qpos + 1)) break;
			qpos = 0;

		} else {	/* 一致文字列なし */
			que[++qpos] = wbuf[rpos];
			if (qpos >= UNMATCH_MAX) {	/* que[] を書き出す */
				que[0] = qpos - 1;
				if (er = (*WriteBytesFn)((VP)que, qpos + 1))
							break;
				qpos = 0;
			}
			mlen = 0;
		}

		/* rpos, tpos & ハッシュテーブルの更新 */
		do {
			i = tpos++;
			if (Hdmy > 0) Hdmy--;	/* 未生成なので削除しない */
			else {
				/* Hash からの削除 : 先頭エントリのはず */
				n = MkHash(i);
				if (Hash_s[n] == tpos) {
					if ((Hash_s[n] = Hnxt[i]) == 0)
							Hash_e[n] = 0;
					Hnxt[i] = 0;
				}
			}
			tpos = BUFPOS(tpos);

			/* Hash への追加 : 最後に追加する
				終了の時の最後の 2 バイトのハッシュは不正
				となるが使用されないため無視する */
			n = MkHash(i = rpos++);
			Hnxt[i] = 0;
			if ((i = Hash_e[n]) != 0)	Hnxt[i - 1] = rpos;
			else				Hash_s[n] = rpos;
			Hash_e[n] = rpos;
			rpos = BUFPOS(rpos);
		} while (--mlen >= 0);

		if (fin != 0) {	/* 終了 : 最後まで処理する */
			if (rpos != wpos) continue;
			if (qpos > 0) {		/* que[] を書き出す */
				que[0] = qpos - 1;
				er = (*WriteBytesFn)((VP)que, qpos + 1);
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
EXPORT	W	CompWrite(VP buf, W len)
{
	W	er, n;
	UB	*tmp = (UB*)buf;

	if (len == 0) return CompOut(1); /* バッファ内のデータを全部掃き出す */

	/* バッファ内にデータを書き込む */
	while (len > 0) {
		if ((n = tpos) == wpos) {/* バッファのデータを掃き出す */
			if ((er = CompOut(0)) != 0) return er;
		} else {		/* バッファにデータをコピーする */
			if (n < wpos) n = BUFSZ;
			if ((n -= wpos) > len) n = len;
			memcpy(&wbuf[wpos], tmp, n);
			tmp += n;
			len -= n;
			wpos = BUFPOS(wpos + n);
		}
	}
	return 0;
}
/*
	圧縮展開処理
*/
EXPORT	W	UnCompBuf(UB *dst, UB* src, W dlen, W slen)
{
#define	R_BUFSZ		(1 << PBITS)
#define	R_POSMSK	((1 << PBITS) - 1)
#define	ER_REC		(-1)
#define	ER_OK		(0)

	W	n, pos, cnt, c;
	UB	buf[R_BUFSZ];

	memset(buf, 0, sizeof(buf));	/* 出力バッファのクリア */

	for (pos = 0; slen > 0; ) {
		n = *src++;
		if (n & 0xE0) {
			if (n < 0xF0) {
				cnt = 0;
				slen -= 2;
			} else {
				cnt = (n & 0x0F) << 4;
				n = *src++;
				slen -= 3;
			}
			if (slen < 0) return ER_REC;

			n = (n << 8) | *src++;

			cnt += (n >> PBITS) & 0x0F;
			if ((dlen -= cnt + 1) < 0) return ER_REC;

			n = (pos - (n & R_POSMSK) + R_BUFSZ) & R_POSMSK;

			while (cnt-- >= 0) {
				*dst++ = c = buf[n &= R_POSMSK];
				buf[pos &= R_POSMSK] = c;
				pos++;
				n++;
			}
		} else {
			if ((slen -= n + 2) < 0) return ER_REC;
			if ((dlen -= n + 1) < 0) return ER_REC;
			while (n-- >= 0) {
				*dst++ = c = *src++;
				buf[pos &= R_POSMSK] = c;
				pos++;
			}
		}
	}
	return (slen == 0 && dlen >= 0) ? dlen : ER_REC;
}
