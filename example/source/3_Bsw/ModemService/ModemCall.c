/*******************************************************
  * Name    :ModemCall.c
  * Author  :LoH
  * Function:Private modem call state processing implementation.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
#include "ModemCall.h"

typedef struct
{
    uint16_t u16ProcessCount;
} ModemCallRuntime_t;

static ModemCallRuntime_t s_mCallRun;

/*******************************************************
  * Name    :ModemCall_Init
  * Author  :LoH
  * Function:Initialize the private call processing runtime.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemCall_Init(void)
{
    s_mCallRun.u16ProcessCount = 0U;
}

/*******************************************************
  * Name    :ModemCall_Reset
  * Author  :LoH
  * Function:Clear the private call delay counter for one new call request.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemCall_Reset(void)
{
    s_mCallRun.u16ProcessCount = 0U;
}

/*******************************************************
  * Name    :ModemCall_Process
  * Author  :LoH
  * Function:Process the temporary call timing model until real ATD URCs are integrated.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemCall_Process(ModemCallState_t *penCallState)
{
    if (penCallState == 0)
    {
        return;
    }

    if (*penCallState == MODEM_CALL_DIALING)
    {
        s_mCallRun.u16ProcessCount++;
        if (s_mCallRun.u16ProcessCount >= 3U)
        {
            s_mCallRun.u16ProcessCount = 0U;
            *penCallState = MODEM_CALL_ACTIVE;
        }
    }
    else if (*penCallState == MODEM_CALL_ACTIVE)
    {
        s_mCallRun.u16ProcessCount++;
        if (s_mCallRun.u16ProcessCount >= 10U)
        {
            s_mCallRun.u16ProcessCount = 0U;
            *penCallState = MODEM_CALL_ENDED;
        }
    }
    else
    {
        /* No processing is required for other call states. */
    }
}
