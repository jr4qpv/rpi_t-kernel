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
 *	@(#)asm_common.h (tk) 2016/03/14
 *
 *	Assembler Macro
 */

#ifndef __TK_ASM_COMMON_H__
#define __TK_ASM_COMMON_H__

#if STD_SH7727
#  include <tk/sysdepend/std_sh7727/asm_depend.h>
#endif
#if STD_SH7751R
#  include <tk/sysdepend/std_sh7751r/asm_depend.h>
#endif
#if MIC_M32104
#  include <tk/sysdepend/mic_m32104/asm_depend.h>
#endif
#if STD_S1C38K
#  include <tk/sysdepend/std_s1c38k/asm_depend.h>
#endif
#if STD_MC9328
#  include <tk/sysdepend/std_mc9328/asm_depend.h>
#endif
#if MIC_VR4131
#  include <tk/sysdepend/mic_vr4131/asm_depend.h>
#endif
#if STD_VR5500
#  include <tk/sysdepend/std_vr5500/asm_depend.h>
#endif
#if STD_MB87Q1100
#  include <tk/sysdepend/std_mb87q1100/asm_depend.h>
#endif
#if STD_SH7760
#  include <tk/sysdepend/std_sh7760/asm_depend.h>
#endif
#if TEF_EM1D
#  include <tk/sysdepend/tef_em1d/asm_depend.h>
#endif

#if RPI_BCM283x
#  include <tk/sysdepend/rpi_bcm283x/asm_depend.h>
#endif
#if APP_SH7760
#  include <tk/sysdepend/app_sh7760/asm_depend.h>
#endif
#if APP_RZT1
#  include <tk/sysdepend/app_rzt1/asm_depend.h>
#endif

#endif /* __TK_ASM_COMMON_H__ */


/*----------------------------------------------------------------------*/
#if 0
#|History of "asm_common.h"
#|=========================
#|* 2015/11/25	[app_sh7760]用に、APP_SH7760 処理の追加
#|* 2016/03/01	[rpi_bcm283x]用に、RPI_BCM283x 処理の追加
#|* 2016/03/14	[app_rzt1]用に、APP_RZT1 処理の追加
#|
#endif
