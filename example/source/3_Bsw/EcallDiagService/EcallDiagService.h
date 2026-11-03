/*******************************************************
  * Name    :EcallDiagService.h
  * Author  :xiaojunzhong
  * Function:定义现有下层证据到诊断 RTE 的只读适配接口。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
#ifndef ECALL_DIAG_SERVICE_H
#define ECALL_DIAG_SERVICE_H
/* 只发布现有能力缺口，在 APP 建立新诊断代次之后初始化。 */
void EcallDiagService_Init(void);
/* AppTask 消费稳定服务快照，不发 AT、不清 UART、不控制电源。 */
void EcallDiagService_Process(void);
#endif
