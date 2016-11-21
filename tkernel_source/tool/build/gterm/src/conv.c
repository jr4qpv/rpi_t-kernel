/*
	conv.c		gterm : 文字コード変換処理

	Copyright (C) 2007 by Personal Media Corporation
*/
#include <stdio.h>
#include "gterm.h"

/*
	SHIFT JIS -> EUC 変換
*/
void	sjtoeuc(int *ph, int *pl)
{
	if (*ph <= 0x9f) {
		if (*pl < 0x9f) {
			*ph = (*ph << 1) - 0xe1;
		} else {
			*ph = (*ph << 1) - 0xe0;
		}
	} else {
		if (*pl < 0x9f) {
			*ph = (*ph << 1) - 0x161;
		} else {
			*ph = (*ph << 1) - 0x160;
		}
	}
	if (*pl < 0x7f) {
		*pl -= 0x1f;
	} else if (*pl < 0x9f) {
		*pl -= 0x20;
	} else {
		*pl -= 0x7e;
	}
	*ph |= 0x80;
	*pl |= 0x80;
}

/*
	EUC -> SHIFT JIS 変換
*/
void	euctosj(int *ph, int *pl)
{
	*ph &= 0x7f;
	*pl &= 0x7f;
	if (*ph & 1) {
		if (*pl < 0x60) {
			*pl += 0x1f;
		} else {
			*pl += 0x20;
		}
	} else {
		*pl += 0x7e;
	}
	if (*ph < 0x5f) {
		*ph = (*ph + 0xe1) >> 1;
	} else {
		*ph = (*ph + 0x161) >> 1;
	}
}
