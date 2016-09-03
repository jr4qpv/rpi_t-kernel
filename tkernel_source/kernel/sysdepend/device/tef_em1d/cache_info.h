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
 */

/*
 *	cache_info.h (EM1-D512)
 *	Cache Information
 */

#ifndef _CACHE_INFO_
#define _CACHE_INFO_

/*
 * Set non-cache area memory
 *	When using the control table for non-cache area memory
 *	by memory manager routines, define the address of non-cache area.
 *	
 *	When not using, specify 0 for UseNoCacheMemoryTable.
 */
#define USE_NOCACHE_MEMTBL	(0)	/* Do not use */

#define NoCacheMemoryTop	(0)	/* Top address of non-cache area  */
#define NoCacheMemoryEnd	(0)	/* End address of non-cache area  */

/*
 * Conversion between page number and address
 *	When switching ON/Off of cache by an address,
 *	define the conversion formula for the following Macro:
 */
#define CachingAddr(p)		(p)	/* Do not convert */
#define NoCachingAddr(p)	(p)

/*
 * Conversion between physical address and logical address of
 * real memory area (physical space)
 */
#define toLogicalAddress(paddr)		(void*)(paddr)
#define toPhysicalAddress(laddr)	(void*)(laddr)

/*
 * Convert logical address into logical address of the cache off area
 */
#define toNoCacheLogicalAddress(laddr)	(void*)(laddr)

#endif /* _CACHE_INFO_ */
