/*******************************************************
  * Name    :Rte.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Types.h"
#include "Rte.h"
#include "RteSys.h"
/*-------------variable statement---------------------*/
static Bool g_RteCurErrFlag[DTC_ID_MAX_NUM];
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Rte_GetDtcCurErrFlag
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Rte_GetDtcCurErrFlag(Rte_Dtc_Type id)
{
    return g_RteCurErrFlag[(uint8_t)id];
}

/*******************************************************
  * Name    :Rte_SetDtcCurErrFlag
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Rte_SetDtcCurErrFlag(Rte_Dtc_Type id, Bool flag)
{
    g_RteCurErrFlag[(uint8_t)id] = flag;
}

