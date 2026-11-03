/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Fee_MemMap.h
 *
 * Description: Implementation of FEE MEMMAP High Level layer
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
 *   @file       Fee_MemMap.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 FEE Driver
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
#define FEE_MEMMAP_AR_RELEASE_MAJOR_VERSION        4
#define FEE_MEMMAP_AR_RELEASE_MINOR_VERSION        4
#define FEE_MEMMAP_AR_RELEASE_REVISION_VERSION     0
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
#ifdef FEE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef FEE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
    #pragma ghs section rodata=".mcal_const_cfg.fee"
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef FEE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CONFIG_DATA_8
  #undef FEE_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
    #pragma ghs section rodata=".mcal_const_cfg.fee"
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_8
  #undef FEE_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CONFIG_DATA_16
  #undef FEE_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
    #pragma ghs section rodata=".mcal_const_cfg.fee"
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_16
  #undef FEE_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CONFIG_DATA_32
  #undef FEE_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
    #pragma ghs section rodata=".mcal_const_cfg.fee"
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_32
  #undef FEE_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
    #pragma ghs section rodata=".mcal_const_cfg.fee"
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CONST_BOOLEAN
  #undef FEE_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
    #pragma ghs section rodata=".mcal_const.fee"
#endif

#ifdef FEE_STOP_SEC_CONST_BOOLEAN
  #undef FEE_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CONST_8
  #undef FEE_START_SEC_CONST_8
  #undef MEMMAP_ERROR
    #pragma ghs section rodata=".mcal_const.fee"
#endif

#ifdef FEE_STOP_SEC_CONST_8
  #undef FEE_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CONST_16
  #undef FEE_START_SEC_CONST_16
  #undef MEMMAP_ERROR
    #pragma ghs section rodata=".mcal_const.fee"
#endif

#ifdef FEE_STOP_SEC_CONST_16
  #undef FEE_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CONST_32
  #undef FEE_START_SEC_CONST_32
  #undef MEMMAP_ERROR
    #pragma ghs section rodata=".mcal_const.fee"
#endif

#ifdef FEE_STOP_SEC_CONST_32
  #undef FEE_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CONST_UNSPECIFIED
  #undef FEE_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
    #pragma ghs section rodata=".mcal_const.fee"
#endif

#ifdef FEE_STOP_SEC_CONST_UNSPECIFIED
  #undef FEE_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CODE
  #undef FEE_START_SEC_CODE
  #undef MEMMAP_ERROR
    #pragma ghs section text=".mcal_text.fee"
#endif

#ifdef FEE_STOP_SEC_CODE
  #undef FEE_STOP_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CODE_FAST
  #undef FEE_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
    #pragma ghs section text=".mcal_text.fast.fee"
#endif

#ifdef FEE_STOP_SEC_CODE_FAST
  #undef FEE_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CODE_SLOW
  #undef FEE_START_SEC_CODE_SLOW
  #undef MEMMAP_ERROR
  #pragma ghs section text=".mcal_text.fee"
#endif

#ifdef FEE_STOP_SEC_CODE_SLOW
  #undef FEE_STOP_SEC_CODE_SLOW
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_CALLOUT_CODE
  #undef FEE_START_SEC_CALLOUT_CODE
  #undef MEMMAP_ERROR
  #pragma ghs section text=".mcal_text.fee"
#endif

#ifdef FEE_STOP_SEC_CALLOUT_CODE
  #undef FEE_STOP_SEC_CALLOUT_CODE
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_RAMCODE
  #undef FEE_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
    #pragma ghs section text=".ramcode.fee"
  #pragma ghs inlineprologue
  #pragma ghs callmode=far
#endif

#ifdef FEE_STOP_SEC_RAMCODE
  #undef FEE_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma ghs section
  #pragma ghs noinlineprologue
  #pragma ghs callmode=default
#endif

#ifdef FEE_START_SEC_CODE_AC
  #undef FEE_START_SEC_CODE_AC
  #undef MEMMAP_ERROR
  #pragma ghs section text=".access_code_rom"
  #pragma ghs inlineprologue
#endif

#ifdef FEE_STOP_SEC_CODE_AC
  #undef FEE_STOP_SEC_CODE_AC
  #undef MEMMAP_ERROR
  #pragma ghs section
  #pragma ghs noinlineprologue
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef FEE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef FEE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_8
  #undef FEE_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_8
  #undef FEE_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_16
  #undef FEE_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_16
  #undef FEE_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_32
  #undef FEE_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_32
  #undef FEE_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_VAR_INIT_BOOLEAN
  #undef FEE_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef FEE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_VAR_INIT_8
  #undef FEE_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_8
  #undef FEE_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_VAR_INIT_16
  #undef FEE_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_16
  #undef FEE_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_VAR_INIT_32
  #undef FEE_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_32
  #undef FEE_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef FEE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef FEE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

/**********************************************************************************************************************/
/************************************************************************** IAR ***************************************/
/**********************************************************************************************************************/
#elif defined (_IAR_C_HC32A4XX_)
#ifdef FEE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef FEE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_const_cfg.fee"
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef FEE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif
#ifdef FEE_START_SEC_CONFIG_DATA_8
  #undef FEE_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_const_cfg.fee"
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_8
  #undef FEE_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_CONFIG_DATA_16
  #undef FEE_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_const_cfg.fee"
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_16
  #undef FEE_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_CONFIG_DATA_32
  #undef FEE_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_const_cfg.fee"
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_32
  #undef FEE_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_const_cfg.fee"
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_CONST_BOOLEAN
  #undef FEE_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_const.fee"
#endif

#ifdef FEE_STOP_SEC_CONST_BOOLEAN
  #undef FEE_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_CONST_8
  #undef FEE_START_SEC_CONST_8
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_const.fee"
#endif

#ifdef FEE_STOP_SEC_CONST_8
  #undef FEE_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_CONST_16
  #undef FEE_START_SEC_CONST_16
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_const.fee"
#endif

#ifdef FEE_STOP_SEC_CONST_16
  #undef FEE_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_CONST_32
  #undef FEE_START_SEC_CONST_32
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_const.fee"
#endif

#ifdef FEE_STOP_SEC_CONST_32
  #undef FEE_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_CONST_UNSPECIFIED
  #undef FEE_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_const.fee"
#endif

#ifdef FEE_STOP_SEC_CONST_UNSPECIFIED
  #undef FEE_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_CODE
  #undef FEE_START_SEC_CODE
  #undef MEMMAP_ERROR
    #pragma default_function_attributes = @ ".mcal_text.fee"
#endif

#ifdef FEE_STOP_SEC_CODE
  #undef FEE_STOP_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef FEE_START_SEC_CODE_FAST
  #undef FEE_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
    #pragma default_function_attributes = @ ".mcal_text.fast.fee"
#endif

#ifdef FEE_STOP_SEC_CODE_FAST
  #undef FEE_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef FEE_START_SEC_CODE_SLOW
  #undef FEE_START_SEC_CODE_SLOW
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".mcal_text"
#endif

#ifdef FEE_STOP_SEC_CODE_SLOW
  #undef FEE_STOP_SEC_CODE_SLOW
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef FEE_START_SEC_CALLOUT_CODE
  #undef FEE_START_SEC_CALLOUT_CODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".mcal_text"
#endif

#ifdef FEE_STOP_SEC_CALLOUT_CODE
  #undef FEE_STOP_SEC_CALLOUT_CODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef FEE_START_SEC_RAMCODE
  #undef FEE_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
    #pragma default_function_attributes = @ ".ramcode.fee"
#endif

#ifdef FEE_STOP_SEC_RAMCODE
  #undef FEE_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef FEE_START_SEC_CODE_AC
  #undef FEE_START_SEC_CODE_AC
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".access_code_rom"
#endif

#ifdef FEE_STOP_SEC_CODE_AC
  #undef FEE_STOP_SEC_CODE_AC
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef FEE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef FEE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_8
  #undef FEE_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_8
  #undef FEE_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_16
  #undef FEE_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_16
  #undef FEE_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_32
  #undef FEE_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_32
  #undef FEE_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_VAR_INIT_BOOLEAN
  #undef FEE_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef FEE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_VAR_INIT_8
  #undef FEE_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_8
  #undef FEE_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_VAR_INIT_16
  #undef FEE_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_16
  #undef FEE_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_VAR_INIT_32
  #undef FEE_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_32
  #undef FEE_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef FEE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef FEE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fee"
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

/**********************************************************************************************************************/
/***************************************************************** GCC ************************************************/
/**********************************************************************************************************************/
#elif defined (_GCC_C_HC32A4XX_)
#ifdef FEE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef FEE_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef FEE_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
#endif
#ifdef FEE_START_SEC_CONFIG_DATA_8
  #undef FEE_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_8
  #undef FEE_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CONFIG_DATA_16
  #undef FEE_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_16
  #undef FEE_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CONFIG_DATA_32
  #undef FEE_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_32
  #undef FEE_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CONST_BOOLEAN
  #undef FEE_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CONST_BOOLEAN
  #undef FEE_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CONST_8
  #undef FEE_START_SEC_CONST_8
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CONST_8
  #undef FEE_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CONST_16
  #undef FEE_START_SEC_CONST_16
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CONST_16
  #undef FEE_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CONST_32
  #undef FEE_START_SEC_CONST_32
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CONST_32
  #undef FEE_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CONST_UNSPECIFIED
  #undef FEE_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CONST_UNSPECIFIED
  #undef FEE_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CODE
  #undef FEE_START_SEC_CODE
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CODE
  #undef FEE_STOP_SEC_CODE
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CODE_FAST
  #undef FEE_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CODE_FAST
  #undef FEE_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CODE_SLOW
  #undef FEE_START_SEC_CODE_SLOW
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CODE_SLOW
  #undef FEE_STOP_SEC_CODE_SLOW
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CALLOUT_CODE
  #undef FEE_START_SEC_CALLOUT_CODE
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CALLOUT_CODE
  #undef FEE_STOP_SEC_CALLOUT_CODE
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_RAMCODE
  #undef FEE_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_RAMCODE
  #undef FEE_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_CODE_AC
  #undef FEE_START_SEC_CODE_AC
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_CODE_AC
  #undef FEE_STOP_SEC_CODE_AC
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef FEE_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef FEE_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_8
  #undef FEE_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_8
  #undef FEE_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_16
  #undef FEE_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_16
  #undef FEE_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_32
  #undef FEE_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_32
  #undef FEE_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_VAR_INIT_BOOLEAN
  #undef FEE_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef FEE_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_VAR_INIT_8
  #undef FEE_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_8
  #undef FEE_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_VAR_INIT_16
  #undef FEE_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_16
  #undef FEE_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_VAR_INIT_32
  #undef FEE_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_32
  #undef FEE_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef FEE_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
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
