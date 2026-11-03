; /*****************************************************************************
; * Project Name: XHSC AUTOSAR MCAL
; * File Name: startup_hc32a4a0.s
; *
; * Description: Implementation of the Start-up
; *              Compiler:                 IAR GCC
; *              Autosar Version           4.4.0
; *
; * (c) Copyright 2022-2025 XIAOHUA SEMICONDUCTOR CO., LTD. All rights reserved.
; *
; * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT
; * (the "License"). By downloading, installing, or using the Software, you
; * indicate that you accept the terms of the License, and you acknowledge that
; * you have the authority, for yourself or on behalf of your company, to bind
; * your company to these terms. If you do not agree to all of the terms of this
; * License, you should not download or install the Software.
; *
; *****************************************************************************/

    .syntax unified
    .arch armv7-m

.section ".init_table", "a"
  .long __VECTORTABLE_START
  .long __INIT_VECTORTABLE_START
  .long __INIT_VECTORTABLE_END
  .long __MCALTEXTFAST_START
  .long __INIT_MCALTEXTFAST_START
  .long __INIT_MCALTEXTFAST_END
  .long __MCALDATAFAST_START
  .long __INIT_MCALDATAFAST_START
  .long __INIT_MCALDATAFAST_END
  .long __DATA_START
  .long __INIT_DATA_START
  .long __INIT_DATA_END

.section ".zero_table", "a"
  .long __BSS_START
  .long __BSS_END
  
.section ".intvec","ax"
.align 2
.thumb
.globl __STACK_START
.type Reset_Handler, %function
.globl Reset_Handler
.globl VTABLE
.globl NMI_Handler
.globl HardFault_Handler
.globl MemManage_Handler
.globl BusFault_Handler
.globl UsageFault_Handler
.globl UnKnown_IRQhandler
.globl SVC_Handler
.globl DebugMon_Handler
.globl PendSV_Handler
.globl SysTick_Handler

VTABLE:
.long __STACK_START
.long Reset_Handler
.long NMI_Handler
.long HardFault_Handler
.long MemManage_Handler
.long BusFault_Handler
.long UsageFault_Handler
.long 0
.long 0
.long 0
.long 0
.long SVC_Handler
.long DebugMon_Handler
.long 0
.long PendSV_Handler
.long SysTick_Handler

;/* Interrupts */
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler
.long UnKnown_IRQhandler

.size VTABLE, . - VTABLE

; /*****************************************************************************
; * Before the MCU driver can be initialized, a basic initialization of 
; * the MCU has to be executed. This MCU specific initialization is typically
; * executed in a start-up code.
; * The start-up code of the MCU shall be executed after power up and any kind 
; * of microcontroller reset. It shall perform very basic and microcontroller
; * specific start-up initialization and shall be kept short because the MCU 
; * clock and PLL are not yet initialized. The start-up code shall cover MCU 
; * specific initialization which is not part of other MCU services or other 
; * MCAL drivers. The following description summarizes the basic functionality 
; * to be included in the start-up code. It is listed for guidance because some 
; * functionality might not be supported in all MCU’s.
; *****************************************************************************/

.section ".startup","ax"
.thumb
.set VTOR_REG, 0xE000ED08

.thumb 

.thumb_func
.globl Reset_Handler
Reset_Handler:
.globl _start
_start:
                CPSID   i               ;/* Mask interrupts */
; /*****************************************************************************
; * The start-up code shall initialize the base addresses for interrupt and trap
; * vector tables. These base addresses are provided as configuration parameters
; * or linker/locator setting.
; * The start-up code shall initialize the interrupt stack pointer if an 
; * interrupt stack is supported by the MCU. The interrupt stack pointer base 
; * address and the stack size are provided as configuration parameter or 
; * linker/locator setting.
; *****************************************************************************/
Set_VTOR:
                ; /* Initialize interrupt vector table */
                LDR     R0, =VTOR_REG
                LDR     R1, =__VECTORTABLE_START
                STR     R1, [R0]

; /*****************************************************************************
; * The start-up code shall initialize the user stack pointer. The user stack 
; * pointer base address and the stack size are provided as configuration 
; * parameter or linker/locator setting.
; *****************************************************************************/
SetStack:
                LDR     R0,  =__STACK_START
                MSR     MSP, R0
; /*****************************************************************************
; * If the MCU supports context save operation, the start-up code shall 
; * initialize the memory which is used for context save operation. The maximum 
; * amount of consecutive context save operations is provided as configuration 
; * parameter or linker/locator setting.
; *****************************************************************************/

; /* Clear SRAM1 to SRAM4 */
ClearSRAM:
    LDR     R0, =__RAM_INIT
    CMP     R0, #0
    BEQ     SRAM_Loop_End
SRAM_Init:
                LDR     R1, =__INIT_SRAM_START
                LDR     R2, =__INIT_SRAM_END

                SUBS    R2, R1
                BLE     SRAM_Loop_End

                MOVS    R0, #0
                MOVS    R3, #0
SRAM_Loop:
                STM     R1!, {R0, R3}
                SUBS    R2, #8
                BGE     SRAM_Loop
SRAM_Loop_End:
                NOP

; /* Clear SRAMH */
ClearSRAMH:
    LDR     R0, =__RAM_INIT
    CMP     R0, #0
    BEQ     SRAMH_Loop_End
SRAMH_Init:
                LDR     R1, =__INIT_SRAMH_START
                LDR     R2, =__INIT_SRAMH_END

                SUBS    R2, R1
                BLE     SRAMH_Loop_End

                MOVS    R0, #0
                MOVS    R3, #0
SRAMH_Loop:
                STM     R1!, {R0, R3}
                SUBS    R2, #8
                BGE     SRAMH_Loop
SRAMH_Loop_End:
                NOP

; /***********************************************************************************************************************
; /* Init Data
; /**********************************************************************************************************************/
InitData:
                LDR     R0, =__INIT_TABLE
                LDR     R6, =__INIT_TABLE_END
                SUBS    R5, R6, R0
                ADDS    R5, R5, #12
SetRegionData:
                SUBS    R5, R5, #12
                CMP     R5, #0
                BEQ     InitData_Loop_End
                LDR     R1, [R0]
                LDR     R2, [R0, #4]
                LDR     R3, [R0, #8]
                ADDS    R0, R0, #12
                SUBS    R4, R3, R2
                CMP     R4, #0
                BEQ     SetRegionData

; /***********************************************************************************************************************
; /* Copy data from rom to ram 
; /**********************************************************************************************************************/
CopyDataRomtoRam:
                LDRB    R4, [R2]
                STRB    R4, [R1]
                ADDS    R1, R1, #1
                ADDS    R2, R2, #1
                CMP     R2, R3
                BEQ     SetRegionData
                B       CopyDataRomtoRam
InitData_Loop_End:
                NOP


; /***********************************************************************************************************************
; /* Clear Bss
; /**********************************************************************************************************************/
ClearBss:
                LDR     R0, =__ZERO_TABLE
                LDR     R6, =__ZERO_TABLE_END
                SUBS    R5, R6, R0
                ADDS    R5, R5, #8
SetaddRegionBss:
                SUBS    R5, R5, #8
                CMP     R5, #0
                BEQ     ClearBss_Loop_End
                LDR     R1, [R0]
                LDR     R2, [R0, #4]
                ADDS    R0, R0, #8
                SUBS    R4, R2, R1
                CMP     R4, #0
                BEQ     SetaddRegionBss
EraseBss:
                MOVS    R4, #0
                STRB    R4, [R1]
                ADDS    R1, R1, #1
                CMP     R2, R1
                BEQ     SetaddRegionBss
                B       EraseBss
ClearBss_Loop_End:
                NOP
    
    
; /*****************************************************************************
; * Call Main Routine                                              
; *****************************************************************************/
_MAIN:
                LDR     R0, =SystemInit
                BLX     R0

                CPSIE   i   ; /* Unmask interrupts */
                BL main


; /***********************************************************************************************************************
; /* End Of Main
; /**********************************************************************************************************************/
.globl _end_of_main
_end_of_main:
                B       _end_of_main
