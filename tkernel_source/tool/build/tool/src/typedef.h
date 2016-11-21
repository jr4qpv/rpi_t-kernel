/*
 *	@(#)typedef.h 02-03-09
 *
 *	標準データ型定義
 *	Copyright (C) 2002 by Personal Media Corporation
 */

#ifndef	__TYPEDEF_H__
#define	__TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 汎用的なデータタイプ
 */
typedef char		B;	/* 符号付き  8ビット整数 */
typedef short		H;	/* 符号付き 16ビット整数 */
typedef int		W;	/* 符号付き 32ビット整数 */
typedef unsigned char	UB;	/* 符号無し  8ビット整数 */
typedef unsigned short  UH;	/* 符号無し 16ビット整数 */
typedef unsigned int	UW;	/* 符号無し 32ビット整数 */

typedef char		VB;	/* 型が一定しない  8ビットのデータ */
typedef short		VH;	/* 型が一定しない 16ビットのデータ */
typedef int		VW;	/* 型が一定しない 32ビットのデータ */
typedef void		*VP;	/* 型が一定しないデータへのポインタ */

typedef volatile B	_B;	/* volatile 宣言付 */
typedef volatile H	_H;
typedef volatile W	_W;
typedef volatile UB	_UB;
typedef volatile UH	_UH;
typedef volatile UW	_UW;

typedef int		INT;	/* プロセッサのビット幅の符号付き整数 */
typedef unsigned int	UINT;	/* プロセッサのビット幅の符号無し整数 */

typedef INT		ID;	/* ID 一般 */
typedef	INT		MSEC;	/* 時間一般(ミリ秒) */

typedef void		(*FP)();	/* 関数アドレス一般 */
typedef INT		(*FUNCP)();	/* 関数アドレス一般 */

#define	LOCAL		static		/* ローカルシンボル定義 */
#define	EXPORT				/* グローバルシンボル定義 */
#define	IMPORT		extern		/* グローバルシンボル参照 */

/*
 * ブール値
 *	TRUE = 1 と定義するが、0 以外はすべて真(TRUE)である。
 *	したがって、bool == TRUE の様な判定をしてはいけない。
 *	bool != FALSE の様に判定すること。
 */
typedef INT		BOOL;
#define TRUE		1	/* 真 */
#define FALSE		0	/* 偽 */

/*
 * TRON 文字コード
 */
typedef UH		TC;		/* TRON 文字コード */
#define	TNULL		((TC)0)		/* TRON コード文字列の終端 */

#ifdef __cplusplus
}
#endif
#endif /* __TYPEDEF_H__ */
