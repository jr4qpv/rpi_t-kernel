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
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : RZ/T1 Init program
* File Name    : r_main_convert.c
* Version      : 1.1
* Device       : R7S910017
* Abstract     : Convert SPI Mode <-> Out Addr Mode
* Tool-Chain   : ARM DS-5 Development Studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : Description interrupt service routine of RZ/T1
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 22.12.2015 1.0      First Release
*              : 14.06.2016 1.1      Second Release
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"
#include "r_serial_flash_api.h"
#include "r_main_convert.h"

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define SPI_REGION_1				(1)
#define SPI_REGION_4				(4)
#define SPI_ADDR_AREA 				(0x10000000)
#define SPI_ADDR_MIRROR_AREA 		(0x30000000)
#define SPI_ADDR_AREA_SIZE 			(0x33)			/* 64MByte */
#define SPI_ADDR_XNBITON_MODE1 		(0x130C)
#define SPI_ADDR_XNBITON_MODE2 		(0x130F)
#define SPI_ADDR_XNBITOFF_MODE1 	(0x030C)
#define SPI_ADDR_XNBITOFF_MODE2 	(0x030F)
#define SPIBSC_DRCR_RCF_EXE_LOCAL 	(1)

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
static void level1_data_cache_cleaning(void);
static void invalid_icache_and_flush_btac(void);

/*******************************************************************************
* Function Name: R_SPIBSC_Outaddr2SPI
* Description  : Convert OutAddrMode into SPIMode of 
*                Preparation process / Posteriori process
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_SPIBSC_Outaddr2SPI(void)
{
	/* region change 1 and 3 */
	mpu_init(SPI_REGION_1, SPI_ADDR_AREA, SPI_ADDR_XNBITON_MODE1, SPI_ADDR_AREA_SIZE);
	mpu_init(SPI_REGION_4, SPI_ADDR_MIRROR_AREA, SPI_ADDR_XNBITON_MODE2, SPI_ADDR_AREA_SIZE);
	
	/* Cleaning of the L1 data cache */
	level1_data_cache_cleaning();
	/* Invalidate instruction cache, also flushes BTAC */
	invalid_icache_and_flush_btac();
	
	return;
}

/*******************************************************************************
* Function Name: R_SPIBSC_SPI2Outaddr
* Description  : Convert SPIMode into OutAddrMode of
*                Preparation process / Posteriori process
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_SPIBSC_SPI2Outaddr(void)
{
	volatile uint32_t dummy32;
	
	/* region change 1 and 3 */
	mpu_init(SPI_REGION_1, SPI_ADDR_AREA, SPI_ADDR_XNBITOFF_MODE1, SPI_ADDR_AREA_SIZE);
	mpu_init(SPI_REGION_4, SPI_ADDR_MIRROR_AREA, SPI_ADDR_XNBITOFF_MODE2, SPI_ADDR_AREA_SIZE);
	
	/* Cleaning of the L1 data cache */
	level1_data_cache_cleaning();
	/* Invalidate instruction cache, also flushes BTAC */
	invalid_icache_and_flush_btac();
	
	SPIBSC.DRCR.BIT.RCF = SPIBSC_DRCR_RCF_EXE_LOCAL;
	dummy32 = SPIBSC.DRCR.LONG;
	
	return;
}

/*******************************************************************************
* Function Name: level1_data_cache_cleaning
* Description  : Cleaning of the L1 data cache
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void level1_data_cache_cleaning(void)
{
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r7;
	
	/* r7 = p15(CLIDR) */
#if defined(__ICCARM__)
	r7 = __MRC(15U, 1U, 0U, 0U, 1U);
#elif defined(__CC_ARM)
	{
		register uint32_t reg_csselr __asm("cp15:1:c0:c0:1");
		r7 = reg_csselr;
	}
#elif defined(__GNUC__)
	__asm__ __volatile__ (
		"mrc p15, 1, %0, c0, c0, 1	\n\t"
		:"=r"(r7)
	);
#else
#error Unsupported compiler.
#endif
	
	/* [r4] CLIDR(LoC) * 2 */
	/* LoC: level of coherency */
	r4 = (r7 >> 23U) & 0xEU;
	/* LoC == 0, no need to clean */
	if((int32_t)r4 != 0){
		/* [r3]: current_cache_level * 2 */
		for(r3 = 0U; r3 < r4; r3 += 2U){
			/* r2 = current_cache_level * 3 */
			r2 = r3 + (r3 >> 1);
			/* r0 = CLIDR(Ctype#) */
			r0 = (r7 >> r2) & 7U;
			
			/* ( r0 < 2 ) goto Continue1 */
			/* If no cache or only instruction cache at this level */
			if(r0 >= 2U){
				/* p15(CSSELR) = r3 */
				/* CSSELR: Cache Size Selection Register */
#if defined(__ICCARM__)
				__MCR(15U, 2U, r3, 0U, 0U, 0U);
				__ISB();
#elif defined(__CC_ARM)
				{
					register uint32_t reg_csselr __asm("cp15:2:c0:c0:0");
					reg_csselr = r3;
				}
                // __asm("isb");
                __isb(15);
#elif defined(__GNUC__)
				__asm__ __volatile__ (
					"mcr p15, 2, %0, c0, c0, 0	\n\t"
					"isb						\n\t"	/* Guarantee MCR operation */
					:"+r"(r3)
				);
#else
#error Unsupported compiler.
#endif
				
				/* r0 = p15(CCSIDR) */
				/* CCSIDR: Cache Size ID Registers */
#if defined(__ICCARM__)
				r0 = __MRC(15U, 1U, 0U, 0U, 0U);
#elif defined(__CC_ARM)
				{
					register uint32_t reg_csselr __asm("cp15:1:c0:c0:0");
					r0 = reg_csselr;
				}
#elif defined(__GNUC__)
				__asm__ __volatile__ (
					"mrc p15, 1, %0, c0, c0, 0	\n\t"
					:"=r"(r0)
				);
#else
#error Unsupported compiler.
#endif
				
				/* [r7] CCSIDR(Associativity): associativity_value - 1 */
				r7 = 0x3FFU & (r0 >> 3U);
				
				/* CCSIDR(NumSets) : max number of the index size - 1 */
				for(r2 = 0x7FFFU & (r0 >> 13U); (int32_t)r2 >= 0; r2 -= 1U){
					for(r1 = r7; (int32_t)r1 >= 0; r1 -= 1U){
						r0 = (r3 | (r1 << 30)) | (r2 << 5);
#if defined(__ICCARM__)
						__MCR(15U, 0U, r0, 7U, 14U, 2U);
#elif defined(__CC_ARM)
						{
							register uint32_t reg_csselr __asm("cp15:0:c7:c14:2");
							reg_csselr = r0;
						}
#elif defined(__GNUC__)
						__asm__ __volatile__ (
							"mcr p15, 0, %0, c7, c14, 2	\n\t"
							:"+r"(r0)
						);
#else
#error Unsupported compiler.
#endif
					}
				}
			}
		}
	}
	
	return;
}

/*******************************************************************************
* Function Name: invalid_icache_and_flush_btac
* Description  : Invalidate instruction cache, also flushes BTAC
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void invalid_icache_and_flush_btac(void)
{
	uint32_t r0;
	
#if defined(__ICCARM__)
	__DSB();
#elif defined(__CC_ARM)
	// __asm("dsb");
	__dsb(15);
#elif defined(__GNUC__)
	__asm__ __volatile__("dsb");
#else
#error Unsupported compiler.
#endif
	
	r0 = 0U;
	
#if defined(__ICCARM__)
	__MCR(15U, 0U, r0, 7U, 5U, 0U);
	__DSB();
	__ISB();
#elif defined(__CC_ARM)
	{
		register uint32_t reg_csselr __asm("cp15:0:c7:c5:0");
		reg_csselr = r0;
	}
	// __asm("dsb");
	// __asm("isb");
	__dsb(15);
	__isb(15);
#elif defined(__GNUC__)
	__asm__ __volatile__ (
		"mcr p15, 0, %0, c7, c5, 0	\n\t"
		"dsb						\n\t"
		"isb						\n\t"
		:"+r"(r0)
	);
#else
#error Unsupported compiler.
#endif
	
	return;
}

/*----------------------------------------------------------------------
#|History of "r_main_convert.c"
#|============================
#|* 2017/02/08	__CC_ARM時に、isb,dsb命令は組み込み関数を使うように修正。
#|
*/
