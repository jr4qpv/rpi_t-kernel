/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)disp_mem.c (libmisc) 2016/11/16
 *  DISP_MEM関係ﾗｲﾌﾞﾗﾘｰ
 */
#include <basic.h>
#ifdef __GNUC__
 #include <t2ex/ctype.h>				/* for isprint() */
#else
 #include <ctype.h>						/* for isprint() */
#endif
#include <misc/libmisc.h>


/*================================================
;|
;|□mem_dumpx           《出力先指定付きﾒﾓﾘﾀﾞﾝﾌﾟｻﾌﾞﾙｰﾁﾝ》
;|  ［形式］#include <misc/libmisc.h>
;|          uint32_t mem_dumpx(uint32_t addr, long count, int size, char disp);
;|  ［引数］addr…表示開始ｱﾄﾞﾚｽ
;|          count…表示ﾊﾞｲﾄｶｳﾝﾄ
;|          size…ﾒﾓﾘｻｲｽﾞ(0:byte, 1:word, 2:long)
;|          disp…表示指定(0:しない,1:標準出力,2:ｴﾗｰ出力)
;|  ［戻値］最後に対象となったaddr
;|  ［Note］・SPACEキーの入力により表示を中断する。
;|
 ================================================*/
uint32_t mem_dumpx(uint32_t addr, long count, int size, char disp)
{
	char buff[17], c;
	int i, j;
	union d_tag {
		char cdata;
		short sdata;
		long ldata;
		char strdata[4];
	} d;

	addr &= 0xfffffff0L;					/* 16の倍数に調整 */
	count += 15;
	count >>= 4;							/* ÷16 */

	switch (size) {
	case 1:
		size = 2; break;					/* 2ﾊﾞｲﾄ長 */
	case 2:
		size = 4; break;					/* 4ﾊﾞｲﾄ長 */
	default:
		size = 1; break;					/* 1ﾊﾞｲﾄ長 */
	}

	do {
		cxprintf(disp, "%08X:  ", addr);

		for (i=0; i<16; i+=size) {
			if (i==8)
				cxprintf(disp, " ");

			switch (size) {
			case 1:
				d.cdata = *(char *)addr;
				cxprintf(disp, "%02X ", (uint8_t)d.cdata);
				break;
			case 2:
				d.sdata = *(short *)addr;
				cxprintf(disp, "%04X ", (uint16_t)d.sdata);
				break;
			case 4:
				d.ldata = *(long *)addr;
				cxprintf(disp, "%08X ", d.ldata);
				break;
			}

			for (j=0; j<size; j++) {
				c = d.strdata[j];
				buff[i+j] = (isprint(c)) ? c : '.';
			}

			addr += size;
		}
		buff[16] = '\0';

		cxprintf(disp, "  %s\n", buff);

		if (kbhit()) {
			if (getch() == ' ')
				break;						/* 中断 */
		}
	} while (--count > 0);

	return addr;
}


/*================================================
;|
;|□mem_dump            《ﾒﾓﾘﾀﾞﾝﾌﾟｻﾌﾞﾙｰﾁﾝ》
;|  ［形式］#include <misc/libmisc.h>
;|          uint32_t mem_dump(uint32_t addr, long count, int size);
;|  ［引数］addr…表示開始ｱﾄﾞﾚｽ
;|          count…表示ﾊﾞｲﾄｶｳﾝﾄ
;|          size…ﾒﾓﾘｻｲｽﾞ(0:byte, 1:word, 2:long)
;|  ［戻値］最後に対象となったaddr
;|  ［Note］・SPACEキーの入力により表示を中断する。
;|
 ================================================*/
uint32_t mem_dump(uint32_t addr, long count, int size)
{
	return mem_dumpx(addr, count, size, 1);		/* 標準出力 */
}


/*================================================
;|
;|□mem_set             《ﾒﾓﾘｾｯﾄﾙｰﾁﾝ》
;|  ［形式］#include <misc/libmisc.h>
;|          uint32_t mem_set(uint32_t addr, int size);
;|  ［引数］addr…開始ｱﾄﾞﾚｽ
;|          size…ﾒﾓﾘｻｲｽﾞ(0:byte, 1:word, 2:long)
;|  ［戻値］最後に対象となったaddr
;|  ［Note］
;|
 ================================================*/
uint32_t mem_set(uint32_t addr, int size)
{
	char buff[15], *p;					/* ｷｰ入力ﾊﾞｯﾌｧ */
	int len;

	switch (size) {
	case 1:
		addr &= 0xfffffffeL;			/* 2の倍数に調整 */
		size = 2;						/* 2ﾊﾞｲﾄ長 */
		break;

	case 2:
		addr &= 0xfffffffcL;			/* 4の倍数に調整 */
		size = 4;						/* 4ﾊﾞｲﾄ長 */
		break;

	default:
		size = 1;						/* 1ﾊﾞｲﾄ長 */
		break;
	}

	while (1) {
		cprintf("%08X ", addr);

		switch (size) {
		case 1:
			cprintf("%02X - ", (uint8_t)(*(char *)addr)); break;
		case 2:
			cprintf("%04X - ", (uint16_t)(*(short *)addr)); break;
		case 4:
			cprintf("%08X - ", (uint32_t)(*(long *)addr)); break;
		}

		buff[0] = 10;
		p = cgets(buff);					/* 一行入力 */
///		disp_nl(1);
		len = buff[1];
		if (len == 0) {
			addr += size;
			continue;						/* 入力文字がなければ次へ */
		}

		if (*p == '-') {
			addr -= size;
			continue;
		}
		if (*p == ',') {
			continue;
		}
		else if (*p == '.') {
			return addr;					/* 終了 */
		}
		else
			;

		switch (size) {
		case 1:
			*(char *)addr = atochex(p);		/* ﾃﾞｰﾀのｾｯﾄ */
			break;
		case 2:
			*(short *)addr = atoshex(p);	/* ﾃﾞｰﾀのｾｯﾄ */
			break;
		case 4:
			*(long *)addr = atolhex(p);		/* ﾃﾞｰﾀのｾｯﾄ */
			break;
		}

		/* 最後の文字が','の時はｱﾄﾞﾚｽを進めない */
		if (p[len - 1] != ',')
			addr += size;
	}
}


/*----------------------------------------------------------------------
#|History of "disp_mem.c"
#|=======================
#|* 2016/02/11	New created.(By T.Yokobayashi)
#|  "sh_std/lib/disp_mem.c"を流用
#|* 2016/11/09	mem_set()で','文字ﾁｪｯｸで、strchr()を使わないように変更。
#|
*/
