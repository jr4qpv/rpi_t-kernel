#! /usr/bin/perl
#
# ----------------------------------------------------------------------
#     T-Kernel 2.0 Software Package
#
#     Copyright 2011 by Ken Sakamura.
#     This software is distributed under the latest version of T-License 2.x.
# ----------------------------------------------------------------------
#
#     Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
#     Modified by T-Engine Forum at 2014/07/14.
#     Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
#
# ----------------------------------------------------------------------
#
#     Modified by T.Yokobayashi at 2015/12/25.
#
# ----------------------------------------------------------------------
#


#
#	makeifdrv.pl
#
#	generate driver interface library for SH7760
#

sub makelibdrv
{
	print LIB <<EndOfExtIfLibBody;
#include <machine.h>
#include <tk/sysdef.h>
#include "$fn_h"

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
