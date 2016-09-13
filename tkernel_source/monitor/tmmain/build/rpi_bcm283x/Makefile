#
# ----------------------------------------------------------------------
#     T-Kernel 2.0 Software Package
#
#     Copyright 2011 by Ken Sakamura.
#     This software is distributed under the latest version of T-License 2.x.
# ----------------------------------------------------------------------
#
#     Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
#     Modified by T-Engine Forum at 2011/09/08.
#     Modified by T-Engine Forum at 2012/11/07.
#     Modified by T-Engine Forum at 2013/03/01.
#     Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
#
# ----------------------------------------------------------------------
#
#     Modified by T.Yokobayashi at 2016/03/02.
#
# ----------------------------------------------------------------------
#

#	@(#)T-Monitor : tmmain (bcm283x) 2016/09/08

MACHINE = bcm283x
TETYPE = rpi

# Raspberry Pi default type
TYPE_RPI = 1

SRC_SYSDEP =

# ----------------------------------------------------------------------------

DEPS = Dependencies
DEPENDENCIES_OUTPUT := $(DEPS)

include $(BD)/etc/makerules

# install directory
TMONITOR_INSTALLDIR = $(BD)/monitor/bin/$(TETYPE)_$(MACHINE)

# h/w depend module path
HWDEPEND_PATH = $(BD)/monitor/hwdepend/$(TETYPE)_$(MACHINE)/build

# command/svc module path
CMDSVC_PATH = $(BD)/monitor/cmdsvc/build/$(TETYPE)_$(MACHINE)

# each driver module path
DRV_SIO_PATH = $(BD)/monitor/driver/sio/build/$(TETYPE)_$(MACHINE)
DRV_FLASH_PATH = $(BD)/monitor/driver/flash/build/$(TETYPE)_$(MACHINE)
DRV_MEMDISK_PATH = $(BD)/monitor/driver/memdisk/build/$(TETYPE)_$(MACHINE)

# header paths 
HEADER = $(BD)/include $(BD)/monitor/include

# ----------------------------------------------------------------------------

# vsersion for rpi
VER_RPI := 0.10

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

# version
VER := 2.01.00

# T-Monitor boot message
define VERSION
char const Version[] = "$(VER)";
char const * const Title[] = {
  "T-Monitor/$(TETYPE)_$(MACHINE) Version $(VER) for $(BOARDNAME) r$(VER_RPI)",
  0,
  "@(#)$(shell LC_TIME=C date)",
  "$(BOARDNAME)",
  "$(VER_RPI)"
};
endef

# ----------------------------------------------------------------------------

TARGET = tmonitor

S = ../../src

VPATH = $(S)
HEADER += $(S)

SRC = $(SRC_SYSDEP)

OBJ	= $(addsuffix .o, $(basename $(SRC)))

CFLAGS += $(CFLAGS_WARNING)

# ----------------------------------------------------------------------------

M_OBJ =	$(TMONITOR_INSTALLDIR)/hwdepend.o \
	$(TMONITOR_INSTALLDIR)/cmdsvc.o \
	$(TMONITOR_INSTALLDIR)/wrkbuf.o \
	$(TMONITOR_INSTALLDIR)/sio.o \
	$(TMONITOR_INSTALLDIR)/flash.o \
	$(TMONITOR_INSTALLDIR)/memdisk.o

LDLIBS = -lgcc

LDFLAGS += -static -nostdlib -T ../$(TETYPE)_$(MACHINE)/$(LINKSCRIPT)

ifneq ($(filter ram, $(options)), )
  # RAM version, for debug purpose
  LINKSCRIPT = monitor-ram.lds
else
  LINKSCRIPT = monitor.lds
endif

# ----------------------------------------------------------------------------

.PHONY: all clean install

ALL = $(TARGET).bin $(TARGET).mot $(TARGET).map

all: hwdepend cmdsvc drivers $(ALL) install

$(TARGET).bin:	$(TARGET)
	$(OBJCOPY) -O binary $< $@

$(TARGET).mot:	$(TARGET)
	$(OBJCOPY) -O srec --srec-forceS3 --srec-len 32 $< $@

$(TARGET): $(M_OBJ) $(OBJ) version.o
	$(CC) $(TARGET_ARCH) $(LDFLAGS) $^ $(LDLIBS) $(OUTPUT_OPTION)

version.c: Makefile $(OBJ) $(M_OBJ)
	echo "$$VERSION" > version.c

hwdepend:
	( cd $(HWDEPEND_PATH) ; $(MAKE) ; $(MAKE) install ; )

cmdsvc:
	( cd $(CMDSVC_PATH) ; $(MAKE) ; $(MAKE) install ; )

drivers:
	( cd $(DRV_SIO_PATH) ; $(MAKE) ; $(MAKE) install ; )
	( cd $(DRV_FLASH_PATH) ; $(MAKE) ; $(MAKE) install ; )
	( cd $(DRV_MEMDISK_PATH) ; $(MAKE) ; $(MAKE) install ; )

clean:
	( cd $(HWDEPEND_PATH) ; $(MAKE) clean )
	( cd $(CMDSVC_PATH) ; $(MAKE) clean )
	( cd $(DRV_SIO_PATH) ; $(MAKE) clean )
	( cd $(DRV_FLASH_PATH) ; $(MAKE) clean )
	( cd $(DRV_MEMDISK_PATH) ; $(MAKE) clean )
	$(RM) version.[co] $(OBJ) $(M_OBJ) $(TARGET) $(ALL) $(DEPS)

install: $(addprefix $(EXE_INSTALLDIR)/, $(ALL))

ifdef DEPENDENCIES_OUTPUT
  $(DEPS): ; touch $(DEPS)
else
  $(DEPS): $(SRC) ; $(MAKEDEPS) $@ $?
endif
-include $(DEPS)

$(SRC.C):


# ----------------------------------------------------------------------
#|History of "Makefile"
#|=====================
#|* 2016/03/02	../tef_em1d/から、 [rpi_bcm283x]用にコピーし、
#|  MACHINE, TETYPE を修正。
#|* 2016/03/02	ﾘﾝｸｽｸﾘﾌﾟﾄの拡張子は｢.lnk｣→｢.lds｣に変更｡
#|  (Windowsでは｢.lnk｣は特殊ﾌｧｲﾙの為)
#|* 2016/03/02	.binファイルを生成するようにルールの追加。
#|* 2016/04/11	debugﾌｫﾙﾀﾞとmakefileとﾘﾝｸｽｸﾘﾌﾟﾄが共通にできるように、
#|  ﾘﾝｸｽｸﾘﾌﾟﾄに"../$(TETYPE)_$(MACHINE)/"のパス記述追加
#|* 2016/04/20	Title[0]にBOARDNAME文字列と、Title[3]にBOARDNAME文字列の追加
#|* 2016/08/08	TYPE_RPI指定を追加(1,2,3を指定)、但しﾌｫﾙﾀﾞ名による指定が優先
#|* 2016/09/08	VER_RPI定義の追加し、起動時表示するようにTitle[0]に含めた
#|* 2016/09/08	Title[4]にVER_RPI文字列の追加
#|
