/*******************************************************
  * Name    :CanNm.c
  * Author  :WangHu
  * Function:周期生成本节点的 NM 报文。
			启停普通业务报文和诊断帧的收发权限。
			向 CanManage、CanTransciever 提供工作状态和发送控制接口。
			CanNm.c	启停控制、周期计时、生成 NM 帧、状态查询
			CanNm_Cfg.h	NM ID、发送周期，以及 CanIf 接口映射
			由 CanManage 控制启停的周期 NM 帧发生器，同时通过 CanIf 管理 APP/UDS 通信权限，
			并为收发故障恢复提供暂停、恢复接口
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "CanNm_Cfg.h"
/*-------------variable statement---------------------*/
static Bool     g_CanNmWorkIsOn;
static Bool     g_CanNmTimeCtr;
static uint16_t g_CanNmTimeTick;
static Can_Type g_CanNmTxMsg;
/*-------------function statement---------------------*/
/*******************************************************
  * Name    :CanNm_TimerCtrl
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanNm_TimerCtrl(void)
{
    if (g_CanNmTimeCtr == TRUE)
    {
        if (g_CanNmTimeTick < 0xFFFF)
        {
            g_CanNmTimeTick++;
        }
    }
}

/*******************************************************
  * Name    :CanNm_EnableRxTxAppUdsMsg
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanNm_EnableRxTxAppUdsMsg(void)
{
    g_CanNmTimeTick = 0;
    g_CanNmTimeCtr  = TRUE;
    CanNm_SwtichAppUdsCtlMsg(TRUE, TRUE);
    CanNm_SwtichAppUdsCtlMsg(FALSE, TRUE);
}

/*******************************************************
  * Name    :CanNm_DisableRxTxAppUdsMsg
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanNm_DisableRxTxAppUdsMsg(void)
{
    g_CanNmTimeTick = 0;
    g_CanNmTimeCtr  = FALSE;
    CanNm_SwtichAppUdsCtlMsg(TRUE, FALSE);
    CanNm_SwtichAppUdsCtlMsg(FALSE, FALSE);
}

/*******************************************************
  * Name    :CanNm_Start
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanNm_Start(void)
{
    g_CanNmWorkIsOn = TRUE;
    CanNm_EnableRxTxAppUdsMsg();
    g_CanNmTimeTick = CANNM_CYCLE_TIME - 50u;
}

/*******************************************************
  * Name    :CanNm_Stop
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanNm_Stop(void)
{
    g_CanNmWorkIsOn = FALSE;
    CanNm_DisableRxTxAppUdsMsg();
}

/*******************************************************
  * Name    :CanNm_WakeUp
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanNm_WakeUp(void)
{
    g_CanNmWorkIsOn = TRUE;
    CanNm_EnableRxTxAppUdsMsg();
    g_CanNmTimeTick = CANNM_CYCLE_TIME - 50;
}

/*******************************************************
  * Name    :CanNm_Sleep
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanNm_Sleep(void)
{
    g_CanNmWorkIsOn = FALSE;
    CanNm_DisableRxTxAppUdsMsg();
}

/*******************************************************
  * Name    :CanNm_SetSleeped
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanNm_SetSleeped(void)
{
    CanNm_DisableRxTxAppUdsMsg();
}

/*******************************************************
  * Name    :CanNm_StartTxMsg
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanNm_StartTxMsg(void)
{
    g_CanNmTimeTick = CANNM_CYCLE_TIME;
    g_CanNmTimeCtr  = TRUE;
}

/*******************************************************
  * Name    :CanNm_StopTxMsg
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanNm_StopTxMsg(void)
{
    g_CanNmTimeTick = 0;
    g_CanNmTimeCtr  = FALSE;
}

/*******************************************************
  * Name    :CanNm_SetTxMsgOKFlag
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanNm_SetTxMsgOKFlag(void)
{
}

/*******************************************************
  * Name    :CanNM_CheckIfAllowTxMsg
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool CanNM_CheckIfAllowTxMsg(void)
{
    if (g_CanNmWorkIsOn == TRUE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*******************************************************
  * Name    :CanNm_CheckIfSleep
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool CanNm_CheckIfSleep(void)
{
    if (g_CanNmWorkIsOn == FALSE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*******************************************************
  * Name    :CanNm_TxMsgInfo
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanNm_TxMsgInfo(void)
{
    g_CanNmTxMsg.id  = CANNM_ID;
    g_CanNmTxMsg.len = 8;
    CanNm_WriteTxMsg(CAN, g_CanNmTxMsg.id, g_CanNmTxMsg.buff, g_CanNmTxMsg.len);
}

/*******************************************************
  * Name    :CanNm_Main
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanNm_Main(void)
{
    if (g_CanNmWorkIsOn == TRUE)
    {
        if (g_CanNmTimeTick >= CANNM_CYCLE_TIME)
        {
            g_CanNmTimeTick = 0;
            CanNm_TxMsgInfo();
        }
    }
}

