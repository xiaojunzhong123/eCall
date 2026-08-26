/*******************************************************
  * Name    :Memory.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :MemoryCopy
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Memory_Copy(volatile uint8_t *p_des, volatile uint8_t *p_src, uint8_t len)
{
    while (len--)
    {
        *(p_des++) = *(p_src++);
    }
}

/*******************************************************
  * Name    :Memory_CopyConst
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Memory_CopyConst(volatile uint8_t *p_des, const uint8_t *p_src, uint8_t len)
{
    while (len--)
    {
        *(p_des++) = *(p_src++);
    }
}

/*******************************************************
  * Name    :Memory_CopyShort
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Memory_CopyShort(volatile uint8_t *p_des, volatile uint8_t *p_src, uint16_t len)
{
    while (len--)
    {
        *(p_des++) = *(p_src++);
    }
}

/*******************************************************
  * Name    :Memory_CopyLong
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Memory_CopyLong(volatile uint8_t *p_des, volatile uint8_t *p_src, uint32_t len)
{
    while (len--)
    {
        *(p_des++) = *(p_src++);
    }
}

/*******************************************************
  * Name    :MemoryFill
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Memory_Fill(volatile uint8_t *p_buff, uint8_t data, uint8_t len)
{
    while (len--)
    {
        *(p_buff++) = data;
    }
}

/*******************************************************
  * Name    :Memory_FillShort
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Memory_FillShort(volatile uint8_t *p_buff, uint8_t data, uint16_t len)
{
    while (len--)
    {
        *(p_buff++) = data;
    }
}

/*******************************************************
  * Name    :Memory_FillLong
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Memory_FillLong(volatile uint8_t *p_buff, uint8_t data, uint32_t len)
{
    while (len--)
    {
        *(p_buff++) = data;
    }
}

/*******************************************************
  * Name    :Memory_Compare
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Memory_Compare(volatile uint8_t *p_des, uint8_t *p_src, uint8_t len)
{
    while (len--)
    {
        if (*(p_des++) != *(p_src++))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*******************************************************
  * Name    :Memory_CheckIfInvalid
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Memory_CheckIfInvalid(uint8_t *p_buff, uint8_t len)
{
    while (len--)
    {
        if ((p_buff[len] != 0x00) && (p_buff[len] != 0xFF))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*******************************************************
  * Name    :Memory_AverageShort
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
uint16_t Memory_AverageShort(volatile uint16_t *p_buff, uint8_t len)
{
    uint32_t sum   = 0;
    uint8_t  lenth = len;

    while (len--)
    {
        sum += *(p_buff++);
    }
    return (uint16_t)(sum / lenth);
}

