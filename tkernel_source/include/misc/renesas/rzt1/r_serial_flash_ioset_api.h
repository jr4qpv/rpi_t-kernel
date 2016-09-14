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
* Tool-Chain   : IAR Embedded Workbench for ARM 7.30.4
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
#ifndef R_SERIAL_FLASH_IOSET_API_H
#define R_SERIAL_FLASH_IOSET_API_H


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    /* SMENR: SPI Mode Enable Register --------------------------------------*/
        /* b.31-30  CDB[1:0]: Command Bit Width                              */
        /* b.29-28  OCDB[1:0]: Optional Command Bit Width                    */
        /* b.25-24  ADB[1:0]: Address Bit Width                              */
        /* b.21-20  OPDB[1:0]: Option Data Bit Width                         */
        /* b.17-16  SPIDB[1:0]: Tx Data Bit Width                            */
        /* b.15     DME: Dummy Cycle Enable                                  */
        /* b.14     CDE: Command Enable                                      */
        /* b.12     OCDE: Optional Command Enable                            */
        /* b.11-8   ADE[3:0]: Address Enable                                 */
        /* b.7-4    OPDE[3:0]: Option Data Enable                            */
        /* b.3-0    SPIDE[3:0]: Tx Data Enable                               */
    uint32_t cdb;
    uint32_t ocdb;
    uint32_t adb;
    uint32_t opdb;
    uint32_t spidb;
    uint32_t dme;     /* Dummy cycle enable at the time of a SPI mode */
    uint32_t cde;
    uint32_t ocde;
    uint32_t ade;
    uint32_t opde;
    uint32_t spide;
    /* SMCR: SPI Mode Control Register --------------------------------------*/
        /* b.8      SSLKP: SPBSSL Signal level                               */
        /* b.2      SPIRE: Data Read Enable                                  */
        /* b.1      SPIWE: Data Write Enable                                 */
        /* b.0      SPIE: SPI Data Tx Enable                                 */
    uint32_t sslkp;
    uint32_t spire;
    uint32_t spiwe;

    /* DRDMCR: Data Read Dummy Cycle Setting Register -----------------------*/
        /* b.17-16  DMDB[1:0]: Dummy Cycle Bit Width                         */
        /* b.2-0    DMCYC[2:0]: Dummy Cycle                                  */
    uint32_t dmdb;    /* The dummy cycle bit width of the time of a SPI mode  */
    uint32_t dmcyc;   /* The number of dummy cycles of the time of a SPI mode */

    /* SMCMR: SPI Mode Command Setting Register -----------------------------*/
        /* b.23-16  CMD[7:0]: Command                                        */
        /* b.7-0    OCMD[7:0]: Option Command                                */
    uint32_t cmd;
    uint32_t ocmd;

    /* SMADR: SPI Mode Address Setting Register -----------------------------*/
        /* b.31-24  ADRE[7:0]: Address (for 32-bit Address)                  */
        /* b.23-0   ADR[23:0]: Address                                       */
    uint32_t addr;

    /* SMOPR: SPI Mode Option Setting Register ------------------------------*/
        /* b.31-24  OPD3[7:0]: Option Data 3                                 */
        /* b.23-16  OPD2[7:0]: Option Data 2                                 */
        /* b.15-8   OPD1[7:0]: Option Data 1                                 */
        /* b.7-0    OPD0[7:0]: Option Data 0                                 */
    uint32_t opd[4];

    /* SMRDR0: SPI Mode Read Data Register 0 --------------------------------*/
        /* b.31-0   RDATA0[31:0]: Read Data                                  */
    uint32_t smrdr;

    /* SMWDR0: SPI Mode Write Data Register 0 -------------------------------*/
        /* b.31-0   WDATA0[31:0]: Write Data                                 */
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
int32_t   R_SPIBSC_Exmode(void);
int32_t   R_SPIBSC_Spimode(void);
int32_t   R_SPIBSC_ExmodeInit(st_spibsc_cfg_t *spibsccfg);
int32_t   R_SPIBSC_ExmodeSetting(st_spibsc_cfg_t *spibsccfg);
void      R_SPIBSC_WaitTend(void);
int32_t   R_SPIBSC_SetConfig(st_spibsc_cfg_t *spibsccfg);


/* User defined function */
void     userdef_spibsc_set_config(st_spibsc_cfg_t *spibsccfg);
int32_t   userdef_sflash_set_mode(uint32_t data_width, uint32_t addr_mode);

/* driver function */
int32_t   sflash_bsz_set(void);
int32_t   sflash_common_init(uint32_t data_width, uint32_t spbr, uint32_t brdv);
void      sflash_wait_tend(void);
int32_t   sflash_dr_init(st_spibsc_cfg_t *spibsccfg);
int32_t   sflash_exmode(void);
int32_t   sflash_spimode(void);
int32_t   sflash_transfer(st_spibsc_spimd_reg_t *regset);
int32_t   sflash_transfer_erase(uint32_t addr, uint32_t cmd, uint32_t addr_mode);
int32_t   sflash_transfer_program_1(uint32_t addr, uint32_t cmd, uint32_t addr_mode);
int32_t   sflash_transfer_program_2(uint32_t spide, uint32_t spidb, uint32_t *data, int32_t size);


#endif /* R_SERIAL_FLASH_IOSET_API_H */


/* End of File */
