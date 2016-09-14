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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : RZ/T1 Init program
* File Name    : r_spibsc_ioset_api.h
* Version      : 1.1
* Device       : R7S910017
* Abstract     : API for SPIBSC IO settings
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : SPIBSC IO setting API for RZ/T1
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 22.12.2015 1.0      First Release
*              : 18.03.2016 1.1      Second Release
*******************************************************************************/
#ifndef R_SPIBSC_IOSET_API_H
#define R_SPIBSC_IOSET_API_H


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "sflash.h"
#include "r_spibsc.h"


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    uint32_t cdb;
    uint32_t ocdb;
    uint32_t adb;
    uint32_t opdb;
    uint32_t spidb;
    uint32_t cde;
    uint32_t ocde;
    uint32_t ade;
    uint32_t opde;
    uint32_t spide;
    uint32_t sslkp;
    uint32_t spire;
    uint32_t spiwe;

    uint32_t dme;     /* Dummy cycle enable at the time of a SPI mode */
    uint32_t addre;   /* Address DDR enable                           */
    uint32_t opdre;   /* Option data DDRenable                        */
    uint32_t spidre;  /* Transmission data DDR enable                 */

    uint32_t dmdb;    /* The dummy cycle bit width of the time of a SPI mode  */
    uint32_t dmcyc;   /* The number of dummy cycles of the time of a SPI mode */

    uint32_t cmd;
    uint32_t ocmd;
    uint32_t addr;
    uint32_t opd[4];
    uint32_t smrdr;
    uint32_t smwdr;

} st_spibsc_spimd_reg_t;

typedef struct
{
    uint32_t udef_cmd;
    uint32_t udef_cmd_width;
    uint32_t udef_opd3;
    uint32_t udef_opd2;
    uint32_t udef_opd1;
    uint32_t udef_opd0;
    uint32_t udef_opd_enable;
    uint32_t udef_opd_width;
    uint32_t udef_dmycyc_num;
    uint32_t udef_dmycyc_enable;
    uint32_t udef_dmycyc_width;
    uint32_t udef_data_width;
    uint32_t udef_spbr;
    uint32_t udef_brdv;
    uint32_t udef_addr_width;
    uint32_t udef_addr_mode;

} st_spibsc_cfg_t;


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
/* api function */
int32_t   R_SFLASH_Exmode(void);
int32_t   R_SFLASH_Spimode(void);
int32_t   R_SFLASH_SpibscStop(void);
int32_t   R_SFLASH_Spimode_Init(uint32_t data_width, uint32_t addr_mode, uint32_t spbr, uint32_t brdv);
int32_t   R_SFLASH_Exmode_Init(st_spibsc_cfg_t *spibsccfg);
int32_t   R_SFLASH_Exmode_Setting(st_spibsc_cfg_t *spibsccfg);
void      R_SFLASH_WaitTend(void);
int32_t   R_SFLASH_Set_Config(st_spibsc_cfg_t *spibsccfg);


/* User defined function */
void      Userdef_SPIBSC_Set_Config(st_spibsc_cfg_t *spibsccfg);


/* driver function */
int32_t   spibsc_bsz_set(void);
int32_t   spibsc_common_init(uint32_t data_width, uint32_t spbr, uint32_t brdv);
void      spibsc_wait_tend(void);
int32_t   spibsc_dr_init(st_spibsc_cfg_t *spibsccfg);
int32_t   spibsc_exmode(void);
int32_t   spibsc_spimode(void);
int32_t   spibsc_stop(void);
int32_t   spibsc_transfer(st_spibsc_spimd_reg_t *regset);
int32_t   spibsc_transfer_erase(    uint32_t addr, uint32_t cmd, uint32_t addr_mode);
int32_t   spibsc_transfer_program_1(uint32_t addr, uint32_t cmd, uint32_t addr_mode);
int32_t   spibsc_transfer_program_2(uint32_t spide, uint32_t spidb, uint32_t *data, int32_t size);


#endif /* R_SPIBSC_IOSET_API_H */


/* End of File */
