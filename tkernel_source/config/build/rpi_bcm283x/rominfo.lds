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
 *    Modified by T.Yokobayashi at 2016/03/01.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)rominfo.lnk (config, BCM283x) 2016/08/18
 *	ROM information linker script
 */

/* OUTPUT_FORMAT("elf32-larm-tkernel", "elf32-barm-tkernel", "elf32-larm-tkernel") */
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(START)


PHDRS {
	data	PT_LOAD;
}

SECTIONS {
	/* start address of data section = ROMInfo */
	.data 0x00004000 : {
		*(.data)
		*(.text)
	} :data
}


/*----------------------------------------------------------------------
#|History of "rominfo.lds"
#|========================
#|* 2016/03/01	../tef_em1d/から、 [rpi_bcm283x]用にコピーし修正。
#|* 2016/03/01	拡張子は｢.lnk｣→｢.lds｣に変更｡(Windowsでは｢.lnk｣は特殊ﾌｧｲﾙの為)
#|* 2016/07/19	KPIT GCCに対応の為、OUTPUT_FORMAT 指定を
#|  "elf32-larm-tkernel"→"elf32-littlearm",
#|  "elf32-barm-tkernel"→"elf32-bigarm" に変更
#|* 2016/08/18	ROMINFOは0x4000番地配置に変更
#|
*/
