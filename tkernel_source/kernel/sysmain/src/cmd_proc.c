/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Package
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)cmd_proc.c (t2ex) 2016/12/08
 *
 */

#include <basic.h>
#include <string.h>
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include <misc/libmisc.h>
#include "cmd_local.h"


#define	N_ARGS		16


/* Command functions */
IMPORT	INT	exec_cmd(INT ac, B *av[]);
IMPORT	int	exec_extcmd(int ac, char *av[]);



/*
	setup parameters
*/
LOCAL	INT	setup_param(B *bp, B **av)
{
	INT	ac;

	for (ac = 0; ac < N_ARGS; ac++) {
		while (*((UB*)bp) <= ' ' && *bp != '\0') bp++;	// 先頭の文字以外削除
		if (*bp == '\0') break;
		av[ac] = bp;
		while (*((UB*)bp) > ' ' && *bp != ',') bp++;	// 区切りに','も含める
		if (*bp != '\0') {
			*bp++ = '\0';
		}
	}
	av[ac] = NULL;
	return ac;
}


/*
 *	Cmmand processing
 */
void cmd_proc(void)
{
	B	buf[256];
	INT	fin, n;
	INT	ac;
	B	*av[N_ARGS];

	for (fin = 0; fin == 0; ) {
		P(CMD_PROMPT);
		Gets(buf, sizeof(buf));
		for (n = strlen((char*)buf); --n >= 0 && buf[n] <= ' '; ) 
			buf[n] = '\0';

		if (strncmp((char*)buf, "quit", 1) == 0) {
			fin = 1;

		/* t-monitor */
		} else if (strncmp((char*)buf, "#", 1) == 0) {
			tm_command((UB*)&buf[1]);

		/* misc. command */
		} else {
			/* コマンドの抽出 */
			ac = setup_param(buf, av);
			if (ac < 1) {
				P("q[uit]      quit\n");
				P("# [cmd]     exec t-monitor command\n");
				P("?           command help\n");
				P("<command>   misc. command\n");
			}
			else if (av[0][0] == '?') {
				/* ヘルプ表示 */
				exec_cmd(ac, av);
				exec_extcmd(ac, (char**)av);
			}
			else {
				/* コマンド実行 */
				if (exec_cmd(ac, av) == 0) {
					exec_extcmd(ac, (char**)av);
				}
			}
		}
	}
}


/*----------------------------------------------------------------------
#|History of "cmd_proc.c"
#|=======================
#|* 2016/12/08	New created.(By T.Yokobayashi)
#|  T2EXの"kernel/sysmain/src/appl_main.c"を参考に作成
#|* 2016/12/08	setup_param()で区切文字に空白の他に','文字も見るようにした。
#|
*/
