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
 *    Modified by T.Yokobayashi at 2015/11/26.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sysdef_depend.h (tk/SH7760)
 *
 *	Definition about SH7760
 *
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_H__
#define __TK_SYSDEF_DEPEND_H__

/*
 * �p�r�w�背�W�X�^
 */
#define	SP	r15			/* �X�^�b�N�|�C���^ */

/*
 * BANK1 ���W�X�^
 *      R0     ��Ɨp                   �^�X�N�t��(With task)
 *      R1     �\��                       �V
 *      R2 MDR ���샂�[�h���W�X�^         �V
 *      R3 SST �V�X�e���X�^�b�N�g�b�v     �V
 *      R4     ��Ɨp                   �^�X�N�Ɨ�(Task independent)
 *      R5     �\��                       �V
 *      R6 ISP ��O�X�^�b�N�|�C���^       �V
 *      R7     ��O��Ɨp                 �V     SR.BL=1 �̎��̂ݎg�p��
 */
#define	MDR	r2			/* ���샂�[�h���W�X�^ */
#define	SST	r3			/* �V�X�e���X�^�b�N�g�b�v */
#define	ISP	r6			/* ��O�X�^�b�N�|�C���^ */

/*
 * MDR register
 */
#define MDR_DCT		0x80000000		/* Delayed context trap request */
#define MDR_PPL(n)	( (n) << 16 )	/* Previous protection level (0-3) */
#define MDR_CPL(n)	( (n) )			/* Current protection level (0-3) */

/*
 * SR ���W�X�^
 */
#define	SR_MD		0x40000000		/* �������[�h */
#define	SR_RB		0x20000000		/* ���W�X�^�o���N */
#define	SR_BL		0x10000000		/* �����u���b�N */
#define	SR_FD		0x00008000		/* FPU �g�p�֎~(SH4) */
#define	SR_I(n)		( (n) << 4 )	/* �����}�X�N���x��(0�`15) */

/*
 * FPU �X�e�[�^�X/�R���g���[�����W�X�^
 */
#define	FPSCR_FR	0x00200000		/* FPU ���W�X�^�o���N */
#define	FPSCR_SZ	0x00100000		/* �]���T�C�Y���[�h */
#define	FPSCR_PR	0x00080000		/* ���x���[�h */
#define	FPSCR_DN	0x00040000		/* �񐳋K�����[�h */
#define	FPSCR_C(n)	((n) << 12)		/* ��O�v��(6bit) */
#define	FPSCR_E(n)	((n) << 7)		/* ��O����(5bit) */
#define	FPSCR_F(n)	((n) << 2)		/* ��O�t���O(5bit) */
#define	FPSCR_RM(n)	(n)				/* �ۂ߃��[�h(2bit) */

					/* FPU ��O */
#define	FPE_I		0x01			/* �s���m */
#define	FPE_U		0x02			/* �A���_�[�t���[ */
#define	FPE_O		0x04			/* �I�[�o�[�t���[ */
#define	FPE_Z		0x08			/* �[�����Z */
#define	FPE_V		0x10			/* �������Z */
#define	FPE_E		0x20			/* FPU �G���[ */

/*
 * ��O�������W�X�^
 */
#define	TRA			0xff000020		/* W:TRAPA ��O */
#define	EXPEVT		0xff000024		/* W:��O���� */
#define	INTEVT		0xff000028		/* W:�������� */

/*
 * MMU register
 */
#define PTEH		0xff000000		/* W:Page table entry high */
#define PTEL		0xff000004		/* W:Page table entry low */
#define PTEA		0xff000034		/* W:Page table entry assistance */
#define TTB			0xff000008		/* W:Page table base */
#define TEA			0xff00000c		/* W:TLB exception address */
#define MMUCR		0xff000010		/* W:MMU control */

#define MMU_AT		0x00000001		/* MMU enable */
#define MMU_TF		0x00000004		/* TLB flush */
#define MMU_SV		0x00000100		/* Single virtual memory mode */
#define MMU_SQMD	0x00000200		/* Store queue mode */

/*
 * Cache register
 */
#define CCR			0xff00001c		/* W:Cache control */
#define QACR0		0xff000038		/* W:Queue address control 0 */
#define QACR1		0xff00003c		/* W:Queue address control 1 */

#define CCR_OCE		0x00000001		/* OC enable */
#define CCR_WT		0x00000002		/* Write through enable (U0 P0 P3) */
#define CCR_CB		0x00000004		/* Copy back enable (P1) */
#define CCR_OCI		0x00000008		/* OC invalidation */
#define CCR_ORA		0x00000020		/* OC RAM enable */
#define CCR_OIX		0x00000080		/* OC index enable */
#define CCR_ICE		0x00000100		/* IC enable */
#define CCR_ICI		0x00000800		/* IC invalidation */
#define CCR_IIX		0x00008000		/* IC index enable */
#define CCR_EMODE	0x80000000		/* Cache double mode */

/*
 * Interrupt controller
 */
#define ICR			0xffd00000		/* H:Interrupt control */
#define IPRA		0xffd00004		/* H:Interrupt priority A */
#define IPRB		0xffd00008		/* H:Interrupt priority B */
#define IPRC		0xffd0000c		/* H:Interrupt priority C */
#define IPRD		0xffd00010		/* H:Interrupt priority D */
#define INTPRI00	0xfe080000		/* W:Interrupt priority level 00 */
#define INTPRI04	0xfe080004		/* W:Interrupt priority level 04 */
#define INTPRI08	0xfe080008		/* W:Interrupt priority level 08 */
#define INTPRI0C	0xfe08000C		/* W:Interrupt priority level 0C */
#define INTREQ00	0xfe080020		/* W:Interrupt factor 00 */
#define INTREQ04	0xfe080024		/* W:Interrupt factor 04 */
#define INTMSK00	0xfe080040		/* W:Interrupt mask 00 */
#define INTMSK04	0xfe080044		/* W:Interrupt mask 04 */
#define INTMSKCLR00	0xfe080060		/* W:Interrupt mask clear 00 */
#define INTMSKCLR04	0xfe080064		/* W:Interrupt mask clear 04 */

/* ------------------------------------------------------------------------ */

/*
 * T-Monitor �pTRAPA �ԍ�
 */
#define	TRAP_MONITOR	0x90		/* T-Monitor �T�[�r�X�R�[�� */

/*
 * T-Kernel �pTRAPA �ԍ�
 */
#define	TRAP_SVC		0x91		/* T-Kernel �V�X�e���R�[���E�g��SVC */
#define	TRAP_RETINT		0x92		/* tk_ret_int() �V�X�e���R�[�� */
#define	TRAP_DISPATCH	0x93		/* �^�X�N�f�B�X�p�b�`���ďo */
									/*  0x94 �\�� */
#define	TRAP_LOADSR		0x95		/* SR ���W�X�^���[�h�@�\ */
#define	TRAP_DEBUG		0x96		/* �f�o�b�K�T�|�[�g�@�\ */

/*
 * Extension�pTRAPA �ԍ�
 */
#define	TRAP_KILLPROC	0x97		/* �v���Z�X�����I���v�� */

#endif /* __TK_SYSDEF_DEPEND_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|�ysysdef_depend.h �ύX�����z
#|��2015/11/26	V1.02.04��"include/tk/sysdepend/std_sh7760/"����A
#|  [app_sh7760]�p�ɃR�s�[
#|��2015/11/26	T-Kernel V1.02.04 Software Package�uSH7760 �����d�l���v
#|  imp_sh7760.txt ��3.5�����Q�Ƃ��ăR�����g����{�ꉻ
#|
#endif
