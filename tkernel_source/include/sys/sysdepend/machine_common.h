/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/01.
 *
 *----------------------------------------------------------------------
 *
 *    Modified by T.Yokobayashi at 2015/11/24.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)machine_common.h (sys) 2016/03/14
 *
 *	Machine type definition
 */

#ifndef __SYS_MACHINE_COMMON_H__
#define __SYS_MACHINE_COMMON_H__

#ifdef _STD_SH7727_
#  include <sys/sysdepend/std_sh7727/machine_depend.h>
#endif
#ifdef _STD_SH7751R_
#  include <sys/sysdepend/std_sh7751r/machine_depend.h>
#endif
#ifdef _MIC_M32104_
#  include <sys/sysdepend/mic_m32104/machine_depend.h>
#endif
#ifdef _STD_S1C38K_
#  include <sys/sysdepend/std_s1c38k/machine_depend.h>
#endif
#ifdef _STD_MC9328_
#  include <sys/sysdepend/std_mc9328/machine_depend.h>
#endif
#ifdef _MIC_VR4131_
#  include <sys/sysdepend/mic_vr4131/machine_depend.h>
#endif
#ifdef _STD_VR5500_
#  include <sys/sysdepend/std_vr5500/machine_depend.h>
#endif
#ifdef _STD_MB87Q1100_
#  include <sys/sysdepend/std_mb87q1100/machine_depend.h>
#endif
#ifdef _STD_SH7760_
#  include <sys/sysdepend/std_sh7760/machine_depend.h>
#endif
#ifdef _TEF_EM1D_
#  include <sys/sysdepend/tef_em1d/machine_depend.h>
#endif

#ifdef _RPI_BCM283x_
#  include <sys/sysdepend/rpi_bcm283x/machine_depend.h>
#endif
#ifdef _APP_SH7760_
#  include <sys/sysdepend/app_sh7760/machine_depend.h>
#endif
#ifdef _APP_RZT1_
#  include <sys/sysdepend/app_rzt1/machine_depend.h>
#endif

#endif /* __SYS_MACHINE_COMMON_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|History of "machine_common.h"
#|=============================
#|* 2015/11/24	[app_sh7760]用に、_APP_SH7760_ 処理の追加
#|* 2016/03/01	[rpi_bcm283x]用に、_RPI_BCM283x_ 処理の追加
#|* 2016/03/14	[app_rzt1]用に、_APP_RZT1_ 処理の追加
#|
#endif
