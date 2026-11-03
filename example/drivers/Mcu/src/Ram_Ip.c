/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Ram_Ip.c
 *
 * Description: Implementation of MCU IP layer
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
 *   @file       Ram_Ip.c
 *   @version    V1.0.2
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

/***********************************************************************************************************************
 *  MISRA VIOLATIONS - Summary and analysis.
 **********************************************************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Ram_Ip.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief SRAM_Configuration_Bits_Mask SRAM Configuration Bits Mask
 */
#define SRAM_CYCLE_MASK                 (0x00000007U)

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

static void Ram_Ip_ClearStatus(
    uint32_t para_Flag_u32
);

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          Clear the status of the specified flag of SRAM.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_ClearStatus_Activity
 *                  Sync/Async:        Synchronous
 *  @param[in]      u32Flag     The flag of SRAM.
 *                              This parameter can be values of
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Ram_Ip_ClearStatus(
    uint32_t para_Flag_u32
)
{
    WRITE_REG32(CM_SRAMC->CKSR, para_Flag_u32);
}

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          Specifies access wait cycle for SRAM.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_SetWaitCycle_Activity
 *                  Sync/Async:        Synchronous
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
)
{
  uint8_t f_Index_u8 = 0U;
  uint8_t f_WriteOffset_u8;
  uint8_t f_ReadOffset_u8;
  uint32_t f_SramSelTemp_u32 = para_SramSel_u32;

  /* Iterate through each bit in the SRAM selection mask */
  while (0U != f_SramSelTemp_u32) 
  {
      /* Check if the current bit is set (this SRAM is selected) */
      if (0U != (f_SramSelTemp_u32 & 0x1U)) 
      {
          /* Calculate bit positions for read and write cycles in the WTCR register
             Each SRAM uses 8 bits: 4 bits for read cycles + 4 bits for write cycles */
          f_ReadOffset_u8 = f_Index_u8 * 8U;        /* Read cycles start at bit position index*8 */
          f_WriteOffset_u8 = f_ReadOffset_u8 + 4U;  /* Write cycles start 4 bits after read cycles */
          
          /* Modify the wait cycle register: clear the existing values and set new ones
             for both read and write cycles of the selected SRAM */
          MODIFY_REG32(CM_SRAMC->WTCR,
                      ((SRAM_CYCLE_MASK << f_WriteOffset_u8) | (SRAM_CYCLE_MASK << f_ReadOffset_u8)),
                      ((para_WriteCycle_u32 << f_WriteOffset_u8) | (para_ReadCycle_u32 << f_ReadOffset_u8)));
      }
      /* Move to the next bit position */
      f_SramSelTemp_u32 >>= 1U;
      f_Index_u8++;
  }
}

/**
 *  @brief          Specifies ECC mode.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_SetEccMode_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      u32EccSram    The ECC SRAM.
 *                                This parameter can be any combination of
 *  @param[in]      u32EccMode    The ECC mode.
 *                                This parameter can be any combination of
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
)
{
    uint32_t f_Mask_u32 = 0U;
    uint32_t f_Position_u32 = 0U;
    uint32_t f_EccSramTemp_u32 = para_EccSram_u32;

    /* Iterate through each bit in the ECC SRAM selection mask */
    while (0U != f_EccSramTemp_u32) 
    {
        /* Check if the current bit is set (this SRAM is selected for ECC configuration) */
        if (1U == (f_EccSramTemp_u32 & 0x01U)) 
        {
          /* Build the mask for ECC mode bits based on chip variant
             Each SRAM uses 2 bits for ECC mode configuration */
          #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
          f_Mask_u32 |= (SRAMC_CKCR_SRAM1_ECCMOD << f_Position_u32);
          #else
          f_Mask_u32 |= (SRAMC_CKCR_ECCMOD << f_Position_u32);
          #endif
        }
        /* Move to the next bit position and increment position by 2 (2 bits per SRAM) */
        f_EccSramTemp_u32 >>= 1U;
        f_Position_u32 += 2U;
    }

    /* Modify the control register to set ECC mode for selected SRAMs */
    MODIFY_REG32(CM_SRAMC->CKCR, f_Mask_u32, para_EccMode_u32);
}

/**
 *  @brief          Specifies the exception type while the chosen sram check error occurred.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_SetExceptionType_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      u32CheckSram        The check SRAM.
 *                                      This parameter can be any combination of
 *  @param[in]      u32ExceptionType    The operation after check error occurred.
 *                                      This parameter can be a value of
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
)
{
    if (MCU_SRAM_EXP_TYPE_RST == para_ExceptionType_u32) 
    {
        SET_REG32_BIT(CM_SRAMC->CKCR, para_CheckSram_u32);
    } 
    else 
    {
        CLR_REG32_BIT(CM_SRAMC->CKCR, para_CheckSram_u32);
    }
}

/**
 *  @brief          This function provides status of RAM
 *                  Functions get status of SRAM status register
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_GetRamState_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_Flag_u32      The flag of SRAM.
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
)
{
  Ram_Ip_StateType  f_RamStatus_en = MCU_RAMSTATE_VALID;
  if (0U != READ_REG32_BIT(CM_SRAMC->CKSR, para_Flag_u32)) 
  {
      f_RamStatus_en = MCU_RAMSTATE_INVALID;
  }
  return f_RamStatus_en;
}

#if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
/**
 *  @brief          Enable or disable error injection.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_ErrorInjectCmd_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      u32EccSram    The SRAM selection.
 *                                This parameter can be any combination of
 *  @param[in]      enNewState    An
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
)
{
    if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM1)) 
    {
        WRITE_REG32(CM_SRAMC->SRAM1_EIEN, para_NewState_en);
    }
    if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM2)) 
    {
        WRITE_REG32(CM_SRAMC->SRAM2_EIEN, para_NewState_en);
    }
    if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM3)) 
    {
        WRITE_REG32(CM_SRAMC->SRAM3_EIEN, para_NewState_en);
    }
    if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM4)) 
    {
        WRITE_REG32(CM_SRAMC->SRAM4_EIEN, para_NewState_en);
    }
    if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_CACHE_RAM)) 
    {
        WRITE_REG32(CM_SRAMC->CACHE_EIEN, para_NewState_en);
    }
    if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAMH)) 
    {
        WRITE_REG32(CM_SRAMC->SRAMH_EIEN, para_NewState_en);
    }
    if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAMB)) 
    {
        WRITE_REG32(CM_SRAMC->SRAMB_EIEN, para_NewState_en);
    }
}
#endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

#if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
/**
 *  @brief          Enable or disable error injection bit of SRAM_ECC_SRAM.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_ErrorInjectBitCmd_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      u32EccSram    The SRAM selection.
 *                                This parameter can be any combination of
 *  @param[in]      u64BitSel     Bit selection.  Only bit0~bit38 valid.
 *  @param[in]      enNewState    An
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
)
{
    uint32_t f_Eibit0_u32 = (uint32_t)(para_BitSel_u64 & 0xFFFFFFFFU);
    uint32_t f_Eibit1_u32 = (uint32_t)((para_BitSel_u64 >> 32U) & 0x0000007FU);

    if (MCU_ERRORINJECT_ENABLE == para_NewState_en) 
    {
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM1)) 
        {
            SET_REG32_BIT(CM_SRAMC->SRAM1_EIBIT0, f_Eibit0_u32);
            SET_REG32_BIT(CM_SRAMC->SRAM1_EIBIT1, f_Eibit1_u32);
        }
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM2)) 
        {
            SET_REG32_BIT(CM_SRAMC->SRAM2_EIBIT0, f_Eibit0_u32);
            SET_REG32_BIT(CM_SRAMC->SRAM2_EIBIT1, f_Eibit1_u32);
        }
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM3)) 
        {
            SET_REG32_BIT(CM_SRAMC->SRAM3_EIBIT0, f_Eibit0_u32);
            SET_REG32_BIT(CM_SRAMC->SRAM3_EIBIT1, f_Eibit1_u32);
        }
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM4)) 
        {
            SET_REG32_BIT(CM_SRAMC->SRAM4_EIBIT0, f_Eibit0_u32);
            SET_REG32_BIT(CM_SRAMC->SRAM4_EIBIT1, f_Eibit1_u32);
        }
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAMH)) 
        {
            SET_REG32_BIT(CM_SRAMC->SRAMH_EIBIT0, f_Eibit0_u32);
            SET_REG32_BIT(CM_SRAMC->SRAMH_EIBIT1, f_Eibit1_u32);
        }
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAMB)) 
        {
            SET_REG32_BIT(CM_SRAMC->SRAMB_EIBIT0, f_Eibit0_u32);
            SET_REG32_BIT(CM_SRAMC->SRAMB_EIBIT1, f_Eibit1_u32);
        }
    } 
    else 
    {
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM1)) 
        {
            CLR_REG32_BIT(CM_SRAMC->SRAM1_EIBIT0, f_Eibit0_u32);
            CLR_REG32_BIT(CM_SRAMC->SRAM1_EIBIT1, f_Eibit1_u32);
        }
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM2)) 
        {
            CLR_REG32_BIT(CM_SRAMC->SRAM2_EIBIT0, f_Eibit0_u32);
            CLR_REG32_BIT(CM_SRAMC->SRAM2_EIBIT1, f_Eibit1_u32);
        }
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM3)) 
        {
            CLR_REG32_BIT(CM_SRAMC->SRAM3_EIBIT0, f_Eibit0_u32);
            CLR_REG32_BIT(CM_SRAMC->SRAM3_EIBIT1, f_Eibit1_u32);
        }
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAM4)) 
        {
            CLR_REG32_BIT(CM_SRAMC->SRAM4_EIBIT0, f_Eibit0_u32);
            CLR_REG32_BIT(CM_SRAMC->SRAM4_EIBIT1, f_Eibit1_u32);
        }
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAMH)) 
        {
            CLR_REG32_BIT(CM_SRAMC->SRAMH_EIBIT0, f_Eibit0_u32);
            CLR_REG32_BIT(CM_SRAMC->SRAMH_EIBIT1, f_Eibit1_u32);
        }
        if (0U != (para_EccSram_u32 & MCU_SRAM_ECC_SRAMB)) 
        {
            CLR_REG32_BIT(CM_SRAMC->SRAMB_EIBIT0, f_Eibit0_u32);
            CLR_REG32_BIT(CM_SRAMC->SRAMB_EIBIT1, f_Eibit1_u32);
        }
    }
}
#endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

#if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
/**
 *  @brief          Enable or disable error injection bit(s) of SRAM Cache.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_Cache_ErrorInjectBitCmd_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      pstcBitSel    Pointer to a stc_sram_ecc_err_inject_bit_t structure.
 *  @param[in]      enNewState    An
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
)
{
    if (MCU_ERRORINJECT_ENABLE == para_NewState_en) 
    {
        SET_REG32_BIT(CM_SRAMC->CACHE_EIBIT0, para_BitSel_pst->McuSramDataBit0_31_u32);
        SET_REG32_BIT(CM_SRAMC->CACHE_EIBIT1, para_BitSel_pst->McuSramDataBit32_63_u32);
        SET_REG32_BIT(CM_SRAMC->CACHE_EIBIT2, para_BitSel_pst->McuSramDataBit64_95_u32);
        SET_REG32_BIT(CM_SRAMC->CACHE_EIBIT3, para_BitSel_pst->McuSramDataBit96_127_u32);
        SET_REG32_BIT(CM_SRAMC->CACHE_EIBIT4, para_BitSel_pst->McuSramDataBit128_136_u32);
    } 
    else 
    {
        CLR_REG32_BIT(CM_SRAMC->CACHE_EIBIT0, para_BitSel_pst->McuSramDataBit0_31_u32);
        CLR_REG32_BIT(CM_SRAMC->CACHE_EIBIT1, para_BitSel_pst->McuSramDataBit32_63_u32);
        CLR_REG32_BIT(CM_SRAMC->CACHE_EIBIT2, para_BitSel_pst->McuSramDataBit64_95_u32);
        CLR_REG32_BIT(CM_SRAMC->CACHE_EIBIT3, para_BitSel_pst->McuSramDataBit96_127_u32);
        CLR_REG32_BIT(CM_SRAMC->CACHE_EIBIT4, para_BitSel_pst->McuSramDataBit128_136_u32);
    }
}
#endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

#if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
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
)
{
    uint32_t f_RetAddr_u32;

    if (MCU_SRAM_ECC_SRAM1 == para_EccSram_u32) 
    {
        f_RetAddr_u32 = READ_REG32(CM_SRAMC->SRAM1_ECCERRADDR);
    } 
    else if (MCU_SRAM_ECC_SRAM2 == para_EccSram_u32) 
    {
        f_RetAddr_u32 = READ_REG32(CM_SRAMC->SRAM2_ECCERRADDR);
    } 
    else if (MCU_SRAM_ECC_SRAM3 == para_EccSram_u32) 
    {
        f_RetAddr_u32 = READ_REG32(CM_SRAMC->SRAM3_ECCERRADDR);
    } 
    else if (MCU_SRAM_ECC_SRAM4 == para_EccSram_u32) 
    {
        f_RetAddr_u32 = READ_REG32(CM_SRAMC->SRAM4_ECCERRADDR);
    } 
    else if (MCU_SRAM_ECC_CACHE_RAM == para_EccSram_u32) 
    {
        f_RetAddr_u32 = READ_REG32(CM_SRAMC->CACHE_ECCERRADDR);
    } 
    else if (MCU_SRAM_ECC_SRAMH == para_EccSram_u32) 
    {
        f_RetAddr_u32 = READ_REG32(CM_SRAMC->SRAMH_ECCERRADDR);
    } 
    else 
    {
        f_RetAddr_u32 = READ_REG32(CM_SRAMC->SRAMB_ECCERRADDR);
    }

    return f_RetAddr_u32;
}
#endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

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
)
{
  Std_ReturnType  f_RamStatus_u8 = E_OK;
  Ram_Ip_RamIndexType  f_RamCounter_u32 = 0U;
  Ram_Ip_RamSizeType  f_RamCounterLimit_u32 = 0U;
  uint64_t  f_McuRamDefaultVal_u64;
  uint32_t f_Offset_u32 = 0U;
  const uint8_t * f_McuMemPtr_u8;
  const uint16_t * f_McuMemPtr_u16;
  const uint32_t * f_McuMemPtr_u32;
  const uint64_t * f_McuMemPtr_u64;
  uint8_t * f_McuWritePtr_u8;
  uint16_t * f_McuWritePtr_u16;
  uint32_t * f_McuWritePtr_u32;
  uint64_t * f_McuWritePtr_u64;

  /* get the Ram default value */
  f_McuRamDefaultVal_u64  = para_RamConfig_ptr->McuRamDefaultValue_u64;
  /* get the Ram base address */
  f_McuWritePtr_u8 = para_RamConfig_ptr->McuRamSectionBase_ptr;
  /* Calculate the limit of f_RamCounter_u32 */
  f_RamCounterLimit_u32 = (Ram_Ip_RamSizeType) ((Ram_Ip_RamSizeType)(para_RamConfig_ptr->McuRamSectionSize_u32) / (para_RamConfig_ptr->McuRamWriteSize_u32));

  /* Initialize write pointers based on base address */
  f_McuWritePtr_u16 = (uint16_t *)f_McuWritePtr_u8;
  f_McuWritePtr_u32 = (uint32_t *)f_McuWritePtr_u8;
  f_McuWritePtr_u64 = (uint64_t *)f_McuWritePtr_u8;

  while((f_RamCounter_u32 < f_RamCounterLimit_u32) && (E_OK == f_RamStatus_u8))
  {
    switch (para_RamConfig_ptr->McuRamWriteSize_u32)
    {
      case (Ram_Ip_RamWriteSizeType) 1U:
        f_McuWritePtr_u8[f_Offset_u32] = (uint8_t)f_McuRamDefaultVal_u64;
        break;
      case (Ram_Ip_RamWriteSizeType) 2U:
        f_McuWritePtr_u16[f_Offset_u32] = (uint16_t)f_McuRamDefaultVal_u64;
        break;
      case (Ram_Ip_RamWriteSizeType) 4U:
        f_McuWritePtr_u32[f_Offset_u32] = (uint32_t)f_McuRamDefaultVal_u64;
        break;
      case (Ram_Ip_RamWriteSizeType) 8U:
        f_McuWritePtr_u64[f_Offset_u32] = f_McuRamDefaultVal_u64;
        break;
      default:
        f_RamStatus_u8 = E_NOT_OK;
        break;
    }

    f_Offset_u32++;
    f_RamCounter_u32++;
  }

  /* Read Ram value to check the Ram initialized correctly */
  if (E_OK == f_RamStatus_u8)
  {
    f_RamCounter_u32 = 0U;
    f_Offset_u32 = 0U;
    /* Initialize read pointers */
    f_McuMemPtr_u8 = (const uint8_t *)f_McuWritePtr_u8;
    f_McuMemPtr_u16 = (const uint16_t *)f_McuWritePtr_u8;
    f_McuMemPtr_u32 = (const uint32_t *)f_McuWritePtr_u8;
    f_McuMemPtr_u64 = (const uint64_t *)f_McuWritePtr_u8;

    while((f_RamCounter_u32 < f_RamCounterLimit_u32) && (E_OK == f_RamStatus_u8))
    {
      switch (para_RamConfig_ptr->McuRamWriteSize_u32)
      {
        case (Ram_Ip_RamWriteSizeType) 1U:
          if (f_McuRamDefaultVal_u64 != f_McuMemPtr_u8[f_Offset_u32])
          {
            f_RamStatus_u8 = E_NOT_OK;
          }
          break;
        case (Ram_Ip_RamWriteSizeType) 2U:
          if (f_McuRamDefaultVal_u64 != f_McuMemPtr_u16[f_Offset_u32])
          {
            f_RamStatus_u8 = E_NOT_OK;
          }
          break;
        case (Ram_Ip_RamWriteSizeType) 4U:
          if (f_McuRamDefaultVal_u64 != f_McuMemPtr_u32[f_Offset_u32])
          {
            f_RamStatus_u8 = E_NOT_OK;
          }
          break;
        case (Ram_Ip_RamWriteSizeType) 8U:
          if (f_McuRamDefaultVal_u64 != f_McuMemPtr_u64[f_Offset_u32])
          {
            f_RamStatus_u8 = E_NOT_OK;
          }
          break;
        default:
          f_RamStatus_u8 = E_NOT_OK;
          break;
      }

      f_Offset_u32++;
      f_RamCounter_u32++;
    }
  }

  return f_RamStatus_u8;
}

/**
 *  @brief          This function clear SRAM flag error and MNI flag
 *                  when a user notification the Function call by ISR routine, for MCU SRAM event is configured
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Ram_Ip_SRAM_INT_ERR_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Ram_Ip_SRAM_INT_ERR(
    void
)
{
  /* Clear SRAM error flag */
  Ram_Ip_ClearStatus(MCU_SRAM_FLAG_ALL);

#ifdef RAM_IP_ERROR_NOTIFICATION
  RAM_IP_ERROR_NOTIFICATION(RAM_IP_SRAM_ERROR_DETECT);
#endif
}

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
