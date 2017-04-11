#/*
#    Copyright (C) 2007 By eSOL Co.,Ltd. Tokyo, Japan
#
#    This software is protected by the law and the agreement concerning
#    a Japanese country copyright method, an international agreement,
#    and other intellectual property right and may be used and copied
#    only in accordance with the terms of such license and with the inclusion
#    of the above copyright notice.
#
#    This software or any other copies thereof may not be provided
#    or otherwise made available to any other person.  No title to
#    and ownership of the software is hereby transferred.
#
#    The information in this software is subject to change without
#    notice and should not be construed as a commitment by eSOL Co.,Ltd.
#*/
#/****************************************************************************
#[ appl.mk ] - eT-Kernel application local make rules
#****************************************************************************/
ifndef $(APPL_MK)

APPL_MK = 1

include $(USER_HOME)/make/macros.mk

APPL_HOME = $(PARTS_HOME)/appl

endif # ifndef $(APPL_MK)
