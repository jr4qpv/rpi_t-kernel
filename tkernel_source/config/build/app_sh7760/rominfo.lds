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
 *	rominfo.lnk (config, SH7760)
 *	ROM information linker script
 */

OUTPUT_FORMAT("elf32-sh-tkernel", "elf32-sh-tkernel", "elf32-sh-tkernel")
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


/*----------------------------------------------------------------------*/
/*
#|【moninfo.lds 変更履歴】
#|□2015/12/17	../tef_em1d/から、 [app_sh7760]用にコピーし、修正。
#|□2015/12/17	拡張子は｢.lnk｣→｢.lds｣に変更｡(Windowsでは｢.lnk｣は特殊ﾌｧｲﾙの為)
#|
 */
