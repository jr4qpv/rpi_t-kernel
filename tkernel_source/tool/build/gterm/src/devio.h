/*
	devio.h		gterm : デバイス I/O ヘッダ

	Copyright (C) 1996-2007 by Personal Media Corporation
*/

/* 制御kコード */
#define	ESC	0x1b
#define	NL	0x0a
#define	LF	0x0a
#define	CR	0x0d
#define	BS	0x08
#define	TAB	0x09

/* コンソール(TTY) 制御コード */
#define cur_end()	TYouts("\033[@")
#define cur_up()	TYouts("\033[A")
#define cur_dwn()	TYouts("\033[B")
#define cur_fwd()	TYouts("\033[C")
#define cur_bwd()	TYouts("\033[D")
#define cur_home()	TYouts("\033[H")
#define scrn_clr()	TYout('L'-'@')
#define scrn_erase()	TYouts("\033[J")
#define line_erase()	TYouts("\033[K")
#define delm_on()	/* impossible */
#define delm_off()	/* impossible */
#define rev_on()	TYouts("\033[q")
#define rev_off()	TYouts("\033[p")
#define cur_del()	TYouts("\033[P")
#define cur_bs()	TYouts("\010\033[P")

/* キーコード */
#define	DEL		0x7f
#define	CUR_UP		0x0181
#define CUR_DWN 	0x0182
#define CUR_FWD 	0x0183
#define CUR_BWD 	0x0184
#define	CUR_UPx		0x0185
#define CUR_DWNx	0x0186
#define CUR_FWDx	0x0187
#define CUR_BWDx	0x0188
#define SCRN_CLR	'L'-'@'
#define LINE_ERASE	'K'-'@'
#define CUR_HOME	'H'-'@'
#define CANCEL		'U'-'@'
#define PREVIOUS	'A'-'@'

#define QUIT		'Q'-'@'		/* ^Q  */
#define ESCAPE		0x1b		/* ESC */
#define CMND		'E'-'@'		/* ^E -- command */
