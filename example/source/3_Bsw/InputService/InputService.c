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
#define INPUT_SERVICE_SOS_DEBOUNCE_MS       50U
#define INPUT_SERVICE_KL15_DEBOUNCE_MS      200U

static RteSwitchState_t m_enSosSwitchState = RTE_SWITCH_RELEASED;
static uint8_t m_u8SosDebounceTime = 0U;
static RteKl15State_t m_enKl15State = RTE_KL15_OFF;
static uint16_t m_u16Kl15DebounceTime = 0U;

/*******************************************************
  * Name    :InputService_Init
  * Author  :LoH
  * Function:Initialize debounced SOS and KL15 input states.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void InputService_Init(void)
{
    m_enSosSwitchState = RTE_SWITCH_RELEASED;
    m_u8SosDebounceTime = 0U;
    m_enKl15State = RTE_KL15_OFF;
    m_u16Kl15DebounceTime = 0U;
    RteSys_SetU8Sig(RTESYS_U8_SOS_SWITCH_STATE, (uint8_t)m_enSosSwitchState);
    RteSys_SetU8Sig(RTESYS_U8_KL15_STATE, (uint8_t)m_enKl15State);
}

/*******************************************************
  * Name    :InputService_Process10ms
  * Author  :LoH
  * Function:Debounce SOS and PB7 KL15 inputs, then update RTE states.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void InputService_Process10ms(void)
{
    RteSwitchState_t enRawState;
    RteKl15State_t enRawKl15State;

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

    if (GPIO_GetInputState(E_INPUT_KL15_SWITCH) == HIGH)
    {
        enRawKl15State = RTE_KL15_ON;
    }
    else
    {
        enRawKl15State = RTE_KL15_OFF;
    }

    if (enRawKl15State == m_enKl15State)
    {
        m_u16Kl15DebounceTime = 0U;
    }
    else
    {
        m_u16Kl15DebounceTime += INPUT_SERVICE_PERIOD_MS;
        if (m_u16Kl15DebounceTime >= INPUT_SERVICE_KL15_DEBOUNCE_MS)
        {
            m_enKl15State = enRawKl15State;
            m_u16Kl15DebounceTime = 0U;
            RteSys_SetU8Sig(RTESYS_U8_KL15_STATE, (uint8_t)m_enKl15State);
        }
    }
}
