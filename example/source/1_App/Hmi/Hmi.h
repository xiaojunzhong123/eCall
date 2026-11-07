/*******************************************************
  * Name    :Hmi.h
  * Author  :xiaojunzhong
  * Function:多任务日志静态队列及 RTT 输出；不能代替乘员提示。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __HMI_H__
#define __HMI_H__

#include "Ecall.h"

/* Set to 0U to remove the corresponding log calls at compile time. */
#define HMI_LOG_ECALL_ENABLE          1U
#define HMI_LOG_CAN_ENABLE            0U
#define HMI_LOG_MODEM_ENABLE          1U
#define HMI_LOG_MONITOR_ENABLE        0U
#define HMI_LOG_POWER_ENABLE          0U
#define HMI_LOG_APP_ENABLE            1U
#define HMI_LOG_TASK_ENABLE           1U

/* 初始化 RTT 和静态日志环形队列，清零读写索引及丢弃计数。 */
/* 初始化 RTT 和静态日志环形队列，清零读写索引及丢弃计数。 */
void Hmi_Init(void);
/* 在临界区复制日志到固定槽；队列满则统计丢弃，不阻塞业务任务。 */
/* 在临界区复制日志到固定槽；队列满则统计丢弃，不阻塞业务任务。 */
void Hmi_Log(const char *pcText);
/* 组合模块前缀和正文后入队，避免多任务日志交叉。 */
void Hmi_LogWithPrefix(const char *pcPrefix, const char *pcText);
/* AppTask 每周期取出并输出一条日志，限制单周期输出开销。 */
/* AppTask 每周期取出并输出一条日志，限制单周期输出开销。 */
void Hmi_Process(void);
/* 构造任务启动日志，日志出现不等于该任务全部功能已验证。 */
/* 构造任务启动日志，日志出现不等于该任务全部功能已验证。 */
void Hmi_ShowTaskStarted(const char *pcTaskName);
void Hmi_ShowEcallState(EcallState_t enState);
/* 将文本提交到日志队列；日志不等同实际乘员显示。 */
void Hmi_ShowText(const char *pcText);
/* 转交手动取消窗口提示意图，当前物理提示实现仍是预留。 */
/* 转交手动取消窗口提示意图，当前物理提示实现仍是预留。 */
void Hmi_SetEcallCancelWindow(uint8_t u8Active);

#if (HMI_LOG_ECALL_ENABLE != 0U)
#define HMI_LOG_ECALL(pcText)         Hmi_LogWithPrefix("[ECALL] ", pcText)
#else
#define HMI_LOG_ECALL(pcText)         do { } while (0)
#endif

#if (HMI_LOG_CAN_ENABLE != 0U)
#define HMI_LOG_CAN(pcText)           Hmi_LogWithPrefix("[CAN] ", pcText)
#else
#define HMI_LOG_CAN(pcText)           do { } while (0)
#endif

#if (HMI_LOG_MODEM_ENABLE != 0U)
#define HMI_LOG_MODEM(pcText)         Hmi_LogWithPrefix("[MODEM] ", pcText)
#else
#define HMI_LOG_MODEM(pcText)         do { } while (0)
#endif


#if (HMI_LOG_MONITOR_ENABLE != 0U)
#define HMI_LOG_MONITOR(pcText)       Hmi_LogWithPrefix("[MONITOR] ", pcText)
#else
#define HMI_LOG_MONITOR(pcText)       do { } while (0)
#endif

#if (HMI_LOG_POWER_ENABLE != 0U)
#define HMI_LOG_POWER(pcText)         Hmi_LogWithPrefix("[POWER] ", pcText)
#else
#define HMI_LOG_POWER(pcText)         do { } while (0)
#endif

#if (HMI_LOG_APP_ENABLE != 0U)
#define HMI_LOG_APP(pcText)           Hmi_LogWithPrefix("[APP] ", pcText)
#else
#define HMI_LOG_APP(pcText)           do { } while (0)
#endif

#if (HMI_LOG_TASK_ENABLE != 0U)
#define HMI_LOG_TASK(pcText)          Hmi_LogWithPrefix("[TASK] ", pcText)
#else
#define HMI_LOG_TASK(pcText)          do { } while (0)
#endif

#endif
