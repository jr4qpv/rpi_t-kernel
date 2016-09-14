/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/04.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2016/01/05.
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
 *	@(#)setspc.c (libtk/SH7760)
 *
 *	Address space control
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/sysdef.h>					/* for MDR_PPL */
#include "getsvcenv.h"

/*
 * Set task address space
 */
EXPORT ER SetTaskSpace( ID taskid )
{
	UW	taskmode, tmp;
	ER	ercd;

	if ( taskid == TSK_SELF ) {
		/* Set current CPL in RPL */
		taskmode = getsvcenv();
		Asm("stc r2_bank, %0": "=r"(tmp));
		tmp ^= (taskmode ^ (taskmode << 16)) & MDR_PPL(3);
		Asm("ldc %0, r2_bank": "=r"(tmp));
	} else {
		T_EIT		r_eit;
		T_CREGS		r_cregs;
		T_TSKSPC	tskspc;

		/* Get logical space/taskmode for taskid tasks */
		ercd = tk_get_reg(taskid, NULL, &r_eit, &r_cregs);
		if ( ercd < E_OK ) {
			goto err_ret;
		}

		/* Change to logical space for nominated tasks */
		tskspc.uatb = r_cregs.uatb;
		tskspc.lsid = r_cregs.lsid;
		ercd = tk_set_tsp(TSK_SELF, &tskspc);
		if ( ercd < E_OK ) {
			goto err_ret;
		}

		/* Change to PPL for nominated tasks */
		taskmode = getsvcenv();
		Asm("stc r2_bank, %0": "=r"(tmp));
		tmp ^= (taskmode ^ r_eit.mdr) & MDR_PPL(3);
		Asm("ldc %0, r2_bank": "=r"(tmp));
	}

	return E_OK;

err_ret:
	return ercd;
}


/*----------------------------------------------------------------------*/
#if 0
#|【setspc.c 変更履歴】
#|□2016/01/05	[app_sh7760]用に、[tef_em1d]の"setspc.c"を参考に作成。
#|□2016/01/05	TMF_PPL→MDR_PPLに変更。
#|  ("include/tk/sysdepend/app_sh7760\sysdef_depend.h"を参照)
#|
#endif
