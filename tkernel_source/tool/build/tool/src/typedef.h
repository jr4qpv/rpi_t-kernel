/*
 *	@(#)typedef.h 02-03-09
 *
 *	ɸ��ǡ��������
 *	Copyright (C) 2002 by Personal Media Corporation
 */

#ifndef	__TYPEDEF_H__
#define	__TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ����Ū�ʥǡ���������
 */
typedef char		B;	/* ����դ�  8�ӥå����� */
typedef short		H;	/* ����դ� 16�ӥå����� */
typedef int		W;	/* ����դ� 32�ӥå����� */
typedef unsigned char	UB;	/* ���̵��  8�ӥå����� */
typedef unsigned short  UH;	/* ���̵�� 16�ӥå����� */
typedef unsigned int	UW;	/* ���̵�� 32�ӥå����� */

typedef char		VB;	/* �������ꤷ�ʤ�  8�ӥåȤΥǡ��� */
typedef short		VH;	/* �������ꤷ�ʤ� 16�ӥåȤΥǡ��� */
typedef int		VW;	/* �������ꤷ�ʤ� 32�ӥåȤΥǡ��� */
typedef void		*VP;	/* �������ꤷ�ʤ��ǡ����ؤΥݥ��� */

typedef volatile B	_B;	/* volatile ����� */
typedef volatile H	_H;
typedef volatile W	_W;
typedef volatile UB	_UB;
typedef volatile UH	_UH;
typedef volatile UW	_UW;

typedef int		INT;	/* �ץ��å��Υӥå���������դ����� */
typedef unsigned int	UINT;	/* �ץ��å��Υӥå��������̵������ */

typedef INT		ID;	/* ID ���� */
typedef	INT		MSEC;	/* ���ְ���(�ߥ���) */

typedef void		(*FP)();	/* �ؿ����ɥ쥹���� */
typedef INT		(*FUNCP)();	/* �ؿ����ɥ쥹���� */

#define	LOCAL		static		/* �����륷��ܥ���� */
#define	EXPORT				/* �����Х륷��ܥ���� */
#define	IMPORT		extern		/* �����Х륷��ܥ뻲�� */

/*
 * �֡�����
 *	TRUE = 1 ��������뤬��0 �ʳ��Ϥ��٤ƿ�(TRUE)�Ǥ��롣
 *	�������äơ�bool == TRUE ���ͤ�Ƚ��򤷤ƤϤ����ʤ���
 *	bool != FALSE ���ͤ�Ƚ�ꤹ�뤳�ȡ�
 */
typedef INT		BOOL;
#define TRUE		1	/* �� */
#define FALSE		0	/* �� */

/*
 * TRON ʸ��������
 */
typedef UH		TC;		/* TRON ʸ�������� */
#define	TNULL		((TC)0)		/* TRON ������ʸ����ν�ü */

#ifdef __cplusplus
}
#endif
#endif /* __TYPEDEF_H__ */
