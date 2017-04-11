/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *    Modified by T-Engine Forum at 2014/07/28.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/04.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/02/04.
 *
 *----------------------------------------------------------------------
 */
/*
 * This software package is available for use, modification, 
 * and redistribution in accordance with the terms of the attached 
 * T-License 2.x.
 * If you want to redistribute the source code, you need to attach 
 * the T-License 2.x document.
 * There's no obligation to publish the content, and no obligation 
 * to disclose it to the TRON Forum if you have modified the 
 * software package.
 * You can also distribute the modified source code. In this case, 
 * please register the modification to T-Kernel traceability service.
 * People can know the history of modifications by the service, 
 * and can be sure that the version you have inherited some 
 * modification of a particular version or not.
 *
 *    http://trace.tron.org/tk/?lang=en
 *    http://trace.tron.org/tk/?lang=ja
 *
 * As per the provisions of the T-License 2.x, TRON Forum ensures that 
 * the portion of the software that is copyrighted by Ken Sakamura or 
 * the TRON Forum does not infringe the copyrights of a third party.
 * However, it does not make any warranty other than this.
 * DISCLAIMER: TRON Forum and Ken Sakamura shall not be held
 * responsible for any consequences or damages caused directly or
 * indirectly by the use of this software package.
 *
 * The source codes in bsd_source.tar.gz in this software package are 
 * derived from NetBSD or OpenBSD and not covered under T-License 2.x.
 * They need to be changed or redistributed according to the 
 * representation of each source header.
 */

/*
 *	@(#)appl_main.c (t2ex) 2016/11/22
 *
 */
#include <basic.h>
//#include <stdlib.h>
//#include <string.h>
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include <misc/libmisc.h>


#ifdef	USE_APP_SAMPLE
IMPORT	void	sample_exec(void);
#endif
#ifdef	USE_T2EX_DT
IMPORT	void	init_calendar_date(void);
#endif

IMPORT	void cmd_proc(void);



/*
 *	Application main entry
 */
EXPORT	void	appl_main( void )
{
	/* sample task execute */
#ifdef	USE_APP_SAMPLE
	sample_exec();
#endif

	/* initialize calendar date */
#ifdef	USE_T2EX_DT
	init_calendar_date();
#endif

	/* initialize library */
	init_libmisc();

	/* command processing */
	cmd_proc();
}


/*----------------------------------------------------------------------
#|History of "appl_main.c"
#|========================
#|* 2016/09/12	"USE_T2EX_DT"未定義時は、init_calendar_date()関数は呼ばない
#|* 2016/09/12	"USE_MISC_CPRINT"定義時には、cprintf()を使うようにした。
#|  "#include <misc/libmisc.h>"の追加。
#|* 2016/11/22	init_libmisc()呼び出し処理の追加。
#|
*/
