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
 *    Modified by T.Yokobayashi at 2016/04/16.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)cpuctrl2.S (BCM283x) 2016/08/29
 *
 *       ARM CPU control
 *
 *       Assume that system control processor (CP15) exists.
 */
#define	_in_asm_source_

#include <machine.h>
#include <tk/sysdef.h>

#ifdef CPU_BCM2837
/* L1 cache=16word,4way,128sets,32KB (CCSIDR=0x700FE01A) */
#define	DCACHE_NWAY		4	/* N as in the number of N-way data cache */
#define	DCACHE_NWAY_SHIFT	30	/* 上位2bit */
#define	DCACHE_NSEG		128	/* number of segments of data cache */
#define	DCACHE_NSEG_SHIFT	6
/* L2 cache=16word,16way,512sets,512KB (CCSIDR=0x703FE07A) */
#define	L2_CACHE_NWAY		16	/* N as in the number of N-way data cache */
#define	L2_CACHE_NWAY_SHIFT	28	/* 上位4bit */
#define	L2_CACHE_NSEG		512	/* number of segments of data cache */
#define	L2_CACHE_NSEG_SHIFT	6
#endif

#ifdef CPU_BCM2836
/* L1 cache=16word,4way,128sets,32KB (CCSIDR=0x700FE01A) */
#define	DCACHE_NWAY		4	/* N as in the number of N-way data cache */
#define	DCACHE_NWAY_SHIFT	30	/* 上位2bit */
#define	DCACHE_NSEG		128	/* number of segments of data cache */
#define	DCACHE_NSEG_SHIFT	6
/* L2 cache=16word,8way,1024sets,512KB (CCSIDR=0x707FE03A) */
#define	L2_CACHE_NWAY		8	/* N as in the number of N-way data cache */
#define	L2_CACHE_NWAY_SHIFT	29	/* 上位3bit */
#define	L2_CACHE_NSEG		1024	/* number of segments of data cache */
#define	L2_CACHE_NSEG_SHIFT	6
#endif

#ifdef CPU_BCM2835
/* L1 cache=8word,4way,128sets,16KB (CTR=0x1D152152) */
#define	DCACHE_NWAY		4	/* N as in the number of N-way data cache */
#define	DCACHE_NWAY_SHIFT	30	/* 上位2bit */
#define	DCACHE_NSEG		128	/* number of segments of data cache */
#define	DCACHE_NSEG_SHIFT	5
#endif

/*
 * flush the entire cache (write back and then invalidate)
 *	void FlushCache( void )
 */
	.text
	.balign	4
	.globl	Csym(FlushCache)
	.type	Csym(FlushCache), %function
Csym(FlushCache):
	ldr	r2, =DCACHE_NWAY-1
  l_flush_dcache1:
	ldr	r3, =DCACHE_NSEG-1
  l_flush_dcache2:
	mov	ip, r2, lsl #DCACHE_NWAY_SHIFT
	orr	ip, ip, r3, lsl #DCACHE_NSEG_SHIFT
	mcr	p15, 0, ip, cr7, c14, 2	// data cache is written back, (DCCISW)
	subs	r3, r3, #1		// and is invalidated
	bpl	l_flush_dcache2
	subs	r2, r2, #1
	bpl	l_flush_dcache1

#ifdef	L2_CACHE_NWAY			/* L2キャッシュはあるか？ */
	ldr	r2, =L2_CACHE_NWAY-1
  l_flush_dcache3:
	ldr	r3, =L2_CACHE_NSEG-1
  l_flush_dcache4:
	mov	ip, r2, lsl #L2_CACHE_NWAY_SHIFT
	orr	ip, ip, r3, lsl #L2_CACHE_NSEG_SHIFT
	orr	ip, ip, #0x2		// レベル(L2キャッシュ)
	mcr	p15, 0, ip, cr7, c14, 2	// data cache is written back, (DCCISW)
	subs	r3, r3, #1		// and is invalidated
	bpl	l_flush_dcache4
	subs	r2, r2, #1
	bpl	l_flush_dcache3
#endif

#ifdef	CPU_ARMv6
	ldr	ip, =0
	mcr	p15, 0, ip, cr7, c7, 0		// Invalidate I/D-Cache
	mcr	p15, 0, ip, cr7, c10, 4		// Drain Write Buffer
#else	/* CPU_ARMv7以降 */
	ldr	ip, =0
	mcr	p15, 0, ip, cr7, c5, 0		// Invalidate I-Cache(ICIALLU)
	mcr	p15, 0, ip, cr7, c6, 1		// Invalidate D-Cache(DCIMVAC)
	mcr	p15, 0, ip, cr7, c14, 2		// Drain Write Buffer(DCCISW)
#endif

	bx	lr

/*
 * cache and MMU control
 *	void setCacheMMU( UW cp15r1 )
 */
	.text
	.balign	4
	.globl	Csym(setCacheMMU)
	.type	Csym(setCacheMMU), %function
Csym(setCacheMMU):
	stmfd	sp!, {r4, lr}			// save registers
	mov	r4, r0				// save argument

        /* flush cache */
	bl	Csym(FlushCache)

        /* TLB flush */
	ldr	ip, =0
	mcr	p15, 0, ip, cr8, c7, 0		// Invalidate I/D-TLB(TLBIALL)

        /* set new r1 for CP15 */
	mrc	p15, 0, r2, cr1, cr0, 0		// SCTLR read
#ifdef	CPU_ARMv6
	ldr	r3, =0x3307			// V,I,R,S,C,A,M (B = 0)
#else	/* CPU_ARMv7以降 */
	ldr	r3, =0x3007			// V,I,C,A,M (B = 0)
#endif
	and	r0, r4, r3
	mvn	r3, r3				// clear old V,I,R,S,C,A,M
	and	r2, r2, r3
	orr	r0, r0, r2
	mcr	p15, 0, r0, cr1, cr0, 0		// SCTLR write
	nop
	nop

	ldmfd	sp!, {r4, lr}			// restore registers
	bx	lr


/*----------------------------------------------------------------------
#|History of "cpuctrl2.S"
#|=======================
#|* 2016/04/16	It's made by making reference to "reset.S" for [tef_em1d].
#|* 2016/04/19	ARMv7ｱｰｷﾃｸﾁｬ(CPU_ARMv7)に対応
#|* 2016/07/21	L2キャッシュのフラッシュ処理を追加。
#|* 2016/08/20	CPU_BCM2837定義に対応
#|
*/
