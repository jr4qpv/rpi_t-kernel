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
* File Name    : bus_init_nor_boot.c
* Version      : 1.2
* Device       : R7S910017
* Abstract     : BSC initialize settings for NOR flash boot
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : Initialize the BSC settings of RZ/T1
* Limitation   : This source file assumes that RZ/T1 boot mode is
*                16-bit bus boot mode (NOR flash). 
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.11.2015 1.2      First Release
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"
#include "iodefine.h"
#include "r_system.h"
#include "r_port.h"
#include "r_mpc.h"
#include "r_bsc.h"
#include "r_reset.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define BSC_RTCOR_COUNT (0x24)

#define SDRAM_MODE_CS2 (*(volatile uint16_t *)(0xA0003040))
#define SDRAM_MODE_CS3 (*(volatile uint16_t *)(0xA0004040))

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
void bus_init(void) __attribute__ ((section (".loader_text2")));

/*******************************************************************************
Private variables and functions
*******************************************************************************/
void set_bus_port_nor_boot(void) __attribute__ ((section (".loader_text2")));
void bsc_init(void) __attribute__ ((section (".loader_text2")));

/*******************************************************************************
* Function Name : bus_init
* Description   : Initialize bus (BSC) settings. 
* Arguments    : none
* Return Value : none
*******************************************************************************/
void bus_init(void)
{
    /* Initialize BSC function */
    bsc_init();

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
    
    /* Release from the CKIO pin module-stop state  */
    MSTP(CKIO) = 0;
    
    /* Disables writing to the registers related to Reset and Low-Power function */
    R_RST_WriteDisable();
    
  
    /* Set bus port settings when operation mode is nor flash boot */
    /*  A21 to A25, WE0#/DQMLL, WE1#/DQMLU, CS1#, CS2#, CS3#, RAS#, CAS#, RD/WR#, 
        CKE and CKIO pin */
    /* Note : A1 to A20, D0 to D15, RD# and CS0# already be initialized in boot sequence */    
    set_bus_port_nor_boot();


    
    /* PORT1 high drive setting for connecting SDRAM */
    PORT1.DSCR.WORD = PORT_P10_HIGH_DRIVE;
    
    /**************************************/
    /*      Set CS0 area : NOR Flash 1    */
    /**************************************/
    
    /* This area already be initialized by 16-bit bus boot mode sequence
       based on loader parameter settings (loader_param.c) */
  
    
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
    
    BSC.RTCOR = (BSC_PROTECT_KEY | BSC_RTCOR_COUNT);  // Set reflesh timer constant 0x24
    
    
    /* Write SDRAM mode register (CS2 area) to Burst read (length 1) and Burst write */
    SDRAM_MODE_CS2 = 0x0000;  // The write data is arbitrary
       
    /* Write SDRAM mode register (CS3 area) to Burst read (length 1) and Burst write */
    SDRAM_MODE_CS3 = 0x0000;  // The write data is arbitrary
       
}

/*******************************************************************************
 End of function bsc_init
*******************************************************************************/

/*******************************************************************************
* Function Name : set_bus_port_nor_boot
* Description   : Initialize bus port settings when operation mode is nor flash boot. 
*                 A21 to A25, WE0#/DQMLL, WE1#/DQMLU, CS1#, CS2#, CS3#, RAS#, CAS#, RD/WR#, 
*                 CKE and CKIO pin are enabled.
*                 (A1 to A20, D0 to D15, RD# and CS0# already be initialized in boot sequence)
* Arguments    : none
* Return Value : none
*******************************************************************************/
void set_bus_port_nor_boot(void)
{
    /* Set multi-function pin controller setting */  
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
    PORT3.PDR.BIT.B6 = PORT_DIRECTION_HIZ;  // Initialize P36(WE0#/DQMLL)
    PORT3.PMR.BIT.B6 = PORT_MODE_GENERAL;
    PORT3.PDR.BIT.B7 = PORT_DIRECTION_HIZ;  // Initialize P37(WE1#/DQMLU)
    PORT3.PMR.BIT.B7 = PORT_MODE_GENERAL;
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
    
    MPC.PT6PFS.BIT.PSEL = MPC_PSEL_PT6_A21 ;        // Set PT6 to A21
    MPC.PT7PFS.BIT.PSEL = MPC_PSEL_PT7_A22 ;        // Set PT7 to A22
    MPC.PK2PFS.BIT.PSEL = MPC_PSEL_PK2_A23 ;        // Set PK2 to A23
    MPC.PK3PFS.BIT.PSEL = MPC_PSEL_PK3_A24 ;        // Set PK3 to A24
    MPC.P97PFS.BIT.PSEL = MPC_PSEL_P97_A25 ;        // Set P97 to A25
    MPC.P36PFS.BIT.PSEL = MPC_PSEL_P36_WE0_DQMLL ;  // Set P36 to WE0#/DQMLL
    MPC.P37PFS.BIT.PSEL = MPC_PSEL_P37_WE1_DQMLU ;  // Set P37 to WE1#/DQMLU
    MPC.PD1PFS.BIT.PSEL = MPC_PSEL_PD1_CS1 ;        // Set PD1 to CS1
    MPC.P45PFS.BIT.PSEL = MPC_PSEL_P45_CS2 ;        // Set P45 to CS2
    MPC.PT4PFS.BIT.PSEL = MPC_PSEL_PT4_CS3 ;        // Set PT4 to CS3
    MPC.P90PFS.BIT.PSEL = MPC_PSEL_P90_RAS ;        // Set P90 to RAS#
    MPC.PK0PFS.BIT.PSEL = MPC_PSEL_PK0_CAS ;        // Set PK0 to CAS#
    MPC.P24PFS.BIT.PSEL = MPC_PSEL_P24_RD_WR ;      // Set P24 to RD/WR#
    MPC.P46PFS.BIT.PSEL = MPC_PSEL_P46_CKE ;        // Set P46 to CKE 
    MPC.P10PFS.BIT.PSEL = MPC_PSEL_P10_CKIO;        // Set P10 to CKIO
    
    R_MPC_WriteDisable();  // Disables writing to the PFS register
       
    /* Set each port to peripheral function */
    PORTT.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORTT.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;
    PORTK.PMR.BIT.B2 = PORT_MODE_PERIPHERAL;
    PORTK.PMR.BIT.B3 = PORT_MODE_PERIPHERAL;
    PORT9.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;
    PORT3.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORT3.PMR.BIT.B7 = PORT_MODE_PERIPHERAL;
    PORTD.PMR.BIT.B1 = PORT_MODE_PERIPHERAL;
    PORT4.PMR.BIT.B5 = PORT_MODE_PERIPHERAL;
    PORTT.PMR.BIT.B4 = PORT_MODE_PERIPHERAL;
    PORT9.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORTK.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;
    PORT2.PMR.BIT.B4 = PORT_MODE_PERIPHERAL;
    PORT4.PMR.BIT.B6 = PORT_MODE_PERIPHERAL;
    PORT1.PMR.BIT.B0 = PORT_MODE_PERIPHERAL;

}

/*******************************************************************************
 End of function set_bus_port_nor_boot
*******************************************************************************/

/* End of File */


