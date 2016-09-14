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
* File Name    : spibsc_flash_api.c
* Version      : 1.2
* Device       : R7S910017
* Abstract     : API of SPIBSC Flash commands
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : Flash commands API for SPIBSC of RZ/T1 
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


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
st_spibsc_spimd_reg_t    g_spibsc_spimd_reg;


/******************************************************************************
Private global variables and functions
******************************************************************************/
int32_t R_SFLASH_EraseSector(uint32_t addr, uint32_t data_width, uint32_t addr_mode) __attribute__ ((section (".loader_text2")));
int32_t R_SFLASH_ByteProgram(uint32_t addr, uint8_t *buf, int32_t size, uint32_t data_width, uint32_t addr_mode) __attribute__ ((section (".loader_text2")));
int32_t R_SFLASH_Spibsc_Transfer(st_spibsc_spimd_reg_t *regset) __attribute__ ((section (".loader_text2")));
int32_t R_SFLASH_Ctrl_Protect(uint32_t req, uint32_t data_width) __attribute__ ((section (".loader_text2")));

/******************************************************************************
* Function Name: R_SFLASH_EraseSector
* Description  : Serial flash memory is sector erase
* Arguments    : uint32_t addr  : erase address(address of serial flash memory)
*              : uint32_t data_width
*              : uint32_t addr_mode
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SFLASH_EraseSector(uint32_t addr, uint32_t data_width, uint32_t addr_mode)
{
    int32_t     ret;
    uint32_t    cmd;

    /* sector erase in Single-SPI   */
    ret = Userdef_SFLASH_Write_Enable();    /* WREN Command */
    if (ret != 0)
    {
        return ret;
    }

    if (addr_mode == SPIBSC_OUTPUT_ADDR_32)
    {
        /* SE:Sector Erase(4-byte address) */
        cmd = SFLASHCMD_SECTOR_ERASE_4B;
    }
    else
    {
        /* SE:Sector Erase(3-byte address) */
        cmd = SFLASHCMD_SECTOR_ERASE;
    }

    ret = spibsc_transfer_erase(addr, cmd, addr_mode);
    if (ret != 0)
    {
        return ret;
    }

    ret = Userdef_SFLASH_Busy_Wait(data_width);
    if (ret != 0)
    {
        return ret;
    }

    return ret;
}

/******************************************************************************
* Function Name: R_SFLASH_ByteProgram
* Description  : Data of an argument is written on a serial flash memory.
*              : When 1 was designated by SPI_QUAD macro, the page program command
*              : corresponding to Quad is used.
*              : When 0 was designated by SPI_QUAD macro, The page program command
*              : corresponding to Single is used.
* Arguments    : uint32_t addr
*              : uint8_t  *buf
*              : int32_t  size
*              : uint32_t data_width
*              : uint32_t addr_mode
* Return Value :  0 : success
*              : -1 : error
* Limitation   : Followings are not support.
*                 Data width : 4bit
*                 Data size align : less than 4byte 
******************************************************************************/
int32_t R_SFLASH_ByteProgram(uint32_t addr, uint8_t *buf, int32_t size,
                             uint32_t data_width, uint32_t addr_mode)
{
    int32_t     ret;
    uint32_t    cmd;

    ret = Userdef_SFLASH_Write_Enable();    /* WREN Command */
    if (ret != 0)
    {
        return ret;
    }

    if (data_width == SPIBSC_1BIT)
    {
        if (addr_mode == SPIBSC_OUTPUT_ADDR_32)
        {
            /* PP: Page Program(4-byte address) */
            cmd = SFLASHCMD_BYTE_PROGRAM_4B;
        }
        else
        {
            /* PP: Page Program(3-byte address) */
            cmd = SFLASHCMD_BYTE_PROGRAM;
        }
    }
    else
    {
        /* Not Support 4bit Porgramming */
        return -1;
    }

    if ( (size % 4) != 0 )
    {
        return -1;
    }

    ret = spibsc_transfer_program_1(addr, cmd, addr_mode);
    if (ret != 0)
    {
        return ret;
    }

    ret = spibsc_transfer_program_2(SPIBSC_OUTPUT_SPID_32, data_width, (uint32_t *)buf, size);
    if (ret != 0)
    {
        return ret;
    }

    /* wait for busy each page size */
    ret = Userdef_SFLASH_Busy_Wait(data_width);

    return ret;
}

/******************************************************************************
* Function Name: R_SFLASH_Spibsc_Transfer
* Description  : Transmission setting of a SPI multi-I/O bus controller.
* Arguments    : void
*              : st_spibsc_spimd_reg_t *regset
*              : The pointer to a structure for the transfer
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SFLASH_Spibsc_Transfer(st_spibsc_spimd_reg_t *regset)
{
    return spibsc_transfer(regset);
}

/******************************************************************************
* Function Name: R_SFLASH_Ctrl_Protect
* Description  : Protection of a cereal flash memory is released or set.
* Arguments    : uint32_t req : 
*              :     SF_REQ_UNPROTECT -> clear all sector protection
*              :     SF_REQ_PROTECT   -> protect all sectors
*              : uint32_t data_width
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SFLASH_Ctrl_Protect(uint32_t req, uint32_t data_width)
{
    return Userdef_SFLASH_Ctrl_Protect(req, data_width);
}

#endif


/* End of File */

