/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Dio_MemMap.h
 *
 * Description: Implementation of DIO MEMMAP High Level layer
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
 *   @file       Dio_MemMap.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 DIO Driver
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
#define DIO_MEMMAP_AR_RELEASE_MAJOR_VERSION        4
#define DIO_MEMMAP_AR_RELEASE_MINOR_VERSION        4
#define DIO_MEMMAP_AR_RELEASE_REVISION_VERSION     0
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
#ifdef DIO_START_SEC_CONFIG_DATA_BOOLEAN
  #undef DIO_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.dio"
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef DIO_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CONFIG_DATA_8
  #undef DIO_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.dio"
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_8
  #undef DIO_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CONFIG_DATA_16
  #undef DIO_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.dio"
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_16
  #undef DIO_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CONFIG_DATA_32
  #undef DIO_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.dio"
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_32
  #undef DIO_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.dio"
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CONST_BOOLEAN
  #undef DIO_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.dio"
#endif

#ifdef DIO_STOP_SEC_CONST_BOOLEAN
  #undef DIO_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CONST_8
  #undef DIO_START_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.dio"
#endif

#ifdef DIO_STOP_SEC_CONST_8
  #undef DIO_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CONST_16
  #undef DIO_START_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.dio"
#endif

#ifdef DIO_STOP_SEC_CONST_16
  #undef DIO_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CONST_32
  #undef DIO_START_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.dio"
#endif

#ifdef DIO_STOP_SEC_CONST_32
  #undef DIO_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CONST_UNSPECIFIED
  #undef DIO_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.dio"
#endif

#ifdef DIO_STOP_SEC_CONST_UNSPECIFIED
  #undef DIO_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CODE
  #undef DIO_START_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma ghs section text=".mcal_text.dio"
#endif

#ifdef DIO_STOP_SEC_CODE
  #undef DIO_STOP_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_CODE_FAST
  #undef DIO_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma ghs section text=".mcal_text.fast.dio"
#endif

#ifdef DIO_STOP_SEC_CODE_FAST
  #undef DIO_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_RAMCODE
  #undef DIO_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma ghs section text=".ramcode.dio"
  #pragma ghs inlineprologue
  #pragma ghs callmode=far
#endif

#ifdef DIO_STOP_SEC_RAMCODE
  #undef DIO_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma ghs section
  #pragma ghs noinlineprologue
  #pragma ghs callmode=default
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_BOOLEAN
  #undef DIO_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef DIO_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_8
  #undef DIO_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_8
  #undef DIO_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_16
  #undef DIO_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_16
  #undef DIO_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_32
  #undef DIO_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_32
  #undef DIO_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef DIO_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef DIO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_INIT_BOOLEAN
  #undef DIO_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_BOOLEAN
  #undef DIO_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_INIT_8
  #undef DIO_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_8
  #undef DIO_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_INIT_16
  #undef DIO_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_16
  #undef DIO_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_INIT_32
  #undef DIO_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_32
  #undef DIO_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_INIT_UNSPECIFIED
  #undef DIO_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef DIO_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef DIO_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_8
    #undef DIO_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_8
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_16
    #undef DIO_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_16
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_32
    #undef DIO_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_32
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef DIO_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef DIO_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef DIO_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_8
    #undef DIO_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_8
    #undef DIO_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_16
    #undef DIO_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_16
    #undef DIO_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_32
    #undef DIO_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_32
    #undef DIO_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef DIO_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef DIO_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

/**********************************************************************************************************************/
/************************************************************************** IAR ***************************************/
/**********************************************************************************************************************/
#elif defined (_IAR_C_HC32A4XX_)
#ifdef DIO_START_SEC_CONFIG_DATA_BOOLEAN
  #undef DIO_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.dio"
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef DIO_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif
#ifdef DIO_START_SEC_CONFIG_DATA_8
  #undef DIO_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.dio"
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_8
  #undef DIO_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_CONFIG_DATA_16
  #undef DIO_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.dio"
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_16
  #undef DIO_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_CONFIG_DATA_32
  #undef DIO_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.dio"
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_32
  #undef DIO_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.dio"
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_CONST_BOOLEAN
  #undef DIO_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.dio"
#endif

#ifdef DIO_STOP_SEC_CONST_BOOLEAN
  #undef DIO_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_CONST_8
  #undef DIO_START_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.dio"
#endif

#ifdef DIO_STOP_SEC_CONST_8
  #undef DIO_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_CONST_16
  #undef DIO_START_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.dio"
#endif

#ifdef DIO_STOP_SEC_CONST_16
  #undef DIO_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_CONST_32
  #undef DIO_START_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.dio"
#endif

#ifdef DIO_STOP_SEC_CONST_32
  #undef DIO_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_CONST_UNSPECIFIED
  #undef DIO_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.dio"
#endif

#ifdef DIO_STOP_SEC_CONST_UNSPECIFIED
  #undef DIO_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_CODE
  #undef DIO_START_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".mcal_text.dio"
#endif

#ifdef DIO_STOP_SEC_CODE
  #undef DIO_STOP_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef DIO_START_SEC_CODE_FAST
  #undef DIO_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".mcal_text.fast.dio"
#endif

#ifdef DIO_STOP_SEC_CODE_FAST
  #undef DIO_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef DIO_START_SEC_RAMCODE
  #undef DIO_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".ramcode.dio"
#endif

#ifdef DIO_STOP_SEC_RAMCODE
  #undef DIO_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_BOOLEAN
  #undef DIO_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef DIO_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_8
  #undef DIO_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_8
  #undef DIO_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_16
  #undef DIO_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_16
  #undef DIO_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_32
  #undef DIO_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_32
  #undef DIO_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef DIO_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef DIO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_INIT_BOOLEAN
  #undef DIO_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_BOOLEAN
  #undef DIO_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_INIT_8
  #undef DIO_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_8
  #undef DIO_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_INIT_16
  #undef DIO_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_16
  #undef DIO_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_INIT_32
  #undef DIO_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_32
  #undef DIO_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_INIT_UNSPECIFIED
  #undef DIO_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef DIO_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef DIO_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_8
    #undef DIO_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_8
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_16
    #undef DIO_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_16
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_32
    #undef DIO_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_32
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef DIO_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef DIO_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef DIO_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_8
    #undef DIO_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_8
    #undef DIO_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_16
    #undef DIO_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_16
    #undef DIO_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_32
    #undef DIO_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_32
    #undef DIO_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef DIO_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.dio"
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef DIO_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

/**********************************************************************************************************************/
/***************************************************************** GCC ************************************************/
/**********************************************************************************************************************/
#elif defined (_GCC_C_HC32A4XX_)
#ifdef DIO_START_SEC_CONFIG_DATA_BOOLEAN
  #undef DIO_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef DIO_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
#endif
#ifdef DIO_START_SEC_CONFIG_DATA_8
  #undef DIO_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_8
  #undef DIO_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_CONFIG_DATA_16
  #undef DIO_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_16
  #undef DIO_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_CONFIG_DATA_32
  #undef DIO_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_32
  #undef DIO_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_CONST_BOOLEAN
  #undef DIO_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CONST_BOOLEAN
  #undef DIO_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_CONST_8
  #undef DIO_START_SEC_CONST_8
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CONST_8
  #undef DIO_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_CONST_16
  #undef DIO_START_SEC_CONST_16
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CONST_16
  #undef DIO_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_CONST_32
  #undef DIO_START_SEC_CONST_32
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CONST_32
  #undef DIO_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_CONST_UNSPECIFIED
  #undef DIO_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CONST_UNSPECIFIED
  #undef DIO_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_CODE
  #undef DIO_START_SEC_CODE
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CODE
  #undef DIO_STOP_SEC_CODE
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_CODE_FAST
  #undef DIO_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_CODE_FAST
  #undef DIO_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_RAMCODE
  #undef DIO_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_RAMCODE
  #undef DIO_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_BOOLEAN
  #undef DIO_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef DIO_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_8
  #undef DIO_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_8
  #undef DIO_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_16
  #undef DIO_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_16
  #undef DIO_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_32
  #undef DIO_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_32
  #undef DIO_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef DIO_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef DIO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_INIT_BOOLEAN
  #undef DIO_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_BOOLEAN
  #undef DIO_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_INIT_8
  #undef DIO_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_8
  #undef DIO_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_INIT_16
  #undef DIO_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_16
  #undef DIO_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_INIT_32
  #undef DIO_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_32
  #undef DIO_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_INIT_UNSPECIFIED
  #undef DIO_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef DIO_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef DIO_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_8
    #undef DIO_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_8
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_16
    #undef DIO_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_16
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_32
    #undef DIO_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_32
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef DIO_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef DIO_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef DIO_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef DIO_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_8
    #undef DIO_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_8
    #undef DIO_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_16
    #undef DIO_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_16
    #undef DIO_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_32
    #undef DIO_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_32
    #undef DIO_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef DIO_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef DIO_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef DIO_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
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
