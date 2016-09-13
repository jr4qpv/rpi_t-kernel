/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/03/02.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/01/18.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)testdata.lnk (testdata) 2016/04/20
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)


SECTIONS {
	/* start address of data section = ROMInfo in rominfo_depend.h */
/*	.data 0x8c080000 :		/* /// */
	.data 0x00030000 :		/* /// */
	{
		*(.text)
		*(.data)
	}
}


/*----------------------------------------------------------------------
#|【testdata.lds 変更履歴】
#|□2016/01/18	新規作成
#|
*/
