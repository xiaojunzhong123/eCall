/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Spi_MemMap.h
 *
 * Description: Implementation of SPI MEMMAP High Level layer
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
 *   @file       Spi_MemMap.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 SPI Driver
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
#define SPI_MEMMAP_AR_RELEASE_MAJOR_VERSION        4
#define SPI_MEMMAP_AR_RELEASE_MINOR_VERSION        4
#define SPI_MEMMAP_AR_RELEASE_REVISION_VERSION     0
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
#ifdef SPI_START_SEC_CONFIG_DATA_BOOLEAN
  #undef SPI_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.spi"
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef SPI_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CONFIG_DATA_8
  #undef SPI_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.spi"
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_8
  #undef SPI_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CONFIG_DATA_16
  #undef SPI_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.spi"
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_16
  #undef SPI_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CONFIG_DATA_32
  #undef SPI_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.spi"
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_32
  #undef SPI_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef SPI_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const_cfg.spi"
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef SPI_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CONST_BOOLEAN
  #undef SPI_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.spi"
#endif

#ifdef SPI_STOP_SEC_CONST_BOOLEAN
  #undef SPI_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CONST_8
  #undef SPI_START_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.spi"
#endif

#ifdef SPI_STOP_SEC_CONST_8
  #undef SPI_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CONST_16
  #undef SPI_START_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.spi"
#endif

#ifdef SPI_STOP_SEC_CONST_16
  #undef SPI_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CONST_32
  #undef SPI_START_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.spi"
#endif

#ifdef SPI_STOP_SEC_CONST_32
  #undef SPI_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CONST_UNSPECIFIED
  #undef SPI_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section rodata=".mcal_const.spi"
#endif

#ifdef SPI_STOP_SEC_CONST_UNSPECIFIED
  #undef SPI_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CODE
  #undef SPI_START_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma ghs section text=".mcal_text.spi"
#endif

#ifdef SPI_STOP_SEC_CODE
  #undef SPI_STOP_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_CODE_FAST
  #undef SPI_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma ghs section text=".mcal_text.fast.spi"
#endif

#ifdef SPI_STOP_SEC_CODE_FAST
  #undef SPI_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_RAMCODE
  #undef SPI_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma ghs section text=".ramcode.spi"
  #pragma ghs inlineprologue
  #pragma ghs callmode=far
#endif

#ifdef SPI_STOP_SEC_RAMCODE
  #undef SPI_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma ghs section
  #pragma ghs noinlineprologue
  #pragma ghs callmode=default
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_BOOLEAN
  #undef SPI_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef SPI_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_8
  #undef SPI_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_8
  #undef SPI_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_16
  #undef SPI_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_16
  #undef SPI_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_32
  #undef SPI_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_32
  #undef SPI_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef SPI_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section bss=".mcal_bss.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef SPI_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_INIT_BOOLEAN
  #undef SPI_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.spi" 
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_BOOLEAN
  #undef SPI_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_INIT_8
  #undef SPI_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.spi" 
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_8
  #undef SPI_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_INIT_16
  #undef SPI_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.spi" 
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_16
  #undef SPI_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_INIT_32
  #undef SPI_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.spi" 
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_32
  #undef SPI_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_INIT_UNSPECIFIED
  #undef SPI_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section data=".mcal_data.spi" 
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef SPI_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef SPI_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_8
    #undef SPI_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_8
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_16
    #undef SPI_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_16
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_32
    #undef SPI_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_32
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef SPI_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section bss=".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef SPI_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef SPI_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_8
    #undef SPI_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_8
    #undef SPI_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_16
    #undef SPI_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_16
    #undef SPI_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_32
    #undef SPI_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_32
    #undef SPI_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef SPI_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section data=".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef SPI_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma ghs section
#endif

/**********************************************************************************************************************/
/************************************************************************** IAR ***************************************/
/**********************************************************************************************************************/
#elif defined (_IAR_C_HC32A4XX_)
#ifdef SPI_START_SEC_CONFIG_DATA_BOOLEAN
  #undef SPI_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.spi"
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef SPI_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif
#ifdef SPI_START_SEC_CONFIG_DATA_8
  #undef SPI_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.spi"
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_8
  #undef SPI_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_CONFIG_DATA_16
  #undef SPI_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.spi"
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_16
  #undef SPI_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_CONFIG_DATA_32
  #undef SPI_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.spi"
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_32
  #undef SPI_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef SPI_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const_cfg.spi"
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef SPI_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_CONST_BOOLEAN
  #undef SPI_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.spi"
#endif

#ifdef SPI_STOP_SEC_CONST_BOOLEAN
  #undef SPI_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_CONST_8
  #undef SPI_START_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.spi"
#endif

#ifdef SPI_STOP_SEC_CONST_8
  #undef SPI_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_CONST_16
  #undef SPI_START_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.spi"
#endif

#ifdef SPI_STOP_SEC_CONST_16
  #undef SPI_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_CONST_32
  #undef SPI_START_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.spi"
#endif

#ifdef SPI_STOP_SEC_CONST_32
  #undef SPI_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_CONST_UNSPECIFIED
  #undef SPI_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_const.spi"
#endif

#ifdef SPI_STOP_SEC_CONST_UNSPECIFIED
  #undef SPI_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_CODE
  #undef SPI_START_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".mcal_text.spi"
#endif

#ifdef SPI_STOP_SEC_CODE
  #undef SPI_STOP_SEC_CODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef SPI_START_SEC_CODE_FAST
  #undef SPI_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".mcal_text.fast.spi"
#endif

#ifdef SPI_STOP_SEC_CODE_FAST
  #undef SPI_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef SPI_START_SEC_RAMCODE
  #undef SPI_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes = @ ".ramcode.spi"
#endif

#ifdef SPI_STOP_SEC_RAMCODE
  #undef SPI_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
  #pragma default_function_attributes =
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_BOOLEAN
  #undef SPI_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef SPI_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_8
  #undef SPI_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_8
  #undef SPI_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_16
  #undef SPI_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_16
  #undef SPI_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_32
  #undef SPI_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_32
  #undef SPI_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef SPI_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_bss.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef SPI_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_INIT_BOOLEAN
  #undef SPI_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_BOOLEAN
  #undef SPI_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_INIT_8
  #undef SPI_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_8
  #undef SPI_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_INIT_16
  #undef SPI_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_16
  #undef SPI_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_INIT_32
  #undef SPI_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_32
  #undef SPI_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_INIT_UNSPECIFIED
  #undef SPI_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes = @ ".mcal_data.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef SPI_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
  #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef SPI_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_8
    #undef SPI_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_8
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_16
    #undef SPI_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_16
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_32
    #undef SPI_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_32
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef SPI_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef SPI_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef SPI_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_8
    #undef SPI_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_8
    #undef SPI_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_16
    #undef SPI_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_16
    #undef SPI_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_32
    #undef SPI_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_32
    #undef SPI_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef SPI_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef SPI_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef SPI_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_8
    #undef SPI_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_8
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_16
    #undef SPI_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_16
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_32
    #undef SPI_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_32
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef SPI_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_bss.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef SPI_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef SPI_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_8
    #undef SPI_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_8
    #undef SPI_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_16
    #undef SPI_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_16
    #undef SPI_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_32
    #undef SPI_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_32
    #undef SPI_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef SPI_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes = @ ".mcal_data.fast.spi"
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef SPI_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #pragma default_variable_attributes =
#endif

/**********************************************************************************************************************/
/***************************************************************** GCC ************************************************/
/**********************************************************************************************************************/
#elif defined (_GCC_C_HC32A4XX_)
#ifdef SPI_START_SEC_CONFIG_DATA_BOOLEAN
  #undef SPI_START_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef SPI_STOP_SEC_CONFIG_DATA_BOOLEAN
  #undef MEMMAP_ERROR
#endif
#ifdef SPI_START_SEC_CONFIG_DATA_8
  #undef SPI_START_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_8
  #undef SPI_STOP_SEC_CONFIG_DATA_8
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_CONFIG_DATA_16
  #undef SPI_START_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_16
  #undef SPI_STOP_SEC_CONFIG_DATA_16
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_CONFIG_DATA_32
  #undef SPI_START_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_32
  #undef SPI_STOP_SEC_CONFIG_DATA_32
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef SPI_START_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef SPI_STOP_SEC_CONFIG_DATA_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_CONST_BOOLEAN
  #undef SPI_START_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CONST_BOOLEAN
  #undef SPI_STOP_SEC_CONST_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_CONST_8
  #undef SPI_START_SEC_CONST_8
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CONST_8
  #undef SPI_STOP_SEC_CONST_8
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_CONST_16
  #undef SPI_START_SEC_CONST_16
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CONST_16
  #undef SPI_STOP_SEC_CONST_16
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_CONST_32
  #undef SPI_START_SEC_CONST_32
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CONST_32
  #undef SPI_STOP_SEC_CONST_32
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_CONST_UNSPECIFIED
  #undef SPI_START_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CONST_UNSPECIFIED
  #undef SPI_STOP_SEC_CONST_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_CODE
  #undef SPI_START_SEC_CODE
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CODE
  #undef SPI_STOP_SEC_CODE
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_CODE_FAST
  #undef SPI_START_SEC_CODE_FAST
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_CODE_FAST
  #undef SPI_STOP_SEC_CODE_FAST
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_RAMCODE
  #undef SPI_START_SEC_RAMCODE
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_RAMCODE
  #undef SPI_STOP_SEC_RAMCODE
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_BOOLEAN
  #undef SPI_START_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef SPI_STOP_SEC_VAR_CLEARED_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_8
  #undef SPI_START_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_8
  #undef SPI_STOP_SEC_VAR_CLEARED_8
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_16
  #undef SPI_START_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_16
  #undef SPI_STOP_SEC_VAR_CLEARED_16
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_32
  #undef SPI_START_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_32
  #undef SPI_STOP_SEC_VAR_CLEARED_32
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef SPI_START_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef SPI_STOP_SEC_VAR_CLEARED_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_INIT_BOOLEAN
  #undef SPI_START_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_BOOLEAN
  #undef SPI_STOP_SEC_VAR_INIT_BOOLEAN
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_INIT_8
  #undef SPI_START_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_8
  #undef SPI_STOP_SEC_VAR_INIT_8
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_INIT_16
  #undef SPI_START_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_16
  #undef SPI_STOP_SEC_VAR_INIT_16
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_INIT_32
  #undef SPI_START_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_32
  #undef SPI_STOP_SEC_VAR_INIT_32
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_INIT_UNSPECIFIED
  #undef SPI_START_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef SPI_STOP_SEC_VAR_INIT_UNSPECIFIED
  #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef SPI_START_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_8
    #undef SPI_START_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_8
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_8
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_16
    #undef SPI_START_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_16
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_16
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_32
    #undef SPI_START_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_32
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_32
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef SPI_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef SPI_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef SPI_START_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef SPI_STOP_SEC_VAR_FAST_INIT_BOOLEAN
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_8
    #undef SPI_START_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_8
    #undef SPI_STOP_SEC_VAR_FAST_INIT_8
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_16
    #undef SPI_START_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_16
    #undef SPI_STOP_SEC_VAR_FAST_INIT_16
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_32
    #undef SPI_START_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_32
    #undef SPI_STOP_SEC_VAR_FAST_INIT_32
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef SPI_START_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
#endif

#ifdef SPI_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
    #undef SPI_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
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
