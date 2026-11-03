/*******************************************************
  * Name    :TaskManager.c
  * Author  :xiaojunzhong
  * Function:按服务初始化、共享状态建立、任务创建的顺序启动 MCU 业务。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "CanService.h"
#include "Ecall.h"
#include "EcallDiag.h"
#include "EcallDiagService.h"
#include "EcallHmi.h"
#include "OutputService_Ecall.h"
#include "GnssService.h"
#include "Hmi.h"
#include "InputService.h"
#include "ModemPower.h"
#include "OutputService.h"
#include "PowerService.h"
#include "TaskManager.h"
#include "TimeService.h"
#include "Uds.h"
#include "CanMatrix.h"
#include "CanTp.h"
#include "CanManage.h"
#include "RteSys.h"

extern void EcallTask_Create(void);
extern void CanTask_Create(void);
extern void GnssTask_Create(void);
extern void MonitoringTask_Create(void);
extern void AppTask_Create(void);
extern void PowTask_Create(void);

/*******************************************************
  * Name    :TaskManager_Start
  * Author  :xiaojunzhong
  * Function:初始化服务及诊断提示接口后创建任务，所有初值在并发执行前建立。
  * Version :V1.1.0
  * Data    :2026.11.01
*******************************************************/
void TaskManager_Start(void)
{
    /* 一、建立共享数据和日志，任务创建前没有并发访问。 */
    RteSys_InitMsdSignals();
    Hmi_Init();
    PowerService_Init();
    GnssService_Init();
    TimeService_Init();
    CanService_Init();
    InputService_Init();
    OutputService_Init();
    ModemPower_Init();
    Ecall_Init();
    /* 二、建立自检代次及乘员提示接口；缺少真实检测项保持不支持。 */
    RteSys_InitDiagnostics();
    Ecall_DiagInit();
    EcallDiagService_Init();
    OutputService_InitEcallIndication();
    Ecall_HmiInit();
	Uds_Init();
	CanMatrix_Init();

    /* 三、创建十个保留任务；无 AT、拨号和以太网任务。
     * PowTask 管电源，AppTask 管输入/自检/提示，EcallTask 管事件仲裁。
     * CAN/诊断原有任务保留；其时基和实板闭环仍需单独验收。 */
    PowTask_Create();
    GnssTask_Create();
    CanTask_Create();
    MonitoringTask_Create();
    AppTask_Create();
    EcallTask_Create();
	UdsTask_Create();
	CanTpTask_Create();
	CanManageTask_Create();
	CanMatrixRxMsgTask_Create();
}
