/*******************************************************
  * Name    :CanMatrix_Cfg.h
  * Author  :xiaojunzhong
  * Function:TBOX CAN matrix configuration
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
/* Source: CAKY_TBOX_CANFD_TBOX_20261014_182806_V1.5.xlsx,
 * sheet TBOX_TBOX_CANFD. Each message/signal cites its source row below.
 * Def.h is used unchanged. All 55 layouts were checked against matrix bits.
 *
 * Ó¦ÓÃ±í½öº¬ 38 Rx + 12 Tx¡£2 ¸ö NM ºÍ 3 ¸öÕï¶Ï±¨ÎÄÓÉ×¨ÓÃÄ£¿é´¦Àí¡£
 * Íâ²¿ÅäÖÃÐèÍ¬²½£ºCANNM_ID=0x405£¬UDS_PHYS_ADDR_ID=0x705£¬
 * UDS_RESP_ADDR_ID=0x785£»UDS_FUNC_ADDR_ID=0x7DF¡£±¾ÎÄ¼þÎ´ÐÞ¸ÄÍâ²¿Ä£¿é¡£
 * ±ØÐëÍ¬Ê±Ìæ»»ÅäÌ× CanMatrix.c£»¾ÉºËÐÄ²»Ö§³Ö´Ë°æ±¾µÄ³¬Ê±Óë·¢ËÍÐÐÎª¡£
 * RxList/TxList ²ÎÊýÖ±½ÓÌîÐ´ÊýÖµ£»ÐÞ¸ÄÖÜÆÚ¡¢³¤¶È»ò³¬Ê±Çë±à¼­ Cfg.c µÄ±í¡£
 * ÖÜÆÚ±¨ÎÄ°´ CANFD ¹æ·¶ 7.6 µÄÍ¨ÐÅ³¬Ê±¹æÔòÅäÖÃ£»ÊÂ¼þ±¨ÎÄ²»×öÖÜÆÚ³¬Ê±¡£
 * ±¾¹æÔòÊÇÍ¨ÐÅ¼à²â»ùÏß£¬²»Ìæ´ú¹¦ÄÜÌØ¶¨µÄÐÅºÅÐÂÏÊ¶ÈÒªÇó¡£
 * ÒÑÅäÖÃ CCU ¹Ø¼ü±¨ÎÄ 0x116 µÄ½Úµã×´Ì¬¼à²â£¬²»¼Ù¶¨Íø¹ØÔ´½ÚµãÔÚÏß¡£
 * legacy DTC ±íÈÔÎª 0£»CCU ×´Ì¬Í¨¹ý»Øµ÷/²éÑ¯Ìá¹©£¬Ðè°ó¶¨ DTC ·þÎñ¡£
 * ÉÏ²ãÐèÍ¨¹ý CanMatrix_SetRxMonitorAllowed ½ÓÈëÉÏµç¡¢¹©µçÓë»Ö¸´ÃÅ¿Ø¡£
 * ³õÊ¼¼à¿Ø¹Ø±Õ£»RTE °ó¶¨¡¢CRC/¼ÆÊýÆ÷ÑéÖ¤Óë¸üÐÂÈÔÐè½ÓÈë¡£
 * ÐÅºÅ¿Õ°×³õÊ¼Öµ¼°Î´¶¨ÒåµÄ±£ÁôÎ»ÔÝÓÃ 0£¬ÒÑÔÚ³õÊ¼»¯´úÂëÖÐÖðÏî±ê×¢¡£
 */
#ifndef _CANMATRIX_CFG_H_
#define _CANMATRIX_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Rte.h"
#include "Can.h"
#include "RteSys.h"
#include "CanNM.h"
#include "CanIf.h"
#include "Memory.h"
#include "CanMatrix_Def.h"
#include "CanTransciever.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"

/*-------------define---------------------------------*/
#define CANMATRIX_DTC_LIST_NUM                            0
#define CANMATRIX_RX_LIST_NUM                             38
#define CANMATRIX_TX_LIST_NUM                             12
#define CANMATRIX_CTR_DTC_TIMES                           1
#define CANMATRIX_RX_SCHEDULE_TIME                        10
#define CANMATRIX_TX_SCHEDULE_TIME                        2

/* Inactive storage keeps the existing core compatible with standard C arrays.
 * Iterate CANMATRIX_DTC_LIST_NUM, never this storage count.
 */
#define CANMATRIX_DTC_LIST_STORAGE_NUM 1

/* CANFD 7.6: one directly received shortest-period key message for CCU.
 * Multi-message node groups use ALL monitored messages lost, not ANY lost.
 * Do not add gateway-proxied nodes without confirming source Timeout_Flag /
 * invalid-value handling and the vehicle-specific routing table.
 */
#define CANMATRIX_NODE_MONITOR_LIST_NUM 1U
#define CANMATRIX_DTC_CCU_LOST 0xC0EA87UL

/*-------------function port--------------------------*/
/*-------------Rte port-------------------------------*/
#define CanMatrix_SetDtcCurErrFlag(id, flag)              Rte_SetDtcCurErrFlag(id, flag)
/*-------------RteSys port----------------------------*/
#define CanMatrix_GetDtcDeactiveFlag()                    RteSys_GetDtcDeactiveFlag()
/*-------------Can------------------------------------*/
#define CanMatrix_CheckIsInNoAck()                        Can_GetNoAckFlag()
#define CanMatrix_CheckIsInBusOff()                       Can_GetBusOffFlag()
/*-------------CanIf port-----------------------------*/
#define CanMatrix_ReadRxMsg(p_frame, p_id, p_buff, p_len) CanIf_ReadAppRxMsg(p_frame, p_id, p_buff, p_len)
#define CanMatrix_WriteTxMsg(frame, id, p_buff, len)      CanIf_WriteAppTxMsg(frame, id, p_buff, len)
/*-------------Memory---------------------------------*/
#define CanMatrix_Compare(p_des, p_src, len)              Memory_Compare(p_des, p_src, len)
#define CanMatrix_Copy(p_buff, p_src, len)                Memory_Copy(p_buff, p_src, len)
#define CanMatrix_Fill(p_buff, data, len)                 Memory_Fill(p_buff, data, len)
/*-------------CanTransciever port---------------------*/
#define CanMatrix_CheckStartAppTimerIsOk()                CanTransciever_CheckStartAppTimerIsOk()
/*-------------CanNM port------------------------------*/
#define CanMatrix_CheckNMIfAllowTxMsg()                   CanNM_CheckIfAllowTxMsg()
/*-------------enum and struct-------------------------*/
typedef enum
{
    CANMATRIX_STATE_INIT   = 0x00,
    CANMATRIX_STATE_NORMAL = 0x01,
    CANMATRIX_STATE_OFF    = 0x02
} CanMatrix_State_Type;

typedef enum
{
    CANMATRIX_RX_STATE_INIT = 0x00,
    CANMATRIX_RX_STATE_NORMAL,
    CANMATRIX_RX_STATE_LOST
} CanMatrix_RxState_Type;

typedef enum
{
    CANMATRIX_TX_STATE_CHECK = 0x00,
    CANMATRIX_TX_STATE_UPDATE,
    CANMATRIX_TX_STATE_CLEAR
} CanMatrix_TxState_Type;

typedef void    (*CanMatrix_DtcHandle_Fun)(void);
typedef void    (*CanMatrix_RxHandle_Fun)(CanMatrix_RxState_Type state);
typedef uint8_t (*CanMatrix_TxHandle_Fun)(CanMatrix_TxState_Type state);

typedef struct
{
    Bool     flag;
    Bool     lost;
    uint8_t  tick;
    uint16_t time;
} CanMatrix_DtcMsgInfo_TYPE;

typedef struct
{
    Bool     flag;
    Bool     lost;
    uint16_t time;
} CanMatrix_RxMsgInfo_Type;

typedef struct
{
    Bool     txFlag;
    uint8_t  fastTick;
    uint16_t timeTick;
    uint8_t  txTimes;
} CanMatrix_TxMsgInfo_Type;

typedef struct
{
    const uint32_t          id;
    const uint16_t          lostTime;
    Bool                   *lost;
    CanMatrix_DtcHandle_Fun dataHandleFun;
} CanMatrix_DtcList_Type;

typedef struct
{
    const Frame_Type             frame;
    const uint32_t               id;
    const uint16_t               lostTime;
    const uint8_t                len;
    const Bool                   IsChangeIn;
    uint8_t                     *p_buff;
    const CanMatrix_RxHandle_Fun dataHandleFun;
} CanMatrix_RxList_Type;

typedef struct
{
    const Frame_Type             frame;
    const uint32_t               id;
    const uint8_t                fastPeroid;
    const uint16_t               peroid;
    const uint8_t                len;
    const Msg_Type               type;
    uint8_t                     *p_buff;
    const CanMatrix_TxHandle_Fun dataHandleFun;
} CanMatrix_TxList_Type;
/*-------------node monitor types---------------------*/
typedef enum
{
    CANMATRIX_NODE_UNKNOWN = 0,
    CANMATRIX_NODE_NORMAL,
    CANMATRIX_NODE_LOST
} CanMatrix_NodeState_Type;

typedef void (*CanMatrix_NodeStateHandle_Fun)(uint32_t dtc, CanMatrix_NodeState_Type state);

typedef struct
{
    const uint32_t dtc;
    const uint8_t *rxIndices;
    const uint8_t rxCount;
} CanMatrix_NodeMonitorList_Type;

/*-------------function statement---------------------*/
/* Initialize matrix buffers to signal initial values (blank cells use zero). */
extern void CanMatrix_DefaultDataInit(void);
/* Request one event transmission after populating the matching Tx buffer.
 * Called only by the same task that owns CanMatrix_TxMsgMain, after NM allows Tx.
 * Repeated requests before CHECK coalesce. Returns FALSE for non-event IDs.
 * Never call from an ISR or another task without caller-provided synchronization.
 */
extern Bool CanMatrix_RequestEventTx(uint32_t id);
/* These calls share the same single CAN owner-task contract as event requests.
 * MonitorAllowed must incorporate KL15/power validity, startup delay and the
 * OEM-approved recovery delay. Default FALSE prevents invented timing inputs.
 * UNKNOWN means monitoring is inhibited/unqualified; never treat it as recovery.
 * The handler is a state notification, not a DTC persistence implementation.
 */
extern void CanMatrix_Init(void);
extern Bool CanMatrix_CheckIfRxMsg(uint32_t id);
extern void CanMatrix_RxMsgMain(void);
extern void CanMatrix_TxMsgMain(void);
extern void CanMatrix_SetRxMonitorAllowed(Bool allowed);
extern void CanMatrix_RegisterNodeStateHandler(CanMatrix_NodeStateHandle_Fun handler);
extern Bool CanMatrix_GetNodeState(uint32_t dtc, CanMatrix_NodeState_Type *state);
/* Internal bridge: called by the matching core when Tx is inhibited. */
extern void CanMatrix_ClearPendingEventTx(void);
extern const CanMatrix_NodeMonitorList_Type g_CanMatrixNodeMonitorList[CANMATRIX_NODE_MONITOR_LIST_NUM];
/*-------------variable statement---------------------*/
extern CanMatrix_DtcList_Type g_CanMatrixDtcList[CANMATRIX_DTC_LIST_STORAGE_NUM];
extern CanMatrix_RxList_Type  g_CanMatrixRxList[CANMATRIX_RX_LIST_NUM];
extern CanMatrix_TxList_Type  g_CanMatrixTxList[CANMATRIX_TX_LIST_NUM];
#endif

