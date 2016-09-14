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
#     Modified by T.Yokobayashi at 2016/03/14.
#
# ----------------------------------------------------------------------
#

#
#	@(#)Makefile () 2016/03/16
#		for GNU make
#
#	device driver : sysdsk (RZT1)

MACHINE = rzt1
TETYPE = app

# ----------------------------------------------------------------------------

DEPS = Dependencies
DEPENDENCIES_OUTPUT := $(DEPS)

include $(BD)/etc/makerules

HEADER = $(BD)/include

# ----------------------------------------------------------------------------

TARGET = sysdsk

S = ../src

VPATH = $(S)
HEADER += $(S)

SRC = main.c accept.c common.c inthdr.c pccard.c \
	ata.c atacmd.c ataio.c mem.c

###SOBJ =  ../obj/mmc-common.o ../obj/mmc-em1d512.o
SOBJ =
OBJ = $(addsuffix .o, $(basename $(SRC)))

CFLAGS += -DUSE_MEMDRV -DUSE_MMCDRV
CFLAGS += -DUSE_MMC_MULTI -DUSE_MMC_SD4BIT

CFLAGS += $(CFLAGS_WARNING)

# ----------------------------------------------------------------------------

.PHONY: all clean install clean_install

ALL = $(TARGET).o

all: $(ALL)

$(TARGET).o: $(OBJ)
	$(LINK_R.o) $^ $(SOBJ) $(OUTPUT_OPTION)

clean:
	$(RM) $(OBJ) $(ALL) $(DEPS)

install: $(addprefix $(DRIVER_INSTALLDIR)/, $(ALL))

clean_install:
	$(RM) $(addprefix $(DRIVER_INSTALLDIR)/, $(ALL))

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
#|
