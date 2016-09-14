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
 *    Modified by T.Yokobayashi at 2016/04/23.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)spi_mx25.c () 2016/04/23
 *
 *       Flash ROM write: MX25(Macronix) specification SPI configuration
 *
 * [Note]
 *  ・本ﾌｧｲﾙ内の関数は、RAMに転送されて実行される。又、ﾌﾗｯｼｭの消去中等の
 *    ｱｸｾｽ中はﾌﾗｯｼｭROMが見えなくなるので、ROM上の関数はｺｰﾙしない事!!
 *
 */

#include <tmonitor.h>
#include "flash.h"
#include "platform.h"				///////////////
#include "r_serial_flash_api.h"		//////////////
#include "serial_flash_spibsc.h"	//////////////
#include "r_main_convert.h"			///////////////

#ifndef	SECSZ
#define	SECSZ		0x1000			// sector size 4kB x 1
#endif

#define	SECMSK		(SECSZ - 1)		// sector mask

#define	MAX_RETRY	(3)			// maximum number of retries
#define	WAIT_CNT	0x10000000		// wait count (enough time)

#define	WBSZ		(16)			// write buffer size (H unit)

EXPORT	const UW	FROM_SECSZ = SECSZ;	// sector size

/*
 * check Flash ROM
 */
LOCAL	const JEDEC_SPEC	*checkFlashROM(_UB *rom)
{
	const JEDEC_SPEC	*spec;
	UH	man, dev;
	W	i;

	/* read Signature */
#if 1	/////////// kari kari ///////////
	man = 0x11;				// ignore upper 8 bits
	dev = 0x22;				// ignore upper 8 bits
#else
	rom[0] = 0x0090;
	man = rom[0] & 0x00ff;	// ignore upper 8 bits
	dev = rom[1] & 0x00ff;	// ignore upper 8 bits
#endif

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
LOCAL	ER	writesec(_UB *rom, UB *data, const JEDEC_SPEC *spec)
{
	_UB	*rp;
	UB	*dp, *ep;
	UW	n, mask;
	int32_t ret;

#if 0	/* for debug */
	DSP_F4(S,"\nwritesec() rom=",08X,(UW)rom,S,",data=",08X,(UW)data);
	DSP_F3(S,",spec=",08X,(UW)spec,S,"\n");
	return E_OK;
#endif

	mask = (spec->size * (1024*1024) - 1) & ~SECMSK;
	n = (UW)rom & mask;

	/*----- erase sector -----*/
	/* Set to SPI mode */
	R_SPIBSC_Outaddr2SPI();
	ret = R_SPIBSC_Spimode();

	if (0 != ret) {
///		DSP_S("\nFailed to erase Serial Flash(change mode)\n\n");
		return E_IO;
	}

	ret = R_SPIBSC_EraseSector(n, (uint32_t)R_SERIAL_FLASH_DATA_WIDTH,
							(uint32_t)R_SERIAL_FLASH_ADDR_MODE);

	/* Set to OutAddrMode */
	R_SPIBSC_Exmode();
	R_SPIBSC_SPI2Outaddr();
            
	if (0 != ret) {
///		DSP_S("\nFailed to erase Serial Flash(do erase)\n\n");
		return E_IO;
	}

	if (data == NULL) return E_OK;		// erase only

	/*----- write -----*/
	/* Set to SPI mode */
	R_SPIBSC_Outaddr2SPI();
	ret = R_SPIBSC_Spimode();

	if (0 != ret) {
///		DSP_S("\nSerial Flash Error\n\n");
		return E_IO;
	}

	rp = rom;
	ep = data + SECSZ / sizeof(UB);
	for (dp = data; dp < ep; ) {

		/* page program(256byte) */
		ret = R_SPIBSC_ByteProgram((uint32_t)rp, dp, 256,
				(uint32_t)R_SERIAL_FLASH_DATA_WIDTH, (uint32_t)R_SERIAL_FLASH_ADDR_MODE);

		if (0 != ret) {
///			DSP_S("\nFailed to write Serial Flash\n\n");
			goto abort;
		}

		rp += 256;
		dp += 256;						/* page size */
	}

 abort:
	/* write end */
	/* Set to OutAddrMode */
	R_SPIBSC_Exmode();
	R_SPIBSC_SPI2Outaddr();

	if (dp < ep) return E_IO;

	/* Verify write */
	rp = rom;
	for (dp = data; dp < ep; ) {
		if (*rp ++ != *dp++) return E_IO;
	}

	return E_OK;
}

/*
 * FlashROM write
 */
EXPORT	ER	flashwr(UW addr, void *data, W nsec, BOOL reset)
{
	const JEDEC_SPEC	*spec;
	_UB	*rom;
	W	sec, retry;
	ER	err;

	/* FlashROM sector address */
	rom = (_UB *)NOCACHE_ADDR(addr & ~SECMSK);

	/* check FlashROM */
	spec = checkFlashROM(rom);

///	/* reset FlashROM */
///	*rom = 0x00FF;

	/* report error for unsupported FlashROM */
	if (spec == NULL) return E_IO;

	/* erase or write in sector unit */
	for (sec = 0; sec < nsec; sec++) {
		retry = MAX_RETRY;

		do {
			err = writesec(rom, data, spec);
			if (err >= E_OK) break;

///			/* firstly, reset */
///			*rom = 0x00FF;
		} while (--retry > 0);

		if (err < E_OK) return err;

		rom = (_UB *)((B *)rom + SECSZ);
		data = (B *)data + SECSZ;
	}

	if (reset) flashwr_reset();	// do not return

	return E_OK;
}


/*----------------------------------------------------------------------
#|History of "spi_mx25.c"
#|=======================
#|* 2016/04/25	[app_rzt1]用に、[tef_em1d]用の"cfi_16x1.c"を参考に作成。
#|
*/
