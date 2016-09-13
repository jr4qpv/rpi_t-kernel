/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2012/11/27.
 *    Modified by T-Engine Forum at 2014/07/28.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/08/30.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)tmonitor.h (tm) 2016/08/30
 *
 *	T-Monitor
 */

#ifndef __TM_TMONITOR_H__
#define __TM_TMONITOR_H__

#include <basic.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define L_DEVNM		8	/* Device name length */

/*
 * Boot information
 */
typedef struct BootInfo {
	UB	devnm[L_DEVNM];	/* Physical device name */
	INT	part;		/* Partition number
				   (-1: no partition) */
	INT	start;		/* Partition first sector number
				   (0: no partition) */
	INT	secsz;		/* Sector size (byte) */
} BootInfo;

/*
 * Extended service call function number (tm_extsvc)
 */
#define TMEF_PORTBPS	0x00U	/* Debug port speed (bps) */
#define TMEF_RDAINFO	0x01U	/* ROM disk information */
#define TMEF_PCIINFO	0x02U	/* PCI device information */

#define TMEF_PORTNO		0x04U	/* Debug port number */

#define TMEF_DIPSW		0x10U	/* DIPSW state */
#define TMEF_LED		0x11U	/* LED control */
#define TMEF_WROM		0x20U	/* Flash ROM write */

/*
 *	ROM disk information
 */
typedef struct	{
	UW	rd_type;	/* ROM disk type (1: ROM disk) */
	UW	rd_blksz;	/* ROM disk block size (normally 512) */
	UW	rd_saddr;	/* ROM disk start address */
	UW	rd_eaddr;	/* ROM disk end address */
} RdaInfo;

/*
 *	PCI device information
 */
#define CADDR(bus, dev, func)	(((bus) << 8) | ((dev) << 3) | (func))

typedef struct	{
	UH	caddr;		/* Configuration address */
	UH	vendor; 	/* Vendor ID */
	UH	devid;		/* Device ID */
	UH	devclass;	/* Device class */
} PciInfo;

/*
 * Monitor service function
 */
IMPORT void tm_monitor( void );
IMPORT INT  tm_getchar( INT wait );
IMPORT INT  tm_putchar( INT c );
IMPORT INT  tm_getline( UB *buff );
IMPORT INT  tm_putstring( const UB *buff );
IMPORT INT  tm_command( const UB *buff );
IMPORT INT  tm_readdisk( const UB *dev, INT sec, INT nsec, void *addr );
IMPORT INT  tm_writedisk( const UB *dev, INT sec, INT nsec, void *addr );
IMPORT INT  tm_infodisk( const UB *dev, INT *blksz, INT *nblks );
IMPORT void tm_exit( INT mode );
IMPORT INT  tm_extsvc( INT fno, INT par1, INT par2, INT par3 );

/*
 * Monitor related library
 */
IMPORT int tm_vprintf( const char *format, va_list ap );
IMPORT int tm_vsprintf( char *str, const char *format, va_list ap );
IMPORT int tm_sprintf( char *str, const char *format, ... );
IMPORT int tm_printf( const char *format, ... );
#define	bms_printf	tm_printf

#ifdef __cplusplus
}
#endif
#endif /* __TM_TMONITOR_H__ */


/*----------------------------------------------------------------------
#|History of "tmonitor.h"
#|=======================
#|* 2016/08/30	It's made by making reference to "tmonitor.h" for [tef_em1d].
#|* 2016/08/30	add TMEF_PORTNO,TMEF_LED define.
#|
*/
