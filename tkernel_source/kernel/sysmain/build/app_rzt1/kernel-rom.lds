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
 *    Modified by T.Yokobayashi at 2016/03/14.
 *
 *----------------------------------------------------------------------
 */

/*
 *      @(#)kernel-rom.lnk (sysmain, RZT1) 2016/03/16
 *	linker script (ROM)
 */

/* OUTPUT_FORMAT("elf32-larm-tkernel", "elf32-barm-tkernel", "elf32-larm-tkernel") */
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS {
	.text 0x30030000 : {
		KEEP (*(.init))
		*(.text)
		*(.text.*)
		*(.stub)
		KEEP (*(.fini))
		PROVIDE (__etext = .);
		PROVIDE (_etext = .);
		PROVIDE (etext = .);
		*(.rodata)
		*(.rodata.*)
		*(.rodata1)
		. = ALIGN(0x10);
	} =0
	__ARM_exidx = .;
	.ARM.exidx : { *(.ARM.exidx*) }
	__data_org = . ;
	.data 0x30006000 : AT(__data_org) {
		__data_start = . ;
		*(.data)
		*(.data.*)
		SORT(CONSTRUCTORS)
		*(.data1)
		KEEP (*(.eh_frame))
		*(.gcc_except_table)
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o ) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o ) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
		*(.sdata)
		*(.sdata.*)
		. = ALIGN(4);
		_edata = .;
		PROVIDE (edata = .);
	}
	.bss ALIGN(4) : {
		__bss_start = .;
		PROVIDE (__sbss_start = .);
		PROVIDE (___sbss_start = .);
		*(.sbss)
		*(.sbss.*)
		*(.scommon)
		PROVIDE (__sbss_end = .);
		PROVIDE (___sbss_end = .);
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		_end = .;
		PROVIDE (end = .);
	}
}


/*----------------------------------------------------------------------
#|History of "kernel-rom.lds"
#|===========================
#|* 2016/03/14	../tef_em1d/から、 [app_rzt1]用にコピーし、修正。
#|* 2016/03/14	拡張子は｢.lnk｣→｢.lds｣に変更｡(Windowsでは｢.lnk｣は特殊ﾌｧｲﾙの為)
#|* 2016/03/16	KPIT GCCに対応の為、OUTPUT_FORMAT 指定を
#|  "elf32-larm-tkernel"→"elf32-littlearm",
#|  "elf32-barm-tkernel"→"elf32-bigarm" に変更
#|* 2016/03/16	.ARM.exidx ｾｸｼｮﾝの定義追加
#|
*/
