/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)ccp_main.c (appl) 2016/11/17
 *	コマンドコンソールプロセッサ(CCP)
 */
#include <basic.h>
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <misc/libmisc.h>
#include <string.h>
#include "ccp_local.h"


/* Device drivers */
IMPORT ER ConsoleIO( INT ac, UB *av[] );

/* Command functions */
///IMPORT	void	init_calendar_date(void);
IMPORT	int	exec_cmd(char *cmd);


/*
 * Entry routine for the ccp application.
 */
EXPORT  INT ccp_main( INT schid )
{
	ER	ercd;
	char buf[256];
	INT	fin, n;

	tm_putstring((UB*)"\n<< ccp_main() start >>\n");

#ifdef __ARMCC_VERSION					/* eT-Kernel ? */
	/* Start the device drivers */
	tk_dly_tsk(100);					/* ｼﾘｱﾙ出力が完了するまで待つ */
	ercd = ConsoleIO(0, NULL);
	if (!(ercd >= E_OK)) {
		while (1) { ; };				/* Errror */
	}
#endif

	/* initialize library */
	init_libmisc();

	/* ccp start */
	P("\n");
	P("Console Command Processor.  CCP Rev0.10\n\n");

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

	cprintf("\n<<ccp exit>>\n");

#if 1
	while (1) {
		if (++n & 1)
			tm_extsvc(TMEF_LED, 0xd0002, 0, 0);	// LED1 on
		else
			tm_extsvc(TMEF_LED, 0xd0000, 0, 0);	// LED1 on

		tk_dly_tsk(100);
	}
#else
	while (1) {
		if ((++n & 1) != 0)
			tm_extsvc(TMEF_LED, 0xe0001, 0, 0);	// LED0 on
		else
			tm_extsvc(TMEF_LED, 0xe0000, 0, 0);	// LED0 off

		tk_dly_tsk(500);
	}
#endif

//	return 0;
}


/*----------------------------------------------------------------------
#|History of "ccp_main.c"
#|=======================
#|* 2016/11/17	New created.(By T.Yokobayashi)
#|  T2EXの"kernel/sysmain/src/appl_main.c"を参考に作成
#|
*/
