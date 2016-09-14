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
#     Modified by T-Engine Forum at 2013/03/01.
#     Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
#
# ----------------------------------------------------------------------
#
#     Modified by T.Yokobayashi at 2016/03/14.
#
# ----------------------------------------------------------------------
#

#	@(#)T-Monitor : cmdsvc (RZT1) 2016/09/14

MACHINE = rzt1
TETYPE = app

SRC_SYSDEP = monhdr.S monent.c chkaddr.c register.c break.c \
	disassemble.c misc.c step.c

# ----------------------------------------------------------------------------

DEPS = Dependencies
DEPENDENCIES_OUTPUT := $(DEPS)

include $(BD)/etc/makerules

TMONITOR_INSTALLDIR = $(BD)/monitor/bin/$(TETYPE)_$(MACHINE)

HEADER = $(BD)/include $(BD)/monitor/include

# ----------------------------------------------------------------------------

TARGET = cmdsvc
TARGET2 = wrkbuf

S = ../../src

VPATH = $(S):$(S)/arm
HEADER += $(S)

SRC = command.c helpmsg.c svc.c memory.c \
	console.c load.c boot.c string.c
SRC += $(SRC_SYSDEP)

OBJ = $(addsuffix .o, $(basename $(SRC)))

CFLAGS += $(CFLAGS_WARNING)

# ----------------------------------------------------------------------------

.PHONY: all clean install

ALL = $(TARGET).o $(TARGET2).o

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
#|* 2016/09/14	VPATH指定をarmv6→armに変更。
#|
