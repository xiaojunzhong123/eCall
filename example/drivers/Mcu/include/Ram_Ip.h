/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Ram_Ip.h
 *
 * Description: Implementation of RAM IP layer
 *              Compiler:                IAR GCC
 *              AutoSAR Version:         4.4.0
 *
 *
 * Copyright (c) 2022 - 2026 Xiaohua Semiconductor Co., Ltd.
 * 
 * All rights reserved.
 *
 * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT (the "License"). By downloading, installing, or
 * using the Software, you indicate that you accept the terms of the License, and you acknowledge that you have the
 * authority, for yourself or on behalf of your company, to bind your company to these terms. If you do not agree to all
 * of the terms of this License, you should not download or install the Software.
 *
 **********************************************************************************************************************/

/**
 *   @file       Ram_Ip.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef RAM_IP_H
#define RAM_IP_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Ram_Ip_Types.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          Specifies access wait cycle for SRAM.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_SetWaitCycle_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_SramSel_u32         The SRAM selection.
 *                                           This parameter can be values of
 *  @param[in]      para_WriteCycle_u32      The write access wait cycle for the specified SRAM
 *                                           This parameter can be a value of
 *  @param[in]      para_ReadCycle_u32       The read access wait cycle for the specified SRAM.
 *                                           This parameter can be a value of
 *  @arg            MCU_SRAM_WAIT_CYCLE0     Wait 0 CPU cycle.
 *  @arg            MCU_SRAM_WAIT_CYCLE1     Wait 1 CPU cycle.
 *  @arg            MCU_SRAM_WAIT_CYCLE2     Wait 2 CPU cycles.
 *  @arg            MCU_SRAM_WAIT_CYCLE3     Wait 3 CPU cycles.
 *  @arg            MCU_SRAM_WAIT_CYCLE4     Wait 4 CPU cycles.
 *  @arg            MCU_SRAM_WAIT_CYCLE5     Wait 5 CPU cycles.
 *  @arg            MCU_SRAM_WAIT_CYCLE6     Wait 6 CPU cycles.
 *  @arg            MCU_SRAM_WAIT_CYCLE7     Wait 7 CPU cycles.
 *
 *  @return         void
 *
 *  @note           Call SRAM_REG_Unlock to unlock register WTCR first.
 *
 *  @pre            None
 *
 */
void Ram_Ip_SetWaitCycle(
    uint32_t para_SramSel_u32,
    uint32_t para_WriteCycle_u32,
    uint32_t para_ReadCycle_u32
);

/**
 *  @brief          Specifies ECC mode.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_SetEccMode_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_EccSram_u32    The ECC SRAM.
 *                                      This parameter can be any combination of
 *  @param[in]      para_EccMode_u32    The ECC mode.
 *                                      This parameter can be any combination of
 *
 *  @return         void
 *
 *  @note           Call SRAM_REG_Unlock to unlock register CKCR first.
 *                  The sram of u32EccMode should be the same with the sram of u32EccSram.
 *
 *  @pre            None
 *
 */
void Ram_Ip_SetEccMode(
    uint32_t para_EccSram_u32,
    uint32_t para_EccMode_u32
);

/**
 *  @brief          Specifies the exception type while the chosen sram check error occurred.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_SetExceptionType_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_CheckSram_u32        The check SRAM.
 *                                            This parameter can be any combination of
 *  @param[in]      para_ExceptionType_u32    The operation after check error occurred.
 *                                            This parameter can be a value of
 *
 *  @return         void
 *
 *  @note           Call SRAM_REG_Unlock to unlock register CKCR first.
 *
 *  @pre            None
 *
 */
void Ram_Ip_SetExceptionType(
    uint32_t para_CheckSram_u32,
    uint32_t para_ExceptionType_u32
);

/**
 *  @brief          This function provides status of RAM
 *                  Functions get status of SRAM status register
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_GetRamState_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_Flag_u32    The flag of SRAM.
 *                                   This parameter can be values of
 *
 *  @return         Ram_Ip_StateType    content is valid or not valid             
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Ram_Ip_StateType Ram_Ip_GetRamState(
    uint32_t para_Flag_u32
);

/**
 *  @brief          Enable or disable error injection.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_ErrorInjectCmd_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_EccSram_u32    The SRAM selection.
 *                                      This parameter can be any combination of
 *  @param[in]      para_NewState_en    An
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Ram_Ip_ErrorInjectCmd(
    uint32_t para_EccSram_u32,
    Ram_Ip_ErrorInjectEnableType para_NewState_en
);

/**
 *  @brief          Enable or disable error injection bit of SRAM_ECC_SRAM.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_ErrorInjectBitCmd_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_EccSram_u32    The SRAM selection.
 *                                      This parameter can be any combination of
 *  @param[in]      para_BitSel_u64     Bit selection.  Only bit0~bit38 valid.
 *  @param[in]      para_NewState_en    An
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Ram_Ip_ErrorInjectBitCmd(
    uint32_t para_EccSram_u32,
    uint64_t para_BitSel_u64,
    Ram_Ip_ErrorInjectEnableType para_NewState_en
);

/**
 *  @brief          Enable or disable error injection bit(s) of SRAM Cache.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_Cache_ErrorInjectBitCmd_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_BitSel_pst     Pointer to a stc_sram_ecc_err_inject_bit_t structure.
 *  @param[in]      para_NewState_en    An
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Ram_Ip_Cache_ErrorInjectBitCmd(
    const Ram_Ip_ErrorInjectBitType * para_BitSel_pst,
    Ram_Ip_ErrorInjectEnableType para_NewState_en
);

/**
 *  @brief          Get access address when 1bit or 2bit ECC error occurs in SRAM0SRAMB.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_GetEccErrorAddr_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_EccSram_u32    The SRAM selection.
 *                                      This parameter can be any combination of
 *
 *  @return         uint32_t    ECC error access address
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
uint32_t Ram_Ip_GetEccErrorAddr(
    uint32_t para_EccSram_u32
);

/**
 *  @brief          This function initializes a given RAM section of the MCU on the common (main) platform.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_InitRamSection_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_RamConfig_ptr    Pointer to the SRAM section configuration structure.
 *
 *  @return         Std_ReturnType    Status of RAM section initialization
 *  @retval         E_OK              The RAM section init was successful.
 *  @retval         E_NOT_OK          The RAM section init was not successful.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Ram_Ip_InitRamSection(
    const Ram_Ip_HwRamConfigType * para_RamConfig_ptr
);

/**
 *  @brief          This function clear SRAM flag error and MNI flag
 *                  when a user notification the Function call by ISR routine, for MCU SRAM event is configured
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_SRAM_INT_ERR_Activity
 *                  Sync/Async:        Synchronous
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Ram_Ip_SRAM_INT_ERR(
    void
);

/**
 * @brief      This function configures SRAM CKCR register for ECC control
 *
 * @param[in]  para_EccMode_u32    The ECC mode to be configured for each SRAM region
 *                                 Bit[31:14] Reserved
 *                                 Bit[13:0] ECCCOAD[13:0] ECC control bits for each SRAM region
 *
 * @return     void
 *
 * @note       When enabling RAM ECC check, must initialize RAM first
 *
 */
void Ram_Ip_SramCkcrConfig(
    const uint32_t para_EccMode_u32
);

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* SRAM_IP_H */

/*---------------------- End of File ----------------------------------------*/
