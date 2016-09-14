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
	sts.l	pr, @-r15
	mov	#0, r12

	.section	.fini
	.global		_fini
_fini:
	mov.l	r12, @-r15
	sts.l	pr, @-r15
	mov	#0, r12
