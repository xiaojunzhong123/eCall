/*******************************************************
  * Name    :TaskManager.c
  * Author  :xiaojunzhong
  * Function:按服务初始化、共享状态建立、任务创建的顺序启动 MCU 业务。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "CanService.h"
#include "Did.h"
#include "Ecall.h"
#include "Hmi.h"
#include "InputService.h"
#include "ModemPower.h"
#include "OutputService.h"
#include "PowerService.h"
#include "TaskManager.h"
#include "Uds.h"
#include "CanMatrix.h"
#include "CanTp.h"
#include "CanManage.h"
#include "RteSys.h"

extern void EcallTask_Create(void);
extern void CanTask_Create(void);
extern void MonitoringTask_Create(void);
extern void AppTask_Create(void);
extern void PowTask_Create(void);

/*******************************************************
  * Name    :TaskManager_Start
  * Author  :xiaojunzhong
  * Function:初始化车辆事件、输入输出及通信服务后创建任务，初值在并发前建立。
  * Version :V1.3.0
  * Data    :2026.11.05
  * Input   :无；由 main 在启动内核前调用。
  * Output  :无返回值；建立服务初值并创建任务。
  * Precond :基础硬件已初始化；配置读取须支持内核启动前同步完成。
  * Failure :配置读取失败或非法时保持相应字段无效，不阻止紧急触发。
  * Effect  :调用 Did_Init 装载车辆配置；不新增任务、ISR 或动态内存。
*******************************************************/
void TaskManager_Start(void)
{
    RteSys_InitMsdSignals(); //建立共享数据和日志
    /* 配置读取接口接通后，上电先装载 VIN、车型及能源，再允许事件冻结。 */
    Did_Init();
    Hmi_Init();
    PowerService_Init();
    CanService_Init();
    InputService_Init();
    OutputService_Init();
    ModemPower_Init();
    Ecall_Init();
    /* 初始化原有通信框架；上电诊断由后续需求重新实现。 */
	Uds_Init();
	CanMatrix_Init();

    PowTask_Create(); //电源管理
    CanTask_Create(); //CAN任务
    MonitoringTask_Create();
    AppTask_Create(); //SOS按键触发检测
    EcallTask_Create();
	UdsTask_Create();
	CanTpTask_Create();
	CanManageTask_Create();
	CanMatrixRxMsgTask_Create();
}
