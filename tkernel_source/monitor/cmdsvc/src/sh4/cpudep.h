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
 *    Modified by T.Yokobayashi at 2016/01/26.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cpudep.h
 *
 *       CPU-dependent definitions(SH-4,SH7760)
 */

#include <tk/sysdef.h>



#ifndef	_in_asm_source_
#include <tk/syslib.h>			/* out_w/h/b(), in_w/h/b() の定義がある */


IMPORT W	bootFlag;	/* boot flag */

/*
 * Memory access through physical address
 *       In the case of ARM, actually it is an access by logical address.
 */
Inline UW rd_w( UW *pa )
{
	return *pa;
}
Inline UH rd_h( UH *pa )
{
	return *pa;
}
Inline UB rd_b( UB *pa )
{
	return *pa;
}

Inline void wr_w( UW *pa, UW data )
{
	*pa = data;
}
Inline void wr_h( UH *pa, UH data )
{
	*pa = data;
}
Inline void wr_b( UB *pa, UB data )
{
	*pa = data;
}

/*
 * read/write the SH4-specific registered under monitor management
 *      read/set the value of registers at the time of monitor entry.
 */
IMPORT UW   getCCR( void );				/* CCR register */
///IMPORT UW   getCP15( W reg, W opcd );	/* CP15 register reg: CRn, opcd: Op2 */
IMPORT UW   getCurPC( void );			/* PC register (unmodified) */
IMPORT void setCurPC( UW val );			/* PC register (unmodified) */
IMPORT UW   getCurSR( void );			/* SR register */

/*
 * Validate PC address
 *       Allow only ARM instructions (on 4 bytes boundary).
 *      If addr is valid then return 0, otherwise return -1.
 */
IMPORT W invalidPC2( UW addr );

/*
 * obtain step address
 */
IMPORT W getStepAddr( UW pc, UW cpsr, W mode, UW* npc, UW *rep );

#endif	/* _in_asm_source_ */


/*----------------------------------------------------------------------*/
#if 0
#|【cpudep.h 変更履歴】
#|□2015/12/10	[app_sh7760]用に、新規作成。
#|  [tef_em1d]用の"armv6/cpudep.h"を参考に作成。
#|
#endif
