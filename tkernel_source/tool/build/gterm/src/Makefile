#
#	@(#)Makefile (gterm) 09-03-26
#		GNU make 用
#
#	Gterm ターミナルエミュレータ
#	(C) Copyright 1996-2009 by Personal Media Corporation
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

# 共通 標準ルール
ifdef BD
  include $(BD)/tool/build/etc/makerules
else
  include ../../etc/makerules
endif

# ----------------------------------------------------------------------------

# 作成対象
TARGET = gterm$(HOSTEXT)

# ソースファイルのサーチパス
S = ../src
VPATH = $S
HEADER += $S

# ソースファイル
SRC = gterm.c devio.c load.c sym.c conv.c

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

install: regist_version $(addprefix $(TOOL_INSTALLDIR)/, $(TARGET))

regist_version: $(TARGET)
	@if [ -d .versions ]; then \
		ver=`strings $< | awk -e '/Gterm ver/ {print $$4}'`; \
		if [ -f .versions/$$ver ]; then \
			echo "Error: version $$ver has been already installed. Bump gterm.c:START_MSG."; exit 1; \
		fi; \
		touch .versions/$$ver; \
	fi

# ソースの依存関係
ifdef DEPENDENCIES_OUTPUT
  $(DEPS): ; touch $(DEPS)
else
  $(DEPS): $(SRC) ; $(MAKEDEPS) $@ $?
endif
-include $(DEPS)

$(SRC.C:%.C=%.c):
