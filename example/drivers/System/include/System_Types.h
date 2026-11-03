/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: System_Types.h
 *
 * Description: Implementation of SYSTEM TYPES High Level layer
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
 *   @file       System_Types.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 SYSTEM Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifndef SYSTEM_TYPES_H
#define SYSTEM_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"
#include "System_Hw_Reg.h"
#include "Compiler_Cfg.h"
/***********************************************************************************************************************
 *   HEADER FILE VERSION INFORMATION
 **********************************************************************************************************************/
#define SYSTEM_TYPES_AR_RELEASE_MAJOR_VERSION        4
#define SYSTEM_TYPES_AR_RELEASE_MINOR_VERSION        4
#define SYSTEM_TYPES_AR_RELEASE_REVISION_VERSION     0
/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief Function pointer type to void/void function
 */
typedef void (*func_ptr_t)(
    void
);

/**
 * @brief   Parameter is invalid
 * @details Errors and exceptions that will be detected by the SYSTEM driver
 */
#define SYSTEM_E_INVALID_PARAMETER                       ((uint8_t)0x04U)
/**
 * @brief   Pointer is invalid
 * @details Errors and exceptions that will be detected by the SYSTEM driver
 */
#define SYSTEM_E_INVALID_POINTER                       ((uint8_t)0x03U)

/**
 * @brief   Channel invalid
 * @details Errors and exceptions that will be detected by the SYSTEM driver
 */
#define SYSTEM_E_INVALID_CHANNEL                        ((uint8_t)0x02U)

/**
 * @brief   System is not configured
 * @details Errors and exceptions that will be detected by the SYSTEM driver
 */
#define SYSTEM_E_NOT_CONFIGURED                         ((uint8_t)0x01U)

/**
 * @brief   API service ID for System_EnableIrq() function
 * @details Parameters used when raising an error/exception
 */
#define SYSTEM_ENABLEIRQ_ID                     ((uint8_t)0x01U)

/**
 * @brief   API service ID for System_DisableIrq() function
 * @details Parameters used when raising an error/exception
 */
#define SYSTEM_DISABLEIRQ_ID                     ((uint8_t)0x02U)

/**
 * @brief   API service ID for System_Init() function
 * @details Parameters used when raising an error/exception
 */
#define SYSTEM_INIT_ID                     ((uint8_t)0x03U)

/**
 * @brief   API service ID for System_GenerateSwTrigger() function
 * @details Parameters used when raising an error/exception
 */

#define SYSTEM_GENERATESWTRIGGER_ID       ((uint8_t)0x04U)
/**
 * @brief   API service ID for System_UpdateAosConfigSet() function
 * @details Parameters used when raising an error/exception
 */

#define SYSTEM_UPDATEAOSCONFIGSET_ID       ((uint8_t)0x05U)
/**
 * @brief   API service ID for System_ResetAllAosTriggerTarget() function
 * @details Parameters used when raising an error/exception
 */
#define SYSTEM_RESETALLAOSTRIGGERTARGET_ID       ((uint8_t)0x06U)

/**
 * @brief   API service ID for System_VersionInfo() function
 * @details Parameters used when raising an error/exception
 */
#define SYSTEM_VERSIONINFO_ID                ((uint8_t)0x07U)

/**
 * @brief   API service ID for System_UpdateIntConfigSet() function
 * @details Parameters used when raising an error/exception
 */
#define SYSTEM_INTUPDATECONFIG_ID                ((uint8_t)0x08U)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief  Interrupt priority definition
 * @details SRS ID:     AR_RS_System_00001
 */
typedef enum
{
  PRIORITY_0   = 0,
  PRIORITY_1   = 1,
  PRIORITY_2   = 2,
  PRIORITY_3   = 3,
  PRIORITY_4   = 4,
  PRIORITY_5   = 5,
  PRIORITY_6   = 6,
  PRIORITY_7   = 7,
  PRIORITY_8   = 8,
  PRIORITY_9   = 9,
  PRIORITY_10  = 10,
  PRIORITY_11  = 11,
  PRIORITY_12  = 12,
  PRIORITY_13  = 13,
  PRIORITY_14  = 14,
  PRIORITY_15  = 15,
  PRIORITY_MAX = 16
} System_IntPriorityType;

/**
 * @brief  Trigger Target enumeration
 * @details SRS ID:     AR_RS_System_00002
 */
typedef enum
{
  DCU_TRGSEL1                  = 0x04,
  DCU_TRGSEL2                  = 0x08,
  DCU_TRGSEL3                  = 0x0C,
  DCU_TRGSEL4                  = 0x10,
  DMA1_TRGSEL0                 = 0x14,
  DMA1_TRGSEL1                 = 0x18,
  DMA1_TRGSEL2                 = 0x1C,
  DMA1_TRGSEL3                 = 0x20,
  DMA1_TRGSEL4                 = 0x24,
  DMA1_TRGSEL5                 = 0x28,
  DMA1_TRGSEL6                 = 0x2C,
  DMA1_TRGSEL7                 = 0x30,
  DMA2_TRGSEL0                 = 0x34,
  DMA2_TRGSEL1                 = 0x38,
  DMA2_TRGSEL2                 = 0x3C,
  DMA2_TRGSEL3                 = 0x40,
  DMA2_TRGSEL4                 = 0x44,
  DMA2_TRGSEL5                 = 0x48,
  DMA2_TRGSEL6                 = 0x4C,
  DMA2_TRGSEL7                 = 0x50,
  DMA_TRGSELRC                 = 0x54,
  TMR6_HTSSR0                  = 0x58,
  TMR6_HTSSR1                  = 0x5C,
  TMR6_HTSSR2                  = 0x60,
  TMR6_HTSSR3                  = 0x64,
  PEVNTTRGSR12                 = 0x68,
  PEVNTTRGSR34                 = 0x6C,
  TMR0_HTSSR                   = 0x70,
  TMR2_HTSSR                   = 0x74,
  HASH_ITRGSELA                = 0x78,
  HASH_ITRGSELB                = 0x7C,
  TMRA_HTSSR0                  = 0x80,
  TMRA_HTSSR1                  = 0x84,
  TMRA_HTSSR2                  = 0x88,
  TMRA_HTSSR3                  = 0x8C,
  OTS_TRG                      = 0x90,
  ADC1_ITRGSELR0               = 0x94,
  ADC1_ITRGSELR1               = 0x98,
  ADC2_ITRGSELR0               = 0x9C,
  ADC2_ITRGSELR1               = 0xA0,
  ADC3_ITRGSELR0               = 0xA4,
  ADC3_ITRGSELR1               = 0xA8
} System_AosTargetTriggerTypes;

/**
 * @brief  AOS Configuration type structure
 * @details SRS ID:     AR_RS_System_00003
 */
typedef struct
{
  System_AosTargetTriggerTypes SystemAOSTrigerSel_u32;  /* System AOS trigger Target selected. */
  uint8_t SystemAosCommTrigger1Status_u8;                 /* System AOS common trigger event 1 state */
  uint8_t SystemAosCommTrigger2Status_u8;                 /* System AOS common trigger event 2 state */
  en_event_src_t SystemAosEventSrc_en;                  /* System AOS event source */
} System_AosTriggerTargetConfigType;

/**
 * @brief  AOS Configuration set type
 * @details SRS ID:     AR_RS_System_00004
 */
typedef struct
{
  uint32_t SystemAosConfigSetIdx_u32;                                               /* System AOS ConfigSet ID */
  uint8_t SystemAosNumberOfAosTargetConfig_u8;                                      /* Total number of AOS Target configures. */
  en_event_src_t SystemAosCommonTrigger1_en;                                      /* Common trigger Source 1 */
  en_event_src_t SystemAosCommonTrigger2_en;                                      /* Common trigger Source 2 */
  const System_AosTriggerTargetConfigType *SystemAosTriggerTargetConfigList_ptr;  /* System AOS Trigger Target Config List */
} System_AosConfigSetType;

/**
 * @brief  INTC Configuration type
 * @details SRS ID:     AR_RS_System_00005
 */
typedef struct
{
  en_int_src_t SystemIntReqSource_en;                 /* Interrupt request source */
  IRQn_Type SystemIntIrqNumber_en;                    /* Interrupt request vector number */
  System_IntPriorityType SystemIntPriority_en;        /* Interrupt priority */
  func_ptr_t SystemIntIrqHandler_dd;                  /* Interrupt request handler */
} System_IntConfigType;

/**
 * @brief  INTC Configuration set type
 * @details SRS ID:     AR_RS_System_00006
 */
typedef struct
{
  uint32_t SystemIntConfigSetIdx_u32;                          /* System INTC ConfigSet ID */
  uint8_t SystemIntNumberOfIntcConfig_u8;                      /* Total number of INTC Config */
  const System_IntConfigType *SystemIntListConfig_ptr;       /* System Intc List Config */
} System_IntConfigSetType;

/**
 * @brief  System Configuration type structure
 * @details SRS ID:     AR_RS_System_00007
 */
typedef struct
{
  uint32_t SystemIntNumberOfIntcConfigSet_u32;                 /* Total number of INTC Set configures. */
  const System_IntConfigSetType *SystemIntConfigSet_ptr;    /* Pointer to INTC configures set. */  
  uint32_t SystemAosNumberOfAosSetConfig_u32;                   /* Total number of AOS Set configures. */
  const System_AosConfigSetType *SystemAosConfigSet_ptr;      /* Pointer to AOS configures set. */
} System_ConfigType;

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* SYSTEM_TYPES_H */

/*---------------------- End of File ----------------------------------------*/
