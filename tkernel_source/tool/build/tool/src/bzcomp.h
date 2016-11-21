/*
 *	@(#)bzcomp.h 02-07-16
 *
 *	bz 圧縮形式
 *	Copyright (C) 1997-2002 by Personal Media Corporation
 */

#ifndef __BZCOMP_H__
#define __BZCOMP_H__

#include "typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 特殊圧縮形式レコード: < Bz_HDR > < 圧縮データ >..
 *	BzMAGIC1S は、システムプロセス指定として使用(BTRON)
 */
typedef struct {
	UW	magic1;			/* マジックナンバ 1 */
	UW	magic2;			/* マジックナンバ 2 */
	UW	magic3;			/* マジックナンバ 3 */
	UW	expsz;			/* 展開後のサイズ */
} Bz_HDR;

#define	BzMAGIC1	0x427a436f	/* マジックナンバ 1 ( = 'BzCo' ) */
#define	BzMAGIC1S	0x627a436f	/* マジックナンバ 1 ( = 'bzCo' ) */
#define	BzMAGIC2	0x6d705265	/* マジックナンバ 2 ( = 'mpRe' ) */
#define	BzMAGIC3	0x63446174	/* マジックナンバ 3 ( = 'cDat' ) */
#define	BzRT1		(31)		/* レコードタイプ */
#define	BzRT2		(9)		/* レコードタイプ */

#ifdef __cplusplus
}
#endif
#endif /* __BZCOMP_H__ */
