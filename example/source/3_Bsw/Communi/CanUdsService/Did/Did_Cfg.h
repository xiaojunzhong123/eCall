/*******************************************************
  * Name    :Did_Cfg.h
  * Author  :xiaojunzhong
  * Function:BSW 诊断 DID 配置及车辆配置读取适配边界。
  * Version :V1.1.0
  * Data    :2026.11.04
*******************************************************/
#ifndef _DID_CFG_H_
#define _DID_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Uds_Def.h"
#include "Uds.h"
#include "Memory.h"
#include "RteSys.h"
#include "Nvm.h"
//#include "Nvm.h"
/*-------------define---------------------------------*/
#define DID_ADDR_NOE                    0x00
/*-------------function port--------------------------*/
/*-------------Uds port-------------------------------*/
#define Did_GetCurrentSessionMode()     Uds_GetCurrentSessionMode()
#define Did_GetCurrentSecurityLevel()   Uds_GetCurrentSecurityLevel()
/*-------------Memory---------------------------------*/
#define Did_Copy(p_buff, p_src, len)    Memory_CopyConst(p_buff, p_src, len)
#define Did_CheckIfInvalid(p_buff, len) Memory_CheckIfInvalid(p_buff, len)
/*-------------RteSys port----------------------------*/
#define Did_GetVoltageValue()           RteSys_GetVoltageValue()
/*-------------Nvm port-------------------------------*/
#define Did_NvmRead(addr, p_buff, len)  Nvm_Read(addr, p_buff, len)
#define Did_NvmWrite(addr, p_buff, len) Nvm_Write(addr, p_buff, len)
#define Did_NvmGetProgramStatus()       Nvm_GetProgramStatus()
/* 上电低频配置读取适配口，后续由用户接入实际存储；不预设地址或默认车型。
 * 仅在同步填充全部输出后返回 TRUE，不能将异步受理视为完成。
 * 车型和旧能源分别通过 RTESYS_MSD_VEHICLE_TYPE_VALID、
 * RTESYS_MSD_ENERGY_TYPE_VALID 标记；无来源字段必须清除相应有效位。
 * 组合能源仅低七位有意义，返回 TRUE 表示配置整体有效。
 */
#define Did_ReadVehicleProperties(p_vehicleType, p_energyType, p_validMask) FALSE
#define Did_ReadPropulsion(p_propulsionMask) FALSE
/*-------------enum and struct------------------------*/
typedef enum
{
    DID_READ_NOE     = 0x80,
    DID_READ_LV0     = 0x00,
    DID_READ_LV1     = 0x10,
    DID_READ_LV2     = 0x20,
    DID_READ_LV_MASK = 0x70
} Did_Read_Type;

typedef enum
{
    DID_WRITE_NOE     = 0x08,
    DID_WRITE_LV0     = 0x00,
    DID_WRITE_LV1     = 0x01,
    DID_WRITE_LV2     = 0x02,
    DID_WRITE_LV_MASK = 0x07
} Did_Write_Type;

typedef enum
{
    DID_STORAGE_FLS = 0x00,
    DID_STORAGE_NVM,
    DID_STORAGE_RAM
} Did_Storage_Type;

typedef Bool (*Did_Handle_Fun)(Bool isRead, uint8_t *p_buff, uint8_t len);

typedef struct
{
    const uint16_t         did;
    const uint8_t          len;
    const uint8_t          secuAccsLvl;
    const Did_Storage_Type storage;
    const uint32_t         addr;
    const uint8_t         *p_buff;
    const Did_Handle_Fun   dataHandleFun;
} Did_Cfg_Type;
/*-------------function statement---------------------*/
extern Bool Did_CheckIfService0x22ConditionsNotCorrect(uint16_t did);
extern Bool Did_CheckIfService0x2EConditionsNotCorrect(uint16_t did);
/*-------------variable statement---------------------*/
extern const uint8_t      g_DidCfgListNum;
extern const Did_Cfg_Type g_DidCfgList[];
#endif

