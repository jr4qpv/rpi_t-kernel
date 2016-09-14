/*
 *----------------------------------------------------------------------
 *    T-Kernel
 *
 *    Copyright (C) 2004-2008 by Ken Sakamura. All rights reserved.
 *    T-Kernel is distributed under the T-License.
 *----------------------------------------------------------------------
 *
 *    Version:   1.02.04
 *    Released by T-Engine Forum(http://www.t-engine.org) at 2008/02/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)setspc.c (libtk/SH7760)
 *
 *	Address space control 
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/sysdef.h>

#include "getsvcenv.h"

/*
 * Set task address space 
 */
EXPORT ER SetTaskSpace( ID taskid )
{
	UW	mdr;
	ER	ercd;

	if ( taskid == TSK_SELF ) {
		/* Set current CPL in MDR.RPL */
		mdr = getsvcenv();
		mdr ^= (mdr ^ (mdr << 16)) & MDR_PPL(3);
		Asm("ldc %0, r2_bank" :: "r"(mdr));
	} else {
		T_EIT		r_eit;
		T_CREGS		r_cregs;
		T_TSKSPC	tskspc;

		/* Get logical space/MDR register for taskid tasks */
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

		/* Change to MDR.PPL for nominated tasks */
		mdr = getsvcenv();
		mdr ^= (mdr ^ r_eit.mdr) & MDR_PPL(3);
		Asm("ldc %0, r2_bank" :: "r"(mdr));
	}

	return E_OK;

err_ret:
	return ercd;
}
