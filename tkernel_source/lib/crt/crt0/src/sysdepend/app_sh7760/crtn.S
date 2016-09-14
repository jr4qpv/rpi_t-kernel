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
	lds.l	@r15+, pr
	rts
	mov.l	@r15+, r12

	.section	.fini
	lds.l	@r15+, pr
	rts
	mov.l	@r15+, r12
