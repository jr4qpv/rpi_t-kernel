#
# ----------------------------------------------------------------------
#     T-Kernel 2.0 Software Package
#
#     Copyright 2011 by Ken Sakamura.
#     This software is distributed under the latest version of T-License 2.x.
# ----------------------------------------------------------------------
#
#     Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
#     Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
#
# ----------------------------------------------------------------------
#
#     Modified by T.Yokobayashi at 2015/12/22.
#
# ----------------------------------------------------------------------
#

#
#	Makefile for gmake
#	make all drivers for app_sh7760
#

DRV_TARGET = clk console
DRV_SRC_TARGET = console
###DRV_TARGET = clk console kbpd lowkbpd screen sysdsk

# ----------------------------------------------------------------------------
.PHONY: all clean source clean_source clean_all

all: $(DRV_TARGET)

$(DRV_TARGET):
	cd ../$@/build ; \
	$(MAKE) ; \
	$(MAKE) install

clean:
	@( cd ../ ; \
	for dir in $(DRV_TARGET); do \
		( cd $$dir/build ; $(MAKE) clean clean_install ) ; \
	done ) ;

source:
	@( cd ../ ; \
	for dir in $(DRV_SRC_TARGET); do \
		( cd $$dir/build ; $(MAKE) source ) ; \
	done ) ;

clean_source:
	@( cd ../ ; \
	for dir in $(DRV_SRC_TARGET); do \
		( cd $$dir/build ; $(MAKE) clean_source ) ; \
	done ) ;

clean_all: clean clean_source


# ----------------------------------------------------------------------
#|【Makefile 変更履歴】
#|□2015/12/22	../../tef_em1d/から、 [app_sh7760]用にコピーし修正。
#|
