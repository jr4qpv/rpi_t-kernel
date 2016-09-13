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

#
#	makeifex.pl
#
#	generate extended SVC interface library for EM1-D512
#

sub makelibex
{
	print LIB <<EndOfExtIfLibBody;
#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/$fn_h>

	.text
	.balign	4
	.globl	Csym(${func})
	.type	Csym(${func}), %function
Csym(${func}):
	stmfd	sp!, {r0-r3}
	mov	r0, sp
	stmfd	sp!, {lr}
	ldr	ip, =${prefix}_${Func}_FN
	swi	SWI_SVC
	ldmfd	sp!, {lr}
	add	sp, sp, #4*4
	bx	lr

EndOfExtIfLibBody
}

1;
