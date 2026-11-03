/*******************************************************
  * Name    :CanMatrix.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _CANMATRIX_H_
#define _CANMATRIX_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------function port--------------------------*/
/* Task-context MSD-only ingress; frame format and exact DLC are checked.
 * Call from the single CAN owner, not ISR or a second matrix RX task.
 */
uint8_t CanMatrix_ProcessMsdFrame(uint32_t u32Id, uint8_t u8Fd, const uint8_t *pu8Payload, uint8_t u8Length);
extern void CanMatrix_Init(void);
extern void CanMatrix_RxMsgMain(void);
extern void CanMatrix_TxMsgMain(void);
extern Bool CanMatrix_CheckIfRxMsg(uint32_t id);
extern void CanMatrixRxMsgTask_Create(void);
#endif

