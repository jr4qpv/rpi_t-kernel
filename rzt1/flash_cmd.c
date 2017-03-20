/*
 *----------------------------------------------------------------------
 *
 *    T-Kernel Software Package
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)flash_cmd.c (appl) 2017/02/09
 *
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <misc/libmisc.h>
#include <string.h>

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "platform.h"
#include "iodefine.h"
//#include "r_ecm.h"
#include "r_reset.h"
#include "r_serial_flash_api.h"
//#include "r_system.h"
//#include "serial_flash_spibsc.h"
#include "r_spibsc.h"   ///////////////////////////
#include "r_main_convert.h"
//#include "sio_char.h"

#define	printf	cprintf


/*******( 定数の定義 )*******/
#define	SPIBSC_INIT_USE		0		/* 初期ローダ側で実行するようにした */


/*******( 外部参照 )*******/
extern uint32_t kaddr;				/* D,Mｺﾏﾝﾄﾞ等のｱﾄﾞﾚｽ記憶用 */
extern int ksize;					/* D,Mｺﾏﾝﾄﾞ等の表示幅記憶用 */


/*******( 共通変数定義 )*******/







#if 0	//////////////////////
/*******************************************************************************
Macro definitions
*******************************************************************************/
#define STRING_SIZE (1024)

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private variables and functions
*******************************************************************************/
static void ecm_init(void);
static void icu_init(void);
static void serial_flash_demo(void);
static uint32_t input_top_address(void);
static void serial_flash_erase(void);
static void serial_flash_write(void);
static void serial_flash_read(void);
static void display_menu (void);
static void spibsc_init(void);
#endif	/////////////////////////////////

///static char gbuff[STRING_SIZE];

#if SPIBSC_INIT_USE
static st_spibsc_cfg_t spibsc_cfg;
#endif

///static char ghex[STRING_SIZE];


#if 0	////////////////////////////////
/*******************************************************************************
* Function Name: ecm_init
* Description  : Initialize ECM setting as following.
*                   - Clear all error source
*                   - Clear ERROROUT# pin output to in-active (High) level.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void ecm_init(void)
{
    /* Initialize ECM function  */
    R_ECM_Init();
    
}
/*******************************************************************************
 End of function ecm_init
*******************************************************************************/

/*******************************************************************************
* Function Name: icu_init
* Description  : Initialize Interrupt Controller Unit setting.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void icu_init(void)
{
    /* Initialize VIC (dummy writing to HVA0 register) */
    HVA0_DUMMY_WRITE();

    /* Enable IRQ interrupt (Clear CPSR.I bit to 0) */
#if   defined(__ICCARM__)
    asm("cpsie i");   // Clear CPSR.I bit to 0
    asm("isb");       // Ensuring Context-changing
#elif defined(__CC_ARM)
    __enable_irq();   // Clear CPSR.I bit to 0
    __asm("isb");     // Ensuring Context-changing
#elif defined(__GNUC__)
    asm("cpsie i");   // Clear CPSR.I bit to 0
    asm("isb");       // Ensuring Context-changing
#endif
    
}
/*******************************************************************************
 End of function icu_init
*******************************************************************************/

/*******************************************************************************
* Function Name: serial_flash_demo
* Description  : Serial Flash sample program
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void serial_flash_demo(void)
{
    int32_t ret;
    uint32_t buf_l;
 
    while(1)
    {
        /* Display Out Addr value */
        printf("\nOutAddrMode  %08X  %08X\n", R_SERIAL_FLASH_CNT_ADDR, *(volatile uint32_t *)R_SERIAL_FLASH_CNT_ADDR);

        /* Set to SPI mode */
        R_SPIBSC_Outaddr2SPI();
        ret = R_SPIBSC_Spimode();

        if(0 != ret)
        {
            printf("\nFailed to convert into SPI mode\n\n");
            break;
        }
           
        /* Read counter variable in SPI mode*/
        ret = R_SPIBSC_ByteRead((uint32_t)R_SERIAL_FLASH_CNT_ADDR, (uint8_t *)&buf_l, (int32_t)R_SERIAL_FLASH_READ_BYTE, 
                     (uint32_t)R_SERIAL_FLASH_DATA_WIDTH, (uint32_t)R_SERIAL_FLASH_ADDR_MODE);
        
        /* Set to OutAddrMode */
        R_SPIBSC_Exmode();
        R_SPIBSC_SPI2Outaddr();

        if(0 != ret)
        {
            printf("\nSerial Flash Read Error\n\n");
            break;
        }
        
        /* Display SPIMode Addr value */
        printf("SPIMode      %08X  %08X\n\n", R_SERIAL_FLASH_CNT_ADDR, buf_l);
        
    	/* Addr Increment */
    	buf_l ++;
            
        /* Set to SPI mode */
        R_SPIBSC_Outaddr2SPI();
        ret = R_SPIBSC_Spimode();
            
        if(0 != ret)
        {
            printf("Failed to convert into SPI mode\n\n");
            break;
        }
            
        /* Erase before writing */
        ret = R_SPIBSC_EraseSector((uint32_t)R_SERIAL_FLASH_CNT_ADDR, 
                 (uint32_t)R_SERIAL_FLASH_DATA_WIDTH, (uint32_t)R_SERIAL_FLASH_ADDR_MODE);
            
        if(0 != ret)
        {
            /* Set to OutAddrMode */
            R_SPIBSC_Exmode();
            R_SPIBSC_SPI2Outaddr();
          
            printf("Failed to write Serial Flash\n\n");
            break;
        }

        /* Write to serial flash */
        ret = R_SPIBSC_ByteProgram((uint32_t)R_SERIAL_FLASH_CNT_ADDR, (uint8_t *)&buf_l, (int32_t)R_SERIAL_FLASH_WRITE_BYTE,
                    (uint32_t)R_SERIAL_FLASH_DATA_WIDTH, (uint32_t)R_SERIAL_FLASH_ADDR_MODE);
        
        /* Set to OutAddrMode */
        R_SPIBSC_Exmode();
        R_SPIBSC_SPI2Outaddr();
                    
        if(0 != ret)
        {
            printf("Failed to write Serial Flash\n\n");
            break;
        }
        
        break;
    }
}
/*******************************************************************************
 End of function serial_flash_demo
*******************************************************************************/

/*******************************************************************************
* Function Name: input_top_address
* Description  :  
* Arguments    : none
* Return Value : address
*******************************************************************************/
static uint32_t input_top_address(void)
{
    char     *pend;
    uint32_t address;
	
	/* Get the address */
    while(1)
    {  
        memset(gbuff, '\0', (size_t)STRING_SIZE);
        memset(ghex, '\0', (size_t)STRING_SIZE);
        printf("\nInput the top address (10000000 - 13FFFFFC)\n\n>");    
        gets (gbuff);
        ghex[0] = '0';
        ghex[1] = 'x';
        strcat (ghex, gbuff);	/* Never Overflow buffer */
        address = (uint32_t)strtoul(ghex, &pend ,0);

        /* Check the parameter */
        if ((address < (uint32_t)R_SERIAL_FLASH_TOP) 
         || (address > (uint32_t)R_SERIAL_FLASH_END))
        {
            printf("\nInvalid address\n");
        }
        else
        {
            break;
        }
    }
	
	return address;
}
/*******************************************************************************
 End of function input_top_address
*******************************************************************************/

/*******************************************************************************
* Function Name: serial_flash_erase
* Description  :  
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void serial_flash_erase(void)
{
    uint32_t address;
    int32_t ret;
    
	/* Get the address */
	address = input_top_address();
	
    while(1)
    {
        /* Set to SPI mode */
        R_SPIBSC_Outaddr2SPI();
        ret = R_SPIBSC_Spimode();

        if (0 != ret)
        {          
            printf("\nFailed to erase Serial Flash\n\n");
            break;
        }
            
        ret = R_SPIBSC_EraseSector(address, (uint32_t)R_SERIAL_FLASH_DATA_WIDTH, 
                                   (uint32_t)R_SERIAL_FLASH_ADDR_MODE);
        
        /* Set to OutAddrMode */
        R_SPIBSC_Exmode();
        R_SPIBSC_SPI2Outaddr();
            
        if (0 != ret)
        {
            printf("\nFailed to erase Serial Flash\n\n");
            break;
        }
        
        printf("\n");
        break;
    }
}

/*******************************************************************************
 End of function serial_flash_erase
*******************************************************************************/
#endif	//////////////////////////////////////

/*******************************************************************************
* Function Name: serial_flash_write
* Description  :  
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void serial_flash_write(void)
{
	printf("sirial_flash_write(%08X, %04X)\n", 0, 0);    ////////////////

#if 0	///////////////////////////
  uint32_t address;
    char     *pend;
    uint32_t buf_l;
    int32_t ret;
        
    /* Get the address */
	address = input_top_address();
	
    printf("\nInput the data (00000000 - FFFFFFFF)\n");
    printf("Input just [Enter] without data to exit\n\n");
    while (1)
    {
        memset(gbuff, '\0', (size_t)STRING_SIZE);
        memset(ghex, '\0', (size_t)STRING_SIZE);
        
    	printf("%08X", address);

        /* Set to SPI mode */
        R_SPIBSC_Outaddr2SPI();
        ret = R_SPIBSC_Spimode();
        
        if (0 != ret)
        {
            printf("\n\nSerial Flash Error\n\n");
            break;
        }
        ret = R_SPIBSC_ByteRead(address, (uint8_t *)&buf_l, (int32_t)R_SERIAL_FLASH_READ_BYTE, 
                   (uint32_t)R_SERIAL_FLASH_DATA_WIDTH, (uint32_t)R_SERIAL_FLASH_ADDR_MODE);
        
        /* Set to OutAddrMode */
        R_SPIBSC_Exmode();
        R_SPIBSC_SPI2Outaddr();
        
        if (0 != ret)
        {
             printf("\n\nSerial Flash Error\n\n");
             break;
        }     
        printf(" %08X > ", buf_l);

        /* Get the address to change */
        gets (gbuff);
        if(('\0' == gbuff[0]) || ('\0' != gbuff[8]))
        {
        	/* No data or over 8 strings data */
           printf("\ninvalid data\n");
           printf("    \n\n");
           break;
        }
    	
        ghex[0] = '0';
        ghex[1] = 'x';
        strcat (ghex, gbuff);
        buf_l = (uint32_t)strtoul(ghex, &pend ,0);
        
    	if((*pend) != '\0'){
        	/* Invalid data */
           printf("\ninvalid data\n");
           printf("    \n\n");
           break;
    	}
    	
        /* Set to SPI mode */
        R_SPIBSC_Outaddr2SPI();
        ret = R_SPIBSC_Spimode();
     
        if (0 != ret)
        {
           printf("\nSerial Flash Error\n\n");
           break;
        }
        
        ret = R_SPIBSC_ByteProgram(address, (uint8_t *)&buf_l, (int32_t)R_SERIAL_FLASH_WRITE_BYTE, 
             (uint32_t)R_SERIAL_FLASH_DATA_WIDTH, (uint32_t)R_SERIAL_FLASH_ADDR_MODE);
        
        /* Set to OutAddrMode */
        R_SPIBSC_Exmode();
        R_SPIBSC_SPI2Outaddr();
        if (0 != ret)
        {
           printf("\nFailed to write Serial Flash\n\n");
           break;
        }
        address += (uint32_t)R_SERIAL_FLASH_WRITE_BYTE;
    	
    	if(address > R_SERIAL_FLASH_END)
    	{
    	   printf("\n\n");
    	   break;
    	}
    }

#endif	//////////////////////////////     
}
/*******************************************************************************
 End of function serial_flash_write
*******************************************************************************/


/*******************************************************************************
* Function Name: serial_flash_read
* Description  :  
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void serial_flash_read(uint32_t address, long count)
{
	uint16_t size;
	int16_t  i;
	uint32_t buf_l;
	int32_t ret;
	//   char  *pend;
	uint32_t limit;

	size = count;
	///  address = 0x10000000;          ///////////////
	printf("sirial_flash_read(%08X, %04X)\n", address, size);    ////////////////

  
	///  /* Get the address */
	///  address = input_top_address();
	
	/* Get the size and display data */
	while(1)
	{
		///      memset(gbuff, '\0', (size_t)STRING_SIZE);
		limit = 0U;
		if(0x13FFFF00U <= address)
		{
			limit = (R_SERIAL_FLASH_ADDR_LIMIT - address) >> 2U;
		}
		else
		{
			limit = R_SERIAL_FLASH_READ_SIZE;
		}
      
		///	  printf("\nInput the number of blocks (1 - %d : 4byte blocks)\n\n>", limit);
		///   	  gets (gbuff);
		///      size = (uint16_t)strtoul(gbuff, &pend, 0);

		/* Check the parameter */
		if ((size > (uint16_t)0) && (size <= limit))
		{
			break;
		}
		else
		{
			printf("\ninvalid size\n");
			return;
		}
	}
   
	/* Print the read data */
	while(1)
	{
		i = 0;
		printf("\n               00       04       08       0C     \n");
		while (1)
		{
			/* Set to SPI mode */
			R_SPIBSC_Outaddr2SPI();
			ret = R_SPIBSC_Spimode();
           
			if (0 != ret)
			{
				printf("\nSerial Flash Read Error\n\n");
				break;
			}
			ret = R_SPIBSC_ByteRead(address, (uint8_t *)&buf_l, (int32_t)R_SERIAL_FLASH_READ_BYTE, 
				(uint32_t)R_SERIAL_FLASH_DATA_WIDTH, (uint32_t)R_SERIAL_FLASH_ADDR_MODE);
           
			/* Set to OutAddrMode */
			R_SPIBSC_Exmode();
			R_SPIBSC_SPI2Outaddr();
			if (0 != ret)
			{
				printf("\nSerial Flash Read Error\n\n");
				break;
			}
			if ((i % 4) == 0)
			{
				printf("%08X", address);
			}
			printf(" %08X", buf_l);
			address += (uint32_t)4;
			if(((i++) % 4) == 3)
			{
				printf("    \n");
			}
			if ((uint16_t)i == size)
			{
				printf("    \n\n");
				kaddr = address;			/* 次回の為の記憶 */
				break;
			}
		}
		break; 
	}
  
	return; 
}
/*******************************************************************************
 End of function serial_flash_read
*******************************************************************************/

#if 0	//////////////////////////////////////////
/*******************************************************************************
* Function Name: display_menu
* Description  : 
*              : 
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void display_menu (void)
{
   printf("[1] Demo\n");
   printf("[2] Read Serial Flash\n");  
   printf("[3] Write Serial Flash\n"); 
   printf("[4] Erase Serial Flash\n\n"); 
   printf("[9] Exit\n\n"); 
   printf(">");
}
/*******************************************************************************
 End of function display_menu
*******************************************************************************/
#endif	//////////////////////////


/*******************************************************************************
* Function Name : spibsc_init
* Description   : Initialize SPI multi I/O bus controller (SPIBSC) settings. 
* Arguments    : none
* Return Value : none
*******************************************************************************/
#if SPIBSC_INIT_USE
static void spibsc_init(void)
{
	///  printf("spibsc_init()\n"); ///////////
    /* Enables writing to the registers related to Reset and Low-Power function */
    R_RST_WriteEnable();
    
    /* Release from the SPIBSC module-stop state  */
    SYSTEM.MSTPCRC.BIT.MSTPCRC9 = 0U;
    
    /* Disables writing to the registers related to Reset and Low-Power function */
    R_RST_WriteDisable();
        
    /* SPIBSC stop */
    SPIBSC.DRCR.BIT.SSLN = 1U;
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

    SPIBSC.SSLDR.LONG = 0U;    /* The next access delayed setting : 1SPBCLK   */

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

    SPIBSC.SPBCR.BIT.SPBR = 2U;

    /*--------------------------------------*/
    /*    BDRV =  0                         */
    /*--------------------------------------*/
    SPIBSC.SPBCR.BIT.BRDV = 0U;

    /* data read control register(DRCR) */
    SPIBSC.DRCR.BIT.RBURST = SPIBSC_BURST_2;         /* read burst 2 */
    SPIBSC.DRCR.BIT.RBE    = SPIBSC_BURST_ENABLE;    /* read cache enable */

    /* Wait TEND=1 for setting change in SPIBSC. */
    R_SPIBSC_WaitTend();

    /* ==== Sets the SPIBSC ==== */
    R_SPIBSC_SetConfig(&spibsc_cfg);
    if ( 0 != R_SPIBSC_ExmodeSetting(&spibsc_cfg) )
    {
        while (1)
        {
            ; /* Please describe the error sequence */
        }
    }      
}
#endif
/*******************************************************************************
 End of function spibsc_init
*******************************************************************************/


/*******************************************************************************
* Outline      : main processing
* Function Name: main
* Description  : Serial Flash sample program of RZ/T1.
*                Display the Serial flash operational menu to
*                RS-232C I/F.
* Arguments    : none
* Return Value : 0
*******************************************************************************/
void cmd_flash(int argc, char *argv[])
//uint32_t main (void)
{
	long count;
  
	///  /* Initialize the ECM function */
	///  ecm_init();
    
	///  /* Initialize the ICU settings */
	///  icu_init();

	///  /* Initialize SCIFA 2 */
	///  IoInitScifa2();
    
#if SPIBSC_INIT_USE
	/* Initialize SPIBSC function (NOR boot) */
	if( 0U != SYSTEM.MSTPCRC.BIT.MSTPCRC9) {
		spibsc_init();
	}
#endif
  
	if (argc >= 2) {
		count = 0x40;
	
		if (argc >= 3)					/* ｱﾄﾞﾚｽの獲得 */
			kaddr = atolhex(argv[2]);

		if (kaddr < R_SERIAL_FLASH_TOP) {
			printf("\ninvalid addr\n");
			return ;
		}
	
		if (argc >= 4)					/* ｶｳﾝﾄの獲得 */
			count = atolhex(argv[3]);


		/* 各コマンドの呼び出し */
		if (strcmp(argv[1], "read") == 0) {
			serial_flash_read(kaddr, count);
		}
		else if (strcmp(argv[1], "write") == 0) {
			serial_flash_write();
		}
		else {
			printf("Input Parameter Error\n\n");
		}
	}
	else {
		printf("*** help ***\n");
	}

  
#if 0	///////////////////////////////
    memset(gbuff, '\0', (size_t)STRING_SIZE);
    printf("Serial Flash example program\n\n");
    display_menu(); 
    while (1)
    {
        gets (gbuff);   
        if(strcmp(gbuff, "1") == 0)   
        {   
           serial_flash_demo();
        }   
        else if(strcmp(gbuff, "2") == 0)   
        {   
           serial_flash_read(); 
        }   
        else if(strcmp(gbuff, "3") == 0)   
        {   
           serial_flash_write();
        }
        else if(strcmp(gbuff, "4") == 0)   
        {   
           serial_flash_erase();
        }
        else if(strcmp(gbuff, "9") == 0)   
        {
           break;   
        }
        else   
        {     
            printf("Input Parameter Error\n\n");    
        }
        display_menu();
    }
#endif	//////////////////////////////////////
	
	return;
}
/*******************************************************************************
 End of function main
*******************************************************************************/

/* End of File */


/*----------------------------------------------------------------------
#|History of "flash_cmd.c"
#|==========================
#|* 2017/02/07	New created.(By T.Yokobayashi)
#|
*/
