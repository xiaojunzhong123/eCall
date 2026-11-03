/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip_Pvd.c
 *
 * Description: Implementation of POWER IP layer
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
 *   @file       Power_Ip_Pvd.c
 *   @version    V1.0.3
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
#include "Power_Ip_Pvd.h"
#include "Power_Ip.h"
#include "Power_Ip_Cfg.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define PWC_PVD2_POS                (4U)
#define PWC_PVD_BIT_OFFSET(x)       ((uint8_t)((x) * PWC_PVD2_POS))

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
 *  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION DEFINITIONS
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          This function configures the Power Management Controller for PVD1
 *                  The operating voltages are monitored by a set of on-chip supervisory circuits
 *                  to ensure that this device works within the correct voltage range.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_PVD1Init_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PVDConfig_ptr    Pointer to PVD1 configuration structure
 *
 *  @return         void
 *
 *  @note           This function configures PVD1 (Programmable Voltage Detector 1) including
 *                  filter settings, threshold voltage, enable/disable state, compare output,
 *                  trigger edge, and exception type (IRQ/NMI). This function will be called by Power_Ip_Init().
 *
 *  @pre            None
 *
 */
void Power_Ip_PVD1Init(
    const Power_Ip_PWC_PVD1ConfigType * para_PVDConfig_ptr
)
{
  /* Disable filter function in advance */
  SET_REG8_BIT(CM_PWC->PVDFCR, (PWC_PVDFCR_PVD1NFDIS << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1)));
  MODIFY_REG8(CM_PWC->PVDFCR, (PWC_PVDFCR_PVD1NFDIS | PWC_PVDFCR_PVD1NFCKS) << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1), \
    (para_PVDConfig_ptr->PWC_PVDFilter_u32 | para_PVDConfig_ptr->PWC_PVDFilterClock_u32) << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1));
  /* Config PVD threshold voltage */
  MODIFY_REG8(CM_PWC->PVDLCR, PWC_PVDLCR_PVD1LVL << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1), \
    para_PVDConfig_ptr->PWC_PVDThresholdVoltage_u32 << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1));
  /* Enable PVD */
  MODIFY_REG8(CM_PWC->PVDCR0, PWC_PVDCR0_PVD1EN << PWC_PVD_CH1, para_PVDConfig_ptr->PWC_PVDState_u32 << PWC_PVD_CH1);
  /* Enable compare output */
  MODIFY_REG8(CM_PWC->PVDCR1, PWC_PVDCR1_PVD1CMPOE << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1), \
    para_PVDConfig_ptr->PWC_PVDCompareOutputState_u32 << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1));
  MODIFY_REG8(CM_PWC->PVDICR, PWC_PVDICR_PVD1EDGS <<  PWC_PVD_BIT_OFFSET(PWC_PVD_CH1), \
    para_PVDConfig_ptr->PWC_PVDTriggerEdge_u32 << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1));
  /* Config PVDIRE & PWC_PVDCR1_PVD1IRS while PVDEN & PVDCMPOE enable */
  MODIFY_REG8(CM_PWC->PVDCR1, (PWC_PVDCR1_PVD1IRE | PWC_PVDCR1_PVD1IRS) << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1), \
    (para_PVDConfig_ptr->PWC_PVDExceptionType_u32 & 0xFFU) << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1));
  MODIFY_REG8(CM_PWC->PVDICR, PWC_PVDICR_PVD1NMIS <<  PWC_PVD_BIT_OFFSET(PWC_PVD_CH1), \
    ((para_PVDConfig_ptr->PWC_PVDExceptionType_u32 >> PWC_PVD_EXP_NMI_POS) & 0xFFU) << PWC_PVD_BIT_OFFSET(PWC_PVD_CH1));
}

/**
 *  @brief          This function configures the Power Management Controller for PVD2
 *                  The operating voltages are monitored by a set of on-chip supervisory circuits
 *                  to ensure that this device works within the correct voltage range.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_PVD2Init_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PVDConfig_ptr    Pointer to PVD2 configuration structure
 *
 *  @return         void
 *
 *  @note           This function configures PVD2 (Programmable Voltage Detector 2) including
 *                  filter settings, threshold voltage, enable/disable state, compare output,
 *                  trigger edge, and exception type (IRQ/NMI). This function will be called by Power_Ip_Init().
 *
 *  @pre            None
 *
 */
void Power_Ip_PVD2Init(
    const Power_Ip_PWC_PVD2ConfigType * para_PVDConfig_ptr
)
{
  /* Disable filter function in advance */
  SET_REG8_BIT(CM_PWC->PVDFCR, (PWC_PVDFCR_PVD1NFDIS << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2)));
  MODIFY_REG8(CM_PWC->PVDFCR, (PWC_PVDFCR_PVD1NFDIS | PWC_PVDFCR_PVD1NFCKS) << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2), \
    (para_PVDConfig_ptr->PWC_PVDFilter_u32 | para_PVDConfig_ptr->PWC_PVDFilterClock_u32) << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2));
  /* Config PVD threshold voltage */
  MODIFY_REG8(CM_PWC->PVDLCR, PWC_PVDLCR_PVD1LVL << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2), \
    para_PVDConfig_ptr->PWC_PVDThresholdVoltage_u32 << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2));
  /* Enable PVD */
  MODIFY_REG8(CM_PWC->PVDCR0, PWC_PVDCR0_PVD1EN << PWC_PVD_CH2, para_PVDConfig_ptr->PWC_PVDState_u32 << PWC_PVD_CH2);
  /* Enable compare output */
  MODIFY_REG8(CM_PWC->PVDCR1, PWC_PVDCR1_PVD1CMPOE << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2), \
    para_PVDConfig_ptr->PWC_PVDCompareOutputState_u32 << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2));
  MODIFY_REG8(CM_PWC->PVDICR, PWC_PVDICR_PVD1EDGS <<  PWC_PVD_BIT_OFFSET(PWC_PVD_CH2), \
    para_PVDConfig_ptr->PWC_PVDTriggerEdge_u32 << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2));
  /* Config PVDIRE & PWC_PVDCR1_PVD1IRS while PVDEN & PVDCMPOE enable */
  MODIFY_REG8(CM_PWC->PVDCR1, (PWC_PVDCR1_PVD1IRE | PWC_PVDCR1_PVD1IRS) << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2), \
    (para_PVDConfig_ptr->PWC_PVDExceptionType_u32 & 0xFFU) << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2));
  MODIFY_REG8(CM_PWC->PVDICR, PWC_PVDICR_PVD1NMIS <<  PWC_PVD_BIT_OFFSET(PWC_PVD_CH2), \
    ((para_PVDConfig_ptr->PWC_PVDExceptionType_u32 >> PWC_PVD_EXP_NMI_POS) & 0xFFU) << PWC_PVD_BIT_OFFSET(PWC_PVD_CH2));
}

/**
 *  @brief          This function configures the Power Monitor.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_PowerMonitorInit_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PowerMonitorConfig_ptr    Pointer to Power Monitor configuration structure
 *
 *  @return         void
 *
 *  @note           This function configures VBAT monitoring and power monitoring features
 *                  including source selection (1.8V/2.1V for VBAT, VBAT/2 or internal for power monitor)
 *                  and enable/disable states. This function will be called by Power_Ip_Init().
 *
 *  @pre            None
 *
 */
void Power_Ip_PowerMonitorInit(
    const Power_Ip_PowerMonitorConfigType * para_PowerMonitorConfig_ptr
)
{
  /* Vbat Monitor source */
  if (PWC_VBAT_MONITOR_SRC_1_8_V == para_PowerMonitorConfig_ptr->PWC_VbatMonitorSrc_u32)
  {
    MODIFY_REG(CM_PWC->PWRC4, PWC_PWRC4_VBATREFSEL, (uint32_t)(PWC_VBAT_MONITOR_SRC_1_8_V << PWC_PWRC4_VBATREFSEL_POS));
  }
  else
  {
    MODIFY_REG(CM_PWC->PWRC4, PWC_PWRC4_VBATREFSEL, (uint32_t)(PWC_VBAT_MONITOR_SRC_2_1_V << PWC_PWRC4_VBATREFSEL_POS));
  }
  
  /* Vbat Monitor enable */
  if (PWC_VBAT_MONITOR_ENABLE == para_PowerMonitorConfig_ptr->PWC_VbatMonitorCmd_u32)
  {
    MODIFY_REG(CM_PWC->PWRC4, PWC_PWRC4_VBATME, (uint32_t)(PWC_VBAT_MONITOR_ENABLE << PWC_PWRC4_VBATME_POS));
  }
  else
  {
    MODIFY_REG(CM_PWC->PWRC4, PWC_PWRC4_VBATME, (uint32_t)(PWC_VBAT_MONITOR_DISABLE << PWC_PWRC4_VBATME_POS));
  }

  /* Power Monitor source */
  if (PWC_POWER_MONITOR_SRC_VBAT_DIV2 == para_PowerMonitorConfig_ptr->PWC_PowerMonitorSrc_u32)
  {
    MODIFY_REG(CM_PWC->VBATCR, PWC_VBATCR_VBATDIVMONE, (uint32_t)(PWC_VBAT_VBATDIV_ENABLE << PWC_VBATCR_VBATDIVMONE_POS));
    MODIFY_REG(CM_PWC->PWRC4, PWC_PWRC4_ADBUFS, (uint32_t)(PWC_POWER_MONITOR_SRC_VBAT_DIV2 << PWC_PWRC4_ADBUFS_POS));
  }
  else
  {
    MODIFY_REG(CM_PWC->PWRC4, PWC_PWRC4_ADBUFS, (uint32_t)(PWC_POWER_MONITOR_SRC_INTERNAL << PWC_PWRC4_ADBUFS_POS));
    MODIFY_REG(CM_PWC->VBATCR, PWC_VBATCR_VBATDIVMONE, (uint32_t)(PWC_VBAT_VBATDIV_DISABLE << PWC_VBATCR_VBATDIVMONE_POS));
  }
  
  /* Power Monitor enable */
  if (PWC_POWER_MONITOR_ENABLE == para_PowerMonitorConfig_ptr->PWC_PowerMonitorCmd_u32)
  {
    MODIFY_REG(CM_PWC->PWRC4, PWC_PWRC4_ADBUFE, (uint32_t)(PWC_POWER_MONITOR_ENABLE << PWC_PWRC4_ADBUFE_POS));
  }
  else
  {
    MODIFY_REG(CM_PWC->PWRC4, PWC_PWRC4_ADBUFE, (uint32_t)(PWC_POWER_MONITOR_DISABLE << PWC_PWRC4_ADBUFE_POS));
  }
}

/**
 *  @brief          This function handles the voltage error detection.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_PVD_VoltageError_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           This function is called when PVD1 or PVD2 detects a voltage error.
 *                  It clears the corresponding detection flags and optionally triggers
 *                  a user notification if POWER_IP_PWC_PVD_NOTIFICATION is defined.
 *
 *  @pre            None
 *
 */
void Power_Ip_PVD_VoltageError(
    void
)
{
  if (0x00U != READ_REG8_BIT(CM_PWC->PVDDSR, PWC_PVD1_FLAG_DETECT))
  {
    /* clear IRQ flags */
    CLR_REG8_BIT(CM_PWC->PVDDSR, PWC_PVDDSR_PVD1DETFLG);
  }
  if (0x00U != READ_REG8_BIT(CM_PWC->PVDDSR, PWC_PVD2_FLAG_DETECT))
  {
    /* clear IRQ flags */
    CLR_REG8_BIT(CM_PWC->PVDDSR, PWC_PVDDSR_PVD2DETFLG);
  }
#ifdef POWER_IP_PWC_PVD_NOTIFICATION
  POWER_IP_PWC_PVD_NOTIFICATION(POWER_IP_ISR_POWER_PVD_DETECT);
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
