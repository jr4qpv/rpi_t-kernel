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
 */


	.section	.init
	.global		_init
_init:
	mov.l	r12, @-r15
	mova	I_GOT, r0
	mov.l	I_GOT, r12
	sts.l	pr, @-r15
	bra	I_TOP
	add	r0, r12
I_GOT:	.long	_GLOBAL_OFFSET_TABLE_
I_TOP:

	.section	.fini
	.global		_fini
_fini:
	mov.l	r12, @-r15
	mova	F_GOT, r0
	mov.l	F_GOT, r12
	sts.l	pr, @-r15
	bra	F_TOP
	add	r0, r12
F_GOT:	.long	_GLOBAL_OFFSET_TABLE_
F_TOP:
