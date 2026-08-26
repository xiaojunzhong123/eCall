/*******************************************************
  * Name    :Task.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Task_Cfg.h"
/*-------------variable statement---------------------*/
static Bool g_Task2msTimerFlag  = FALSE;
static Bool g_Task5msTimerFlag  = FALSE;
static Bool g_Task10msTimerFlag = FALSE;
static Bool g_Task20msTimerFlag = FALSE;
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Task_2msSchdule
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void Task_2msSchdule(void)
{
    if (g_Task2msTimerFlag == TRUE)
    {
        Task_2msSchduleTable();
        g_Task2msTimerFlag = FALSE;
    }
}

/*******************************************************
  * Name    :Task_5msSchdule
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void Task_5msSchdule(void)
{
    if (g_Task5msTimerFlag == TRUE)
    {
        Task_5msSchduleTable();
        g_Task5msTimerFlag = FALSE;
    }
}

/*******************************************************
  * Name    :Task_10msSchdule
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void Task_10msSchdule(void)
{
    if (g_Task10msTimerFlag == TRUE)
    {
        Task_10msSchduleTable();
        g_Task10msTimerFlag = FALSE;
    }
}

/*******************************************************
  * Name    :Task_20msSchdule
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void Task_20msSchdule(void)
{
    if (g_Task20msTimerFlag == TRUE)
    {
        Task_20msSchduleTable();
        g_Task20msTimerFlag = FALSE;
    }
}

/*******************************************************
  * Name    :Task_IdleSchdule
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void Task_IdleSchdule(void)
{
    Task_IdleSchduleTable();
}

/*******************************************************
  * Name    :Task_InitSchdule
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Task_InitSchdule(void)
{
    Task_InitSchduleTable();
}

/*******************************************************
  * Name    :Task_1msSchdule
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Task_1msSchdule(void)
{
    static uint8_t task_2msTimerCount  = 0;
    static uint8_t task_5msTimerCount  = 0;
    static uint8_t task_10msTimerCount = 0;
    static uint8_t task_20msTimerCount = 0;

    if (++task_2msTimerCount >= 2)
    {
        task_2msTimerCount = 0;
        g_Task2msTimerFlag = TRUE;
    }

    if (++task_5msTimerCount >= 5)
    {
        task_5msTimerCount = 0;
        g_Task5msTimerFlag = TRUE;
    }

    if (++task_10msTimerCount >= 10)
    {
        task_10msTimerCount = 0;
        g_Task10msTimerFlag = TRUE;
    }

    if (++task_20msTimerCount >= 20)
    {
        task_20msTimerCount = 0;
        g_Task20msTimerFlag = TRUE;
    }
    Task_SetSysTimeMs(1);
    Task_1msSchduleTable();
}

/*******************************************************
  * Name    :Task_RunSchdule
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Task_RunSchdule(void)
{
    Task_2msSchdule();
    Task_5msSchdule();
    Task_10msSchdule();
    Task_20msSchdule();
    Task_IdleSchdule();
}

