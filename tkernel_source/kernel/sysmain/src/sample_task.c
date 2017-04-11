/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Package
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sample_task.c (t2ex) 2016/12/08
 *
 */

#include <basic.h>
#include <stdio.h>
#include <sys/stat.h>					/* for stat64 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <t2ex/string.h>
#include <misc/libmisc.h>
#include "cmd_local.h"



/*
 *  sample - task
 */
LOCAL void sample_tsk( INT stacd, void *exinf )
{
	tk_dly_tsk(100);
	P("3 - Sample task started (tid=%d).\n", tk_get_tid());

	while (1) {
		tm_extsvc(TMEF_LED, 0x00, 0, 0);		// LED off
		tk_dly_tsk(250);
		tm_extsvc(TMEF_LED, 0x01, 0, 0);		// LED on
		tk_dly_tsk(250);
	}
}

/*
 *  sample task regist
 */
EXPORT	void	sample_exec(void)
{
	ER	er;
	ID	ttskid;
	T_CTSK	ctsk;

	P("1 - sample started.\n");

	/* Create task */
	memset(&ctsk, 0, sizeof(ctsk));

	ctsk.tskatr = TA_HLNG | TA_RNG0;	/* Task attribute		*/
	ctsk.task = sample_tsk;				/* Task startup address		*/
	ctsk.itskpri = 140;					/* Priority at task startup	*/
	ctsk.stksz = 2 * 1024;				/* User stack size (byte)	*/

	ttskid = tk_cre_tsk(&ctsk);
	if ( ttskid < E_OK ) {
		P("ERROR: tk_cre_tsk [%#x].\n", ttskid);
		goto err_ret;
	}

	/* Start task */
	er = tk_sta_tsk(ttskid, tk_get_tid());
	if ( er < E_OK ) {
		tk_del_tsk(ttskid);
		P("ERROR: tk_sta_tsk [%#x].\n", er);
		goto err_ret;
	}

	P("2 - Start sample task (tid=%d) and wait.\n", ttskid);
	tk_dly_tsk(100);

err_ret:
	return;
}


#ifndef	USE_T2EX_FS	/* ///リンクエラーを回避する為のダミー関数定義/// */
EXPORT	ER	fs_close(int fd) { return 0; }
EXPORT	int	fs_read(int fd, void *buf, size_t count) { return 0; }
EXPORT	int	fs_write(int fd, const void *buf, size_t count) { return 0; }
EXPORT	off64_t	fs_lseek64(int fd, off64_t offset64, int whence) { return 0; }
EXPORT	ER	fs_fstat64(int fd, struct stat64 *buf64) { return 0; }
#endif


/*----------------------------------------------------------------------
#|History of "sample_task.c"
#|=======================
#|* 2016/09/12	新規作成(By T.Yokobayashi)
#|
*/
