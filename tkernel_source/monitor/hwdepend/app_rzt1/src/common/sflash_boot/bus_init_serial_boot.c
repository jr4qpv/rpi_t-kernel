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
* File Name    : bus_init_serial_boot.c
* Version      : 1.2
* Device       : R7S910017
* Abstract     : Bus initialize settings for Serial flash boot
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : Initialize the Bus settings of RZ/T1
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
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"
#include "iodefine.h"
#include "r_system.h"
#include "r_port.h"
#include "r_mpc.h"
#include "r_bsc.h"
#include "r_spibsc.h"
#include "r_spibsc_ioset_api.h"
#include "r_reset.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define BSC_RTCOR_COUNT    (0x24)

#define SDRAM_MODE_CS2     (*(volatile uint16_t *)(0xA0003040))
#define SDRAM_MODE_CS3     (*(volatile uint16_t *)(0xA0004040))


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void bus_init(void) __attribute__ ((section (".loader_text2")));

/******************************************************************************
Private global variables and functions
******************************************************************************/
st_spibsc_cfg_t g_spibsc_cfg;

void bsc_init(void) __attribute__ ((section (".loader_text2")));
void spibsc_init(void) __attribute__ ((section (".loader_text2")));
void set_bus_port_serial_boot(void) __attribute__ ((section (".loader_text2")));

/*******************************************************************************
* Function Name : bus_init
* Description   : Initialize bus (BSC, SPIBSC) settings. 
* Arguments    : none
* Return Value : none
*******************************************************************************/
void bus_init(void)
{
    /* Initialize BSC function */
    bsc_init();
    
    /* Initialize SPIBSC function */
    spibsc_init();
    
}  

/*******************************************************************************
 End of function bus_init
*******************************************************************************/

/*******************************************************************************
* Function Name : bsc_init
* Description   : Initialize bus controller unit (BSC) settings. 
* Arguments    : none
* Return Value : none
*******************************************************************************/
void bsc_init(void)
{   
    /* Enables writing to the registers related to Reset and Low-Power function */
    R_RST_WriteEnable();
    
    /* Release from the CKIO pin and BSC module-stop state  */
    MSTP(CKIO) = 0;
    MSTP(BSC) = 0;
    
    /* Disables writing to the registers related to Reset and Low-Power function */
    R_RST_WriteDisable(); 
     
    /* Set bus port settings when operation mode is serial flash boot */
    /*  A0 to A25, D0 to D15, RD#, WE0#/DQMLL, WE1#/DQMLU, CS0#, CS1#, CS2#,
        CS3#, RAS#, CAS#, RD/WR#, CKE and CKIO pin */
    set_bus_port_serial_boot();
    
    /* PORT1 high drive setting for connecting SDRAM */
    PORT1.DSCR.WORD = PORT_P10_HIGH_DRIVE;
    
    /**************************************/
    /*      Set CS0 area : NOR Flash 1    */
    /**************************************/
    BSC.CS0BCR.BIT.IWW = BSC_IDLE_CYCLE_1;   // Set idle cycle 1
    BSC.CS0BCR.BIT.IWRWD = BSC_IDLE_CYCLE_0; // Set idle cycle 0
    BSC.CS0BCR.BIT.IWRWS = BSC_IDLE_CYCLE_0; // Set idle cycle 0
    BSC.CS0BCR.BIT.IWRRD = BSC_IDLE_CYCLE_0; // Set idle cycle 0 
    BSC.CS0BCR.BIT.IWRRS = BSC_IDLE_CYCLE_0; // Set idle cycle 0  
    BSC.CS0BCR.BIT.TYPE = BSC_TYPE_NORMAL;  // Set memory type to normal memory  
    BSC.CS0BCR.BIT.BSZ = BSC_WIDTH_16_BIT;  // Set data bus width to 16-bit
    
    BSC.CS0WCR.CS0WCR_0.BIT.SW = BSC_DELAY_STATE_CYCLE_2_5; // Set delay cycle to 2.5 state
    BSC.CS0WCR.CS0WCR_0.BIT.WR = BSC_ACCESS_WAIT_6 ;        // Set access wait to 6
    BSC.CS0WCR.CS0WCR_0.BIT.WM = BSC_EXT_WAIT_IGNORED;      // Mask external wait input
    BSC.CS0WCR.CS0WCR_0.BIT.HW = BSC_DELAY_STATE_CYCLE_0_5; // Set delay cycle to 0.5 state
    
    /**************************************/
    /*      Set CS1 area : NOR Flash 2    */
    /**************************************/
    BSC.CS1BCR.BIT.IWW = BSC_IDLE_CYCLE_1;   // Set idle cycle 1
    BSC.CS1BCR.BIT.IWRWD = BSC_IDLE_CYCLE_0; // Set idle cycle 0
    BSC.CS1BCR.BIT.IWRWS = BSC_IDLE_CYCLE_0; // Set idle cycle 0
    BSC.CS1BCR.BIT.IWRRD = BSC_IDLE_CYCLE_0; // Set idle cycle 0 
    BSC.CS1BCR.BIT.IWRRS = BSC_IDLE_CYCLE_0; // Set idle cycle 0   
    BSC.CS1BCR.BIT.TYPE = BSC_TYPE_NORMAL;   // Set memory type to normal memory  
    BSC.CS1BCR.BIT.BSZ = BSC_WIDTH_16_BIT;   // Set data bus width to 16-bit
    
    BSC.CS1WCR.BIT.SW = BSC_DELAY_STATE_CYCLE_2_5; // Set delay cycle to 2.5 state
    BSC.CS1WCR.BIT.WR = BSC_ACCESS_WAIT_6 ;        // Set access wait to 6
    BSC.CS1WCR.BIT.WM = BSC_EXT_WAIT_IGNORED;      // Mask external wait input
    BSC.CS1WCR.BIT.HW = BSC_DELAY_STATE_CYCLE_0_5; // Set delay cycle to 0.5 state
    
    
    /**************************************/
    /*      Set CS2 area : SDRAM 1        */
    /**************************************/
    BSC.CS2BCR.BIT.TYPE = BSC_TYPE_SDRAM;    // Set memory type to SDRAM
    BSC.CS2BCR.BIT.BSZ = BSC_WIDTH_16_BIT;   // Set data bus width to 16-bit 
    
    BSC.CS2BCR.BIT.IWW = BSC_IDLE_CYCLE_0;   // Set idle cycle 0
    BSC.CS2BCR.BIT.IWRWD = BSC_IDLE_CYCLE_0; // Set idle cycle 0
    BSC.CS2BCR.BIT.IWRWS = BSC_IDLE_CYCLE_0; // Set idle cycle 0 
    BSC.CS2BCR.BIT.IWRRD = BSC_IDLE_CYCLE_0; // Set idle cycle 0     
    BSC.CS2BCR.BIT.IWRRS = BSC_IDLE_CYCLE_0; // Set idle cycle 0    
    
    BSC.CS2WCR.CS2WCR_1.BIT.A2CL = BSC_CAS_LATENCY_2; // Set CAS latency to 2
     
    /**************************************/
    /*      Set CS3 area : SDRAM 2        */
    /**************************************/
    BSC.CS3BCR.BIT.TYPE = BSC_TYPE_SDRAM;  // Set memory type to SDRAM 
    BSC.CS3BCR.BIT.BSZ = BSC_WIDTH_16_BIT;  // Set data bus width to 16-bit 
    
    BSC.CS3BCR.BIT.IWW = BSC_IDLE_CYCLE_0;            // Set idle cycle 0
    BSC.CS3BCR.BIT.IWRWD = BSC_IDLE_CYCLE_0;          // Set idle cycle 0
    BSC.CS3BCR.BIT.IWRWS = BSC_IDLE_CYCLE_0;          // Set idle cycle 0 
    BSC.CS3BCR.BIT.IWRRD = BSC_IDLE_CYCLE_0;          // Set idle cycle 0     
    BSC.CS3BCR.BIT.IWRRS = BSC_IDLE_CYCLE_0;          // Set idle cycle 0  
    
    BSC.CS3WCR.CS3WCR_1.BIT.WTRP = BSC_WTRP_WAIT_1;   // Set pre-charge wait to 1       
    BSC.CS3WCR.CS3WCR_1.BIT.WTRCD = BSC_WTRCD_WAIT_1; // Set wait to 1                  
    BSC.CS3WCR.CS3WCR_1.BIT.A3CL = BSC_CAS_LATENCY_2; // Set CAS latency to 2
    BSC.CS3WCR.CS3WCR_1.BIT.TRWL = BSC_TRWL_CYCLE_2;  // Set pre-charge wait to 2
    BSC.CS3WCR.CS3WCR_1.BIT.WTRC = BSC_WTRC_IDLE_5;   // Set idle state to 5           
                        
    /* Reflesh setting for SDRAM */
    BSC.SDCR.BIT.A2ROW = BSC_ROW_13_BIT;     // Row address is 13 bit
    BSC.SDCR.BIT.A2COL = BSC_COL_10_BIT;     // Column address is 10 bit
    BSC.SDCR.BIT.DEEP = BSC_DEEP_SELF;       // Deep power down mode
    BSC.SDCR.BIT.RFSH = BSC_RFSH_DONE;       // Reflesh is done
    BSC.SDCR.BIT.RMODE = BSC_RMODE_AUTO;     // Auto reflesh 
    BSC.SDCR.BIT.PDOWN = BSC_PDOWN_INVALID;  // Power down is invalid
    BSC.SDCR.BIT.BACTV = BSC_BACTV_AUTO;     // Auto pre-charge mode
    BSC.SDCR.BIT.A3ROW = BSC_ROW_13_BIT;     // Row address is 13 bit
    BSC.SDCR.BIT.A3COL = BSC_COL_10_BIT;     // Column address is 10 bit
   
    BSC.RTCSR.LONG = ( BSC_PROTECT_KEY 
                      | BSC_CMIE_DISABLE    // Disable CMF interrupt
                      | BSC_CKS_DIV_16      // Reflesh timer clock is CKIO/16
                      | BSC_RFSH_TIME_1 );  // Continuous reflesh time is 1
    
    BSC.RTCOR = (BSC_PROTECT_KEY | BSC_RTCOR_COUNT);  // Set reflesh timer constant 0x30
    
    
    /* Write SDRAM mode register (CS2 area) to Burst read (length 1) and Burst write */
    SDRAM_MODE_CS2 = 0x0000;  // The write data is arbitrary
       
    /* Write SDRAM mode register (CS3 area) to Burst read (length 1) and Burst write */
    SDRAM_MODE_CS3 = 0x0000;  // The write data is arbitrary
    
}   

/*******************************************************************************
 End of function bsc_init
*******************************************************************************/

/*******************************************************************************
* Function Name : spibsc_init
* Description   : Initialize SPI multi I/O bus controller (SPIBSC) settings. 
* Arguments    : none
* Return Value : none
*******************************************************************************/
void spibsc_init(void)
{
    if( 0U != SYSTEM.MSTPCRC.BIT.MSTPCRC9)		// ﾓｼﾞｭｰﾙｽﾄｯﾌﾟ解除されてる?
    {
		/* Enables writing to the registers related to Reset and Low-Power function */
		R_RST_WriteEnable();

		/* Release from the SPIBSC module-stop state  */
		SYSTEM.MSTPCRC.BIT.MSTPCRC9 = 0U;

		/* Disables writing to the registers related to Reset and Low-Power function */
		R_RST_WriteDisable();
	}

/* If using SPIBSC as default settings, please define "__SPIBSC_DEFAULT__" */
#ifndef __SPIBSC_DEFAULT__
    /* SPIBSC stop */
    SPIBSC.DRCR.BIT.SSLN = 1;
    while (1)
    {    
        if (SPIBSC_SSL_NEGATE == SPIBSC.CMNSR.BIT.SSLF)
        {
            break;
        }
    }   
    
    /* Wait TEND=1 for setting change in SPIBSC. */
    while (SPIBSC_TRANS_END != SPIBSC.CMNSR.BIT.TEND)
    {
        ; /* Wait for transfer-end */
    }

    SPIBSC.SSLDR.LONG = 0;    /* The next access delayed setting : 1SPBCLK   */
                              /* SPBSSL negate delayed setting   : 1.5SPBCLK */
                              /* Clock delayed setting           : 1SPBCLK   */

    /* bitrate */
    /*--------------------------------------*/
    /*    SPBR =  1 : 75.00Mbps(1/ 2 PCLKA) */
    /*    SPBR =  2 : 37.50Mbps(1/ 4 PCLKA) */
    /*    SPBR =  3 : 25.00Mbps(1/ 6 PCLKA) */
    /*    SPBR =  4 : 18.75Mbps(1/ 8 PCLKA) */
    /*    SPBR =  5 : 15.00Mbps(1/10 PCLKA) */
    /*    SPBR =  6 : 12.50Mbps(1/12 PCLKA) */
    /*--------------------------------------*/
    SPIBSC.SPBCR.BIT.SPBR = 2;
    /*--------------------------------------*/
    /*    BDRV =  0                         */
    /*--------------------------------------*/
    SPIBSC.SPBCR.BIT.BRDV = 0;

    /* data read control register(DRCR) */
    SPIBSC.DRCR.BIT.RBURST = SPIBSC_BURST_2;         /* read burst 2 */
    SPIBSC.DRCR.BIT.RBE    = SPIBSC_BURST_ENABLE;    /* read cache enable */

    /* Wait TEND=1 for setting change in SPIBSC. */
    R_SFLASH_WaitTend();

    /* ==== Sets the SPIBSC ==== */
    R_SFLASH_Set_Config(&g_spibsc_cfg);
    if ( 0 != R_SFLASH_Exmode_Setting(&g_spibsc_cfg) )
    {
        while (1)
        {
            /* Please describe the error sequence */
        }
    }    
    
#endif 
    
}

/*******************************************************************************
 End of function spibsc_init
*******************************************************************************/

/*******************************************************************************
* Function Name : set_bus_port_serial_boot
* Description   : Initialize bus port settings when operation mode is serial flash boot.
*                 A0 to A25, D0 to D15, RD#, WE0#/DQMLL, WE1#/DQMLU, CS0#, CS1#, CS2#,
*                 CS3#, RAS#, CAS#, RD/WR#, CKE and CKIO pin are enbaled.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void set_bus_port_serial_boot(void)
{
    /* Set multi-function pin controller setting */  
    PORT2.PDR.BIT.B3 = PORT_DIRECTION_HIZ;  // Initialize P23(A0)
    PORT2.PMR.BIT.B3 = PORT_MODE_GENERAL;
    PORTG.PDR.BIT.B0 = PORT_DIRECTION_HIZ;  // Initialize PG0(A1)
    PORTG.PMR.BIT.B0 = PORT_MODE_GENERAL;   
    PORTG.PDR.BIT.B1 = PORT_DIRECTION_HIZ;  // Initialize PG1(A2)
    PORTG.PMR.BIT.B1 = PORT_MODE_GENERAL;   
    PORTG.PDR.BIT.B2 = PORT_DIRECTION_HIZ;  // Initialize PG2(A3)
    PORTG.PMR.BIT.B2 = PORT_MODE_GENERAL;
    PORTG.PDR.BIT.B3 = PORT_DIRECTION_HIZ;  // Initialize PG3(A4)
    PORTG.PMR.BIT.B3 = PORT_MODE_GENERAL;
    PORTG.PDR.BIT.B4 = PORT_DIRECTION_HIZ;  // Initialize PG4(A5)
    PORTG.PMR.BIT.B4 = PORT_MODE_GENERAL;
    PORTG.PDR.BIT.B5 = PORT_DIRECTION_HIZ;  // Initialize PG5(A6)
    PORTG.PMR.BIT.B5 = PORT_MODE_GENERAL;
    PORTG.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize PG6(A7)
    PORTG.PMR.BIT.B6 = PORT_MODE_GENERAL;
    PORTG.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize PG7(A8)
    PORTG.PMR.BIT.B7 = PORT_MODE_GENERAL;   
    PORTH.PDR.BIT.B0 = PORT_DIRECTION_HIZ;  // Initialize PH0(A9)
    PORTH.PMR.BIT.B0 = PORT_MODE_GENERAL;   
    PORTH.PDR.BIT.B1 = PORT_DIRECTION_HIZ;  // Initialize PH1(A10)
    PORTH.PMR.BIT.B1 = PORT_MODE_GENERAL;   
    PORTH.PDR.BIT.B2 = PORT_DIRECTION_HIZ;  // Initialize PH2(A11)
    PORTH.PMR.BIT.B2 = PORT_MODE_GENERAL;
    PORTH.PDR.BIT.B3 = PORT_DIRECTION_HIZ;  // Initialize PH3(A12)
    PORTH.PMR.BIT.B3 = PORT_MODE_GENERAL;
    PORTH.PDR.BIT.B4 = PORT_DIRECTION_HIZ;  // Initialize PH4(A13)
    PORTH.PMR.BIT.B4 = PORT_MODE_GENERAL;
    PORTH.PDR.BIT.B5 = PORT_DIRECTION_HIZ;  // Initialize PH5(A14)
    PORTH.PMR.BIT.B5 = PORT_MODE_GENERAL;
    PORTH.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize PH6(A15)
    PORTH.PMR.BIT.B6 = PORT_MODE_GENERAL;
    PORTH.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize PH7(A16)
    PORTH.PMR.BIT.B7 = PORT_MODE_GENERAL;   
    PORT2.PDR.BIT.B0 = PORT_DIRECTION_HIZ;  // Initialize P20(A17)
    PORT2.PMR.BIT.B0 = PORT_MODE_GENERAL;
    PORT2.PDR.BIT.B5 = PORT_DIRECTION_HIZ;  // Initialize P25(A18)
    PORT2.PMR.BIT.B5 = PORT_MODE_GENERAL;
    PORT2.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize P26(A19)
    PORT2.PMR.BIT.B6 = PORT_MODE_GENERAL;
    PORT2.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize P27(A20)
    PORT2.PMR.BIT.B7 = PORT_MODE_GENERAL;   
    PORTT.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize PT6(A21)
    PORTT.PMR.BIT.B6 = PORT_MODE_GENERAL;
    PORTT.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize PT7(A22)
    PORTT.PMR.BIT.B7 = PORT_MODE_GENERAL;
    PORTK.PDR.BIT.B2 = PORT_DIRECTION_HIZ;  // Initialize PK2(A23)
    PORTK.PMR.BIT.B2 = PORT_MODE_GENERAL;
    PORTK.PDR.BIT.B3 = PORT_DIRECTION_HIZ;  // Initialize PK3(A24)
    PORTK.PMR.BIT.B3 = PORT_MODE_GENERAL;
    PORT9.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize P97(A25)
    PORT9.PMR.BIT.B7 = PORT_MODE_GENERAL;
    
    PORT0.PDR.BIT.B0 = PORT_DIRECTION_HIZ;  // Initialize P00(D0)
    PORT0.PMR.BIT.B0 = PORT_MODE_GENERAL;   
    PORT0.PDR.BIT.B1 = PORT_DIRECTION_HIZ;  // Initialize P01(D1)
    PORT0.PMR.BIT.B1 = PORT_MODE_GENERAL;   
    PORT0.PDR.BIT.B2 = PORT_DIRECTION_HIZ;  // Initialize P02(D2)
    PORT0.PMR.BIT.B2 = PORT_MODE_GENERAL;
    PORT0.PDR.BIT.B3 = PORT_DIRECTION_HIZ;  // Initialize P03(D3)
    PORT0.PMR.BIT.B3 = PORT_MODE_GENERAL;
    PORT0.PDR.BIT.B4 = PORT_DIRECTION_HIZ;  // Initialize P04(D4)
    PORT0.PMR.BIT.B4 = PORT_MODE_GENERAL;
    PORT0.PDR.BIT.B5 = PORT_DIRECTION_HIZ;  // Initialize P05(D5)
    PORT0.PMR.BIT.B5 = PORT_MODE_GENERAL;
    PORT0.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize P06(D6)
    PORT0.PMR.BIT.B6 = PORT_MODE_GENERAL;
    PORT0.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize P07(D7)
    PORT0.PMR.BIT.B7 = PORT_MODE_GENERAL;   
    PORTE.PDR.BIT.B0 = PORT_DIRECTION_HIZ;  // Initialize PE0(D8)
    PORTE.PMR.BIT.B0 = PORT_MODE_GENERAL;   
    PORTE.PDR.BIT.B1 = PORT_DIRECTION_HIZ;  // Initialize PE1(D9)
    PORTE.PMR.BIT.B1 = PORT_MODE_GENERAL;   
    PORTE.PDR.BIT.B2 = PORT_DIRECTION_HIZ;  // Initialize PE2(D10)
    PORTE.PMR.BIT.B2 = PORT_MODE_GENERAL;
    PORTE.PDR.BIT.B3 = PORT_DIRECTION_HIZ;  // Initialize PE3(D11)
    PORTE.PMR.BIT.B3 = PORT_MODE_GENERAL;
    PORTE.PDR.BIT.B4 = PORT_DIRECTION_HIZ;  // Initialize PE4(D12)
    PORTE.PMR.BIT.B4 = PORT_MODE_GENERAL;
    PORTE.PDR.BIT.B5 = PORT_DIRECTION_HIZ;  // Initialize PE5(D13)
    PORTE.PMR.BIT.B5 = PORT_MODE_GENERAL;
    PORTE.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize PE6(D14)
    PORTE.PMR.BIT.B6 = PORT_MODE_GENERAL;
    PORTE.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize PE7(D15)
    PORTE.PMR.BIT.B7 = PORT_MODE_GENERAL;   
    
    PORT2.PDR.BIT.B2 = PORT_DIRECTION_HIZ;  // Initialize P22(RD#)
    PORT2.PMR.BIT.B2 = PORT_MODE_GENERAL;
    PORT3.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize P36(WE0#/DQMLL)
    PORT3.PMR.BIT.B6 = PORT_MODE_GENERAL;
    PORT3.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize P37(WE1#/DQMLU)
    PORT3.PMR.BIT.B7 = PORT_MODE_GENERAL;
    PORT2.PDR.BIT.B1 = PORT_DIRECTION_HIZ;  // Initialize P21(CS0#)
    PORT2.PMR.BIT.B1 = PORT_MODE_GENERAL;    
    PORTD.PDR.BIT.B1 = PORT_DIRECTION_HIZ;  // Initialize PD1(CS1#)
    PORTD.PMR.BIT.B1 = PORT_MODE_GENERAL;
    PORT4.PDR.BIT.B5 = PORT_DIRECTION_HIZ;  // Initialize P45(CS2#)
    PORT4.PMR.BIT.B5 = PORT_MODE_GENERAL;
    PORTT.PDR.BIT.B4 = PORT_DIRECTION_HIZ;  // Initialize PT4(CS3#)
    PORTT.PMR.BIT.B4 = PORT_MODE_GENERAL;
    PORT9.PDR.BIT.B0 = PORT_DIRECTION_HIZ;  // Initialize P90(RAS#)
    PORT9.PMR.BIT.B0 = PORT_MODE_GENERAL;
    PORTK.PDR.BIT.B0 = PORT_DIRECTION_HIZ;  // Initialize PK0(CAS#)
    PORTK.PMR.BIT.B0 = PORT_MODE_GENERAL;
    PORT2.PDR.BIT.B4 = PORT_DIRECTION_HIZ;  // Initialize P24(RD/WR#)
    PORT2.PMR.BIT.B4 = PORT_MODE_GENERAL;
    PORT4.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize P46(CKE)
    PORT4.PMR.BIT.B6 = PORT_MODE_GENERAL;
    PORT1.PDR.BIT.B0 = PORT_DIRECTION_HIZ;  // Initialize P10(CKIO)
    PORT1.PMR.BIT.B0 = PORT_MODE_GENERAL;
    
    R_MPC_WriteEnable(); // Enables writing to the PFS register
    
    MPC.P23PFS.BIT.PSEL = MPC_PSEL_P23_A0;         // Set P23 to A0
    MPC.PG0PFS.BIT.PSEL = MPC_PSEL_PG0_A1;         // Set PG0 to A1    
    MPC.PG1PFS.BIT.PSEL = MPC_PSEL_PG1_A2;         // Set PG1 to A2
    MPC.PG2PFS.BIT.PSEL = MPC_PSEL_PG2_A3;         // Set PG2 to A3
    MPC.PG3PFS.BIT.PSEL = MPC_PSEL_PG3_A4;         // Set PG3 to A4
    MPC.PG4PFS.BIT.PSEL = MPC_PSEL_PG4_A5;         // Set PG4 to A5
    MPC.PG5PFS.BIT.PSEL = MPC_PSEL_PG5_A6;         // Set PG5 to A6
    MPC.PG6PFS.BIT.PSEL = MPC_PSEL_PG6_A7;         // Set PG6 to A7
    MPC.PG7PFS.BIT.PSEL = MPC_PSEL_PG7_A8;         // Set PG7 to A8
    MPC.PH0PFS.BIT.PSEL = MPC_PSEL_PH0_A9;         // Set PH0 to A9    
    MPC.PH1PFS.BIT.PSEL = MPC_PSEL_PH1_A10;        // Set PH1 to A10
    MPC.PH2PFS.BIT.PSEL = MPC_PSEL_PH2_A11;        // Set PH2 to A11
    MPC.PH3PFS.BIT.PSEL = MPC_PSEL_PH3_A12;        // Set PH3 to A12
    MPC.PH4PFS.BIT.PSEL = MPC_PSEL_PH4_A13;        // Set PH4 to A13
    MPC.PH5PFS.BIT.PSEL = MPC_PSEL_PH5_A14;        // Set PH5 to A14
    MPC.PH6PFS.BIT.PSEL = MPC_PSEL_PH6_A15;        // Set PH6 to A15
    MPC.PH7PFS.BIT.PSEL = MPC_PSEL_PH7_A16;        // Set PH7 to A16    
    MPC.P20PFS.BIT.PSEL = MPC_PSEL_P20_A17;        // Set P20 to A17
    MPC.P25PFS.BIT.PSEL = MPC_PSEL_P25_A18;        // Set P25 to A18
    MPC.P26PFS.BIT.PSEL = MPC_PSEL_P26_A19;        // Set P26 to A19
    MPC.P27PFS.BIT.PSEL = MPC_PSEL_P27_A20;        // Set P27 to A20  
    MPC.PT6PFS.BIT.PSEL = MPC_PSEL_PT6_A21;        // Set PT6 to A21
    MPC.PT7PFS.BIT.PSEL = MPC_PSEL_PT7_A22;        // Set PT7 to A22
    MPC.PK2PFS.BIT.PSEL = MPC_PSEL_PK2_A23;        // Set PK2 to A23
    MPC.PK3PFS.BIT.PSEL = MPC_PSEL_PK3_A24;        // Set PK3 to A24
    MPC.P97PFS.BIT.PSEL = MPC_PSEL_P97_A25;        // Set P97 to A25
    
    MPC.P00PFS.BIT.PSEL = MPC_PSEL_P00_D0;         // Set P00 to D0    
    MPC.P01PFS.BIT.PSEL = MPC_PSEL_P01_D1;         // Set P01 to D1
    MPC.P02PFS.BIT.PSEL = MPC_PSEL_P02_D2;         // Set P02 to D2
    MPC.P03PFS.BIT.PSEL = MPC_PSEL_P03_D3;         // Set P03 to D3
    MPC.P04PFS.BIT.PSEL = MPC_PSEL_P04_D4;         // Set P04 to D4
    MPC.P05PFS.BIT.PSEL = MPC_PSEL_P05_D5;         // Set P05 to D5
    MPC.P06PFS.BIT.PSEL = MPC_PSEL_P06_D6;         // Set P06 to D6
    MPC.P07PFS.BIT.PSEL = MPC_PSEL_P07_D7;         // Set P07 to D7
    MPC.PE0PFS.BIT.PSEL = MPC_PSEL_PE0_D8;         // Set PE0 to D8    
    MPC.PE1PFS.BIT.PSEL = MPC_PSEL_PE1_D9;         // Set PE1 to D9
    MPC.PE2PFS.BIT.PSEL = MPC_PSEL_PE2_D10;        // Set PE2 to D10
    MPC.PE3PFS.BIT.PSEL = MPC_PSEL_PE3_D11;        // Set PE3 to D11
    MPC.PE4PFS.BIT.PSEL = MPC_PSEL_PE4_D12;        // Set PE4 to D12
    MPC.PE5PFS.BIT.PSEL = MPC_PSEL_PE5_D13;        // Set PE5 to D13
    MPC.PE6PFS.BIT.PSEL = MPC_PSEL_PE6_D14;        // Set PE6 to D14
    MPC.PE7PFS.BIT.PSEL = MPC_PSEL_PE7_D15;        // Set PE7 to D15
    
    MPC.P22PFS.BIT.PSEL = MPC_PSEL_P22_RD;         // Set P22 to RD#
    MPC.P36PFS.BIT.PSEL = MPC_PSEL_P36_WE0_DQMLL;  // Set P36 to WE0#/DQMLL
    MPC.P37PFS.BIT.PSEL = MPC_PSEL_P37_WE1_DQMLU;  // Set P37 to WE1#/DQMLU
    
    MPC.P21PFS.BIT.PSEL = MPC_PSEL_P21_CS0;        // Set P21 to CS0
    MPC.PD1PFS.BIT.PSEL = MPC_PSEL_PD1_CS1;        // Set PD1 to CS1
    MPC.P45PFS.BIT.PSEL = MPC_PSEL_P45_CS2;        // Set P45 to CS2
    MPC.PT4PFS.BIT.PSEL = MPC_PSEL_PT4_CS3;        // Set PT4 to CS3
    MPC.P90PFS.BIT.PSEL = MPC_PSEL_P90_RAS;        // Set P90 to RAS#
    MPC.PK0PFS.BIT.PSEL = MPC_PSEL_PK0_CAS;        // Set PK0 to CAS#
    MPC.P24PFS.BIT.PSEL = MPC_PSEL_P24_RD_WR;      // Set P24 to RD/WR#
    MPC.P46PFS.BIT.PSEL = MPC_PSEL_P46_CKE;        // Set P46 to CKE 
    MPC.P10PFS.BIT.PSEL = MPC_PSEL_P10_CKIO;       // Set P10 to CKIO
    
    R_MPC_WriteDisable();  // Disables writing to the PFS register
       
    /* Set each port to peripheral function */
    PORTT.PMR.BIT.B4 = PORT_MODE_PERIPHERAL;
    PORTT.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORTT.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;
    PORTK.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORTK.PMR.BIT.B2 = PORT_MODE_PERIPHERAL;
    PORTK.PMR.BIT.B3 = PORT_MODE_PERIPHERAL;
    PORTH.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORTH.PMR.BIT.B1 = PORT_MODE_PERIPHERAL;
    PORTH.PMR.BIT.B2 = PORT_MODE_PERIPHERAL;
    PORTH.PMR.BIT.B3 = PORT_MODE_PERIPHERAL;
    PORTH.PMR.BIT.B4 = PORT_MODE_PERIPHERAL;
    PORTH.PMR.BIT.B5 = PORT_MODE_PERIPHERAL;
    PORTH.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORTH.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;
    PORTG.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORTG.PMR.BIT.B1 = PORT_MODE_PERIPHERAL;
    PORTG.PMR.BIT.B2 = PORT_MODE_PERIPHERAL;
    PORTG.PMR.BIT.B3 = PORT_MODE_PERIPHERAL;
    PORTG.PMR.BIT.B4 = PORT_MODE_PERIPHERAL;
    PORTG.PMR.BIT.B5 = PORT_MODE_PERIPHERAL;
    PORTG.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORTG.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;
    PORTE.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORTE.PMR.BIT.B1 = PORT_MODE_PERIPHERAL;
    PORTE.PMR.BIT.B2 = PORT_MODE_PERIPHERAL;
    PORTE.PMR.BIT.B3 = PORT_MODE_PERIPHERAL;
    PORTE.PMR.BIT.B4 = PORT_MODE_PERIPHERAL;
    PORTE.PMR.BIT.B5 = PORT_MODE_PERIPHERAL;
    PORTE.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORTE.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;
    PORTD.PMR.BIT.B1 = PORT_MODE_PERIPHERAL;
    PORT9.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORT9.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;
    PORT4.PMR.BIT.B5 = PORT_MODE_PERIPHERAL;
    PORT4.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORT3.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORT3.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;
    PORT2.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORT2.PMR.BIT.B1 = PORT_MODE_PERIPHERAL;
    PORT2.PMR.BIT.B2 = PORT_MODE_PERIPHERAL;
    PORT2.PMR.BIT.B3 = PORT_MODE_PERIPHERAL;
    PORT2.PMR.BIT.B4 = PORT_MODE_PERIPHERAL;
    PORT2.PMR.BIT.B5 = PORT_MODE_PERIPHERAL;
    PORT2.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORT2.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;
    PORT1.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORT0.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORT0.PMR.BIT.B1 = PORT_MODE_PERIPHERAL;
    PORT0.PMR.BIT.B2 = PORT_MODE_PERIPHERAL;
    PORT0.PMR.BIT.B3 = PORT_MODE_PERIPHERAL;
    PORT0.PMR.BIT.B4 = PORT_MODE_PERIPHERAL;
    PORT0.PMR.BIT.B5 = PORT_MODE_PERIPHERAL;
    PORT0.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORT0.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;  
  
}

/*******************************************************************************
 End of function set_bus_port_serial_boot
*******************************************************************************/

/* End of File */


/*----------------------------------------------------------------------
#|History of "loader_init2.c"
#|===========================
#|* 2016/04/22	Renesasの｢初期設定｣ｻﾝﾌﾟﾙｺｰﾄﾞから修正。(by T.Yokobayashi)
#|* 2016/04/22	spibsc_init()で、MSTPCRC9 のﾓｼﾞｭｰﾙｽﾄｯﾌﾟ解除(=0)する処理追加
#|
*/
