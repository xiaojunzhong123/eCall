/*******************************************************
  * Name    :Lin_Cfg.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Lin_Cfg.h"
/*-------------variable statement---------------------*/
#define LIN_BIT(x, pos) (((x) >> (pos)) & 0x01U)
/*-------------Function-------------------------------*/
/*******************************************************
  * Name    :LIN_CalculateChecksum
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
uint8_t LIN_CalculateChecksum(uint8_t u8PID, const uint8_t au8Data[], uint8_t u8Len)
{
    uint8_t  i;
    uint8_t  u8ID = u8PID & 0x3FU;
    uint16_t u16Checksum;

    /* 0x3C 0x3D Classic Checksum */
    if ((u8ID == 0x3CU) || (u8ID == 0x3DU))
    {
        u16Checksum = 0U;
    }
    else
    {
        u16Checksum = (uint16_t)u8PID; /* Enhanced Checksum */
    }

    for (i = 0U; i < u8Len; i++)
    {
        u16Checksum += au8Data[i];

        /* Carry bit */
        if (u16Checksum > 0xFFU)
        {
            u16Checksum -= 0xFFU;
        }
    }

    return (uint8_t)((uint16_t)(~u16Checksum)); /* reversed */
}
/*******************************************************
  * Name    :LIN_CalculatePIDParity
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
uint8_t LIN_CalculatePIDParity(uint8_t u8ID)
{
    uint8_t u8P0;
    uint8_t u8P1;
    uint8_t u8Parity;

    u8Parity  = u8ID;
    u8P0      = (LIN_BIT(u8Parity, 0U) ^ LIN_BIT(u8Parity, 1U) ^ LIN_BIT(u8Parity, 2U) ^ LIN_BIT(u8Parity, 4U)) << 6U;
    u8P1      = (((LIN_BIT(u8Parity, 1U) ^ LIN_BIT(u8Parity, 3U) ^ LIN_BIT(u8Parity, 4U) ^ LIN_BIT(u8Parity, 5U)) > 0U) ? 0U : 1U);
    u8P1      = u8P1 << 7U;
    u8Parity |= (u8P0 | u8P1);

    return u8Parity;
}
