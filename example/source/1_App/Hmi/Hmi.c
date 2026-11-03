/*******************************************************
  * Name    :Hmi.c
  * Author  :xiaojunzhong
  * Function:多任务日志静态队列及 RTT 输出；不能代替乘员提示。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "FreeRTOS.h"
#include "Hmi.h"
#include "OutputService.h"
#include "SEGGER_RTT.h"
#include "cmsis_os.h"

#define HMI_LOG_SLOT_COUNT             32U
#define HMI_LOG_SLOT_LENGTH            64U
#define HMI_TIMESTAMP_LENGTH            14U

typedef struct
{
    uint8_t au8Text[HMI_LOG_SLOT_LENGTH]; /* 日志文字的固定容量副本。 */
    uint8_t u8Length; /* 当前有效字节数。 */
    uint32_t u32TimestampMs; /* 日志入队时的单调毫秒时刻。 */
} HmiLogSlot_t;


static HmiLogSlot_t m_astLogSlot[HMI_LOG_SLOT_COUNT]; /* 固定日志槽，业务任务写入，AppTask 消费。 */
static uint8_t m_u8ReadIndex = 0U; /* 日志环形队列读取位置。 */
static uint8_t m_u8WriteIndex = 0U; /* 日志环形队列写入位置。 */
static uint8_t m_u8LogCount = 0U; /* 当前日志槽占用数。 */
static uint16_t m_u16DropCount = 0U; /* 因队列已满而丢弃的日志数量。 */

/*******************************************************
  * Name    :Hmi_Write
  * Author  :xiaojunzhong
  * Function:通过 RTT 输出一条日志；只由日志消费任务调用，不直接占用模组串口。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void Hmi_Write(const uint8_t *pu8Text, uint8_t u8Length)
{
    (void)SEGGER_RTT_Write(0U, pu8Text, (unsigned int)u8Length);
}

/*******************************************************
  * Name    :Hmi_WriteTimestamp
  * Author  :xiaojunzhong
  * Function:将单调毫秒时间格式化为秒和毫秒，输出日志时间前缀。
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void Hmi_WriteTimestamp(uint32_t u32TimestampMs)
{
    uint8_t au8Timestamp[HMI_TIMESTAMP_LENGTH]; /* 日志时间前缀字符数组。 */
    uint32_t u32Seconds; /* 秒分量或待推进的秒数。 */
    uint32_t u32Milliseconds; /* 一秒内的毫秒分量。 */
    uint32_t u32Divisor; /* 十进制逐位转换的除数。 */
    uint8_t u8Index; /* 当前数组或通道索引。 */

    u32Seconds = u32TimestampMs / 1000U;
    u32Milliseconds = u32TimestampMs % 1000U;
    au8Timestamp[0U] = '[';
    u32Divisor = 1000000U;
    u8Index = 1U;
    while (u8Index < 8U)
    {
        au8Timestamp[u8Index] = (uint8_t)('0' + (u32Seconds / u32Divisor));
        u32Seconds %= u32Divisor;
        u32Divisor /= 10U;
        u8Index++;
    }
    au8Timestamp[8U] = '.';
    au8Timestamp[9U] = (uint8_t)('0' + (u32Milliseconds / 100U));
    u32Milliseconds %= 100U;
    au8Timestamp[10U] = (uint8_t)('0' + (u32Milliseconds / 10U));
    au8Timestamp[11U] = (uint8_t)('0' + (u32Milliseconds % 10U));
    au8Timestamp[12U] = ']';
    au8Timestamp[13U] = ' ';
    Hmi_Write(au8Timestamp, HMI_TIMESTAMP_LENGTH);
}

/*******************************************************
  * Name    :Hmi_Init
  * Author  :xiaojunzhong
  * Function:初始化 RTT 和静态日志环形队列，清零读写索引及丢弃计数。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_Init(void)
{
    SEGGER_RTT_Init();
    m_u8ReadIndex = 0U;
    m_u8WriteIndex = 0U;
    m_u8LogCount = 0U;
    m_u16DropCount = 0U;
}

/*******************************************************
  * Name    :Hmi_Log
  * Author  :xiaojunzhong
  * Function:在临界区复制日志到固定槽；队列满则统计丢弃，不阻塞业务任务。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_Log(const char *pcText)
{
    uint8_t u8Length = 0U; /* 当前有效字节数。 */
    uint8_t u8Index; /* 当前数组或通道索引。 */

    if (pcText == 0)
    {
        return;
    }

    while ((pcText[u8Length] != '\0') && (u8Length < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        u8Length++;
    }

    portENTER_CRITICAL();
    if (m_u8LogCount >= HMI_LOG_SLOT_COUNT)
    {
        m_u16DropCount++;
    }
    else
    {
        u8Index = 0U;
        while (u8Index < u8Length)
        {
            m_astLogSlot[m_u8WriteIndex].au8Text[u8Index] = (uint8_t)pcText[u8Index];
            u8Index++;
        }
        m_astLogSlot[m_u8WriteIndex].au8Text[u8Length] = '\0';
        m_astLogSlot[m_u8WriteIndex].u8Length = u8Length;
        m_astLogSlot[m_u8WriteIndex].u32TimestampMs = osKernelSysTick();
        m_u8WriteIndex++;
        if (m_u8WriteIndex >= HMI_LOG_SLOT_COUNT)
        {
            m_u8WriteIndex = 0U;
        }
        m_u8LogCount++;
    }
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :Hmi_LogWithPrefix
  * Author  :xiaojunzhong
  * Function:组合模块前缀和正文后入队，避免多任务日志交叉。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_LogWithPrefix(const char *pcPrefix, const char *pcText)
{
    uint8_t au8Text[HMI_LOG_SLOT_LENGTH]; /* 日志文字的固定容量副本。 */
    uint8_t u8Index = 0U; /* 当前数组或通道索引。 */

    if ((pcPrefix == 0) || (pcText == 0))
    {
        return;
    }

    while ((*pcPrefix != '\0') && (u8Index < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        au8Text[u8Index++] = (uint8_t)*pcPrefix++;
    }
    while ((*pcText != '\0') && (u8Index < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        au8Text[u8Index++] = (uint8_t)*pcText++;
    }
    au8Text[u8Index] = '\0';
    Hmi_Log((const char *)au8Text);
}

/*******************************************************
  * Name    :Hmi_Process
  * Author  :xiaojunzhong
  * Function:AppTask 每周期取出并输出一条日志，限制单周期输出开销。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_Process(void)
{
    uint8_t au8Text[HMI_LOG_SLOT_LENGTH]; /* 日志文字的固定容量副本。 */
    uint8_t u8Length = 0U; /* 当前有效字节数。 */
    uint8_t u8Index; /* 当前数组或通道索引。 */
    uint32_t u32TimestampMs = 0U; /* 日志入队时的单调毫秒时刻。 */

    portENTER_CRITICAL();
    if (m_u8LogCount != 0U)
    {
        u8Length = m_astLogSlot[m_u8ReadIndex].u8Length;
        u32TimestampMs = m_astLogSlot[m_u8ReadIndex].u32TimestampMs;
        u8Index = 0U;
        while (u8Index < u8Length)
        {
            au8Text[u8Index] = m_astLogSlot[m_u8ReadIndex].au8Text[u8Index];
            u8Index++;
        }
        m_u8ReadIndex++;
        if (m_u8ReadIndex >= HMI_LOG_SLOT_COUNT)
        {
            m_u8ReadIndex = 0U;
        }
        m_u8LogCount--;
    }
    portEXIT_CRITICAL();

    if (u8Length != 0U)
    {
        Hmi_WriteTimestamp(u32TimestampMs);
        Hmi_Write(au8Text, u8Length);
    }
}

/*******************************************************
  * Name    :Hmi_ShowTaskStarted
  * Author  :xiaojunzhong
  * Function:构造任务启动日志，日志出现不等于该任务全部功能已验证。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_ShowTaskStarted(const char *pcTaskName)
{
#if (HMI_LOG_TASK_ENABLE != 0U)
    uint8_t au8Text[HMI_LOG_SLOT_LENGTH]; /* 日志文字的固定容量副本。 */
    uint8_t u8Index = 0U; /* 当前数组或通道索引。 */
    const char *pcSuffix = " started\r\n";

    if (pcTaskName == 0)
    {
        return;
    }

    while ((*pcTaskName != '\0') && (u8Index < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        au8Text[u8Index++] = (uint8_t)*pcTaskName++;
    }
    while ((*pcSuffix != '\0') && (u8Index < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        au8Text[u8Index++] = (uint8_t)*pcSuffix++;
    }
    au8Text[u8Index] = '\0';
    HMI_LOG_TASK((const char *)au8Text);
#else
    (void)pcTaskName;
#endif
}

/*******************************************************
  * Name    :Hmi_ShowEcallState
  * Author  :xiaojunzhong
  * Function:输出 MCU 事件阶段；数据交付完成不代表模组呼叫完成。
  * Version :V1.1.0
  * Data    :2026.11.01
*******************************************************/
void Hmi_ShowEcallState(EcallState_t enState)
{
#if (HMI_LOG_ECALL_ENABLE != 0U)
    if (enState == ECALL_STATE_STANDBY)
    {
        HMI_LOG_ECALL("STANDBY\r\n");
    }
    else if (enState == ECALL_STATE_MANUAL_CANCEL_WINDOW)
    {
        HMI_LOG_ECALL("MANUAL_CANCEL_WINDOW\r\n");
    }
    else if (enState == ECALL_STATE_DATA_PENDING)
    {
        HMI_LOG_ECALL("DATA_PENDING\r\n");
    }
    else if (enState == ECALL_STATE_COMPLETE)
    {
        HMI_LOG_ECALL("COMPLETE\r\n");
    }
    else if (enState == ECALL_STATE_FAILED)
    {
        HMI_LOG_ECALL("FAILED\r\n");
    }
    else
    {
        HMI_LOG_ECALL("UNKNOWN\r\n");
    }
#else
    (void)enState;
#endif
}

/*******************************************************
  * Name    :Hmi_ShowText
  * Author  :xiaojunzhong
  * Function:将文本提交到日志队列；日志不等同实际乘员显示。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_ShowText(const char *pcText)
{
    Hmi_Log(pcText);
}

/*******************************************************
  * Name    :Hmi_SetEcallCancelWindow
  * Author  :xiaojunzhong
  * Function:转交手动取消窗口提示意图，当前物理提示实现仍是预留。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_SetEcallCancelWindow(uint8_t u8Active)
{
    OutputService_SetEcallCancelWindow(u8Active);
}
