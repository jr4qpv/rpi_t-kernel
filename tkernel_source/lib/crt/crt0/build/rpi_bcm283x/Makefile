#
# ----------------------------------------------------------------------
#     T-Kernel 2.0 Software Package
#
#     Copyright 2011 by Ken Sakamura.
#     This software is distributed under the latest version of T-License 2.x.
# ----------------------------------------------------------------------
#
#     Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
#     Modified by T-Engine Forum at 2012/11/07.
#     Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
#
# ----------------------------------------------------------------------
#
#     Modified by T.Yokobayashi at 2016/03/02.
#
# ----------------------------------------------------------------------
#

#
#	Makefile for gmake
#	crt0 (system-dependent description)
#

# T-Engine type
MACHINE = bcm283x
TETYPE = rpi

# default rules
include ../../../../../etc/makerules

# target files
TARGET = crt0.o crt1f.o crt1s.o crti.o crtir.o crtn.o

# source files
SRC = $(TARGET:%.o=%.S)

# include main makefile (common description)
include ../../src/Makefile.common


# ----------------------------------------------------------------------
#|History of "Makefile"
#|=====================
#|* 2016/03/02	../tef_em1d/から、 [rpi_bcm283x]用にコピーし、
#|  MACHINE, TETYPE を修正。
#|
