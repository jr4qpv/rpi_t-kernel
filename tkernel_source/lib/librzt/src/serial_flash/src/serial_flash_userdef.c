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
* File Name    : serial_flash_userdef.c
* Version      : 1.1
* Device       : R7S910017
* Abstract     : User define program for serial flash settings
* Tool-Chain   : ARM DS-5 Development Studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : User define program for serial flash by using SPIBSC of RZ/T1 
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
#include "r_serial_flash_api.h"
#include "serial_flash.h"
#include "serial_flash_spibsc.h"
#include "serial_flash_iobitmask.h"


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
static int32_t set_mode(uint32_t data_width, uint32_t addr_mode);
static int32_t read_status(uint8_t *status, uint32_t data_width);
static int32_t read_config(uint8_t *config, uint32_t data_width);
static int32_t write_status(uint8_t status, uint8_t config, uint32_t data_width);
static int32_t spibsc_transfer_write_enable(void);
static int32_t spibsc_transfer_read_status(uint32_t *data);
extern st_spibsc_spimd_reg_t g_spimd_reg;

/******************************************************************************
* Function Name: userdef_sflash_set_mode
* Description  : The setting function of serial flash memory dependence
* Arguments    : uint32_t data_width
*              : uint32_t addr_mode
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t userdef_sflash_set_mode(uint32_t data_width, uint32_t addr_mode)
{
    int32_t    ret;

    ret = set_mode(data_width, addr_mode);

    return ret;
} /* End of function userdef_sflash_set_mode() */


/******************************************************************************
* Function Name: userdef_sflash_write_enable
* Description  : Issuing the write enable command to permit to erase/program
*              : in the serial flash memory
* Arguments    : void
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t userdef_sflash_write_enable(void)
{
    int32_t    ret;

    ret = spibsc_transfer_write_enable();

    return ret;
} /* End of function userdef_sflash_write_enable() */

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
    SPIBSC.SMENR.BIT.CDE = SPIBSC_OUTPUT_ENABLE;	/* Enable/Disable */
    SPIBSC.SMENR.BIT.CDB = SPIBSC_1BIT;				/* Single/Dual/Quad */
    SPIBSC.SMCMR.BIT.CMD = SFLASHCMD_WRITE_ENABLE;	/* Command */

    /* ---- Option Command ---- */
    /* ---- Address ---- */
    /* ---- Option Data ---- */
    /* ---- Dummy ---- */
    /* ---- Data ---- */
    temp_reg  = SPIBSC.SMENR.LONG;
    temp_reg &= (~(((((SPIBSC_SMENR_OCDE | SPIBSC_SMENR_ADE)
    		| SPIBSC_SMENR_OPDE) | SPIBSC_SMENR_DME) | SPIBSC_SMENR_SPIDE)));
    SPIBSC.SMENR.LONG = temp_reg;

    /* ---- SPIRE(disable) and SPIWE(disable) ---- */
    temp_reg  = SPIBSC.SMCR.LONG;
    temp_reg &= (~(SPIBSC_SMCR_SPIRE | SPIBSC_SMCR_SPIWE));
    SPIBSC.SMCR.LONG = temp_reg;

    /* ---- SSL(NEGATE) and SPIE---- */
    smcr_reg  = SPIBSC.SMCR.LONG;
    smcr_reg &= (~SPIBSC_SMCR_SSLKP);
    smcr_reg |= SPIBSC_SMCR_SPIE;

    /* execute after setting SPNDL bit */
    SPIBSC.SMCR.LONG = smcr_reg;

    /* wait for transfer-start */
    R_SPIBSC_WaitTend();

    temp_reg = SPIBSC.SMRDR0.LONG;

    return 0;
} /* End of function spibsc_transfer_write_enable() */

/******************************************************************************
* Function Name: userdef_sflash_busy_wait
* Description  : Loops internally when the serial flash memory is busy.
* Arguments    : uint32_t data_width
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t userdef_sflash_busy_wait(uint32_t data_width)
{
    uint8_t    st_reg;
    int32_t    ret;

    while (1)
    {
        ret = read_status(&st_reg, data_width);

        /* s-flash */
        if ( (st_reg & (uint8_t)STREG_WIP_BIT) == 0 )
        {
            break;
        }

        /* serial flash is busy */
    }

    return ret;
} /* End of function userdef_sflash_busy_wait() */

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
    if (0 == ret)
    {
        ret = read_config(&cf_reg, data_width);
        if (0 == ret)
        {
            /* set serial-flash(1) same value of serial-flash(0) */
            if ((uint32_t)SPIBSC_1BIT == data_width)
            {
                /* ---- set serial mode ---- */
                st_reg = (st_reg & (uint8_t)(~STREG_QUAD_BIT));    
            }
            else if ((uint32_t)SPIBSC_4BIT == data_width)
            {
                /* ---- set quad mode ---- */
                st_reg = (st_reg | (uint8_t)STREG_QUAD_BIT);
            }
            else 
            {
                ret = -1;	/* error in argument */
            }
            
            if(0 == ret)
            {
                /* Latency Code = b'00 fixing */
                cf_reg = (cf_reg & (uint8_t)(~CFREG_LC_BIT));
                cf_reg = (cf_reg | (uint8_t)0x80);    /* Dummy cycle is 8cycle */

                ret = write_status(st_reg, cf_reg, data_width);
            }
        }
    }

    return ret;
} /* End of function set_mode() */

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
    if (0 == ret)
    {
        /* SFDE=1 [D3][D2][D1][D0] */
        /* Read Status Register (RDSR) output same DATA in [D3][D2][D1][D0] */
        *status = (uint8_t)(data >> 24);         /* store [D3] Data[31:24]  */
    }

    return ret;
} /* End of function read_status() */

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
    SPIBSC.SMENR.BIT.CDE = SPIBSC_OUTPUT_ENABLE;	/* Enable/Disable */
    SPIBSC.SMENR.BIT.CDB = SPIBSC_1BIT;			/* Single/Dual/Quad */
    SPIBSC.SMCMR.BIT.CMD = SFLASHCMD_READ_STATUS;	/* Command */

    /* ---- Option Command ---- */
    /* ---- Address ---- */
    /* ---- Option Data ---- */
    /* ---- Dummy ---- */
    temp_reg  = SPIBSC.SMENR.LONG;
    temp_reg &= (~((((SPIBSC_SMENR_OCDE | SPIBSC_SMENR_ADE) | SPIBSC_SMENR_OPDE) | SPIBSC_SMENR_DME)));
    SPIBSC.SMENR.LONG = temp_reg;

    /* ---- Data ---- */
    SPIBSC.SMENR.BIT.SPIDE = SPIBSC_OUTPUT_SPID_8;	/* Enable/Disable */
    SPIBSC.SMENR.BIT.SPIDB = SPIBSC_1BIT;			/* Single/Dual/Quad */
    SPIBSC.SMWDR0.LONG = 0x00000000U;				/* Data */

    /* ---- SPIRE(enable) and SPIWE(enable) ---- */
    temp_reg  = SPIBSC.SMCR.LONG;
    temp_reg |= (SPIBSC_SMCR_SPIRE | SPIBSC_SMCR_SPIWE);
    SPIBSC.SMCR.LONG = temp_reg;

    /* ---- SSL(NEGATE) and SPIE---- */
    smcr_reg  = SPIBSC.SMCR.LONG;
    smcr_reg &= (~SPIBSC_SMCR_SSLKP);
    smcr_reg |= SPIBSC_SMCR_SPIE;

    /* execute after setting SPNDL bit */
    SPIBSC.SMCR.LONG = smcr_reg;

    /* wait for transfer-start */
    R_SPIBSC_WaitTend();

    /* SFDE=1 [D3][D2][D1][D0] */
    *data = SPIBSC.SMRDR0.LONG;

    return 0;
} /* End of function spibsc_transfer_read_status() */

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
    g_spimd_reg.cdb   = (uint32_t)SPIBSC_1BIT;                /* Command bit-width = Single */
    g_spimd_reg.spidb = (uint32_t)SPIBSC_1BIT;                /* Data bit-width = Single */
    g_spimd_reg.cde   = (uint32_t)SPIBSC_OUTPUT_ENABLE;       /* Command Enable */
    g_spimd_reg.ocde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;      /* Optional-Command Disable */
    g_spimd_reg.ade   = (uint32_t)SPIBSC_OUTPUT_DISABLE;      /* Address Disable */
    g_spimd_reg.opde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;      /* Option-Data Disable */

    g_spimd_reg.spide = (uint32_t)SPIBSC_OUTPUT_SPID_8;       /* Enable(8bit) */

    g_spimd_reg.sslkp = (uint32_t)SPIBSC_SPISSL_NEGATE;       /* Negate after transfer */
    g_spimd_reg.spire = (uint32_t)SPIBSC_SPIDATA_ENABLE;      /* Data Access (Read Enable) */
    g_spimd_reg.spiwe = (uint32_t)SPIBSC_SPIDATA_ENABLE;      /* Data Access (Write Enable) */
    g_spimd_reg.cmd   = (uint32_t)SFLASHCMD_READ_CONFIG;      /* RCR:Read Configuration Register */
    g_spimd_reg.smwdr = (uint32_t)0x00;                       /* Output 0 in read config */

    /* SDR/DDR setting */
    g_spimd_reg.dme    = (uint32_t)SPIBSC_DUMMY_CYC_DISABLE;  /* Dummy cycle disable */

    ret = R_SPIBSC_SpibscTransfer(&g_spimd_reg);
    if (0 == ret)
    {
        *config = (uint8_t)(g_spimd_reg.smrdr >> 24);   /* Data[31:24]  */
    }

    return ret;
    
} /* End of function read_config() */

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
    ret = userdef_sflash_write_enable();    /* WREN Command */
    if (0 == ret)
    {
        g_spimd_reg.cdb   = (uint32_t)SPIBSC_1BIT;               /* Command bit-width = Single */
        g_spimd_reg.opdb  = (uint32_t)SPIBSC_1BIT;               /* Optional-Data bit-width = Single */
        g_spimd_reg.cde   = (uint32_t)SPIBSC_OUTPUT_ENABLE;      /* Command Enable */
        g_spimd_reg.ocde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;     /* Optional-Command Disable */
        g_spimd_reg.ade   = (uint32_t)SPIBSC_OUTPUT_DISABLE;     /* Address Disable */
        g_spimd_reg.opde  = (uint32_t)SPIBSC_OUTPUT_OPD_32;      /* Option-Data OPD3, OPD2 */
        g_spimd_reg.spide = (uint32_t)SPIBSC_OUTPUT_DISABLE;     /* Disable */
        g_spimd_reg.sslkp = (uint32_t)SPIBSC_SPISSL_NEGATE;      /* Negate after transfer */
        g_spimd_reg.spire = (uint32_t)SPIBSC_SPIDATA_DISABLE;    /* Data Access (Read Disable) */
        g_spimd_reg.spiwe = (uint32_t)SPIBSC_SPIDATA_DISABLE;    /* Data Access (Write Disable) */
        g_spimd_reg.cmd   = (uint32_t)SFLASHCMD_WRITE_STATUS;    /* WRR:Write Register */
        g_spimd_reg.opd[0] = (uint32_t)status;
        g_spimd_reg.opd[1] = (uint32_t)config;
        g_spimd_reg.opd[2] = (uint32_t)0x00;
        g_spimd_reg.opd[3] = (uint32_t)0x00;

        /* SDR setting */
        g_spimd_reg.dme    = (uint32_t)SPIBSC_DUMMY_CYC_DISABLE; /* Dummy cycle disable */

        ret = R_SPIBSC_SpibscTransfer(&g_spimd_reg);
        if (0 == ret)
        {
            ret = userdef_sflash_busy_wait(data_width);
        }
        
    }

    return ret;
} /* End of function write_status() */

#endif

/* End of File */

