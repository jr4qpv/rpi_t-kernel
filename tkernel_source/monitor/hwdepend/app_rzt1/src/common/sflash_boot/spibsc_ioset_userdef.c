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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : RZ/T1 Init program
* File Name    : spibsc_ioset_userdef.c
* Version      : 1.2
* Device       : R7S910017
* Abstract     : User define program of SPIBSC I/O settings
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : User define program for SPIBSC of RZ/T1 
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.07.2015 1.1      First Release
*              : 30.11.2015 1.2      Second Release
*                                     - Add the SPIBSC settings (DREAR register)
*******************************************************************************/


/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"
#include "iodefine.h"
#include "r_spibsc_ioset_api.h"


#ifndef __SPIBSC_DEFAULT__

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define SPIBSC_BUS_WITDH         (4)
#define SPIBSC_OUTPUT_ADDR       (SPIBSC_OUTPUT_ADDR_32)


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name: Userdef_SPIBSC_Set_Config
* Description  : The setting function of SPIBSC.
* Arguments    : st_spibsc_cfg_t *spibsccfg
* Return Value : none
******************************************************************************/
void Userdef_SPIBSC_Set_Config(st_spibsc_cfg_t *spibsccfg)
{
#if (SPIBSC_BUS_WITDH == 1)
    /* command */
    #if (SPIBSC_OUTPUT_ADDR == SPIBSC_OUTPUT_ADDR_32)
    spibsccfg->udef_cmd = SFLASHCMD_FAST_READ_4B;       /* 0C(hex) Command */
    #else
    spibsccfg->udef_cmd = SFLASHCMD_FAST_READ;          /* 0B(hex) Command */
    #endif

    /* address width */
    spibsccfg->udef_addr_width = SPIBSC_1BIT;

    /* optional data */
    spibsccfg->udef_opd_enable = SPIBSC_OUTPUT_DISABLE;

    /* option data bus width */
    spibsccfg->udef_opd_width  = SPIBSC_1BIT;

    /* dummy cycle number */
    spibsccfg->udef_dmycyc_num    = SPIBSC_DUMMY_8CYC;
    spibsccfg->udef_dmycyc_enable = SPIBSC_DUMMY_CYC_ENABLE;

    /* dummy cycle width */
    spibsccfg->udef_dmycyc_width  = SPIBSC_1BIT;

    /* data bit width */
    spibsccfg->udef_data_width = SPIBSC_1BIT;

#elif (SPIBSC_BUS_WITDH == 4)
    /* command */
    #if (SPIBSC_OUTPUT_ADDR == SPIBSC_OUTPUT_ADDR_32)
    spibsccfg->udef_cmd = SFLASHCMD_QUAD_IO_READ_4B;    /* EC(hex) Command */
    #else
    spibsccfg->udef_cmd = SFLASHCMD_QUAD_IO_READ;       /* EB(hex) Command */
    #endif

    /* address width */
    spibsccfg->udef_addr_width = SPIBSC_4BIT;

    /* optional data */
    spibsccfg->udef_opd_enable = SPIBSC_OUTPUT_OPD_3;

    /* option data bus width */
    spibsccfg->udef_opd_width  = SPIBSC_4BIT;

    /* dummy cycle number */
    spibsccfg->udef_dmycyc_num    = SPIBSC_DUMMY_6CYC;    /* OPD_3 is 2cycle */
                                                          /* dummy is 6cycle */
                                                          /*          8cycle */
    spibsccfg->udef_dmycyc_enable = SPIBSC_DUMMY_CYC_ENABLE;

    /* dummy cycle width */
    spibsccfg->udef_dmycyc_width  = SPIBSC_1BIT;

    /* data bit width */
    spibsccfg->udef_data_width = SPIBSC_4BIT;

#endif
    /* command width */
    spibsccfg->udef_cmd_width = SPIBSC_1BIT;

    /* optional data */
    spibsccfg->udef_opd3 = 0x00;
    spibsccfg->udef_opd2 = 0x00;
    spibsccfg->udef_opd1 = 0x00;
    spibsccfg->udef_opd0 = 0x00;

    /* bitrate */
    /*-------------------------------------------*/
    /*    udef_spbr =  1 : 75.00Mbps(1/ 2 PCLKA) */
    /*    udef_spbr =  2 : 37.50Mbps(1/ 4 PCLKA) */
    /*    udef_spbr =  3 : 25.00Mbps(1/ 6 PCLKA) */
    /*    udef_spbr =  4 : 18.75Mbps(1/ 8 PCLKA) */
    /*    udef_spbr =  5 : 15.00Mbps(1/10 PCLKA) */
    /*    udef_spbr =  6 : 12.50Mbps(1/12 PCLKA) */
    /*-------------------------------------------*/
    spibsccfg->udef_spbr = 1;
    /*-------------------------------------------*/
    /*    udef_bdrv =  0                         */
    /*-------------------------------------------*/
    spibsccfg->udef_brdv = 0;

    SPIBSC.DREAR.BIT.EAC = 1;    /* Enables external address bits [25:0] */
    spibsccfg->udef_addr_mode = SPIBSC_OUTPUT_ADDR;
}

#endif


/* End of File */

