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
* File Name    : spibsc_ioset.c
* Version      : 1.2
* Device       : R7S910017
* Abstract     : Initialize the SPIBSC I/O settings.
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : Initialize the SPIBSC I/O settings of RZ/T1
* Limitation   : This source file assumes that RZ/T1 boot mode is
*                SPI boot mode (Serial flash). 
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
#include "iodefine.h"
#include "r_spibsc_ioset_api.h"


#ifndef __SPIBSC_DEFAULT__

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define MPC_PFS_SPIBSC    (0x1B)


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static int32_t io_spibsc_port_setting(uint32_t data_width) __attribute__ ((section (".loader_text2")));
static void spibsc_1bit_setting(void) __attribute__ ((section (".loader_text2")));
static void spibsc_4bit_setting(void) __attribute__ ((section (".loader_text2")));

int32_t  spibsc_bsz_set(void) __attribute__ ((section (".loader_text2")));
int32_t spibsc_common_init(uint32_t data_width, uint32_t spbr, uint32_t brdv) __attribute__ ((section (".loader_text2")));
void spibsc_wait_tend(void) __attribute__ ((section (".loader_text2")));
int32_t spibsc_dr_init(st_spibsc_cfg_t *spibsccfg) __attribute__ ((section (".loader_text2")));
int32_t spibsc_exmode(void) __attribute__ ((section (".loader_text2")));
int32_t spibsc_spimode(void) __attribute__ ((section (".loader_text2")));
int32_t spibsc_stop(void) __attribute__ ((section (".loader_text2")));
int32_t spibsc_transfer(st_spibsc_spimd_reg_t *regset) __attribute__ ((section (".loader_text2")));
int32_t spibsc_transfer_erase(uint32_t addr, uint32_t cmd, uint32_t addr_mode) __attribute__ ((section (".loader_text2")));
int32_t spibsc_transfer_program_1(uint32_t addr, uint32_t cmd, uint32_t addr_mode) __attribute__ ((section (".loader_text2")));
int32_t spibsc_transfer_program_2(uint32_t spide, uint32_t spidb, uint32_t *data, int32_t size) __attribute__ ((section (".loader_text2")));

/******************************************************************************
* Function Name: spibsc_bsz_set
* Description  : Set a data bus width of a SPI multi-I/O bus controller.
* Arguments    : void
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t  spibsc_bsz_set(void)
{
    if ( SPIBSC.CMNSR.BIT.TEND != SPIBSC_TRANS_END )
    {
        return -1;
    }

    SPIBSC.CMNCR.BIT.BSZ = SPIBSC_CMNCR_BSZ_SINGLE;
    SPIBSC.DRCR.BIT.RCF  = SPIBSC_DRCR_RCF_EXE;

    return 0;
}

/******************************************************************************
* Function Name: spibsc_common_init
* Description  : Initialize the operation mode independent part of a SPI 
*              : multi-I/O bus controller.
* Arguments    : uint32_t data_width
*              : uint32_t spbr
*              : uint32_t brdv
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t spibsc_common_init(uint32_t data_width, uint32_t spbr, uint32_t brdv)
{
//org    volatile uint32_t   dummy_buf_32b;
//org    volatile uint8_t    dummy_buf_8b;

    /* PORT setting of SPIBSC */
    if ( io_spibsc_port_setting(data_width) < 0 )
    {
        return -1;
    }

    /* SPIBSC stop */
    SPIBSC.DRCR.BIT.SSLN = 1;
    while (1)
    {
        if ( SPIBSC.CMNSR.BIT.SSLF == SPIBSC_SSL_NEGATE )
        {
            break;
        }
    }

    if ( SPIBSC.CMNSR.BIT.TEND != SPIBSC_TRANS_END )
    {
        return -1;
    }

    SPIBSC.CMNCR.BIT.MOIIO3 = SPIBSC_OUTPUT_HiZ;
    SPIBSC.CMNCR.BIT.MOIIO2 = SPIBSC_OUTPUT_HiZ;
    SPIBSC.CMNCR.BIT.MOIIO1 = SPIBSC_OUTPUT_HiZ;
    SPIBSC.CMNCR.BIT.MOIIO0 = SPIBSC_OUTPUT_HiZ;

    SPIBSC.CMNCR.BIT.IO3FV = SPIBSC_OUTPUT_HiZ;
    SPIBSC.CMNCR.BIT.IO2FV = SPIBSC_OUTPUT_HiZ;
    SPIBSC.CMNCR.BIT.IO0FV = SPIBSC_OUTPUT_HiZ;

    /* swap by 8-bit unit(Defaults) */
    SPIBSC.CMNCR.BIT.SFDE = 1;

    /* S-flash mode 0 */
    /* even edge : write */
    SPIBSC.CMNCR.BIT.CPHAT = SPIBSC_CMNCR_CPHAT_EVEN;
    /* even edge : read */
    SPIBSC.CMNCR.BIT.CPHAR = SPIBSC_CMNCR_CPHAR_EVEN;
    /* SPBSSL   : low active */
    SPIBSC.CMNCR.BIT.SSLP  = SPIBSC_CMNCR_SSLP_LOW;
    /* SPBCLK   : low at negate */    
    SPIBSC.CMNCR.BIT.CPOL  = SPIBSC_CMNCR_CPOL_LOW;

    spibsc_bsz_set();

    /* next access delay */
    SPIBSC.SSLDR.BIT.SPNDL = SPIBSC_DELAY_1SPBCLK;
    /* SPBSSL negate delay */
    SPIBSC.SSLDR.BIT.SLNDL = SPIBSC_DELAY_1SPBCLK;
    /* clock delay */
    SPIBSC.SSLDR.BIT.SCKDL = SPIBSC_DELAY_1SPBCLK;

    /* ---- Bit rate Setting ---- */
    SPIBSC.SPBCR.BIT.SPBR = spbr;
    SPIBSC.SPBCR.BIT.BRDV = brdv;

    return 0;
}

/******************************************************************************
* Function Name: spibsc_wait_tend
* Description  : Wait TEND
* Arguments    : void
* Return Value : none
******************************************************************************/
void spibsc_wait_tend(void)
{
    while ( SPIBSC.CMNSR.BIT.TEND != SPIBSC_TRANS_END )
    {
        /* wait for transfer-end */
    }
}

/******************************************************************************
* Function Name: spibsc_dr_init
* Description  : The setting which makes a SPI multi-I/O bus controller activate
*              : an outside address space read mode.
* Arguments    : st_spibsc_cfg_t *spibsccfg
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t spibsc_dr_init(st_spibsc_cfg_t *spibsccfg)
{
    if ( SPIBSC.CMNSR.BIT.TEND != SPIBSC_TRANS_END )
    {
        return -1;
    }

    /* SPI I/O mode */
    SPIBSC.CMNCR.BIT.MD    = SPIBSC_CMNCR_MD_EXTRD;

    SPIBSC.DRCR.BIT.RBURST = SPIBSC_BURST_2;
    SPIBSC.DRCR.BIT.RBE    = SPIBSC_BURST_ENABLE;
    /* Keep SSL after read  */
    SPIBSC.DRCR.BIT.SSLE   = SPIBSC_SPISSL_KEEP;
                                    /* if not continuous address it negeted */

    /* ---- Command ---- */
    /* Command */
    SPIBSC.DRCMR.BIT.CMD = spibsccfg->udef_cmd;
    /* Single */
    SPIBSC.DRENR.BIT.CDB = spibsccfg->udef_cmd_width;
    /* Enable */
    SPIBSC.DRENR.BIT.CDE = SPIBSC_OUTPUT_ENABLE;
    /* ---- Option Command ---- */
    SPIBSC.DRCMR.BIT.OCMD = 0x00;
    /* Single */
    SPIBSC.DRENR.BIT.OCDB = SPIBSC_1BIT;
    /* Disable */
    SPIBSC.DRENR.BIT.OCDE = SPIBSC_OUTPUT_DISABLE;

    /* ---- Address ---- */
    SPIBSC.DRENR.BIT.ADB = spibsccfg->udef_addr_width;
    SPIBSC.DRENR.BIT.ADE = spibsccfg->udef_addr_mode;

    /* EAV */
    SPIBSC.DREAR.BIT.EAV = 0;

    /* ---- Option Data ---- */
    /* Option Data */
    SPIBSC.DROPR.BIT.OPD3 = spibsccfg->udef_opd3;
    /* Option Data */
    SPIBSC.DROPR.BIT.OPD2 = spibsccfg->udef_opd2;
    /* Option Data */
    SPIBSC.DROPR.BIT.OPD1 = spibsccfg->udef_opd1;
    /* Option Data */
    SPIBSC.DROPR.BIT.OPD0 = spibsccfg->udef_opd0;
    /* Single */
    SPIBSC.DRENR.BIT.OPDB = spibsccfg->udef_opd_width;
    /* Disable */
    SPIBSC.DRENR.BIT.OPDE = spibsccfg->udef_opd_enable;

    /* ---- Data ---- */
    SPIBSC.DRENR.BIT.DRDB = spibsccfg->udef_data_width;
    /* dummycycle enable */
    SPIBSC.DRENR.BIT.DME  = spibsccfg->udef_dmycyc_enable;

    /* Set data read dummycycle */
    SPIBSC.DRDMCR.BIT.DMDB  = spibsccfg->udef_dmycyc_width;
    SPIBSC.DRDMCR.BIT.DMCYC = spibsccfg->udef_dmycyc_num;

    return 0;
}

/*******************************************************************************
* Function Name: int32_t spibsc_exmode(void);
* Description  :
* Arguments    : void
* Return Value :  0 :
*              : -1 : error
*******************************************************************************/
int32_t spibsc_exmode(void)
{
    if ( SPIBSC.CMNCR.BIT.MD != SPIBSC_CMNCR_MD_EXTRD )
    {
        if ( spibsc_stop() < 0 )
        {
            return -1;
        }

        SPIBSC.CMNCR.BIT.MD = SPIBSC_CMNCR_MD_EXTRD;
    }

    return 0;
}

/*******************************************************************************
* Function Name: int32_t spibsc_spimode(void);
* Description  :
* Arguments    : void
* Return Value :  0 :
*              : -1 : error
*******************************************************************************/
int32_t spibsc_spimode(void)
{
    if ( SPIBSC.CMNCR.BIT.MD != SPIBSC_CMNCR_MD_SPI )
    {
        if ( spibsc_stop() < 0 )
        {
            return -1;
        }

        /* SPI Mode */
        SPIBSC.CMNCR.BIT.MD = SPIBSC_CMNCR_MD_SPI;
    }

    return 0;
}

/*******************************************************************************
* Function Name: int32_t spibsc_stop(void);
* Description  :
* Arguments    : void
* Return Value :  0 :
*              : -1 : error
*******************************************************************************/
int32_t spibsc_stop(void)
{
    SPIBSC.DRCR.BIT.SSLN = 1;
    while (1)
    {
        if ( SPIBSC.CMNSR.BIT.SSLF == SPIBSC_SSL_NEGATE )
        {
            break;
        }
    }
    return 0;
}

/******************************************************************************
* Function Name: spibsc_transfer
* Description  : Transmission setting of a SPI multi-I/O bus controller.
* Arguments    : st_spibsc_spimd_reg_t *regset :
*              :     The pointer to a structure for the transfer
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t spibsc_transfer(st_spibsc_spimd_reg_t *regset)
{
    if ( SPIBSC.CMNCR.BIT.MD != SPIBSC_CMNCR_MD_SPI )
    {
        if ( SPIBSC.CMNSR.BIT.SSLF != SPIBSC_SSL_NEGATE )
        {
            return -1;
        }

        /* SPI Mode */
        SPIBSC.CMNCR.BIT.MD = SPIBSC_CMNCR_MD_SPI;

    }

    if ( SPIBSC.CMNSR.BIT.TEND != SPIBSC_TRANS_END )
    {
        return -1;
    }

    /* ---- Command ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.CDE = regset->cde;
    if ( regset->cde != SPIBSC_OUTPUT_DISABLE )
    {
        /* Single/Dual/Quad */
        SPIBSC.SMENR.BIT.CDB = regset->cdb;
        /* Command */
        SPIBSC.SMCMR.BIT.CMD = regset->cmd;
    }

    /* ---- Option Command ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.OCDE = regset->ocde;
    if ( regset->ocde != SPIBSC_OUTPUT_DISABLE )
    {
        /* Single/Dual/Quad */
        SPIBSC.SMENR.BIT.OCDB = regset->ocdb;
        /* Option Command */
        SPIBSC.SMCMR.BIT.OCMD = regset->ocmd;
    }

    /* ---- Address ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.ADE = regset->ade;
    if ( regset->ade != SPIBSC_OUTPUT_DISABLE )
    {
        /* Single/Dual/Quad */
        SPIBSC.SMENR.BIT.ADB = regset->adb;
        /* Address */
        SPIBSC.SMADR.BIT.ADR = regset->addr;
    }

    /* ---- Option Data ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.OPDE = regset->opde;
    if ( regset->opde != SPIBSC_OUTPUT_DISABLE )
    {
        /* Single/Dual/Quad */
        SPIBSC.SMENR.BIT.OPDB = regset->opdb;
        /* Option Data */
        SPIBSC.SMOPR.BIT.OPD3 = regset->opd[0];
        /* Option Data */
        SPIBSC.SMOPR.BIT.OPD2 = regset->opd[1];
        /* Option Data */
        SPIBSC.SMOPR.BIT.OPD1 = regset->opd[2];
        /* Option Data */
        SPIBSC.SMOPR.BIT.OPD0 = regset->opd[3];
    }

    /* ---- Dummy ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.DME = regset->dme;
    if ( regset->dme != SPIBSC_DUMMY_CYC_DISABLE )
    {
        /* Single/Dual/Quad */
        SPIBSC.SMDMCR.BIT.DMDB  = regset->dmdb;
        /* Dummy Cycle */
        SPIBSC.SMDMCR.BIT.DMCYC = regset->dmcyc;
    }

    /* ---- Data ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.SPIDE = regset->spide;
    if ( regset->spide != SPIBSC_OUTPUT_DISABLE )
    {
        /* Single/Dual/Quad */
        SPIBSC.SMENR.BIT.SPIDB = regset->spidb;
        /* bit check */
        if ( regset->spidb != SPIBSC_1BIT )
        {
            if ((regset->spire == SPIBSC_SPIDATA_ENABLE) && (regset->spiwe == SPIBSC_SPIDATA_ENABLE))
            {
                /* not set in same time */
                return -1;
            }
        }
        /* Data */
        SPIBSC.SMWDR0.LONG = regset->smwdr;
    }

    /* ---- SPIRE(Enable/Disable) and SPIWE(Enable/Disable) ---- */
    SPIBSC.SMCR.BIT.SPIRE = regset->spire;
    SPIBSC.SMCR.BIT.SPIWE = regset->spiwe;

    /* ---- SSL(NEGATE or KEEP)---- */
    SPIBSC.SMCR.BIT.SSLKP = regset->sslkp;

    /* execute after setting SPNDL bit */
    SPIBSC.SMCR.BIT.SPIE  = SPIBSC_SPI_ENABLE;

    /* wait for transfer-start */
    R_SFLASH_WaitTend();

    regset->smrdr = SPIBSC.SMRDR0.LONG;

    return 0;
}

/******************************************************************************
* Function Name: spibsc_transfer_erase
* Description  : Transmission setting of a SPI multi-I/O bus controller.
* Arguments    : uint32_t addr
*              : uint32_t cmd
*              : uint32_t addr_mode
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t spibsc_transfer_erase(uint32_t addr, uint32_t cmd, uint32_t addr_mode)
{
    uint32_t    temp_reg;
    uint32_t    smcr_reg;

    /* ---- Command ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.CDE = SPIBSC_OUTPUT_ENABLE;
    /* Single/Dual/Quad */
    SPIBSC.SMENR.BIT.CDB = SPIBSC_1BIT;
    /* Command */
    SPIBSC.SMCMR.BIT.CMD = cmd;

    /* ---- Option Command ---- */
    /* ---- Option Data ---- */
    /* ---- Dummy ---- */
    /* ---- Data ---- */
    temp_reg  = SPIBSC.SMENR.LONG;
    temp_reg &= ~(SPIBSC_SMENR_OCDE | SPIBSC_SMENR_OPDE | SPIBSC_SMENR_DME | SPIBSC_SMENR_SPIDE);
    SPIBSC.SMENR.LONG = temp_reg;

    /* ---- Address ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.ADE = addr_mode;
    /* Single/Dual/Quad */
    SPIBSC.SMENR.BIT.ADB = SPIBSC_1BIT;
    /* Address */
    SPIBSC.SMADR.BIT.ADR = addr;

    /* ---- SPIRE(disable) and SPIWE(disable) ---- */
    temp_reg  = SPIBSC.SMCR.LONG;
    temp_reg &= ~(SPIBSC_SMCR_SPIRE | SPIBSC_SMCR_SPIWE);
    SPIBSC.SMCR.LONG = temp_reg;


    /* ---- SSL(NEGATE) and SPIE---- */
    smcr_reg  = SPIBSC.SMCR.LONG;
    smcr_reg &= ~SPIBSC_SMCR_SSLKP;
    smcr_reg |= SPIBSC_SMCR_SPIE;

    /* execute after setting SPNDL bit */
    SPIBSC.SMCR.LONG = smcr_reg;

    /* wait for transfer-start */
    R_SFLASH_WaitTend();

    temp_reg = SPIBSC.SMRDR0.LONG;

    return 0;
}

/******************************************************************************
* Function Name: spibsc_transfer_program_1
* Description  : Transmission setting of a SPI multi-I/O bus controller.
* Arguments    : uint32_t addr
*              : uint32_t cmd
*              : uint32_t addr_mode
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t spibsc_transfer_program_1(uint32_t addr, uint32_t cmd, uint32_t addr_mode)
{
    uint32_t    temp_reg;
    uint32_t    smcr_reg;

    /* ---- Command ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.CDE = SPIBSC_OUTPUT_ENABLE;
    /* Single/Dual/Quad */
    SPIBSC.SMENR.BIT.CDB = SPIBSC_1BIT;
    /* Command */
    SPIBSC.SMCMR.BIT.CMD = cmd;

    /* ---- Option Command ---- */
    /* ---- Option Data ---- */
    /* ---- Dummy ---- */
    /* ---- Data ---- */
    temp_reg  = SPIBSC.SMENR.LONG;
    temp_reg &= ~(SPIBSC_SMENR_OCDE | SPIBSC_SMENR_OPDE | SPIBSC_SMENR_DME | SPIBSC_SMENR_SPIDE);
    SPIBSC.SMENR.LONG = temp_reg;

    /* ---- Address ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.ADE = addr_mode;
    /* Single/Dual/Quad */
    SPIBSC.SMENR.BIT.ADB = SPIBSC_1BIT;
    /* Address */
    SPIBSC.SMADR.BIT.ADR = addr;

    /* ---- SPIRE(disable) and SPIWE(disable) ---- */
    temp_reg  = SPIBSC.SMCR.LONG;
    temp_reg &= ~(SPIBSC_SMCR_SPIRE | SPIBSC_SMCR_SPIWE);
    SPIBSC.SMCR.LONG = temp_reg;

    /* ---- SSL(KEEP) and SPIE---- */
    smcr_reg  = SPIBSC.SMCR.LONG;
    smcr_reg |= (SPIBSC_SMCR_SSLKP | SPIBSC_SMCR_SPIE);

    /* execute after setting SPNDL bit */
    SPIBSC.SMCR.LONG = smcr_reg;

    /* wait for transfer-start */
    R_SFLASH_WaitTend();

    temp_reg = SPIBSC.SMRDR0.LONG;

    return 0;
}

/******************************************************************************
* Function Name: spibsc_transfer_program_2
* Description  : Transmission setting of a SPI multi-I/O bus controller.
* Arguments    : uint32_t spide
*              : uint32_t spidb
*              : uint32_t *data
*              : int32_t  size
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t spibsc_transfer_program_2(uint32_t spide, uint32_t spidb, uint32_t *data, int32_t size)
{
    uint32_t    temp_reg;
    uint32_t    smcr_reg;

    /* ---- Command ---- */
    /* ---- Option Command ---- */
    /* ---- Address ---- */
    /* ---- Option Data ---- */
    temp_reg  = SPIBSC.SMENR.LONG;
    temp_reg &= ~(SPIBSC_SMENR_CDE | SPIBSC_SMENR_OCDE | SPIBSC_SMENR_ADE | SPIBSC_SMENR_OPDE | SPIBSC_SMENR_DME);
    SPIBSC.SMENR.LONG = temp_reg;

    /* ---- Data ---- */
    /* Single/Dual/Quad */
    SPIBSC.SMENR.BIT.SPIDB = spidb;
    SPIBSC.SMENR.BIT.SPIDE = spide;
    SPIBSC.SMCR.BIT.SPIRE  = SPIBSC_SPIDATA_DISABLE;
    SPIBSC.SMCR.BIT.SPIWE  = SPIBSC_SPIDATA_ENABLE;

    /* ---- SSL(KEEP) and SPIE---- */
    smcr_reg  = SPIBSC.SMCR.LONG;
    smcr_reg |= (SPIBSC_SMCR_SSLKP | SPIBSC_SMCR_SPIE);
    while ( size > 4 )
    {
        /* ---- Data ---- */
        SPIBSC.SMWDR0.LONG = *data++;

        /* execute after setting SPNDL bit */
        SPIBSC.SMCR.LONG = smcr_reg;

        size -= 4;

        /* wait for transfer-start */
        R_SFLASH_WaitTend();

        temp_reg = SPIBSC.SMRDR0.LONG;
    }

    /* ==== for last data ==== */

    /* ---- SSL(NEGATE) and SPIE---- */
    smcr_reg &= ~SPIBSC_SMCR_SSLKP;

    /* ---- Data ---- */
    SPIBSC.SMWDR0.LONG = *data;

    /* execute after setting SPNDL bit */
    SPIBSC.SMCR.LONG = smcr_reg;

    /* wait for transfer-start */
    R_SFLASH_WaitTend();

    temp_reg = SPIBSC.SMRDR0.LONG;

    return 0;
}

/******************************************************************************
* Function Name: io_spibsc_port_setting
* Description  : Port Settiong of SPIBSC
* Arguments    : uint32_t data_width
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
static int32_t io_spibsc_port_setting(uint32_t data_width)
{
    if (data_width == SPIBSC_4BIT)
    {
        spibsc_4bit_setting();
    }
    else
    {
        spibsc_1bit_setting();
    }

    return 0;
}

/******************************************************************************
* Function Name: spibsc_1bit_setting
* Description  : PFS Common Setting for SPIBSC
* Arguments    : none
* Return Value : none
******************************************************************************/
static void spibsc_1bit_setting(void)
{
    volatile uint8_t dummy;

    MPC.PWPR.BYTE = 0x00;
    dummy    = MPC.PWPR.BYTE;
    MPC.PWPR.BYTE = 0x40;
    dummy    = MPC.PWPR.BYTE;

    MPC.P60PFS.BYTE = MPC_PFS_SPIBSC;    /* SPBSSL */
    MPC.P62PFS.BYTE = MPC_PFS_SPIBSC;    /* SPBCLK */
    MPC.P63PFS.BYTE = MPC_PFS_SPIBSC;    /* SPBMO/SPBIO0 */
    MPC.P64PFS.BYTE = MPC_PFS_SPIBSC;    /* SPBMI/SPBIO1 */

    MPC.PWPR.BYTE = 0x80;
    dummy    = MPC.PWPR.BYTE;
    (void)dummy;						/* 未使用変数警告を出さない対策 */

    PORT6.PMR.BYTE |= 0x1d;   /* b'0001_1101  */
                              /* P60 : SPBSSL */
                              /* P62 : SPBCLK */
                              /* P63 : SPBMO/SPBIO0 */
                              /* P64 : SPBMI/SPBIO1 */
}

/******************************************************************************
* Function Name: spibsc_4bit_setting
* Description  : PFS Common Setting for SPIBSC 4bit
* Arguments    : none
* Return Value : none
******************************************************************************/
static void spibsc_4bit_setting(void)
{
    volatile uint8_t dummy;

    MPC.PWPR.BYTE = 0x00;
    dummy    = MPC.PWPR.BYTE;
    MPC.PWPR.BYTE = 0x40;
    dummy    = MPC.PWPR.BYTE;

    MPC.P60PFS.BYTE = MPC_PFS_SPIBSC;    /* SPBSSL */
    MPC.P61PFS.BYTE = MPC_PFS_SPIBSC;    /* SPBIO3 */
    MPC.P62PFS.BYTE = MPC_PFS_SPIBSC;    /* SPBCLK */
    MPC.P63PFS.BYTE = MPC_PFS_SPIBSC;    /* SPBIO0 */
    MPC.P64PFS.BYTE = MPC_PFS_SPIBSC;    /* SPBIO1 */
    MPC.P65PFS.BYTE = MPC_PFS_SPIBSC;    /* SPBIO2 */

    MPC.PWPR.BYTE = 0x80;
    dummy    = MPC.PWPR.BYTE;
    (void)dummy;						/* 未使用変数警告を出さない対策 */

    PORT6.PMR.BYTE |= 0x3f;   /* b'0011_1111  */
                              /* P60 : SPBSSL */
                              /* P61 : SPBIO3 */
                              /* P62 : SPBCLK */
                              /* P63 : SPBIO0 */
                              /* P64 : SPBIO1 */
                              /* P65 : SPBIO2 */
}

#endif


/* End of File */


/*----------------------------------------------------------------------*/
#if 0
#|History of "spibsc_ioset_drv.c"
#|====================
#|* 2016/04/08	Renesasの｢初期設定｣ｻﾝﾌﾟﾙｺｰﾄﾞから修正。(by T.Yokobayashi)
#|* 2016/04/08	(void)dummy;行を追加して、警告を出さないようにした。
#|
#endif
