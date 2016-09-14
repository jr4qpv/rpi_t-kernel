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
* File Name    : spibsc_ioset_api.c
* Version      : 1.2
* Device       : R7S910017
* Abstract     : API of SPIBSC I/O settings
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : SPIBSC settings API for SPIBSC of RZ/T1 
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.07.2015 1.1      First Release
*              : 30.11.2015 1.2      Second Release
*                                     - No modification
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"
#include "r_spibsc_ioset_api.h"


#ifndef __SPIBSC_DEFAULT__

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern int32_t Userdef_SFLASH_Set_Mode(uint32_t data_width, uint32_t addr_mode);


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static int32_t SPIBSC_Exread_Mode_Config(st_spibsc_cfg_t *spibsccfg) __attribute__ ((section (".loader_text2")));

int32_t R_SFLASH_Exmode(void) __attribute__ ((section (".loader_text2")));
int32_t R_SFLASH_Spimode(void) __attribute__ ((section (".loader_text2")));
int32_t R_SFLASH_SpibscStop(void) __attribute__ ((section (".loader_text2")));
int32_t R_SFLASH_Spimode_Init(uint32_t data_width, uint32_t addr_mode, uint32_t spbr, uint32_t brdv) __attribute__ ((section (".loader_text2")));
int32_t R_SFLASH_Exmode_Init(st_spibsc_cfg_t *spibsccfg) __attribute__ ((section (".loader_text2")));
int32_t R_SFLASH_Exmode_Setting(st_spibsc_cfg_t *spibsccfg) __attribute__ ((section (".loader_text2")));
void R_SFLASH_WaitTend() __attribute__ ((section (".loader_text2")));
int32_t R_SFLASH_Set_Config(st_spibsc_cfg_t *spibsccfg) __attribute__ ((section (".loader_text2")));


/******************************************************************************
* Function Name: R_SFLASH_Exmode
* Description  :
* Arguments    : void
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SFLASH_Exmode(void)
{
    int32_t    ret;

    ret = spibsc_exmode();

    return ret;
}

/******************************************************************************
* Function Name: R_SFLASH_Spimode
* Description  :
* Arguments    : void
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SFLASH_Spimode(void)
{
    int32_t    ret;

    ret = spibsc_spimode();

    return ret;
}

/******************************************************************************
* Function Name: R_SFLASH_SpibscStop
* Description  :
* Arguments    : void
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SFLASH_SpibscStop(void)
{
    int32_t    ret;

    ret = spibsc_stop();

    return ret;
}

/******************************************************************************
* Function Name: R_SFLASH_Spimode_Init
* Description  : Initialization in a basic part of a SPI multi-I/O bus controller.
*              : Serial flash memory is set as a Quad operation mode.
* Arguments    : uint32_t data_width
*              : uint32_t addr_mode
*              : uint32_t spbr
*              : uint32_t brdv
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SFLASH_Spimode_Init(uint32_t data_width, uint32_t addr_mode, uint32_t spbr, uint32_t brdv)
{
    int32_t     ret;

    /* ==== Initialization of SPIBSC ==== */
    ret = spibsc_common_init(data_width, spbr, brdv);
    if (ret !=0)
    {
        return ret;
    }

    ret = Userdef_SFLASH_Set_Mode(data_width, addr_mode);

    return ret;
}

/******************************************************************************
* Function Name: R_SFLASH_Exmode_Init
* Description  : An outside address space read mode set as a SPI multi-I/O bus
*              : controller.
* Arguments    : st_spibsc_cfg_t *spibsccfg
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SFLASH_Exmode_Init(st_spibsc_cfg_t *spibsccfg)
{
    int32_t    ret;

    ret = spibsc_bsz_set();
    if (ret != 0)
    {
        return ret;
    }

    ret = spibsc_dr_init(spibsccfg);

    return ret;
}

/******************************************************************************
* Function Name: R_SFLASH_Exmode_Setting
* Description  : Initialization in a basic part of a SPI multi-I/O bus controller.
*              : Serial flash memory is set as a Quad operation mode.
* Arguments    : st_spibsc_cfg_t *spibsccfg
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SFLASH_Exmode_Setting(st_spibsc_cfg_t *spibsccfg)
{
    int32_t     ret;

    /* ==== check of SPIBSC register ==== */
    ret = SPIBSC_Exread_Mode_Config(spibsccfg);
    if (ret != 0)
    {
        return ret;
    }
    /* ==== Initialization of SPIBSC ==== */
    ret = spibsc_common_init(spibsccfg->udef_data_width,
                             spibsccfg->udef_spbr,
                             spibsccfg->udef_brdv);
    if (ret != 0)
    {
        return ret;
    }

    ret = Userdef_SFLASH_Set_Mode(spibsccfg->udef_data_width, spibsccfg->udef_addr_mode);
    if (ret != 0)
    {
        return ret;
    }

    ret = R_SFLASH_Exmode_Init(spibsccfg);

    return ret;
}

/******************************************************************************
* Function Name: R_SFLASH_WaitTend
* Description  : Wait TEND
* Arguments    :  void
* Return Value :  void
******************************************************************************/
void R_SFLASH_WaitTend()
{
    spibsc_wait_tend();
}

/******************************************************************************
* Function Name: R_SFLASH_Set_Config
* Description  : The setting function of SPIBSC.
* Arguments    : st_spibsc_cfg_t *spibsccfg
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SFLASH_Set_Config(st_spibsc_cfg_t *spibsccfg)
{
    int32_t    ret;

    Userdef_SPIBSC_Set_Config(spibsccfg);
    ret = SPIBSC_Exread_Mode_Config(spibsccfg);

    return ret;
}

/******************************************************************************
* Function Name: SPIBSC_Exread_Mode_Config
* Description  : Called the configuration function
* Arguments    :  st_spibsc_cfg_t *spibsccfg
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
static int32_t SPIBSC_Exread_Mode_Config(st_spibsc_cfg_t *spibsccfg)
{
    if ((spibsccfg->udef_cmd_width > 2)    || (spibsccfg->udef_addr_width > 2)     ||
       (spibsccfg->udef_opd_enable > 15)  || (spibsccfg->udef_opd_width > 2)      ||
       (spibsccfg->udef_dmycyc_num > 7)   || (spibsccfg->udef_dmycyc_enable > 1)  ||
       (spibsccfg->udef_dmycyc_width > 2) || (spibsccfg->udef_data_width > 2)     ||
       (spibsccfg->udef_brdv > 3)                                                    )
    {
        return -1;
    }

    return 0;
}

#endif


/* End of File */


