/*----------------------------------------------------------------------
	comp.h		ñ��ʥ��饤�ɼ��������ˤ�밵�̥ץ����

	000N NNNN	³�� (N + 1) �Х��Ȥ���� (31 >= N >= 0)

	LLLL PPPP PPPP PPPP
			P (1 �� 4095) �Х������ΰ��֤��� (L + 1) �Х��Ȥ����
			(14 >= L >= 2)
	1111 LLLL LLLL PPPP PPPP PPPP
			P (1 �� 4095) �Х������ΰ��֤��� (L + 1) �Х��Ȥ����
			(255 >= L >= 15)

	VOID	CompWriteInit(FUNCP wfn)		���̽񤭹��߽����

		W (*wfn)(VP buf, W len)
			return	== 0 : ����
				 < 0 : ���顼
			len == 0 �λ��� Flush ���롣

	W	CompWrite(VP buf, W len)		���̽񤭹���
			return	== 0 : ����
				 < 0 : ���顼
			len == 0 �λ��� Flush ���롣

	W	UnCompBuf(UB *dst, UB *src, W dlen, W slen)	����Ÿ������
			return	>= 0 : ���� (dlen ��;��Х��ȿ�)
				 < 0 : ���顼

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

LOCAL	FUNCP	WriteBytesFn;		/* �Х��Ƚ񤭹��ߴؿ�		*/

LOCAL	UB	wbuf[BUFSZ];		/* ���ϥ�󥰥Хåե�		*/
LOCAL	W	tpos;			/* ���ϥХåե������Ƭ����	*/
LOCAL	W	rpos;			/* ���ϥХåե�����ɤ߹��߰���	*/
LOCAL	W	wpos;			/* ���ϥХåե���ν񤭹��߰���	*/

LOCAL	UB	que[UNMATCH_MAX + 4];	/* ���ϥ��塼			*/
LOCAL	W	qpos;			/* ���ϥ��塼����		*/

#define	HASH_SZ	1024
LOCAL	H	Hash_s[HASH_SZ];	/* �ϥå���ơ��֥����Ƭ��	*/
LOCAL	H	Hash_e[HASH_SZ];	/* �ϥå���ơ��֥�ʺǸ��	*/
LOCAL	H	Hnxt[BUFSZ];		/* �ϥå���ꥹ�ȥơ��֥�	*/
LOCAL	W	Hdmy;			/* ���ߡ��ϥå��奫�����	*/

/*
	�ϥå��女���ɤ����� (Ϣ³���� 3 �Х��Ȥ����)
*/
LOCAL	W	MkHash(W pos)
{
	return ((wbuf[pos] + (wbuf[BUFPOS(pos + 1)] << 1) +
			(wbuf[BUFPOS(pos + 2)] << 1)) & (HASH_SZ - 1));
}
/*
	�񤭹��ߤν����
*/
EXPORT	void	CompWriteInit(FUNCP wfn)
{
	WriteBytesFn = wfn;

	/* ���ϥХåե��ν���� */
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
LOCAL	W	CompOut(W fin)
{
	W	i, n, cnt, er;
	W	mlen, mpos;

	for (er = 0; rpos != wpos; ) {

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

				for (n = 0; n < cnt; n++) {
					if (wbuf[BUFPOS(i + n)] !=
					    wbuf[BUFPOS(rpos + n)]) break;
				}
				if (n <= mlen) continue;
				mpos = rpos - i;
				if ((mlen = n) >= MATCH_MAX) break;
			}
		}
		if (--mlen >= 2) {	/* 3 ʸ���ʾ�ΰ���ʸ���󤢤� */
			if (qpos > 0)	que[0] = qpos - 1;
			else		qpos--;

			if ((n = mpos) < 0) n += BUFSZ;
			if ((i = mlen) >= 15)
				que[++qpos] = 0xf0 | ((i >> 4) & 0xf);

			n |= (i & 0xf) << PBITS;
			que[++qpos] = (UB)(n >> 8);
			que[++qpos] = (UB)n;

			/* que[] ��񤭽Ф� */
			if (er = (*WriteBytesFn)((VP)que, qpos + 1)) break;
			qpos = 0;

		} else {	/* ����ʸ����ʤ� */
			que[++qpos] = wbuf[rpos];
			if (qpos >= UNMATCH_MAX) {	/* que[] ��񤭽Ф� */
				que[0] = qpos - 1;
				if (er = (*WriteBytesFn)((VP)que, qpos + 1))
							break;
				qpos = 0;
			}
			mlen = 0;
		}

		/* rpos, tpos & �ϥå���ơ��֥�ι��� */
		do {
			i = tpos++;
			if (Hdmy > 0) Hdmy--;	/* ̤�����ʤΤǺ�����ʤ� */
			else {
				/* Hash ����κ�� : ��Ƭ����ȥ�ΤϤ� */
				n = MkHash(i);
				if (Hash_s[n] == tpos) {
					if ((Hash_s[n] = Hnxt[i]) == 0)
							Hash_e[n] = 0;
					Hnxt[i] = 0;
				}
			}
			tpos = BUFPOS(tpos);

			/* Hash �ؤ��ɲ� : �Ǹ���ɲä���
				��λ�λ��κǸ�� 2 �Х��ȤΥϥå��������
				�Ȥʤ뤬���Ѥ���ʤ�����̵�뤹�� */
			n = MkHash(i = rpos++);
			Hnxt[i] = 0;
			if ((i = Hash_e[n]) != 0)	Hnxt[i - 1] = rpos;
			else				Hash_s[n] = rpos;
			Hash_e[n] = rpos;
			rpos = BUFPOS(rpos);
		} while (--mlen >= 0);

		if (fin != 0) {	/* ��λ : �Ǹ�ޤǽ������� */
			if (rpos != wpos) continue;
			if (qpos > 0) {		/* que[] ��񤭽Ф� */
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
	���̽񤭹���
*/
EXPORT	W	CompWrite(VP buf, W len)
{
	W	er, n;
	UB	*tmp = (UB*)buf;

	if (len == 0) return CompOut(1); /* �Хåե���Υǡ����������ݤ��Ф� */

	/* �Хåե���˥ǡ�����񤭹��� */
	while (len > 0) {
		if ((n = tpos) == wpos) {/* �Хåե��Υǡ������ݤ��Ф� */
			if ((er = CompOut(0)) != 0) return er;
		} else {		/* �Хåե��˥ǡ����򥳥ԡ����� */
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
	����Ÿ������
*/
EXPORT	W	UnCompBuf(UB *dst, UB* src, W dlen, W slen)
{
#define	R_BUFSZ		(1 << PBITS)
#define	R_POSMSK	((1 << PBITS) - 1)
#define	ER_REC		(-1)
#define	ER_OK		(0)

	W	n, pos, cnt, c;
	UB	buf[R_BUFSZ];

	memset(buf, 0, sizeof(buf));	/* ���ϥХåե��Υ��ꥢ */

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
