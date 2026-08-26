/*******************************************************
  * Name    :Hmi.h
  * Author  :LoH
  * Function:TBOX HMI log and status display interface.
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
#define HMI_LOG_GNSS_ENABLE           0U
#define HMI_LOG_MONITOR_ENABLE        0U
#define HMI_LOG_POWER_ENABLE          0U
#define HMI_LOG_APP_ENABLE            1U
#define HMI_LOG_TASK_ENABLE           1U

/* Initialize the HMI log buffer. */
void Hmi_Init(void);
/* Submit one log message, optionally with a module prefix. */
void Hmi_Log(const char *pcText);
void Hmi_LogWithPrefix(const char *pcPrefix, const char *pcText);
/* Process one queued log message in AppTask context. */
void Hmi_Process(void);
/* Show task startup and eCall state information. */
void Hmi_ShowTaskStarted(const char *pcTaskName);
void Hmi_ShowEcallState(EcallState_t enState);
void Hmi_ShowText(const char *pcText);

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

#if (HMI_LOG_GNSS_ENABLE != 0U)
#define HMI_LOG_GNSS(pcText)          Hmi_LogWithPrefix("[GNSS] ", pcText)
#else
#define HMI_LOG_GNSS(pcText)          do { } while (0)
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
