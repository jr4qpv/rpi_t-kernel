/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/03/07.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)segment.h (sys)
 *
 *	Segment management
 */

#ifndef __SYS_SEGMENT_H__
#define __SYS_SEGMENT_H__

#include <tk/sysext.h>

#ifdef __cplusplus
extern "C" {
#endif

#define INVADR			((void*)-1)	/* Invalid address */

/*
 * CheckSpace(),CheckStrSpace() mode
 */
#define MA_READ		0x04U	/* Read */
#define MA_WRITE	0x02U	/* Write */
#define MA_EXECUTE	0x01U	/* Execute */


/*
 * Definitions for interface library auto generate (mkiflib)
 */
/*** DEFINE_IFLIB
[INCLUDE FILE]
<sys/segment.h>

[PREFIX]
SEG
***/

/* [BEGIN SYSCALLS] */

/* ALIGN_NO 0x100 */
#ifndef LockSpace
IMPORT ER  LockSpace( CONST void *laddr, INT len );
IMPORT ER  UnlockSpace( CONST void *laddr, INT len );
#endif
IMPORT INT CnvPhysicalAddr( CONST void *laddr, INT len, void **paddr );
IMPORT ER  ChkSpace( CONST void *laddr, INT len, UINT mode, UINT env );
IMPORT INT ChkSpaceTstr( CONST TC *str, INT max, UINT mode, UINT env );
IMPORT INT ChkSpaceBstr( CONST UB *str, INT max, UINT mode, UINT env );
IMPORT INT ChkSpaceLen( CONST void *laddr, INT len, UINT mode, UINT env, INT lsid );
IMPORT INT ReadMemSpace( void *laddr, void *buf, INT len, INT lsid );
IMPORT INT WriteMemSpace( void *laddr, void *buf, INT len, INT lsid );
IMPORT INT SetMemSpaceB( void *laddr, INT len, UB data, INT lsid );
IMPORT ER  FlushMemCache( void *laddr, INT len, UINT mode );
IMPORT INT SetCacheMode( void *addr, INT len, UINT mode );
IMPORT INT ControlCache( void *addr, INT len, UINT mode );
IMPORT ER  GetSpaceInfo( CONST void *addr, INT len, T_SPINFO *pk_spinfo );
IMPORT INT SetMemoryAccess( CONST void *addr, INT len, UINT mode );

/* ALIGN_NO 0x100 */
IMPORT ER MapMemory( CONST void *paddr, INT len, UINT attr, void **laddr );
IMPORT ER UnmapMemory( CONST void *laddr );

/* ALIGN_NO 0x1000 */
IMPORT ER MakeSpace( void *laddr, INT npage, INT lsid, UINT pte );
IMPORT ER UnmakeSpace( void *laddr, INT npage, INT lsid );
IMPORT ER ChangeSpace( void *laddr, INT npage, INT lsid, UINT pte );
/* [END SYSCALLS] */

#ifdef __cplusplus
}
#endif
#endif /* __SYS_SEGMENT_H__ */
