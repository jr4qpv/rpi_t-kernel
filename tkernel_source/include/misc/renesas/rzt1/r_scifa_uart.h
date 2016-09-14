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
* System Name  : RZ/T1 SCIF program
* File Name    : r_scifa_uart.h
* Version      : 1.1
* Device       : R7S910017
* Abstract     : API for SCIFA_UART function
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : SCIFA_UART function API of RZ/T1
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 22.12.2015 1.0      First Release
*              : 18.03.2016 1.1      Second Release
*******************************************************************************/
#ifndef _R_SCIFA_UART_HEADER_
#define _R_SCIFA_UART_HEADER_


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== Arguments, return values ==== */
/* ---- ch ---- */
#define SCIFA_UART_SUCCESS          (0)
#define SCIFA_UART_ERR              (-1)
#define SCIFA_UART_ERR_RECEIVE      (-2)
#define SCIFA_UART_CH_TOTAL         (5)     /* The number of UART channels */
#define SCIFA_UART_CH_0             (0)
#define SCIFA_UART_CH_1             (1)
#define SCIFA_UART_CH_2             (2)
#define SCIFA_UART_CH_3             (3)
#define SCIFA_UART_CH_4             (4)

/* ---- Mode ---- */
#define SCIFA_UART_MODE_R           (1)     /* Receive          */
#define SCIFA_UART_MODE_W           (2)     /* Transfer         */
#define SCIFA_UART_MODE_RW          (3)     /* Transfer/receive */

/* ==== CKS division setting (Select SCIFA clock clock source) ==== */
#define SCIFA_UART_CKS_DIVISION_1   (0)     /*  1 division of SERICLK clock */
#define SCIFA_UART_CKS_DIVISION_4   (1)     /*  4 division of SERICLK clock */
#define SCIFA_UART_CKS_DIVISION_16  (2)     /* 16 division of SERICLK clock */
#define SCIFA_UART_CKS_DIVISION_64  (3)     /* 64 division of SERICLK clock */

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
/* ==== API function ==== */
int32_t R_SCIFA_UART_Init (uint32_t channel, uint32_t mode, uint16_t cks, uint8_t brr);
int32_t R_SCIFA_UART_Open (uint32_t channel, uint32_t mode);
int32_t R_SCIFA_UART_Receive (uint32_t channel, uint8_t * data);
int32_t R_SCIFA_UART_Send (uint32_t channel, uint8_t data);

/* ==== User-defined function ==== */
void userdef_scifa2_uart_init (uint32_t mode, uint16_t cks, uint8_t brr);
void userdef_scifa2_uart_open (uint32_t mode);
int32_t userdef_scifa2_uart_receive (uint8_t * data);
void userdef_scifa2_uart_send (uint8_t data);


#endif  /* _R_SCIFA_UART_HEADER_ */

/* End of File */
