/*******************************************************
  * Name    :CanMatrix.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "CanMatrix_Cfg.h"
/* 任务启动日志使用已有声明，避免隐式推断函数类型。 */
#include "Hmi.h"
/*-------------variable statement---------------------*/
static Can_Type                  g_CanMatrixRxMsg;
static CanMatrix_DtcMsgInfo_TYPE g_CanMatrixDtcMsgInfo[CANMATRIX_RX_LIST_NUM];
static CanMatrix_RxMsgInfo_Type  g_CanMatrixRxMsgInfo[CANMATRIX_RX_LIST_NUM];
static CanMatrix_TxMsgInfo_Type  g_CanMatrixTxMsgInfo[CANMATRIX_TX_LIST_NUM];
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :CanMatrix_Init
  * Author  :xiaojunzhong
  * Function:初始化矩阵缓存，仅调用已配置的接收初始化回调。
  * Version :V1.2.0
  * Data    :2026.11.07
  * Caller  :TaskManager_Start，任务创建前一次。
  * Input   :沿用既有参数；接收表回调允许为空。
  * Output  :沿用既有返回约定，不新增业务成功结论。
  * Precond :共享RTE及CAN服务已完成启动初始化。
  * Failure :无回调项只保留缓存/匹配行为，不能调用空函数指针。
  * Effect  :0x10C不再经矩阵初始化SRS，统一由共享RTE启动初始化。
*******************************************************/
void CanMatrix_Init(void)
{
    uint8_t i = 0;

    CanMatrix_DefaultDataInit();
    for (i = 0; i < CANMATRIX_RX_LIST_NUM; i++)
    {
        if (g_CanMatrixRxList[i].dataHandleFun != NULL)
        {
            g_CanMatrixRxList[i].dataHandleFun(CANMATRIX_RX_STATE_INIT);
        }
    }
}

/*******************************************************
  * Name    :CanMatrix_CheckIfRxMsg
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool CanMatrix_CheckIfRxMsg(uint32_t id)
{
    uint8_t i = 0;

    for (i = 0; i < CANMATRIX_RX_LIST_NUM; i++)
    {
        if (g_CanMatrixRxList[i].id == id)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*******************************************************
  * Name    :CanMatrix_IsMsdRxId
  * Author  :xiaojunzhong
  * Function:Identify MSD observations owned exclusively by the CAN service ingress.
  * Version :V1.1.0
  * Data    :2026.10.31
*******************************************************/
static uint8_t CanMatrix_IsMsdRxId(uint32_t u32Id)
{
    return ((u32Id == 0x10CU) || (u32Id == 0x116U) || (u32Id == 0x28BU)) ? 1U : 0U;
}

/*******************************************************
  * Name    :CanMatrix_ChechRxMsg
  * Author  :xiaojunzhong
  * Function:路由普通帧并保护可选回调，三项专属观测仍跳过旧接收入口。
  * Version :V1.2.0
  * Data    :2026.11.07
  * Caller  :CanMatrixRxMsgTask，既有接收队列消费者。
  * Input   :沿用既有参数；接收表回调允许为空。
  * Output  :沿用既有返回约定，不新增业务成功结论。
  * Precond :共享RTE及CAN服务已完成启动初始化。
  * Failure :无回调项只保留缓存/匹配行为，不能调用空函数指针。
  * Effect  :不把普通CanIf队列变成第二个SRS生产者。
*******************************************************/
static void CanMatrix_ChechRxMsg(Can_Type *p_msg)
{
    uint8_t i = 0;

    /* Prevent a legacy CanIf queue from becoming a second MSD producer. */
    if (CanMatrix_IsMsdRxId(p_msg->id) != 0U)
    {
        return;
    }
    for (i = 0; i < CANMATRIX_RX_LIST_NUM; i++)
    {
        if ((p_msg->frame == g_CanMatrixRxList[i].frame)
            && (p_msg->id == g_CanMatrixRxList[i].id)
            && (p_msg->len >= g_CanMatrixRxList[i].len))
        {
            if (!CanMatrix_Compare(g_CanMatrixRxList[i].p_buff, p_msg->buff, p_msg->len)
                || (g_CanMatrixRxList[i].IsChangeIn == FALSE))
            {
                CanMatrix_Copy(g_CanMatrixRxList[i].p_buff, p_msg->buff, p_msg->len);
                if (g_CanMatrixRxList[i].dataHandleFun != NULL)
                {
                    g_CanMatrixRxList[i].dataHandleFun(CANMATRIX_RX_STATE_NORMAL);
                }
            }
            g_CanMatrixRxMsgInfo[i].flag = TRUE;
            break;
        }
    }

    for (i = 0; i < CANMATRIX_DTC_LIST_NUM; i++)
    {
        if (p_msg->id == g_CanMatrixDtcList[i].id)
        {
            g_CanMatrixDtcMsgInfo[i].flag = TRUE;
            break;
        }
    }
}

/*******************************************************
  * Name    :CanMatrix_CheckRxIfLost
  * Author  :xiaojunzhong
  * Function:沿用普通报文丢失监测，空回调项不执行超时业务。
  * Version :V1.2.0
  * Data    :2026.11.07
  * Caller  :CanMatrixRxMsgTask周期处理。
  * Input   :沿用既有参数；接收表回调允许为空。
  * Output  :沿用既有返回约定，不新增业务成功结论。
  * Precond :共享RTE及CAN服务已完成启动初始化。
  * Failure :无回调项只保留缓存/匹配行为，不能调用空函数指针。
  * Effect  :三项车辆观测继续由RTE按原采样时间判断有效期。
*******************************************************/
static void CanMatrix_CheckRxIfLost(void)
{
    uint8_t i = 0;

    for (i = 0; i < CANMATRIX_RX_LIST_NUM; i++)
    {
        /* These observations are aged in RTE, never by this legacy task. */
        if (CanMatrix_IsMsdRxId(g_CanMatrixRxList[i].id) != 0U)
        {
            continue;
        }
        if (g_CanMatrixRxMsgInfo[i].flag == TRUE)
        {
            g_CanMatrixRxMsgInfo[i].time = 0;
            g_CanMatrixRxMsgInfo[i].flag = FALSE;
            g_CanMatrixRxMsgInfo[i].lost = FALSE;
        }
        else
        {
            if (g_CanMatrixRxMsgInfo[i].lost == TRUE)
            {
                continue;
            }
            else if (g_CanMatrixRxMsgInfo[i].time >= g_CanMatrixRxList[i].lostTime)
            {
                g_CanMatrixRxMsgInfo[i].lost = TRUE;
                if (g_CanMatrixRxList[i].dataHandleFun != NULL)
                {
                    g_CanMatrixRxList[i].dataHandleFun(CANMATRIX_RX_STATE_LOST);
                }
            }
            else
            {
                g_CanMatrixRxMsgInfo[i].time += CANMATRIX_RX_SCHEDULE_TIME;
            }
        }
    }
}

/*******************************************************
  * Name    :CanMatrix_CheckDtcIfLost
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanMatrix_CheckDtcIfLost(void)
{
    uint8_t i = 0;

    for (i = 0; i < CANMATRIX_DTC_LIST_NUM; i++)
    {
        if ((CanMatrix_GetDtcDeactiveFlag() == TRUE)
            || (CanMatrix_CheckNMIfAllowTxMsg() == FALSE)
            || (CanMatrix_CheckIsInNoAck() == TRUE)
            || (CanMatrix_CheckIsInBusOff() == TRUE))
        {
            g_CanMatrixDtcMsgInfo[i].time = 0;
            g_CanMatrixDtcMsgInfo[i].tick = 0;
            g_CanMatrixDtcMsgInfo[i].flag = FALSE;
            if (g_CanMatrixDtcMsgInfo[i].lost == TRUE)
            {
                g_CanMatrixDtcMsgInfo[i].lost = FALSE;
                g_CanMatrixDtcList[i].dataHandleFun();
            }
            continue;
        }

        if (g_CanMatrixDtcMsgInfo[i].flag == TRUE)
        {
            g_CanMatrixDtcMsgInfo[i].flag = FALSE;
            if (g_CanMatrixDtcMsgInfo[i].lost == TRUE)
            {
                if (++g_CanMatrixDtcMsgInfo[i].tick > CANMATRIX_CTR_DTC_TIMES)
                {
                    g_CanMatrixDtcMsgInfo[i].lost = FALSE;
                    g_CanMatrixDtcList[i].dataHandleFun();
                }
            }
        }
        else
        {
            if (g_CanMatrixDtcMsgInfo[i].lost == TRUE)
            {
                continue;
            }
            else if (g_CanMatrixDtcMsgInfo[i].time >= g_CanMatrixDtcList[i].lostTime)
            {
                g_CanMatrixDtcMsgInfo[i].tick = 0;
                g_CanMatrixDtcMsgInfo[i].lost = TRUE;
                g_CanMatrixDtcList[i].dataHandleFun();
            }
            else
            {
                g_CanMatrixDtcMsgInfo[i].time += CANMATRIX_RX_SCHEDULE_TIME;
            }
        }
    }
}

/*******************************************************
  * Name    :CanMatrix_ClearTxMsgTimer
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanMatrix_ClearTxMsgTimer(void)
{
    uint8_t i = 0;

    for (i = 0; i < CANMATRIX_TX_LIST_NUM; i++)
    {
        g_CanMatrixTxMsgInfo[i].txTimes  = 0;
        g_CanMatrixTxMsgInfo[i].fastTick = 0;
        g_CanMatrixTxMsgInfo[i].timeTick = 0;
        g_CanMatrixTxMsgInfo[i].txFlag   = FALSE;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxEventMsg
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanMatrix_TxEventMsg(uint8_t index)
{
    uint8_t                   txTimes = 0;
    CanMatrix_TxList_Type    *p_msg   = NULL;
    CanMatrix_TxMsgInfo_Type *p_info  = NULL;

    p_msg                             = &g_CanMatrixTxList[index];
    p_info                            = &g_CanMatrixTxMsgInfo[index];
    txTimes                           = p_msg->dataHandleFun(CANMATRIX_TX_STATE_CHECK);
    if (txTimes > p_info->txTimes)
    {
        p_info->txTimes = txTimes;
    }
    if (p_info->timeTick >= p_msg->fastPeroid)
    {
        if (p_info->txTimes > 0)
        {
            p_info->txTimes--;
            (void)p_msg->dataHandleFun(CANMATRIX_TX_STATE_UPDATE);
            (void)CanMatrix_WriteTxMsg(p_msg->frame, p_msg->id, p_msg->p_buff, p_msg->len);
            if (p_info->txTimes == 0)
            {
                (void)p_msg->dataHandleFun(CANMATRIX_TX_STATE_CLEAR);
            }
            p_info->timeTick = CANMATRIX_TX_SCHEDULE_TIME;
        }
    }
    else
    {
        p_info->timeTick += CANMATRIX_TX_SCHEDULE_TIME;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxCycleMsg
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanMatrix_TxCycleMsg(uint8_t index)
{
    CanMatrix_TxList_Type    *p_msg  = NULL;
    CanMatrix_TxMsgInfo_Type *p_info = NULL;

    p_msg                            = &g_CanMatrixTxList[index];
    p_info                           = &g_CanMatrixTxMsgInfo[index];
    (void)p_msg->dataHandleFun(CANMATRIX_TX_STATE_CHECK);
    if (p_info->timeTick >= p_msg->peroid)
    {
        (void)p_msg->dataHandleFun(CANMATRIX_TX_STATE_UPDATE);
        (void)CanMatrix_WriteTxMsg(p_msg->frame, p_msg->id, p_msg->p_buff, p_msg->len);
        (void)p_msg->dataHandleFun(CANMATRIX_TX_STATE_CLEAR);
        p_info->timeTick = CANMATRIX_TX_SCHEDULE_TIME;
    }
    else
    {
        p_info->timeTick += CANMATRIX_TX_SCHEDULE_TIME;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxCycEvMsg
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanMatrix_TxCycEvMsg(uint8_t index)
{
    uint8_t                   txTimes = 0;
    CanMatrix_TxList_Type    *p_msg   = NULL;
    CanMatrix_TxMsgInfo_Type *p_info  = NULL;

    p_msg                             = &g_CanMatrixTxList[index];
    p_info                            = &g_CanMatrixTxMsgInfo[index];
    if (p_info->timeTick >= p_msg->peroid)
    {
        p_info->txFlag   = TRUE;
        p_info->timeTick = CANMATRIX_TX_SCHEDULE_TIME;
    }
    else
    {
        p_info->timeTick += CANMATRIX_TX_SCHEDULE_TIME;
    }
    txTimes = p_msg->dataHandleFun(CANMATRIX_TX_STATE_CHECK);
    if (txTimes > p_info->txTimes)
    {
        p_info->txTimes = txTimes;
    }
    if (p_info->fastTick >= p_msg->fastPeroid)
    {
        if (p_info->txTimes > 0)
        {
            p_info->txTimes--;
            (void)p_msg->dataHandleFun(CANMATRIX_TX_STATE_UPDATE);
            (void)CanMatrix_WriteTxMsg(p_msg->frame, p_msg->id, p_msg->p_buff, p_msg->len);
            if (p_info->txTimes == 0)
            {
                (void)p_msg->dataHandleFun(CANMATRIX_TX_STATE_CLEAR);
            }
            p_info->fastTick = CANMATRIX_TX_SCHEDULE_TIME;
        }
        else if (p_info->txFlag == TRUE)
        {
            (void)p_msg->dataHandleFun(CANMATRIX_TX_STATE_UPDATE);
            (void)CanMatrix_WriteTxMsg(p_msg->frame, p_msg->id, p_msg->p_buff, p_msg->len);
            p_info->txFlag   = FALSE;
            p_info->fastTick = CANMATRIX_TX_SCHEDULE_TIME;
        }
    }
    else
    {
        p_info->fastTick += CANMATRIX_TX_SCHEDULE_TIME;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxMsgTask
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void CanMatrix_TxMsgTask(void)
{
    uint8_t i = 0;

    for (i = 0; i < CANMATRIX_TX_LIST_NUM; i++)
    {
        switch (g_CanMatrixTxList[i].type)
        {
            case EVENT:
                CanMatrix_TxEventMsg(i);
                break;
            case CYCLE:
                CanMatrix_TxCycleMsg(i);
                break;
            case CYCEV:
                CanMatrix_TxCycEvMsg(i);
                break;
            default:
                break;
        }
    }
}

/*******************************************************
  * Name    :CanMatrix_RxMsgMain
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanMatrix_RxMsgMain(void)
{
    while (TRUE)
    {
        if (CanMatrix_ReadRxMsg(&g_CanMatrixRxMsg.frame, &g_CanMatrixRxMsg.id, g_CanMatrixRxMsg.buff, &g_CanMatrixRxMsg.len) == TRUE)
        {
            CanMatrix_ChechRxMsg(&g_CanMatrixRxMsg);
        }
        else
        {
            break;
        }
    }
    CanMatrix_CheckRxIfLost();
    CanMatrix_CheckDtcIfLost();
}

/*******************************************************
  * Name    :CanMatrix_TxMsgMain
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanMatrix_TxMsgMain(void)
{
    static CanMatrix_State_Type canMatrixState = CANMATRIX_STATE_INIT;

    switch (canMatrixState)
    {
        case CANMATRIX_STATE_INIT:
            if ((CanMatrix_CheckStartAppTimerIsOk() == TRUE)
                && (CanMatrix_CheckNMIfAllowTxMsg() == TRUE))
            {
                canMatrixState = CANMATRIX_STATE_NORMAL;
            }
            break;
        case CANMATRIX_STATE_NORMAL:
            CanMatrix_TxMsgTask();
            if ((CanMatrix_CheckStartAppTimerIsOk() == FALSE)
                || (CanMatrix_CheckNMIfAllowTxMsg() == FALSE))
            {
                CanMatrix_ClearTxMsgTimer();
                canMatrixState = CANMATRIX_STATE_INIT;
            }
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrixRxMsgTask_Entry
  * Author  :xiaojunzhong
  * Function:Create CanMatrixRxMsgTask with real-time RTOS priority.
  * Version :V1.0.0
  * Data    :2026.10.21
*******************************************************/
static void CanMatrixRxMsgTask_Entry(void const *argument)
{
    (void)argument;
    Hmi_ShowTaskStarted("CanMatrixRxMsgTask");

    for (;;)
    {
        CanMatrix_RxMsgMain();
        osDelay(10U);
    }
}

/*******************************************************
  * Name    :CanManageTask_Create
  * Author  :xiaojunzhong
  * Function:Create CanMatrixRxMsgTask with real-time RTOS priority.
  * Version :V1.0.0
  * Data    :2026.10.21
*******************************************************/
void CanMatrixRxMsgTask_Create(void)
{
    osThreadDef(CanMatrixRxMsgTask, CanMatrixRxMsgTask_Entry, osPriorityHigh, 0U, 512U);
    (void)osThreadCreate(osThread(CanMatrixRxMsgTask), NULL);
}


/*******************************************************
  * Name    :CanMatrix_ProcessMsdFrame
  * Author  :xiaojunzhong
  * Function:匹配并缓存三项车辆帧；SRS无回调，由CanService唯一发布。
  * Version :V1.2.0
  * Data    :2026.11.07
  * Caller  :CanTask经CanService_ProcessRx调用。
  * Input   :沿用既有参数；接收表回调允许为空。
  * Output  :沿用既有返回约定，不新增业务成功结论。
  * Precond :共享RTE及CAN服务已完成启动初始化。
  * Failure :无回调项只保留缓存/匹配行为，不能调用空函数指针。
  * Effect  :保持匹配成功返回一，使CanService继续完成SRS发布与触发；NULL不表示格式失败。
*******************************************************/
uint8_t CanMatrix_ProcessMsdFrame(uint32_t u32Id, uint8_t u8Fd, const uint8_t *pu8Payload, uint8_t u8Length)
{
    uint8_t u8Index;
    Frame_Type enFrame;
    uint8_t u8Byte;

    if ((pu8Payload == NULL)
        || (u8Length != 8U)
        || (u8Fd > 1U)
        || (CanMatrix_IsMsdRxId(u32Id) == 0U))
    {
        return 0U;
    }
    enFrame = (u8Fd != 0U) ? CANFD : CAN;
    for (u8Index = 0U; u8Index < CANMATRIX_RX_LIST_NUM; u8Index++)
    {
        if ((g_CanMatrixRxList[u8Index].id == u32Id)
            && (g_CanMatrixRxList[u8Index].frame == enFrame)
            && (g_CanMatrixRxList[u8Index].len == u8Length))
        {
            /* Repeated equal payloads are fresh observations too.
             * RTE age checks own MSD freshness on this ingress; legacy matrix
             * schedule/DTC state is not driven by a second periodic consumer.
             */
            for (u8Byte = 0U; u8Byte < u8Length; u8Byte++)
            {
                g_CanMatrixRxList[u8Index].p_buff[u8Byte] = pu8Payload[u8Byte];
            }
            if (g_CanMatrixRxList[u8Index].dataHandleFun != NULL)
            {
                g_CanMatrixRxList[u8Index].dataHandleFun(CANMATRIX_RX_STATE_NORMAL);
            }
            return 1U;
        }
    }
    return 0U;
}
