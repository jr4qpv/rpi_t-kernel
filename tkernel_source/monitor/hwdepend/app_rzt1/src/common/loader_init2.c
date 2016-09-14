/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : RZ/T1 Init program
* File Name    : loader_init2.c
* Version      : 1.2
* Device       : R7S910017
* Abstract     : Loader program 2
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : Initialize the peripheral settings of RZ/T1
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.11.2015 1.2      First Release
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"
#include "iodefine.h"
#include "r_system.h"
#include "r_reset.h"
#include "r_cpg.h"
#include "r_atcm_init.h"
#include "r_port.h"
#include "r_mpc.h"
#include "r_ecm.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
extern int reset_main(void);
extern void bus_init(void);
extern void set_low_vec(void);
extern void cache_init(void);
extern void copy_to_atcm(void);
extern void __exit(int code);

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private variables and functions
*******************************************************************************/
void loader_init2(void) __attribute__ ((section (".loader_text2")));
void reset_check(void) __attribute__ ((section (".loader_text2")));
void cpg_init(void) __attribute__ ((section (".loader_text2")));

/*******************************************************************************
* Function Name : loader_init2
* Description   : Initialize sysytem by loader program 2
* Arguments    : none
* Return Value : none
*******************************************************************************/
void loader_init2(void)
{ 
	int retval;

	/* Check the reset source */
    reset_check();
  
    /* Set CPU clock and LOCO clock */
    cpg_init();
    
    /* Set ATCM access wait to 1-wait with optimization */
    /* Caution: ATCM_WAIT_0 is permitted if CPUCLK = 150MHz or 300MHz.
                ATCM_WAIT_1_OPT is permitted if CPUCLK = 450MHz or 600MHz.*/
    R_ATCM_WaitSet(ATCM_WAIT_1_OPT);

    /* Copy the application program from external memory to ATCM */
    copy_to_atcm();

    /* Initialize the bus settings */
    bus_init();

    /* Initialize I1, D1 Cache and MPU setting */
    cache_init();
    
    /* Set RZ/T1 to Low-vector (SCTLR.V = 0) */
    set_low_vec();
                
    /* Jump to _main() */
    retval = reset_main();				// see "..\reset_main.c"

    __exit(retval);
}

/*******************************************************************************
 End of function loader_init2
*******************************************************************************/

/*******************************************************************************
* Function Name : reset_check
* Description   : Check the reset source and execute the each sequence.
*                 When error source number 35 is generated, set P77 pin to High.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void reset_check(void)
{
#ifndef RAM_BOOT
    volatile uint8_t result;
    volatile uint32_t dummy;
#endif
    
    /* Check the reset status flag and execute the each sequence */
    if (RST_SOURCE_ECM == SYSTEM.RSTSR0.LONG) // ECM reset is generated
    {
        /* Clear reset status flag */ 
        R_RST_WriteEnable();              // Enable writing to the RSTSR0 register
        SYSTEM.RSTSR0.LONG = 0x00000000;  // Clear reset factor flag
        R_RST_WriteDisable();             // Disable writing to the RSTSR0 register
        
      /* Please coding the User program */ 
#ifndef RAM_BOOT
        /* Check the ECM error source */
        if (1 == ECMM.ECMMESSTR1.BIT.ECMMSSE102) // Error source number 35 is generated
        {
            /* Clear error source status register with special sequence */
            /*** Note: If use R_ECM_Write_Reg32(), please modify the icf files ***/ 
            /***       (Add object of r_ecm.o to LDR_PRG blocks )              ***/
            ECM.ECMPCMD1.LONG = ECM_COMMAND_KEY;  // Write fixed value
            dummy = ECM.ECMPCMD1.LONG;
            ECM.ECMESSTC1.LONG = 0x00000004;      // Write expected value
            ECM.ECMESSTC1.LONG = 0xFFFFFFFB;      // Write inversed value of the expected value
            ECM.ECMESSTC1.LONG = 0x00000004;      // Write expected value again   
            dummy = ECM.ECMESSTC1.LONG;
            /* Check the ECMPS register whether special sequence is success or failure
                   result = 0 : Special sequence is success.
                          = 1 : Special sequence is failure.                              */
            result = ECM.ECMPS.BYTE;  
                                             
            /* Set P77 to Output port pin and High level (LED2 on) as error sequence */
            PORT7.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize port settings
            PORT7.PMR.BIT.B7 = PORT_MODE_GENERAL;
  
            PORT7.PODR.BIT.B7 = PORT_OUTPUT_HIGH;      // Set output level to High
            PORT7.PDR.BIT.B7 = PORT_DIRECTION_OUTPUT;  // Set port direction to output

            (void)dummy;					/* 未使用変数警告を出さない対策 */
            (void)result;					/* 未使用変数警告を出さない対策 */
        }
#endif
        
    }
    else if (RST_SOURCE_SWR1 == SYSTEM.RSTSR0.LONG) // Software reset 1 is generated
    {
        /* Clear reset status flag */
        R_RST_WriteEnable();              // Enable writing to the RSTSR0 register
        SYSTEM.RSTSR0.LONG = 0x00000000;  // Clear reset factor flag
        R_RST_WriteDisable();             // Disable writing to the RSTSR0 register

        /* Please coding the User program */

    }
    else if (RST_SOURCE_RES == SYSTEM.RSTSR0.LONG) // RES# pin reset is generated
    {
        /* Clear reset status flag */ 
        R_RST_WriteEnable();              // Enable writing to the RSTSR0 register
        SYSTEM.RSTSR0.LONG = 0x00000000;  // Clear reset factor flag
        R_RST_WriteDisable();             // Disable writing to the RSTSR0 register
        
        /* Please coding the User program */    
        
    }
    else // Any reset is not generated
    {        
        /* Please coding the User program */  
    }

}

/*******************************************************************************
 End of function reset_check
*******************************************************************************/

/*******************************************************************************
* Function Name : cpg_init
* Description   : Set CPU clock and LOCO clock by CPG function
* Arguments    : none
* Return Value : none
*******************************************************************************/
void cpg_init(void)
{
    volatile uint32_t dummy; 
      
    /* Enables writing to the registers related to CPG function */
    R_CPG_WriteEnable();
    
    /* Enables LOCO clock operation */
    SYSTEM.LOCOCR.BIT.LCSTP = CPG_LOCO_ENABLE;
    
    /* Set CPUCLK to 600MHz, and dummy read at three times */
    SYSTEM.PLL1CR.LONG = CPG_CPUCLK_600_MHz;	//org:CPG_CPUCLK_450_MHz;
    dummy = SYSTEM.PLL1CR.LONG;
    dummy = SYSTEM.PLL1CR.LONG;
    dummy = SYSTEM.PLL1CR.LONG;
    (void)dummy;							/* 未使用変数警告を出さない対策 */
     
    /* Enables PLL1 operation */
    SYSTEM.PLL1CR2.LONG = CPG_PLL1_ON;    
    
    /* Disables writing to the registers related to CPG function */
    R_CPG_WriteDisable(); 
    
    /* Wait about 100us for PLL1 (and LOCO) stabilization */
    R_CPG_PLL_Wait();

    /* Enables writing to the registers related to CPG function */
    R_CPG_WriteEnable();  
     
    /* Selects the PLL1 as clock source */
    SYSTEM.SCKCR2.LONG = CPG_SELECT_PLL1;
    
    /* Set BSC CKIO clock to 75MHz */
    SYSTEM.SCKCR.BIT.CKIO = CPG_CKIO_75_MHz;

    /* Disables writing to the registers related to CPG function */
    R_CPG_WriteDisable();
  
}

/*******************************************************************************
 End of function cpg_init
*******************************************************************************/

/* End of File */


/*----------------------------------------------------------------------
#|History of "loader_init2.c"
#|===========================
#|* 2016/03/30	Renesasの｢初期設定｣ｻﾝﾌﾟﾙｺｰﾄﾞから修正。(by T.Yokobayashi)
#|* 2016/04/02	SFLASH,NORブート時の処理を条件コンパイルで共通化
#|* 2016/04/05	初期化後はt-monitorの初期化(reset_main)へジャンプ
#|* 2016/04/07	CPUｸﾛｯｸを600MHzに変更
#|* 2016/04/08	copy_to_atcm()のextern定義を追加（警告対策）
#|* 2016/04/08	(void)dummy;行を追加して、警告を出さないようにした。
#|* 2016/04/08	(void)result;行を追加して、警告を出さないようにした。
#|
*/
