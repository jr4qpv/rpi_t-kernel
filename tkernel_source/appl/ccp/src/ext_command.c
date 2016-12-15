/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Package
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)ext_command.c (appl) 2016/11/17
 *
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <misc/libmisc.h>
#include <string.h>
#include "ccp_local.h"


/*------( 外部参照関数 )------*/


/*******( 定数の定義 )*******/


/*******( 外部参照変数 )*******/


/*******( 共通変数定義 )*******/
uint32_t kaddr;								/* D,Mｺﾏﾝﾄﾞ等のｱﾄﾞﾚｽ記憶用 */
int ksize;									/* D,Mｺﾏﾝﾄﾞ等の表示幅記憶用 */
int kdata;									/* MFｺﾏﾝﾄﾞ等のﾃﾞｰﾀ記憶用 */



/*============================================
       メモリダンプコマンド(db,dh,dw,d)
 =============================================*/
LOCAL void cmd_dump(int argc, char *argv[])
{
	long count;
	int loop;

	switch (argv[0][1]) {					/* 2文字目? */
	  case 'b': ksize = 0; break;			/* ﾊﾞｲﾄｻｲｽﾞ表示(8bit) */
	  case 'h': ksize = 1; break;			/* ﾊｰﾌｻｲｽﾞ表示(16bit) */
	  case 'w': ksize = 2; break;			/* ﾜｰﾄﾞｻｲｽﾞ表示(32bit) */
	  default:
		if ((ksize < 0) || (ksize > 2))
			ksize = 1;
		break;
	}
	count = 0x100;
	loop = 0;

	if (argc >= 2)							/* ｱﾄﾞﾚｽの獲得 */
		kaddr = atolhex(argv[1]);

	if (argc >= 3)							/* ｶｳﾝﾄの獲得 */
		count = atolhex(argv[2]);

	if (argc >= 4)							/* ﾙｰﾌﾟ指定獲得 */
		loop = atolhex(argv[3]);

	if (loop) {
#if 1	//////// kari ///////////
		P("loop not support\n");

 #if 0
P("push SP key--->");
while (1) {
	if (kbhit()) {
		if (getch() == ESC_CODE)
			break;						/* ESCが押された */
	}
	tk_dly_tsk(10);
}
return;
 #endif
#else	/////////////////////////
		if (loop == 1) {
			cls();
			cursor_off();
		}
		P(" *** memory dump loop ***\n");

		if (count > 0x100)
			count = 0x100;

		int ind = 0;
		while (1) {
			if (loop == 1) {
				locate(1, 1);
				if ((ind++) & 1)
					P("*\n");
				else
					P(" \n");
			}

			mem_dump(kaddr, count, ksize);
			tk_dly_tsk(500);				// 500m秒待ち
			if (kbhit_esc()) {
				cursor_on();
				return;
			}
		}
#endif
	}
	else {
		kaddr = mem_dump(kaddr, count, ksize);
	}

	return;
}


/*============================================
       メモリセットコマンド(mb,mh,mw,m)
 =============================================*/
LOCAL void cmd_mem(int argc, char *argv[])
{
	switch (argv[0][1]) {					/* 2文字目? */
	  case 'b': ksize = 0; break;			/* ﾊﾞｲﾄｻｲｽﾞ表示(8bit) */
	  case 'h': ksize = 1; break;			/* ﾊｰﾌｻｲｽﾞ表示(16bit) */
	  case 'w': ksize = 2; break;			/* ﾜｰﾄﾞｻｲｽﾞ表示(32bit) */
	  default:
		if ((ksize < 0) || (ksize > 2))
			ksize = 1;
		break;
	}

	if (argc >= 2)
		kaddr = atolhex(argv[1]);			/* ｱﾄﾞﾚｽの獲得 */

	kaddr = mem_set(kaddr, ksize);
	return;
}


/*============================================
       ダウンロードコマンド(LO,lo,load)

	LO protocol[,loading_addr]
 =============================================*/
LOCAL void cmd_load(int argc, char *argv[])
{
	int rc;
	W	i, par;
	UW	addr;
LOCAL	const	struct {
	char *nm;
	UH	par;
} proto[] = {
	{"S",  P_TEXT | P_SFORM},
	{"XS", P_XMODEM | P_SFORM},
	{"XM", P_XMODEM | P_MEMIMG},
	{"",   0x00}
};

	if (argc < 2) {
		P("ERR: Less Parameters\n");
		return;							// パラメータが少ない
	}
//P("argv[1]=%s\n", argv[1]);	/////

	// extract protocol
	par = 0;
	for (i = 0; proto[i].par != 0; i++) {
		if (strncmp(argv[1], proto[i].nm, 2) == 0) {
			par = proto[i].par;
			break;
		}
	}
	if (par == 0) {
		P("ERR: Protocol Parameters\n");
		return;							// プロトコル指定不正
	}		
//P("par=%02X\n", par);	/////

    // extract start address
	addr = 0;
	if (argc >= 3) {
		addr = atolhex(argv[2]);		// ｱﾄﾞﾚｽの獲得

	    // check the address
		if (ChkSpaceRW((void *)addr, 1)) {
			P("ERR: Illigal RW Address\n");
			return;						// アドレス指定が不正
		}
	}

	if ((par & P_MEMIMG) && (addr == 0)) {
		P("ERR: No Load Address\n");
		return;							// アドレスパラメータ不正
	}

	// execute loading
	tk_dly_tsk(100);
	rc = doLoading(par, addr, NULL);

	//////////////////////
	tk_dly_tsk(100);
	P("doLoading(par=%02X, addr=%08X)=>%d\n", par, addr, rc);	////////////
}







///////////////////// debug ////////////////////////

#if 0
/*============================================
       ファイル名表示(DIR)
 =============================================*/
LOCAL void cmd_dir(int argc, char *argv[])
{
	char *path;

	if (argc >= 2)
		path = argv[1];
	else
		path = "*.*";

	file_directory(path, 1, 0);

	return;
}
#endif


/*============================================
       ダウンロード(FLOAD)
 =============================================*/
LOCAL void cmd_fload(int argc, char *argv[])
{
///	int rc;
///	char *fn;
	long addr;

	addr = 0x8c800800;

	if (argc < 2) {
		cprintf("ERR: Less Parameters\n");
		return;							// パラメータが少ない
	}
///	fn = argv[1];

	if (argc >= 3) {
		addr = atolhex(argv[2]);		// ｱﾄﾞﾚｽの獲得
	}

	P("Loading to %08X...", addr);
#if 0	/////////////
	rc = file_read(fn, (void *)addr, 0x40000);
	P("...%d\n", rc);
#endif	/////////////

	return;
}


/*============================================
       テストコマンド(t,test)
 =============================================*/
//extern void fmem_test(void);	/////////


LOCAL void cmd_test(int argc, char *argv[])
{
	FP	fnc;
	W	p1, p2, p3;

	fnc = (FP)0x8cc00000;				/* ｱﾄﾞﾚｽの獲得 */
	if (argc >= 2)
		p1 = -1;
	else
		p1 = 0;
	p2 = 0;
	p3 = 0;
	(*fnc)(p1, p2, p3);



#if 0	///////////
///	fmem_test();
///	ltmp = _file_get_free();
///	ltmp = file_directory("*.*", 1, 0);
//	ltmp = file_read("@:testdata.bin", 0x8c800800, 0x20000);
//	ltmp = file_read("@:kernel~4.bin", 0x8c800800, 0x40000);
//	P("file_read()=>%d\n", ltmp);

///	P(">>>\n");
	while (1) {
///		if (kbhit_esc()) break;
		if (kbhit())
			putch('1');
		else
			putch('0');

		tk_dly_tsk(500);
	}

//	for (i=0; i<5; i++) {
//		c = getch();
//		putch(c);
//	}
	P("<<<\n");
#endif	///////////

	return;
}


/*============================================
      使用法の表示 (?)
 =============================================*/
LOCAL void help_cmd(int ac, char *av[])
{
	P("ref      [item]\n");
	P("d{b/h/w} [adr [cnt]] : memory Dump.\n");
	P("m{b/h/w} [adr] : Memory set.\n");
}


IMPORT void cmd_ref(int ac, char *av[]);



/*---------------------------------------------------------
	execute command
*/
EXPORT	int	exec_extcmd(int ac, char *av[])
{
	int i, n_cmd_table;

	/*** コマンドテーブル ***/
	static const struct {
		char  *cmd_str ;
		void (*cmd_func)(int ac, char *av[]) ;
    } cmd_table[] = {
#ifdef	USE_APP_EXTCMD
		{ "d",			cmd_dump		},
		{ "db",			cmd_dump		},
		{ "dh",			cmd_dump		},
		{ "dw",			cmd_dump		},

		{ "m",			cmd_mem			},
		{ "mb",			cmd_mem			},
		{ "mh",			cmd_mem			},
		{ "mw",			cmd_mem			},

		{ "LO",			cmd_load		},
		{ "lo",			cmd_load		},
		{ "load",		cmd_load		},

///		{ "dir",		cmd_dir		},
		{ "fload",		cmd_fload		},

		{ "t",			cmd_test		},
		{ "test",		cmd_test		},
#endif

		{ "ref",		cmd_ref			},

#if 0	//////////
		{ "lua",		lua_main		},
#endif	//////////
		{ "?",			help_cmd		}
	};

	/*======( コマンドのチェック )======*/
	if (ac < 1) return 0;

	/*======( コマンド行の解析＆実行 )======*/
	n_cmd_table = sizeof(cmd_table) / sizeof(cmd_table[0]) ;
	for (i=0 ; i<n_cmd_table ; i++) {
	    if (strcmp(av[0], cmd_table[i].cmd_str) == 0)
			break;
	}

	if (i < n_cmd_table) {					// ﾃｰﾌﾞﾙにあった?
		/* コマンド関数の呼び出し */
		(*(cmd_table[i].cmd_func))(ac, av);
	}
	else {									// ﾃｰﾌﾞﾙにない
		return 0;
	}

	return 1;
}


/*----------------------------------------------------------------------
#|History of "ext_command.c"
#|==========================
#|* 2016/09/12	New created.(By T.Yokobayashi)
#|
*/
