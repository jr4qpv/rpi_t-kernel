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
 *	monitor.lds (monitor) 2016/04/12
 *
 *      boot link specification for monitor
 */

/* OUTPUT_FORMAT("elf32-larm-tkernel", "elf32-barm-tkernel", "elf32-larm-tkernel") */
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
/* ENTRY(_start) */
/*// ENTRY(_PowerON_Reset)		/* for SFLASH,NOR boot */
ENTRY(_start_loader_copy) 		/* for RAM debug boot */

MEMORY
{
	ATCM : ORIGIN = 0x0, LENGTH = 524288
	BTCM : ORIGIN = 0x800000, LENGTH = 32768
	INST_RAM : ORIGIN = 0x4000000, LENGTH = 524288
	SPIBSC : ORIGIN = 0x10000000, LENGTH = 67108864
	DATA_RAM : ORIGIN = 0x20000000, LENGTH = 524288
	DATA_RAM_MIRROR : ORIGIN = 0x22000000, LENGTH = 524288
	INST_RAM_MIRROR : ORIGIN = 0x24000000, LENGTH = 524288
	SPIBSC_MIRROR : ORIGIN = 0x30000000, LENGTH = 67108864
	CSx_MIRROR : ORIGIN = 0x40000000, LENGTH = 402653184
	CSx : ORIGIN = 0x60000000, LENGTH = 402653184
}

SECTIONS {
/*//	__rom_start = 0x30000000;	/* for SFLASH boot top addres */
/*//	__rom_start = 0x40000000;	/* for NOR boot top addres */
	__rom_start = 0x4040000;	/* for RAM debug boot top addres */

	.loader_param __rom_start : AT (__rom_start)	/* ROM boot to 0x800000 */
	{
		KEEP(*(.loader_param))
		end = .;
/*//	} > SPIBSC_MIRROR		/* for SFLASH boot */
/*//	} > CSx_MIRROR			/* for NOR boot */
	} > INST_RAM			/* for RAM debug boot */

	.flash_contents __rom_start + 0x4C : AT (__rom_start + 0x4C)
	{
		_mloader_text = .;			/* .loader_text */
		. = . + (_loader_text_end - _loader_text_start);
		_mtext = .;				/* .eitbra */
		. = . + (_text_end - _text_start);
		_mdata = .;				/* .data */
		__data_org = . ;	/* /// 使ってない? 削除する /// */
		. = . + (_data_end - _data_start);
		__flashwr_org = .;			/* .flashwr */
		. = . + (__flashwr_end - __flashwr_start);
/*//	} > SPIBSC_MIRROR		/* for SFLASH boot */
/*//	} > CSx_MIRROR			/* for NOR boot */
	} > INST_RAM			/* for RAM debug boot */

	.startup ALIGN(0x100) :
	{
		__loadaddr = .;		/* org: __loadaddr = 0x70000000; */
		_start = .;		/* org: _start = 0x70000000; */
		KEEP(*(.startup))
		. = ALIGN(0x10);
/*//	} > SPIBSC_MIRROR		/* for SFLASH boot */
/*//	} > CSx_MIRROR			/* for NOR boot */
	} > INST_RAM			/* for RAM debug boot */

	.text ALIGN(0x10) :
	{
		__text_org = .;		/* /// 使ってない? 削除する /// */
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
/*//	} > SPIBSC_MIRROR = 0		/* for SFLASH boot */
/*//	} > CSx_MIRROR = 0		/* for NOR boot */
	} > INST_RAM = 0		/* for RAM debug boot */


	.eitbra 0x0 : AT(_mtext)
	{
		_text_start = .;
		__eitbra_start = .;
		KEEP(*(EITBRA))
		. = ALIGN(0x10);
		_text_end = .;
	} > ATCM

	.data 0x00004440 : AT(_mdata)
	{
		_data_start = .;
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
		_data_end = .;
	} > ATCM

	.bss (NOLOAD) :
	{
		_bss = .;
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
		_ebss = .;
		_end = .;
		PROVIDE (end = .);
	} > ATCM

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
	} > ATCM

	.stack ALIGN(0x10) (NOLOAD) :
	{
		__stack_top    = .;
/*		__stack_bottom = 0x30006000 - ABSOLUTE(.);	/* /// */
/*		__stack_bottom = 0x00006000 - ABSOLUTE(.);	/* /// */
		. = ALIGN(0x1000); 	/* /// */
		__stack_bottom = .;
	} > ATCM

/*//	.loader_text 0x802000 : AT (_mloader_text)   /* for SFLASH,NOR boot */
	.loader_text 0x4078000 : AT (_mloader_text)  /* for RAM debug boot */
	{
		_loader_text_start = .;
		*(.loader_text)
/*//	} > BTCM			/* for SFLASH,NOR boot */
	} > INST_RAM			/* for RAM debug boot */
	.loader_text2 ALIGN(0x10) :
	{
		*(.loader_text2)
		. = . + (512 - ((. - _loader_text_start) % 512));
		_loader_text_end = .;
/*//	} > BTCM			/* for SFLASH,NOR boot */
	} > INST_RAM			/* for RAM debug boot */

	.sys_stack 0x807A00 (NOLOAD)  : AT (0x807A00)
	{
		*(.sys_stack)
		_sys_stack = .;
	} > BTCM
	.svc_stack 0x807C00 (NOLOAD)  : AT (0x807C00)
	{
		*(.svc_stack)
		_svc_stack = .;
	} > BTCM
	.irq_stack 0x807D00 (NOLOAD)  : AT (0x807D00)
	{
		*(.irq_stack)
		_irq_stack = .;
	} > BTCM
	.fiq_stack 0x807E00 (NOLOAD)  : AT (0x807E00)
	{
		*(.fiq_stack)
		_fiq_stack = .;
	} > BTCM
	.und_stack 0x807F00 (NOLOAD)  : AT (0x807F00)
	{
		*(.und_stack)
		_und_stack = .;
	} > BTCM
	.abt_stack 0x808000 (NOLOAD)  : AT (0x808000)
	{
		*(.und_stack)
		_abt_stack = .;
	}
}


/*----------------------------------------------------------------------
#|History of "monitor.lds"
#|========================
#|* 2016/02/29	../tef_em1d/から、 [app_rzt1]用にコピーし修正。
#|* 2016/02/29	拡張子は｢.lnk｣→｢.lds｣に変更｡(Windowsでは｢.lnk｣は特殊ﾌｧｲﾙの為)
#|* 2016/03/16	KPIT GCCに対応の為、OUTPUT_FORMAT 指定を
#|  "elf32-larm-tkernel"→"elf32-littlearm",
#|  "elf32-barm-tkernel"→"elf32-bigarm" に変更
#|* 2016/03/30	RZ/T1用のMEMORY記述を追加
#|* 2016/03/30	ﾙﾈｻｽのRZ/T1｢初期設定(nor起動)｣ｻﾝﾌﾟﾙｺｰﾄﾞを組み込み
#|* 2016/04/01	RAM起動用の設定もｺﾒﾝﾄで埋め込み
#|* 2016/04/02	SFLASH起動用の設定追加し変更
#|
*/
