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
* File Name    : r_serial_flash_ioset_api.c
* Version      : 1.1
* Device       : R7S910017
* Abstract     : API of SPIBSC I/O settings
* Tool-Chain   : ARM DS-5 Development Studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : SPIBSC settings API for SPIBSC of RZ/T1 
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 22.12.2015 1.0      First Release
*              : 14.06.2016 1.1      Second Release
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"
#include "iodefine.h"
#include "r_serial_flash_ioset_api.h"
#include "serial_flash.h"
#include "serial_flash_spibsc.h"


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


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static int32_t spibsc_exread_mode_config(st_spibsc_cfg_t *spibsccfg);


/******************************************************************************
* Function Name: R_SPIBSC_Exmode
* Description  :
* Arguments    : void
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SPIBSC_Exmode(void)
{
    int32_t    ret;

    ret = sflash_exmode();

    return ret;
    
} /* End of function R_SPIBSC_Exmode() */

/******************************************************************************
* Function Name: R_SPIBSC_Spimode
* Description  :
* Arguments    : void
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SPIBSC_Spimode(void)
{
    int32_t    ret;

    ret = sflash_spimode();

    return ret;
    
} /* End of function R_SPIBSC_Spimode() */

/******************************************************************************
* Function Name: R_SPIBSC_ExmodeInit
* Description  : An outside address space read mode set as a SPI multi-I/O bus
*              : controller.
* Arguments    : st_spibsc_cfg_t *spibsccfg
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SPIBSC_ExmodeInit(st_spibsc_cfg_t *spibsccfg)
{
    int32_t    ret;

    ret = sflash_bsz_set();
    if (0 == ret)
    {
        ret = sflash_dr_init(spibsccfg);
    }

    return ret;
} /* End of function R_SPIBSC_ExmodeInit() */

/******************************************************************************
* Function Name: R_SPIBSC_ExmodeSetting
* Description  : Initialization in a basic part of a SPI multi-I/O bus controller.
*              : Serial flash memory is set as a Quad operation mode.
* Arguments    : st_spibsc_cfg_t *spibsccfg
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SPIBSC_ExmodeSetting(st_spibsc_cfg_t *spibsccfg)
{
    int32_t     ret;

    /* ==== check of SPIBSC register ==== */
    ret = spibsc_exread_mode_config(spibsccfg);
    if (0 == ret)
    {
        /* ==== Initialization of SPIBSC ==== */
        ret = sflash_common_init(spibsccfg->udef_data_width,
                                spibsccfg->udef_spbr,
                                spibsccfg->udef_brdv);
        if (0 == ret)
        {
            ret = userdef_sflash_set_mode(spibsccfg->udef_data_width, spibsccfg->udef_addr_mode);
            if (0 == ret)
            {
                ret = R_SPIBSC_ExmodeInit(spibsccfg);
            }
        }
        
    }

    return ret;
    
} /* End of function R_SPIBSC_ExmodeSetting() */

/******************************************************************************
* Function Name: R_SPIBSC_SetConfig
* Description  : The setting function of SPIBSC.
* Arguments    : st_spibsc_cfg_t *spibsccfg
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SPIBSC_SetConfig(st_spibsc_cfg_t *spibsccfg)
{
    int32_t    ret;

    userdef_spibsc_set_config(spibsccfg);
    ret = spibsc_exread_mode_config(spibsccfg);

    return ret;

} /* End of function R_SPIBSC_SetConfig() */

/******************************************************************************
* Function Name: spibsc_exread_mode_config
* Description  : Called the configuration function
* Arguments    :  st_spibsc_cfg_t *spibsccfg
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
static int32_t spibsc_exread_mode_config(st_spibsc_cfg_t *spibsccfg)
{
    int32_t    ret;

    if ((((((((((spibsccfg->udef_cmd_width > (uint32_t)2)  || (spibsccfg->udef_addr_width > (uint32_t)2))  ||
    	(spibsccfg->udef_opd_enable > (uint32_t)15))  || (spibsccfg->udef_opd_width > (uint32_t)2))      ||
        (spibsccfg->udef_dmycyc_num > (uint32_t)7))  || (spibsccfg->udef_dmycyc_enable > (uint32_t)1))  ||
        (spibsccfg->udef_dmycyc_width > (uint32_t)2)) || (spibsccfg->udef_data_width > (uint32_t)2))     ||
        (spibsccfg->udef_brdv > (uint32_t)3))                                                    )
    {
        ret = -1;
    }
    else
    {
        ret = 0;
    }

    return ret;
} /* End of function spibsc_exread_mode_config() */

#endif


/* End of File */


