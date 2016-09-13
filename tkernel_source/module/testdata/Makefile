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
#     Modified by T.Yokobayashi at 2015/12/17.
#
# ----------------------------------------------------------------------
#

#
#	@(#)Makefile (testdata) 2016/04/20
#
#	testdata
#

# T-Engine type
MACHINE = rzt1
TETYPE = app

# default rules
include ../../etc/makerules

# source file dependencies (generated automatically)
DEPS = Dependencies
DEPENDENCIES_OUTPUT := $(DEPS)

# ----------------------------------------------------------------------------

.PHONY: all clean install

ALL = testdata.mot testdata.bin

all: $(ALL) install

%.bin: %
	$(OBJCOPY) -O binary $< $@

%.mot: %
	$(OBJCOPY) $(OUTPUT_SREC) $< $@

testdata: testdata.o
	$(LINK.o) -nostdlib -T testdata$(_GCCVER).lds $^ $(OUTPUT_OPTION)

clean:
	$(RM) $(ALL)
	$(RM) testdata.o
	$(RM) $(DEPS)

install: $(addprefix $(EXE_INSTALLDIR)/, $(ALL))

# generate dependencies
$(DEPS): ; touch $(DEPS)
-include $(DEPS)


# ----------------------------------------------------------------------
#|【Makefile 変更履歴】
#|□2016/01/18	新規作成。
#| 
