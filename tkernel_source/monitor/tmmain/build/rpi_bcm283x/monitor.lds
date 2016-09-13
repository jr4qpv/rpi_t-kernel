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
 *    Modified by T.Yokobayashi at 2016/02/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)monitor.lds (monitor) 2016/04/16
 *
 *       link specification for monitor
 */

/* OUTPUT_FORMAT("elf32-larm-tkernel", "elf32-barm-tkernel", "elf32-larm-tkernel") */
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY
{
	DRAM0(rwx) : ORIGIN = 0x0, LENGTH = 0x8000
	DRAM1(rwx) : ORIGIN = 0x8000, LENGTH = 0x0fff8000
	DRAM2(rwx) : ORIGIN = 0x10000000, LENGTH = 0x10000000
}

SECTIONS {
	_start = 0x00008000;		/* org:_start = 0x70000000; */

	.eitbra _start : AT(_start)
	{
		__eitbra_start = .;
		*(EITBRA)
		. = ALIGN(0x10);
	} > DRAM1

	.startup ALIGN(0x100) :
	{
		__loadaddr = .;		/* org: __loadaddr = 0x70000000; */
		*(.startup)
		. = ALIGN(0x10);
	} > DRAM1

	.text ALIGN(0x10) :
	{
		__text_org = .;
		_stext = .;
		KEEP (*(.init))
		*(.text)
		*(.text.*)
		*(.stub)
		*(.glue_7t)
		*(.glue_7)
		KEEP (*(.fini))
		PROVIDE (__etext = .);
		PROVIDE (_etext = .);
		PROVIDE (etext = .);
		*(.rodata)
		*(.rodata.*)
		*(.rodata1)
		. = ALIGN(0x10);
	} > DRAM1 =0

	.etc_contents ALIGN(0x10) :
	{
		__data_org = . ;			/* .data */
		. = . + (_edata - __data_start);
		__flashwr_org = .;			/* .flashwr */
		. = . + (__flashwr_end - __flashwr_start);
	} > DRAM1


/*org:	.data 0x30004440 : AT(LOADADDR(.text) + SIZEOF(.text)) {	*/
	.data 0x10004440 : AT(__data_org)
	{
		__data_start = . ;
		*(flashwr.rodata)
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
		*(.got.plt)
		*(.got)
		*(.sdata)
		*(.sdata.*)
		. = ALIGN(0x10);
		_edata = .;
		PROVIDE (edata = .);
	} > DRAM2

	.bss (NOLOAD) :
	{
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
		*(EXCLUDE_FILE(*/wrkbuf.o) COMMON)
		. = ALIGN(0x10);
		_end = .;
		PROVIDE (end = .);
	} > DRAM2

	OVERLAY : AT(__flashwr_org)
	{
		.flashwr {
			__flashwr_start = .;
			*(flashwr.text)
			__flashwr_end = .;
		}
		.wrkbuf {
			*/wrkbuf.o(COMMON)
		}
	} > DRAM2

	.stack ALIGN(0x10) (NOLOAD) :
	{
		__stack_top    = .;
/*org:		__stack_bottom = 0x30006000 - ABSOLUTE(.);	*/
/*		__stack_bottom = 0x10006000 - ABSOLUTE(.);	/* /// */
		. = ALIGN(0x1000); 	/* /// */
		__stack_bottom = .;
	} > DRAM2
}


/*----------------------------------------------------------------------
#|History of "monitor.lds"
#|========================
#|* 2016/02/29	../tef_em1d/から、 [rpi_bcm283x]用にコピーし修正。
#|* 2016/02/29	拡張子は｢.lnk｣→｢.lds｣に変更｡(Windowsでは｢.lnk｣は特殊ﾌｧｲﾙの為)
#|* 2016/03/16	KPIT GCCに対応の為、OUTPUT_FORMAT 指定を
#|  "elf32-larm-tkernel"→"elf32-littlearm",
#|  "elf32-barm-tkernel"→"elf32-bigarm" に変更
#|* 2016/04/14	MEMORY記述を追加し、全体を読みやすく整理し修正
#|
*/
