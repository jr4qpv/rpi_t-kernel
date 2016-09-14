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
 *    Modified by T.Yokobayashi at 2015/12/18.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cache.c (SH7760)
 *	Cache Operation
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/sysdef.h>

#define PAGESIZE	0x1000U		/* Page size */
#define CACHE_LINESZ	0x20U		/* Cache line size */

#define ICACHE_ADR_TOP	0xf0000000	/* Instruction cache address array */
#define ICACHE_DAT_TOP	0xf1000000	/* Instruction cache data array */
#define ICACHE_WAY_MSK	0x00002000
#define ICACHE_ENT_MSK	0x00001fe0

#define DCACHE_ADR_TOP	0xf4000000	/* Data cache address array */
#define DCACHE_DAT_TOP	0xf5000000	/* Data cache data array */
#define DCACHE_WAY_MSK	0x00004000
#define DCACHE_ENT_MSK	0x00003fe0

#define CACHE_V		0x00000001	/* Valid */
#define CACHE_U		0x00000002	/* Dirty */
#define CACHE_TAG	0xfffffc00	/* Tag */

#define CACHE_A		0x00000008	/* Associative specification */


/*
 * Obtain cache line size
 */
EXPORT INT GetCacheLineSize( void )
{
#if 1	////////////// kari //////////////
	return 0;		
#else	//////////////////////////////////
	return CacheLineSZ;
#endif	//////////////////////////////////
}


/*
 *  One page cache flush
 *	Flush one page cache (4KB) corresponding to logical address 'laddr'.
 *	'laddr' must be top address of page.
 *	
 *	Cache control program must be located at non-cached area,
 *	so P2 area (as shadow area) is used to execute the program.
 *	For this purpose, call the program with address pointed by
 *	'_FlushCache_', and don't call '_flush_cache_' directly.
 */
LOCAL void _flush_cache_( UW laddr, UW mode )
{
	UW	icarray, ent;

	ent = laddr;
	if ( (in_w(CCR) & CCR_IIX) != 0 ) {
		ent >>= 25 - 12;
	}
	ent &= ICACHE_ENT_MSK & ~(PAGESIZE-1);

	icarray = ICACHE_ADR_TOP | CACHE_A | ent;

	if ( (mode & TCM_ICACHE) != 0 ) {
		for ( ent = 0; ent < PAGESIZE; ent += CACHE_LINESZ ) {
			/* Instruction cache */
			*(UW*)(icarray + ent) = (laddr + ent) & CACHE_TAG;
		}
	}
	if ( (mode & TCM_DCACHE) != 0 ) {
		for ( ent = 0; ent < PAGESIZE; ent += CACHE_LINESZ ) {
			/* Data cache */
			Asm("ocbp @%0":: "r"(laddr + ent));
		}
	}
}

LOCAL FP _FlushCacheP2_ = (FP)((UW)&_flush_cache_ + 0x20000000);

#define FlushCacheP2(laddr, mode)	(*_FlushCacheP2_)(laddr, mode)

/*
 * Cache flush
 *	Flush cache between 'laddr' and 'len' bytes areas.
 *	Writeback and disable cache
 */
EXPORT void FlushCacheM( VP laddr, INT len, UINT mode )
{
	UW	top, end;
	UINT	imask;

	top = (UW)laddr & ~(PAGESIZE-1);
	end = (UW)laddr + len;

	while ( top < end ) {

		DI(imask);
		FlushCacheP2(top, mode);
		EI(imask);

		top += PAGESIZE;
	}
}

EXPORT void FlushCache( VP laddr, INT len )
{
	FlushCacheM(laddr, len, TCM_ICACHE|TCM_DCACHE);
}


/*
 * Control cache
 *	mode := [CC_FLUSH] | [CC_INVALIDATE]
 */
EXPORT ER ControlCacheM( void *laddr, INT len, UINT mode )
{
#if 0	//////////////// kari ////////////////
	VB	*p, *ep;

	if ( (mode & ~(CC_FLUSH|CC_INVALIDATE)) != 0 ) return E_PAR;

	ep = (VB*)laddr + len;

	p = (VB*)((UINT)laddr & ~(CacheLineSZ - 1));
	while ( p < ep ) {
		switch ( mode ) {
		  case CC_FLUSH:
			/* Clean data cache to PoC */
			Asm("mcr p15, 0, %0, cr7, c10, 1":: "r"(p));
			break;
		  case CC_INVALIDATE:
			/* Invalidate data cache to PoC */
			Asm("mcr p15, 0, %0, cr7, c6, 1":: "r"(p));
			break;
		  default:
			/* Clean and Invalidate data cache to PoC */
			Asm("mcr p15, 0, %0, cr7, c14, 1":: "r"(p));
		}

		/* Invalidate instruction cache to PoC */
		Asm("mcr p15, 0, %0, cr7, c5,  1":: "r"(p));

		p += CacheLineSZ;
	}
	Asm("mcr p15, 0, %0, cr7, c5, 6":: "r"(0));
	DSB();
#endif	//////////////////////////////////////////

	return E_OK;
}


/*----------------------------------------------------------------------*/
#if 0
#|【cache.c 変更履歴】
#|□2015/12/18	[app_sh7760]用に、{V1.02.04}の[std_sh7760の"chache.c"からコピー。
#|
#endif
