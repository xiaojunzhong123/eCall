/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Can_GeneralTypes.h
 *
 * Description: Implementation of CAN GENERALTYPES High Level layer
 *              Compiler:                IAR GCC
 *              AutoSAR Version:         4.4.0
 *
 *
 * Copyright (c) 2022 - 2026 Xiaohua Semiconductor Co., Ltd.
 *
 * All rights reserved.
 *
 * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT (the "License"). By downloading, installing, or
 * using the Software, you indicate that you accept the terms of the License, and you acknowledge that you have the
 * authority, for yourself or on behalf of your company, to bind your company to these terms. If you do not agree to all
 * of the terms of this License, you should not download or install the Software.
 *
 **********************************************************************************************************************/

#ifndef CAN_GENERAL_TYPES_H
#define CAN_GENERAL_TYPES_H

/**
 *   @file       Can_GeneralTypes.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 CAN Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif


/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "ComStack_Cfg.h"

/***********************************************************************************************************************
 *   HEADER FILE VERSION INFORMATION
 **********************************************************************************************************************/
#define CAN_GENERALTYPES_AR_RELEASE_MAJOR_VERSION        4
#define CAN_GENERALTYPES_AR_RELEASE_MINOR_VERSION        4
#define CAN_GENERALTYPES_AR_RELEASE_REVISION_VERSION     0
/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
 /**
 * @brief Can hardware object is busy
 */
#define CAN_BUSY                                (0x02)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief  Represent a hardware object handles of a CAN hardware unit
 */
typedef uint8_t Can_HwHandleType;


/**
 * @brief   Identifier of an L-PDU. The two most significant bits represent the frame type:
 *          - 00 Standard CAN ID with CAN message
 *          - 01 Standard CAN ID with CAN FD message
 *          - 10 Extended CAN ID with CAN message
 *          - 11 Extended CAN ID with CAN FD message
 */
typedef uint32_t Can_IdType;

/**
 * @brief   Data structure for Hardware Object Handle
 */
typedef struct {
  Can_IdType CanId;      /* CAN ID of CAN LPDU */
  Can_HwHandleType Hoh;  /* Hardware object ID */
  uint8_t ControllerId;    /* Controller ID */
} Can_HwType;

/**
 * @brief   States that are used by the several ControllerMode functions.
 */
typedef enum {
  CAN_CS_UNINIT = 0x00,    /* CAN controller state UNINIT */
  CAN_CS_STARTED,          /* CAN controller state STARTED */
  CAN_CS_STOPPED,          /* CAN controller state STOPPE */
  CAN_CS_SLEEP             /* CAN controller state SLEEP */
} Can_ControllerStateType;


/**
 * @brief   Error states of a CAN controller
 */
typedef enum {
  CAN_ERRORSTATE_ACTIVE = 0x00,    /* The CAN controller participates fully in network for communicationtakes */
  CAN_ERRORSTATE_PASSIVE,          /* The CAN controller participates in network for communication, but not send active error frames */
  CAN_ERRORSTATE_BUSOFF            /* The CAN controller does not participate in network for communication */
} Can_ErrorStateType;

/**
 * @brief   Data structure for PDU
 */
typedef struct {
  PduIdType swPduHandle;
  uint8_t length;
  Can_IdType id;
  uint8_t *sdu;
} Can_PduType;

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* CAN_GENERAL_TYPES_H */

/*---------------------- End of File ----------------------------------------*/
