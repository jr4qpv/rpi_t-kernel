/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/01/22.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sst_16x1.c () 2016/04/23
 *
 *       Flash ROM write: SST specification 16bit x 1 configuration
 *
 * [Note]
 *  ・本ﾌｧｲﾙ内の関数は、RAMに転送されて実行される。又、ﾌﾗｯｼｭの消去中等の
 *    ｱｸｾｽ中はﾌﾗｯｼｭROMが見えなくなるので、ROM上の関数はｺｰﾙしない事!!
 *
 */

#include <tmonitor.h>
#include "flash.h"

#ifndef	FLASH_TOP_ADDR
#define	FLASH_TOP_ADDR	0xa0000000	/* フラッシュROMの先頭番地 */
#endif

#ifndef	SECSZ
#define	SECSZ		0x1000			// sector size 4kB x 1 (2KWord)
#endif

#define	SECMSK		(SECSZ - 1)		// sector mask

#define	MAX_RETRY	(3)				// maximum number of retries
///#define	WAIT_CNT	0x10000000		// wait count (enough time)

///#define	WBSZ		(16)			// write buffer size (H unit)

EXPORT	const UW	FROM_SECSZ = SECSZ;	// sector size


/*------( Flash Command 定義 )------*/
typedef _UH		FWSZ;				/* 16bit幅 */
#define	BLANK_DATA				0xFFFF

/* Unlock cycle */
#define	FIRST_ADDR				0x5555
#define	SECOND_ADDR				0x2AAA
#define	THIRD_ADDR				0x5555
#define	FOURTH_ADDR				0x5555
#define	FIFTH_ADDR				0x2AAA
#define	SIXTH_ADDR				0x5555

/* Address Offset For Autoselect */
#define	MANUFACTURE_OFFSET		0x0
#define	DEVICE_OFFSET			0x01


#define	FIRST_DATA				0xAA
#define	SECOND_DATA				0x55

/* Command */
#define	RESET_COMMAND			0xF0
#define	SOFTWAREID_COMMAND		0x90
#define	PROGRAM_COMMAND			0xA0
#define	ERASE_SETUP_COMMAND		0x80
#define	SECTOR_ERASE_COMMAND	0x30
#define	CHIP_ERASE_COMMAND		0x10
#define	SUSPEND_COMMAND			0xB0
#define	RESUME_COMMAND			0x30

/* Polling Bit */
#define	DATA_POLLING_BIT		0x80
#define	TOGGLE_BIT				0x40
#define	HANGUP_BIT				0x20		/* for MBM29LV800TA */

/* Flag Talbe */
#define	PASS_FLAG				0
#define	FAIL_FLAG				1
#define	ERROR_FLAG				2




/*------------------------------------------------
;
;□Data_Polling_Check   《データポーリング終了検出》
;  ［形式］int Data_Polling_Check(FWSZ *addr, FWSZ data);
;  ［引数］addr…ﾌﾗｯｼｭﾒﾓﾘｰのｱﾄﾞﾚｽ
;          data…ﾎﾟｰﾘﾝｸﾞﾃﾞｰﾀ
;  ［戻値］＝0:正常, ≠0:エラー
;  ［Note］・
;
 ------------------------------------------------*/
LOCAL	int Data_Polling_Check(FWSZ *addr, FWSZ data)
{
	static short cn = 0;					/* 本関数の呼び出し回数 */
	FWSZ read_data, read_data0;

	if (1) {								/* for SST39VF800A */
		read_data = *addr;
		do {
			read_data0 = read_data;			/* 前回値の記憶 */
			read_data = *addr;

			cn++;
#if 1
			WDT_dog_food();					/* WDTトリガ */
#else
			if ((cn & 0x7FF) == 0)			/* 2048回に1回? */
				p_delay(1);					/* 他のﾀｽｸに回す機会を与える */
			else
				p_switch();					/* DOG_FOODの目的 */
#endif
			/* ///// ﾀｲﾑｱｳﾄをﾘﾌﾚｯｼｭｶｳﾝﾀなど使い監視が必要 ///// */

		} while (((read_data ^ read_data0) & TOGGLE_BIT) != 0);	/* 一致? */
	}
	else {									/* for MBM29LV800TA */
#if 0
		read_data0 = data & DATA_POLLING_BIT;
		while (1) {
			read_data = *addr;
			if ((FWSZ)(read_data & DATA_POLLING_BIT) == read_data0)
				break;

			if (read_data & HANGUP_BIT) {	/* DQ6=1 ? */
				read_data = *addr;

				if ((FWSZ)(read_data & DATA_POLLING_BIT) == read_data0) {
					break;
				}
				else {
					FMB_Reset((long)addr);
					return FAIL_FLAG;
				}
			}

			cn++;
			if ((cn & 0x7FF) == 0)			/* 2048回に1回? */
				p_delay(1);					/* 他のﾀｽｸに回す機会を与える */
			else
				p_switch();					/* DOG_FOODの目的 */
		}
#endif
	}

	read_data = *addr;
	if (read_data != data) {
		*addr = RESET_COMMAND;
		return FAIL_FLAG;
	}

	return PASS_FLAG;
}


/*------------------------------------------------
;
;□Toggle_Bit_Check     《トグルビット終了検出》
;  ［形式］int Toggle_Bit_Check(int ftyp, FWSZ *volatile addr, int disp);
;  ［引数］addr…ﾌﾗｯｼｭﾒﾓﾘｰのｱﾄﾞﾚｽ
;          disp…1秒経過表示の'.'を表示(0:しない,1:標準出力,2:ｴﾗｰ出力)
;  ［戻値］＝0:正常, ≠0:エラー
;  ［Note］・
;
 ------------------------------------------------*/
#if 0	/////////////////////
LOCAL	int Toggle_Bit_Check(FWSZ *addr, int disp)
{
	int i;
	FWSZ read_data, read_data0;

	i = 0;
///	p_delay(10);							/* 10m秒待ち */

	if (1) {								/* for SST39VF800A */
		read_data = *addr;
		do {

			/* ///// ﾀｲﾑｱｳﾄをﾘﾌﾚｯｼｭｶｳﾝﾀなど使い監視が必要 ///// */

#if 1
			WDT_dog_food();					/* WDTトリガ */
#else
			p_delay(10);					/* 10m秒待ち */
#endif
			if (i++ > 100) {
///				cxprintf(disp, ".");
				i = 0;
			}

			read_data0 = read_data;			/* 前回値の記憶 */
			read_data = *addr;
		} while (((read_data ^ read_data0) & TOGGLE_BIT) != 0);	/* 一致? */
	}
	else {									/* for MBM29LV800TA */
#if 0
		read_data = *addr;
		while (1) {
			p_delay(10);					/* 10m秒待ち */
			if (i++ > 100) {
				cxprintf(disp, ".");
				i = 0;
			}

			read_data0 = read_data;			/* 前回値の記憶 */
			read_data = *addr;
			if (((read_data ^ read_data0) & TOGGLE_BIT) == 0)	/* 一致? */
				break;

			if (read_data & HANGUP_BIT) {	/* DQ6=1 ? */
				read_data0 = *addr;
				read_data = *addr;
				if (((read_data ^ read_data0) & TOGGLE_BIT) == 0) {	/* 一致? */
					break;
				}
				else {
					FMB_Reset((long)addr);
					return FAIL_FLAG;
				}

			}
		}
#endif
	}

	return PASS_FLAG;
}
#endif	////////////////////////////////////////


/*
 * check Flash ROM
 */
LOCAL	const JEDEC_SPEC	*checkFlashROM(_UH *rom)
{
	const JEDEC_SPEC	*spec;
	_UH	*flash;
	UH	man, dev;
	W	i;

	flash = (_UH *)FLASH_TOP_ADDR;

	/* read Signature */
	flash[FIRST_ADDR]  = FIRST_DATA;
	flash[SECOND_ADDR] = SECOND_DATA;
	flash[THIRD_ADDR]  = SOFTWAREID_COMMAND;

	man = flash[MANUFACTURE_OFFSET] & 0x00ff;	// ignore upper 8 bits
	dev = flash[DEVICE_OFFSET] & 0x00ff;		// ignore upper 8 bits

	flash[0] = RESET_COMMAND;			// ID読み出し終了

	/* obtain Flash ROM specification */
	for (i = 0; i < N_JedecSpec; ++i) {
		spec = &JedecSpec[i];
		if (spec->man != man || spec->dev != dev) continue;

		return spec;
	}

	return NULL;	// unsupported target
}


/*
 * write one sector
 *       rom should be the beginning of sector
 *       if data = NULL, only erasure is performed
 */
LOCAL	ER	writesec(_UH *rom, UH *data, const JEDEC_SPEC *spec)
{
	_UH	*rp;
	_UH	*flash;
	UH	*dp, *ep;
	UW	mask;
	UH	d;

#if 0	/* for debug */
	DSP_F4(S,"\writesec() rom=",08X,(UW)rom,S,",data=",08X,(UW)data);
	DSP_F3(S,",spec=",08X,(UW)spec,S,"\n");
	return E_OK;
#endif

	mask = ~(spec->size * (1024*1024) - 1);
	flash = (_UH *)((UW)rom & mask);		/* ﾌﾗｯｼｭROMの先頭番地 */

	/*--- erase sector ---*/
	/* Command write */
	flash[FIRST_ADDR]  = FIRST_DATA;
	flash[SECOND_ADDR] = SECOND_DATA;
	flash[THIRD_ADDR]  = ERASE_SETUP_COMMAND;
	flash[FOURTH_ADDR] = FIRST_DATA;
	flash[FIFTH_ADDR]  = SECOND_DATA;
	*rom = SECTOR_ERASE_COMMAND;
///	p_delay(1);								/* 複数ｾｸﾀ消去受付期間ｽｷｯﾌﾟ */

	/* Data Polling Check */
	if (Data_Polling_Check(rom, BLANK_DATA)) {
		return E_IO;						/* 異常 */
	}

	if (data == NULL) return E_OK;			// erase only

	/*--- write ---*/
	rp = rom;
	ep = data + SECSZ / sizeof(UH);
	for (dp = data; dp < ep; dp++) {
		d = *dp;
		if (d == BLANK_DATA) {
			rp++;
			continue;						/* skip */
		}

		/* Command write */
		flash[FIRST_ADDR]  = FIRST_DATA;
		flash[SECOND_ADDR] = SECOND_DATA;
		flash[THIRD_ADDR]  = PROGRAM_COMMAND;
		*rp = d;

		/* Data Polling Check */
		if (Data_Polling_Check(rp, d)) {
			goto abort;
		}

		rp++;
	}

 abort:
	/* write end */
///	*rom = 0x00FF;
	if (dp < ep) return E_IO;

	/* Verify write */
	for (dp = data; dp < ep; ) {
		if (*rom ++ != *dp++) return E_IO;
	}

	return E_OK;
}


/*
 * FlashROM write
 */
EXPORT	ER	flashwr(UW addr, void *data, W nsec, BOOL reset)
{
	const JEDEC_SPEC	*spec;
	_UH	*rom;
	W	sec, retry;
	ER	err;

	/* FlashROM sector address */
	rom = (_UH *)NOCACHE_ADDR(addr & ~SECMSK);	/* セクタの先頭番地 */

	/* check FlashROM */
	spec = checkFlashROM(rom);

	/* reset FlashROM */
	*rom = RESET_COMMAND;

	/* report error for unsupported FlashROM */
	if (spec == NULL) return E_IO;

	/* erase or write in sector unit */
	for (sec = 0; sec < nsec; sec++) {
		retry = MAX_RETRY;

		do {
			err = writesec(rom, data, spec);
			if (err >= E_OK) break;

			/* firstly, reset */
///			*rom = 0x00FF;
		} while (--retry > 0);

		if (err < E_OK) return err;

		rom = (_UH *)((B *)rom + SECSZ);
		data = (B *)data + SECSZ;
	}

	if (reset) flashwr_reset();	// do not return

	return E_OK;
}


/*----------------------------------------------------------------------
#|History of "sst_16x1.c"
#|=======================
#|* 2016/01/22	[app_sh7760]用に、[tef_em1d]の"cfi_16x1.c"と、
#|  共通ライブラリー"flash.c"を参考に作成。
#|
*/
