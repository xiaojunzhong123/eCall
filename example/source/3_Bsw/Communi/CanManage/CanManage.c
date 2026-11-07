/*******************************************************
  * Name    :CanManage.c
  * Author  :WangHu
  * Function:CanManage 管整个 CAN 通信处于什么工作阶段。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "CanManage_Cfg.h"
/* 任务启动日志使用已有声明，避免隐式推断函数类型。 */
#include "Hmi.h"
#include "CanManage.h"
/*-------------variable statement---------------------*/

/*-------------function-------------------------------*/
/*******************************************************
  * Name    :CanManage_TimerCtrl
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanManage_TimerCtrl(void)
{
    CanManage_NmTimerCtrl();
    CanManage_TranscieverTimerCtrl();
}

/*******************************************************
  * Name    :CanManage_Init
  * Author  :WangHu
  * Function:启动：INIT → WAIT → NORMAL
			休眠：NORMAL → CanNm_Sleep → TWBS → SLEEP
			唤醒：SLEEP → 恢复 CAN/NM → WAIT → NORMAL
			LocalSleepFlag 是输入：系统是否要求休眠；
			CanSleepFlag 是输出：本模块是否走到休眠状态。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void CanManage_Main(void)
{
    static uint32_t            waitTime      = 0;
    static CanManage_Mode_Type canManageMode = CANMANAGE_MODE_INIT;

    switch (canManageMode)
    {
        case CANMANAGE_MODE_INIT:
            if (CanManage_CheckVoltageIfNotOK() == FALSE)
            {
                CanManage_TranscieverInit();
                CanManage_NmStart();
                waitTime      = CanManage_GetSysTimeMs();
                canManageMode = CANMANAGE_MODE_WAIT;
            }
            else
            {
                if (CanManage_CheckLocalIfSleep() == TRUE)
                {
                    CanManage_SetCanSleepFlag(TRUE);
                    canManageMode = CANMANAGE_MODE_SLEEP;
                }
            }
            break;
        case CANMANAGE_MODE_WAIT:
            if ((CanManage_GetSysTimeMs() - waitTime) >= CANMANAGE_ACTIVE_TIME)
            {
                canManageMode = CANMANAGE_MODE_NORMAL;
            }
            break;
        case CANMANAGE_MODE_NORMAL:
            CanManage_NmMain();
            CanManage_TranscieverNormalDeal();
            if (CanManage_CheckVoltageIfNotOK() == TRUE)
            {
                CanManage_NmStop();
                CanManage_TranscieverDeInit();
                canManageMode = CANMANAGE_MODE_INIT;
            }
            else
            {
                if (CanManage_CheckLocalIfSleep() == TRUE)
                {
                    CanManage_NmSleep();
                    canManageMode = CANMANAGE_MODE_TWBS;
                }
            }
            break;
        case CANMANAGE_MODE_TWBS: //TWBS是等待进入总线休眠的过渡阶段
            CanManage_NmMain();
            CanManage_TranscieverNormalDeal();
            if (CanManage_CheckLocalIfSleep() == TRUE)
            {
                if ((CanManage_NmCheckIfSleep() == TRUE) || (CanManage_CheckVoltageIfNotOK() == TRUE))
                {
                    if (CanManage_NmCheckIfSleep() == FALSE)
                    {
                        CanManage_NmSetSleeped();
                    }
                    CanManage_TranscieverSleep();
                    CanManage_SetCanSleepFlag(TRUE);
                    canManageMode = CANMANAGE_MODE_SLEEP;
                }
                else
                {
                    canManageMode = CANMANAGE_MODE_NORMAL;
                }
            }
            else
            {
                CanManage_WakeUp();
                canManageMode = CANMANAGE_MODE_NORMAL;
            }
            break;
        case CANMANAGE_MODE_SLEEP:
            if (CanManage_CheckLocalIfSleep() == FALSE)
            {
                CanManage_SetCanSleepFlag(FALSE);
                CanManage_TranscieverWakeUp();
                CanManage_WakeUp();
                waitTime      = CanManage_GetSysTimeMs();
                canManageMode = CANMANAGE_MODE_WAIT;
            }
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanManageTask_Entry
  * Author  :xiaojunzhong
  * Function:Create CanManageTask with real-time RTOS priority.
  * Version :V1.0.0
  * Data    :2026.10.21
*******************************************************/
static void CanManageTask_Entry(void const *argument)
{
    (void)argument;
    Hmi_ShowTaskStarted("CanManageTask");

    for (;;)
    {
        CanManage_Main();
        osDelay(10U);
    }
}

/*******************************************************
  * Name    :CanManageTask_Create
  * Author  :xiaojunzhong
  * Function:Create CanManageTask with real-time RTOS priority.
  * Version :V1.0.0
  * Data    :2026.10.21
*******************************************************/
void CanManageTask_Create(void)
{
    osThreadDef(CanManageTask, CanManageTask_Entry, osPriorityHigh, 0U, 512U);
    (void)osThreadCreate(osThread(CanManageTask), NULL);
}


