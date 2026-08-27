/*******************************************************
  * Name    :ModemAt_Cfg.h
  * Author  :LoH
  * Function:Modem AT command and response configuration.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __MODEM_AT_CFG_H__
#define __MODEM_AT_CFG_H__

/* ML307A AT commands and response tokens. */
#define MODEM_AT_CMD_BASIC                      "AT\r\n"
#define MODEM_AT_CMD_SIM_STATUS                 "AT+CPIN?\r\n"
#define MODEM_AT_CMD_SIGNAL_QUALITY             "AT+CSQ\r\n"
#define MODEM_AT_CMD_CIRCUIT_REGISTRATION       "AT+CREG?\r\n"
#define MODEM_AT_CMD_TCP_KEEPALIVE_FORMAT       "AT+MIPTKA=%u,1,30,30,3\r\n"
#define MODEM_AT_CMD_HEX_ENCODING_FORMAT        "AT+MIPCFG=\"encoding\",%u,1,0\r\n"
#define MODEM_AT_CMD_TCP_OPEN_FORMAT            "AT+MIPOPEN=%u,\"TCP\",\"%s\",%u\r\n"
#define MODEM_AT_CMD_SEND_HEX_FORMAT            "AT+MIPSEND=%u,%u,\"%s\",0,1,0\r\n"

/* Dynamic commands must be assembled with snprintf and these format macros. */
#define MODEM_AT_CMD_DIAL_FORMAT                "ATD%s;\r\n"

/* Common response tokens. Actual URC handling remains module-specific. */
#define MODEM_AT_RSP_OK                         "OK"
#define MODEM_AT_RSP_ERROR                      "ERROR"
#define MODEM_AT_RSP_BUSY                        "BUSY"
#define MODEM_AT_RSP_NO_CARRIER                  "NO CARRIER"
#define MODEM_AT_RSP_SEND                        "MIPSEND"

/* ML307A platform test configuration from the approved bare-metal reference. */
#define MODEM_PLATFORM_LINK_ID                  0U
#define MODEM_PLATFORM_SERVER_IP                "8.137.175.163"
#define MODEM_PLATFORM_SERVER_PORT              8902U
#define MODEM_PLATFORM_TEST_DEVICE_ID           0x7091U
#define MODEM_PLATFORM_TEST_VIN                 "LDC12345678900001"
#define MODEM_PLATFORM_DIAL_NUMBER              "15978989439"

#define MODEM_PLATFORM_MSG_REGISTER             0x01U
#define MODEM_PLATFORM_MSG_HEARTBEAT            0x02U
#define MODEM_PLATFORM_MSG_MSD                  0x10U
#define MODEM_PLATFORM_REGISTER_JSON_FORMAT     "{\"role\":\"device\",\"deviceId\":\"%04X\"}"
#define MODEM_PLATFORM_HEARTBEAT_JSON_FORMAT    "{\"type\":\"heartbeat\",\"deviceId\":\"%04X\"}"

/* Timeouts are used by the future ModemPort AT transaction adapter. */
#define MODEM_AT_CFG_SYNC_TIMEOUT_MS            1000U
#define MODEM_AT_CFG_SIM_TIMEOUT_MS             1000U
#define MODEM_AT_CFG_NETWORK_TIMEOUT_MS         1000U
#define MODEM_AT_CFG_DIAL_TIMEOUT_MS            30000U
#define MODEM_AT_CFG_SEND_TIMEOUT_MS            2000U
#define MODEM_PLATFORM_HEARTBEAT_PERIOD_MS      25000U

/* The bare-metal ML307A platform uploads a hexadecimal MSD frame by MIPSEND. */
#define MODEM_AT_CFG_MSD_UPLOAD_SUPPORTED       1U

#endif
