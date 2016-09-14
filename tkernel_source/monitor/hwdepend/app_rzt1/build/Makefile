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
#     Modified by T-Engine Forum at 2013/02/20.
#     Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
#
# ----------------------------------------------------------------------
#
#     Modified by T.Yokobayashi at 2016/03/14.
#
# ----------------------------------------------------------------------
#

#     @(#)T-Monitor : hwdepend (RZT1) 2016/04/22

MACHINE = rzt1
TETYPE = app

SRC_SYSDEP = reset_main.c \
	eitproc.c cpuctrl.c misc.c \
	eitent.S reset.S system.c waitusec.c config.c \
	cpuctrl2.S memattr.S

SRC_COMMON = loader_param.c loader_init.S loader_init2.c exit.c \
	r_atcm_init.c r_cpg.c r_ecm.c r_icu_init.c \
	r_mpc.c r_ram_init.c r_reset.c \
	bus_init_serial_boot.c spibsc_flash_api.c spibsc_flash_userdef.c \
	spibsc_ioset_api.c spibsc_ioset_drv.c spibsc_ioset_userdef.c

##	bus_init_nor_boot.c

# ----------------------------------------------------------------------------

DEPS = Dependencies
DEPENDENCIES_OUTPUT := $(DEPS)

include $(BD)/etc/makerules

TMONITOR_INSTALLDIR = $(BD)/monitor/bin/$(TETYPE)_$(MACHINE)

HEADER = $(BD)/include $(BD)/monitor/include

# ----------------------------------------------------------------------------

TARGET = hwdepend

S = ../src

VPATH = $(S) $(S)/common
VPATH += $(S)/common/sflash_boot

##VPATH += $(S)/common/nor_boot

HEADER += $(S) $(BD)/include/misc/renesas/rzt1

SRC = startup.c hwinfo.c sio.c diskio.c
SRC += $(SRC_SYSDEP)
SRC += $(SRC_COMMON)

OBJ = $(addsuffix .o, $(basename $(SRC)))

CFLAGS += $(CFLAGS_WARNING)

# ----------------------------------------------------------------------------

.PHONY: all clean install

ALL = $(TARGET).o

all: $(ALL)

$(TARGET).o: $(OBJ)
	$(LINK_R.o) $^ $(OUTPUT_OPTION)

clean:
	$(RM) $(OBJ) $(ALL) $(DEPS)

install: $(addprefix $(TMONITOR_INSTALLDIR)/, $(ALL))

$(TMONITOR_INSTALLDIR)/%: %
	$(BD)/etc/backup_copy -t -d !OLD $< $(TMONITOR_INSTALLDIR)

ifdef DEPENDENCIES_OUTPUT
  $(DEPS): ; touch $(DEPS)
else
  $(DEPS): $(SRC) ; $(MAKEDEPS) $@ $?
endif
-include $(DEPS)


# ----------------------------------------------------------------------
#|History of "Makefile"
#|=====================
#|* 2016/03/14	../tef_em1d/から、 [app_rzt1]用にコピーし、
#|  MACHINE, TETYPE を修正。
#|* 2016/03/30	Renesas RZ/T1の初期設定ｻﾝﾌﾟﾙｺｰﾄﾞ SRC_COMMON /common 関係を追加
#|* 2016/04/21	RZ/T1関係のinluceﾌｧｲﾙを"$(BD)/include/misc/rzt1"ﾌｫﾙﾀﾞに移動
#|* 2016/04/22	reset_main.cの追加
#|
