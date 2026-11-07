/*******************************************************
  * Name    :Did.c
  * Author  :xiaojunzhong
  * Function:BSW 诊断 DID 读写及上电车辆配置装载。
  * Version :V1.1.0
  * Data    :2026.11.04
*******************************************************/
/*-------------include file---------------------------*/
#include "Did_Cfg.h"
#include "Did.h"
#include <string.h>

/* UdsTask owns the pending VIN copy for one accepted 0x2E transaction.
 * Never publish the shared request buffer while NVM completion is pending.
 */
typedef struct
{
    uint8_t au8Vin[RTESYS_MSD_VIN_LENGTH];
    uint8_t u8Pending;
} DidVinRuntime_t;
static DidVinRuntime_t s_mDidVinRun;
#if (UDS_VIN_LEN != RTESYS_MSD_VIN_LENGTH)
#error Inconsistent_VIN_length
#endif
/*-------------variable statement---------------------*/

/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Did_Init
  * Author  :xiaojunzhong
  * Function:上电独立装载 VIN、车型、旧能源及组合能源到车辆 RTE。
  * Version :V1.0.0
  * Data    :2026.11.04
  * Input   :无；VIN 沿用既有地址，其他配置经 Did_Cfg.h 读取适配口。
  * Output  :无返回值；每组仅发布成功读取的数据，VIN 非法不发布。
  * Caller  :TaskManager_Start，内核启动及任务创建前调用一次。
  * Precond :RTE 已初始化；NVM 读接口须可在内核启动前同步读取。
  * Failure :当前读取适配口未接通；无配置时保持对应字段无效。
  * Effect  :清理 DID 待写运行态；成功时更新各组 RTE 及发布版本。
*******************************************************/
void Did_Init(void)
{
    uint8_t au8StoredVin[RTESYS_MSD_VIN_LENGTH] = {0U}; /* 启动调用独占的十七字节读取副本，函数返回后失效。 */
    uint8_t u8VehicleType = 0xFFU; /* 启动局部车型代码，一至十三有效；未读取时为无效占位。 */
    uint8_t u8EnergyType = 0xFFU; /* 启动局部旧能源代码，实际映射由配置来源确认。 */
    uint8_t u8VehicleValidMask = 0U; /* 启动局部有效位，分别描述车型和旧能源是否可用。 */
    uint8_t u8PropulsionMask = 0U; /* 启动局部七种组合能源位；读取成功前零不代表已配置。 */

    /* 初始化发生在 UdsTask 运行前，不能在诊断写入事务中重复调用。 */
    (void)memset(&s_mDidVinRun, 0, sizeof(s_mDidVinRun));
    /* TRUE 必须表示目标缓存已完整填充；异步受理不能当作读取完成。 */
    if (Did_NvmRead(UDS_VIN_ADDR, au8StoredVin, UDS_VIN_LEN) == TRUE)
    {
        /* 发布接口检查长度和合法字符，非法内容保持上电无效状态。 */
        (void)RteSys_UpdateMsdVin(au8StoredVin, UDS_VIN_LEN);
    }

    if (Did_ReadVehicleProperties(&u8VehicleType, &u8EnergyType, &u8VehicleValidMask) == TRUE)
    {
        if ((u8VehicleType < 1U) || (u8VehicleType > 13U))
        {
            u8VehicleValidMask &= (uint8_t)(~RTESYS_MSD_VEHICLE_TYPE_VALID);
        }
        RteSys_PublishMsdVehicleProperties(u8VehicleType, u8EnergyType, u8VehicleValidMask);
    }
    if (Did_ReadPropulsion(&u8PropulsionMask) == TRUE)
    {
        /* 旧能源代码不能推导组合能源；高位非法时由 RTE 标为无效。 */
        RteSys_PublishMsdPropulsion(u8PropulsionMask, 1U);
    }
}

/*******************************************************
  * Name    :Did_FinalRespNrcDeal
  * Author  :LoH
  * Function:Release the pending VIN copy when the diagnostic transaction terminates.
  * Version :V1.0.0
  * Data    :2026.10.29
*******************************************************/
static void Did_FinalRespNrcDeal(Uds_DcmOpStatus_Type opStatus)
{
    (void)opStatus;
    (void)memset(&s_mDidVinRun, 0, sizeof(s_mDidVinRun));
}

/*******************************************************
  * Name    :Did_0x22ReadDataByIdentifier
  * Author  :LoH
  * Function:Read configured DIDs and publish successfully read VIN data to RTE.
  * Version :V1.0.0
  * Data    :2026.10.29
*******************************************************/
Return_Type Did_0x22ReadDataByIdentifier(Uds_DcmOpStatus_Type opStatus, Uds_DcmMsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode)
{
    uint8_t  i   = 0;
    uint8_t  j   = 0;
    uint16_t did = 0;

    if (opStatus != UDS_DCM_INITIAL)
    {
        *p_errCode = UDS_DCM_E_NRC_CNC;
        return RET_NOT_OK;
    }

    if (p_msgContext->rxLen < 2)
    {
        *p_errCode = UDS_DCM_E_NRC_IMLOIF;
        return RET_NOT_OK;
    }

    if ((p_msgContext->rxLen % 2) != 0)
    {
        *p_errCode = UDS_DCM_E_NRC_IMLOIF;
        return RET_NOT_OK;
    }

    p_msgContext->txLen = 0;
    for (i = 0; i < (p_msgContext->rxLen / 2); i++)
    {
        did  = (uint16_t)(p_msgContext->p_rxData[(2 * i) + 0] << 8);
        did += (uint16_t)(p_msgContext->p_rxData[(2 * i) + 1] << 0);
        for (j = 0; j < g_DidCfgListNum; j++)
        {
            if (did == g_DidCfgList[j].did)
            {
                if ((g_DidCfgList[j].secuAccsLvl & 0xF0) == DID_READ_NOE)
                {
                    *p_errCode = UDS_DCM_E_NRC_ROOR;
                    return RET_NOT_OK;
                }

                if (Did_GetCurrentSecurityLevel() < ((g_DidCfgList[j].secuAccsLvl & DID_READ_LV_MASK) >> 4))
                {
                    *p_errCode = UDS_DCM_E_NRC_SAD;
                    return RET_NOT_OK;
                }

                if (Did_CheckIfService0x22ConditionsNotCorrect(did) == TRUE)
                {
                    *p_errCode = UDS_DCM_E_NRC_CNC;
                    return RET_NOT_OK;
                }

                p_msgContext->p_txData[p_msgContext->txLen++] = (uint8_t)((did >> 8) & 0xFF);
                p_msgContext->p_txData[p_msgContext->txLen++] = (uint8_t)((did >> 0) & 0xFF);
                switch (g_DidCfgList[j].storage)
                {
                    case DID_STORAGE_FLS:
                        if (g_DidCfgList[j].p_buff != NULL)
                        {
                            Did_Copy(&p_msgContext->p_txData[p_msgContext->txLen], g_DidCfgList[j].p_buff, g_DidCfgList[j].len);
                        }
                        break;
                    case DID_STORAGE_NVM:
                        if (g_DidCfgList[j].addr != DID_ADDR_NOE)
                        {
                            if (Did_NvmRead(g_DidCfgList[j].addr, &p_msgContext->p_txData[p_msgContext->txLen], g_DidCfgList[j].len) == FALSE)
                            {
                                *p_errCode = UDS_DCM_E_NRC_CNC;
                                return RET_NOT_OK;
                            }
                        }
                        break;
                    case DID_STORAGE_RAM:
                        if (g_DidCfgList[j].dataHandleFun != NULL)
                        {
                            if (g_DidCfgList[j].dataHandleFun(TRUE, &p_msgContext->p_txData[p_msgContext->txLen], g_DidCfgList[j].len) == FALSE)
                            {
                                *p_errCode = UDS_DCM_E_NRC_CNC;
                                return RET_NOT_OK;
                            }
                        }
                        break;
                    default:
                        *p_errCode = UDS_DCM_E_NRC_CNC;
                        return RET_NOT_OK;
                }
                if (did == 0xF190U)
                {
                    (void)RteSys_UpdateMsdVin(&p_msgContext->p_txData[p_msgContext->txLen], g_DidCfgList[j].len);
                }
                p_msgContext->txLen += g_DidCfgList[j].len;
                break;
            }
        }
        if (j >= g_DidCfgListNum)
        {
            *p_errCode = UDS_DCM_E_NRC_ROOR;
            return RET_NOT_OK;
        }
    }
    return RET_OK;
}

/*******************************************************
  * Name    :Did_0x2EWriteDataByIdentifier
  * Author  :LoH
  * Function:Write DIDs and publish the accepted VIN copy only after NVM completion.
  * Version :V1.0.0
  * Data    :2026.10.29
*******************************************************/
Return_Type Did_0x2EWriteDataByIdentifier(Uds_DcmOpStatus_Type opStatus, Uds_DcmMsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode)
{
    uint16_t    did = 0;
    uint8_t     i   = 0;
    Return_Type ret = RET_NOT_OK;

    if (opStatus == UDS_DCM_CANCEL)
    {
        (void)memset(&s_mDidVinRun, 0, sizeof(s_mDidVinRun));
        *p_errCode = UDS_DCM_E_NRC_CNC;
        return RET_NOT_OK;
    }
    if (opStatus == UDS_DCM_INITIAL)
    {
        (void)memset(&s_mDidVinRun, 0, sizeof(s_mDidVinRun));
        if (p_msgContext->rxLen < 3)
        {
            *p_errCode = UDS_DCM_E_NRC_IMLOIF;
            return RET_NOT_OK;
        }

        did  = (uint16_t)(p_msgContext->p_rxData[0] << 8);
        did += (uint16_t)(p_msgContext->p_rxData[1] << 0);
        for (i = 0; i < g_DidCfgListNum; i++)
        {
            if (did == g_DidCfgList[i].did)
            {
                break;
            }
        }
        if (i >= g_DidCfgListNum)
        {
            *p_errCode = UDS_DCM_E_NRC_ROOR;
            return RET_NOT_OK;
        }

        if ((g_DidCfgList[i].secuAccsLvl & 0x0F) == DID_WRITE_NOE)
        {
            *p_errCode = UDS_DCM_E_NRC_ROOR;
            return RET_NOT_OK;
        }

        if ((p_msgContext->rxLen - 2) != g_DidCfgList[i].len)
        {
            *p_errCode = UDS_DCM_E_NRC_IMLOIF;
            return RET_NOT_OK;
        }

        if (Did_GetCurrentSecurityLevel() < (g_DidCfgList[i].secuAccsLvl & DID_WRITE_LV_MASK))
        {
            *p_errCode = UDS_DCM_E_NRC_SAD;
            return RET_NOT_OK;
        }

        if (Did_CheckIfService0x2EConditionsNotCorrect(did) == TRUE)
        {
            *p_errCode = UDS_DCM_E_NRC_CNC;
            return RET_NOT_OK;
        }

        if ((Did_CheckIfInvalid(&p_msgContext->p_rxData[2], p_msgContext->rxLen - 2) == TRUE) && (p_msgContext->rxLen > 3))
        {
            *p_errCode = UDS_DCM_E_NRC_ROOR;
            return RET_NOT_OK;
        }

        if ((did == 0xF190U)
            && (RteSys_IsMsdVinValid(&p_msgContext->p_rxData[2], g_DidCfgList[i].len) == 0U))
        {
            *p_errCode = UDS_DCM_E_NRC_ROOR;
            return RET_NOT_OK;
        }

        if (g_DidCfgList[i].storage != DID_STORAGE_NVM)
        {
            *p_errCode = UDS_DCM_E_NRC_CNC;
            return RET_NOT_OK;
        }

        if (g_DidCfgList[i].dataHandleFun != NULL)
        {
            if (g_DidCfgList[i].dataHandleFun(FALSE, &p_msgContext->p_rxData[2], g_DidCfgList[i].len) == FALSE)
            {
                *p_errCode = UDS_DCM_E_NRC_CNC;
                return RET_NOT_OK;
            }
        }

        if (g_DidCfgList[i].addr != DID_ADDR_NOE)
        {
            if (Did_NvmWrite(g_DidCfgList[i].addr, &p_msgContext->p_rxData[2], g_DidCfgList[i].len) == FALSE)
            {
                *p_errCode = UDS_DCM_E_NRC_CNC;
                return RET_NOT_OK;
            }
        }
        if (did == 0xF190U)
        {
            (void)memcpy(s_mDidVinRun.au8Vin, &p_msgContext->p_rxData[2], RTESYS_MSD_VIN_LENGTH);
            s_mDidVinRun.u8Pending = 1U;
        }
        p_msgContext->finalRespNrc      = UDS_DCM_E_NRC_GPF;
        p_msgContext->finalRespToNotify = Did_FinalRespNrcDeal;
        return RET_RCRRP;
    }
    else
    {
        ret = Did_NvmGetProgramStatus();
        if (ret == RET_NOT_OK)
        {
            (void)memset(&s_mDidVinRun, 0, sizeof(s_mDidVinRun));
            *p_errCode = UDS_DCM_E_NRC_GPF;
            return RET_NOT_OK;
        }

        if (ret == RET_OK)
        {
            if (s_mDidVinRun.u8Pending != 0U)
            {
                (void)RteSys_UpdateMsdVin(s_mDidVinRun.au8Vin, RTESYS_MSD_VIN_LENGTH);
                (void)memset(&s_mDidVinRun, 0, sizeof(s_mDidVinRun));
            }
            p_msgContext->p_txData[0] = p_msgContext->p_rxData[0];
            p_msgContext->p_txData[1] = p_msgContext->p_rxData[1];
            p_msgContext->txLen       = 2;
            return RET_OK;
        }
        return ret;
    }
}

