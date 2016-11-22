/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)ccp_main.c (appl) 2016/11/22
 *	コマンドコンソールプロセッサ(CCP)
 */
#include <basic.h>
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <misc/libmisc.h>
#include <string.h>
#include "ccp_local.h"


/* Command functions */
IMPORT	int	exec_cmd(char *cmd);


/*
 * Entry routine for the ccp application.
 */
EXPORT  int ccp_main( int flag )
{
	char buf[256];
	INT	fin, n;

	/* ccp start */
	P("\nConsole Command Processor.  CCP Rev0.10\n\n");

	/* command processing */
	for (fin = 0; fin == 0; ) {
		P("CCP> ");
		Gets(buf, sizeof(buf));
		for (n = strlen(buf); --n >= 0 && buf[n] <= ' '; ) 
			buf[n] = '\0';

		if (strncmp(buf, "quit", 1) == 0) {
			fin = 1;

		/* t-monitor */
		} else if (strncmp(buf, "#", 1) == 0) {
			tm_command((UB*)&buf[1]);

		/* misc. command */
		} else {
			if (exec_cmd(buf) == 0) {
				P("q[uit]      quit\n");
				P("# [cmd]     exec t-monitor command\n");
				P("?           command help\n");
				P("<command>   misc. command\n");
			}
		}
	}

	return 0;
}


/*----------------------------------------------------------------------
#|History of "ccp_main.c"
#|=======================
#|* 2016/11/17	New created.(By T.Yokobayashi)
#|  T2EXの"kernel/sysmain/src/appl_main.c"を参考に作成
#|
*/
