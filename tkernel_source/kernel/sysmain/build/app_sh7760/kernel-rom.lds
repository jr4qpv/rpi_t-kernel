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
 *    Modified by T.Yokobayashi at 2015/12/17.
 *
 *----------------------------------------------------------------------
 */

/*
 *      @(#)kernel-rom.lnk (sysmain, SH7760) 2016/11/09
 *	linker script (ROM)
 */

/* OUTPUT_FORMAT("elf32-sh-tkernel", "elf32-sh-tkernel", "elf32-sh-tkernel") */
OUTPUT_FORMAT("elf32-shl", "elf32-shl", "elf32-shl")
OUTPUT_ARCH(sh)
/* ENTRY(_start) */
ENTRY(__start)

SECTIONS
{
	.text 0x80020000 : {
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

	__data_org = . ;
	___data_org = __data_org;                 /* for Csym(1) */
	.data 0x8c002000 : AT(__data_org) {
		__data_start = . ;
		___data_start = __data_start;     /* for Csym(1) */
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
		__edata = _edata;                   /* for Csym(1) */
		PROVIDE (edata = .);
	}

	.bss ALIGN(4) : {
		__bss_start = .;
		___bss_start = __bss_start;         /* for Csym(1) */
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
		__end = _end;                        /* for Csym(1) */
		PROVIDE (end = .);
	}
}


/*----------------------------------------------------------------------
#|History of "kernel-rom.lds"
#|===========================
#|* 2015/12/17	../tef_em1d/から、 [app_sh7760]用にコピーし、修正。
#|* 2015/12/17	拡張子は｢.lnk｣→｢.lds｣に変更｡(Windowsでは｢.lnk｣は特殊ﾌｧｲﾙの為)
#|* 2016/11/09	KPIT GCCに対応の為、OUTPUT_FORMAT 指定を
#|  "elf32-sh-tkernel"→"elf32-shl" に変更
#|* 2016/11/09	KPIT GCCではC言語から参照するﾗﾍﾞﾙには`_`付加する対応 Csym(1)
#|
*/
