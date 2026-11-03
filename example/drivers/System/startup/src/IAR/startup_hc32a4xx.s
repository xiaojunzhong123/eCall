; /*****************************************************************************
; * Project Name: XHSC AUTOSAR MCAL
; * File Name: startup_hc32a4xx.s
; *
; * Description: Implementation of the Start-up
; *               Compiler:                 IAR GCC
; *               Autosar Version           4.4.0
; *
; * (c) Copyright 2023 - 2026 XIAOHUA SEMICONDUCTOR CO., LTD. All rights reserved.
; *
; * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT
; * (the "License"). By downloading, installing, or using the Software, you
; * indicate that you accept the terms of the License, and you acknowledge that
; * you have the authority, for yourself or on behalf of your company, to bind
; * your company to these terms. If you do not agree to all of the terms of this
; * License, you should not download or install the Software.
; *
; *****************************************************************************/

#define         VTOR_REG (0xE000ED08)
                MODULE  ?cstartup

                ;; Forward declaration of sections.
                SECTION CSTACK:DATA:NOROOT(3)

                EXTERN main
                EXTERN SystemInit
                EXTERN UnKnown_IRQhandler
                EXTERN NMI_Handler
                EXTERN HardFault_Handler
                EXTERN MemManage_Handler
                EXTERN BusFault_Handler
                EXTERN UsageFault_Handler
                EXTERN SVC_Handler
                EXTERN DebugMon_Handler
                EXTERN PendSV_Handler
                EXTERN SysTick_Handler                
                EXTERN SramH_Vectortable$$Base
                EXTERN Rom_Initvectortable$$Base
                EXTERN Rom_Initvectortable$$Limit
                EXTERN SramH_SramhCode$$Base
                EXTERN Rom_InitSramhCode$$Base
                EXTERN Rom_InitSramhCode$$Limit
                EXTERN SramH_SramhData$$Base
                EXTERN Rom_InitSramhData$$Base
                EXTERN Rom_InitSramhData$$Limit    
                EXTERN Ram_Data$$Base
                EXTERN Rom_Initdata$$Base
                EXTERN Rom_Initdata$$Limit
                EXTERN Ram_McalData$$Base
                EXTERN Rom_InitMcalData$$Base
                EXTERN Rom_InitMcalData$$Limit
                EXTERN Ram_RamCode$$Base
                EXTERN Rom_InitRamCode$$Base
                EXTERN Rom_InitRamCode$$Limit
                EXTERN Ram_Bss$$Base
                EXTERN Ram_Bss$$Limit
                EXTERN Ram_McalBss$$Base
                EXTERN Ram_McalBss$$Limit
                EXTERN SramH_SramhBss$$Base
                EXTERN SramH_SramhBss$$Limit
                EXTERN __RAM_INIT

;/* Copying data from rom to ram and erase bss */
                SECTION .init_table:CONST(4)
__INIT_TABLE
                DCD SramH_Vectortable$$Base
                DCD Rom_Initvectortable$$Base
                DCD Rom_Initvectortable$$Limit
                DCD SramH_SramhCode$$Base
                DCD Rom_InitSramhCode$$Base
                DCD Rom_InitSramhCode$$Limit
                DCD SramH_SramhData$$Base
                DCD Rom_InitSramhData$$Base
                DCD Rom_InitSramhData$$Limit    
                DCD Ram_Data$$Base
                DCD Rom_Initdata$$Base
                DCD Rom_Initdata$$Limit
                DCD Ram_McalData$$Base
                DCD Rom_InitMcalData$$Base
                DCD Rom_InitMcalData$$Limit
                DCD Ram_RamCode$$Base
                DCD Rom_InitRamCode$$Base
                DCD Rom_InitRamCode$$Limit
__INIT_TABLE_END
                SECTION .zero_table:CONST(4)
__ZERO_TABLE
                DCD Ram_Bss$$Base
                DCD Ram_Bss$$Limit
                DCD Ram_McalBss$$Base
                DCD Ram_McalBss$$Limit
                DCD SramH_SramhBss$$Base
                DCD SramH_SramhBss$$Limit
__ZERO_TABLE_END

                SECTION .intvec:CODE:NOROOT(2)
                PUBLIC  VTABLE
                EXTERN __STACK_START
                DATA
VTABLE:
                DCD     __STACK_START             ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; Peripheral Interrupts
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler
                DCD     UnKnown_IRQhandler

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
                THUMB
; Dummy Exception Handlers (infinite loops which can be modified)

                PUBWEAK Reset_Handler
                SECTION .text:CODE:NOROOT:REORDER(2)

                EXTERN __INIT_SRAM_START
                EXTERN __INIT_SRAM_END
                EXTERN __INIT_SRAMH_START
                EXTERN __INIT_SRAMH_END
Reset_Handler:
                CPSID   i               ; Mask interrupts
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
                ; Initialize interrupt vector table
                LDR     R0, =VTOR_REG
                LDR     R1, =VTABLE
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

; Clear SRAM1 to SRAM4
ClearSRAM:
                LDR     R0, =__RAM_INIT
                CMP     R0, #0
                ; Skip if __RAM_INIT is not set
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

; Clear SRAMH
ClearSRAMH:
                LDR     R0, =__RAM_INIT
                CMP     R0, #0
                ; Skip if __RAM_INIT is not set
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


; Init Data
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
;Copy data from rom to ram
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

;Clear Bss
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

                CPSIE   i               ; Unmask interrupts

                BL      main

; /*****************************************************************************
; * End Of Main                                              
; *****************************************************************************/
                PUBLIC _end_of_main
_end_of_main:
                B       _end_of_main

                END
