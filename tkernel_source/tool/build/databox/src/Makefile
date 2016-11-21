#
#	@(#)Makefile (databox) 04-04-01
#		GNU make 用
#
#	データボックスコンパイラ
#	Copyright (C) 1996-2004 by Personal Media Corporation
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

# ----------------------------------------------------------------------------

# 作成対象
TARGET = databox$(HOSTEXT)

# ソースファイルのサーチパス
S = ../src
VPATH = $S

# ソースファイル
SRC =	databox.c

OBJ	= $(addsuffix .o, $(basename $(SRC)))
SRC.C	= $(filter %.C, $(SRC))

# リンクオプション変更
LDFLAGS =

# ----------------------------------------------------------------------------

.PHONY: all clean install

ALL = $(TARGET) $(addsuffix .map, $(basename $(TARGET)))

all: $(ALL)

$(TARGET): $(OBJ)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

clean:
	$(RM) $(OBJ) $(SRC.C:%.C=%.c) $(ALL) $(DEPS)

install: $(addprefix $(TOOL_INSTALLDIR)/, $(TARGET))

# ソースの依存関係
ifdef DEPENDENCIES_OUTPUT
  $(DEPS): ; touch $(DEPS)
else
  $(DEPS): $(SRC) ; $(MAKEDEPS) $@ $?
endif
include $(DEPS)

$(SRC.C:%.C=%.c):
