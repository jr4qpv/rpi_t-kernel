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
#     Modified by T.Yokobayashi at 2016/01/12.
#
# ----------------------------------------------------------------------
#

#	T-Monitor : flash (sh7760)

MACHINE = sh7760
TETYPE = app

SRC_SYSDEP    = setup-sh4.c
SRC_WR_SYSDEP = reset-sh4.c sst_16x1.c cfi.c

# ----------------------------------------------------------------------------

DEPS = Dependencies
DEPENDENCIES_OUTPUT := $(DEPS)

include $(BD)/etc/makerules

TMONITOR_INSTALLDIR = $(BD)/monitor/bin/$(TETYPE)_$(MACHINE)

HEADER = $(BD)/include $(BD)/monitor/include

# ----------------------------------------------------------------------------

TARGET = flash

S = ../../src

VPATH = $(S)
HEADER += $(S)

SRC     = main.c
SRC    += $(SRC_SYSDEP)
SRC_WR  =
SRC_WR += $(SRC_WR_SYSDEP)

OBJ    = $(addsuffix .o, $(basename $(SRC)))
OBJ_WR = $(addsuffix .o, $(basename $(SRC_WR)))

CFLAGS += $(CFLAGS_WARNING)

# adjust for flashwr object
##REN_SECTION =	--rename-section .text=flashwr.text \
##		--rename-section .rodata=flashwr.rodata
KEEP_SYMBOL =	--keep-global-symbol=flashwr \
		--keep-global-symbol=FROM_SECSZ

# ----------------------------------------------------------------------------

.PHONY: all clean install

ALL = $(TARGET).o flashwr.o

all: $(ALL)

$(TARGET).o: $(OBJ)
	$(LINK_R.o) $^ $(OUTPUT_OPTION)

flashwr.o: $(OBJ_WR)
	$(LINK_R.o) $^ -lgcc $(OUTPUT_OPTION)
	$(OBJCOPY) $(KEEP_SYMBOL) $@

clean:
	$(RM) $(OBJ) $(OBJ_WR) flashwr.o $(ALL) $(DEPS)

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
#|【Makefile 改版履歴】
#|□2016/01/12	../tef_em1d/から、 [app_sh7760]用にコピーし、
#|  MACHINE, TETYPE を修正。
#|□2016/01/13	SH版GCC3.0.4の、OBJCOPYは「--rename-section」オプションに対応
#|  していないので、"flashwr.o"の.textと.rodataセクションをリンク側でRAMに
#|  マッピングするように修正。
#| 
