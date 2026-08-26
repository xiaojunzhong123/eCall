/*******************************************************
  * Name    :InputService.c
  * Author  :LoH
  * Function:PA0 SOS switch debounce and RTE state update.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "Gpio.h"
#include "InputService.h"
#include "RteSys.h"

#define INPUT_SERVICE_PERIOD_MS             10U
#define INPUT_SERVICE_SOS_DEBOUNCE_MS       30U

static RteSwitchState_t m_enSosSwitchState = RTE_SWITCH_RELEASED;
static uint8_t m_u8SosDebounceTime = 0U;

/*******************************************************
  * Name    :InputService_Init
  * Author  :LoH
  * Function:Initialize debounced SOS switch state to released.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void InputService_Init(void)
{
    m_enSosSwitchState = RTE_SWITCH_RELEASED;
    m_u8SosDebounceTime = 0U;
    RteSys_SetU8Sig(RTESYS_U8_SOS_SWITCH_STATE, (uint8_t)m_enSosSwitchState);
}

/*******************************************************
  * Name    :InputService_Process10ms
  * Author  :LoH
  * Function:Debounce PA0 SOS switch and update RTE state.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void InputService_Process10ms(void)
{
    RteSwitchState_t enRawState;

    if (GPIO_GetInputState(E_INPUT_ECALL_SOS_SWITCH) == HIGH)
    {
        enRawState = RTE_SWITCH_PRESSED;
    }
    else
    {
        enRawState = RTE_SWITCH_RELEASED;
    }

    if (enRawState == m_enSosSwitchState)
    {
        m_u8SosDebounceTime = 0U;
    }
    else
    {
        m_u8SosDebounceTime += INPUT_SERVICE_PERIOD_MS;
        if (m_u8SosDebounceTime >= INPUT_SERVICE_SOS_DEBOUNCE_MS)
        {
            m_enSosSwitchState = enRawState;
            m_u8SosDebounceTime = 0U;
            RteSys_SetU8Sig(RTESYS_U8_SOS_SWITCH_STATE, (uint8_t)m_enSosSwitchState);
        }
    }
}
