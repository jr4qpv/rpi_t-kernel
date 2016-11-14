#/*
#	 Copyright (C) 2006 By eSOL Co.,Ltd. Tokyo, Japan
#
#	 This software is protected by the law and the agreement concerning
#	 a Japanese country copyright method, an international agreement,
#	 and other intellectual property right and may be used and copied
#	 only in accordance with the terms of such license and with the inclusion
#	 of the above copyright notice.
#
#	 This software or any other copies thereof may not be provided
#	 or otherwise made available to any other person.  No title to
#	 and ownership of the software is hereby transferred.
#
#	 The information in this software is subject to change without
#	 notice and should not be construed as a commitment by eSOL Co.,Ltd.
#*/
#/****************************************************************************
#[ makefile ] - MAKEFILE
#****************************************************************************/
include $(USER_HOME)/lib/lib.mk

#
# MACROS
#
OBJDIRS = sysdepend/$(USER_CPU)

OBJ_HOME = lib/libmisc/src

LIB = libmisc

SO = libmisc

SO_LINK_SOS =

ifeq ($(USER_CPU), $(CPU_ARM))
ifeq ($(USER_ARM_CL), $(ARM_CL_RVCT))
USER_RVCT_LINK_PATCH = 0
endif # ifeq ($(USER_ARM_CL), $(ARM_CL_RVCT))
endif # ifeq ($(USER_CPU), $(CPU_ARM))

#
# MAKE RULES
#
include $(MAKE_HOME)/rules.mk
