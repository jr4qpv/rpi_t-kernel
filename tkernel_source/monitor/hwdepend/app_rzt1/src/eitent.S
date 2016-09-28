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
 *	@(#)eitent.S (monitor/hwdepend) 2016/09/28
 *
 *      RZT1 (Cortex-R4F) exception branch handling
 */

#define	_in_asm_source_

#include <machine.h>
#include <tk/sysdef.h>
#include <arm/rzt1.h>
#include <sys/sysinfo.h>

#define	base(n)		( (n) & 0xfffff000 )
#define	offs(n)		( (n) & 0x00000fff )

// see <sys/sysdepend/app_rzt1/sysinfo_depend.h>
#define	N_INTVEC	512

/*
 *       EIT entry
 */

	.section EITBRA, "ax"
	.arm
	.org	0x00000000
	b	startup_entry		// 00 : reset
	b	undef_entry		// 04 : undefined instruction exception
	b	svc_entry		// 08 : supervisor call (SVC)
	b	iabort_entry		// 0C : prefetch abort
	b	dabort_entry		// 10 : data abort
	nop				// 14 : (reserved)
	b	irq_entry		// 18 : interrupt
	.org	0x0000001c		// 1C : fast interrupt

/*
 * fast interrupt
 *       calls the handler defined at FIQ interrupt vector unconditionally.
 *       no saving of registers to stack is performed.
 *       the content of R12_fiq(ip) register is overwritten.
 */
fiq_entry:
	ldr	ip, =base(EIT_VECTBL)
	ldr	ip, [ip, #offs(EITVEC(EIT_FIQ))]
	bx	ip

/*
 * interrupt
 *       ACPU interrupt mask status register of Interrupt controller (AINT)
 *      Judge the priority of interrupts using (IT0_MST0,1,2),
 *       the highest interrupt's handler is called by jumping into it.
 *       Interrupt priority is descending order of interrupt factor (INT 0-95) , and INT 95 (IRQ 95) is highest.
 *       INT 0 (IRQ 0) has the lowest priority.
 *       If there is no cause of the interrupt, the handler of INT 95 (IRQ95) is called.
 *		+---------------+
 *	sp  ->	|R3		|
 *		|R12=ip		|
 *              |R14=lr         | <- return address from interrupt
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
irq_entry:
#if 0	//////////
	sub	lr, lr, #4		// return address adjustment
	srsdb	sp!, #PSR_IRQ		// save registers
	stmfd	sp!, {r3, ip}

	ldr	lr, =base(AINTBase)
	ldr	ip, =EITVEC(EIT_IRQ(95))

	ldr	r3, [lr, #offs(IT0_MST2)]
	cmp	r3, #0
	bne	l_irq_br

	sub	ip, ip, #32*4
	ldr	r3, [lr, #offs(IT0_MST1)]
	cmp	r3, #0
	bne	l_irq_br

	sub	ip, ip, #32*4
	ldr	r3, [lr, #offs(IT0_MST0)]
	cmp	r3, #0
	bne	l_irq_br

	ldr	ip, =EITVEC(EIT_IRQ(95))
#else	//////////
irq_common_entry:
	ldr	ip, =EITVEC(EIT_IRQ(0))
#endif	//////////////

  l_irq_br:
////	clzne	r3, r3
////	ldr	lr, [ip, -r3, lsl #2]!
	ldr	lr, [ip, r3, lsl #2]!
	cmp	lr, #0
	bxne	lr
	b	default_entry


#if 0 ////////////////////////////
/*
 * GPIO interrupt
 *      Interrupt obtained by means of input port interrupt maskable status register (GIO_MST)
 *       is analyzed to check the interrupt priority, and if appropriate, the highest priority interrupt handler is entered.
 *       branch and call handler.
 *       interrupt priority is descending order of input port NUMBER (port 0 - port 127). port 127 has the highest priority, and
 *       port 0 has the lowest priority GPIO interrupts are grouped : each group has 16 interrupts, and
 *       their priorities are considered only within the context of each group.
 *       if there is no cause of interupt, the handler of IRQ95 is called.
 *		+---------------+
 *	sp  ->	|R3		|
 *		|R12=ip		|
 *              |R14=lr         | <- return address from interrupt
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
 .macro gio_e reg, vec
	ldr	lr, =base(\reg)
	ldr	r3, [lr, #offs(\reg)]
	lsls	r3, r3, #16
	beq	l_gio_spurious

	ldr	ip, =\vec
	clz	r3, r3
	ldr	lr, [ip, -r3, lsl #2]!
	cmp	lr, #0
	bxne	lr
	b	default_entry
 .endm
 .macro gio_o reg, vec
	ldr	lr, =base(\reg)
	ldr	r3, [lr, #offs(\reg)]
	lsrs	ip, r3, #16
	beq	l_gio_spurious

	ldr	ip, =\vec
	clz	r3, r3
	ldr	lr, [ip, -r3, lsl #2]!
	cmp	lr, #0
	bxne	lr
	b	default_entry
 .endm

	.globl	Csym(_gio0Hdr)
	.type	Csym(_gio0Hdr), %function
	.globl	Csym(_gio1Hdr)
	.type	Csym(_gio1Hdr), %function
	.globl	Csym(_gio2Hdr)
	.type	Csym(_gio2Hdr), %function
	.globl	Csym(_gio3Hdr)
	.type	Csym(_gio3Hdr), %function
	.globl	Csym(_gio4Hdr)
	.type	Csym(_gio4Hdr), %function
	.globl	Csym(_gio5Hdr)
	.type	Csym(_gio5Hdr), %function
	.globl	Csym(_gio6Hdr)
	.type	Csym(_gio6Hdr), %function
	.globl	Csym(_gio7Hdr)
	.type	Csym(_gio7Hdr), %function
Csym(_gio0Hdr):	gio_e	GIO_MST(GIO_L),   EITVEC(EIT_GPIO( 15))
Csym(_gio1Hdr):	gio_o	GIO_MST(GIO_L),   EITVEC(EIT_GPIO( 31))
Csym(_gio2Hdr):	gio_e	GIO_MST(GIO_H),   EITVEC(EIT_GPIO( 47))
Csym(_gio3Hdr):	gio_o	GIO_MST(GIO_H),   EITVEC(EIT_GPIO( 63))
Csym(_gio4Hdr):	gio_e	GIO_MST(GIO_HH),  EITVEC(EIT_GPIO( 79))
Csym(_gio5Hdr):	gio_o	GIO_MST(GIO_HH),  EITVEC(EIT_GPIO( 95))
Csym(_gio6Hdr):	gio_e	GIO_MST(GIO_HHH), EITVEC(EIT_GPIO(111))
Csym(_gio7Hdr):	gio_o	GIO_MST(GIO_HHH), EITVEC(EIT_GPIO(127))

  l_gio_spurious:
	ldr	ip, =base(EITVEC(EIT_IRQ(95)))
	ldr	lr, [ip, #offs(EITVEC(EIT_IRQ(95)))]!
	cmp	lr, #0
	bxne	lr
	b	default_entry
#endif	//////////////////////////////////

/*
 * undefined instruction
 *		+---------------+
 *	sp  ->	|R12=ip		|
 *              |R14=lr         | <- the return address, i.e., the next address that follows the undefined instruction
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
undef_entry:
	srsdb	sp!, #PSR_UND		// save registers
	stmfd	sp!, {ip}

	ldr	ip, =base(EITVEC(EIT_UNDEF))
	ldr	lr, [ip, #offs(EITVEC(EIT_UNDEF))]!
	cmp	lr, #0
	bxne	lr
	b	default_entry

/*
 * supervisor call(SVC)
 *      the valid range of supervisor call number is 0-255 (N_INTVEC - 1).
 *       if an out of range value is given, treat it as SVC 0, and invokes the default handler.
 *		+---------------+
 *	sp  ->	|R12=ip		|
 *              |R14=lr         | <- return address: the address that follows the SVC instruction
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
svc_entry:
	srsdb	sp!, #PSR_SVC		// save registers
	stmfd	sp!, {ip}

	mrs	ip, spsr
	tst	ip, #PSR_T
	ldrneh	ip, [lr, #-2]		// Thumb instruction
	ldreq	ip, [lr, #-4]		// ARM instruction
	bicne	lr, ip, #0xff00
	biceq	lr, ip, #0xff000000
	cmp	lr, #N_INTVEC		// lr = software interrupt number
	movge	lr, #0

	ldr	ip, =EIT_VECTBL
	ldr	lr, [ip, lr, lsl #2]!
	cmp	lr, #0
	bxne	lr
	b	default_entry

/*
 * prefetch abort
 *       in the case of debug event, debug abort (instruction) handler is called.
 *       Otherwise, prefetch abort handler is called.
 *		+---------------+
 *	sp  ->	|R12=ip		|
 *              |R14=lr         | <- return address: the address of aborted instruction
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
iabort_entry:
	sub	lr, lr, #4		// return address adjustment
	srsdb	sp!, #PSR_ABT		// save registers
	stmfd	sp!, {ip}

	mrc	p15, 0, ip, c5, c0, 1	// IFSR
	tst	ip, #0x400		// FS[4]
	and	ip, ip, #0x00f		// FS[3:0]
	cmpeq	ip, #FSR_DebugEvent

	ldr	ip, =base(EITVEC(EIT_IABORT))
	ldrne	lr, [ip, #offs(EITVEC(EIT_IABORT))]!
	ldreq	lr, [ip, #offs(EITVEC(EIT_IDEBUG))]!
	cmp	lr, #0
	bxne	lr
	b	default_entry

/*
 * data abort
 *       in the case of debug event, debug abort (data) handler is called.
 *       Otherwise, data abort handler is called.
 *		+---------------+
 *	sp  ->	|R12=ip		|
 *              |R14=lr         | <- return address: the address of aborted instruction
 *		|SPSR		|
 *		+---------------+
 *      registers upon handler invocation
 *       ip = vector table address
 *       lr = indeterminate
 */
dabort_entry:
	sub	lr, lr, #8		// return address adjustment
	srsdb	sp!, #PSR_ABT		// save registers
	stmfd	sp!, {ip}

	mrc	p15, 0, ip, c5, c0, 0	// DFSR
	tst	ip, #0x400		// FS[4]
	and	ip, ip, #0x00f		// FS[3:0]
	cmpeq	ip, #FSR_DebugEvent

	ldr	ip, =base(EITVEC(EIT_DABORT))
	ldrne	lr, [ip, #offs(EITVEC(EIT_DABORT))]!
	ldreq	lr, [ip, #offs(EITVEC(EIT_DDEBUG))]!
	cmp	lr, #0
	bxne	lr
	b	default_entry

/*
 * default handler
 *       stack contains the details of the generated exception.
 *      registers upon handler invocation
 *       ip = address of the vector table for the raised exception
 *       lr = indeterminate
 */
default_entry:
	ldr	lr, =base(EITVEC(EIT_DEFAULT))
	ldr	lr, [lr, #offs(EITVEC(EIT_DEFAULT))]
	bx	lr

	.pool


/*
 *       INTerrupt entry
 *
 *  ※RZ/T1の割り込みは、要因別の独立したアドレスへジャンプする仕様で、
 *    要因別の割り込みから、割り込み情報をR3レジスタに格納して、
 *    irq_common_entryに分岐する処理を、以下に定義。
 *  ※下記のマクロ処理の命令数が変わったら、system.cの初期化処理を修正
 *
 */
 .macro INT_ENTRY0 intno		// 5word(20byte)
	sub	lr, lr, #4		// return address adjustment
	srsdb	sp!, #PSR_IRQ		// save registers
	stmfd	sp!, {r3, ip}

	mov	r3, #\intno
	b	irq_common_entry
 .endm

 .macro INT_ENTRY1 intno		// 6work(24byte)
	sub	lr, lr, #4		// return address adjustment
	srsdb	sp!, #PSR_IRQ		// save registers
	stmfd	sp!, {r3, ip}

	ldr	r3, 1f
	b	irq_common_entry
1:	.word	\intno
 .endm

	.balign	4
	.globl	Csym(interrupt_entry_1)
Csym(interrupt_entry_1):
	INT_ENTRY0	1
	INT_ENTRY0	2
	INT_ENTRY0	3
	INT_ENTRY0	4
	INT_ENTRY0	5
	INT_ENTRY0	6
	INT_ENTRY0	7
	INT_ENTRY0	8
	INT_ENTRY0	9

	INT_ENTRY0	10
	INT_ENTRY0	11
	INT_ENTRY0	12
	INT_ENTRY0	13
	INT_ENTRY0	14
	INT_ENTRY0	15
	INT_ENTRY0	16
	INT_ENTRY0	17
	INT_ENTRY0	18
	INT_ENTRY0	19

	INT_ENTRY0	20
	INT_ENTRY0	21
	INT_ENTRY0	22
	INT_ENTRY0	23
	INT_ENTRY0	24
	INT_ENTRY0	25
	INT_ENTRY0	26
	INT_ENTRY0	27
	INT_ENTRY0	28
	INT_ENTRY0	29

	INT_ENTRY0	30
	INT_ENTRY0	31
	INT_ENTRY0	32
	INT_ENTRY0	33
	INT_ENTRY0	34
	INT_ENTRY0	35
	INT_ENTRY0	36
	INT_ENTRY0	37
	INT_ENTRY0	38
	INT_ENTRY0	39

	INT_ENTRY0	40
	INT_ENTRY0	41
	INT_ENTRY0	42
	INT_ENTRY0	43
	INT_ENTRY0	44
	INT_ENTRY0	45
	INT_ENTRY0	46
	INT_ENTRY0	47
	INT_ENTRY0	48
	INT_ENTRY0	49

	INT_ENTRY0	50
	INT_ENTRY0	51
	INT_ENTRY0	52
	INT_ENTRY0	53
	INT_ENTRY0	54
	INT_ENTRY0	55
	INT_ENTRY0	56
	INT_ENTRY0	57
	INT_ENTRY0	58
	INT_ENTRY0	59

	INT_ENTRY0	60
	INT_ENTRY0	61
	INT_ENTRY0	62
	INT_ENTRY0	63
	INT_ENTRY0	64
	INT_ENTRY0	65
	INT_ENTRY0	66
	INT_ENTRY0	67
	INT_ENTRY0	68
	INT_ENTRY0	69

	INT_ENTRY0	70
	INT_ENTRY0	71
	INT_ENTRY0	72
	INT_ENTRY0	73
	INT_ENTRY0	74
	INT_ENTRY0	75
	INT_ENTRY0	76
	INT_ENTRY0	77
	INT_ENTRY0	78
	INT_ENTRY0	79

	INT_ENTRY0	80
	INT_ENTRY0	81
	INT_ENTRY0	82
	INT_ENTRY0	83
	INT_ENTRY0	84
	INT_ENTRY0	85
	INT_ENTRY0	86
	INT_ENTRY0	87
	INT_ENTRY0	88
	INT_ENTRY0	89

	INT_ENTRY0	90
	INT_ENTRY0	91
	INT_ENTRY0	92
	INT_ENTRY0	93
	INT_ENTRY0	94
	INT_ENTRY0	95
	INT_ENTRY0	96
	INT_ENTRY0	97
	INT_ENTRY0	98
	INT_ENTRY0	99

	INT_ENTRY0	100
	INT_ENTRY0	101
	INT_ENTRY0	102
	INT_ENTRY0	103
	INT_ENTRY0	104
	INT_ENTRY0	105
	INT_ENTRY0	106
	INT_ENTRY0	107
	INT_ENTRY0	108
	INT_ENTRY0	109

	INT_ENTRY0	110
	INT_ENTRY0	111
	INT_ENTRY0	112
	INT_ENTRY0	113
	INT_ENTRY0	114
	INT_ENTRY0	115
	INT_ENTRY0	116
	INT_ENTRY0	117
	INT_ENTRY0	118
	INT_ENTRY0	119

	INT_ENTRY0	120
	INT_ENTRY0	121
	INT_ENTRY0	122
	INT_ENTRY0	123
	INT_ENTRY0	124
	INT_ENTRY0	125
	INT_ENTRY0	126
	INT_ENTRY0	127
	INT_ENTRY0	128
	INT_ENTRY0	129

	INT_ENTRY0	130
	INT_ENTRY0	131
	INT_ENTRY0	132
	INT_ENTRY0	133
	INT_ENTRY0	134
	INT_ENTRY0	135
	INT_ENTRY0	136
	INT_ENTRY0	137
	INT_ENTRY0	138
	INT_ENTRY0	139

	INT_ENTRY0	140
	INT_ENTRY0	141
	INT_ENTRY0	142
	INT_ENTRY0	143
	INT_ENTRY0	144
	INT_ENTRY0	145
	INT_ENTRY0	146
	INT_ENTRY0	147
	INT_ENTRY0	148
	INT_ENTRY0	149

	INT_ENTRY0	150
	INT_ENTRY0	151
	INT_ENTRY0	152
	INT_ENTRY0	153
	INT_ENTRY0	154
	INT_ENTRY0	155
	INT_ENTRY0	156
	INT_ENTRY0	157
	INT_ENTRY0	158
	INT_ENTRY0	159

	INT_ENTRY0	160
	INT_ENTRY0	161
	INT_ENTRY0	162
	INT_ENTRY0	163
	INT_ENTRY0	164
	INT_ENTRY0	165
	INT_ENTRY0	166
	INT_ENTRY0	167
	INT_ENTRY0	168
	INT_ENTRY0	169

	INT_ENTRY0	170
	INT_ENTRY0	171
	INT_ENTRY0	172
	INT_ENTRY0	173
	INT_ENTRY0	174
	INT_ENTRY0	175
	INT_ENTRY0	176
	INT_ENTRY0	177
	INT_ENTRY0	178
	INT_ENTRY0	179

	INT_ENTRY0	180
	INT_ENTRY0	181
	INT_ENTRY0	182
	INT_ENTRY0	183
	INT_ENTRY0	184
	INT_ENTRY0	185
	INT_ENTRY0	186
	INT_ENTRY0	187
	INT_ENTRY0	188
	INT_ENTRY0	189

	INT_ENTRY0	190
	INT_ENTRY0	191
	INT_ENTRY0	192
	INT_ENTRY0	193
	INT_ENTRY0	194
	INT_ENTRY0	195
	INT_ENTRY0	196
	INT_ENTRY0	197
	INT_ENTRY0	198
	INT_ENTRY0	199

	INT_ENTRY0	200
	INT_ENTRY0	201
	INT_ENTRY0	202
	INT_ENTRY0	203
	INT_ENTRY0	204
	INT_ENTRY0	205
	INT_ENTRY0	206
	INT_ENTRY0	207
	INT_ENTRY0	208
	INT_ENTRY0	209

	INT_ENTRY0	210
	INT_ENTRY0	211
	INT_ENTRY0	212
	INT_ENTRY0	213
	INT_ENTRY0	214
	INT_ENTRY0	215
	INT_ENTRY0	216
	INT_ENTRY0	217
	INT_ENTRY0	218
	INT_ENTRY0	219

	INT_ENTRY0	220
	INT_ENTRY0	221
	INT_ENTRY0	222
	INT_ENTRY0	223
	INT_ENTRY0	224
	INT_ENTRY0	225
	INT_ENTRY0	226
	INT_ENTRY0	227
	INT_ENTRY0	228
	INT_ENTRY0	229

	INT_ENTRY0	230
	INT_ENTRY0	231
	INT_ENTRY0	232
	INT_ENTRY0	233
	INT_ENTRY0	234
	INT_ENTRY0	235
	INT_ENTRY0	236
	INT_ENTRY0	237
	INT_ENTRY0	238
	INT_ENTRY0	239

	INT_ENTRY0	240
	INT_ENTRY0	241
	INT_ENTRY0	242
	INT_ENTRY0	243
	INT_ENTRY0	244
	INT_ENTRY0	245
	INT_ENTRY0	246
	INT_ENTRY0	247
	INT_ENTRY0	248
	INT_ENTRY0	249

	INT_ENTRY0	250
	INT_ENTRY0	251
	INT_ENTRY0	252
	INT_ENTRY0	253
	INT_ENTRY0	254
	INT_ENTRY0	255

interrupt_entry_256:
	INT_ENTRY1	256
	INT_ENTRY1	257
	INT_ENTRY1	258
	INT_ENTRY1	259

	INT_ENTRY1	260
	INT_ENTRY1	261
	INT_ENTRY1	262
	INT_ENTRY1	263
	INT_ENTRY1	264
	INT_ENTRY1	265
	INT_ENTRY1	266
	INT_ENTRY1	267
	INT_ENTRY1	268
	INT_ENTRY1	269

	INT_ENTRY1	270
	INT_ENTRY1	271
	INT_ENTRY1	272
	INT_ENTRY1	273
	INT_ENTRY1	274
	INT_ENTRY1	275
	INT_ENTRY1	276
	INT_ENTRY1	277
	INT_ENTRY1	278
	INT_ENTRY1	279

	INT_ENTRY1	280
	INT_ENTRY1	281
	INT_ENTRY1	282
	INT_ENTRY1	283
	INT_ENTRY1	284
	INT_ENTRY1	285
	INT_ENTRY1	286
	INT_ENTRY1	287
	INT_ENTRY1	288
	INT_ENTRY1	289

	INT_ENTRY1	290
	INT_ENTRY1	291
	INT_ENTRY1	292
	INT_ENTRY1	293
	INT_ENTRY1	294
	INT_ENTRY1	295
	INT_ENTRY1	296
	INT_ENTRY1	297
	INT_ENTRY1	298
	INT_ENTRY1	299

	INT_ENTRY1	300
interrupt_entry_bottom:


/*----------------------------------------------------------------------
#|History of "eitent.S"
#|=====================
#|* 2016/03/14	[app_rzt1]用に、[tef_em1d]用の"eitent.S"を参考に作成。
#|* 2016/07/22	割り込みエントリー処理を追加。
#|* 2016/07/22	N_INTVECを256→512に拡張。
#|
*/
