/******************************************************************************
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
/********************************************************************************
* System Name  : RZ/T1 Init program
* File Name    : loader_init.asm
* Version      : 1.2
* Device       : R7S910017
* Abstract     : Loader program 1
* Tool-Chain   : e2studio
* OS           : not use
* H/W Platform : RZ/T1 Evaluation Board (RTK7910022C00000BR)
* Description  : Description interrupt service routine of RZ/T1
* Limitation   : none
*******************************************************************************/
/********************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.11.2015 1.2      First Release
******************************************************************************/

    .section .loader_text,"ax"

	.global _PowerON_Reset
    .global loader_init1
    .global set_low_vec
    .global cache_init
    .global mpu_init
    .global copy_to_atcm
    .extern loader_init2
 
/***********************************************************************
* Function Name : loader_init1
* Description   : Initialize sysytem by loader program
* Arguments     : none
* Return Value  : none
************************************************************************/
_PowerON_Reset:
loader_init1:
 
stack_init:
    /* Stack setting  */
    cps  #17  /* FIQ mode */
    ldr  sp, =_fiq_stack
    cps  #18  /* IRQ mode */
    ldr  sp, =_irq_stack
    cps  #23  /* Abort mode */
    ldr  sp, =_abt_stack
    cps  #27  /* Undef mode */
    ldr  sp, =_und_stack
    cps  #31  /* System mode */
    ldr  sp, =_sys_stack
    cps  #19  /* SVC mode */
    ldr  sp, =_svc_stack
 
vfp_init:
    /* Initialize VFP setting */
    mrc  p15, #0, r0, c1, c0, #2  /* Enables cp10 and cp11 accessing */
    orr  r0, r0, #0xF00000
    mcr  p15, #0, r0, c1, c0, #2
    isb                           /* Ensuring Context-changing */

    mov  r0, #0x40000000  /* Enables VFP operation */
    vmsr  fpexc, r0

#if defined (RAM_BOOT)
    mrc  p15, 0, r0, c1, c0, 0  /* Set SCTLR.VE bit to 1 (Use VIC) */
    orr  r0, r0, #0x01000000
    mcr  p15, 0, r0, c1, c0, 0
    isb                         /* Ensuring Context-changing */

    mrc  p15, 0, r0, c1, c0, 1  /* Enalbe ECC */
    orr  r0, r0, #0x06000000
    mcr  p15, 0, r0, c1, c0, 1
    isb                         /* Ensuring Context-changing */

    /* Clear ATCM. */
#if 0	/* //// kernelのデバック用にクリアしない //// */
    ldr  r0, =0x00000000
    ldr  r1, =0x00000000
    ldr  r2, =0x0007ffff
clear_atcm:
    strb  r0, [r1], #0
    add   r1, r1, #1
    cmp   r2, r1
    bne   clear_atcm
#endif
    dsb                      /*Ensuring data-changing */

    mrs r0, cpsr                /* Enalbe FIQ */
    and r0, r0, #0xFFFFFFBF
    msr cpsr, r0
    isb
#endif

#if 1
bss_init:
    mov  r2, #0
    ldr  r0, =_bss
    ldr  r1, =_ebss
    cmp  r1, r0
    beq  data_init

clear_to_BSS:
    strb  r2, [r0], #0
    add   r0, r0, #1
    cmp   r1, r0
    bne   clear_to_BSS
    dsb                      /*Ensuring data-changing */
#endif

data_init:   
    /* Initialize variables has initialized value of loader_init2. */
    /*  Variables has no initialized value already be initialized to zero */
    /* in boot sequence(Clear ATCM and BTCM). */
    ldr  r0, =_mdata
    ldr  r1, =_data_start
    ldr  r2, =_data_end
    cmp  r2, r1
    beq  jump_loader_init2

copy_to_LDR_DATA:
    ldrb  r3, [r0], #0
    strb  r3, [r1], #0
    add   r0, r0, #1
    add   r1, r1, #1
    cmp   r2, r1
    bne   copy_to_LDR_DATA    
    dsb                      /*Ensuring data-changing */


    /* Jump to loader_init2 */
jump_loader_init2:
    ldr  r0, =loader_init2
    bx  r0

/***********************************************************************
* Function Name : copy_to_atcm
* Description   : Cpoy .text section to ATCM
* Arguments     : none
* Return Value  : none
************************************************************************/
copy_to_atcm:
    /* Initialize variables has initialized value of loader_init2. */
    /*  Variables has no initialized value already be initialized to zero */
    /* in boot sequence(Clear ATCM and BTCM). */
    ldr  r0, =_mtext
    ldr  r1, =_text_start
    ldr  r2, =_text_end
    cmp  r2, r1
    beq  copy_end
   
copy_to_LDR_TEXT:
    ldrb  r3, [r0], #0
    strb  r3, [r1], #0
    add   r0, r0, #1
    add   r1, r1, #1
    cmp   r2, r1
    bne   copy_to_LDR_TEXT
copy_end:
    dsb                      /*Ensuring data-changing */

	bx  lr

/***********************************************************************
* Function Name : cache_init
* Description   : Initialize I1, D1 cache and MPU settings
* Arguments     : none
* Return Value  : none
************************************************************************/

/*******************************************************************************
* Macro definitions
********************************************************************************/
#define SCTLR_BR  0x00020000
#define SCTLR_M   0x00000001
#define SCTLR_I_C 0x00001004

/* I-RAM(512KB) */
#define DRBAR_REGION_0  0x04000000  /* Base address = 0400_0000h */
#define DRACR_REGION_0  0x0000030C  /* R/W(full), Normal, Non-cache, share */
#define DRSR_REGION_0   0x00000025  /* Size 512KB, MPU enable */

/* SPI Multi I/O(64MB) */
#define DRBAR_REGION_1  0x10000000  /* Base address = 1000_0000h */
#define DRACR_REGION_1  0x0000030C  /* R/W(full), Normal, Non-cache, share */
#define DRSR_REGION_1   0x00000033  /* Size 64MB, MPU enable */

/* D-RAM, D-RAM&I-RAM Mirror(128MB) */
#define DRBAR_REGION_2  0x20000000  /* Base address = 2000_0000h */
#define DRACR_REGION_2  0x0000030C  /* R/W(full), Normal, Non-cache, share */
#define DRSR_REGION_2   0x00000035  /* Size 128MB, MPU enable */

/* SPI Multi I/O(64MB) Mirror */
#define DRBAR_REGION_3  0x30000000  /* Base address = 3000_0000h */
#define DRACR_REGION_3  0x0000030C  /* R/W(full), Normal, Non-cache, share */
//#define DRACR_REGION_3  0x00000306  /* R/W(full), Normal, cache, B=0, share */
#define DRSR_REGION_3   0x00000033  /* Size 64MB, MPU enable */

/* CS0-CS1:NOR(128MB) Mirror */
#define DRBAR_REGION_4  0x40000000  /* Base address = 4000_0000h */
#define DRACR_REGION_4  0x0000030C  /* R/W(full), Normal, Non-cache, share */
#define DRSR_REGION_4   0x00000035  /* Size 128MB, MPU enable */

/* CS2-CS3:SDRAM(128MB) Mirror */
#define DRBAR_REGION_5  0x48000000  /* Base address = 4800_0000h */
#define DRACR_REGION_5  0x0000030C  /* R/W(full), Normal, Non-cache, share */
#define DRSR_REGION_5   0x00000035  /* Size 128MB, MPU enable */

/* CS4-CS5:No use(128MB) Mirror */
#define DRBAR_REGION_6  0x50000000  /* Base address = 5000_0000h */
#define DRACR_REGION_6  0x00001305  /* R/W(full), XN, Device, share */
#define DRSR_REGION_6   0x00000035  /* Size 128MB, MPU enable */

/* CS0-CS1:NOR(128MB) */
#define DRBAR_REGION_7  0x60000000  /* Base address = 6000_0000h */
#define DRACR_REGION_7  0x0000030C  /* R/W(full), Normal, Non-cache, share */
#define DRSR_REGION_7   0x00000035  /* Size 128MB, MPU enable */

/* CS2-CS3:SDRAM(128MB) */
#define DRBAR_REGION_8  0x68000000  /* Base address = 6800_0000h */
#define DRACR_REGION_8  0x0000030C  /* R/W(full), Normal, Non-cache, share */
#define DRSR_REGION_8   0x00000035  /* Size 128MB, MPU enable */

/* CS4-CS5:No use(128MB) */
#define DRBAR_REGION_9  0x70000000 /* Base address = 7000_0000h */
#define DRACR_REGION_9  0x00001305 /* R/W(full), XN, Device, share */
#define DRSR_REGION_9   0x00000035 /* Size 128MB, MPU enable */

/* I/O Registers(2GB) */
#define DRBAR_REGION_10 0x80000000 /* Base address = 8000_0000h */
#define DRACR_REGION_10 0x00001305 /* R/W(full), XN, Device, share */
#define DRSR_REGION_10  0x0000003D /* Size 2GB, MPU enable */

cache_init:
    push  {lr}

cache_invalidate:
    /* Invalidate the I1, D1 cache */
    mov  r0, #0
    mcr  p15, #0, r0, c7, c5, #0   /* Invalidate all Instruction Caches (Write-value is Ignored) */
    isb                            /* Ensuring Context-changing */
    mcr  p15, #0, r0, c15, c5, #0  /* Invalidate all Data Caches (Write-value is Ignored) */
    isb                            /* Ensuring Context-changing */
  
    /* Adopt default memory map as background map. */
    ldr  r0, =SCTLR_BR           /* Set SCTLR.BR bit to 1 */
    mrc  p15, 0, r1, c1, c0, 0  
    orr  r1, r1, r0
    dsb
    mcr  p15, 0, r1, c1, c0, 0  
    isb                         /* Ensuring Context-changing */
    
    /* Initialize MPU settings (region 0 to 10) */
    /* Define region 0 */
    mov  r0,  #0
    ldr  r1, =DRBAR_REGION_0
    ldr  r2, =DRACR_REGION_0
    ldr  r3, =DRSR_REGION_0
    bl  mpu_init

    /* Define region 1 */
    mov  r0,  #1
    ldr  r1, =DRBAR_REGION_1
    ldr  r2, =DRACR_REGION_1
    ldr  r3, =DRSR_REGION_1
    bl  mpu_init

    /* Define region 2 */
    mov  r0,  #2
    ldr  r1, =DRBAR_REGION_2
    ldr  r2, =DRACR_REGION_2
    ldr  r3, =DRSR_REGION_2
    bl  mpu_init

    /* Define region 3 */
    mov  r0,  #3
    ldr  r1, =DRBAR_REGION_3
    ldr  r2, =DRACR_REGION_3
    ldr  r3, =DRSR_REGION_3
    bl  mpu_init

    /* Define region 4 */
    mov  r0,  #4
    ldr  r1, =DRBAR_REGION_4
    ldr  r2, =DRACR_REGION_4
    ldr  r3, =DRSR_REGION_4
    bl  mpu_init

    /* Define region 5 */
    mov  r0,  #5
    ldr  r1, =DRBAR_REGION_5
    ldr  r2, =DRACR_REGION_5
    ldr  r3, =DRSR_REGION_5
    bl  mpu_init

    /* Define region 6 */
    mov  r0,  #6
    ldr  r1, =DRBAR_REGION_6
    ldr  r2, =DRACR_REGION_6
    ldr  r3, =DRSR_REGION_6
    bl  mpu_init

    /* Define region 7 */
    mov  r0,  #7
    ldr  r1, =DRBAR_REGION_7
    ldr  r2, =DRACR_REGION_7
    ldr  r3, =DRSR_REGION_7
    bl  mpu_init

    /* Define region 8 */
    mov  r0,  #8
    ldr  r1, =DRBAR_REGION_8
    ldr  r2, =DRACR_REGION_8
    ldr  r3, =DRSR_REGION_8
    bl  mpu_init

    /* Define region 9 */
    mov  r0,  #9
    ldr  r1, =DRBAR_REGION_9
    ldr  r2, =DRACR_REGION_9
    ldr  r3, =DRSR_REGION_9
    bl  mpu_init

    /* Define region 10 */
    mov  r0,  #10
    ldr  r1, =DRBAR_REGION_10
    ldr  r2, =DRACR_REGION_10
    ldr  r3, =DRSR_REGION_10
    bl  mpu_init

    
    /* Enables MPU operation */
    ldr  r0, =SCTLR_M           /* Set SCTLR.M bit to 1 */
    mrc  p15, 0, r1, c1, c0, 0  
    orr  r1, r1, r0
    dsb
    mcr  p15, 0, r1, c1, c0, 0  
    isb                         /* Ensuring Context-changing */
    
    /* Enables I1,D1 cache operation */
    ldr  r0, =SCTLR_I_C         /* Set SCTLR.I and C bit to 1 */
    mrc  p15, 0, r1, c1, c0, 0  
    orr  r1, r1, r0
    dsb
    mcr  p15, 0, r1, c1, c0, 0  
    isb                         /* Ensuring Context-changing */

    pop  {pc}
    bx  lr

/***********************************************************************
* Function Name : mpu_init
* Description   : Initialize MPU settings
* Arguments     : none
* Return Value  : none
***********************************************************************/
mpu_init:
    /* RGNR(MPU Memory Region Number Register) */
    mcr p15, #0, r0, c6, c2, #0
    isb                             /* Ensuring Context-changing */
    
    /* DRBAR(Data Region Base Address Register) */
    mcr  p15, #0, r1, c6, c1, #0
    isb                             /* Ensuring Context-changing */

    /* DRACR(Data Region Access Control Register) */
    mcr p15, #0, r2, c6, c1, #4
    isb                             /* Ensuring Context-changing */

    /* DRSR(Data Region Size and Enable Register) */
    mcr p15, #0, r3, c6, c1, #2
    isb                             /* Ensuring Context-changing */
    
    bx      lr


/***********************************************************************
* Function Name : set_low_vec
* Description   : Initialize sysytem by loader program
* Arguments     : none
* Return Value  : none
***********************************************************************/
set_low_vec:
    mrc  p15, 0, r0, c1, c0, 0  /* Set SCTLR.V bit to 0 (low-vector) */
    and  r0, r0, #0xFFFFDFFF
    mcr  p15, 0, r0, c1, c0, 0
    isb                         /* Ensuring Context-changing */
    bx  lr  



#if defined (RAM_BOOT)
/***********************************************************************
* Function Name : loader_copy
* Description   : Cpoy .loader_text(2) section to BTCM and jump loader
* Arguments     : none
* Return Value  : none
************************************************************************/
    .section .loader_param,"ax"

    .global _start_loader_copy
_start_loader_copy:
    ldr  r0, =_mloader_text
    ldr  r1, =_loader_text_start
    ldr  r2, =_loader_text_end
    cmp  r2, r1
    beq  loader_copy_end

loader_copy_loop:
    ldrb  r3, [r0], #0
    strb  r3, [r1], #0
    add   r0, r0, #1
    add   r1, r1, #1
    cmp   r2, r1
    bne   loader_copy_loop
loader_copy_end:
    dsb                      /* Ensuring data-changing */
    isb                      /* Ensuring Context-changing */

    ldr  r0, =_loader_text_start
    bx   r0

   .pool
#endif

    .end


/*----------------------------------------------------------------------
#|History of "loader_init.S"
#|==========================
#|* 2016/03/29	Renesasの｢初期設定｣ｻﾝﾌﾟﾙｺｰﾄﾞから修正。(by T.Yokobayashi)
#|* 2016/04/02	RAMﾌﾞｰﾄ用に｢_start_loader_copy｣処理の追加
#|* 2016/04/02	SFLASHﾌﾞｰﾄ用のｿｰｽと合わせてBSSの初期化処理など入れた
#|
*/
