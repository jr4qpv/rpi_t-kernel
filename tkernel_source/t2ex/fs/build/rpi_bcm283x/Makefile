#
# ----------------------------------------------------------------------
#    T2EX Software Package
#
#    Copyright 2012 by Ken Sakamura.
#    This software is distributed under the latest version of T-License 2.x.
# ----------------------------------------------------------------------
#
#    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
#    Modified by T-Engine Forum at 2013/02/20.
#    Modified by TRON Forum(http://www.tron.org/) at 2015/06/04.
#
# ----------------------------------------------------------------------
#
#     Modified by T.Yokobayashi at 2016/03/02.
#
# ----------------------------------------------------------------------
#

#
# This software package is available for use, modification, 
# and redistribution in accordance with the terms of the attached 
# T-License 2.x.
# If you want to redistribute the source code, you need to attach 
# the T-License 2.x document.
# There's no obligation to publish the content, and no obligation 
# to disclose it to the TRON Forum if you have modified the 
# software package.
# You can also distribute the modified source code. In this case, 
# please register the modification to T-Kernel traceability service.
# People can know the history of modifications by the service, 
# and can be sure that the version you have inherited some 
# modification of a particular version or not.
#
#    http://trace.tron.org/tk/?lang=en
#    http://trace.tron.org/tk/?lang=ja
#
# As per the provisions of the T-License 2.x, TRON Forum ensures that 
# the portion of the software that is copyrighted by Ken Sakamura or 
# the TRON Forum does not infringe the copyrights of a third party.
# However, it does not make any warranty other than this.
# DISCLAIMER: TRON Forum and Ken Sakamura shall not be held
# responsible for any consequences or damages caused directly or
# indirectly by the use of this software package.
#
# The source codes in bsd_source.tar.gz in this software package are 
# derived from NetBSD or OpenBSD and not covered under T-License 2.x.
# They need to be changed or redistributed according to the 
# representation of each source header.
#

#
#	@(#)Makefile
#
#

#	T2EX: file system functions

# T-Engine type
MACHINE = bcm283x
TETYPE	= rpi

BUILD_PATH = build/$(TETYPE)_$(MACHINE)

# ----------------------------------------------------------------------------

.PHONY: all clean install source clean_source clean_install

all:
	( cd ../../fs_if/$(BUILD_PATH) ; $(MAKE) )
	( cd ../../fs/$(BUILD_PATH) ; $(MAKE) )
	( cd ../../fimp/$(BUILD_PATH) ; $(MAKE) )

clean:
	( cd ../../fs_if/$(BUILD_PATH) ; $(MAKE) clean )
	( cd ../../fs/$(BUILD_PATH) ; $(MAKE) clean )
	( cd ../../fimp/$(BUILD_PATH) ; $(MAKE) clean )

install:
	( cd ../../fs_if/$(BUILD_PATH) ; $(MAKE) install )
	( cd ../../fs/$(BUILD_PATH) ; $(MAKE) install )
	( cd ../../fimp/$(BUILD_PATH) ; $(MAKE) install )

clean_install:
	( cd ../../fs_if/$(BUILD_PATH) ; $(MAKE) clean_install )
	( cd ../../fs/$(BUILD_PATH) ; $(MAKE) clean_install )
	( cd ../../fimp/$(BUILD_PATH) ; $(MAKE) clean_install )

source:
	( cd ../../fs_if/$(BUILD_PATH) ; $(MAKE) source )

clean_source:
	( cd ../../fs_if/$(BUILD_PATH) ; $(MAKE) clean_source )


# ----------------------------------------------------------------------
#|History of "Makefile"
#|=====================
#|* 2016/03/02	../tef_em1d/から、 [rpi_bcm283x]用にコピーし、
#|  MACHINE, TETYPE を修正。
#|
