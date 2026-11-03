/*******************************************************
  * Name    :CanService.c
  * Author  :xiaojunzhong
  * Function:CAN 碰撞接收、车辆数据发布、时间发送及总线错误恢复。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
/* 流程阅读说明：
 * 执行路径：CAN ISR -> 驱动环形队列 -> CanTask -> ProcessRx -> 矩阵三报文回调 -> RTE。
 * 碰撞位上升沿锁存后由 CanTask 转交 EcallTask；ISR 不执行应用状态机。
 * 当前只路由已接入的三项数据，其他诊断帧路由和完整性校验仍待独立完善。
 * 已删除 MCU 通话状态计时及旧 0x21C 周期发送，不编造模组呼叫状态。
 */

#include "CanService.h"
#include "Can.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Hmi.h"
#include "TimeService.h"
#include "CanMatrix.h"
#include <string.h>

#define CAN_SERVICE_SRS_INFO_ID                0x10CU
#define CAN_SERVICE_TIME_ID                   0x2A3U
#define CAN_SERVICE_FRAME_LENGTH              8U
#define CAN_SERVICE_TIME_TX_PERIOD_MS         100U
#define CAN_SERVICE_TIME_TX_PHASE_MS          50U
#define CAN_SERVICE_LOCAL_UTC_OFFSET_HOURS     8U
#define CAN_SERVICE_BUS_OFF_RECOVERY_MS       1000U
#define CAN_SERVICE_TX_ERROR_REARM_MS          1000U

typedef struct
{
    uint32_t u32LastTimeTxMs; /* 上次时间报文发送时刻，毫秒。 */
    uint32_t u32BusOffDetectedMs; /* 本次总线关闭故障的检测时刻，毫秒。 */
    uint8_t u8CrashArmed; /* 收到零后置一，碰撞上升沿锁存后清零。 */
    uint8_t u8TriggerPending; /* 尚未由 CanTask 取走的碰撞触发位。 */
    uint8_t u8BusOffRecoveryPending; /* 是否等待任务上下文恢复 CAN。 */
} CanServiceRuntime_t;

typedef struct
{
    uint32_t u32LastRxOverflowCount; /* 上次已经报告的接收溢出计数。 */
    uint32_t u32LastTxErrorCount; /* 上次已经报告的发送错误计数。 */
    uint32_t u32LastTxErrorMs; /* 最近一次发送错误日志时刻，毫秒。 */
    uint8_t u8TxErrorReported; /* 发送错误日志的抑制标志。 */
    uint8_t u8TimeTxFailed; /* 时间报文发送失败锁存，仅变化时记录。 */
} CanServiceStatus_t;

/*******************************************************
  * Name    :CanService_ReportTxResult
  * Author  :xiaojunzhong
  * Function:发送失败及恢复时各记录一次，避免周期日志淹没业务。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void CanService_ReportTxResult(uint8_t u8Success, uint8_t *pu8FailureLatched,
                                      const char *pcFailureLog, const char *pcRecoveryLog)
{
    if (u8Success == 0U)
    {
        if (*pu8FailureLatched == 0U)
        {
            *pu8FailureLatched = 1U;
            HMI_LOG_CAN(pcFailureLog);
        }
    }
    else if (*pu8FailureLatched != 0U)
    {
        *pu8FailureLatched = 0U;
        HMI_LOG_CAN(pcRecoveryLog);
    }
}

static CanServiceRuntime_t s_mCanSvcRun; /* CanTask 拥有的接收和恢复过程。 */
static CanServiceStatus_t s_mCanSvcSts; /* 总线错误及发送统计结论。 */

/*******************************************************
  * Name    :CanService_IsSrsInfoFrame
  * Author  :xiaojunzhong
  * Function:确认 0x10C 为标准经典 CAN 八字节帧，拒绝格式不符的碰撞输入。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static uint8_t CanService_IsSrsInfoFrame(const CanRxFrame_t *pstFrame)
{
    if ((pstFrame == NULL)
        || (pstFrame->u32Id != CAN_SERVICE_SRS_INFO_ID)
        || (pstFrame->u8Ide != 0U)
        || (pstFrame->u8Fdf != 0U)
        || (pstFrame->u8Brs != 0U)
        || (pstFrame->u8Length != CAN_SERVICE_FRAME_LENGTH))
    {
        return 0U;
    }
    return 1U;
}

/*******************************************************
  * Name    :CanService_HandleSrsInfoFrame
  * Author  :xiaojunzhong
  * Function:对碰撞位锁存一次上升沿；收到零后重新使能，持续非零不重复触发。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static void CanService_HandleSrsInfoFrame(const CanRxFrame_t *pstFrame)
{
    CanServiceSrsInfoFrame_t stSrsInfo; /* 八字节碰撞报文的本地解析视图。 */
    uint8_t u8Crash; /* 碰撞观测位，置位本身不表示传感器故障。 */
    uint8_t u8TriggerLatched = 0U; /* 本帧是否刚锁存一次碰撞，用于一次性日志。 */

    if (CanService_IsSrsInfoFrame(pstFrame) == 0U)
    {
        HMI_LOG_CAN("RX_10C_FORMAT_REJECTED\r\n");
        return;
    }

    (void)memcpy(stSrsInfo.U, pstFrame->au8Data, CAN_SERVICE_FRAME_LENGTH);
    u8Crash = (stSrsInfo.B.u8CrashOutputStatus != 0U) ? 1U : 0U;
    portENTER_CRITICAL();
    /* SRS_Info defines only the crash state; MSD impact fields remain unknown. */

    if ((u8Crash != 0U) && (s_mCanSvcRun.u8CrashArmed != 0U))
    {
        s_mCanSvcRun.u8CrashArmed = 0U;
        s_mCanSvcRun.u8TriggerPending = 1U;
        u8TriggerLatched = 1U;
    }
    else if (u8Crash == 0U)
    {
        s_mCanSvcRun.u8CrashArmed = 1U;
    }
    portEXIT_CRITICAL();

    if (u8TriggerLatched != 0U)
    {
        HMI_LOG_CAN("RX_10C_ECALL_TRIGGER\r\n");
    }
}

/*******************************************************
  * Name    :CanService_ProcessRx
  * Author  :xiaojunzhong
  * Function:唯一消费硬件接收队列；先通过矩阵回调发布三项数据，再锁存碰撞事件。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
static void CanService_ProcessRx(void)
{
    CanRxFrame_t stFrame; /* 从 ISR 环形队列复制出来的完整帧。 */

    while (Can_TakeRxFrame(&stFrame) != 0U)
    {
        if ((stFrame.u8Ide != 0U)
            || ((stFrame.u8Fdf == 0U) && (stFrame.u8Brs != 0U)))
        {
            continue;
        }
        if (CanMatrix_ProcessMsdFrame(stFrame.u32Id, stFrame.u8Fdf, stFrame.au8Data, stFrame.u8Length) != 0U)
        {
            if (stFrame.u32Id == CAN_SERVICE_SRS_INFO_ID)
            {
                /* Matrix has already published this exact crash observation. */
                CanService_HandleSrsInfoFrame(&stFrame);
            }
        }
    }
}


/*******************************************************
  * Name    :CanService_IsLeapYear
  * Author  :xiaojunzhong
  * Function:判断公历闰年，供时间报文转换使用。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t CanService_IsLeapYear(uint16_t u16Year)
{
    return (((u16Year % 4U) == 0U) && (((u16Year % 100U) != 0U) || ((u16Year % 400U) == 0U))) ? 1U : 0U;
}

/*******************************************************
  * Name    :CanService_GetMonthDays
  * Author  :xiaojunzhong
  * Function:根据年月返回天数；非法月份返回零。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t CanService_GetMonthDays(uint16_t u16Year, uint8_t u8Month)
{
    static const uint8_t au8MonthDays[12] = {31U, 28U, 31U, 30U, 31U, 30U,
                                              31U, 31U, 30U, 31U, 30U, 31U};

    if ((u8Month == 0U) || (u8Month > 12U))
    {
        return 0U;
    }

    return (uint8_t)(au8MonthDays[u8Month - 1U]
                     + (((u8Month == 2U) && (CanService_IsLeapYear(u16Year) != 0U)) ? 1U : 0U));
}

/*******************************************************
  * Name    :CanService_ConvertUtcToLocal
  * Author  :xiaojunzhong
  * Function:将来源 UTC 转换为现有 CAN 本地时间配置，保持既有时区策略。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void CanService_ConvertUtcToLocal(TimeServiceTime_t *pstTime)
{
    uint16_t u16Year; /* 完整公历年份。 */

    pstTime->u8Hour = (uint8_t)(pstTime->u8Hour + CAN_SERVICE_LOCAL_UTC_OFFSET_HOURS);
    if (pstTime->u8Hour < 24U)
    {
        return;
    }

    pstTime->u8Hour -= 24U;
    pstTime->u8Day++;
    u16Year = (uint16_t)(2000U + pstTime->u8Year);
    if (pstTime->u8Day <= CanService_GetMonthDays(u16Year, pstTime->u8Month))
    {
        return;
    }

    pstTime->u8Day = 1U;
    pstTime->u8Month++;
    if (pstTime->u8Month <= 12U)
    {
        return;
    }

    pstTime->u8Month = 1U;
    pstTime->u8Year++;
}



/*******************************************************
  * Name    :CanService_BuildTimeFrame
  * Author  :xiaojunzhong
  * Function:按现有矩阵比例编码时间；没有有效来源时保留无效值。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void CanService_BuildTimeFrame(uint8_t *pu8Data)
{
    CanServiceTimeFrame_t stTimeFrame; /* 现有时间矩阵的本地编码视图。 */
    TimeServiceTime_t stUtcTime; /* 时间服务提供的有效 UTC 副本。 */
    uint16_t u16Year; /* 完整公历年份。 */

    (void)memset(&stTimeFrame, 0, sizeof(stTimeFrame));
    if (TimeService_GetUtc(&stUtcTime) == TIME_SERVICE_SOURCE_INVALID)
    {
        (void)memcpy(pu8Data, stTimeFrame.U, CAN_SERVICE_FRAME_LENGTH);
        return;
    }

    CanService_ConvertUtcToLocal(&stUtcTime);
    u16Year = (uint16_t)(2000U + stUtcTime.u8Year);
    stTimeFrame.B.u8YearHigh = (uint8_t)(u16Year >> 8U);
    stTimeFrame.B.u8YearLow = (uint8_t)u16Year;
    stTimeFrame.B.u8Month = stUtcTime.u8Month;
    stTimeFrame.B.u8DayRaw = (uint8_t)(stUtcTime.u8Day * 4U);
    stTimeFrame.B.u8Hour = stUtcTime.u8Hour;
    stTimeFrame.B.u8Minute = stUtcTime.u8Minute;
    stTimeFrame.B.u8SecondRaw = (uint8_t)(stUtcTime.u8Second * 4U);
    (void)memcpy(pu8Data, stTimeFrame.U, CAN_SERVICE_FRAME_LENGTH);
}

/*******************************************************
  * Name    :CanService_ProcessTimeTx
  * Author  :xiaojunzhong
  * Function:按现有一百毫秒周期发送时间帧，并集中报告发送异常。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void CanService_ProcessTimeTx(void)
{
    uint8_t au8Data[CAN_SERVICE_FRAME_LENGTH]; /* 本次发送的有界报文字节数组。 */
    uint32_t u32NowMs; /* 当前单调毫秒计数，用无符号差值处理回绕。 */

    u32NowMs = osKernelSysTick();
    portENTER_CRITICAL();
    if ((u32NowMs - s_mCanSvcRun.u32LastTimeTxMs) < CAN_SERVICE_TIME_TX_PERIOD_MS)
    {
        portEXIT_CRITICAL();
        return;
    }
    s_mCanSvcRun.u32LastTimeTxMs = u32NowMs;
    portEXIT_CRITICAL();

    CanService_BuildTimeFrame(au8Data);
    CanService_ReportTxResult(Can_TxMsg(CANFD, CAN_SERVICE_TIME_ID, au8Data,
                                        CAN_SERVICE_FRAME_LENGTH), &s_mCanSvcSts.u8TimeTxFailed,
                              "TX_2A3_FAILED\r\n", "TX_2A3_RECOVERED\r\n");
}

/*******************************************************
  * Name    :CanService_ProcessErrors
  * Author  :xiaojunzhong
  * Function:在任务上下文记录溢出/发送错误，执行既有 Bus-Off 延迟恢复。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static void CanService_ProcessErrors(void)
{
    uint32_t u32NowMs; /* 当前单调毫秒计数，用无符号差值处理回绕。 */
    uint32_t u32RxOverflowCount; /* 底层累计接收溢出数量。 */
    uint32_t u32TxErrorCount; /* 底层累计发送错误数量。 */

    u32NowMs = osKernelSysTick();
    u32RxOverflowCount = Can_GetRxOverflowCount();
    u32TxErrorCount = Can_GetTxErrorCount();

    if (u32RxOverflowCount != s_mCanSvcSts.u32LastRxOverflowCount)
    {
        s_mCanSvcSts.u32LastRxOverflowCount = u32RxOverflowCount;
        HMI_LOG_CAN("RX_OVERFLOW\r\n");
    }
    if (u32TxErrorCount != s_mCanSvcSts.u32LastTxErrorCount)
    {
        s_mCanSvcSts.u32LastTxErrorCount = u32TxErrorCount;
        s_mCanSvcSts.u32LastTxErrorMs = u32NowMs;
        if (s_mCanSvcSts.u8TxErrorReported == 0U)
        {
            s_mCanSvcSts.u8TxErrorReported = 1U;
            HMI_LOG_CAN("TX_ERROR\r\n");
        }
    }
    else if ((s_mCanSvcSts.u8TxErrorReported != 0U)
             && ((u32NowMs - s_mCanSvcSts.u32LastTxErrorMs) >= CAN_SERVICE_TX_ERROR_REARM_MS))
    {
        s_mCanSvcSts.u8TxErrorReported = 0U;
    }

    if ((Can_GetBusOffFlag() != 0U) && (s_mCanSvcRun.u8BusOffRecoveryPending == 0U))
    {
        s_mCanSvcRun.u8BusOffRecoveryPending = 1U;
        s_mCanSvcRun.u32BusOffDetectedMs = u32NowMs;
        HMI_LOG_CAN("BUS_OFF\r\n");
    }
    if ((s_mCanSvcRun.u8BusOffRecoveryPending != 0U)
        && ((u32NowMs - s_mCanSvcRun.u32BusOffDetectedMs) >= CAN_SERVICE_BUS_OFF_RECOVERY_MS))
    {
        Net_Can_DeInit();
        Can_Init();
        s_mCanSvcRun.u8BusOffRecoveryPending = 0U;
        s_mCanSvcSts.u32LastRxOverflowCount = 0U;
        s_mCanSvcSts.u32LastTxErrorCount = 0U;
        s_mCanSvcSts.u8TxErrorReported = 0U;
        HMI_LOG_CAN("BUS_OFF_RECOVERY\r\n");
    }
}

/*******************************************************
  * Name    :CanService_Init
  * Author  :xiaojunzhong
  * Function:初始化 CAN 服务私有缓存和碰撞重新使能标志，启动已有 CAN2 配置。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void CanService_Init(void)
{
    (void)memset(&s_mCanSvcRun, 0, sizeof(s_mCanSvcRun));
    (void)memset(&s_mCanSvcSts, 0, sizeof(s_mCanSvcSts));
    s_mCanSvcRun.u8CrashArmed = 1U;
    s_mCanSvcRun.u32LastTimeTxMs = osKernelSysTick()
                                  - (CAN_SERVICE_TIME_TX_PERIOD_MS - CAN_SERVICE_TIME_TX_PHASE_MS);
    Can_Init();
    HMI_LOG_CAN("CAN2_FD_500K_2M_INIT\r\n");
}

/*******************************************************
  * Name    :CanService_Process
  * Author  :xiaojunzhong
  * Function:按接收、错误恢复、时间发送的顺序处理；不维护模组通话状态。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void CanService_Process(void)
{
    CanService_ProcessRx();
    CanService_ProcessErrors();
    if (s_mCanSvcRun.u8BusOffRecoveryPending == 0U)
    {
        CanService_ProcessTimeTx();
    }
}

/*******************************************************
  * Name    :CanService_TakeEcallTrigger
  * Author  :xiaojunzhong
  * Function:原子取走一次碰撞锁存位；由 CanTask 转交 EcallTask。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
uint8_t CanService_TakeEcallTrigger(void)
{
    uint8_t u8Trigger; /* 本周期取走的一次碰撞意图。 */

    portENTER_CRITICAL();
    u8Trigger = s_mCanSvcRun.u8TriggerPending;
    s_mCanSvcRun.u8TriggerPending = 0U;
    portEXIT_CRITICAL();
    return u8Trigger;
}
