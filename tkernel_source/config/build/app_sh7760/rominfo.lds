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
 *    Modified by T.Yokobayashi at 2015/12/17.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)rominfo.lnk (config, SH7760) 2016/11/10
 *	ROM information linker script
 */

/* OUTPUT_FORMAT("elf32-sh-tkernel", "elf32-sh-tkernel", "elf32-sh-tkernel") */
OUTPUT_FORMAT("elf32-shl", "elf32-shl", "elf32-shl")
OUTPUT_ARCH(sh)

PHDRS {
	data	PT_LOAD;
}

SECTIONS {
	/* start address of data section = ROMInfo in rominfo_depend.h */
	.data 0x80010000 : {
		*(.data)
		*(.text)
	} :data
}


/*----------------------------------------------------------------------
#|History of "rominfo.lds"
#|========================
#|* 2015/12/17	../tef_em1d/から、 [app_sh7760]用にコピーし、修正。
#|* 2015/12/17	拡張子は｢.lnk｣→｢.lds｣に変更｡(Windowsでは｢.lnk｣は特殊ﾌｧｲﾙの為)
#|* 2016/11/10	KPIT GCCに対応の為、OUTPUT_FORMAT 指定を
#|  "elf32-sh-tkernel"→"elf32-shl" に変更
#|
*/
