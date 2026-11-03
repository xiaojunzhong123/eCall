/*******************************************************
  * Name    :EcallDiag_Cfg.h
  * Author  :xiaojunzhong
  * Function:上电诊断和持续监测，来源观测、故障确认及恢复分别处理。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
#ifndef ECALL_DIAG_CFG_H
#define ECALL_DIAG_CFG_H
/* 以下为主机/台架候选参数；负责人确认前，不允许聚合健康或确认物理故障。 */
#ifndef ECALL_DIAG_POLICY_CONFIRMED
#define ECALL_DIAG_POLICY_CONFIRMED 0U
#endif
/* 默认不豁免任何一项；例如不可拆卸 SIM 必须批准后显式配置对应位。 */
#ifndef ECALL_DIAG_NOT_APPLICABLE_MASK
#define ECALL_DIAG_NOT_APPLICABLE_MASK 0U
#endif
#ifndef ECALL_DIAG_PREPARATION_MS
#define ECALL_DIAG_PREPARATION_MS 60000UL
#endif
#ifndef ECALL_DIAG_SAMPLE_MAX_AGE_MS
#define ECALL_DIAG_SAMPLE_MAX_AGE_MS 5000UL
#endif
#ifndef ECALL_DIAG_FAIL_CONFIRM_MS
#define ECALL_DIAG_FAIL_CONFIRM_MS 1000UL
#endif
#ifndef ECALL_DIAG_RECOVER_CONFIRM_MS
#define ECALL_DIAG_RECOVER_CONFIRM_MS 2000UL
#endif
#ifndef ECALL_DIAG_CONFIRM_SAMPLES
#define ECALL_DIAG_CONFIRM_SAMPLES 2U
#endif
#define ECALL_HMI_RETRY_MS 1000UL
#define ECALL_HMI_ACK_TIMEOUT_MS 2000UL
#ifndef ECALL_HMI_ARABIC_REVIEWED
#define ECALL_HMI_ARABIC_REVIEWED 0U
#endif
#ifndef ECALL_HMI_FAULT_AR
#define ECALL_HMI_FAULT_AR ""
#endif
#ifndef ECALL_HMI_INCOMPLETE_AR
#define ECALL_HMI_INCOMPLETE_AR ""
#endif
#ifndef ECALL_HMI_CHECKING_AR
#define ECALL_HMI_CHECKING_AR ""
#endif
#if ((ECALL_DIAG_POLICY_CONFIRMED > 1U) || (ECALL_DIAG_CONFIRM_SAMPLES == 0U) || (ECALL_DIAG_CONFIRM_SAMPLES > 65535U))
#error Invalid_ecall_diag_policy
#endif
#if ((ECALL_DIAG_NOT_APPLICABLE_MASK > 0x03FFU) || (ECALL_HMI_ARABIC_REVIEWED > 1U))
#error Invalid_ecall_diag_applicability_or_language
#endif
#if ((ECALL_DIAG_SAMPLE_MAX_AGE_MS == 0U) || (ECALL_DIAG_SAMPLE_MAX_AGE_MS >= 0x80000000UL) || (ECALL_DIAG_PREPARATION_MS >= 0x80000000UL) || (ECALL_DIAG_FAIL_CONFIRM_MS >= 0x80000000UL) || (ECALL_DIAG_RECOVER_CONFIRM_MS >= 0x80000000UL))
#error Invalid_ecall_diag_time_window
#endif
#endif
