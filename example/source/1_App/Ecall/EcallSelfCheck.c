/*******************************************************
  * Name    :EcallSelfCheck.c
  * Author  :xiaojunzhong
  * Function:APP 层十项精简自检，默认正常、来源待接入及故障灯请求。
  * Version :V1.0.0
  * Data    :2026.11.05
*******************************************************/
#include "EcallSelfCheck.h"
#include "OutputService.h"
#include "Hmi.h"
#include "FreeRTOS.h"
#include <string.h>

typedef enum
{
    ECALL_SELF_CHECK_PHASE_STARTUP = 0U, /* 首周期接收结果，不等待缺失来源。 */
    ECALL_SELF_CHECK_PHASE_MONITOR      /* 后续周期继续监测，故障不因阶段切换清除。 */
} EcallSelfCheckPhase_t;

typedef struct
{
    EcallSelfCheckPhase_t enPhase; /* AppTask 私有阶段，从初始化持续至本次运行结束。 */
} EcallSelfCheckRuntime_t;

/* 任务生产者写、AppTask 原子取走的结果邮箱，低十位有效。
 * 所有访问均由短临界区保护，禁止 ISR 调用，不用 volatile 代替同步。
 * 同批次同项故障优先；取走后清零，随后新结果留待下一轮消费。
 */
typedef struct
{
    uint16_t u16ReportedMask;      /* 本批次至少上报过一次结果的检查项。 */
    uint16_t u16FaultReportedMask; /* 本批次至少收到一次 FALSE 的检查项。 */
    uint8_t u8Initialized;         /* 初始化完成后为 TRUE，之前拒绝结果和查询。 */
} EcallSelfCheckMailbox_t;

static EcallSelfCheckRuntime_t s_ecScRun; /* AppTask 独占的阶段状态，无动态内存。 */
static EcallSelfCheckStatus_t s_ecScSts; //eCall上电10项自检
static EcallSelfCheckMailbox_t s_ecScMailbox; //自检错误mask

/*******************************************************
  * Name    :Ecall_SelfCheckApplyResult
  * Author  :xiaojunzhong
  * Function:将已确认结果写入本轮局部副本，不设置硬件或刷新其他检查项。
  * Version :V1.0.0
  * Data    :2026.11.05
  * Input   :合法检查项、TRUE/FALSE 和非空状态副本。
  * Output  :无返回值；仅改变对应布尔字段。
  * Caller  :AppTask 内部消费函数。
  * Precond :索引及布尔值已在上报接口校验。
  * Failure :非法索引不修改副本，不接受外部可写指针。
  * Effect  :只写调用者的局部状态，不访问共享邮箱。
*******************************************************/
static void Ecall_SelfCheckApplyResult(EcallSelfCheckItem_t enItem, uint8_t u8Normal, EcallSelfCheckStatus_t *pstStatus)
{
    switch (enItem)
    {
        case ECALL_SELF_CHECK_ECU:
            pstStatus->u8EcuNormal = u8Normal;
            break;
        case ECALL_SELF_CHECK_MOBILE_ANTENNA:
            pstStatus->u8MobileAntennaNormal = u8Normal;
            break;
        case ECALL_SELF_CHECK_MODEM:
            pstStatus->u8ModemNormal = u8Normal;
            break;
        case ECALL_SELF_CHECK_GNSS_ANTENNA:
            pstStatus->u8GnssAntennaNormal = u8Normal;
            break;
        case ECALL_SELF_CHECK_GNSS_RECEIVER:
            pstStatus->u8GnssReceiverNormal = u8Normal;
            break;
        case ECALL_SELF_CHECK_CRASH_ECU:
            pstStatus->u8CrashEcuNormal = u8Normal;
            break;
        case ECALL_SELF_CHECK_COMMUNICATION:
            pstStatus->u8CommunicationNormal = u8Normal;
            break;
        case ECALL_SELF_CHECK_SIM:
            pstStatus->u8SimPresent = u8Normal;
            break;
        case ECALL_SELF_CHECK_POWER_CONNECTION:
            pstStatus->u8PowerConnectionNormal = u8Normal;
            break;
        case ECALL_SELF_CHECK_POWER_ENERGY:
            pstStatus->u8PowerEnergySufficient = u8Normal;
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :Ecall_SelfCheckUpdate
  * Author  :xiaojunzhong
  * Function:消费本批次结果，保持未更新故障，并向服务提交汇总灯请求。
  * Version :V1.0.0
  * Data    :2026.11.05
  * Input   :静态邮箱中任务生产者报告的已确认结果。
  * Output  :无返回值；发布完整状态并更新 D29 逻辑请求。
  * Caller  :AppTask，首次周期或持续监测阶段。
  * Precond :初始化已完成；来源无新结果时不生成替代健康报告。
  * Failure :尚无检测结果的项保持 TRUE 和待接入位；旧故障无结果时保持。
  * Effect  :清已取走邮箱、更新稳定状态，故障掩码变化时仅记录一次日志。
*******************************************************/
static void Ecall_SelfCheckUpdate(void)
{
    EcallSelfCheckStatus_t stNext; /* AppTask 本轮独占的稳定状态副本。 */
    uint16_t u16ReportedMask; /* 本轮取走的结果位，取走后生产者可继续上报。 */
    uint16_t u16FaultReportedMask; /* 本轮故障优先位，与结果位一起原子复制。 */
    uint16_t u16PreviousFaultMask; /* 发布前故障集合，用于单次转态日志。 */
    uint16_t u16ItemBit; /* 当前检查项位，仅使用低十位。 */
    uint8_t u8Index; /* 十项有界循环索引，单轮不等待任何检测完成。 */

    portENTER_CRITICAL();
    stNext = s_ecScSts;
    u16ReportedMask = s_ecScMailbox.u16ReportedMask;
    u16FaultReportedMask = s_ecScMailbox.u16FaultReportedMask;
    s_ecScMailbox.u16ReportedMask = 0U;
    s_ecScMailbox.u16FaultReportedMask = 0U;
    portEXIT_CRITICAL();

    u16PreviousFaultMask = stNext.u16FaultMask;
    for (u8Index = 0U; u8Index < (uint8_t)ECALL_SELF_CHECK_COUNT; u8Index++)
    {
        u16ItemBit = (uint16_t)(1U << u8Index);
        if ((u16ReportedMask & u16ItemBit) != 0U)
        {
            Ecall_SelfCheckApplyResult((EcallSelfCheckItem_t)u8Index,
                ((u16FaultReportedMask & u16ItemBit) != 0U) ? FALSE : TRUE, &stNext);
            stNext.u16PendingCheckMask &= (uint16_t)(~u16ItemBit);
        }
    }
    stNext.u16FaultMask = (uint16_t)((stNext.u16FaultMask & (uint16_t)(~u16ReportedMask)) | u16FaultReportedMask);
    portENTER_CRITICAL();
    s_ecScSts = stNext;
    portEXIT_CRITICAL();

    if (stNext.u16FaultMask != u16PreviousFaultMask)
    {
        HMI_LOG_ECALL((stNext.u16FaultMask != 0U)
            ? "SELF_CHECK_FAULT_MASK_CHANGED\r\n" : "SELF_CHECK_REPORTED_FAULTS_CLEARED\r\n");
    }
    /* D29 专用故障请求不受取消窗口和 KL15 OFF 影响，GPIO 留给输出服务。 */
    OutputService_SetEcallFault((stNext.u16FaultMask != 0U) ? TRUE : FALSE);
}

/*******************************************************
  * Name    :Ecall_SelfCheckEnterMonitor
  * Author  :xiaojunzhong
  * Function:首周期结束后进入持续监测，不清除检查结果和新到达邮箱。
  * Version :V1.0.0
  * Data    :2026.11.05
  * Input   :无。
  * Output  :无返回值。
  * Caller  :AppTask 首次周期。
  * Precond :已执行本轮结果消费，不等待未接入来源。
  * Failure :无超时或外设失败判定，不把阶段转换当作检查通过。
  * Effect  :只改变 AppTask 私有阶段，新阶段下一周期执行。
*******************************************************/
static void Ecall_SelfCheckEnterMonitor(void)
{
    s_ecScRun.enPhase = ECALL_SELF_CHECK_PHASE_MONITOR;
}

/*******************************************************
  * Name    :Ecall_SelfCheckProcessStartup
  * Author  :xiaojunzhong
  * Function:首周期处理已到达结果，不阻塞等待未知信号。
  * Version :V1.0.0
  * Data    :2026.11.05
  * Input   :无。
  * Output  :无返回值；保留待接入标记。
  * Caller  :AppTask，STARTUP 阶段。
  * Precond :自检与输出服务已初始化。
  * Failure :缺少来源仍保持默认正常，不标记真实检查通过。
  * Effect  :更新状态及灯请求，进入持续监测。
*******************************************************/
static void Ecall_SelfCheckProcessStartup(void)
{
    Ecall_SelfCheckUpdate();
    Ecall_SelfCheckEnterMonitor();
}

/*******************************************************
  * Name    :Ecall_SelfCheckProcessMonitor
  * Author  :xiaojunzhong
  * Function:持续处理上报结果，无新结果时保持已有状态和故障灯。
  * Version :V1.0.0
  * Data    :2026.11.05
  * Input   :无。
  * Output  :无返回值。
  * Caller  :AppTask，MONITOR 阶段。
  * Precond :来源仅报告已确认结果，不以重复轮询生成恢复结果。
  * Failure :失联或无结果不会自动消除旧故障。
  * Effect  :更新状态及 D29 请求，不发 AT、不访问 MCAL。
*******************************************************/
static void Ecall_SelfCheckProcessMonitor(void)
{
    Ecall_SelfCheckUpdate();
}

/*******************************************************
  * Name    :Ecall_SelfCheckInit
  * Author  :xiaojunzhong
  * Function:上电集中初始化十项默认正常，并标记全部检测来源待接入。
  * Version :V1.0.0
  * Data    :2026.11.05
  * Input   :无。
  * Output  :无返回值；十项 TRUE，待接入掩码为低十位全一。
  * Caller  :Ecall_Init，在任务创建前调用一次。
  * Precond :OutputService 已初始化；运行期不得用此接口清故障。
  * Failure :当前无真实检查生产者，不宣称完整自检通过。
  * Effect  :清私有状态和邮箱，设置默认值，提交 D29 熄灭请求。
*******************************************************/
void Ecall_SelfCheckInit(void)
{
    (void)memset(&s_ecScRun, 0, sizeof(s_ecScRun));
    (void)memset(&s_ecScSts, 0, sizeof(s_ecScSts));
    (void)memset(&s_ecScMailbox, 0, sizeof(s_ecScMailbox));
    s_ecScSts.u8EcuNormal = TRUE;
    s_ecScSts.u8MobileAntennaNormal = TRUE;
    s_ecScSts.u8ModemNormal = TRUE;
    s_ecScSts.u8GnssAntennaNormal = TRUE;
    s_ecScSts.u8GnssReceiverNormal = TRUE;
    s_ecScSts.u8CrashEcuNormal = TRUE;
    s_ecScSts.u8CommunicationNormal = TRUE;
    s_ecScSts.u8SimPresent = TRUE;
    s_ecScSts.u8PowerConnectionNormal = TRUE;
    s_ecScSts.u8PowerEnergySufficient = TRUE;
    s_ecScSts.u16PendingCheckMask = ECALL_SELF_CHECK_ALL_MASK;
    s_ecScRun.enPhase = ECALL_SELF_CHECK_PHASE_STARTUP;
    s_ecScMailbox.u8Initialized = TRUE;
    OutputService_SetEcallFault(FALSE);
}

/*******************************************************
  * Name    :Ecall_SelfCheckProcess
  * Author  :xiaojunzhong
  * Function:由顶层阶段分发有界自检处理，不等待外部响应。
  * Version :V1.0.0
  * Data    :2026.11.05
  * Input   :无。
  * Output  :无返回值；报警结果由状态查询读取。
  * Caller  :AppTask，在通用输出应用前调用。
  * Precond :Ecall_Init 已执行，初始化前不调度本函数。
  * Failure :不判断未确认阈值，不把待接入信号主动改为故障或真实通过。
  * Effect  :只执行当前阶段，新阶段下一周期执行。
*******************************************************/
void Ecall_SelfCheckProcess(void)
{
    switch (s_ecScRun.enPhase)
    {
        case ECALL_SELF_CHECK_PHASE_STARTUP:
            Ecall_SelfCheckProcessStartup();
            break;
        case ECALL_SELF_CHECK_PHASE_MONITOR:
            Ecall_SelfCheckProcessMonitor();
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :Ecall_SelfCheckReportResult
  * Author  :xiaojunzhong
  * Function:接收任务上下文确认结果到固定邮箱，同批故障优先于恢复。
  * Version :V1.0.0
  * Data    :2026.11.05
  * Input   :合法检查项及严格 TRUE/FALSE；检测来源待用户后续接入。
  * Output  :一为已接受，零为索引、布尔值非法或尚未初始化。
  * Caller  :后续检测服务的任务上下文，不允许 ISR 调用。
  * Precond :已完成检测及所需确认，恢复 TRUE 也必须有真实依据。
  * Failure :拒绝非法值；无数据时不要调用，以保留旧状态。
  * Effect  :只更新共享邮箱，不直接写稳定状态或驱动 LED。
*******************************************************/
uint8_t Ecall_SelfCheckReportResult(EcallSelfCheckItem_t enItem, uint8_t u8Normal)
{
    uint16_t u16ItemBit; /* 当前检查项位，合法索引校验后才移位。 */
    uint8_t u8Accepted = FALSE; /* 只表示邮箱受理，不表示实际检测或 LED 验证完成。 */
    if (((uint32_t)enItem >= (uint32_t)ECALL_SELF_CHECK_COUNT)
        || (u8Normal > (uint8_t)TRUE))
    {
        return FALSE;
    }
    u16ItemBit = (uint16_t)(1U << (uint32_t)enItem);
    portENTER_CRITICAL();
    if (s_ecScMailbox.u8Initialized == TRUE)
    {
        s_ecScMailbox.u16ReportedMask |= u16ItemBit;
        if (u8Normal == FALSE)
        {
            s_ecScMailbox.u16FaultReportedMask |= u16ItemBit;
        }
        u8Accepted = TRUE;
    }
    portEXIT_CRITICAL();
    return u8Accepted;
}

/*******************************************************
  * Name    :Ecall_SelfCheckReadStatus
  * Author  :xiaojunzhong
  * Function:复制完整自检状态，查询不清故障、不推进阶段或消费邮箱。
  * Version :V1.0.0
  * Data    :2026.11.05
  * Input   :非空接收结构体指针。
  * Output  :一为取得副本，零为空指针或尚未初始化。
  * Caller  :任意任务上下文，不允许 ISR 调用。
  * Precond :Ecall_Init 已建立初值，结果归调用者独占。
  * Failure :无效查询不修改调用者目标或模块状态。
  * Effect  :短临界区复制，不返回私有实例指针。
*******************************************************/
uint8_t Ecall_SelfCheckReadStatus(EcallSelfCheckStatus_t *pstStatus)
{
    uint8_t u8Ready = FALSE; /* 初始化资格，复制与资格读取处于同一临界区。 */
    if (pstStatus == NULL)
    {
        return FALSE;
    }
    portENTER_CRITICAL();
    if (s_ecScMailbox.u8Initialized == TRUE)
    {
        *pstStatus = s_ecScSts;
        u8Ready = TRUE;
    }
    portEXIT_CRITICAL();
    return u8Ready;
}
