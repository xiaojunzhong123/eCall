/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Ram_Ip_Types.h
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
 *   @file       Ram_Ip_Types.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef RAM_IP_TYPES_H
#define RAM_IP_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "StandardTypes.h"
#include "System_Hw_Reg.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief SRAM_Sel SRAM Selection
 */
#define MCU_SRAM_SRAMH              (1U << 2U)                 /*!< SRAMH: 0x1FFE0000~0x1FFFFFFF, 128KB */
#define MCU_SRAM_SRAM123            (1U << 0U)                 /*!< SRAM1: 0x20000000~0x2001FFFF, 128KB
                                                                     SRAM2: 0x20020000~0x2003FFFF, 128KB
                                                                     SRAM3: 0x20040000~0x20057FFF, 96KB */
#define MCU_SRAM_SRAM4              (1U << 1U)                 /*!< SRAM4: 0x20058000~0x2005FFFF, 32KB */
#define MCU_SRAM_SRAMB              (1U << 3U)                 /*!< SRAMB: 0x200F0000~0x200F0FFF, 4KB */
#define MCU_SRAM_SRAM_ALL           (MCU_SRAM_SRAMH | MCU_SRAM_SRAM123 | MCU_SRAM_SRAM4 | MCU_SRAM_SRAMB)

/**
 * @brief SRAM_Access_Wait_Cycle SRAM Access Wait Cycle
 */
#define MCU_SRAM_WAIT_CYCLE0        (0U)                        /*!< Wait 0 CPU cycle. */
#define MCU_SRAM_WAIT_CYCLE1        (1U)                        /*!< Wait 1 CPU cycle. */
#define MCU_SRAM_WAIT_CYCLE2        (2U)                        /*!< Wait 2 CPU cycles. */
#define MCU_SRAM_WAIT_CYCLE3        (3U)                        /*!< Wait 3 CPU cycles. */
#define MCU_SRAM_WAIT_CYCLE4        (4U)                        /*!< Wait 4 CPU cycles. */
#define MCU_SRAM_WAIT_CYCLE5        (5U)                        /*!< Wait 5 CPU cycles. */
#define MCU_SRAM_WAIT_CYCLE6        (6U)                        /*!< Wait 6 CPU cycles. */
#define MCU_SRAM_WAIT_CYCLE7        (7U)                        /*!< Wait 7 CPU cycles. */

/**
 * @brief SRAM_ECC_SRAM ECC SRAM Definition
 */
#define MCU_SRAM_ECC_SRAM1          (1U << 0U)
#define MCU_SRAM_ECC_SRAM2          (1U << 1U)
#define MCU_SRAM_ECC_SRAM3          (1U << 2U)
#define MCU_SRAM_ECC_SRAM4          (1U << 3U)
#define MCU_SRAM_ECC_SRAMB          (1U << 4U)
#define MCU_SRAM_ECC_SRAMH          (1U << 5U)
#define MCU_SRAM_ECC_CACHE_RAM      (1U << 6U)
#define MCU_SRAM_ECC_SRAM_ALL       (MCU_SRAM_ECC_SRAM1 | MCU_SRAM_ECC_SRAM2 | MCU_SRAM_ECC_SRAM3 | MCU_SRAM_ECC_SRAM4 | MCU_SRAM_ECC_SRAMB | \
                                     MCU_SRAM_ECC_SRAMH)

/**
 * @brief SRAM_ECC_Mode SRAM ECC Mode
 * @note     XX_INVD: The ECC mode is invalid
 *           XX_MD1:  When 1-bit error occurs, ECC error corrects. No 1-bit-error status flag setting, no interrupt or reset.
 *                    When 2-bit error occurs, ECC error detects. 2-bit-error status flag sets and interrupt or reset occurs.
 *           XX_MD2:  When 1-bit error occurs, ECC error corrects. 1-bit-error status flag sets, no interrupt or reset.
 *                    When 2-bit error occurs, ECC error detects. 2-bit-error status flag sets and interrupt or reset occurs.
 *           XX_MD3:  When 1-bit error occurs, ECC error corrects. 1-bit-error status flag sets and interrupt or reset occurs.
 *                    When 2-bit error occurs, ECC error detects. 2-bit-error status flag sets and interrupt or reset occurs.
 * @{
 */
#define MCU_SRAM_SRAM1_ECC_INVD             (0x0U)
#define MCU_SRAM_SRAM1_ECC_MD1              (SRAMC_CKCR_SRAM1_ECCMOD_0)
#define MCU_SRAM_SRAM1_ECC_MD2              (SRAMC_CKCR_SRAM1_ECCMOD_1)
#define MCU_SRAM_SRAM1_ECC_MD3              (SRAMC_CKCR_SRAM1_ECCMOD)
#define MCU_SRAM_SRAM2_ECC_INVD             (0x0U)
#define MCU_SRAM_SRAM2_ECC_MD1              (SRAMC_CKCR_SRAM2_ECCMOD_0)
#define MCU_SRAM_SRAM2_ECC_MD2              (SRAMC_CKCR_SRAM2_ECCMOD_1)
#define MCU_SRAM_SRAM2_ECC_MD3              (SRAMC_CKCR_SRAM2_ECCMOD)
#define MCU_SRAM_SRAM3_ECC_INVD             (0x0U)
#define MCU_SRAM_SRAM3_ECC_MD1              (SRAMC_CKCR_SRAM3_ECCMOD_0)
#define MCU_SRAM_SRAM3_ECC_MD2              (SRAMC_CKCR_SRAM3_ECCMOD_1)
#define MCU_SRAM_SRAM3_ECC_MD3              (SRAMC_CKCR_SRAM3_ECCMOD)
#define MCU_SRAM_SRAM4_ECC_INVD             (0x0U)
#define MCU_SRAM_SRAM4_ECC_MD1              (SRAMC_CKCR_SRAM4_ECCMOD_0)
#define MCU_SRAM_SRAM4_ECC_MD2              (SRAMC_CKCR_SRAM4_ECCMOD_1)
#define MCU_SRAM_SRAM4_ECC_MD3              (SRAMC_CKCR_SRAM4_ECCMOD)
#define MCU_SRAM_SRAMB_ECC_INVD             (0x0U)
#define MCU_SRAM_SRAMB_ECC_MD1              (SRAMC_CKCR_SRAMB_ECCMOD_0)
#define MCU_SRAM_SRAMB_ECC_MD2              (SRAMC_CKCR_SRAMB_ECCMOD_1)
#define MCU_SRAM_SRAMB_ECC_MD3              (SRAMC_CKCR_SRAMB_ECCMOD)
#define MCU_SRAM_SRAMH_ECC_INVD             (0x0U)
#define MCU_SRAM_SRAMH_ECC_MD1              (SRAMC_CKCR_SRAMH_ECCMOD_0)
#define MCU_SRAM_SRAMH_ECC_MD2              (SRAMC_CKCR_SRAMH_ECCMOD_1)
#define MCU_SRAM_SRAMH_ECC_MD3              (SRAMC_CKCR_SRAMH_ECCMOD)
#define MCU_SRAM_CACHE_ECC_INVD             (0x0U)
#define MCU_SRAM_CACHE_ECC_MD1              (SRAMC_CKCR_CACHE_ECCMOD_0)
#define MCU_SRAM_CACHE_ECC_MD2              (SRAMC_CKCR_CACHE_ECCMOD_1)
#define MCU_SRAM_CACHE_ECC_MD3              (SRAMC_CKCR_CACHE_ECCMOD)
#define MCU_SRAM_ECC_MD_INVD                (MCU_SRAM_SRAM1_ECC_INVD & MCU_SRAM_SRAM2_ECC_INVD & MCU_SRAM_SRAM3_ECC_INVD & \
                                             MCU_SRAM_SRAM4_ECC_INVD & MCU_SRAM_SRAMB_ECC_INVD & MCU_SRAM_SRAMH_ECC_INVD & \
                                             MCU_SRAM_CACHE_ECC_INVD)

/**
 * @brief SRAM_Exception_Type SRAM exception type
 * @note Even-parity check error, ECC check error.
 */
#define MCU_SRAM_EXP_TYPE_NMI               (0U)
#define MCU_SRAM_EXP_TYPE_RST               (1U)


/**
 * @brief SRAM_Check_SRAM SRAM check sram
 */
#define MCU_SRAM_CHECK_SRAM1                (SRAMC_CKCR_SRAM1_ECCOAD)
#define MCU_SRAM_CHECK_SRAM2                (SRAMC_CKCR_SRAM2_ECCOAD)
#define MCU_SRAM_CHECK_SRAM3                (SRAMC_CKCR_SRAM3_ECCOAD)
#define MCU_SRAM_CHECK_SRAM4                (SRAMC_CKCR_SRAM4_ECCOAD)
#define MCU_SRAM_CHECK_SRAMB                (SRAMC_CKCR_SRAMB_ECCOAD)
#define MCU_SRAM_CHECK_SRAMH                (SRAMC_CKCR_SRAMH_ECCOAD)
#define MCU_SRAM_CHECK_CACHE_RAM            (SRAMC_CKCR_CACHE_ECCOAD)
#define MCU_SRAM_CHECK_SRAM_ALL             (MCU_SRAM_CHECK_SRAMB | MCU_SRAM_CHECK_SRAM4 | MCU_SRAM_CHECK_SRAM3 | MCU_SRAM_CHECK_SRAM2 | \
                                             MCU_SRAM_CHECK_SRAM1 | MCU_SRAM_CHECK_SRAMH | MCU_SRAM_CHECK_CACHE_RAM)
/**
 * @brief SRAM_Err_Status_Flag SRAM Error Status Flag
 * @{
 */
#define MCU_SRAM_FLAG_SRAM1_1ERR            (SRAMC_CKSR_SRAM1_1ERR)     /*!< SRAM1 ECC 1-bit error. */
#define MCU_SRAM_FLAG_SRAM1_2ERR            (SRAMC_CKSR_SRAM1_2ERR)     /*!< SRAM1 ECC 2-bit error. */
#define MCU_SRAM_FLAG_SRAM2_1ERR            (SRAMC_CKSR_SRAM2_1ERR)     /*!< SRAM2 ECC 1-bit error. */
#define MCU_SRAM_FLAG_SRAM2_2ERR            (SRAMC_CKSR_SRAM2_2ERR)     /*!< SRAM2 ECC 2-bit error. */
#define MCU_SRAM_FLAG_SRAM3_1ERR            (SRAMC_CKSR_SRAM3_1ERR)     /*!< SRAM3 ECC 1-bit error. */
#define MCU_SRAM_FLAG_SRAM3_2ERR            (SRAMC_CKSR_SRAM3_2ERR)     /*!< SRAM3 ECC 2-bit error. */
#define MCU_SRAM_FLAG_SRAM4_1ERR            (SRAMC_CKSR_SRAM4_1ERR)     /*!< SRAM4 ECC 1-bit error. */
#define MCU_SRAM_FLAG_SRAM4_2ERR            (SRAMC_CKSR_SRAM4_2ERR)     /*!< SRAM4 ECC 2-bit error. */
#define MCU_SRAM_FLAG_SRAMH_1ERR            (SRAMC_CKSR_SRAMH_1ERR)     /*!< SRAMH ECC 1-bit error. */
#define MCU_SRAM_FLAG_SRAMH_2ERR            (SRAMC_CKSR_SRAMH_2ERR)     /*!< SRAMH ECC 2-bit error. */
#define MCU_SRAM_FLAG_SRAMB_1ERR            (SRAMC_CKSR_SRAMB_1ERR)     /*!< SRAMB ECC 1-bit error. */
#define MCU_SRAM_FLAG_SRAMB_2ERR            (SRAMC_CKSR_SRAMB_2ERR)     /*!< SRAMB ECC 2-bit error. */
#define MCU_SRAM_FLAG_CACHE_1ERR            (SRAMC_CKSR_CACHE_1ERR)     /*!< Cache ECC 1-bit error. */
#define MCU_SRAM_FLAG_CACHE_2ERR            (SRAMC_CKSR_CACHE_2ERR)     /*!< Cache ECC 2-bit error. */
#define MCU_SRAM_FLAG_ALL                   (0x0007FEF0U)

/**
 * @brief SRAM Configuration Bits Mask.
 */
#define MCU_SRAM_ECC_MD_MASK            (SRAMC_CKCR_ECCMOD)
#define MCU_SRAM_CYCLE_MASK             (0x00000007U)

/* SRAM Register Protect Key */
#define MCU_SRAM_REG_LOCK_KEY       (0x76U)
#define MCU_SRAM_REG_UNLOCK_KEY     (0x77U)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief  Mcu RamState define state of RAM.
 */
typedef enum {
  MCU_RAMSTATE_INVALID    = 0U,
  MCU_RAMSTATE_VALID
} Ram_Ip_StateType;

typedef enum {
  MCU_ERRORINJECT_DISABLE    = 0U,
  MCU_ERRORINJECT_ENABLE
} Ram_Ip_ErrorInjectEnableType;

/**
 * @brief  The Ram_Ip_RamIndexType variable for indexing RAM sections.
 */
typedef uint32_t Ram_Ip_RamIndexType;

/**
 * @brief  The Ram_Ip_RamSizeType RAM section size.
 */
typedef uint32_t Ram_Ip_RamSizeType;

/**
 * @brief  The Ram_Ip_RamWriteSizeType RAM section write size.
 */
typedef uint32_t Ram_Ip_RamWriteSizeType;

/**
 * @brief  Mcu RamSection Ram Sector structure.
 */
typedef struct {
  uint32_t McuRamSectorId_u32;                                     /*!< Ram sector ID.                           */
  uint8_t* McuRamSectionBase_ptr;                                  /*!< RAM section base address.                */
  uint32_t McuRamSectionSize_u32;                                  /*!< RAM section size.                        */
  uint64_t McuRamDefaultValue_u64;                                 /*!< RAM default value for initialization.    */
  uint32_t McuRamWriteSize_u32;                                    /*!< RAM section write size.                  */
} Ram_Ip_HwRamConfigType;

/**
 * @brief  This structure contains SRAM error injection bits configuration.
 */
typedef struct {
    uint32_t McuSramDataBit0_31_u32;            /*!< data bits 0~31 */
    uint32_t McuSramDataBit32_63_u32;           /*!< data bits 32~63 */
    uint32_t McuSramDataBit64_95_u32;           /*!< data bits 64~95 */
    uint32_t McuSramDataBit96_127_u32;          /*!< data bits 96~127 */
    uint32_t McuSramDataBit128_136_u32;         /*!< data bits 128~136 */
} Ram_Ip_ErrorInjectBitType;

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  ENUMS
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* SRAM_IP_TYPES_H */

/*---------------------- End of File ----------------------------------------*/
