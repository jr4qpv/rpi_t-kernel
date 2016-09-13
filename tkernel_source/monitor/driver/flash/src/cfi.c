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
 *    Modified by T.Yokobayashi at 2016/01/14.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cfi.c
 *
 *       CFI(Intel) Flash ROM specification
 */

#include "flash.h"

/*
  Flash ROM specification:
	Intel 28F256P30B 32MB (256Mbits)
		 32 KB x 4
		128 KB x 255

  command:
	ba: Block Addr, xa: Any Addr, sr: Status Reg, xsr: extend Status Reg.

	Reset(Read Array):
		xa<-FF
	Block Erase:
		ba<-20, xa<-D0, sr<-xa & check
	Write (via Buffer 16 words):
		ba<-E8, xsr<-xa & check, ba<-15(count), da<-data, ..
		xa<-D0, sr<-xa & check
	Write:
		xa<-40(10), da<-data, sr<-xa & check
	Read sr:
		xa<-70, sr<-xa, if any error, xa<-50 (clear sr)
		sr: b7:0x80	1:Ready, 0:busy
		    b5:0x20	1:Error Erase
		    b4:0x10	1:Error Set Lock-bit
		    b3:0x08	1:Low Voltage Error
		    b1:0x02	1:Locked
		xsr:b7:0x80	1:WriteBuffer Available
	Clear Lock-bits:
		xa<-60, xa<-D0, sr<-xa & check
	ID read:
		xa<-90, man-id<- ba:0, dev-id <- ba:2
		man-id:	89	Intel
		dev-id:	001C	28F256P30B


  Flash ROM specification:
	SST SST39VF1601 2MB (16Mbits) 1M x16
		 2KWord(4 KB) x 512 Sector
		32KWord(64 KB) x  32 Block

*/

/*
 * CFI(Intel) Flash ROM specification
 */
EXPORT	const JEDEC_SPEC	JedecSpec[] = {
 { 0x0089, 0x001c, 0x0000, 0x0000, 32, 0, 0x8888, 0x8000 }, /* 28F256P30B */
 { 0x00bf, 0x004b, 0x0000, 0x0000, 2, 0, 0x0000, 0x0000 },  /* SST39VF1601 */
 { 0x0011, 0x0022, 0x0000, 0x0000, 64, 0, 0x0000, 0x0000 },  /* MX25L51245G ////kari */
};

EXPORT	const W	N_JedecSpec = sizeof(JedecSpec) / sizeof(JEDEC_SPEC);


/*----------------------------------------------------------------------
#|History of "cfi.c"
#|=======================
#|* 2016/01/14	[app_sh7760]用に、｢SST39VF1601｣情報を追加。
#|
#endif
*/
