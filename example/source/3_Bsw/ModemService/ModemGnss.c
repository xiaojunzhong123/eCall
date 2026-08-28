/*******************************************************
  * Name    :ModemGnss.c
  * Author  :LoH
  * Function:Background AC7600 CGPSINFO positioning workflow.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
#include "ModemGnss.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include <stdio.h>
#include <string.h>
#include "Hmi.h"
#include "ModemAt_Cfg.h"
#include "ModemPort.h"

typedef enum
{
    MODEM_GNSS_STEP_POWER_QUERY_QUEUE = 0U,
    MODEM_GNSS_STEP_POWER_QUERY_WAIT,
    MODEM_GNSS_STEP_POWER_ON_QUEUE,
    MODEM_GNSS_STEP_POWER_ON_WAIT,
    MODEM_GNSS_STEP_QUERY_HOLD,
    MODEM_GNSS_STEP_POSITION_QUEUE,
    MODEM_GNSS_STEP_POSITION_WAIT
} ModemGnssStep_t;

typedef struct
{
    ModemGnssStep_t enStep;
    uint32_t u32QueryHoldMs;
    ModemGnssSample_t stSample;
    uint8_t u8SamplePending;
    uint8_t u8PowerReady;
} ModemGnssRuntime_t;

static ModemGnssRuntime_t s_mGnssRun;

/*******************************************************
  * Name    :ModemGnss_IsLeapYear
  * Author  :LoH
  * Function:Return whether one Gregorian year contains February 29.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static uint8_t ModemGnss_IsLeapYear(uint16_t u16Year)
{
    return ((((u16Year % 4U) == 0U) && ((u16Year % 100U) != 0U))
            || ((u16Year % 400U) == 0U)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemGnss_ParseCoordinate
  * Author  :LoH
  * Function:Convert NMEA ddmm.mmmmmm text to signed decimal degrees times 1e7.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static uint8_t ModemGnss_ParseCoordinate(const char *pcText, char cHemisphere,
                                         uint8_t u8DegreeDigits, int32_t *pi32CoordinateE7)
{
    uint32_t u32Degrees = 0U;
    uint32_t u32Minutes = 0U;
    uint32_t u32Fraction = 0U;
    uint8_t u8Index;
    uint8_t u8FractionDigits = 0U;
    int32_t i32Coordinate;

    if ((pcText == 0) || (pi32CoordinateE7 == 0))
    {
        return 0U;
    }
    for (u8Index = 0U; u8Index < u8DegreeDigits; u8Index++)
    {
        if ((pcText[u8Index] < '0') || (pcText[u8Index] > '9'))
        {
            return 0U;
        }
        u32Degrees = (u32Degrees * 10U) + (uint32_t)(pcText[u8Index] - '0');
    }
    for (; u8Index < (uint8_t)(u8DegreeDigits + 2U); u8Index++)
    {
        if ((pcText[u8Index] < '0') || (pcText[u8Index] > '9'))
        {
            return 0U;
        }
        u32Minutes = (u32Minutes * 10U) + (uint32_t)(pcText[u8Index] - '0');
    }
    if ((u32Minutes >= 60U) || (pcText[u8Index] != '.'))
    {
        return 0U;
    }
    u8Index++;
    while ((pcText[u8Index] >= '0') && (pcText[u8Index] <= '9'))
    {
        if (u8FractionDigits < 6U)
        {
            u32Fraction = (u32Fraction * 10U) + (uint32_t)(pcText[u8Index] - '0');
            u8FractionDigits++;
        }
        u8Index++;
    }
    if ((u8FractionDigits == 0U) || (pcText[u8Index] != '\0'))
    {
        return 0U;
    }
    while (u8FractionDigits < 6U)
    {
        u32Fraction *= 10U;
        u8FractionDigits++;
    }
    i32Coordinate = (int32_t)((u32Degrees * 10000000UL)
                              + (((u32Minutes * 1000000UL) + u32Fraction + 3U) / 6U));
    if ((cHemisphere == 'S') || (cHemisphere == 'W'))
    {
        i32Coordinate = -i32Coordinate;
    }
    else if ((cHemisphere != 'N') && (cHemisphere != 'E'))
    {
        return 0U;
    }
    *pi32CoordinateE7 = i32Coordinate;
    return 1U;
}

/*******************************************************
  * Name    :ModemGnss_ParseUtc
  * Author  :LoH
  * Function:Convert CGPSINFO ddmmyy and hhmmss fields to Unix UTC seconds.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static uint8_t ModemGnss_ParseUtc(const char *pcDate, const char *pcTime, uint32_t *pu32UtcSeconds)
{
    static const uint8_t au8MonthDays[12] = {31U, 28U, 31U, 30U, 31U, 30U,
                                             31U, 31U, 30U, 31U, 30U, 31U};
    uint16_t u16Year;
    uint16_t u16LoopYear;
    uint32_t u32Days = 0U;
    uint8_t u8Day;
    uint8_t u8Month;
    uint8_t u8Hour;
    uint8_t u8Minute;
    uint8_t u8Second;
    uint8_t u8Index;

    if ((pcDate == 0) || (pcTime == 0) || (pu32UtcSeconds == 0)
        || (strlen(pcDate) != 6U) || (strlen(pcTime) < 6U))
    {
        return 0U;
    }
    for (u8Index = 0U; u8Index < 6U; u8Index++)
    {
        if ((pcDate[u8Index] < '0') || (pcDate[u8Index] > '9')
            || (pcTime[u8Index] < '0') || (pcTime[u8Index] > '9'))
        {
            return 0U;
        }
    }
    u8Day = (uint8_t)(((pcDate[0] - '0') * 10) + (pcDate[1] - '0'));
    u8Month = (uint8_t)(((pcDate[2] - '0') * 10) + (pcDate[3] - '0'));
    u16Year = (uint16_t)(2000U + (uint16_t)(((pcDate[4] - '0') * 10) + (pcDate[5] - '0')));
    u8Hour = (uint8_t)(((pcTime[0] - '0') * 10) + (pcTime[1] - '0'));
    u8Minute = (uint8_t)(((pcTime[2] - '0') * 10) + (pcTime[3] - '0'));
    u8Second = (uint8_t)(((pcTime[4] - '0') * 10) + (pcTime[5] - '0'));
    if ((u8Month == 0U) || (u8Month > 12U) || (u8Day == 0U)
        || (u8Hour > 23U) || (u8Minute > 59U) || (u8Second > 59U))
    {
        return 0U;
    }
    if (u8Day > (uint8_t)(au8MonthDays[u8Month - 1U]
                           + (((u8Month == 2U) && (ModemGnss_IsLeapYear(u16Year) != 0U)) ? 1U : 0U)))
    {
        return 0U;
    }
    for (u16LoopYear = 1970U; u16LoopYear < u16Year; u16LoopYear++)
    {
        u32Days += (ModemGnss_IsLeapYear(u16LoopYear) != 0U) ? 366U : 365U;
    }
    for (u8Index = 1U; u8Index < u8Month; u8Index++)
    {
        u32Days += au8MonthDays[u8Index - 1U];
        if ((u8Index == 2U) && (ModemGnss_IsLeapYear(u16Year) != 0U))
        {
            u32Days++;
        }
    }
    u32Days += (uint32_t)(u8Day - 1U);
    *pu32UtcSeconds = (u32Days * 86400UL) + ((uint32_t)u8Hour * 3600UL)
                      + ((uint32_t)u8Minute * 60UL) + u8Second;
    return 1U;
}

/*******************************************************
  * Name    :ModemGnss_ParsePosition
  * Author  :LoH
  * Function:Parse one valid AC7600 CGPSINFO response into an internal e7 sample.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static uint8_t ModemGnss_ParsePosition(const char *pcResponse, ModemGnssSample_t *pstSample)
{
    char acLatitude[16];
    char acLongitude[16];
    char acDate[8];
    char acTime[12];
    char cNorthSouth;
    char cEastWest;

    if ((pcResponse == 0) || (pstSample == 0)
        || (sscanf(pcResponse, "+CGPSINFO: %15[^,],%c,%15[^,],%c,%7[^,],%11[^,]",
                   acLatitude, &cNorthSouth, acLongitude, &cEastWest, acDate, acTime) != 6))
    {
        return 0U;
    }
    return ((ModemGnss_ParseCoordinate(acLatitude, cNorthSouth, 2U, &pstSample->i32LatitudeE7) != 0U)
            && (ModemGnss_ParseCoordinate(acLongitude, cEastWest, 3U, &pstSample->i32LongitudeE7) != 0U)
            && (ModemGnss_ParseUtc(acDate, acTime, &pstSample->u32UtcSeconds) != 0U)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemGnss_EnterQueryHold
  * Author  :LoH
  * Function:Recover a private GNSS failure and wait before the next query.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static void ModemGnss_EnterQueryHold(void)
{
    ModemPort_ClearTransactionFailure();
    s_mGnssRun.u32QueryHoldMs = osKernelSysTick();
    s_mGnssRun.enStep = MODEM_GNSS_STEP_QUERY_HOLD;
}

/*******************************************************
  * Name    :ModemGnss_Init
  * Author  :LoH
  * Function:Initialize GNSS power discovery and sample storage.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void ModemGnss_Init(void)
{
    (void)memset(&s_mGnssRun, 0, sizeof(s_mGnssRun));
    s_mGnssRun.enStep = MODEM_GNSS_STEP_POWER_QUERY_QUEUE;
}

/*******************************************************
  * Name    :ModemGnss_Process
  * Author  :LoH
  * Function:Drive GNSS power setup and periodic one-shot position queries.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void ModemGnss_Process(uint8_t u8StartAllowed)
{
    const char *pcResponse;
    ModemGnssSample_t stSample;

    switch (s_mGnssRun.enStep)
    {
        case MODEM_GNSS_STEP_POWER_QUERY_QUEUE:
            if ((u8StartAllowed != 0U) && (ModemPort_IsIdle() != 0U))
            {
                ModemPort_ClearTransactionFailure();
                if (ModemPort_QueueCommand(MODEM_AT_CMD_GNSS_POWER_QUERY, MODEM_AT_RSP_GNSS_POWER,
                                           MODEM_AT_CFG_GNSS_POWER_TIMEOUT_MS) != 0U)
                {
                    s_mGnssRun.enStep = MODEM_GNSS_STEP_POWER_QUERY_WAIT;
                }
            }
            break;

        case MODEM_GNSS_STEP_POWER_QUERY_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                s_mGnssRun.u8PowerReady = 0U;
                ModemGnss_EnterQueryHold();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                pcResponse = ModemPort_GetLastMatchedResponse();
                s_mGnssRun.u8PowerReady = (strstr(pcResponse, ": 1") != 0) ? 1U : 0U;
                s_mGnssRun.enStep = (s_mGnssRun.u8PowerReady != 0U)
                                    ? MODEM_GNSS_STEP_QUERY_HOLD : MODEM_GNSS_STEP_POWER_ON_QUEUE;
                s_mGnssRun.u32QueryHoldMs = osKernelSysTick() - MODEM_AT_CFG_GNSS_QUERY_PERIOD_MS;
            }
            break;

        case MODEM_GNSS_STEP_POWER_ON_QUEUE:
            if ((u8StartAllowed != 0U) && (ModemPort_IsIdle() != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_GNSS_POWER_ON, MODEM_AT_RSP_GNSS_READY,
                                           MODEM_AT_CFG_GNSS_POWER_TIMEOUT_MS) != 0U))
            {
                s_mGnssRun.enStep = MODEM_GNSS_STEP_POWER_ON_WAIT;
            }
            break;

        case MODEM_GNSS_STEP_POWER_ON_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                s_mGnssRun.u8PowerReady = 0U;
                ModemGnss_EnterQueryHold();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mGnssRun.u32QueryHoldMs = osKernelSysTick();
                s_mGnssRun.u8PowerReady = 1U;
                s_mGnssRun.enStep = MODEM_GNSS_STEP_QUERY_HOLD;
                HMI_LOG_MODEM("GNSS_POWER_READY\r\n");
            }
            break;

        case MODEM_GNSS_STEP_QUERY_HOLD:
            if ((u8StartAllowed != 0U)
                && ((osKernelSysTick() - s_mGnssRun.u32QueryHoldMs) >= MODEM_AT_CFG_GNSS_QUERY_PERIOD_MS))
            {
                s_mGnssRun.enStep = (s_mGnssRun.u8PowerReady != 0U)
                                    ? MODEM_GNSS_STEP_POSITION_QUEUE : MODEM_GNSS_STEP_POWER_QUERY_QUEUE;
            }
            break;

        case MODEM_GNSS_STEP_POSITION_QUEUE:
            if ((u8StartAllowed != 0U) && (ModemPort_IsIdle() != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_GNSS_POSITION, MODEM_AT_RSP_GNSS_POSITION,
                                           MODEM_AT_CFG_GNSS_POSITION_TIMEOUT_MS) != 0U))
            {
                s_mGnssRun.enStep = MODEM_GNSS_STEP_POSITION_WAIT;
            }
            break;

        case MODEM_GNSS_STEP_POSITION_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                s_mGnssRun.u8PowerReady = 0U;
                ModemGnss_EnterQueryHold();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemGnss_ParsePosition(ModemPort_GetLastMatchedResponse(), &stSample) != 0U)
                {
                    portENTER_CRITICAL();
                    s_mGnssRun.stSample = stSample;
                    s_mGnssRun.u8SamplePending = 1U;
                    portEXIT_CRITICAL();
                }
                ModemGnss_EnterQueryHold();
            }
            break;

        default:
            ModemGnss_Init();
            break;
    }
}

/*******************************************************
  * Name    :ModemGnss_IsBusy
  * Author  :LoH
  * Function:Return whether GNSS owns an active AT command completion result.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
uint8_t ModemGnss_IsBusy(void)
{
    return ((s_mGnssRun.enStep == MODEM_GNSS_STEP_POWER_QUERY_WAIT)
            || (s_mGnssRun.enStep == MODEM_GNSS_STEP_POWER_ON_WAIT)
            || (s_mGnssRun.enStep == MODEM_GNSS_STEP_POSITION_WAIT)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemGnss_TakeSample
  * Author  :LoH
  * Function:Copy and consume one valid GNSS sample across task contexts.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
uint8_t ModemGnss_TakeSample(ModemGnssSample_t *pstSample)
{
    uint8_t u8Available;

    if (pstSample == 0)
    {
        return 0U;
    }
    portENTER_CRITICAL();
    u8Available = s_mGnssRun.u8SamplePending;
    if (u8Available != 0U)
    {
        *pstSample = s_mGnssRun.stSample;
        s_mGnssRun.u8SamplePending = 0U;
    }
    portEXIT_CRITICAL();
    return u8Available;
}
