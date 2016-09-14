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
* File Name    : r_reset.c
* Version      : 1.2
* Device       : R7S910017
* Abstract     : API for RESET and Low-Power function
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : RESET and Low-Power API of RZ/T1
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

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define RST_WRITE_ENABLE  (0x0000A502)
#define RST_WRITE_DISABLE (0x0000A500)

/*******************************************************************************
Typedef definitions
*******************************************************************************/



/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/


/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/



/*******************************************************************************
Private variables and functions
*******************************************************************************/


/*******************************************************************************
* Function Name : R_RST_WriteEnable
* Description   : Enables writing to the registers related to RESET and Low-
*                 Power function. And dummy read the register in order to fix 
*                 the register value.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_RST_WriteEnable(void)
{
    volatile uint32_t dummy; 
  
    /* Enables writing to the Reset and Low-Power register */
    SYSTEM.PRCR.LONG = RST_WRITE_ENABLE;
    dummy = SYSTEM.PRCR.LONG;
    (void)dummy;							/* 未使用変数警告を出さない対策 */
}

/*******************************************************************************
 End of function R_RST_WriteEnable
*******************************************************************************/

/*******************************************************************************
* Function Name : R_RST_WriteDisable
* Description   : Disables writing to the registers related to RESET and Low-
*                 Power function. And dummy read the register in order to fix 
*                 the register value.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_RST_WriteDisable(void)
{
    volatile uint32_t dummy; 
  
    /* Disables writing to the Reset and Low-Power register */
    SYSTEM.PRCR.LONG = RST_WRITE_DISABLE;
    dummy = SYSTEM.PRCR.LONG;
    (void)dummy;							/* 未使用変数警告を出さない対策 */
}

/*******************************************************************************
 End of function R_RST_WriteDisable
*******************************************************************************/

/* End of File */


/*----------------------------------------------------------------------*/
#if 0
#|History of "r_reset.c"
#|====================
#|* 2016/04/08	Renesasの｢初期設定｣ｻﾝﾌﾟﾙｺｰﾄﾞから修正。(by T.Yokobayashi)
#|* 2016/04/08	(void)dummy;行を追加して、警告を出さないようにした。
#|
#endif
