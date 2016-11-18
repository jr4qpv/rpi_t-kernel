/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Library
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)ccp_usermain.c (appl) 2016/11/17
 *	コマンドコンソールプロセッサ(CCP)
 */
#include <basic.h>
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <misc/libmisc.h>
#include <string.h>
#include "ccp_local.h"

#ifdef __ARMCC_VERSION					/* eT-Kernel ? */
 #include <sys/ssid.h>					/* for SERIAL_SVC */
#endif


/* Device drivers */
IMPORT ER SerialIO( INT ac, UB *av[] );

/* Command functions */
///IMPORT	void	init_calendar_date(void);
IMPORT	int	exec_cmd(char *cmd);


/*
 *  CCP - task
 */
LOCAL void ccp_tsk( INT stacd, void *exinf )
{
	char buf[256];
	INT	fin, n;

	P("\n\n");
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
	while (1) {
		if (++n & 1)
			tm_extsvc(TMEF_LED, 0xd0002, 0, 0);	// LED1 on
		else
			tm_extsvc(TMEF_LED, 0xd0000, 0, 0);	// LED1 on

		tk_dly_tsk(100);
	}
}


/*
 * Entry routine for the ccp application.
 */
EXPORT  INT ccp_usermain( INT schid )
{
	ER	ercd;
	ID	ttskid;
	T_CTSK	ctsk;
	INT i;

#if 0
	tm_putstring((UB*)"\n");
	for (i=0; i<10; i++) {
//		tm_putstring((UB*)"*** start ***\n");
		tm_putstring((UB*)"*** rzt1_usermain() start ***\n");
//		tm_putchar('a');
//		tm_printf("*** start %d ***\n", i);
		tk_dly_tsk(500);
	}
	tm_putstring((UB*)"\n");
#else
	tm_putstring((UB*)"\n*** ccp_usermain() start ***\n\n");
#endif


#ifdef __ARMCC_VERSION					/* eT-Kernel ? */
	/* Start the device drivers */
	ercd = SerialIO(0, NULL);
	if (!(ercd >= E_OK)) {
		while (1) { ; };				/* Errror */
	}

	/* Open Serial device drivers */
	i = tm_extsvc(TMEF_PORTNO, 0, 0, 0);	/* get T-Monitor port number */
	ercd = tk_sta_ssy(SERIAL_SVC, i, NULL);
	if (ercd != E_OK) {
		while (1) { ; };				/* Errror */
	}
#endif

	/* initialize library */
	init_libmisc();

	/* Create task */
	memset(&ctsk, 0, sizeof(ctsk));

	ctsk.tskatr = TA_HLNG | TA_RNG0;	/* Task attribute		*/
	ctsk.task = ccp_tsk;				/* Task startup address		*/
	ctsk.itskpri = 140;					/* Priority at task startup	*/
	ctsk.stksz = 2 * 1024;				/* User stack size (byte)	*/

	ttskid = tk_cre_tsk(&ctsk);
	if ( ttskid < E_OK ) {
		goto err_ret;
	}

	/* Start task */
	ercd = tk_sta_tsk(ttskid, tk_get_tid());
	if ( ercd < E_OK ) {
		goto err_ret;
	}

err_ret:

#if 1
	i = 0;
	while (1) {
		if ((++i & 1) != 0)
			tm_extsvc(TMEF_LED, 0xe0001, 0, 0);	// LED0 on
		else
			tm_extsvc(TMEF_LED, 0xe0000, 0, 0);	// LED0 off

		tk_dly_tsk(500);
	}
#else
	return 0;
#endif
}


/*----------------------------------------------------------------------
#|History of "ccp_usermain.c"
#|===========================
#|* 2016/11/17	New created.(By T.Yokobayashi)
#|  T2EXの"kernel/sysmain/src/appl_main.c"を参考に作成
#|
*/
