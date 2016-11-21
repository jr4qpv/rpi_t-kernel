#
#	@(#)Makefile (tools) 04-02-10
#		GNU make 用
#
#	小さなツール類
#	Copyright (C) 1997-2004 by Personal Media Corporation
#
#	make の方法
#	・make
#		リリース用の正式版を作成
#	・make mode=debug
#		デバッグ用に作成
#	・make clean
#		make で作成した全てのファイルを削除
#	・make install
#		規定の場所にインストール
#

# ソースの依存関係ファイル (自動生成)
DEPS = Dependencies
DEPENDENCIES_OUTPUT := $(DEPS)

# 標準ルール
include ../../etc/makerules

#-----------------------------------------------------------------------------

# 作成対象
TARGET = bzcomp mkbtf

# ソースファイルのサーチパス
S = ../src
VPATH = $S

# ソースファイル
SRC := $(notdir $(wildcard $S/*.[Cc]))

OBJ	= $(addsuffix .o, $(basename $(SRC)))
SRC.C	= $(strip $(patsubst %.C, %.c, $(filter %.C, $(SRC))))

HEADER := $(S) $(HEADER)

#-----------------------------------------------------------------------------

.PHONY: all install clean

ifneq ($(HOSTEXT), )
  TARGET := $(addsuffix $(HOSTEXT), $(TARGET))
endif

ALL = $(TARGET) $(addsuffix .map, $(basename $(TARGET)))
INST = $(TARGET)

all: $(ALL)

bzcomp$(HOSTEXT): bzcomp.o comp.o

clean:
	$(RM) $(OBJ) $(SRC.C) $(ALL) $(DEPS)

install: $(addprefix $(TOOL_INSTALLDIR)/, $(INST))


# ソースの依存関係
ifdef DEPENDENCIES_OUTPUT
  $(DEPS): ; touch $(DEPS)
else
  $(DEPS): $(SRC) ; $(MAKEDEPS) $@ $?
endif
include $(DEPS)

$(SRC.C):
$(OBJ):
