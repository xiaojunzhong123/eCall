/*******************************************************
  * Name    :CanTransciever.c
  * Author  :WangHu
  * Function:CAN 收发调度与故障恢复管理
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "CanTransciever_Cfg.h"
/*-------------variable statement---------------------*/
static Can_Type                     g_CanTranscieverTxMsg;
static CanTransciever_TimeTick_Type g_CanTranscieverTimeTick;
static CanTransciever_NoAck_TYPE    g_CanTranscieverNoAckState;
static CanTransciever_BusOff_TYPE   g_CanTranscieverBusOffState;
static Bool                         g_CanTranscieverEnableTxAppMsg;
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :CanTransciever_TimerCtrl
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanTransciever_TimerCtrl(void)
{
    if (g_CanTranscieverEnableTxAppMsg == TRUE)
    {
        if (g_CanTranscieverTimeTick.startApp < 0xFFFF)
        {
            g_CanTranscieverTimeTick.startApp++;
        }
    }

    if (g_CanTranscieverTimeTick.startUds < 0xFFFF)
    {
        g_CanTranscieverTimeTick.startUds++;
    }

    if (g_CanTranscieverTimeTick.noAck < 0xFFFF)
    {
        g_CanTranscieverTimeTick.noAck++;
    }

    if (g_CanTranscieverTimeTick.busOff < 0xFFFF)
    {
        g_CanTranscieverTimeTick.busOff++;
    }
}

/*******************************************************
  * Name    :CanTransciever_Init
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanTransciever_Init(void)
{
    CanTransciever_McalInit();
    CanTransciever_SbcInit();
    g_CanTranscieverNoAckState  = CANTRANSCIEVER_NOACK_SEND;
    g_CanTranscieverBusOffState = CANTRANSCIEVER_BUSOFF_CHECK;
}

/*******************************************************
  * Name    :CanTransciever_DeInit
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanTransciever_DeInit(void)
{
    CanTransciever_McalDeInit();
    CanTransciever_SbcDeInit();
}

/*******************************************************
  * Name    :CanTransciever_WakeUp
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanTransciever_WakeUp(void)
{
    CanTransciever_McalInit();
    CanTransciever_SbcInit();
}

/*******************************************************
  * Name    :CanTransciever_Sleep
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanTransciever_Sleep(void)
{
    CanTransciever_ClearMail();
    CanTransciever_ClearNoAckFlag();
    CanTransciever_ClearBusOffCnt();
    CanTransciever_ClearBusOffFlag();
    CanTransciever_McalDeInit();
    CanTransciever_SbcDeInit();
    g_CanTranscieverNoAckState  = CANTRANSCIEVER_NOACK_SEND;
    g_CanTranscieverBusOffState = CANTRANSCIEVER_BUSOFF_CHECK;
}

/*******************************************************
  * Name    :CanTransciever_CheckStartAppTimerIsOk
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool CanTransciever_CheckStartAppTimerIsOk(void)
{
    if ((g_CanTranscieverEnableTxAppMsg == TRUE) && (g_CanTranscieverTimeTick.startApp >= CANTRANSCIEVER_MAX_START_APP_TIME))
    {
        return TRUE;
    }
    return FALSE;
}

/*******************************************************
  * Name    :CanTransciever_DtcErrorRecord
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanTransciever_DtcErrorRecord(void)
{
    if (CanTransciever_GetBusOffCnt() >= CANTRANSCIEVER_BUSOFF_DTC_CNT)
    {
        CanTransciever_SetDtcCurErrFlag(DTC_ID_BUS_OFF, TRUE);
    }
    else
    {
        CanTransciever_SetDtcCurErrFlag(DTC_ID_BUS_OFF, FALSE);
    }
}

/*******************************************************
  * Name    :CanTransciever_NoAckDeal
  * Author  :WangHu
  * Function:取 NM 帧并提交发送
  			→ WAIT：检查发送成功或错误
      			→ 成功：清错误，首次启动 APP 等待计时，回 SEND
      			→ NoAck 超时：清队列、禁止 APP/UDS 发送入队，进入 TIME_OUT
  			→ TIME_OUT：暂停 150 个计数，重新初始化，必要时重发保存的 NM 帧
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanTransciever_NoAckDeal(void)
{
    switch (g_CanTranscieverNoAckState)
    {
        case CANTRANSCIEVER_NOACK_SEND:
            if (CanTransciever_ReadNmTxMsg(&g_CanTranscieverTxMsg.frame, &g_CanTranscieverTxMsg.id, g_CanTranscieverTxMsg.buff, &g_CanTranscieverTxMsg.len) == TRUE)
            {
                (void)CanTransciever_TxMsg(g_CanTranscieverTxMsg.frame, g_CanTranscieverTxMsg.id, g_CanTranscieverTxMsg.buff, g_CanTranscieverTxMsg.len);
                CanTransciever_ClearNoAckFlag();
                g_CanTranscieverTimeTick.noAck = 0;
                g_CanTranscieverNoAckState     = CANTRANSCIEVER_NOACK_WAIT;
            }
            break;
        case CANTRANSCIEVER_NOACK_WAIT:
            if (CanTransciever_CheckIfTxOK() == TRUE)
            {
                if (g_CanTranscieverEnableTxAppMsg == FALSE)
                {
                    g_CanTranscieverTimeTick.startApp = 0;
                    g_CanTranscieverEnableTxAppMsg    = TRUE;
                }
                CanTransciever_ClearNoAckFlag();
                CanTransciever_ClearBusOffCnt();
                CanTransciever_ClearBusOffFlag();
                CanTransciever_SetTxMsgOKFlag();
                g_CanTranscieverNoAckState = CANTRANSCIEVER_NOACK_SEND;
            }
            else if (g_CanTranscieverTimeTick.startUds <= CANTRANSCIEVER_START_UDS_TIME)
            {
                g_CanTranscieverTimeTick.noAck = 0;
            }
            else if ((g_CanTranscieverTimeTick.noAck > 155) || (CanTransciever_GetBusOffCnt() > 0))
            {
                g_CanTranscieverNoAckState = CANTRANSCIEVER_NOACK_SEND;
            }
            else if (CanTransciever_GetNoAckFlag() == TRUE)
            {
                if (g_CanTranscieverTimeTick.noAck >= CANTRANSCIEVER_NOACK_TIME)
                {
                    CanTransciever_ClearMail();
                    CanTransciever_ClearAllTxBuff();
                    CanTransciever_DisableTxAppUdsMsg();
                    g_CanTranscieverTimeTick.noAck = 0;
                    g_CanTranscieverNoAckState     = CANTRANSCIEVER_NOACK_TIME_OUT;
                }
            }
            break;
        case CANTRANSCIEVER_NOACK_TIME_OUT:
            if (g_CanTranscieverTimeTick.noAck >= CANTRANSCIEVER_PAUSE_TIME)
            {
                CanTransciever_Init();
                CanTransciever_ClearNoAckFlag();
                CanTransciever_EnableTxAppUdsMsg();
                if (CanTransciever_CheckIfAllowTxMsg() == TRUE)
                {
                    g_CanTranscieverTimeTick.noAck = 0;
                    (void)CanTransciever_TxMsg(g_CanTranscieverTxMsg.frame, g_CanTranscieverTxMsg.id, g_CanTranscieverTxMsg.buff, g_CanTranscieverTxMsg.len);
                    g_CanTranscieverNoAckState = CANTRANSCIEVER_NOACK_WAIT;
                }
                else
                {
                    g_CanTranscieverNoAckState = CANTRANSCIEVER_NOACK_SEND;
                }
            }
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanTransciever_BusOffDeal
  * Author  :WangHu
  * Function:CHECK 检测故障
  				→ 清队列、禁 APP/UDS 发送入队、停 NM
  				→ QUICK 等 50，或 SLOW 等 1000
  				→ 初始化 CAN、恢复 NM
  				→ RECOVERY 观察 100
      				→ 再故障：按计数是否大于 10 选择慢/快恢复
      				→ 无故障：清状态，开放 APP/UDS 发送入队，回 CHECK
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanTransciever_BusOffDeal(void)
{
    switch (g_CanTranscieverBusOffState)
    {
        case CANTRANSCIEVER_BUSOFF_CHECK:
            if (g_CanTranscieverTimeTick.startUds > CANTRANSCIEVER_START_UDS_TIME)
            {
                if (CanTransciever_GetBusOffFlag() == TRUE)
                {
                    CanTransciever_ClearAllTxBuff();
                    CanTransciever_DisableTxAppUdsMsg();
                    CanTransciever_StopTxNmMsg();
                    CanTransciever_ClearMail();
                    g_CanTranscieverTimeTick.busOff = 0;
                    if (CANTRANSCIEVER_BUSOFF_FAST_CNT != 0)
                    {
                        g_CanTranscieverBusOffState = CANTRANSCIEVER_BUSOFF_QUICK;
                    }
                    else
                    {
                        g_CanTranscieverBusOffState = CANTRANSCIEVER_BUSOFF_SLOW;
                    }
                }
            }
            break;
        case CANTRANSCIEVER_BUSOFF_QUICK:
            if (g_CanTranscieverTimeTick.busOff >= CANTRANSCIEVER_BUSOFF_FAST_TIME)
            {
                CanTransciever_Init();
                CanTransciever_ClearBusOffFlag();
                CanTransciever_StartTxNmMsg();
                g_CanTranscieverTimeTick.busOff = 0;
                g_CanTranscieverBusOffState     = CANTRANSCIEVER_BUSOFF_RECOVERY;
            }
            break;
        case CANTRANSCIEVER_BUSOFF_SLOW:
            if (g_CanTranscieverTimeTick.busOff >= CANTRANSCIEVER_BUSOFF_SLOW_TIME)
            {
                g_CanTranscieverTimeTick.busOff = 0;
                CanTransciever_Init();
                CanTransciever_ClearBusOffFlag();
                CanTransciever_StartTxNmMsg();
                g_CanTranscieverBusOffState = CANTRANSCIEVER_BUSOFF_RECOVERY;
            }
            break;
        case CANTRANSCIEVER_BUSOFF_RECOVERY:
            if ((g_CanTranscieverTimeTick.busOff >= 100) || (CanTransciever_GetBusOffFlag() == TRUE))
            {
                if (CanTransciever_GetBusOffFlag() == TRUE)
                {
                    CanTransciever_ClearAllTxBuff();
                    CanTransciever_DisableTxAppUdsMsg();
                    CanTransciever_StopTxNmMsg();
                    CanTransciever_ClearMail();
                    g_CanTranscieverTimeTick.busOff = 0;
                    if (CanTransciever_GetBusOffCnt() <= CANTRANSCIEVER_BUSOFF_FAST_CNT)
                    {
                        g_CanTranscieverBusOffState = CANTRANSCIEVER_BUSOFF_QUICK;
                    }
                    else
                    {
                        g_CanTranscieverBusOffState = CANTRANSCIEVER_BUSOFF_SLOW;
                    }
                }
                else
                {
                    CanTransciever_ClearBusOffCnt();
                    CanTransciever_ClearBusOffFlag();
                    CanTransciever_EnableTxAppUdsMsg();
                    g_CanTranscieverBusOffState = CANTRANSCIEVER_BUSOFF_CHECK;
                }
            }
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanTransciever_RxTxMsgHandler
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool CanTransciever_RxMsgHandler(Frame_Type frame, uint32_t id, volatile uint8_t *p_buff, uint8_t len)
{
    if ((id == UDS_FUNC_ADDR_ID) || (id == UDS_PHYS_ADDR_ID))
    {
        return CanTransciever_WriteUdsRxMsg(frame, id, p_buff, len);
    }

    if ((id >= CANNM_BASE_ID) && (id <= (CANNM_BASE_ID + 0xFF)) && (id != CANNM_ID))
    {
        return CanTransciever_WriteNmRxMsg(frame, id, p_buff, len);
    }

    if (CanTransciever_CheckIfRxMsg(id) == TRUE)
    {
        return CanTransciever_WriteAppRxMsg(frame, id, p_buff, len);
    }
    return FALSE;
}

/*******************************************************
  * Name    :CanTransciever_RxTxMsgHandler
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanTransciever_TxMsgHandler(void)
{
    uint8_t  i = 0;
    Can_Type msg;

    for (i = 0; i < CANTRANSCIEVER_MAX_TX_MSG_CNT; i++)
    {
        if (CanTransciever_HasEmptyMail() == TRUE)
        {
            if (CanTransciever_ReadAppTxMsg(&msg.frame, &msg.id, msg.buff, &msg.len) == FALSE)
            {
                if (CanTransciever_ReadUdsTxMsg(&msg.frame, &msg.id, msg.buff, &msg.len) == FALSE)
                {
                    break;
                }
            }
            (void)CanTransciever_TxMsg(msg.frame, msg.id, msg.buff, msg.len);
        }
    }
}

/*******************************************************
  * Name    :CanTransciever_NormalDeal
  * Author  :WangHu
  * Function:NoAck 处理 → Bus-Off 处理 → CanMatrix 生成普通帧 → APP/UDS 发送 → DTC 上报
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanTransciever_NormalDeal(void)
{
    CanTransciever_NoAckDeal();
    CanTransciever_BusOffDeal();
    CanTransciever_MatrixTxMsg();
    CanTransciever_TxMsgHandler();
    CanTransciever_DtcErrorRecord();
}

