/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/12/19.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)bitop.h (sys)
 *
 *	Bit operation
 *
 *	Bit string is defined as follows according to endian of CPU:
 *
 *		Big endian		Little endian
 *	offset	 0 ...... 7		 7 ...... 0
 *		+----------+		+----------+
 *	base +0	|MSB	LSB|		|MSB	LSB|
 *		+----------+		+----------+
 *
 *	offset	 8 ..... 15		 15 ..... 8
 *		+----------+		+----------+
 *	base +1	|MSB	LSB|		|MSB	LSB|
 *		+----------+		+----------+
 *
 *	Specify bit positions with base and offset.
 *	Cannot specify a position that precede base or "offset < 0".
 */

#ifndef	__SYS_BITOP_H__
#define	__SYS_BITOP_H__

#include <basic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Clear/set/flip specified bit
 */
IMPORT void	BitClr( void *base, UW offset );
IMPORT void	BitSet( void *base, UW offset );
IMPORT void	BitNot( void *base, UW offset );

/*
 * Test specified bit
 *	When specified bit is 0, return FALSE; when specified bit is 1, return TRUE(!=0).
 */
IMPORT BOOL	BitTest( const void *base, UW offset );

/*
 * Retrieve bit
 *	Retrieve the width bits from the position of specified bit and return the position.
 *	If not found, return -1.
 *	The found position is returned in relative position from the retrieval start position.
 *	When found, the return value is between 0 and width-1.
 */
IMPORT W	BitSearch0( const void *base, W offset, W width ); /* Retrieve bit 0 */
IMPORT W	BitSearch1( const void *base, W offset, W width ); /* Retrieve bit 1 */

/* Faster BitSearch for aligned memory */
IMPORT W	BitSearch0_w( const UW *base, W offset, W width ); /* Retrieve bit 0 */
IMPORT W	BitSearch1_w( const UW *base, W offset, W width ); /* Retrieve bit 1 */

/*
 * Clear/set bit string
 *	Clear or set the width bits from the position of specified bit.
 */
IMPORT void	BitsClr( void *base, W offset, W width );
IMPORT void	BitsSet( void *base, W offset, W width );

#ifdef __cplusplus
}
#endif
#endif /* __SYS_BITOP_H__ */
