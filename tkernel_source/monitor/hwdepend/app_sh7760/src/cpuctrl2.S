/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(C) 2015-2016 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2016/01/26.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cpuctrl2.S
 *
 *       SH7760(SH-4) CPU control
 *
 */
#define	_in_asm_source_

#include <machine.h>
#include <tk/sysdef.h>


/* ------------------------------------------------------------------------ */
/*
 *       cache control
 *       acts on the whole address space.
 */
	.text

/*
 * turn on cache
 */
	.balign	4
	.globl	Csym(EnableCache)
	.type	Csym(EnableCache), %function
Csym(EnableCache):
	mov.l	CACHE_ON, r0			// Cache on
	jmp	@r0				// P2領域へジャンプ
	nop

/*
 * turn off cache
 */
	.balign	4
	.globl	Csym(DisableCache)
	.type	Csym(DisableCache), %function
Csym(DisableCache):
	mov.l	CACHE_OFF, r0			// Cache off
	jmp	@r0				// P2領域へジャンプ
	nop

/*
 * When accessing the cache control register, the program itself
 * must be in the non-cache area. Therefore convert to the address
 * of the shadow image in P2 area and call.
 */
	.balign	4
CACHE_ON:	.long	cache_on + 0x20000000
CACHE_OFF:	.long	cache_off + 0x20000000


/* ------------------------------------------------------------------------ */
/*
 *	Cache control
 */

#define	CACHE_LINESZ	0x20		/* Cache line size */

/*
 * Cache on
 */
	.text
	.balign	2
cache_on:
	sts.l	pr, @-SP

	bsr	cache_off		// Disable cache once.
	nop

	mov.l	C_CCR, r0
	mov.l	C_CACHE_ON, r1
	mov.l	r1, @r0			// Cache on (Writeback is enabled)

	nop;nop;nop;nop;nop;nop;nop

	lds.l	@SP+, pr
	rts
	nop

/*
 * Cache off
 */
	.text
	.balign	2
cache_off:
	mov.l	C_CCR, r1		// Disable cache and
	mov.l	C_CCR_MASK, r0		// invalidate instruction cache

	mov.l	@r1, r2
	and	r0, r2
	mov.l	C_CACHE_OFF, r0
	or	r0, r2
	mov.l	r2, @r1

	mov	#0, r0			// Flush cache
	mov.l	C_CACHE_TAGTOP, r1	// (Writeback and disable)
	mov.l	C_CACHE_TAGEND, r2
  l_flush:
	mov.l	r0, @r1
	add	#CACHE_LINESZ, r1
	cmp/hi	r1, r2
	bt	l_flush

	rts
	nop

			.balign	4
  C_CCR:		.long	CCR
  C_CACHE_ON:	.long	CCR_OCE|CCR_CB|CCR_OCI|CCR_ICE|CCR_ICI|CCR_EMODE
  C_CACHE_OFF:	.long	CCR_ICI			// Invalidate instruction cache

  C_CCR_MASK:	.long	CCR_ORA|CCR_OIX|CCR_IIX|CCR_EMODE
  C_CACHE_TAGTOP:	.long	0xf4000000	// Data cache only

  C_CACHE_TAGEND:	.long	0xf4008000


/*
 * flush the entire cache (write back and then invalidate)
 *	void FlushCache( void )
 */
	.text
	.balign	4
	.globl	Csym(FlushCache)
	.type	Csym(FlushCache), %function
Csym(FlushCache):
	mov.l	CACHE_FLASH, r0			// Cache flash
	jmp	@r0				// P2領域へジャンプ
	nop

	.balign	4
CACHE_FLASH:	.long	cache_on + 0x20000000



#if 0	///////////////
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
	mcr	p15, 0, ip, cr8, c7, 0		// Invalidate I/D-TLB

        /* set new r1 for CP15 */
	mrc	p15, 0, r2, cr1, cr0, 0
	ldr	r3, =0x3307			// V,I,R,S,C,A,M (B = 0)
	and	r0, r4, r3
	mvn	r3, r3				// clear old V,I,R,S,C,A,M
	and	r2, r2, r3
	orr	r0, r0, r2
	mcr	p15, 0, r0, cr1, cr0, 0
	nop
	nop

	ldmfd	sp!, {r4, lr}			// restore registers
	bx	lr

	rts					//(遅延分岐)
	nop
#endif	/////////////////


/*----------------------------------------------------------------------*/
#if 0
#|【cpuctrl2.S 改版履歴】
#|□2015/12/14	 [app_sh7760]用に新規作成
#|  ・ [tef_em1d]用の"cpuctrl2.S"を参考に作成｡
#|□2016/01/26	EnableCache(),DisableCache()関数を追加。
#|  ("{V1}/kernel/sysdepend/device/std_sh7760\icrt0.S"のコードを参考)
#|
#endif
