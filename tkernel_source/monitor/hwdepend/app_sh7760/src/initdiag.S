/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2015 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2015/11/28.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)initdiag.S () 2015/11/28
 *
 *       SH7760(SH-4) : initial diagnosis test.
 */

/*
 *[Note]
 *・初期診断をBL=1のまま実行。
 *・CPUレジスタの診断を行い、不正の場合はLEDを点滅させる。
 *・T-Monitor(BOOT)のROMのチェックサム診断を行い、不正の場合はLEDを点滅させる。
 *・T-Monitor(BOOT)のRAMのリード/ライト診断を行い、不正の場合はLEDを点滅させる。
 *・内臓RAMがあれば診断を行い、不正の場合はLEDを点滅させる。
 *・初期診断終了後、｢reset_entry｣に分岐する。
 */
#define	_in_asm_source_

#include <machine.h>		/* 機種名、他 */
///#include <tk/asm.h>		/* see <tk/sysdepend/app_sh7760/sysdef_depend.h> */
				/* see <tk/sysdepend/app_sh7760/asm_depend.h> */
///#include <sys/sysinfo.h>	/* see <sys/sysdepend/app_sh7760/sysinfo_depend.h> */
///#include <sh4/sh7760io.h>	/* IOﾚｼﾞｽﾀ定義 */




///#include "setup_sh7760.h"	/* 基板固有定数 */


	.globl	reset_entry


/************************************************
 *    initial diagnosis routine code (ｷｬｯｼｭ有効, BL=1)
 ************************************************/
	.text
	.balign	4
	.globl	init_diag_start
	.type	init_diag_start, %function
init_diag_start:

//-------( 例外コードの確認 )-------







	mov.l	L_reset_entry, r7	// 初期診断処理へ
	jmp	@r7			//(遅延分岐)
	nop

			.balign 4
L_reset_entry:		.long	reset_entry


/*----------------------------------------------------------------------*/
#if 0
#|【initdiag.S 変更履歴】
#|□2015/11/28	 [app_sh7760]用に新規作成
#|  ・ IOPモニタの"BOOT.SRC"を参考に作成｡
#|
#endif







#if 0	/////////////////////  reset.S original //////////////////////////

/////#include <tk/sysdef.h>
#include <sys/sysinfo.h>

#include "setup_em1d512.h"

/*
 * macro for setting up registers
 */
.macro out_w reg, val
  .ifnes "\val", ""	// when val is empty, do nothing.
	ldr	r0, =\reg
	ldr	r1, =\val
	str	r1, [r0]
  .endif
.endm

.macro setup_param	// r0: address of parameter string.
0:			// * r0, r2, and r3 are going to be clobbered.
	ldmia	r0!, {r2, r3}
	cmp	r2, #0
	strne	r3, [r2]
	bne	0b
.endm

.macro	wait_nsec	// r0: wait time (nsec)
			// * Assume one step is 4 ns @ (500MHz)
	lsr	r0, r0, #2
0:
	subs	r0, r0, #1
	bne	0b
.endm

/*
 * memory barrier macros
 */
.macro _mov reg, val
  .ifnes "\reg", "\val"
	mov	\reg, \val
  .endif
.endm
.macro .ISB reg, val=#0
	_mov	\reg, \val
	mcr	p15, 0, \reg, cr7, c5, 4
.endm
.macro .DSB reg, val=#0
	_mov	\reg, \val
	mcr	p15, 0, \reg, cr7, c10, 4
.endm
.macro .DMB reg, val=#0
	_mov	\reg, \val
	mcr	p15, 0, \reg, cr7, c10, 5
.endm

//>>>

/*----------------------------------------------------------------------
        T-Monitor boot processing
----------------------------------------------------------------------*/
	.section .startup, "ax"
	.balign	4
	.globl	startup_entry
	.type	startup_entry, %function
	.org	0x00000000
startup_entry:
// SVC mode, FIQ/IRQ interrupt disabled
	mov	r0, #(PSR_SVC | PSR_I | PSR_F)
	msr	cpsr_fsxc, r0

// use On-Chip SRAM as stack area
	ldr	sp, =0xa0020000

// not in effect: MMU, cache (D/I), program-flow prediction, High-Vector, VIC
// in effect: Force AP, TEX remap, Subpage AP
	.DSB	r0
	mrc	p15, 0, r0, cr1, cr0, 0
	ldr	r1, =~0x01003f85
	and	r0, r0, r1
	ldr	r1, =0x30800000
	orr	r0, r0, r1
	mcr	p15, 0, r0, cr1, cr0, 0

// Setup clock divider
	mov	r0, #0
	ldr	r2, =CHG_L1_HOLD
	str	r0, [r2]		// release data hold when L1 is off
	mov	r0, #0x30000000
	ldr	r2, =AUTO_FRQ_CHANGE
	str	r0, [r2]		// automatic frequency change function is off

setup_clock_divider:
	adr	r0, param_table0
	setup_param

// Setup PLL1 (PLL3 is operating)
setup_pll1:
        // We assume Power ON mode: In other mode setting, we simply take it for granted that PLL has been configured already
	ldr	r2, =CLK_MODE_SEL
	ldr	r0, [r2]
	ands	r0, r0, #0x00000f00
	bne	setup_power_mode

	mov	r0, #0x79		// (default) PLL1=499.712MHz
	ldr	r2, =PLL1CTRL0
	str	r0, [r2]
	mov	r0, #0
	ldr	r2, =PLL1CTRL1
	str	r0, [r2]		// PLL starts to operate
	ldr	r2, =PLL_STATUS
wait_pll1:
	ldr	r0, [r2]		// Wait for PLL1 operation completion
	ands	r0, r0, #0x00000001
	beq	wait_pll1

// Setup power mode
setup_power_mode:
        // Transition from Power ON to Normal Mode A
	mov	r0, #1
	ldr	r2, =CLK_MODE_SEL
	str	r0, [r2]
wait_power_mode_change:
	ldr	r0, [r2]
	and	r0, r0, #0x00000f00
	cmp	r0, #0x00000100
	bne	wait_power_mode_change

// Setup PLL2 (needs to be configured in Normal Mode)
setup_pll2:
	mov	r0, #0xff		// PLL2 ceases to operate
	ldr	r2, =PLL2CTRL1
	str	r0, [r2]
	ldr	r2, =PLL_STATUS
wait_pll2_0:	
	ldr	r0, [r2]		// Wait for PLL1 operation
	ands	r0, r0, #0x00000100
	bne	wait_pll2_0

	mov	r0, #0x61		// PLL2=401.408MHz
	ldr	r2, =PLL2CTRL0
	str	r0, [r2]
	mov	r0, #0			// PLL2 starts to operate
	ldr	r2, =PLL2CTRL1
	str	r0, [r2]
	ldr	r2, =PLL_STATUS
wait_pll2_1:	
	ldr	r0, [r2]		// wait for PLL2 to stop operation.
	ands	r0, r0, #0x00000100
	beq	wait_pll2_1

// Setup pin multiplexer
setup_pin_mux:
	mov	r1, #0xff000000		// since 'adr' cannot be used, we manually make sure
	ldr	r0, =Csym(GPIOConfig)	// that the code is relocatable at 16MB units boundary.
	bic	r0, r0, r1
	and	r1, pc, r1
	orr	r0, r0, r1
	setup_param
	
// release reset of the internal modules
setup_module:
	adr	r0, param_table1
	setup_param
	ldr	r0, =100000
	wait_nsec

// supplying clock to modules.
setup_clock_distribution:
	adr	r0, param_table2
	setup_param

// Setup Bus controller
setup_bcr:
	adr	r0, param_table3
	setup_param

// initialization of DDR memory
	bl	setup_ddr

// creation of temporary page table
	ldr	r0, =PAGETBL_BASE
	ldr	r1, =0x00000000
	ldr	r2, =0x00000402	// Kernel/RW, Strongly-order
tmptable_loop:
	orr	r3, r1, r2
	str	r3, [r0], #4
	adds	r1, r1, #0x00100000
	bne	tmptable_loop

// Mapping of FlashROM area (0x70000000 - 0x72000000 --> 0x00000000 -)
	ldr	r0, =(PAGETBL_BASE + (0x700 << 2))
	ldr	r1, =0x00000000
	ldr	r2, =0x0000940e	// Kernel/RO, Normal WB/WA
flashtable_loop:
	orr	r3, r1, r2
	str	r3, [r0], #4
	adds	r1, r1, #0x00100000
	cmp	r1, #0x02000000
	bne	flashtable_loop
	
// initialization of CP15
	ldr	r0, =0x00000004
	mcr	p15, 0, r0, cr2, cr0, 2		// TTBCR
	ldr	r0, =(PAGETBL_BASE + 0x09)	// WB/WA, no-shared, cachable
	mcr	p15, 0, r0, cr2, cr0, 1		// TTBR1
	mcr	p15, 0, r0, cr2, cr0, 0		// TTBR0
	ldr	r0, =EITENT_BASE
	mcr	p15, 0, r0, cr12, cr0, 0	// VBAR
	ldr	r0, =0x000a8aa4
	mcr	p15, 0, r0, cr10, cr2, 0 	// PRRR
	ldr	r0, =0x44e048e0
	mcr	p15, 0, r0, cr10, cr2, 1	// NMRR
	ldr	r0, =0x55555555			// All client
	mcr	p15, 0, r0, cr3, cr0, 0		// Domain access

// MMU enable
	.DSB	r0
	mcr	p15, 0, r0, cr8, cr7, 0		// I/D TLB invalidate
	mcr	p15, 0, r0, cr7, cr5, 6		// invalidate BTC
	.DSB	r0
	.ISB	r0
	mrc	p15, 0, r0, cr1, cr0, 0
	orr	r0, r0, #0x00000001
	mcr	p15, 0, r0, cr1, cr0, 0
	.ISB	r0

// perform reset processing
	ldr	pc, =reset_entry

// initialization of DDR memory
setup_ddr:
	ldr	r0, =MEMCCLK270_SEL
	ldr	r1, =0x00000001		// MEMCCLK270 no phase delay
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGT1
	ldr	r1, =0x00000006		// start auto-calibration
	str	r1, [r0]
calibrate_loop:
	ldr	r1, [r0]
	ands	r1, r1, #0x00000002	// wait for complete
	beq	calibrate_loop

	ldr	r0, =MEMC_DDR_CONFIGT3
	ldr	r1, [r0]		// get calibration result
	ldr	r0, =MEMC_DDR_CONFIGT2
	str	r1, [r0]		// apply calibrated value

	ldr	r0, =MEMCCLK270_SEL
	ldr	r1, =0x00000000		// MEMCCLK270 270degree delay
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGT1
	ldr	r1, =0x000d0803
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGF
	ldr	r1, =0x00000015
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGA1
	ldr	r1, =0x53443203
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGA2
	ldr	r1, =0x28da1042
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGC2
	ldr	r1, =0x0000001d
	str	r1, [r0]

	ldr	r0, =200000
	wait_nsec

	ldr	r0, =MEMC_DDR_CONFIGC1
	ldr	r1, =0x80200033
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGC2
	ldr	r1, =0x00000018		// CS0: memory initialize sequence
	str	r1, [r0]
ddr_init_loop:
	ldr	r1, [r0]
	ands	r1, r1, #0x00000100
	beq	ddr_init_loop

	ldr	r0, =MEMC_REQSCH
	ldr	r1, =0x0000001f		// memory request schedule
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGC2
	ldr	r1, =0x00000090		// CS0: CMD_REQ release
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGR1
	ldr	r1, =0x00690069		// refresh counter
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGR2
	ldr	r1, =0x3777011f
	str	r1, [r0]

	ldr	r0, =MEMC_DDR_CONFIGR3
	ldr	r1, =0x00001415
	str	r1, [r0]

	bx	lr

	.pool

param_table0:
	.long	NORMALA_DIV		// ACPU =PLL1/1 (499.712MHz)
	.long	0x00244200		// ADSP =PLL1/1 (499.712MHz)
					// HBUS =PLL1/3 (166.571MHz)
					// LBUS =PLL1/6 ( 83.285MHz)
					// FLASH=PLL1/6 ( 83.285MHz)
					// MEMC =PLL1/3 (166.571MHz)
	.long	DIVU70SCLK
	.long	0x00000000		// U70_SCLK=PLL3/1 (229.376MHz)
	.long	DIVU71SCLK
	.long	0x00000000		// U71_SCLK=PLL3/1 (229.376MHz)
	.long	DIVU72SCLK
	.long	0x00000000		// U72_SCLK=PLL3/1 (229.376MHz)
	.long	DIVLCDLCLK
	.long	0x00000004		// LCD_LCLK=PLL2/16 (25.088MHz)
	.long	DIVIICSCLK
	.long	0x00530053		// IIC_SCLK=PLL3/48 (4.779MHz)
	.long	DIVTIMTIN
	.long	0x00000003		// Txx_TIN=PLL3/8 (28.672MHz)
	.long	DIVSP0SCLK
	.long	0x00000074		// SP0_SCLK=PLL3/128 (1.792MHz)
	.long	TI0TIN_SEL
	.long	0x00000000
	.long	TI1TIN_SEL
	.long	0x00000000
	.long	TI2TIN_SEL
	.long	0x00000000
	.long	TI3TIN_SEL
	.long	0x00000000
	.long	TIGnTIN_SEL
	.long	0x00000000

	.long	0x00000000		// (terminate)
	.long	0x00000000

param_table1:
	.long	RESETREQ0ENA
	.long	0xffffffff
	.long	RESETREQ0
	.long	0xffffffe7		// Reset everything, but DSP
	.long	RESETREQ0ENA
	.long	0x00000000
	.long	RESETREQ1ENA
	.long	0xffffffff
	.long	RESETREQ1
	.long	0xffffffff		// Reset everything
	.long	RESETREQ1ENA
	.long	0x00000000
	.long	RESETREQ2ENA
	.long	0xffffffff
	.long	RESETREQ2
	.long	0xffffffff		// Reset everything
	.long	RESETREQ2ENA
	.long	0x00000000
	.long	RESETREQ3ENA
	.long	0xffffffff
	.long	RESETREQ3
	.long	0xffffffff		// Reset everything
	.long	RESETREQ3ENA
	.long	0x00000000

	.long	0x00000000		// (terminate)
	.long	0x00000000

param_table2:
	.long	GCLKCTRL0ENA
	.long	0xffffffff
	.long	GCLKCTRL0
	.long	0xffffffff		// (default) module clock on
	.long	GCLKCTRL0ENA
	.long	0x00000000
	.long	GCLKCTRL1ENA
	.long	0xffffffff
	.long	GCLKCTRL1
	.long	0xffffffff		// (default) module clock on
	.long	GCLKCTRL1ENA
	.long	0x00000000
	.long	GCLKCTRL2ENA
	.long	0xffffffff
	.long	GCLKCTRL2
	.long	0xffffffff		// (default) module clock on
	.long	GCLKCTRL2ENA
	.long	0x00000000
	.long	GCLKCTRL3ENA
	.long	0xffffffff
	.long	GCLKCTRL3
	.long	0xffffffff		// (default) module clock on
	.long	GCLKCTRL3ENA
	.long	0x00000000
	.long	GCLKCTRL4ENA
	.long	0xffffffff
	.long	GCLKCTRL4
	.long	0xffffffff		// (default) module clock on
	.long	GCLKCTRL4ENA
	.long	0x00000000
	.long	AHBCLKCTRL0
	.long	0x00000000		// (default) prohibit automatic control
	.long	AHBCLKCTRL1
	.long	0x00000000		// (default) prohibit automatic control
	.long	APBCLKCTRL0
	.long	0x00000000		// (default) prohibit automatic control
	.long	APBCLKCTRL1
	.long	0x00000000		// (default) prohibit automatic control
	.long	CLKCTRL
	.long	0x00000000		// (default) prohibit automatic control
	.long	CLKCTRL1
	.long	0x00000000

	.long	0x00000000		// (terminate)
	.long	0x00000000

param_table3:
	.long	AB1_U70WAITCTRL
	.long	0x00010200		// recommended value for 83MHz operation
	.long	AB1_U71WAITCTRL
	.long	0x00010200		// recommended value for 83MHz operation
	.long	AB1_U72WAITCTRL
	.long	0x00010200		// recommended value for 83MHz operation
	.long	AB1_IIC2WAITCTRL
	.long	0x00010300		// recommended value for 83MHz operation
	.long	AB1_IICWAITCTRL
	.long	0x00010300		// recommended value for 83MHz operation
	.long	AB1_SDIAWAITCTRL
	.long	0x00010300
	.long	AB1_SDIBWAITCTRL
	.long	0x00010300
	.long	AB1_SDICWAITCTRL
	.long	0x00010300
	.long	AB1_U70READCTRL
	.long	0x00000000		// (default)
	.long	AB1_U71READCTRL
	.long	0x00000000		// (default)
	.long	AB1_U72READCTRL
	.long	0x00000000		// (default)
	.long	AB1_IIC2READCTRL
	.long	0x00000000		// (default)
	.long	AB1_IICREADCTRL
	.long	0x00000000		// (default)
	.long	AB1_SDIAREADCTRL
	.long	0x00000000		// (default)
	.long	AB1_SDIBREADCTRL
	.long	0x00000000		// (default)
	.long	AB1_SDICREADCTRL
	.long	0x00000000		// (default)

	// memory map setup (CS0-3)
	//
	// 0x00000000 +----------------------------+
	//            | Bank0/CS0 (not used)       |
	// 0x10000000 +----------------------------+
	//            | Bank1/CS1 (not used)       |
	// 0x20000000 +----------------------------+
	//            | Bank2/CS2 (not used)       |
	// 0x28000000 +----------------------------+
	//            | Bank2/CS3 (LAN controller) |
	// 0x30000000 +----------------------------+
	.long	AB0_CSnBASEADD(0)
	.long	0x00000000
	.long	AB0_CSnBITCOMP(0)
	.long	0xf0000000
	.long	AB0_CSnBASEADD(1)
	.long	0x10000000
	.long	AB0_CSnBITCOMP(1)
	.long	0xf0000000
	.long	AB0_CSnBASEADD(2)
	.long	0x20000000
	.long	AB0_CSnBITCOMP(2)
	.long	0xf8000000
	.long	AB0_CSnBASEADD(3)
	.long	0x28000000
	.long	AB0_CSnBITCOMP(3)
	.long	0xf8000000
	.long	AB0_FLASHCLKCTRL
	.long	0x00000001		// AB0:Flash=1:2

        // set up LAN controller
        // Assuming the use of 83.333 MHz (12ns/1clk), we set the following values.
	//	CSint=1clk (Read+1clk=24ns, Write+2clk=36ns)
	//	T0=0clk (0ns), T1=3clk(36ns), T2=2clk(24ns)
	.long	AB0_CSnWAITCTRL(3)
	.long	0x01020300
	.long	AB0_CSnWAITCTRL_W(3)
	.long	0x00020300
	.long	AB0_CSnREADCTRL(3)
	.long	0x00000000		// (default)
	.long	AB0_CSnWAIT_MASK(3)
	.long	0x00000000		// (default)
	.long	AB0_CSnCONTROL(3)
	.long	0x00010100		// (default)
	.long	AB0_FLASHCOMSET
	.long	0x00000008		// CS3 value above is reflected.
	
	.long	0x00000000		// (terminate)
	.long	0x00000000




#endif	//////////////////////////////////////////





#if 0	/////////////////////  BOOT.SRC  //////////////////////////


; BOOT common program for IOP4/SCPA/TCUA2
;
	.HEADING """BOOT.SRC"" for IOP4/IOP5/SCPA/TCUA2 2011/10/27"

	.INCLUDE "BOOT_DEF.HA"		;REVCODE, REVDATAの定義
	.INCLUDE "TYPE_DEF.HA"
	.INCLUDE "BOOT.HA"
	.INCLUDE "RAMDIAG.MAC"

  .AIFDEF	_SH3			;_SH3 & _SH4=ｺﾝﾊﾟｲﾙ時ｵﾌﾟｼｮﾝ(@MAKEﾌｧｲﾙ)
    .AIF	"_SH3" EQ "1"		;SH7709S(IOP4) ?
	.INCLUDE "SH7709IO.HA"
    .AELIF	"_SH3" EQ "2"		;SH7708(TCUA2) ?
	.INCLUDE "SH7708IO.HA"
    .AELSE
////// "_SH3" illegal //////
    .AENDI

SCI_IOBASE	.EQU	IOBASE_1
SCI_SCSCR	.EQU	SCSCR_1
SCI_SCSSR	.EQU	SCSSR_1
SCI_SCRDR	.EQU	SCRDR_1
SCI_SCTDR	.EQU	SCTDR_1
  .AENDI

  .AIFDEF	_SH4
    .AIF	"_SH4" EQ "1"		;SH7750(SCPA) ?
	.INCLUDE "SH7750IO.HA"
SCI_IOBASE	.EQU	IOBASE_E0	;ﾒﾝﾃﾎﾟｰﾄﾍﾞｰｽ
SCI_SCSCR	.EQU	SCSCR1_E0	;(B):ｼﾘｱﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ
SCI_SCSSR	.EQU	SCSSR1_E0	;(B):ｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀ
SCI_SCRDR	.EQU	SCRDR1_E0	;(B):ﾚｼｰﾌﾞﾃﾞｰﾀﾚｼﾞｽﾀ
SCI_SCTDR	.EQU	SCTDR1_E0	;(B):ﾄﾗﾝｽﾐｯﾄﾃﾞｰﾀﾚｼﾞｽﾀ
	
    .AELIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	.INCLUDE "SH7760IO.HA"
SCI_IOBASE	.EQU	IOBASE_160	;ﾒﾝﾃﾎﾟｰﾄﾍﾞｰｽ(SCIF#0)
SCI_SCSCR	.EQU	SCSCR_X		;(W):ｼﾘｱﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀ
SCI_SCSSR	.EQU	SCFSR_X		;(W):ｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀ
SCI_SCRDR	.EQU	SCFRDR_X	;(B):ﾚｼｰﾌﾞFIFOﾃﾞｰﾀﾚｼﾞｽﾀ
SCI_SCTDR	.EQU	SCFTDR_X	;(B):ﾄﾗﾝｽﾐｯﾄFIFOﾃﾞｰﾀﾚｼﾞｽﾀ
    .AELSE
////// "_SH4" illegal //////
    .AENDI
  .AENDI


	.AIF	\&FMP_USE EQ 1		;*** FMP使用時のみ ***
	.IMPORT	FMP_START_MSG
	.IMPORT	FM_SERASE_CMD, FM_ERASE_CMD, FM_READ_CMD, FM_CHKSUM_CMD
	.IMPORT	FM_WRITE_CMD, FM_BLKCHK_CMD
	.IMPORT	FM_CHECK
	.AENDI

	.IMPORT	$MON_SETUP, $MON_RAMCOPY
	.IMPORT	$MON_ENTRY

	.IMPORT	BOOT_SYSIO_INIT, BOOT_WDT_STOP, BOOT_RS_INIT, BOOT_GET_DIPSW


	.AIF	\&LCA_CFG EQ 2		;*** LCAｺﾝﾌｨｸﾞ BOOTで実行? ***
	.IMPORT	$LCA_CONFIG
	.AENDI




;*******( 定数定義 )*******
RFCR_1MS_COUNT	.EQU	64		;15.6μ×64＝1m秒



;*******( BOOT footer )*******
	.SECTION BOOTF,CODE,ALIGN=4	;*** フッター定義 ***
	.SDATAZ	"(C)ABC."		;8Byte
	.DATA.W	0			;予備
CHKSUM:	.DATA.W	0			;ここにﾁｪｯｸｻﾑが格納される(2byte)
CODE_END:				;←4バイト境界になるようにする事




;************************************************
;    initial test routine code (ｷｬｯｼｭ有効)
;************************************************
	.ALIGN	4
BOOT_TEST_START:

;-------( 例外コードの確認 )-------
	MOV.L	#EXPEVT_ADDR,R0
	MOV.L	@R0,R0			;R0←(EXPEVT)
	CMP/EQ	#H'00,R0		;ﾊﾟﾜｰｵﾝ ?
	BT	CPU_REG_TEST
	BRA	BOOT_TEST_END		;(遅延分岐)ﾊﾟﾜｰｵﾝ時以外はﾚｼﾞｽﾀ診断ｽｷｯﾌﾟ
	NOP

;------------------------------------------------
;    cpu register test
;------------------------------------------------
CPU_REG_TEST:
;	( R0,R1 test )
	MOV	#0,R0			;R0←0
	CMP/EQ	#0,R0			;R0=0 ?
	BF	CPU_TEST_ERR
	NOT	R0,R0			;R0←H'FFFFFFFF
	CMP/EQ	#0,R0			;R0=0 ?
	BT	CPU_TEST_ERR
	CMP/EQ	#-1,R0
	BF	CPU_TEST_ERR

	MOV	R0,R1			;R1←R0(H'FFFFFFFF)
	CMP/EQ	R0,R1
	BF	CPU_TEST_ERR
	NOT	R1,R1			;R1←0
	TST	R1,R1			;R1=0 ?
	BF	CPU_TEST_ERR

	MOV.L	#H'55555555,R0		;R0←H'55555555
	MOV	R0,R1
	CMP/EQ	R0,R1
	BF	CPU_TEST_ERR
	NOT	R0,R1			;R1←H'AAAAAAAA
	CMP/EQ	R0,R1
	BT	CPU_TEST_ERR

;	( R2 test )
	MOV.L	#H'55555555,R2
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	NOT	R2,R2
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( R3 test )
	MOV.L	#H'55555555,R3
	CMP/EQ	R0,R3
	BF	CPU_TEST_ERR
	NOT	R3,R3
	CMP/EQ	R1,R3
	BF	CPU_TEST_ERR

;	( R4 test )
	MOV.L	#H'55555555,R4
	CMP/EQ	R0,R4
	BF	CPU_TEST_ERR
	NOT	R4,R4
	CMP/EQ	R1,R4
	BF	CPU_TEST_ERR

;	( R5 test )
	MOV.L	#H'55555555,R5
	CMP/EQ	R0,R5
	BF	CPU_TEST_ERR
	NOT	R5,R5
	CMP/EQ	R1,R5
	BF	CPU_TEST_ERR

;	( R6 test )
	MOV.L	#H'55555555,R6
	CMP/EQ	R0,R6
	BF	CPU_TEST_ERR
	NOT	R6,R6
	CMP/EQ	R1,R6
	BF	CPU_TEST_ERR

;	( R7 test )
	MOV.L	#H'55555555,R7
	CMP/EQ	R0,R7
	BF	CPU_TEST_ERR
	NOT	R7,R7
	CMP/EQ	R1,R7
	BF	CPU_TEST_ERR

;	( R8 test )
	MOV.L	#H'55555555,R8
	CMP/EQ	R0,R8
	BF	CPU_TEST_ERR
	NOT	R8,R8
	CMP/EQ	R1,R8
	BF	CPU_TEST_ERR

;	( R9 test )
	MOV.L	#H'55555555,R9
	CMP/EQ	R0,R9
	BF	CPU_TEST_ERR
	NOT	R9,R9
	CMP/EQ	R1,R9
	BF	CPU_TEST_ERR

;	( R10 test )
	MOV.L	#H'55555555,R10
	CMP/EQ	R0,R10
	BF	CPU_TEST_ERR
	NOT	R10,R10
	CMP/EQ	R1,R10
	BF	CPU_TEST_ERR

;	( R11 test )
	MOV.L	#H'55555555,R11
	CMP/EQ	R0,R11
	BF	CPU_TEST_ERR
	NOT	R11,R11
	CMP/EQ	R1,R11
	BF	CPU_TEST_ERR

;	( R12 test )
	MOV.L	#H'55555555,R12
	CMP/EQ	R0,R12
	BF	CPU_TEST_ERR
	NOT	R12,R12
	CMP/EQ	R1,R12
	BF	CPU_TEST_ERR

;	( R13 test )
	MOV.L	#H'55555555,R13
	CMP/EQ	R0,R13
	BF	CPU_TEST_ERR
	NOT	R13,R13
	CMP/EQ	R1,R13
	BF	CPU_TEST_ERR

;	( R14 test )
	MOV.L	#H'55555555,R14
	CMP/EQ	R0,R14
	BF	CPU_TEST_ERR
	NOT	R14,R14
	CMP/EQ	R1,R14
	BF	CPU_TEST_ERR

;	( R15 test )
	MOV	R15,R13			;SAVE R15(for SH7708,TCUA2)
	MOV.L	#H'55555555,R15
	CMP/EQ	R0,R15
	BF	CPU_TEST_ERR
	NOT	R15,R15
	CMP/EQ	R1,R15
	BF	CPU_TEST_ERR
	MOV	R13,R15			;RESTORE R15

;	( MACH test )
	LDS	R0,MACH			;MACH←R0
	STS	MACH,R2			;R2←MACH
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDS	R1,MACH			;MACH←R1
	STS	MACH,R2			;R2←MACH
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( MACL test )
	LDS	R0,MACL			;MACL←R0
	STS	MACL,R2			;R2←MACL
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDS	R1,MACL			;MACL←R1
	STS	MACL,R2			;R2←MACL
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( PR test )
	LDS	R0,PR			;PR←R0
	STS	PR,R2			;R2←PR
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDS	R1,PR			;PR←R1
	STS	PR,R2			;R2←PR
	CMP/EQ	R1,R2
	BT	CPU_TEST10

;	( CPU異常検出 )
CPU_TEST_ERR:
	MOV.W	#CPU_ERR_LED,R1
	BRA	FATAL_TRAP_LED		;(遅延分岐)
	NOP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

CPU_TEST10:
;	( GBR test )
	LDC	R0,GBR			;GBR←R0
	STC	GBR,R2			;R2←GBR
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,GBR			;GBR←R1
	STC	GBR,R2			;R2←GBR
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( VBR test )
	LDC	R0,VBR			;VBR←R0
	STC	VBR,R2			;R2←VBR
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,VBR			;VBR←R1
	STC	VBR,R2			;R2←VBR
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( SSR test )
	LDC	R0,SSR			;SSR←R0
	STC	SSR,R2			;R2←SSR
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,SSR			;SSR←R1
	STC	SSR,R2			;R2←SSR
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( SPC test )
	LDC	R0,SPC			;SPC←R0
	STC	SPC,R2			;R2←SPC
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,SPC			;SPC←R1
	STC	SPC,R2			;R2←SPC
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( R0_BANK test )
	LDC	R0,R0_BANK		;R0_BANK←R0
	STC	R0_BANK,R2		;R2←R0_BANK
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,R0_BANK		;R0_BANK←R1
	STC	R0_BANK,R2		;R2←R0_BANK
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( R1_BANK test )
	LDC	R0,R1_BANK		;R1_BANK←R0
	STC	R1_BANK,R2		;R2←R1_BANK
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,R1_BANK		;R1_BANK←R1
	STC	R1_BANK,R2		;R2←R1_BANK
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( R2_BANK test )
	LDC	R0,R2_BANK		;R2_BANK←R0
	STC	R2_BANK,R2		;R2←R2_BANK
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,R2_BANK		;R2_BANK←R1
	STC	R2_BANK,R2		;R2←R2_BANK
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( R3_BANK test )
	LDC	R0,R3_BANK		;R3_BANK←R0
	STC	R3_BANK,R2		;R2←R3_BANK
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,R3_BANK		;R3_BANK←R1
	STC	R3_BANK,R2		;R2←R3_BANK
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( R4_BANK test )
	LDC	R0,R4_BANK		;R4_BANK←R0
	STC	R4_BANK,R2		;R2←R4_BANK
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,R4_BANK		;R4_BANK←R1
	STC	R4_BANK,R2		;R2←R4_BANK
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( R5_BANK test )
	LDC	R0,R5_BANK		;R5_BANK←R0
	STC	R5_BANK,R2		;R2←R5_BANK
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,R5_BANK		;R5_BANK←R1
	STC	R5_BANK,R2		;R2←R5_BANK
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( R6_BANK test )
	LDC	R0,R6_BANK		;R6_BANK←R0
	STC	R6_BANK,R2		;R2←R6_BANK
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,R6_BANK		;R6_BANK←R1
	STC	R6_BANK,R2		;R2←R6_BANK
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR

;	( R7_BANK test )
	LDC	R0,R7_BANK		;R7_BANK←R0
	STC	R7_BANK,R2		;R2←R7_BANK
	CMP/EQ	R0,R2
	BF	CPU_TEST_ERR
	LDC	R1,R7_BANK		;R7_BANK←R1
	STC	R7_BANK,R2		;R2←R7_BANK
	CMP/EQ	R1,R2
	BF	CPU_TEST_ERR
BOOT_TEST_END:

	.PAGE
;------------------------------------------------
;    LCAの初期化(ｺﾝﾌｨｸﾞﾚｰｼｮﾝ) 
;------------------------------------------------
	.AIF	\&LCA_CFG EQ 2		;*** LCAｺﾝﾌｨｸﾞ BOOTで実行? ***
	MOV.L	#$LCA_CONFIG,R0
	JSR	@R0			;(遅延分岐),LCAのｺﾝﾌｨｸﾞﾚｰｼｮﾝ
	NOP
	.AENDI

;------------------------------------------------
;    RS-232Cﾎﾟｰﾄの初期化
;------------------------------------------------

	MOV.L	#BOOT_RS_INIT,R0
	JSR	@R0			;(遅延分岐)
	NOP
	BSR	$MON_WAIT1		;(遅延分岐),1ﾋﾞｯﾄ期間経過待ち
	MOV	#5,R4			;5m秒待ち

;	( 送信,受信許可 )
	MOV.L	#SCI_IOBASE,R0
	LDC	R0,GBR			;GBR←IOBASE address

    .AIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	MOV.W	@(SCI_SCSCR,GBR),R0	;
	OR	#H'30,R0		;送信,受信許可(ﾋﾞｯﾄｾｯﾄ)
	MOV.W	R0,@(SCI_SCSCR,GBR)	;
    .AELSE
	MOV	#SCI_SCSCR,R0
	OR.B	#H'30,@(R0,GBR)		;送信,受信許可
    .AENDI

;-------( 起動メッセージの表示 )-------
	MOV.L	#EXPEVT_ADDR,R0
	MOV.L	@R0,R0			;R0←(EXPEVT)
	CMP/EQ	#H'00,R0		;ﾊﾟﾜｰｵﾝ ?
	BT	POWON_START

	BRA	_WDT_TIMO_ERROR		;(遅延分岐)
	NOP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
BOOT_MSG	.SDATA	<H'0D><H'0A><H'0D><H'0A><H'1A>
		.SDATA	"SH3/4 Boot Loader REVCODE ("
		.SDATA	"_BOARD_"
		.SDATA	") "
		.SDATAZ	<H'0D><H'0A>

	.ALIGN	4
$MON_RSOUT_JMP:
	BRA	$MON_RSOUT		;(遅延分岐)
	NOP

$MON_STROUT_JMP:
	BRA	$MON_STROUT		;(遅延分岐)
	NOP
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	.ALIGN	4
	.PAGE
;*******( 初期診断 )*******
POWON_START:
	STROUT	#BOOT_MSG		;(DOG_FOOD含む)

INIT_TEST:
	STROUT	#INIT_TEST_MSG		;(DOG_FOOD含む)

;------------------------------------------------
;    boot rom check sum test
;------------------------------------------------
	CHAROUT	#"o"			;(DOG_FOOD含む)

	MOV.L	#$RESET_ENTRY,R1	;R1←先頭ｱﾄﾞﾚｽ
	MOV.L	BOOT_SIZE,R2		;R2←ﾊﾞｲﾄｶｳﾝﾀ
	SHLR	R2			;R2←R2/2

	MOV	#0,R3			;SUMｸﾘｱ
BROM_SUM_LOOP:
	MOV.W	@R1+,R0
	EXTU.W	R0,R0
	DT	R2
	BF/S	BROM_SUM_LOOP		;(遅延分岐)
	ADD	R0,R3			;SUM計算

	.AIF	\&BROM_CHK EQ 1
	EXTU.W	R3,R0
	TST	R0,R0			;R0=0 ?
	BF	BROM_TEST_ERR
	.AENDI

	CHAROUT	#"o"			;(DOG_FOOD含む)

	BRA	BROM_TEST_END		;(遅延分岐)
	NOP

;	( ROM異常検出 )
BROM_TEST_ERR:
	STROUT	#BROM_ERR_MSG		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)
	HEXOUT	R3			;ﾁｪｯｸｻﾑ表示
	STROUT	#TEST_ERR_MSG		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)

	MOV.W	#BROM_ERR_LED,R1
	BRA	FATAL_TRAP_LED		;(遅延分岐)
	NOP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
BOOT_SIZE	.DATA.L	CODE_END - $RESET_ENTRY
BROM_ERR_MSG	.SDATAZ	<H'0D><H'0A>"BOOT-ROM Sum="
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	.ALIGN	4
BROM_TEST_END:


;------------------------------------------------
;    internal-ram register test
;------------------------------------------------
	.AIF	\&IRAM_USE EQ 1		;内蔵RAM使用時のみﾒﾓﾘﾃｽﾄする
	.AIFDEF	_SH4			;SH4バグ回避で内蔵RAMをｷｬｯｼｭで使用の為
	MOV.L	#IRAM_TOP,R1		;R1←先頭番地
	MOV.L	#IRAM_CNT,R2
	SHLR2	R2			;R2←R2/32(ﾙｰﾌﾟ回数)
	SHLR2	R2
	SHLR	R2

?0001:	PREF	@R1			;ｷｬｯｼｭﾌｨﾙ
	ADD	#32,R1
	DT	R2
	BF	?0001
	.AENDI

	RAMDIAG	IRAM_TOP,IRAM_CNT,IRAM_TEST_ERR,#"i"

	BRA	IRAM_TEST_END		;(遅延分岐)
	NOP

;-------( IRAM異常検出 )-------
IRAM_TEST_ERR:				;R0…異常検出値
					;R1…アドレス
					;R3…期待値
	MOV	R1,R5			;アドレス待避
	MOV	R0,R6			;異常検出値待避
	MOV	R3,R7			;期待値待避

	STROUT	#IRAM_ERR_MSG		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)
	HEXOUT	R5			;アドレス表示
	STROUT	#DRAM_ERR_MSG1		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)
	HEXOUT	R6			;データ表示
	CHAROUT	#"("			;(DOG_FOOD含む)
	HEXOUT	R7			;期待値表示
	CHAROUT	#")"			;(DOG_FOOD含む)

	STROUT	#TEST_ERR_MSG		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)

	MOV.W	#IRAM_ERR_LED,R1
	BRA	FATAL_TRAP_LED		;(遅延分岐)
	NOP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
IRAM_ERR_MSG	.SDATAZ	<H'0D><H'0A>"IRAM AD="
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	.ALIGN	4
IRAM_TEST_END:
	.AENDI


;------------------------------------------------
;    data-ram read/write test
;------------------------------------------------
;## IOP5試作仮 "RAMDIAG" ｴﾗｰ時無視で "DRAM_TEST_END" へ(DEBUG#696)
;	RAMDIAG	DRAM_TOP,DRAM_CNT/2,DRAM_TEST_XXX,#"r"
;DRAM_TEST_XXX:	BRA	DRAM_TEST_END		;(遅延分岐)
;	NOP
;## IOP5試作仮 終わり(DEBUG#969)

	RAMDIAG	DRAM_TOP,DRAM_CNT/2,DRAM_TEST_ERR_JMP,#"r"
	BRA	DRAM_TEST_HALF		;(遅延分岐)
	NOP
DRAM_TEST_ERR_JMP:
	BRA	DRAM_TEST_ERR		;(遅延分岐)
	NOP

DRAM_TEST_HALF:
	RAMDIAG	DRAM_TOP+(DRAM_CNT/2),DRAM_CNT/2,DRAM_TEST_ERR,#"r"
	BRA	DRAM_TEST_END		;(遅延分岐)
	NOP

;-------( DRAM異常検出 )-------
DRAM_TEST_ERR:				;R0…異常検出値
					;R1…アドレス
					;R3…期待値
	MOV	R1,R8			;アドレス待避
	MOV	R0,R9			;異常検出値待避
	MOV	R3,R10			;期待値待避

	STROUT	#DRAM_ERR_MSG		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)
	HEXOUT	R8			;アドレス表示
	STROUT	#DRAM_ERR_MSG1		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)
	HEXOUT	R9			;データ表示
	CHAROUT	#"("			;(DOG_FOOD含む)
	HEXOUT	R10			;期待値表示
	CHAROUT	#")"			;(DOG_FOOD含む)

	STROUT	#TEST_ERR_MSG		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)

	MOV.W	#DRAM_ERR_LED,R1
	BRA	FATAL_TRAP_LED		;(遅延分岐)
	NOP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
DRAM_ERR_MSG	.SDATAZ	<H'0D><H'0A>"DRAM AD="
DRAM_ERR_MSG1	.SDATAZ	",DT="
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	.ALIGN	4
DRAM_TEST_END:

;------------------------------------------------
;　　FMSUBコードをＲＡＭに複写
;------------------------------------------------
FMSUB_RAMCOPY:
;##		BRA	FROM_TEST			;>>> FROM_TESTの処理へ(DEBUG#748)!!!
;##		NOP                     ;                     (DEBUG#748)
	CHAROUT	#"c"			;(DOG_FOOD含む)

;-------( FMSUB領域のコピー )------
	MOV.L	ROM_TOP,R1
	MOV.L	RAM_BGN,R2
	MOV.L	RAM_SIZ,R3
?0010:	MOV.B	@R1+,R0
	MOV.B	R0,@R2
	DT	R3			;R3-1
	BF/S	?0010			;(遅延分岐)
	ADD	#1,R2

;-------( MON領域の比較 )------
	MOV.L	ROM_TOP,R1
	MOV.L	RAM_BGN,R2
	MOV.L	RAM_SIZ,R3
?0110:	MOV.B	@R1+,R4
	MOV.B	@R2,R0
	CMP/EQ	R0,R4
	BF	RAMCOPY_ERR
	DT	R3			;R3-1
	.AIFDEF	_SH4
	OCBP	@R2			;ｷｬｯｼｭの吐き出し
	.AENDI
	BF/S	?0110			;(遅延分岐)
	ADD	#1,R2

	BRA	FROM_TEST		;(遅延分岐)
	NOP


;======( 異常検出 )======
RAMCOPY_ERR:
	MOV	R2,R8			;アドレス待避
	MOV	R0,R9			;異常検出値待避
	MOV	R4,R10			;期待値待避

	STROUT	#DRAM_ERR_MSG		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)
	HEXOUT	R8			;アドレス表示
	STROUT	#DRAM_ERR_MSG1		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)
	HEXOUT	R9			;データ表示
	CHAROUT	#"("			;(DOG_FOOD含む)
	HEXOUT	R10			;期待値表示
	CHAROUT	#")"			;(DOG_FOOD含む)

	STROUT	#RAMCOPY_ERR_MSG	;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)

;///	BRA	CMD_LOOP_JMP		;////////for debug
;///	NOP				;////////for debug
	MOV.W	#RAMCOPY_ERR_LED,R1	;LED表示
	BRA	FATAL_TRAP_LED		;(遅延分岐)
	NOP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
	.ALIGN	4
ROM_TOP	.DATA.L	(STARTOF FMSUB)		;ｾｸｼｮﾝFMSUBの先頭ｱﾄﾞﾚｽ(MAKEﾌｧｲﾙ定義)
RAM_BGN	.DATA.L	(STARTOF FMSUB_R)	;ｾｸｼｮﾝFMSUB_Rの先頭ｱﾄﾞﾚｽ(MAKEﾌｧｲﾙ定義)
RAM_SIZ	.DATA.L	(SIZEOF FMSUB_R)	;ｾｸｼｮﾝFMSUB_Rのﾊﾞｲﾄｻｲｽﾞ

RAMCOPY_ERR_MSG	.SDATAZ	"...FMSUB RAM-Copy ***Error***"<H'0D><H'0A>
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


;------------------------------------------------
;　　フラッシュＲＯＭテスト（ブートモード判定）
;　　＜__sysid情報(R13)の#31,#30ビットセット＞
;------------------------------------------------
	.ALIGN	4
FROM_TEST:
	CHAROUT	#"f"			;(DOG_FOOD含む)
	MOV	#0,R13			;R13←__sysid情報クリア
	
	MOV.L	#BROM_TOP,R14	;CS0ｴﾘｱ先頭ｱﾄﾞﾚｽ
	MOV	R14,R4			;R14←FROM先頭ｱﾄﾞﾚｽ
	MOV.L	#FM_CHECK,R0	;ﾌﾗｯｼｭﾒﾓﾘ存在ﾁｪｯｸ
;##		HEXOUT R0				;                    (DEBUG#806)
;##		BRA	FROM_TEST_END		;>>> test_endの処理へ(DEBUG#806)!!!
;##		NOP                     ;                    (DEBUG#806)
	JSR	@R0			;(遅延分岐)
	NOP
	
    .AIF	"_SH4" NE "2"		;SH7760(IOP5)でない ?----------
	CMP/EQ	#0,R0			;CS0-FROM正常?
	BT	FROM_TEST_END		;>>> 通常ﾓｰﾄﾞの処理へ
	
    .AELSE						;IOP5の処理--------------------
	CMP/EQ	#0,R0			;CS-0FROM正常?
	BF	?0030				;>>> F-ROMでない→次の処理へ
	MOV.L	#IOBASE_80,R0
	LDC	R0,GBR			;GBR←IOBASE_80 address
	MOV.W	@(BCR2_80,GBR),R0	;BCR2(MD4)読み込み
	MOV.W	#H'0008000,R1
	TST		R1,R0			;MD4=H(CS0=16BIT)?
	BF	FROM_TEST_END		;>>> 通常ﾓｰﾄﾞの処理へ
?0030:	NOP
    .AENDI						;-------------------------------
    
	CHAROUT	#"F"			;(DOG_FOOD含む)

	MOV.L	#FMEM_TOP,R14	;CS5ｴﾘｱ(ROMｺﾈｸﾀ)先頭ｱﾄﾞﾚｽ
	MOV	R14,R4			;R14←FROM先頭ｱﾄﾞﾚｽ
	MOV.L	#FM_CHECK,R0	;ﾌﾗｯｼｭﾒﾓﾘ存在ﾁｪｯｸ
	JSR	@R0			;(遅延分岐)
	NOP
	CMP/EQ	#0,R0			;正常?
	BT	FROM_TEST_END1		;>>> BOOTﾓｰﾄﾞの処理へ

	                        ;##	DEBUG_LED_LOOP	;<<<【##デバッグ用 LED_LOOP】
;	( FROM異常検出 )
FROM_TEST_ERR:
	MOV	R14,R8
	MOV	R2,R9

	STROUT	#FROM_ERR_MSG		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)
	HEXOUT	R8
	CHAROUT	#","			;(DOG_FOOD含む)
	HEXOUT	R9,4
	STROUT	#TEST_ERR_MSG		;異常ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)

	MOV.W	#FROM_ERR_LED,R1
	BRA	FATAL_TRAP_LED		;(遅延分岐)
	NOP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
FROM_ERR_MSG	.SDATAZ	<H'0D><H'0A>"FROM AD="
TEST_ERR_MSG	.SDATAZ	" ***Error***"<H'0D><H'0A>
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	.ALIGN	4
FROM_TEST_END1:				;*** BOOTモード時 ***
	ROTCL	R13			;T←MSB(R13)
	SETT				;T←1, __sysid情報(bit31:BOOTﾓｰﾄﾞ)
	ROTCR	R13			;T→MSB(R13),

FROM_TEST_END:
	ROTCL	R13			;T←MSB(R13)
	ROTCL	R13			;T←MSB(R13)
	SETT				;T←1, __sysid情報(bit30:FLASH検出)
	ROTCR	R13			;T→MSB(R13),
	ROTCR	R13			;T→MSB(R13),


;------------------------------------------------
;　　SYSIDW情報の獲得
;------------------------------------------------
SYSID_SET:
	CHAROUT	#"s"			;(DOG_FOOD含む)

	BSR	BOOT_GET_DIPSW		;(遅延分岐), R0(0～7)←DIPSW情報
	NOP
	ADD	R0,R13			;R13(0～7)←DIPSW情報

	MOV	#_TYPE_,R0
	SHLL8	R0
	ADD	R0,R13			;R13(bit8～15)←基板識別番号


	.PAGE
;------------------------------------------------
;    初期化完了
;------------------------------------------------
INIT_TEST_END:				;R13…__sysid情報
					;R14…ﾌﾗｯｼｭ先頭ｱﾄﾞﾚｽ
	STROUT	#TEST_OK_MSG		;初期化完了表示,(DOG_FOOD含む)
	HEXOUT	R13			;SYSID情報表示
	CROUT				;改行表示

	LED_GBR_SET			;GBR←LED port GBR
	LED_ALL_OFF			;赤&緑LED消灯
;## 	BRA	T_CMD		;/////(FOR DEBUG#904)
;## 	NOP				;/////(FOR DEBUG#904)


;=======( モニタプログラムへの分岐判定 )=======
	MOV	R13,R0
	EXTU.B	R0,R0
	MOV.W	#H'00F0,R1
	CMP/EQ	R0,R1			;DIPSW=H'F0 ?
	BT	FMP_START_JMP

;	( MONコードが有効かチェック )
MON_TEST_JMP:
	MOV.L	#$MON_ENTRY,R1		;R1←ｱﾄﾞﾚｽ
	MOV.L	#MON_CNT/2,R2		;R2←ｶｳﾝﾀ

;	( check FDSK_ID )
	MOV.B	@(4,R1),R0		;"FDSK" ?
	CMP/EQ	#"F",R0
	BF	FMP_START_JMP
	MOV.B	@(5,R1),R0
	CMP/EQ	#"D",R0
	BF	FMP_START_JMP
	MOV.B	@(6,R1),R0
	CMP/EQ	#"S",R0
	BF	FMP_START_JMP
	MOV.B	@(7,R1),R0
	CMP/EQ	#"K",R0
	BF	FMP_START_JMP

;	( check SUM )
	CMP/PZ	R13			;R13≧0? (通常ﾓｰﾄﾞ?)
	BF	MON_START_JMP		;BOOTﾓｰﾄﾞ時は、ﾁｪｯｸｻﾑ確認せずに分岐

	MOV	#0,R3			;SUMｸﾘｱ
MON_SUM_LOOP:
	MOV.W	@R1+,R0
	EXTU.W	R0,R0
	DT	R2
	BF/S	MON_SUM_LOOP		;(遅延分岐)
	ADD	R0,R3			;SUM計算

	EXTU.W	R3,R0
	TST	R0,R0			;R0=0 ?
	BF	FMP_START_JMP

;	( モニタへ分岐 )
MON_START_JMP:				;R13…__sysid情報
					;R14…ﾌﾗｯｼｭ先頭ｱﾄﾞﾚｽ
	MOV.L	#$MON_ENTRY,R0
	JMP	@R0			;(遅延分岐)
	NOP


;-------( ＦＭＰへ分岐 )-------
FMP_START_JMP:				;R13…__sysid情報
					;R14…ﾌﾗｯｼｭ先頭ｱﾄﾞﾚｽ
	MOV.L	#FMP_START,R0
	JMP	@R0			;(遅延分岐)
	NOP



;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
INIT_TEST_MSG:	.SDATAZ	"Initial Test..."
TEST_OK_MSG	.SDATAZ	"...OK, SYSID="
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



	.ALIGN	4
	.PAGE
;------------------------------------------------
;○_WDT_TIMO_ERROR　　　《WDT TIMEOUT エラー処理》
;　［入力］なし
;　［出力］なし
;  ［Note］・
;------------------------------------------------
_WDT_TIMO_ERROR:
;///	STS	PR,R7			;save PR	
;///	STC	GBR,R6			;save GBR
	STROUT	#WDT_TIMO_MSG		;ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)

	BRA	FATAL_TRAP00		;(遅延分岐)
	NOP


	.ALIGN	4
	.PAGE
;================================================
;○$MON_BIOS_ERROR　　　《BIOS CALL エラー処理》
;　［入力］なし
;　［出力］なし
;  ［Note］・
;================================================
_MON_BIOS_ERROR:
	STS	PR,R7			;save PR
	STC	GBR,R6			;save GBR
	STROUT	#BIOS_ERROR_MSG		;ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)

	BRA	FATAL_TRAP00		;(遅延分岐)
	NOP


	.ALIGN	4
;================================================
;○$MON_FATAL_TRAP　　　《致命的エラー処理》
;　［入力］なし
;　［出力］なし
;  ［Note］・
;================================================
_FATAL_TRAP:
	STS	PR,R7			;save PR
	STC	GBR,R6			;save GBR

	STROUT	#FATAL_MSG		;ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)

;-------( 例外レジスタの表示 )------
FATAL_TRAP00:
	CROUT				;改行表示
	BSR	BOOT_WDT_STOP		;(遅延分岐),特にSH4(SPCA)でWDT停止必要
	NOP

	STROUT	#EXPEVT_MSG		;(DOG_FOOD含む)
	MOV.L	#EXPEVT_ADDR,R0
	MOV.L	@R0,R0			;get EXPEVT
	HEXOUT	R0			;データ表示

	STROUT	#INTEVT_MSG		;(DOG_FOOD含む)
	MOV.L	#INTEVT_ADDR,R0		;※SH-3ではINTEVT2を表示
	MOV.L	@R0,R0			;get INTEVT
	HEXOUT	R0			;データ表示

;//	STROUT	#INTEVT2_MSG		;(DOG_FOOD含む)
;//	MOV.L	#IOBASE_10+INTEVT2_10,R0
;//	MOV.L	@R0,R0			;get INTEVT2
;//	HEXOUT	R0			;データ表示

	STROUT	#TRA_MSG		;(DOG_FOOD含む)
	MOV.L	#TRA_ADDR,R0
	MOV.L	@R0,R0			;get TRA
	HEXOUT	R0			;データ表示

;-------( システムレジスタの表示 )------
	STROUT	#PC_MSG			;(DOG_FOOD含む)
	STC	SPC,R0			;get PC
	HEXOUT	R0			;データ表示

	STROUT	#SR_MSG			;(DOG_FOOD含む)
	STC	SSR,R0			;get SR
	HEXOUT	R0			;データ表示

	STROUT	#PR_MSG			;(DOG_FOOD含む)
	HEXOUT	R7			;PRデータ表示

	STROUT	#GBR_MSG		;(DOG_FOOD含む)
	HEXOUT	R6			;GBRデータ表示

	STROUT	#VBR_MSG		;(DOG_FOOD含む)
	STC	VBR,R0			;get VBR
	HEXOUT	R0			;データ表示

	STROUT	#MACH_MSG		;(DOG_FOOD含む)
	STS	MACH,R0			;get MACH
	HEXOUT	R0			;データ表示

	STROUT	#MACL_MSG		;(DOG_FOOD含む)
	STS	MACL,R0			;get MACL
	HEXOUT	R0			;データ表示

;-------( 汎用レジスタの表示 )------
	STROUT	#R0_MSG			;(DOG_FOOD含む)
	STC	R0_BANK,R0		;get R0
	HEXOUT	R0			;データ表示

	STROUT	#R1_MSG			;(DOG_FOOD含む)
	STC	R1_BANK,R0		;get R1
	HEXOUT	R0			;データ表示

	STROUT	#R2_MSG			;(DOG_FOOD含む)
	STC	R2_BANK,R0		;get R2
	HEXOUT	R0			;データ表示

	STROUT	#R3_MSG			;(DOG_FOOD含む)
	STC	R3_BANK,R0		;get R3
	HEXOUT	R0			;データ表示

	STROUT	#R4_MSG			;(DOG_FOOD含む)
	STC	R4_BANK,R0		;get R4
	HEXOUT	R0			;データ表示

	STROUT	#R5_MSG			;(DOG_FOOD含む)
	STC	R5_BANK,R0		;get R5
	HEXOUT	R0			;データ表示

	STROUT	#R6_MSG			;(DOG_FOOD含む)
	STC	R6_BANK,R0		;get R6
	HEXOUT	R0			;データ表示

	STROUT	#R7_MSG			;(DOG_FOOD含む)
	STC	R7_BANK,R0		;get R7
	HEXOUT	R0			;データ表示

	STROUT	#R8_MSG			;(DOG_FOOD含む)
	HEXOUT	R8			;R8データ表示

	STROUT	#R9_MSG			;(DOG_FOOD含む)
	HEXOUT	R9			;R8データ表示

	STROUT	#R10_MSG		;(DOG_FOOD含む)
	HEXOUT	R10			;R8データ表示

	STROUT	#R11_MSG		;(DOG_FOOD含む)
	HEXOUT	R11			;R8データ表示

	STROUT	#R12_MSG		;(DOG_FOOD含む)
	HEXOUT	R12			;R8データ表示

	STROUT	#R13_MSG		;(DOG_FOOD含む)
	HEXOUT	R13			;R8データ表示

	STROUT	#R14_MSG		;(DOG_FOOD含む)
	HEXOUT	R14			;R8データ表示

	STROUT	#R15_MSG		;(DOG_FOOD含む)
	HEXOUT	R15			;R8データ表示

	STROUT	#FATAL_MSG1,#"$"	;(DOG_FOOD含む)

	MOV.W	#FATAL_ERR_LED,R1	;R1←点滅ﾊﾟﾀｰﾝ
	BRA	FATAL_TRAP_LED		;(遅延分岐)
	NOP

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
WDT_TIMO_MSG	.SDATAZ	<H'0D><H'0A>"***** WDT TIMEOUT Error *****"
BIOS_ERROR_MSG	.SDATAZ	<H'0D><H'0A>"***** BIOS CALL Error *****"
FATAL_MSG	.SDATAZ	<H'0D><H'0A>"***** Fatal TRAP detect *****"
FATAL_MSG1	.SDATA	<H'0D><H'0A>"*****************************"<H'0D><H'0A>
		.SDATAZ	"Hit '@' key!! Jump to command prompt.$"
EXPEVT_MSG	.SDATAZ	"EXPEVT="
INTEVT_MSG	.SDATAZ	" INTEVT="
INTEVT2_MSG	.SDATAZ	" INTEVT2="
TRA_MSG		.SDATAZ	" TRA="
PC_MSG		.SDATAZ	<H'0D><H'0A>"PC="
SR_MSG		.SDATAZ	" SR="
PR_MSG		.SDATAZ	" PR="
GBR_MSG		.SDATAZ	" GBR="
VBR_MSG		.SDATAZ	" VBR="
MACH_MSG	.SDATAZ	<H'0D><H'0A>"MACH="
MACL_MSG	.SDATAZ	" MACL="
R0_MSG		.SDATAZ	<H'0D><H'0A>"R0 ="
R1_MSG		.SDATAZ	" R1 ="
R2_MSG		.SDATAZ	" R2 ="
R3_MSG		.SDATAZ	" R3 ="
R4_MSG		.SDATAZ	<H'0D><H'0A>"R4 ="
R5_MSG		.SDATAZ	" R5 ="
R6_MSG		.SDATAZ	" R6 ="
R7_MSG		.SDATAZ	" R7 ="
R8_MSG		.SDATAZ	<H'0D><H'0A>"R8 ="
R9_MSG		.SDATAZ	" R9 ="
R10_MSG		.SDATAZ	" R10="
R11_MSG		.SDATAZ	" R11="
R12_MSG		.SDATAZ	<H'0D><H'0A>"R12="
R13_MSG		.SDATAZ	" R13="
R14_MSG		.SDATAZ	" R14="
R15_MSG		.SDATAZ	" R15="
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	.ALIGN	4
	.PAGE
;*******( LED点滅処理 )*******
FATAL_TRAP_LED:				;R1…LED点滅ﾊﾟﾀｰﾝ

;-------( LED点灯処理 )-------
FATAL_TRAP_LED00:			;R1…LED点滅ﾊﾟﾀｰﾝ
					;R2…LED点滅回数
	MOV	R1,R3			;R3←LED点滅ﾊﾟﾀｰﾝ(作業用)
	LED_GBR_SET			;GBR←LED port GBR
	LED_ALL_OFF			;赤&緑LED消灯

FATAL_TRAP_LED10:
	MOV	R3,R0
	AND	#3,R0
	CMP/EQ	#0,R0			;R0=0 ?
	BT	FATAL_TRAP_LED50
	CMP/EQ	#1,R0
	BT	FATAL_TRAP_LED11

	MOV.W	#FATAL_LED_UNIT*3,R4	;R4←長短の時間
	BRA	FATAL_TRAP_LED15	;(遅延分岐)
	NOP

FATAL_TRAP_LED11:
	MOV.W	#FATAL_LED_UNIT,R4	;R4←短点の時間

FATAL_TRAP_LED15:
	LED_RED_ON			;赤LED点灯

	BSR	$MON_WAIT1_SENSE	;(遅延分岐)
	NOP
	TST	R0,R0			;R0=0 ?
	BF	CMD_LOOP_JMP

	LED_RED_OFF			;赤LED消灯

;	( 符号間の時間待ち )
	MOV.W	#FATAL_LED_UNIT,R4	;R4←短点の時間
	BSR	$MON_WAIT1_SENSE	;(遅延分岐)
	NOP
	TST	R0,R0			;R0=0 ?
	BF	CMD_LOOP_JMP

	BRA	FATAL_TRAP_LED10	;(遅延分岐)
	SHLR2	R3			;次の符号へ

;-------( 字間の時間待ち )-------
FATAL_TRAP_LED50:
	MOV.W	#FATAL_LED_UNIT*7,R4	;R4←字間の時間
	BSR	$MON_WAIT1_SENSE	;(遅延分岐)
	NOP
	TST	R0,R0			;R0=0 ?
	BF	CMD_LOOP_JMP
	BRA	FATAL_TRAP_LED00	;(遅延分岐)
	NOP

CMD_LOOP_JMP:
	LED_ALL_OFF			;赤&緑LED消灯
	BRA	CMD_LOOP		;(遅延分岐)
	MOV	#0,R14			;R14←0(ﾌﾗｯｼｭ無効)

	.PAGE
;================================================
;○$MON_RESET　　　　　 《リセット待ち》
;　［入力］なし
;　［出力］なし
;  ［Note］・
;================================================
_MON_RESET:
	LED_GBR_SET			;GBR←LED port GBR
	LED_GREEN_OFF			;緑LED消灯

	.AIF	\&IWDT_USE EQ 1		;内蔵WDT使用?
	BSR	BOOT_WDT_STOP		;(遅延分岐),でWDT停止
	NOP

	MOV.L	#EXPEVT_ADDR,R1
	MOV	#0,R0
	MOV.L	R0,@R1			;(EXPEVT)←0 ※ﾊﾟﾜｰｵﾝﾘｾｯﾄに書き換え※

	BRA	_BOOT_START		;(遅延分岐)
	NOP

	.AELSE
RESET_LOOP:
	LED_RED_INV			;赤LED反転
	BRA	RESET_LOOP		;(遅延分岐), 無限ﾙｰﾌﾟで外部WDTﾘｾｯﾄ待ち
	NOP

	.AENDI


;================================================
;○$MON_WAIT1　　　　　 《約1m秒の待ち》
;　［入力］R4(L)…待ち回数
;　［出力］なし
;  ［Note］・ﾘﾌﾚｯｼｭｶｳﾝﾀを利用して待ちﾀｲﾐﾝｸﾞを計る。
;　　　　　・待ち同時にWDTをﾄﾘｶﾞする。
;  ［破壊］R0,R4,R5,R6,PR
;================================================
$MON_WAIT1:				;(R4…待ちｶｳﾝﾄ)
	MOV.L	#RFCR_ADDR,R0
	MOV.W	@R0,R5			;R5←現在のﾘﾌﾚｯｼｭｶｳﾝﾀを保持
WAIT1_LOOP:
	MOV.L	#RFCR_ADDR,R0
	MOV.W	@R0,R0			;現在のﾘﾌﾚｯｼｭｶｳﾝﾀを獲得
	SUB	R5,R0			;R0←R0-R5
	MOV.W	#H'03FF,R6		;ﾏｽｸﾃﾞｰﾀ
	AND	R6,R0
	MOV.W	#RFCR_1MS_COUNT,R6	;比較ﾃﾞｰﾀ(1m秒)
	CMP/HI	R6,R0			;R0＞R6
	BF	WAIT1_LOOP		;約1m秒待ち

	MOV.L	#DOG_FOOD_ADDR,R0
	MOV.W	@R0,R0			;WDTﾄﾘｶﾞ

	DT	R4			;R4-1
	BF	$MON_WAIT1
	RTS				;(遅延分岐)
	NOP


;================================================
;○$MON_WAIT1_SENSE　　 《約1m秒の待ち & '@'文字入力のﾁｪｯｸ》
;　［入力］R4(L)…待ち回数
;　［出力］=1:'@'文字入力あり, =0:'@'文字入力なし
;  ［Note］・ﾘﾌﾚｯｼｭｶｳﾝﾀを利用して待ちﾀｲﾐﾝｸﾞを計る。
;　　　　　・待ち同時にWDTをﾄﾘｶﾞする。
;　　　　　・'@'文字を検出した時はﾙｰﾌﾟの途中でもすぐに抜け出す。
;  ［破壊］R0,R4,R5,R6,PR
;================================================
$MON_WAIT1_SENSE:			;(R4…待ちｶｳﾝﾄ)
	STC	GBR,R6			;save GBR
	MOV.L	#SCI_IOBASE,R0
	LDC	R0,GBR			;GBR←IOBASE address

	.AIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	MOV.W	@(SCI_SCSCR,GBR),R0	;
	AND  	#H'BF,R0			;RIE=0,受信割込禁止(ﾋﾞｯﾄｸﾘｱ)
	MOV.W	R0,@(SCI_SCSCR,GBR)	;

	.AELSE
	MOV	#SCI_SCSCR,R0
	AND.B	#H'BF,@(R0,GBR)		;RIE=0,受信割込禁止
	.AENDI

	LDC	R6,GBR			;restore GBR
	
;-------( 1m秒待ち処理 )-------
WAIT1S_LOOP0:
	MOV.L	#RFCR_ADDR,R0
	MOV.W	@R0,R5			;R5←現在のﾘﾌﾚｯｼｭｶｳﾝﾀを保持

WAIT1S_LOOP:
	STC	GBR,R6			;save GBR
	MOV.L	#SCI_IOBASE,R0
	LDC	R0,GBR			;GBR←IOBASE address

;	( '@'文字のチェック )
	.AIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	MOV.W	@(SCI_SCSSR,GBR),R0	;SCSSR=ｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀ
	TST	#H'91,R0		;ER,BRK,DR(#7,4,0)=1 ?
	BF	WAIT1S_RSERR		;異常検出
	TST	#H'02,R0		;RDF(BIT1)=1 ?
	BT	WAIT1S_CHECK		;文字受信してない

	MOV.W	@(SCI_SCSSR,GBR),R0	;
	AND   	#H'FD,R0			;RDF(BIT1)を0ｸﾘｱ
	MOV.W	R0,@(SCI_SCSSR,GBR)	;

	.AELSE
	MOV.B	@(SCI_SCSSR,GBR),R0
	TST	#H'38,R0		;ORER,PER,FER=1 ?
	BF	WAIT1S_RSERR		;異常検出
	TST	#H'40,R0		;RDFR=1 ?
	BT	WAIT1S_CHECK		;文字受信してない
	
	MOV	#SCI_SCSSR,R0
	AND.B	#H'BF,@(R0,GBR)		;RDRFを0ｸﾘｱ
	.AENDI
	
	MOV.B	@(SCI_SCRDR,GBR),R0	;R0←文字入力
	CMP/EQ	#"@",R0			;'@'文字 ?
	BF	WAIT1S_CHECK
	LDC	R6,GBR			;restore GBR
	RTS				;(遅延分岐)
	MOV	#1,R0			;'@'文字を検出した

WAIT1S_RSERR:
	.AIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	TST	#H'90,R0		;DR(#0)=0 ?
	BT	?0010
	MOV.B	@(SCI_SCRDR,GBR),R0	;文字入力(受信データ廃却)
?0010:
	MOV.W	@(SCI_SCSSR,GBR),R0	;SCSSR=ｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀ
	AND  	#H'6E,R0			;ER,BRK,DR(#7,4,0)をｸﾘｱ
	MOV.W	R0,@(SCI_SCSSR,GBR)	;
	MOV.W	@(SCLSR_X,GBR),R0	;SCLSR=ﾗｲﾝｽﾃｰﾀｽﾚｼﾞｽﾀ
	AND  	#H'FE,R0			;RE(#0)をｸﾘｱ
	MOV.W	R0,@(SCLSR_X,GBR)	;

	.AELSE
	MOV	#SCI_SCSSR,R0
	AND.B	#H'87,@(R0,GBR)		;RDRF,ORER,PER,FERをｸﾘｱ
	.AENDI

;	( ﾘﾌﾚｯｼｭｶｳﾝﾀのﾁｪｯｸ )
WAIT1S_CHECK:
	LDC	R6,GBR			;restore GBR

	MOV.L	#RFCR_ADDR,R0
	MOV.W	@R0,R0			;現在のﾘﾌﾚｯｼｭｶｳﾝﾀを獲得
	SUB	R5,R0			;R0←R0-R5
	MOV.W	#H'03FF,R6		;ﾏｽｸﾃﾞｰﾀ
	AND	R6,R0
	MOV.W	#64,R6			;比較ﾃﾞｰﾀ(15.6μ×64＝1m秒)
	CMP/HI	R6,R0			;R0＞R6
	BF	WAIT1S_LOOP		;約1m秒待ち

	MOV.L	#DOG_FOOD_ADDR,R0
	MOV.W	@R0,R0			;WDTﾄﾘｶﾞ

	DT	R4			;R4-1
	BF	WAIT1S_LOOP0
	RTS				;(遅延分岐)
	MOV	#0,R0			;'@'文字を検出してない


;================================================
;○$MON_MEMDUMP　　　　《メモリー内容の表示》
;　［入力］R4(L)…表示アドレス
;　　　　　R5(L)…カウント
;　　　　　R6(B)…表示サイズ(1=Byte,2=Word,4=Dword)
;　［出力］なし
;  ［Note］・デバック用
;================================================
_MON_MEMDUMP:				;TRAPA命令entry
					;MD=1,特権ﾓｰﾄﾞ
					;BL=1,例外要求ﾏｽｸ
					;RB=1,ﾚｼﾞｽﾀﾊﾞﾝｸﾋﾞｯﾄ
	MOV	R6,R8			;R8←表示サイズ
	BRA	MEMDUMP10		;(遅延分岐)
	MOV	R4,R3			;R3←先頭番地
MEMDUMP00:
	MOV	R3,R0
	TST	#H'F,R0
	BF	MEMDUMP20
MEMDUMP10:
	CROUT
	HEXOUT	R3			;アドレス表示
	CHAROUT	#":"			;(DOG_FOOD含む)

MEMDUMP20:
	MOV	R8,R0
	CMP/EQ	#1,R0
	BT	MEMDUMP21
	CMP/EQ	#2,R0
	BT	MEMDUMP22
	CMP/EQ	#4,R0
	BT	MEMDUMP24

;	( BYTE表示 )
MEMDUMP21:
	MOV.B	@R3+,R0
	HEXOUT	R0,2
	BRA	MEMDUMP29		;(遅延分岐)
	NOP

;	( WORD表示 )
MEMDUMP22:
	MOV.W	@R3+,R0
	HEXOUT	R0,4
	BRA	MEMDUMP29		;(遅延分岐)
	NOP

;	( DWORD表示 )
MEMDUMP24:
	MOV.L	@R3+,R0
	HEXOUT	R0,8
MEMDUMP29:
	CHAROUT	#" "			;(DOG_FOOD含む)

	DT	R5
	BF	MEMDUMP00

	CROUT

	BRA	CMD_LOOP		;(遅延分岐)
	NOP


;================================================
;○$MON_RSOUT　　　　　 《RS-232C 1文字出力》
;　［入力］R4(B)…出力文字
;　［出力］なし
;  ［Note］・割込みは使用せず、ポーリングにより出力。
;　　　　　・文字出力と同時にWDTをﾄﾘｶﾞする。
;  ［破壊］R0,GBR,PR
;================================================
$MON_RSOUT:
	MOV.L	#SCI_IOBASE,R0
	LDC	R0,GBR			;GBR←SCI_IOBASE address

	.AIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	MOV.W	@(SCI_SCSSR,GBR),R0	;SCSSR=ｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀ
	TST   	#H'20,R0			;TDFE(#5)=1 ?
	.AELSE
	MOV	#SCI_SCSSR,R0
	TST.B	#H'80,@(R0,GBR)		;TDRE=1 ?
	.AENDI
	BT	$MON_RSOUT		;書込み可能まで待つ

	MOV	R4,R0
	MOV.B	R0,@(SCI_SCTDR,GBR)	;文字出力

	.AIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	MOV.W	@(SCI_SCSSR,GBR),R0	;SCSSR=ｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀ
	AND  	#H'9F,R0		;TDFE,TEND(#5,#6)を0ｸﾘｱ
	MOV.W	R0,@(SCI_SCSSR,GBR)	;
	.AELSE
	MOV  	#SCI_SCSSR,R0
	AND.B	#H'7F,@(R0,GBR)		;TDREを0ｸﾘｱ
	.AENDI

	MOV.L	#DOG_FOOD_ADDR,R0
	RTS				;(遅延分岐)
	MOV.W	@R0,R0			;WDTﾄﾘｶﾞ


;================================================
;○$MON_STROUT　　　　　《RS-232C 文字列出力》
;　［入力］R4(B)…文字列先頭番地
;　　　　　R5(B)…区切り文字
;　［出力］なし
;  ［Note］・割込みは使用せず、ポーリングにより出力。
;　　　　　・文字出力と同時にWDTをﾄﾘｶﾞする。
;  ［破壊］R0,R1,R2,R4,R5,GBR,PR
;================================================
$MON_STROUT:
	STS	PR,R2			;PRの待避

	BRA	STROUT_CHK		;(遅延分岐)
	MOV	R4,R1			;R1←文字列アドレス
STROUT_LOOP:
	BSR	$MON_RSOUT		;(遅延分岐),1文字表示
	NOP
STROUT_CHK:
	MOV.B	@R1+,R0			;R0←文字
	CMP/EQ	R5,R0			;終了文字?
	BF/S	STROUT_LOOP		;(遅延分岐)
	MOV	R0,R4			;R4←表示文字

	LDS	R2,PR			;PRの復帰
	RTS				;(遅延分岐)
	NOP


;================================================
;○$MON_RSIN　　　　　　《RS-232C 1文字入力》
;　［入力］R0(B)…入力待ち(0:しない,1:する)
;　［出力］R0(L)…結果(1:入力あり, 0:入力なし, -1:異常検出)
;　　　　　R4(B)…入力文字
;  ［Note］・割込みは使用せず、ポーリングにより入力。
;　　　　　・WDTをﾄﾘｶﾞする。
;  ［破壊］GBR,PR
;================================================
$MON_RSIN:
	EXTU.B	R0,R4			;R4←待ち指定
	MOV.L	#SCI_IOBASE,R0
	LDC	R0,GBR			;GBR←SCI_IOBASE address

	.AIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	MOV.W	@(SCI_SCSCR,GBR),R0	;
	AND   	#H'BF,R0			;RIE=0,受信割込禁止
	MOV.W	R0,@(SCI_SCSCR,GBR)	;
	.AELSE
	MOV	#SCI_SCSCR,R0
	AND.B	#H'BF,@(R0,GBR)		;RIE=0,受信割込禁止
	.AENDI

RSIN_LOOP:
	MOV.L	#DOG_FOOD_ADDR,R0
	MOV.W	@R0,R0			;WDTﾄﾘｶﾞ
	
	.AIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	MOV.W	@(SCI_SCSSR,GBR),R0	;SCSSR=ｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀ
	TST	#H'91,R0		;ER,BRK,DR(#7,4,0)=1 ? 
	BF	RSIN_ERR		;異常検出
	TST	#H'02,R0		;RDF(BIT1)=1 ? 
	.AELSE
	MOV.B	@(SCI_SCSSR,GBR),R0
	TST	#H'38,R0		;ORER,PER,FER=1 ?
	BF	RSIN_ERR		;異常検出
	TST	#H'40,R0		;RDFR=1 ?
	.AENDI
	
	BF	RSIN_OK			;文字受信
	TST	R4,R4			;R4=0(待ち指定) ?
	BF	RSIN_LOOP		;文字入力するまでループ
RSIN_NONE:
	RTS				;(遅延分岐)
	MOV	#0,R0			;入力なし

RSIN_OK:
	MOV.B	@(SCI_SCRDR,GBR),R0	;文字入力
	EXTU.B	R0,R4			;R4←入力文字

	.AIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	MOV.W	@(SCI_SCSSR,GBR),R0	;SCSSR=ｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀ
	AND  	#H'FD,R0			;RDF(BIT1)を0ｸﾘｱ
	MOV.W	R0,@(SCI_SCSSR,GBR)	;
	.AELSE
	MOV	#SCI_SCSSR,R0
	AND.B	#H'BF,@(R0,GBR)		;RDRFを0ｸﾘｱ
	.AENDI

	RTS				;(遅延分岐)
	MOV	#1,R0			;入力あり

RSIN_ERR:
	.AIF	"_SH4" EQ "2"		;SH7760(IOP5) ?
	TST	#H'90,R0		;DR(#0)=0 ?
	BT	?0010
	MOV.B	@(SCI_SCRDR,GBR),R0	;文字入力(受信データ廃却)
?0010:
	MOV.W	@(SCI_SCSSR,GBR),R0	;SCSSR=ｼﾘｱﾙｽﾃｰﾀｽﾚｼﾞｽﾀ
	AND  	#H'6E,R0			;ER,BRK,DR(#7,4,0)をｸﾘｱ
	MOV.W	R0,@(SCI_SCSSR,GBR)	;
	MOV.W	@(SCLSR_X,GBR),R0	;SCLSR=ﾗｲﾝｽﾃｰﾀｽﾚｼﾞｽﾀ
	AND  	#H'FE,R0			;RE(#0)をｸﾘｱ
	MOV.W	R0,@(SCLSR_X,GBR)	;
	.AELSE
	
	MOV	#SCI_SCSSR,R0
	AND.B	#H'87,@(R0,GBR)		;RDRF,ORER,PER,FERをｸﾘｱ
	.AENDI

	MOV	#0,R4
	RTS				;(遅延分岐)
	MOV	#-1,R0			;異常検出


;================================================
;○$MON_HCTOB　　　　　 《ASCII文字→BIN変換》
;　［入力］R4(B)…入力(文字)
;　［出力］R0(L)…≧0：変換結果(0～F)
;　　　　　　　　 =-1：その他の文字
;  ［Note］・正常に変換する文字は"0"～"9","A"～"F"。
;  ［破壊］PR
;================================================
$MON_HCTOB:
	EXTU.B	R4,R4

	MOV	#"9",R0
	CMP/HI	R0,R4			;R4＞R0 ?
	BT	?0010
	MOV	#"0",R0
	CMP/HS	R0,R4			;R4≧R0 ?
	BF	HCTOB_ERR		;to error
	MOV	R4,R0
	RTS				;(遅延分岐)
	ADD	#-"0",R0

?0010:
	MOV	#"F",R0
	CMP/HI	R0,R4			;R4＞R0 ?
	BT	HCTOB_ERR
	MOV	#"A",R0
	CMP/HS	R0,R4			;R4≧R0 ?
	BF	HCTOB_ERR		;to error
	MOV	R4,R0
	RTS				;(遅延分岐)
	ADD	#(10-"A"),R0

HCTOB_ERR:
	RTS				;(遅延分岐)
	MOV	#-1,R0


;================================================
;○$MON_BTOHC　　　　　 《BIN→HEX文字変換》
;　［入力］R0(H)…入力(数値…下位4bit)
;　［出力］R0(B)…出力(文字…下位8bit)
;  ［Note］・
;  ［破壊］PR
;================================================
$MON_BTOHC:
	AND	#H'0F,R0
	ADD	#-H'0A,R0
	CMP/PZ	R0
	BF	?0010
	ADD	#H'07,R0
?0010:
	RTS				;(遅延分岐)
	ADD	#H'3A,R0


;================================================
;○$MON_TOUPPER　　　　 《大文字変換》
;　［入力］R4(B)…入力
;　［出力］R4(B)…出力
;  ［Note］・
;  ［破壊］R0,PR
;================================================
$MON_TOUPPER:
	EXTU.B	R4,R4

	MOV	#"a",R0
	CMP/HS	R0,R4			;R4≧R0
	BF	TOUPPER_RET
	MOV	#"z",R0
	CMP/HI	R0,R4			;R4＞R0
	BT	TOUPPER_RET

	RTS				;(遅延分岐)
	ADD	#-H'20,R4		;大文字変換

TOUPPER_RET
	RTS				;(遅延分岐)
	NOP


;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	.PAGE
;************************************************
;    FMP monitor routine code
;************************************************
FMP_START:				;R13…__sysid情報
					;R14…ﾌﾗｯｼｭ先頭ｱﾄﾞﾚｽ
	STROUT	#FMP_START_MSG		;ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)
	CMP/PZ	R13			;R13≧0 ?
	BT	CMD_LOOP
	STROUT	#BOOT_MODE_MSG		;ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)

CMD_LOOP:
;	( プロンプトの表示 )
	CROUT				;改行表示
	MOV	#"#",R4			;ﾌﾟﾛﾝﾌﾟﾄ
	CHAROUT	R4			;(DOG_FOOD含む)

;	( １文字入力 )
?0001:	CHARIN				;R0←結果,R4←入力文字
	CMP/PL	R0			;入力あり?
	BF	?0001			;エラー時は無視
	MOV	R4,R0
	CMP/EQ	#0,R0
	BT	?0001			;｢0｣は無視
	CMP/EQ	#1,R0
	BT	?0001			;｢1｣は無視
	BSR	$MON_RSOUT		;ｴｺｰﾊﾞｯｸ(遅延分岐)
	NOP

;	( コマンド解析処理 )
	BSR	$MON_TOUPPER		;R4←ｺﾏﾝﾄﾞ文字,(遅延分岐)
	NOP

	MOVA	CMD_TABLE,R0
	MOV	R0,R1			;R1←table address
CMD_PARSE:
	MOV.L	@R1+,R0			;R0←比較文字
	CMP/EQ	#0,R0			;R0=0 ?
	BT	CMD_LOOP
	CMP/EQ	R0,R4
	BT	CMD_JUMP

	BRA	CMD_PARSE		;(遅延分岐)
	ADD	#4,R1			;skip jump address

CMD_JUMP:
	MOV.L	@R1+,R0			;R0←jump address
	JMP	@R0			;(遅延分岐)
	NOP


;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
	.ALIGN	4
CMD_TABLE:
	.DATA.L	"D",D_CMD
	.DATA.L	"Z",Z_CMD
	.DATA.L	"G",G_CMD
	.DATA.L	"T",T_CMD
	.DATA.L	"R",FATAL_TRAP00
;##	.DATA.L	"F",$MON_FATAL_TRAP	;for DEBUG#1761

	.AIF	\&FMP_USE EQ 1		;FMP使用時のみ
	.DATA.L	"B",FM_BLKCHK_CMD
	.DATA.L	"S",FM_SERASE_CMD
	.DATA.L	"E",FM_ERASE_CMD
	.DATA.L	"R",FM_READ_CMD
	.DATA.L	"C",FM_CHKSUM_CMD
	.DATA.L	"W",FM_WRITE_CMD
	.DATA.L	"X",FM_WRITE_CMD
	.AENDI
	.DATA.L	0,0			;終了マーク
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	.ALIGN	4
	.PAGE
;*******( モニタ実行開始 )*******
G_CMD:
	TST	R14,R14			;R14=0 ?
	BT	CMD_LOOP		;R14=0の時は無視

	CROUT				;改行表示
	BRA	MON_TEST_JMP		;(遅延分岐)
	NOP


;*******( リセット )*******
Z_CMD:
	STROUT	#RESET_MSG		;ﾒｯｾｰｼﾞ表示,(DOG_FOOD含む)
	BRA	_MON_RESET		;(遅延分岐)
	NOP


;*******( メモリ内容表示 )*******
D_CMD:
	MOV	#0,R3			;アドレス初期値
	MOV	#H'40,R5		;カウント初期値
	MOV	#1,R6			;サイズ初期値

;-------( 表示サイズの入力 )-------
	HEXIN
	MOV	R4,R0
	CMP/EQ	#"@",R0			;@ ?
	BT	D_CMD_END
	CMP/EQ	#H'0D,R0		;CR ?
	BT	D_CMD50
	CMP/EQ	#H'20,R0		;SP ?
	BT	D_CMD05
D_CMD_END:
	BRA	CMD_LOOP		;(遅延分岐)
	NOP

D_CMD05:
	TST	R1,R1			;R1=0 ?
	BT	D_CMD10
	BRA	D_CMD10			;(遅延分岐)
	MOV	R1,R6			;R6←表示サイズ

;-------( アドレスの入力 )-------
D_CMD10:
	HEXIN
	MOV	R1,R3			;R3←アドレス
	MOV	R4,R0
	CMP/EQ	#"@",R0			;@ ?
	BT	D_CMD_END
	CMP/EQ	#H'0D,R0		;CR ?
	BT	D_CMD50

;-------( カウントの入力 )-------
	HEXIN
	MOV	R1,R5			;R5←アドレス
	MOV	R4,R0
	CMP/EQ	#"@",R0			;@ ?
	BT	D_CMD_END
	CMP/EQ	#H'0D,R0		;CR ?
	BT	D_CMD50

;-------( 表示サイズの入力 )-------
	HEXIN
	MOV	R1,R6			;R6←表示サイズ
	MOV	R4,R0
	CMP/EQ	#"@",R0			;@ ?
	BT	D_CMD_END
	CMP/EQ	#H'0D,R0		;CR ?
	BT	D_CMD50

;-------( データ表示 )-------
D_CMD50:
	MOV	R3,R4			;R4←アドレス
					;R5…カウント
					;R6…表示サイズ
	BRA	_MON_MEMDUMP		;(遅延分岐)
	NOP


;*******( テストループ )*******
T_CMD:
	DEBUG_LED_LOOP



;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.POOL
BOOT_MODE_MSG	.SDATAZ	"*** BOOT MODE ***"<H'0D><H'0A>
RESET_MSG	.SDATAZ	<H'0D><H'0A>"Reset."<H'0D><H'0A>
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#endif	//////////////////////////////////////////

