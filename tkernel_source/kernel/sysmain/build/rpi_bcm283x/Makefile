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
#     Modified by T-Engine Forum at 2014/07/31.
#     Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
#
# ----------------------------------------------------------------------
#
#     Modified by T.Yokobayashi at 2016/03/02.
#
# ----------------------------------------------------------------------
#

#
#	@(#)Makefile for gmake (BCM283x) 2017/04/11
#	sysmain (system-dependent description)
#

# T-Engine type
MACHINE = bcm283x
TETYPE	= rpi

# Raspberry Pi default type
TYPE_RPI = 1

# ----------------------------------------------------------------------------
# T-Kernel for Raspberry Pi version
APP_VER = 0.11

# ----------------------------------------------------------------------------
# default rules
include ../../../../etc/makerules

# ----------------------------------------------------------------------------
# rpi board name
ifeq ($(TYPE_RPI),1)
  BOARDNAME := RPi1
endif
ifeq ($(TYPE_RPI),2)
  BOARDNAME := RPi2
endif
ifeq ($(TYPE_RPI),3)
  BOARDNAME := RPi3
endif

# ----------------------------------------------------------------------------
# user source files (system-dependent)
SRC	= usermain.c

# include debug sample code in usermain.c
CFLAGS += -DDEBUG_SAMPLE
##CFLAGS += -DDEBUG_TEST		### for debug ###


# user-level libraries
LDUSRLIBS =

# ----------------------------------------------------------------------------
# device driver definitions
#DRV_CONSOLE = yes
#DRV_CLOCK   = yes
###DRV_SYSDISK = yes
###DRV_SCREEN  = yes
###DRV_KBPD    = yes
###DRV_LOWKBPD = yes

OBJ_DRIVER = ../../../../driver/$(TETYPE)_$(MACHINE)/build
ifdef DRV_CONSOLE
  I_OBJ += $(OBJ_DRIVER)/console.o
  CFLAGS += -DDRV_CONSOLE
  LDUSRLIBS += -lconsolesvc
endif
ifdef DRV_CLOCK
  I_OBJ += $(OBJ_DRIVER)/clkdrv.o
  CFLAGS += -DDRV_CLOCK
  LDUSRLIBS += -lem1diic
endif
ifdef DRV_SYSDISK
  I_OBJ += $(OBJ_DRIVER)/sysdsk.o
  CFLAGS += -DDRV_SYSDISK
endif
ifdef	DRV_SCREEN
  I_OBJ += $(OBJ_DRIVER)/screen.o
  CFLAGS += -DDRV_SCREEN
endif
ifdef	DRV_KBPD
  I_OBJ += $(OBJ_DRIVER)/kbpd.o
  CFLAGS += -DDRV_KBPD
endif
ifdef	DRV_LOWKBPD
  I_OBJ += $(OBJ_DRIVER)/lowkbpd.o
  CFLAGS += -DDRV_LOWKBPD
endif

# ----------------------------------------------------------------------------
# additional libraries
LDUSRLIBS += -ldrvif -lstr

# ----------------------------------------------------------------------------
# include main makefile (common description)
include ../Makefile.common


# ----------------------------------------------------------------------
#|History of "Makefile"
#|=====================
#|* 2016/03/02	../tef_em1d/から、 [rpi_bcm283x]用にコピーし、
#|  MACHINE, TETYPE を修正。
#|* 2016/08/08	TYPE_RPI指定を追加(1,2,3を指定)、但しﾌｫﾙﾀﾞ名による指定が優先
#|* 2016/09/12	APP_VER=0.10の追加
#|* 2017/04/11	APP_VER=0.11に変更
#|
