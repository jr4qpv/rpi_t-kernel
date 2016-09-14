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
* File Name    : spibsc_flash_api.c
* Version      : 1.1
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
*              : 22.12.2015 1.0      First Release
*              : 18.03.2016 1.1      Second Release
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"
#include "iodefine.h"
#include "r_serial_flash_api.h"
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
static int32_t read_data_quad(uint32_t addr, uint8_t * buf, int32_t unit, int32_t addr_mode);
static int32_t read_data_single(uint32_t addr, uint8_t * buf, int32_t unit, int32_t addr_mode);
st_spibsc_spimd_reg_t    g_spimd_reg;

/******************************************************************************
* Function Name: R_SPIBSC_EraseSector
* Description  : Serial flash memory is sector erase
* Arguments    : uint32_t addr  : erase address(address of serial flash memory)
*              : uint32_t data_width
*              : uint32_t addr_mode
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SPIBSC_EraseSector(uint32_t addr, uint32_t data_width, uint32_t addr_mode)
{
    int32_t     ret;
    uint32_t    cmd;

    
    /* sector erase in Single-SPI   */
    ret = userdef_sflash_write_enable();    /* WREN Command */
    if (0 == ret)
    {
        if ((uint32_t)SPIBSC_OUTPUT_ADDR_32 == addr_mode)
        {
            /* SE:Sector Erase(4-byte address) */
            cmd = (uint32_t)SFLASHCMD_SECTOR_ERASE_4B;
        }
        else
        {
            /* SE:Sector Erase(3-byte address) */
            cmd = (uint32_t)SFLASHCMD_SECTOR_ERASE;
        }

        ret = sflash_transfer_erase(addr, cmd, addr_mode);
        if (0 == ret)
        {
            ret = userdef_sflash_busy_wait(data_width);
        }
    }
    
    return ret;
} /* End of function R_SPIBSC_EraseSector() */

/******************************************************************************
* Function Name: R_SPIBSC_ByteProgram
* Description  : Data of an argument is written on a serial flash memory.
*              : When 1 was designated by SPI_QUAD macro, the page program command
*              : corresponding to Quad is used.
*              : When 0 was designated by SPI_QUAD macro, The page program command
*              : corresponding to Single is used.
* Arguments    : uint32_t addr
*              : uint32_t  *buf
*              : int32_t  size
*              : uint32_t data_width
*              : uint32_t addr_mode
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SPIBSC_ByteProgram(uint32_t addr, uint8_t *buf, int32_t size,
                             uint32_t data_width, uint32_t addr_mode)
{
    int32_t     ret;
    uint32_t    cmd;
    
    ret = userdef_sflash_write_enable();    /* WREN Command */
    if (0 == ret)
    {
        if ((uint32_t)SPIBSC_1BIT == data_width)
        {
            if ((uint32_t)SPIBSC_OUTPUT_ADDR_32 == addr_mode)
            {
                /* PP: Page Program(4-byte address) */
                cmd = (uint32_t)SFLASHCMD_BYTE_PROGRAM_4B;
            }
            else
            {
                /* PP: Page Program(3-byte address) */
                cmd = (uint32_t)SFLASHCMD_BYTE_PROGRAM;
            }
        }
        else
        {
            if ((uint32_t)SPIBSC_OUTPUT_ADDR_32 == addr_mode)
            {
                /* PP: Page Program(4-byte address) */
                cmd = (uint32_t)SFLASHCMD_QUAD_PROGRAM_4B;
            }
            else
            {
                /* PP: Page Program(3-byte address) */
                cmd = (uint32_t)SFLASHCMD_QUAD_PROGRAM;
            }  
        }
                     
        if ( (size % 4) == 0 )
        {
            ret = (int32_t)sflash_transfer_program_1(addr, cmd, addr_mode);
            if (0 == ret)
            {
                ret = (int32_t)sflash_transfer_program_2((uint32_t)SPIBSC_OUTPUT_SPID_32, data_width, (uint32_t *)buf, size);
                if (0 == ret)
                {
                    /* wait for busy each page size */
                    ret = userdef_sflash_busy_wait(data_width);
                }
            }    
        }
        else
        {
          /* size must be multiples of 4byte */
          ret = -1;
        }
    }
    
    return ret;

} /* End of function R_SPIBSC_ByteProgram() */

/******************************************************************************
* Function Name: R_SPIBSC_ByteRead
* Description  : The range of the serial flash memory designated by an argument
*                is read and it's stocked in a buffer.
*                When 1 was designated by SPI_QUAD macro, read command corresponding to Quad is used.
*                When 0 was designated by SPI_QUAD macro, read command corresponding to Single is used.
* Arguments    : uint32_t addr  : read address(address of serial flash memory)
*                uint8_t  *buf  : Start address in a buffer
*                int32_t  size  : The number of data byte
*                uint32_t data_width
*                uint32_t addr_mode
* Return Value :  0 : success
*                -1 : error
******************************************************************************/
int32_t R_SPIBSC_ByteRead(uint32_t addr, uint8_t * buf, int32_t size,
                          uint32_t data_width, uint32_t addr_mode)
{
    int32_t ret = 0;
    int32_t unit;
    

    if ( ( ((uint32_t)size) & ((uint32_t)0x3) ) == (uint32_t)0 )
    {
        unit = 4;
    }
    else if ( ( ((uint32_t)size) & ((uint32_t)0x1) ) == (uint32_t)0)
    {
        unit = 2;
    }
    else
    {
        unit = 1;
    }
	
    while (size > 0)
    {
        if((uint32_t)SPIBSC_1BIT == data_width)
        {
            ret = read_data_single(addr, buf, unit, (int32_t)addr_mode);
        }
        else
        {
            ret = read_data_quad(addr, buf, unit, (int32_t)addr_mode);
        }
        if(0 == ret)
        {
            break;
        }

        /* increment address and buf */
        addr += (uint32_t)unit;
        buf  += unit;
        size -= unit;
    }
    
    
    return ret;

} /* End of function R_SPIBSC_ByteRead() */

/******************************************************************************
* Function Name: R_SPIBSC_WaitTend
* Description  : Wait TEND
* Arguments    :  void
* Return Value :  void
******************************************************************************/
void R_SPIBSC_WaitTend(void)
{
    sflash_wait_tend();
    
} /* End of function R_SPIBSC_WaitTend() */


/******************************************************************************
* Function Name: R_SPIBSC_SpibscTransfer
* Description  : Transmission setting of a SPI multi-I/O bus controller.
* Arguments    : void
*              : st_spibsc_spimd_reg_t *regset
*              : The pointer to a structure for the transfer
* Return Value :  0 : success
*              : -1 : error
******************************************************************************/
int32_t R_SPIBSC_SpibscTransfer(st_spibsc_spimd_reg_t *regset)
{
    return sflash_transfer(regset);
  
} /* End of function R_SPIBSC_SpibscTransfer() */

/******************************************************************************
* Function Name: read_data_quad
* Description  : The range of the serial flash memory designated by an argument
*                is read and it's stocked in a buffer.
*                An operation mode is performed by a Quad mode.
* Arguments    : uint32_t addr  : read address(address of serial flash memory)
*                uint32_t *buf   : Start address in a buffer
*                int32_t  unit  : data size
*                int32_t  addr_mode
* Return Value :  0 : success
*                -1 : error
******************************************************************************/
static int32_t read_data_quad(uint32_t addr, uint8_t * buf, int32_t unit, int32_t addr_mode)
{
    int32_t ret = 0;
    uint8_t  *buf_b;
    uint16_t *buf_s;
    uint32_t *buf_l;

   /* ---- Command,Address,Dummy ---- */
    g_spimd_reg.cdb   = (uint32_t)SPIBSC_1BIT;            /* Commmand bit-width = Single */
    g_spimd_reg.adb   = (uint32_t)SPIBSC_1BIT;            /* Address bit-width = Single */
    g_spimd_reg.cde   = (uint32_t)SPIBSC_OUTPUT_ENABLE;   /* Command Enable */
    g_spimd_reg.ocde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Optional-Command Disable */
    g_spimd_reg.ade   = (uint32_t)addr_mode;

    g_spimd_reg.opde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Option-Data Disable */

    g_spimd_reg.spide = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Disable */
    g_spimd_reg.sslkp = (uint32_t)SPIBSC_SPISSL_KEEP;     /* Keep after transfer */
    g_spimd_reg.spire = (uint32_t)SPIBSC_SPIDATA_DISABLE; /* Data Access (Read Disable) */
    g_spimd_reg.spiwe = (uint32_t)SPIBSC_SPIDATA_DISABLE; /* Data Access (Write Disable) */

    if(SPIBSC_OUTPUT_ADDR_32 == addr_mode)
    {
        /* QOR: Quad Output Read(4-byte address) */
        g_spimd_reg.cmd = (uint32_t)SFLASHCMD_QUAD_READ_4B;
    }
    else
    {
        /* QOR: Quad Output Read(4-byte address) */
        g_spimd_reg.cmd = (uint32_t)SFLASHCMD_QUAD_READ;
    }

    /* dummy cycle setting */
    g_spimd_reg.dme   = (uint32_t)SPIBSC_DUMMY_CYC_ENABLE;  /* Dummy cycle insertion enable */
    g_spimd_reg.dmdb  = (uint32_t)SPIBSC_1BIT;
    g_spimd_reg.dmcyc = (uint32_t)SPIBSC_DUMMY_8CYC;        /* Latency Code of configuration register is b'00 */

                                                            /* Mode bit Cycle:0, Dummy Cycle:8 */
    g_spimd_reg.addr = addr;

    ret = sflash_transfer(&g_spimd_reg); /* Command,Address */
    if(0 == ret)
    {
    	/* ---- Data ---- */
    	g_spimd_reg.spidb = (uint32_t)SPIBSC_4BIT;            /* Quad */
    	g_spimd_reg.cde   = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Command Disable */
    	g_spimd_reg.ocde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Optional-Command Disable */
    	g_spimd_reg.ade   = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Disable Adr */
    	g_spimd_reg.opde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Option-Data Disable */
    	g_spimd_reg.spire = (uint32_t)SPIBSC_SPIDATA_ENABLE;  /* Data Access (Read Enable) */
    	g_spimd_reg.spiwe = (uint32_t)SPIBSC_SPIDATA_DISABLE; /* Data Access (Write Disable) */

    	/* DATA input part is dummy cycle disable */
    	g_spimd_reg.dme     = (uint32_t)SPIBSC_DUMMY_CYC_DISABLE;     /* Dummy cycle insertion disable */

        if ( 4 == unit )
        {
            g_spimd_reg.spide = (uint32_t)SPIBSC_OUTPUT_SPID_32;  /* Enable(32bit) */
        }
        else if ( 2 == unit )
        {
            g_spimd_reg.spide = (uint32_t)SPIBSC_OUTPUT_SPID_16;  /* Enable(16bit) */
        }
        else
        {
            g_spimd_reg.spide = (uint32_t)SPIBSC_OUTPUT_SPID_8;   /* Enable(8bit) */
        }

    	g_spimd_reg.sslkp = (uint32_t)SPIBSC_SPISSL_NEGATE;
    	ret = sflash_transfer(&g_spimd_reg);         /* Data input */
    	if(0 == ret)
    	{
            if( 1 == unit )
            {
                buf_b = (uint8_t *)buf;
        	*buf_b = (uint8_t)((g_spimd_reg.smrdr) & 0x000000fful);
            }
            else if( 2 == unit )
            {
                buf_s = (uint16_t *)buf;
        	*buf_s = (uint16_t)((g_spimd_reg.smrdr) & 0x0000fffful);
            }
            else if( 4 == unit )
            {
                buf_l = (uint32_t *)buf;
        	*buf_l = (uint32_t)((g_spimd_reg.smrdr) & 0xfffffffful);
            }
            else
            {
        	/* Do Nothing */
            }
    	}
    }

    return ret;

} /* End of function read_data_quad() */

/******************************************************************************
* Function Name: read_data_single
* Description  : The range of the serial flash memory designated by an argument
*                is read and it's stocked in a buffer.
*                An operation mode is performed by a Single mode.
* Arguments    : uint32_t addr  : read address(address of serial flash memory)
*                uint32_t *buf   : Start address in a buffer
*                int32_t  unit  : data size
*                int32_t  addr_mode
* Return Value :  0 : success
*                -1 : error
******************************************************************************/
static int32_t read_data_single(uint32_t addr, uint8_t * buf, int32_t unit, int32_t addr_mode)
{
    int32_t  ret = 0;
    uint8_t  *buf_b;
    uint16_t *buf_s;
    uint32_t *buf_l;

    /* ---- Command,Address,Dummy ---- */
    g_spimd_reg.cdb   = (uint32_t)SPIBSC_1BIT;            /* Commmand bit-width = Single */
    g_spimd_reg.adb   = (uint32_t)SPIBSC_1BIT;            /* Address bit-width = Single  */
    g_spimd_reg.cde   = (uint32_t)SPIBSC_OUTPUT_ENABLE;   /* Command Enable              */
    g_spimd_reg.ocde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Optional-Command Disable    */
    g_spimd_reg.ade   = (uint32_t)addr_mode;

    g_spimd_reg.opde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Option-Data Disable */

    g_spimd_reg.spide = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Disable                     */
    g_spimd_reg.sslkp = (uint32_t)SPIBSC_SPISSL_KEEP;     /* Keep after transfer         */
    g_spimd_reg.spire = (uint32_t)SPIBSC_SPIDATA_DISABLE; /* Data Access (Read Disable)  */
    g_spimd_reg.spiwe = (uint32_t)SPIBSC_SPIDATA_DISABLE; /* Data Access (Write Disable) */

    if(SPIBSC_OUTPUT_ADDR_32 == addr_mode)
    {
        /* Fast READ(4-byte address) */
        g_spimd_reg.cmd = (uint32_t)SFLASHCMD_BYTE_READ_4B;
    }
    else
    {
        /* Fast READ(4-byte address) */
        g_spimd_reg.cmd = (uint32_t)SFLASHCMD_BYTE_READ;
    }

    /* dummy cycle setting */
    g_spimd_reg.dme   = (uint32_t)SPIBSC_DUMMY_CYC_ENABLE;   /* Dummy cycle insertion enable */
    g_spimd_reg.dmdb  = (uint32_t)SPIBSC_1BIT;
    g_spimd_reg.dmcyc = (uint32_t)SPIBSC_DUMMY_8CYC;         /* Latency Code of configuration register is b'00 */

                                                             /* Mode bit Cycle:0, Dummy Cycle:8 */
    g_spimd_reg.addr = addr;

    ret = sflash_transfer(&g_spimd_reg); /* Command,Address */
    if(0 == ret)
    {
    	/* ---- Data ---- */
    	g_spimd_reg.spidb = (uint32_t)SPIBSC_1BIT;            /* Single                     */
    	g_spimd_reg.cde   = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Command Disable            */
    	g_spimd_reg.ocde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Optional-Command Disable   */
    	g_spimd_reg.ade   = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Disable Adr                */
    	g_spimd_reg.opde  = (uint32_t)SPIBSC_OUTPUT_DISABLE;  /* Option-Data Disable        */
    	g_spimd_reg.spire = (uint32_t)SPIBSC_SPIDATA_ENABLE;  /* Data Access (Read Enable)  */
    	g_spimd_reg.spiwe = (uint32_t)SPIBSC_SPIDATA_ENABLE; /* Data Access (Write Enable) */

    	/* DATA input part is dummy cycle disable */
    	g_spimd_reg.dme     = (uint32_t)SPIBSC_DUMMY_CYC_DISABLE;    /* Dummy cycle insertion disable */

        if (4 == unit)
        {
            g_spimd_reg.spide = (uint32_t)SPIBSC_OUTPUT_SPID_32;  /* Enable(32bit) */
        }
        else if (2 == unit)
        {
            g_spimd_reg.spide = (uint32_t)SPIBSC_OUTPUT_SPID_16;  /* Enable(16bit) */
        }
        else
        {
            g_spimd_reg.spide = (uint32_t)SPIBSC_OUTPUT_SPID_8;   /* Enable(8bit)  */
        }

    	g_spimd_reg.sslkp = (uint32_t)SPIBSC_SPISSL_NEGATE;
    	ret = sflash_transfer(&g_spimd_reg);         /* Data input */
    	if(0 == ret)
    	{
            if( 1 == unit )
            {
        	buf_b = (uint8_t *)buf;
                *buf_b = (uint8_t)((g_spimd_reg.smrdr) & 0x000000fful);
            }
            else if( 2 == unit )
            {
                buf_s = (uint16_t *)buf;
        	*buf_s = (uint16_t)((g_spimd_reg.smrdr) & 0x0000fffful);
            }
            else if( 4 == unit )
            {
                buf_l = (uint32_t *)buf;
        	*buf_l = (uint32_t)((g_spimd_reg.smrdr) & 0xfffffffful);
            }
            else
            {
                /* Do Nothing */
            }
    	}
    }

    return ret;

} /* End of function read_data_single() */

#endif

/* End of File */

