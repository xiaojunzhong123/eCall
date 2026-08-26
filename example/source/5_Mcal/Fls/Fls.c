/*******************************************************
  * Name    :Fls.c
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Fls_Cfg.h"
/*-------------variable statement---------------------*/

/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Fls_Init
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Fls_Init(void)
{
    return TRUE;
}

/*******************************************************
  * Name    :Fls_DeInit
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Fls_DeInit(void)
{
    return TRUE;
}

/*******************************************************
  * Name    :Fls_Erase
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Return_Type Fls_Erase(uint32_t u32Addr, uint32_t u32Size)
{
    uint32_t i;
    uint32_t u32PageNum;

    if (u32Addr >= (FLASH_BASE + FLASH_SIZE))
    {
        return RET_NOT_OK;
    }
    LL_PERIPH_WRITE_ENABLE();
    EFM_FWMC_Cmd(ENABLE);
    if (u32Size == 0U)
    {
        if (EFM_SectorErase(u32Addr) == LL_OK)
        {
            return RET_OK;
        }
        else
        {
            return RET_NOT_OK;
        }
    }
    else
    {
        u32PageNum = u32Size / FLASH_SECTOR_SIZE;
        if ((u32Size % FLASH_SECTOR_SIZE) != 0UL)
        {
            u32PageNum += 1U;
        }
        for (i = 0; i < u32PageNum; i++)
        {
            if (LL_OK != EFM_SectorErase(u32Addr + (i * FLASH_SECTOR_SIZE)))
            {
                return RET_NOT_OK;
            }
        }
    }
    EFM_FWMC_Cmd(DISABLE);
    LL_PERIPH_WRITE_DISABLE();

    return RET_OK;
}

/*******************************************************
  * Name    :Fls_Program
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Fls_Program(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len)
{
    Bool ret = TRUE;

    if ((pu8Buff == NULL) || (u32Len == 0U) || ((u32Addr + u32Len) > (FLASH_BASE + FLASH_SIZE)))
    {
        return FALSE;
    }
    if (0UL != (u32Addr % 4U))
    {
        return FALSE;
    }
    LL_PERIPH_WRITE_ENABLE();
    EFM_FWMC_Cmd(ENABLE);
    ret = (EFM_Program(u32Addr, pu8Buff, u32Len) == LL_OK) ? TRUE : FALSE;
    EFM_FWMC_Cmd(DISABLE);
    LL_PERIPH_WRITE_DISABLE();
    return ret;
}

/*******************************************************
  * Name    :Fls_Read
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Fls_Read(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len)
{
    uint32_t       i;
    uint32_t       u32WordLength, u8ByteRemain;
    uint32_t      *pu32ReadBuff;
    __IO uint32_t *pu32FlashAddr;
    uint8_t       *pu8Byte;
    __IO uint8_t  *pu8FlashAddr;

    if ((pu8Buff == NULL) || (u32Len == 0U) || ((u32Addr + u32Len) > (FLASH_BASE + FLASH_SIZE)))
    {
        return FALSE;
    }
    if (0UL != (u32Addr % 4U))
    {
        return FALSE;
    }

    pu32ReadBuff  = (uint32_t *)(uint32_t)pu8Buff;
    pu32FlashAddr = (uint32_t *)u32Addr;
    u32WordLength = u32Len / 4U;
    u8ByteRemain  = u32Len % 4U;
    /* Read data */
    for (i = 0UL; i < u32WordLength; i++)
    {
        *(pu32ReadBuff++) = *(pu32FlashAddr++);
    }
    if (0UL != u8ByteRemain)
    {
        pu8Byte      = (uint8_t *)pu32ReadBuff;
        pu8FlashAddr = (uint8_t *)pu32FlashAddr;
        for (i = 0UL; i < u8ByteRemain; i++)
        {
            *(pu8Byte++) = *(pu8FlashAddr++);
        }
    }

    return TRUE;
}

/*******************************************************
  * Name    :Fls_GetProgramStatus
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Return_Type Fls_GetProgramStatus(void)
{
    Return_Type ret = RET_OK;
    return ret;
}

/*******************************************************
  * Name    :Fls_DriverRamCode
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Fls_DriverRamCode(uint32_t addr, uint8_t *p_buff, uint16_t len)
{
    return TRUE;
}

