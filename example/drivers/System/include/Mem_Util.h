/***********************************************************************************************************************
 * Project Name: XC2XX
 * File Name: Mem_Util.h
 *
 * Description: Implementation of SYSTEM
 *              Compiler:                IAR
 *
 *
 * (c) Copyright 2024 XIAOHUA SEMICONDUCTOR CO., LTD. All rights reserved.
 *
 * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT (the "License"). By downloading, installing, or
 * using the Software, you indicate that you accept the terms of the License, and you acknowledge that you have the
 * authority, for yourself or on behalf of your company, to bind your company to these terms. If you do not agree to all
 * of the terms of this License, you should not download or install the Software.
 *
 **********************************************************************************************************************/

/**
 *   @file       Mem_Util.h
 *   @version    1.0.0
 *   @brief      Mem_Util IP Layer Source File
 *   @addtogroup system_driver
 *   @{
 */

#ifndef MEM_UTIL_H
#define MEM_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include <PlatformTypes.h>
#include <Compiler.h>

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define memcpy_s(dst, dstmax, src, len)    Secure_Memcpy((dst), (dstmax), (src),   (len))         
#define memset_s(dst, dstmax, value, len)  Secure_Memset((dst), (dstmax), (value), (len))
#define memmov_s(dst, dstmax, src, len)    Secure_Memmove((dst), (dstmax), (src),   (len))

typedef uint32_t Secure_Status_t;
#define Status_MemOptSuccess       ((Secure_Status_t)(0xc35a5ac3u))
#define Status_MemcpyFail          ((Secure_Status_t)(0xc33c81c3u))
#define Status_MemsetFail          ((Secure_Status_t)(0xc33c82c3u))
#define Status_MemmoveFail         ((Secure_Status_t)(0xc33c81c3u))

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          Secure memory copy
 *
 *  @details        SRS ID:            AR_RS_System_00043
 *                  Design ID:         SDD_System_13
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Dst_ptr       destination address
 *  @param[in]      para_DstMax_u32    destination buffer max size
 *  @param[in]      para_Src_ptr       source address
 *  @param[in]      para_Len_u32       data lenth
 *
 *  @return         Secure_Status_t
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 **/
Secure_Status_t Secure_Memcpy(void *para_Dst_ptr, uint32_t para_DstMax_u32, void const *para_Src_ptr, uint32_t para_Len_u32);

/**
 *  @brief          Secure memory set
 *
 *  @details        SRS ID:            AR_RS_System_00044
 *                  Design ID:         SDD_System_14
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Dst_ptr       destination address
 *  @param[in]      para_DstMax_u32    destination buffer max size
 *  @param[in]      para_Value_u8    Value to set
 *  @param[in]      para_Len_u32       data lenth
 *
 *  @return         Secure_Status_t
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 **/
Secure_Status_t Secure_Memset(void *para_Dst_ptr, uint32_t para_DstMax_u32, uint8_t para_Value_u8, uint32_t para_Len_u32);

/**
 *  @brief          Secure memory move
 *
 *  @details        SRS ID:            AR_RS_System_00045
 *                  Design ID:         SDD_System_15
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Dst_ptr     destination address
 *  @param[in]      para_DstMax_u32  destination buffer max size
 *  @param[in]      para_Src_ptr     source address
 *  @param[in]      para_Len_u32     data lenth
 *
 *  @return         Secure_Status_t
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 **/
Secure_Status_t Secure_Memmove(void *para_Dst_ptr, uint32_t para_DstMax_u32, void const *para_Src_ptr, uint32_t para_Len_u32);

#ifdef __cplusplus
}
#endif

#endif /* MEM_UTIL_H */

/**
 * @}
 */
