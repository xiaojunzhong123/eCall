/*******************************************************
  * Name    :RteSys.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Types.h"
#include "RteSys.h"
/*-------------variable statement---------------------*/
static uint8_t g_RteSysU8Sig[RTESYS_U8_TYPE_MAX];

/*******************************************************
  * Name    :RteSys_SetU8Sig
  * Author  :LoH
  * Function:Set TBOX RTE U8 signal value.
  * Version :V1.0.0
*******************************************************/
void RteSys_SetU8Sig(RteSys_U8_Type id, uint8_t sig)
{
    if (id < RTESYS_U8_TYPE_MAX)
    {
        g_RteSysU8Sig[id] = sig;
    }
}

/*******************************************************
  * Name    :RteSys_GetU8Sig
  * Author  :LoH
  * Function:Get TBOX RTE U8 signal value.
  * Version :V1.0.0
*******************************************************/
uint8_t RteSys_GetU8Sig(RteSys_U8_Type id)
{
    if (id < RTESYS_U8_TYPE_MAX)
    {
        return g_RteSysU8Sig[id];
    }
    return (uint8_t)RTE_SWITCH_RELEASED;
}
