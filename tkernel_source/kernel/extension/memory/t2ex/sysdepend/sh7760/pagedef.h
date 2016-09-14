/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/04.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/01/06.
 *
 *----------------------------------------------------------------------
 */
/*
 * This software package is available for use, modification, 
 * and redistribution in accordance with the terms of the attached 
 * T-License 2.x.
 * If you want to redistribute the source code, you need to attach 
 * the T-License 2.x document.
 * There's no obligation to publish the content, and no obligation 
 * to disclose it to the TRON Forum if you have modified the 
 * software package.
 * You can also distribute the modified source code. In this case, 
 * please register the modification to T-Kernel traceability service.
 * People can know the history of modifications by the service, 
 * and can be sure that the version you have inherited some 
 * modification of a particular version or not.
 *
 *    http://trace.tron.org/tk/?lang=en
 *    http://trace.tron.org/tk/?lang=ja
 *
 * As per the provisions of the T-License 2.x, TRON Forum ensures that 
 * the portion of the software that is copyrighted by Ken Sakamura or 
 * the TRON Forum does not infringe the copyrights of a third party.
 * However, it does not make any warranty other than this.
 * DISCLAIMER: TRON Forum and Ken Sakamura shall not be held
 * responsible for any consequences or damages caused directly or
 * indirectly by the use of this software package.
 *
 * The source codes in bsd_source.tar.gz in this software package are 
 * derived from NetBSD or OpenBSD and not covered under T-License 2.x.
 * They need to be changed or redistributed according to the 
 * representation of each source header.
 */

/*
 *	pagedef.h (T2EX)
 *	T2EX: page definitions (em1d)
 */

#ifndef _T2EX_PAGEDEF_
#define _T2EX_PAGEDEF_

#define	PAGESIZE	( 4096 )		/* page size (in bytes) */
#define SECTIONSIZE	( PAGESIZE * 256 )	/* section size (in bytes) */

#define	MAX_SECTION		0x1000		/* The maximum number of sections */

/*
 * Page size
 *	PageCount	gets the number of pages
 *	RoundPage	round-up the number of bytes to page boundaries
 */
Inline UW PageCount( UW byte_sz )
{
	return (byte_sz + (PAGESIZE-1)) / PAGESIZE;
}
Inline UW RoundPage( UW byte_sz )
{
	return (byte_sz + (PAGESIZE-1)) & ~(PAGESIZE-1);
}

/*
 * Page address
 *	PageOffset	address offset in a page
 *	PageAlignL	align memory address to the lower page boundary
 *	PageAlignU	align memory address to the upper page boundary
 *	NextPage	get next page address
 */
Inline UW PageOffset( const void *addr )
{
	return (UW)addr & (PAGESIZE-1);
}
Inline VP PageAlignL( const void *addr )
{
	return (VP)((UW)addr & ~(PAGESIZE-1));
}
Inline VP PageAlignU( const void *addr )
{
	return (VP)(((UW)addr + (PAGESIZE-1)) & ~(PAGESIZE-1));
}
Inline VP NextPage( const void *addr )
{
	return (VB*)addr + PAGESIZE;
}

/*
 * Section address
 */
Inline UW SectionOffset( const void *addr )
{
	return (UW)addr & (SECTIONSIZE-1);
}
Inline VP SectionAlignL( const void *addr )
{
	return (VP)((UW)addr & ~(SECTIONSIZE-1));
}
Inline VP SectionAlignU( const void *addr )
{
	return (VP)(((UW)addr + (SECTIONSIZE-1)) & ~(SECTIONSIZE-1));
}
Inline VP NextSection( const void *addr )
{
	return (VB*)addr + SECTIONSIZE;
}

#endif /* _T2EX_PAGEDEF_ */


/*----------------------------------------------------------------------*/
#if 0
#|【pagedef.h 変更履歴】
#|□2016/01/06	[app_sh7760]用に、[tef_em1d]の"pagedef.h"をとりあえず流用。
#|  ※見直し必要※
#|
#endif
