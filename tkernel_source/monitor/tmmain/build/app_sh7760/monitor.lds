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
 *    Modified by T.Yokobayashi at 2015/11/25.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)monitor.lds () 2016/03/16
 *
 *	linker script for monitor
 */

OUTPUT_FORMAT("elf32-sh-tkernel", "elf32-sh-tkernel", "elf32-sh-tkernel")
/* OUTPUT_FORMAT("elf32-sh", "elf32-sh", "elf32-sh") */
OUTPUT_ARCH(sh)
ENTRY(_start)

SECTIONS {
	__loadaddr = 0x80000000;
	_start = 0x80000000;

	.eitbra _start : AT(__loadaddr) {
/*		__eitbra_start = .;	/* ///bug?/// */
		*(VBRHDR)
		. = ALIGN(0x10);
	}
	__eitbra_start = ADDR(.eitbra);	/* ///bug対策/// */

	.startup  : {
		*(.startup)
		. = ALIGN(0x10);
	}

	__text_org = __loadaddr + SIZEOF(.eitbra) + SIZEOF(.startup);	/**/
	.text __text_org : {
/*		_stext = .;		/* ///bug?/// */
		KEEP (*(.init))
		*(EXCLUDE_FILE(*/flashwr.o) .text)	/* flashwr.o以外 */
		*(.text.*)
		*(.stub)
		*(.glue_7t)
		*(.glue_7)
		KEEP (*(.fini))
		PROVIDE (__etext = .);
		PROVIDE (_etext = .);
		PROVIDE (etext = .);
		*(EXCLUDE_FILE(*/flashwr.o) .rodata)	/* flashwr.o以外 */
		*(.rodata.*)
		*(.rodata1)
		. = ALIGN(0x10);
	} =0
	_stext = ADDR(.text);		/* ///bug対策/// */

	__data_org = LOADADDR(.text) + SIZEOF(.text);
	.data 0x8c000600 : AT(__data_org) {
/*		__isp_bottom = .;	/* ///bug?/// */
/*		__data_start = . ;	/* ///bug?/// */
/*		*(flashwr.rodata)	/* /// */
		*/flashwr.o(.rodata)
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
	}
	__isp_bottm = ADDR(.data);	/* ///bug対策/// */
	__data_start = ADDR(.data);	/* ///bug対策/// */

	.bss (NOLOAD) : AT(LOADADDR(.data) + SIZEOF(.data)) {
/*		__bss_start = .;	/* ///bug?/// */
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
	}
	__bss_start = ADDR(.bss);	/* ///bug対策/// */

	__flashwr_org = __data_org + SIZEOF(.data);
	OVERLAY : AT(LOADADDR(.bss)) {	
		.flashwr {
/*			__flashwr_start = .;	/* ///bug?/// */
/*			*(flashwr.text)		/* /// */
			*/flashwr.o(.text)
			__flashwr_end = .;
		}

		.wrkbuf {
			*/wrkbuf.o(COMMON)
		}
	}
	__flashwr_start = ADDR(.flashwr);	/* ///bug対策/// */

	.stack ALIGN(0x10) (NOLOAD) : {
		__stack_top    = .;
		__stack_bottom = 0x8c002000 - ABSOLUTE(.);
	}
}


/*----------------------------------------------------------------------*/
/*
#|History of "monitor.lds"
#|* 2015/11/25	../tef_em1d/から、 [app_sh7760]用にコピーし修正。
#|* 2015/11/28	拡張子は｢.lnk｣→｢.lds｣に変更｡(Windowsでは｢.lnk｣は特殊ﾌｧｲﾙの為)
#|* 2015/12/09	ｾｸｼｮﾝﾌﾞﾛｯｸ内最初の｢_stext = .;｣の記述がどうもおかしい(ﾊﾞｸﾞ?)、
#|		ｾｸｼｮﾝﾌﾞﾛｯｸの後ろに｢_stext = ADDR(.text);｣の記述で回避。
#|* 2016/01/14	SH版GCC3.0.4の、OBJCOPYは「--rename-section」オプションに対応
#|  していないので、"flashwr.o"の.textと.rodataセクションをリンクスクリプトで
#|  RAMにマッピングするように修正。
#|* 2016/03/16	KPIT GCCに対応の為、OUTPUT_FORMAT 指定を
#|  "elf32-sh-tkernel"→"elf32-sh" に変更
#|
 */
