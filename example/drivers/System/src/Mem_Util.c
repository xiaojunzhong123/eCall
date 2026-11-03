/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Mem_Util.c
 *
 * Description: Implementation of SYSTEM
 *              Compiler:                IAR GCC
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
 *   @file       Mem_Util.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 SYSTEM Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Mem_Util.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
 
#define SECURE_API_CHK(condition)        \
    if(condition)                        \
    {                                    \
        Result_u32 ^= Status_MemOptSuccess;      \
        break;         \
    }

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
 *  @details        Service ID:        0x0d
 *                  SRS ID:            AR_RS_System_00043
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
Secure_Status_t Secure_Memcpy(void *para_Dst_ptr, uint32_t para_DstMax_u32, void const *para_Src_ptr, uint32_t para_Len_u32)
{
    uint32_t Result_u32 = Status_MemcpyFail ^ Status_MemOptSuccess;

    uint32_t const Dst_End_u32 = (uint32_t)para_Dst_ptr + para_Len_u32;
    uint32_t const Src_End_u32 = (uint32_t)para_Src_ptr + para_Len_u32;
    uint32_t i = 0U;

    do
    {
        /* check validity & integrity of all the parameters */
        SECURE_API_CHK(para_Dst_ptr == NULL_PTR); 
        SECURE_API_CHK(para_Src_ptr == NULL_PTR); 
        SECURE_API_CHK((para_Len_u32 == 0U)); /* copy with para_Len_u32 0 is not allowed */
        SECURE_API_CHK((para_Len_u32 > para_DstMax_u32)); /* para_Len_u32 should less than max buffer size */

        /* check if there is overlap issue*/
        if(para_Src_ptr > para_Dst_ptr)
        {
            SECURE_API_CHK(((uint8_t*)para_Src_ptr) < ((uint8_t*)para_Dst_ptr + para_Len_u32)); 
        }
        
        /* check if there is overlap issue*/
        if(para_Src_ptr < para_Dst_ptr)
        {
            SECURE_API_CHK(((uint8_t*)para_Dst_ptr) < ((uint8_t*)para_Src_ptr + para_Len_u32)); 
        }
        
        if (para_Len_u32 >=  sizeof(uint32_t))
        {
            /* start to copy  */
            for (i = 0U; i < para_Len_u32/ sizeof(uint32_t); i++)
            {
                uint32_t const CopyVal_u32 = *(uint32_t const*)para_Src_ptr;
                *(uint32_t *)para_Dst_ptr = CopyVal_u32;
                para_Dst_ptr = (uint32_t *)para_Dst_ptr + 1;
                para_Src_ptr = (uint32_t const *)para_Src_ptr + 1;
            }
            
            SECURE_API_CHK((i != (para_Len_u32 /  sizeof(uint32_t)))); /* check loop completion */
        }
        
        /* copy the left bytes */
        if ((para_Len_u32 %  sizeof(uint32_t)) > 0U)
        {
          
            for (i = 0U; i < para_Len_u32 %  sizeof(uint32_t); i++)
            {
                uint8_t const CopyVal_u8 = *(uint8_t const *)para_Src_ptr;
                *(uint8_t *)para_Dst_ptr = CopyVal_u8;
                para_Dst_ptr = (uint8_t *)para_Dst_ptr + 1;
                para_Src_ptr = (const uint8_t *)para_Src_ptr + 1; 
            }
 
            SECURE_API_CHK((i != para_Len_u32 %  sizeof(uint32_t))); /* check loop completion */
        }

        SECURE_API_CHK(((uint32_t)para_Dst_ptr != Dst_End_u32)); /* check the dest end address  */
        SECURE_API_CHK(((uint32_t)para_Src_ptr != Src_End_u32)); /* check the src  end address  */

        /* return result */
        Result_u32 ^= Status_MemcpyFail;
    }while(0U);
    
    return Result_u32;
}

/**
 *  @brief          Secure memory set
 *
 *  @details        Service ID:        0x0e
 *                  SRS ID:            AR_RS_System_00044
 *                  Design ID:         SDD_System_14
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Dst_ptr       destination address
 *  @param[in]      para_DstMax_u32    destination buffer max size
 *  @param[in]      para_Value_u8      Value to set
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
Secure_Status_t Secure_Memset(void *para_Dst_ptr, uint32_t para_DstMax_u32, uint8_t para_Value_u8, uint32_t para_Len_u32)
{
    uint32_t Result_u32 = Status_MemsetFail ^ Status_MemOptSuccess;

    uint32_t Dst_End_u32  = (uint32_t)para_Dst_ptr + para_Len_u32;
    uint32_t i = 0u;
    uint32_t CopyWord_u32 = 0u;
    do
    {
        /* check validity & integrity of all the parameters */
        SECURE_API_CHK(para_Dst_ptr == NULL_PTR); 
        SECURE_API_CHK(para_Len_u32 == 0u);  /* copy with para_Len_u32 0 is not allowed */
        SECURE_API_CHK((para_Len_u32 > para_DstMax_u32)); /* para_Len_u32 should less than max buffer size */

        if (para_Len_u32 >=  sizeof(uint32_t))
        {
            for (i = 0u; i < sizeof(uint32_t); i++)
            {
                CopyWord_u32 = CopyWord_u32 << 8;
                CopyWord_u32 += para_Value_u8;
            }
            
            /* start to copy  */
            for (i = 0u; i < para_Len_u32 / sizeof(uint32_t); i++)
            {
                *(volatile uint32_t *)para_Dst_ptr = CopyWord_u32;
                para_Dst_ptr = (uint32_t *)para_Dst_ptr + 1;
            }

            SECURE_API_CHK((i != para_Len_u32 /  sizeof(uint32_t))); /* check loop completion */
        }
        
        /* set the left bytes */
        if ((para_Len_u32 %  sizeof(uint32_t)) > 0U)
        {
            for (i = 0U; i < para_Len_u32 %  sizeof(uint32_t); i++)
            {
                *(volatile uint8_t *)para_Dst_ptr = para_Value_u8;
                para_Dst_ptr = (uint8_t *)para_Dst_ptr + 1;
            }
            SECURE_API_CHK((i != para_Len_u32 %  sizeof(uint32_t))); /* check loop completion */
        }

        SECURE_API_CHK(((uint32_t)para_Dst_ptr != Dst_End_u32)); /* check the dest end address  */

        Result_u32 ^= Status_MemsetFail;
        
    } while (0U);
    
    return Result_u32;
}

/**
 *  @brief          Secure memory move
 *
 *  @details        Service ID:        0x0f
 *                  SRS ID:            AR_RS_System_00045
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
Secure_Status_t Secure_Memmove(void *para_Dst_ptr, uint32_t para_DstMax_u32, void const *para_Src_ptr, uint32_t para_Len_u32)
{
    uint32_t Result_u32 = Status_MemcpyFail ^ Status_MemOptSuccess;
    uint32_t i = 0U;

    do
    {
        SECURE_API_CHK(para_Dst_ptr == NULL_PTR); 
        SECURE_API_CHK(para_Src_ptr == NULL_PTR);
        SECURE_API_CHK((para_Len_u32 == 0U)); /* copy with para_Len_u32 0 is not allowed */
        SECURE_API_CHK((para_Len_u32 > para_DstMax_u32)); /* para_Len_u32 should less than max buffer size */

        if(((uint32_t)para_Dst_ptr <= (uint32_t)para_Src_ptr) || (((uint32_t)para_Dst_ptr) >= ((uint32_t)para_Src_ptr + para_Len_u32)))
        {
            uint32_t const Dst_End_u32 = (uint32_t)para_Dst_ptr + para_Len_u32;
            uint32_t const Src_End_u32 = (uint32_t)para_Src_ptr + para_Len_u32;

            /* start to copy  */
            for (i = 0U; i < para_Len_u32; i++)
            {
                uint8_t const CopyVal_u8 = *(uint8_t const *)para_Src_ptr;
                *(uint8_t *)para_Dst_ptr = CopyVal_u8;
                para_Dst_ptr = (uint8_t *)para_Dst_ptr + 1;
                para_Src_ptr = (const uint8_t *)para_Src_ptr + 1; 
            }
     
            SECURE_API_CHK(i != para_Len_u32); /* check loop completion */
            SECURE_API_CHK(((uint32_t)para_Dst_ptr != Dst_End_u32)); /* check the dest end address  */
            SECURE_API_CHK(((uint32_t)para_Src_ptr != Src_End_u32)); /* check the src  end address  */
            
            /* return result */
            Result_u32 ^= Status_MemmoveFail;
        }
        else
        {
          
            uint8_t *Dst_End_u32 = (uint8_t *)((uint32_t)para_Dst_ptr + para_Len_u32 -1U);
            uint8_t *Src_End_u32 = (uint8_t *)((uint32_t)para_Src_ptr + para_Len_u32 -1U);
            
            /* start to copy left bytes  */
            for (i = 0U; i < para_Len_u32; i++)
            {
                uint8_t CopyVal_u32 = *(uint8_t *)Src_End_u32;
                *(uint8_t *)Dst_End_u32 = CopyVal_u32;
                Dst_End_u32 = (uint8_t *)Dst_End_u32 - 1;
                Src_End_u32 = (uint8_t *)Src_End_u32 - 1;
            }

            SECURE_API_CHK(i != para_Len_u32); /* check loop completion */
            
            /* return result */
            Result_u32 ^= Status_MemmoveFail;
        }
    }while(0U);
    
    return Result_u32;
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
