/*
 *	@(#)bzcomp.h 02-07-16
 *
 *	bz ���̷���
 *	Copyright (C) 1997-2002 by Personal Media Corporation
 */

#ifndef __BZCOMP_H__
#define __BZCOMP_H__

#include "typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * �ü찵�̷����쥳����: < Bz_HDR > < ���̥ǡ��� >..
 *	BzMAGIC1S �ϡ������ƥ�ץ�������Ȥ��ƻ���(BTRON)
 */
typedef struct {
	UW	magic1;			/* �ޥ��å��ʥ�� 1 */
	UW	magic2;			/* �ޥ��å��ʥ�� 2 */
	UW	magic3;			/* �ޥ��å��ʥ�� 3 */
	UW	expsz;			/* Ÿ����Υ����� */
} Bz_HDR;

#define	BzMAGIC1	0x427a436f	/* �ޥ��å��ʥ�� 1 ( = 'BzCo' ) */
#define	BzMAGIC1S	0x627a436f	/* �ޥ��å��ʥ�� 1 ( = 'bzCo' ) */
#define	BzMAGIC2	0x6d705265	/* �ޥ��å��ʥ�� 2 ( = 'mpRe' ) */
#define	BzMAGIC3	0x63446174	/* �ޥ��å��ʥ�� 3 ( = 'cDat' ) */
#define	BzRT1		(31)		/* �쥳���ɥ����� */
#define	BzRT2		(9)		/* �쥳���ɥ����� */

#ifdef __cplusplus
}
#endif
#endif /* __BZCOMP_H__ */
