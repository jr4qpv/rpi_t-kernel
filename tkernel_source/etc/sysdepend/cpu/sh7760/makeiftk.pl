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
#	makeiftk.pl
#
#	generate interface library for SH7760
#

sub makelib
{
	print LIB <<EndOfIfLibBody;
#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/$fn_h>

	.text
	.balign	2
	.globl	Csym(${func})
	.type	Csym(${func}), \@function
Csym(${func}):
	mov.l	fno, r0
	trapa	#TRAP_SVC
	rts
	nop

	.balign	4
fno:	.long	TFN_${Func}

EndOfIfLibBody
}

1;
