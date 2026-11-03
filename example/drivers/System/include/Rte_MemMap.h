/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Rte_MemMap.h
 *
 * Description: Implementation of RTE MEMMAP High Level layer
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
 *   @file       Rte_MemMap.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 RTE Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *   HEADER FILE VERSION INFORMATION
 **********************************************************************************************************************/
#define RTE_MEMMAP_AR_RELEASE_MAJOR_VERSION        4
#define RTE_MEMMAP_AR_RELEASE_MINOR_VERSION        4
#define RTE_MEMMAP_AR_RELEASE_REVISION_VERSION     0
/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief Symbol used for checking correctness of the includes
 */
#define MEMMAP_ERROR

/**********************************************************************************************************************/
/************************************************************************************* GREENHILLS *********************/
/**********************************************************************************************************************/
#if defined (_GREENHILLS_C_HC32A4XX_)
#ifdef RTE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef RTE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.rte"
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef RTE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CONFIG_DATA_8
  #undef RTE_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.rte"
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_8
  #undef RTE_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CONFIG_DATA_16
  #undef RTE_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.rte"
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_16
  #undef RTE_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CONFIG_DATA_32
  #undef RTE_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.rte"
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_32
  #undef RTE_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef RTE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.rte"
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef RTE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CONST_BOOLEAN
  #undef RTE_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.rte"
#endif

#ifdef RTE_STOP_SEC_CONST_BOOLEAN
  #undef RTE_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CONST_8
  #undef RTE_START_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.rte"
#endif

#ifdef RTE_STOP_SEC_CONST_8
  #undef RTE_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CONST_16
  #undef RTE_START_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.rte"
#endif

#ifdef RTE_STOP_SEC_CONST_16
  #undef RTE_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CONST_32
  #undef RTE_START_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.rte"
#endif

#ifdef RTE_STOP_SEC_CONST_32
  #undef RTE_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CONST_UNSPECIFIED
  #undef RTE_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.rte"
#endif

#ifdef RTE_STOP_SEC_CONST_UNSPECIFIED
  #undef RTE_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CODE
  #undef RTE_START_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma ghs section text=".mcal_text.rte"
#endif

#ifdef RTE_STOP_SEC_CODE
  #undef RTE_STOP_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_CODE_FAST
  #undef RTE_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma ghs section text=".mcal_text.fast.rte"
#endif

#ifdef RTE_STOP_SEC_CODE_FAST
  #undef RTE_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_RAMCODE
  #undef RTE_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma ghs section text=".ramcode.rte"
  #pragma ghs inlineprologue
  #pragma ghs callmode=far
#endif

#ifdef RTE_STOP_SEC_RAMCODE
  #undef RTE_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma ghs section
  #pragma ghs noinlineprologue
  #pragma ghs callmode=default
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef RTE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef RTE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_8
  #undef RTE_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_8
  #undef RTE_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_16
  #undef RTE_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_16
  #undef RTE_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_32
  #undef RTE_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_32
  #undef RTE_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef RTE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef RTE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_INIT_BOOLEAN
  #undef RTE_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.rte" 
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef RTE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_INIT_8
  #undef RTE_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.rte" 
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_8
  #undef RTE_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_INIT_16
  #undef RTE_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.rte" 
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_16
  #undef RTE_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_INIT_32
  #undef RTE_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.rte" 
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_32
  #undef RTE_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef RTE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.rte" 
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef RTE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef RTE_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_8
    #undef RTE_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_8
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_16
    #undef RTE_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_16
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_32
    #undef RTE_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_32
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef RTE_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef RTE_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef RTE_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_8
    #undef RTE_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_8
    #undef RTE_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_16
    #undef RTE_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_16
    #undef RTE_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_32
    #undef RTE_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_32
    #undef RTE_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef RTE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef RTE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef RTE_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef MEMMAP_ERROR
  #pragma ghs section bss=
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef RTE_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif
/**********************************************************************************************************************/
/************************************************************************** IAR ***************************************/
/**********************************************************************************************************************/
#elif defined (_IAR_C_HC32A4XX_)
#ifdef RTE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef RTE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.rte"
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef RTE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif
#ifdef RTE_START_SEC_CONFIG_DATA_8
  #undef RTE_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.rte"
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_8
  #undef RTE_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_CONFIG_DATA_16
  #undef RTE_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.rte"
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_16
  #undef RTE_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_CONFIG_DATA_32
  #undef RTE_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.rte"
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_32
  #undef RTE_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef RTE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.rte"
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef RTE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_CONST_BOOLEAN
  #undef RTE_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.rte"
#endif

#ifdef RTE_STOP_SEC_CONST_BOOLEAN
  #undef RTE_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_CONST_8
  #undef RTE_START_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.rte"
#endif

#ifdef RTE_STOP_SEC_CONST_8
  #undef RTE_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_CONST_16
  #undef RTE_START_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.rte"
#endif

#ifdef RTE_STOP_SEC_CONST_16
  #undef RTE_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_CONST_32
  #undef RTE_START_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.rte"
#endif

#ifdef RTE_STOP_SEC_CONST_32
  #undef RTE_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_CONST_UNSPECIFIED
  #undef RTE_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.rte"
#endif

#ifdef RTE_STOP_SEC_CONST_UNSPECIFIED
  #undef RTE_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_CODE
  #undef RTE_START_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".mcal_text.rte"
#endif

#ifdef RTE_STOP_SEC_CODE
  #undef RTE_STOP_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef RTE_START_SEC_CODE_FAST
  #undef RTE_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".mcal_text.fast.rte"
#endif

#ifdef RTE_STOP_SEC_CODE_FAST
  #undef RTE_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef RTE_START_SEC_RAMCODE
  #undef RTE_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".ramcode.rte"
#endif

#ifdef RTE_STOP_SEC_RAMCODE
  #undef RTE_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef RTE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef RTE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_8
  #undef RTE_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_8
  #undef RTE_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_16
  #undef RTE_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_16
  #undef RTE_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_32
  #undef RTE_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_32
  #undef RTE_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef RTE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef RTE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_INIT_BOOLEAN
  #undef RTE_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef RTE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_INIT_8
  #undef RTE_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_8
  #undef RTE_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_INIT_16
  #undef RTE_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_16
  #undef RTE_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_INIT_32
  #undef RTE_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_32
  #undef RTE_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef RTE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef RTE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef RTE_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_8
    #undef RTE_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_8
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_16
    #undef RTE_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_16
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_32
    #undef RTE_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_32
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef RTE_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef RTE_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef RTE_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_8
    #undef RTE_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_8
    #undef RTE_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_16
    #undef RTE_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_16
    #undef RTE_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_32
    #undef RTE_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_32
    #undef RTE_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef RTE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef RTE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef RTE_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_8
    #undef RTE_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_8
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_16
    #undef RTE_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_16
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_32
    #undef RTE_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_32
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef RTE_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef RTE_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef RTE_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_8
    #undef RTE_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_8
    #undef RTE_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_16
    #undef RTE_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_16
    #undef RTE_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_32
    #undef RTE_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_32
    #undef RTE_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef RTE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.rte"
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef RTE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef RTE_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = 
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef RTE_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

/**********************************************************************************************************************/
/***************************************************************** GCC ************************************************/
/**********************************************************************************************************************/
#elif defined (_GCC_C_HC32A4XX_)
#ifdef RTE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef RTE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef RTE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
#endif
#ifdef RTE_START_SEC_CONFIG_DATA_8
  #undef RTE_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_8
  #undef RTE_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_CONFIG_DATA_16
  #undef RTE_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_16
  #undef RTE_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_CONFIG_DATA_32
  #undef RTE_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_32
  #undef RTE_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef RTE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef RTE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_CONST_BOOLEAN
  #undef RTE_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CONST_BOOLEAN
  #undef RTE_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_CONST_8
  #undef RTE_START_SEC_CONST_8
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CONST_8
  #undef RTE_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_CONST_16
  #undef RTE_START_SEC_CONST_16
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CONST_16
  #undef RTE_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_CONST_32
  #undef RTE_START_SEC_CONST_32
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CONST_32
  #undef RTE_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_CONST_UNSPECIFIED
  #undef RTE_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CONST_UNSPECIFIED
  #undef RTE_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_CODE
  #undef RTE_START_SEC_CODE
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CODE
  #undef RTE_STOP_SEC_CODE
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_CODE_FAST
  #undef RTE_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_CODE_FAST
  #undef RTE_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_RAMCODE
  #undef RTE_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_RAMCODE
  #undef RTE_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef RTE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef RTE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_8
  #undef RTE_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_8
  #undef RTE_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_16
  #undef RTE_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_16
  #undef RTE_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_32
  #undef RTE_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_32
  #undef RTE_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef RTE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef RTE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_INIT_BOOLEAN
  #undef RTE_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef RTE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_INIT_8
  #undef RTE_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_8
  #undef RTE_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_INIT_16
  #undef RTE_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_16
  #undef RTE_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_INIT_32
  #undef RTE_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_32
  #undef RTE_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef RTE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef RTE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef RTE_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_8
    #undef RTE_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_8
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_16
    #undef RTE_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_16
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_32
    #undef RTE_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_32
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef RTE_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef RTE_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef RTE_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef RTE_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_8
    #undef RTE_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_8
    #undef RTE_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_16
    #undef RTE_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_16
    #undef RTE_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_32
    #undef RTE_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_32
    #undef RTE_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef RTE_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef RTE_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef RTE_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef RTE_START_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef MEMMAP_ERROR
#endif

#ifdef RTE_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef RTE_STOP_SEC_VAR_CLEARED_32_NO_CACHEABLE
  #undef MEMMAP_ERROR
#endif
#endif
/**********************************************************************************************************************/
/************************************************************************* Report error *******************************/
/**********************************************************************************************************************/

#ifdef MEMMAP_ERROR
  #error "MemMap.h, No valid section define found."
#endif


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
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
