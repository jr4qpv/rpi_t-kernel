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
* File Name    : loader_param.c
* Version      : 1.2
* Device       : R7S910017
* Abstract     : Loader program 2
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : Loader settings of RZ/T1
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.11.2015 1.2      First Release
*******************************************************************************/

#define DUMMY 0x0


#if defined (NOR_BOOT)
/*******************************************************************************
Parameter Information for the Loader in NOR Boot Mode
*******************************************************************************/
const unsigned int Loader_Param[]  __attribute__ ((section (".loader_param"))) = {
        0x00000000,  // CACHE_FLG     @4000_0000H
        0x10000C00,  // CS0BCR_V      @4000_0004H
        0x00001340,  // CS0WCR_V      @4000_0008H
        0x00000000,  // DUMMY0        @4000_000CH
        0x00000000,  // BSC_FLG       @4000_0010H
        0x4000004C,  // LDR_ADDR_NML  @4000_0014H
        0x00006000,  // LDR_SIZE_NML  @4000_0018H
        0x00802000,  // DEST_ADDR_NML @4000_001CH
        0x00000000,  // DUMMY1        @4000_0020H
        0x00000000,  // DUMMY2        @4000_0024H
        0x00000000,  // DUMMY3        @4000_0028H
        0x00000000,  // DUMMY4        @4000_002CH
        0x00000000,  // DUMMY5        @4000_0030H
        0x00000000,  // DUMMY6        @4000_0034H
        0x00000000,  // DUMMY7        @4000_0038H
        0x00000000,  // DUMMY8        @4000_003CH
        0x00000000,  // DUMMY9        @4000_0040H
        0x00000000,  // DUMMY10       @4000_0044H
        0x0000F00C   // CHECK_SUM     @4000_0048H
};
#elif defined (RAM_BOOT)
// Parameter is not needed for RAM boot.
#else
/*******************************************************************************
Parameter Information for the Loader in SPI Boot Mode
*******************************************************************************/
const unsigned int Loader_Param[]  __attribute__ ((section (".loader_param"))) = {
		0x00000000, //CACHE_FLG
		0x00070707, //SSLDR_V
		0x00000003, //SPBCR_V
		0x0,        //DRCR_V
		0x0,        //SPIBSC_FLG
		0x3000004C, //LDR_ADDR_NML
		0x00006000, //LDR_SIZE_NML
		0x00802000, //DEST_ADDR_NML
		DUMMY,      //DUMMY1
		DUMMY,      //DUMMY2
		DUMMY,      //DUMMY3
		DUMMY,      //DUMMY4
		DUMMY,      //DUMMY5
		DUMMY,      //DUMMY6
		DUMMY,      //DUMMY7
		DUMMY,      //DUMMY8
		DUMMY,      //DUMMY9
		DUMMY,      //DUMMY10
		0x0000B7DD  //CHECK_SUM
};
#endif
