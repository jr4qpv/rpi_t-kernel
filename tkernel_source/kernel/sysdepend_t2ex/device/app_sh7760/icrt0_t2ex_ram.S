/*
 *----------------------------------------------------------------------
 *    T-Kernel
 *
 *    Copyright (C) 2004-2008 by Ken Sakamura. All rights reserved.
 *    T-Kernel is distributed under the T-License.
 *----------------------------------------------------------------------
 *
 *    Version:   1.02.04
 *    Released by T-Engine Forum(http://www.t-engine.org) at 2008/02/29.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2015/12/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	icrt0_t2ex_ram.S (SH7760)
 *	System Startup
 */

#include <machine.h>
#include <tk/sysdef.h>

#if _Csym == 0
#define	START	_start
#else
#define	START	start
#endif

	/* Low level memory manager information */
	.comm	Csym(lowmem_top), 4	// Head of area (Low address)
	.comm	Csym(lowmem_limit), 4	// End of area (High address)

	.lcomm	monitor_stacktop, 4	// Monitor stack pointer for re-startup

	.text
	.balign	2
	.globl	START
	.type	START, @function
START:
	/* Use the stack set by the boot loader */

	mov.l	INIT_SR, r0		// SR initial setting
	ldc	r0, sr

	/* SR.MD=1, SR.BL=0, SR.RB=0, SR.I=15 */

	mov.l	CACHE_ON, r0		// Cache on
	jsr	@r0
	nop

	mov.l	DATA_ORG, r1		// Initialization of 'data' area (ROM startup)
	mov.l	DATA_START, r2
	mov.l	EDATA, r3
	cmp/eq	r1, r2			// If r1 != r2, start ROM
	bt/s	nocopy_data
	movt	r8			// If start ROM, r8 = 0
  copy_data:
	cmp/hi	r2, r3
	bf	nocopy_data
	mov.l	@r1+, r0
	mov.l	r0, @r2
	bra	copy_data
	add	#4, r2
  nocopy_data:

	mov.l	DATA_USR_ORG, r1	// Initialization of user 'data' area (ROM startup)
	mov.l	DATA_USR_START, r2
	mov.l	EDATA_USR, r3
	cmp/eq	r1, r2
	bt	nocopy_usr_data
  copy_usr_data:
	cmp/hi	r2, r3
	bf	nocopy_usr_data
	mov.l	@r1+, r0
	mov.l	r0, @r2
	bra	copy_usr_data
	add	#4, r2
  nocopy_usr_data:

	mov	#0, r0			// Clear 'bss' area
	mov.l	BSS_TOP, r1
	mov.l	BSS_END, r2
	cmp/hi	r1, r2
	bf	nobss
	add	#4, r1
  clrbss:
	cmp/hi	r1, r2
	bt/s	clrbss
	mov.l	r0, @-r2
  nobss:

	mov	#0, r0			// Clear user 'bss' area
	mov.l	BSS_USR_TOP, r1
	mov.l	BSS_USR_END, r2
	cmp/hi	r1, r2
	bf	nobss_usr
	add	#4, r1
  clrbss_usr:
	cmp/hi	r1, r2
	bt/s	clrbss_usr
	mov.l	r0, @-r2
  nobss_usr:

	mov.l	MON_STACKTOP, r0	// Monitor stack pointer for re-startup
	mov.l	r15, @r0		// Save

	mov.l	RAM_TOP, r1		// Low level memory manager initial setting
	mov.l	@r1, r1
	mov.l	BSS_END, r0
	cmp/hi	r0, r1			// _end or RAM_TOP
	bt	l1			// Either of High addresses
	mov	r0, r1
  l1:
	mov.l	RAM_END, r3
	mov.l	@r3, r3
	mov.l	LOWMEM_TOP, r2
	mov.l	LOWMEM_LIM, r4
	mov.l	r1, @r2			// lowmem_top   = RAM_TOP or _end
	mov.l	r3, @r4			// lowmem_limit = RAM_END

	mov.l	SYSCONF, r1
	mov.l	@r1, r1
	tst	r1, r1			// If SYSCONF == 0,
	bt	l2			// set default parameters to SCInfo

	tst	r8, r8			// If r8 == 0, start Rom
	bf	l3
  l2:
	mov.l	ROM_STARTUP, r0		// Initialization at ROM startup
	jsr	@r0
	nop
  l3:
	mov.l	MAIN, r0		// System start up
	jsr	@r0
	nop

  l_end:				// Not suppose to return from 'main,'
	bra	l_end			// but, just in case, prepare for out of control.
	nop

		.balign	4
  INIT_SR:	.long	SR_MD | SR_I(15)
  DATA_ORG:	.long	Csym(__data_org)
  DATA_START:	.long	Csym(__data_start)
  EDATA:	.long	Csym(_edata)
  DATA_USR_ORG:	.long	Csym(__data_usr_org)
  DATA_USR_START: .long	Csym(__data_usr_start)
  EDATA_USR:	.long	Csym(_edata_usr)
  BSS_TOP:	.long	Csym(__bss_start)
  BSS_END:	.long	Csym(_end)
  BSS_USR_TOP:	.long	Csym(__bss_usr_start)
  BSS_USR_END:	.long	Csym(_end_usr)
  LOWMEM_TOP:	.long	Csym(lowmem_top)
  LOWMEM_LIM:	.long	Csym(lowmem_limit)
  ROM_STARTUP:	.long	Csym(ROM_startup)
  MAIN:		.long	Csym(main)
  MON_STACKTOP:	.long	monitor_stacktop

	/* System shared information */
  RAM_TOP:	.long	0x8c000280
  RAM_END:	.long	0x8c000284
  SYSCONF:	.long	0x8c000288

/*
 * When accessing the cache control register, the program itself
 * must be in the non-cache area. Therefore convert to the address
 * of the shadow image in P2 area and call.
 */
CACHE_ON:	.long	cache_on + 0x20000000

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


/*----------------------------------------------------------------------*/
#if 0
#|【icrt0_t2ex_ram.S 変更履歴】
#|□2015/12/29	"../tef_em1d/icrt0_t2ex_ram.S"を参考に、[app_sh7760]用に作成。
#|
#endif
