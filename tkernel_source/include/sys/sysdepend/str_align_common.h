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
 *    Modified by T.Yokobayashi at 2015/11/25.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)str_align_common.h (sys) 2016/03/14
 *
 *	Bit alignment definitions for structure
 */

#ifndef __SYS_STR_ALIGN_COMMON_H__
#define __SYS_STR_ALIGN_COMMON_H__

#if STD_SH7727
#  include <sys/sysdepend/std_sh7727/str_align_depend.h>
#endif
#if STD_SH7751R
#  include <sys/sysdepend/std_sh7751r/str_align_depend.h>
#endif
#if MIC_M32104
#  include <sys/sysdepend/mic_m32104/str_align_depend.h>
#endif
#if STD_S1C38K
#  include <sys/sysdepend/std_s1c38k/str_align_depend.h>
#endif
#if STD_MC9328
#  include <sys/sysdepend/std_mc9328/str_align_depend.h>
#endif
#if MIC_VR4131
#  include <sys/sysdepend/mic_vr4131/str_align_depend.h>
#endif
#if STD_VR5500
#  include <sys/sysdepend/std_vr5500/str_align_depend.h>
#endif
#if STD_MB87Q1100
#  include <sys/sysdepend/std_mb87q1100/str_align_depend.h>
#endif
#if STD_SH7760
#  include <sys/sysdepend/std_sh7760/str_align_depend.h>
#endif
#if TEF_EM1D
#  include <sys/sysdepend/tef_em1d/str_align_depend.h>
#endif

#if RPI_BCM283x
#  include <sys/sysdepend/rpi_bcm283x/str_align_depend.h>
#endif
#if APP_SH7760
#  include <sys/sysdepend/app_sh7760/str_align_depend.h>
#endif
#if APP_RZT1
#  include <sys/sysdepend/app_rzt1/str_align_depend.h>
#endif

#endif /* __SYS_STR_ALIGN_COMMON_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|History of "str_align_common.h"
#|===============================
#|* 2015/11/25	[app_sh7760]用に、APP_SH7760 処理の追加
#|* 2016/03/01	[rpi_bcm283x]用に、RPI_BCM283x 処理の追加
#|* 2016/03/14	[app_rzt1]用に、APP_RZT1 処理の追加
#|
#endif
