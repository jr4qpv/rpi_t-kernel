#! /usr/local/bin/perl
#
# ----------------------------------------------------------------------
#     T-Kernel
#
#     Copyright (C) 2004-2008 by Ken Sakamura. All rights reserved.
#     T-Kernel is distributed under the T-License.
# ----------------------------------------------------------------------
#
#     Version:   1.02.04
#     Released by T-Engine Forum(http://www.t-engine.org) at 2008/02/29.
#
# ----------------------------------------------------------------------
#

#
#	makeifex.pl
#
#	generate extended SVC interface library for SH7760
#

sub makelibex
{
	print LIB <<EndOfExtIfLibBody;
#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/$fn_h>

	.text
	.balign	2
	.globl	Csym(${func})
	.type	Csym(${func}), \@function
Csym(${func}):
	mov.l	r7, \@-r15
	mov.l	r6, \@-r15
	mov.l	r5, \@-r15
	mov.l	r4, \@-r15
	mov.l	fno, r0
	mov	r15, r4
	trapa	#TRAP_SVC
	rts
	add	#4*4, r15

	.balign	4
fno:	.long	${prefix}_${Func}_FN

EndOfExtIfLibBody
}

1;
