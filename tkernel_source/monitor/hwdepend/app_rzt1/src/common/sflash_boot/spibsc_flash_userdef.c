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
* File Name    : spibsc_flash_userdef.c
* Version      : 1.2
* Device       : R7S910017
* Abstract     : User define program for serial flash settings
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : User define program for serial flash by using SPIBSC of RZ/T1 
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
#include "iodefine.h"
#include "r_spibsc_flash_api.h"


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
extern st_spibsc_spimd_reg_t g_spibsc_spimd_reg;


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static int32_t set_mode(uint32_t data_width, uint32_t addr_mode) __attribute__ ((section (".loader_text2")));
static int32_t read_status(uint8_t *status, uint32_t data_width) __attribute__ ((section (".loader_text2")));
static int32_t read_config(uint8_t *config, uint32_t data_width) __attribute__ ((section (".loader_text2")));
static int32_t write_status(uint8_t status, uint8_t config, uint32_t data_width) __attribute__ ((section (".loader_text2")));

static int32_t spibsc_transfer_write_enable(void) __attribute__ ((section (".loader_text2")));
static int32_t spibsc_transfer_read_status(uint32_t *data) __attribute__ ((section (".loader_text2")));

int32_t Userdef_SFLASH_Set_Mode(uint32_t data_width, uint32_t addr_mode) __attribute__ ((section (".loader_text2")));
int32_t Userdef_SFLASH_Write_Enable(void) __attribute__ ((section (".loader_text2")));
int32_t Userdef_SFLASH_Busy_Wait(uint32_t data_width) __attribute__ ((section (".loader_text2")));
int32_t Userdef_SFLASH_Ctrl_Protect(uint32_t req, uint32_t data_width) __attribute__ ((section (".loader_text2")));

/******************************************************************************
* Function Name: Userdef_SFLASH_Set_Mode
* Description  : The setting function of serial flash memory dependence
* Arguments    : uint32_t data_width
*              : uint32_t addr_mode
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t Userdef_SFLASH_Set_Mode(uint32_t data_width, uint32_t addr_mode)
{
    int32_t    ret;

    ret = set_mode(data_width, addr_mode);

    return ret;
}

/******************************************************************************
* Function Name: Userdef_SFLASH_Write_Enable
* Description  : Issuing the write enable command to permit to erase/program
*              : in the serial flash memory
* Arguments    : void
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t Userdef_SFLASH_Write_Enable(void)
{
    int32_t    ret;

    ret = spibsc_transfer_write_enable();

    return ret;
}

/******************************************************************************
* Function Name: spibsc_transfer_write_enable
* Description  : Transmission setting of a SPI multi-I/O bus controller.
* Arguments    : void
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
static int32_t spibsc_transfer_write_enable(void)
{
    uint32_t    temp_reg;
    uint32_t    smcr_reg;

    /* ---- Command ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.CDE = SPIBSC_OUTPUT_ENABLE;
    /* Single/Dual/Quad */
    SPIBSC.SMENR.BIT.CDB = SPIBSC_1BIT;
    /* Command */
    SPIBSC.SMCMR.BIT.CMD = SFLASHCMD_WRITE_ENABLE;

    /* ---- Option Command ---- */
    /* ---- Address ---- */
    /* ---- Option Data ---- */
    /* ---- Dummy ---- */
    /* ---- Data ---- */
    temp_reg  = SPIBSC.SMENR.LONG;
    temp_reg &= ~(SPIBSC_SMENR_OCDE | SPIBSC_SMENR_ADE | SPIBSC_SMENR_OPDE | SPIBSC_SMENR_DME | SPIBSC_SMENR_SPIDE);
    SPIBSC.SMENR.LONG = temp_reg;

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
* Function Name: Userdef_SFLASH_Busy_Wait
* Description  : Loops internally when the serial flash memory is busy.
* Arguments    : uint32_t data_width
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t Userdef_SFLASH_Busy_Wait(uint32_t data_width)
{
    uint8_t    st_reg;
    int32_t    ret;

    st_reg = 0;							/* 未初期化警告を出さない対策 */
    while (1)
    {
        ret = read_status(&st_reg, data_width);

        /* s-flash */
        if ( (st_reg & STREG_WIP_BIT) == 0 )
        {
            break;
        }

        /* serial flash is busy */
    }

    return ret;
}

/******************************************************************************
* Function Name: Userdef_SFLASH_Ctrl_Protect
* Description  : Protection of a cereal flash memory is released or set.
* Arguments    : uint32_t req : 
*              :     SF_REQ_UNPROTECT -> clear all sector protection
*              :     SF_REQ_PROTECT   -> protect all sectors
*              : uint32_t data_width
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t Userdef_SFLASH_Ctrl_Protect(uint32_t req, uint32_t data_width)
{
    uint8_t    st_reg;
    uint8_t    cf_reg;
    int32_t    ret;

    ret = read_status(&st_reg, data_width);
    if (ret != 0)
    {
        return ret;
    }

    ret = read_config(&cf_reg, data_width);
    if (ret != 0)
    {
        return ret;
    }

    if ( req == SF_REQ_UNPROTECT )
    {
        /* bit5  bit4  bit3  bit2 */
        /* BP3   BP2   BP1   BP0  */
        if ( (st_reg & STREG_BPROTECT_BIT) != 0 )
        {
            st_reg &= (~STREG_BPROTECT_BIT);    /* un-protect in all area   */
            /* ---- clear protect bit in status register ---- */
            ret = write_status(st_reg, cf_reg, data_width);
        }
    }
    else
    {
        st_reg |= STREG_BPROTECT_BIT;           /* protect in all area      */
        /* ---- set protect bit in status register ---- */
        ret = write_status(st_reg, cf_reg, data_width);
    }

    return ret;
}

/******************************************************************************
* Function Name: set_mode
* Description  : Serial flash memory mode setting
* Arguments    : uint32_t data_width
*              : uint32_t addr_mode
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
static int32_t set_mode(uint32_t data_width, uint32_t addr_mode)
{
    uint8_t    st_reg;
    uint8_t    cf_reg;
    int32_t    ret;

    ret = read_status(&st_reg, data_width);
    if (ret != 0)
    {
        return ret;
    }

    ret = read_config(&cf_reg, data_width);
    if (ret != 0)
    {
        return ret;
    }

    /* set serial-flash(1) same value of serial-flash(0) */
    if (data_width == SPIBSC_1BIT)
    {
        /* ---- set serial mode ---- */
        st_reg = (st_reg & (~STREG_QUAD_BIT));
    }
    else if (data_width == SPIBSC_4BIT)
    {
        /* ---- set quad mode ---- */
        st_reg = (st_reg | STREG_QUAD_BIT);
    }
    else
    {
        return -1;
        /* error in argument */
    }

    /* Latency Code = b'00 fixing */
    cf_reg = (cf_reg & ~CFREG_LC_BIT);
    cf_reg = (cf_reg | 0x80);    /* Dummy cycle is 8cycle */

    ret = write_status(st_reg, cf_reg, data_width);
    if (ret != 0)
    {
        return ret;
    }

    return ret;
}

/******************************************************************************
* Function Name: read_status
* Description  : Reads the status of serial flash memory.
* Arguments    : uint8_t  *status
*              : uint32_t data_width
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
static int32_t read_status(uint8_t *status, uint32_t data_width)
{
    int32_t     ret;
    uint32_t    data;

    ret = spibsc_transfer_read_status(&data);
    if (ret != 0)
    {
        return ret;
    }

    /* SFDE=1 [D3][D2][D1][D0] */
    /* Read Status Register (RDSR) output same DATA in [D3][D2][D1][D0] */
    *status = (uint8_t)(data >> 24);         /* store [D3] Data[31:24]  */

    return ret;
}

/******************************************************************************
* Function Name: spibsc_transfer_read_status
* Description  : Transmission setting of a SPI multi-I/O bus controller.
* Arguments    : uint32_t *data0 : SMRDR0
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
static int32_t spibsc_transfer_read_status(uint32_t *data)
{
    uint32_t    temp_reg;
    uint32_t    smcr_reg;

    /* ---- Command ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.CDE = SPIBSC_OUTPUT_ENABLE;
    /* Single/Dual/Quad */
    SPIBSC.SMENR.BIT.CDB = SPIBSC_1BIT;
    /* Command */
    SPIBSC.SMCMR.BIT.CMD = SFLASHCMD_READ_STATUS;

    /* ---- Option Command ---- */
    /* ---- Address ---- */
    /* ---- Option Data ---- */
    /* ---- Dummy ---- */
    temp_reg  = SPIBSC.SMENR.LONG;
    temp_reg &= ~(SPIBSC_SMENR_OCDE | SPIBSC_SMENR_ADE | SPIBSC_SMENR_OPDE | SPIBSC_SMENR_DME);
    SPIBSC.SMENR.LONG = temp_reg;

    /* ---- Data ---- */
    /* Enable/Disable */
    SPIBSC.SMENR.BIT.SPIDE = SPIBSC_OUTPUT_SPID_8;
    /* Single/Dual/Quad */
    SPIBSC.SMENR.BIT.SPIDB = SPIBSC_1BIT;
    /* Data */
    SPIBSC.SMWDR0.LONG = 0x00000000;

    /* ---- SPIRE(enable) and SPIWE(enable) ---- */
    temp_reg  = SPIBSC.SMCR.LONG;
    temp_reg |= (SPIBSC_SMCR_SPIRE | SPIBSC_SMCR_SPIWE);
    SPIBSC.SMCR.LONG = temp_reg;

    /* ---- SSL(NEGATE) and SPIE---- */
    smcr_reg  = SPIBSC.SMCR.LONG;
    smcr_reg &= ~SPIBSC_SMCR_SSLKP;
    smcr_reg |= SPIBSC_SMCR_SPIE;

    /* execute after setting SPNDL bit */
    SPIBSC.SMCR.LONG = smcr_reg;

    /* wait for transfer-start */
    R_SFLASH_WaitTend();

    /* SFDE=1 [D3][D2][D1][D0] */
    *data = SPIBSC.SMRDR0.LONG;

    return 0;
}

/******************************************************************************
* Function Name: read_config
* Description  : Reads the serial flash memory configuration.
* Arguments    : uint8_t  *config
*              : uint32_t data_width
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
static int32_t read_config(uint8_t *config, uint32_t data_width)
{
    int32_t    ret;

    /* read connfig in Single-SPI   */
    g_spibsc_spimd_reg.cdb   = SPIBSC_1BIT;                /* Commmand bit-width = Single */
    g_spibsc_spimd_reg.spidb = SPIBSC_1BIT;                /* Data bit-width = Single */
    g_spibsc_spimd_reg.cde   = SPIBSC_OUTPUT_ENABLE;       /* Command Enable */
    g_spibsc_spimd_reg.ocde  = SPIBSC_OUTPUT_DISABLE;      /* Optional-Command Disable */
    g_spibsc_spimd_reg.ade   = SPIBSC_OUTPUT_DISABLE;      /* Address Disable */
    g_spibsc_spimd_reg.opde  = SPIBSC_OUTPUT_DISABLE;      /* Option-Data Disable */

    g_spibsc_spimd_reg.spide = SPIBSC_OUTPUT_SPID_8;       /* Enable(8bit) */

    g_spibsc_spimd_reg.sslkp = SPIBSC_SPISSL_NEGATE;       /* Negate after transfer */
    g_spibsc_spimd_reg.spire = SPIBSC_SPIDATA_ENABLE;      /* Data Access (Read Enable) */
    g_spibsc_spimd_reg.spiwe = SPIBSC_SPIDATA_ENABLE;      /* Data Access (Write Enable) */
    g_spibsc_spimd_reg.cmd   = SFLASHCMD_READ_CONFIG;      /* RCR:Read Configuration Register */
    g_spibsc_spimd_reg.smwdr = 0x00;                       /* Output 0 in read config */

    /* SDR/DDR setting */
    g_spibsc_spimd_reg.dme    = SPIBSC_DUMMY_CYC_DISABLE;  /* Dummy cycle disable */
    g_spibsc_spimd_reg.addre  = SPIBSC_SDR_TRANS;          /* address    :SDR transmission */
    g_spibsc_spimd_reg.opdre  = SPIBSC_SDR_TRANS;          /* option data:SDR transmission */
    g_spibsc_spimd_reg.spidre = SPIBSC_SDR_TRANS;          /* data       :SDR transmission */

    ret = R_SFLASH_Spibsc_Transfer(&g_spibsc_spimd_reg);
    if (ret != 0)
    {
        return ret;
    }

    *config = (uint8_t)(g_spibsc_spimd_reg.smrdr >> 24);   /* Data[31:24]  */

    return ret;
}

/******************************************************************************
* Function Name: write_status
* Description  : Programs the status and configuration of the serial flash memory.
* Arguments    : uint8_t  status
*              : uint8_t  config
*              : uint32_t data_width
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
static int32_t write_status(uint8_t status, uint8_t config, uint32_t data_width)
{
    int32_t    ret;

    /* write status in Single-SPI   */
    ret = Userdef_SFLASH_Write_Enable();    /* WREN Command */
    if (ret != 0)
    {
        return ret;
    }

    g_spibsc_spimd_reg.cdb   = SPIBSC_1BIT;               /* Commmand bit-width = Single */
    g_spibsc_spimd_reg.opdb  = SPIBSC_1BIT;               /* Optional-Data bit-width = Single */
    g_spibsc_spimd_reg.cde   = SPIBSC_OUTPUT_ENABLE;      /* Command Enable */
    g_spibsc_spimd_reg.ocde  = SPIBSC_OUTPUT_DISABLE;     /* Optional-Command Disable */
    g_spibsc_spimd_reg.ade   = SPIBSC_OUTPUT_DISABLE;     /* Address Disable */
    g_spibsc_spimd_reg.opde  = SPIBSC_OUTPUT_OPD_32;      /* Option-Data OPD3, OPD2 */
    g_spibsc_spimd_reg.spide = SPIBSC_OUTPUT_DISABLE;     /* Disable */
    g_spibsc_spimd_reg.sslkp = SPIBSC_SPISSL_NEGATE;      /* Negate after transfer */
    g_spibsc_spimd_reg.spire = SPIBSC_SPIDATA_DISABLE;    /* Data Access (Read Disable) */
    g_spibsc_spimd_reg.spiwe = SPIBSC_SPIDATA_DISABLE;    /* Data Access (Write Disable) */
    g_spibsc_spimd_reg.cmd   = SFLASHCMD_WRITE_STATUS;    /* WRR:Write Register */
    g_spibsc_spimd_reg.opd[0] = (uint32_t)status;
    g_spibsc_spimd_reg.opd[1] = (uint32_t)config;
    g_spibsc_spimd_reg.opd[2] = (uint32_t)0x00;
    g_spibsc_spimd_reg.opd[3] = (uint32_t)0x00;

    /* SDR setting */
    g_spibsc_spimd_reg.dme    = SPIBSC_DUMMY_CYC_DISABLE; /* Dummy cycle disable */
    g_spibsc_spimd_reg.addre  = SPIBSC_SDR_TRANS;         /* address    :SDR transmission */
    g_spibsc_spimd_reg.opdre  = SPIBSC_SDR_TRANS;         /* option data:SDR transmission */
    g_spibsc_spimd_reg.spidre = SPIBSC_SDR_TRANS;         /* data       :SDR transmission */

    ret = R_SFLASH_Spibsc_Transfer(&g_spibsc_spimd_reg);
    if (ret != 0)
    {
        return ret;
    }

    ret = Userdef_SFLASH_Busy_Wait(data_width);

    return ret;
}

#endif


/* End of File */


/*----------------------------------------------------------------------*/
#if 0
#|History of "spibsc_flash_userdef.c"
#|====================
#|* 2016/04/08	Renesasの｢初期設定｣ｻﾝﾌﾟﾙｺｰﾄﾞから修正。(by T.Yokobayashi)
#|
#endif
