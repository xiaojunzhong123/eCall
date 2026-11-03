/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Eth_GeneralTypes.h
 *
 * Description: Implementation of ETH GENERALTYPES High Level layer
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
/**
 *   @file       Eth_GeneralTypes.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 ETH Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifndef _ETH_GENERAL_TYPES_H_
#define _ETH_GENERAL_TYPES_H_

#if defined(__cplusplus)
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "ComStackTypes.h"
#include "Std_Types.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief   Defines the type of the controller modes.
 *  @details SRS ID:     SWS_Eth_00158
 *           Design ID:  
 */
typedef enum
{
    ETH_MODE_DOWN   = 0x00U, /*!< Controller disable */
    ETH_MODE_ACTIVE = 0x01U  /*!< Controller enable */
} Eth_ModeType;

/**
 *  @brief   Defines the status supervision used for Development Error Detection. The state shall be available for
 *           debugging.
 *  @details SRS ID:     SWS_Eth_00159
 *           Design ID:  
 */
typedef enum
{
    ETH_STATE_UNINIT = 0x00U, /*!< The driver is not yet configured */
    ETH_STATE_INIT   = 0x01U  /*!< The driver is configured */
} Eth_StateType;

/**
 *  @brief   Defines the type of the Ethernet frame type used in the Ethernet frame header.
 *  @details SRS ID:     SWS_Eth_00160
 *           Design ID:  
 */
typedef uint16_t Eth_FrameType;

/**
 *  @brief   Defines the type of the Ethernet data type used for data transmission. Its definition depends on the used
 *           CPU.
 *  @details SRS ID:     SWS_Eth_00161
 *           Design ID:  
 */
typedef uint8_t Eth_DataType;

/**
 *  @brief   Defines the type of the Ethernet buffer identifier type.
 *  @details SRS ID:     SWS_Eth_00175
 *           Design ID:  
 */
typedef uint32_t Eth_BufIdxType;

/**
 *  @brief   Defines the type of the Ethernet Received status for used as out parameter in Eth_Received() indicates
 *           whether a frame has been received and if so, whether more frames are available or frames got lost.
 *  @details SRS ID:     SWS_Eth_00162
 *           Design ID:  
 */
typedef enum
{
    ETH_RECEIVED                     = 0x00U, /*!< Define the Ethernet frame has been received, no further frames available */
    ETH_NOT_RECEIVED                 = 0x01U, /*!< Define the Ethernet frame has not been received, no further frames available */
    ETH_RECEIVED_MORE_DATA_AVAILABLE = 0x02U  /*!< Define the Ethernet frame has been received,
                                                     more frame are available */
} Eth_RxStatusType;

/**
 *  @brief   Define the Enumeration Type Eth_FilterActionType describes the action to be taken for the MAC address given
 *           in PhysAddrPtr.
 *  @details SRS ID:     SWS_Eth_00163
 *           Design ID:  
 */
typedef enum
{
    ETH_ADD_TO_FILTER      = 0x00U, /*!< Add the MAC address to the filter, meaning allow reception */
    ETH_REMOVE_FROM_FILTER = 0x01U  /*!< Remove the MAC address from the filter, meaning reception is blocked in the
                                        lower layer */
} Eth_FilterActionType;

/**
 *  @brief   Depending on the HW, quality information regarding the evaluated time stam might be supported.
 *           If not supported, the value shall be always Valid. For Uncertain and Invalid values, the upper layer shall
 *           discard the time stamp.
 *  @details SRS ID:     SWS_Eth_00177
 *           Design ID:  
 */
typedef enum
{
    ETH_VALID     = 0x00U,
    ETH_INVALID   = 0x01U,
    ETH_UNCERTAIN = 0x02U
} Eth_TimeStampQualType;

/**
 *  @brief   The variables of this type are used for expressing time stamps including relative time and absolute calendar
 *           time. The absolute time starts at 1970-01-01.
 *  @details SRS ID:     SWS_Eth_00178
 *           Design ID:  
 */
typedef struct
{
    uint32_t nanoseconds;     /*!< The nanoseconds part of the time */
    uint32_t seconds;         /*!< The 32-bit LSB of the 48 bits Seconds part of the time */
    uint16_t secondsHi;       /*!< The 16-bit MSB of the 48 bits Seconds part of the time */
} Eth_TimeStampType;

/**
 *  @brief   The variables of this type are used to express time differrences.
 *  @details SRS ID:     SWS_Eth_00179
 *           Design ID:  
 */
typedef struct
{
    Eth_TimeStampType diff;    /*!< Time difference */
    boolean sign;              /*!< Positive (TRUE)/ negative (FALSE) time */
} Eth_TimeIntDiffType;

/**
 *  @brief   The variables of this type are used to express frequency ratios.
 *  @details SRS ID:     SWS_Eth_00180
 *           Design ID:  
 */
typedef struct
{
    Eth_TimeIntDiffType IngressTimeStampDelta; /*!< IngressTimestampSync2 - IngressTimestampSync1 */
    Eth_TimeIntDiffType OriginTimeStampDelta;  /*!< OriginTimeStampSync2[FUP2] - OriginTimeStampSync1[FUP1] */
} Eth_RateRatioType;

/**
 *  @brief   This type is used to read out addresses from the address resolution logic (ARL) table of the switch.
 *  @details SRS ID:     SWS_Eth_91001
 *           Design ID:  
 */
typedef struct
{
    uint8_t MacAddr[6];  /*!< Specifies the MAC address [0..255, 0..255, 0..255, 0..255, 0..255, 0..255] */
    uint16_t VlanId;     /*!< Specifies the VLAN address 0..65535 */
    uint32_t SwitchPort; /*!< Specifies the ports of the switch as bit mask (0x00000001 -> Port0, 0x80000001 -> Port31
                              + Port0) */
} Eth_MacVlanType;

/**
 *  @brief   Statistic counter for diagnostics.
 *  @details SRS ID:     SWS_Eth_91007
 *           Design ID:  
 */
typedef struct
{
    uint32_t DropPktBufOverrun; /*!< Dropped packets due to buffer overrun */
    uint32_t DropPktCrc;        /*!< Dropped packets due to CRC errors */
    uint32_t UndersizePkt;      /*!< Number of undersize packets which were less than 64 octet long (excluding framing
                                          bits, but including FCS octets) and were otherwise well formed. */
    uint32_t OversizePkt;       /*!< Number of oversize packets which are longer than 1518 octets (excluding framing bits,
                                    but including FCS octets) and were otherwise well formed. */
    uint32_t AlgnmtErr;         /*!< Number of alignment errors, i.e. packets which are received and are not an integral
                                    number of octets in length   and do not pass the CRC */
    uint32_t SqeTestErr;        /*!< SQE test error according to IETF RFC1643 dot3StatsSQETestErrors */
    uint32_t DiscInbdPkt;       /*!< The number of inbound packets which were chosen to be discarded even through no error
                                    had been detected to prevent their being deliverable to a higher-layer protocol. One
                                    possible reason for discarding such a packet could be to free up buffer space */
    uint32_t ErrInbdPkt;        /*!< Total number of erroneous inbound packets */
    uint32_t
        DiscOtbdPkt;    /*!< The number of outbound packets which were chosen to be discarded even through no errors
                                had been detected to prevent their being transmitted. One possible reason for discarding
                                such a packet could be to free up buffer space. */
    uint32_t ErrOtbdPkt;  /*!< Total number of erroneous outbound packets. */
    uint32_t SnglCollPkt; /*!< Single collision frames: A count of successfully transmitted frames on a particurlar
                               interface for which transmission is inhibited by exactly one collision. */
    uint32_t MultCollPkt; /*!< Multiple collision frames: A count of successfully transmitted frames on particular
                               interface for which transmission is inhibited by more than one collision. */
    uint32_t
        DfrdPkt;       /*!< Number of deferred transmission: A count of frames for which the first transmission attempt
                                 on a particular interface is delayed because the meduim is busy. */
    uint32_t LatCollPkt; /*!< Number of late collisions: The number of times that a collision is detected on a
                              particular interface later than 512 bit-times into the transmission of a packet. */
    uint32_t HwDepCtr0; /*!< Hardware dependent counter value */
    uint32_t HwDepCtr1; /*!< Hardware dependent counter value */
    uint32_t HwDepCtr2; /*!< Hardware dependent counter value */
    uint32_t HwDepCtr3; /*!< Hardware dependent counter value */
} Eth_CounterType;

/**
 *  @brief   Statistic counter for diagnostics.
 *  @details SRS ID:     SWS_Eth_91002
 *           Design ID:  
 */
typedef struct
{
    uint32_t RxStatsDropEvents;           /*!< The total number of events in which packets were dropped by the probe due to lack
                                                  of resource. Also described in IETF RFC 2819 MIB etherStatsDropEvents.  */
    uint32_t RxStatsOctets;               /*!< The total number of octets of data (including those in bad packets) received on the
                                              network (excluding framing bits but including FCS octets). Also described in IETF RFC
                                              2819 MIB etherStatsOctets. */
    uint32_t RxStatsPkts;                 /*!< The total number of packets (including bad packets, broadcast packets, and multicast
                                                 packets) received. Also described in IETF RFC 2819 MIB etherStatsPkts */
    uint32_t RxStatsBroadcastPkts;        /*!< The total number of good packets received that were directed to the broadcast
                                                address. Also described in IETF RFC 2819 MIB etherStatsBroadcastPkts */
    uint32_t RxStatsMulticastPkts;        /*!< The total number of good packets received that were directed to a multicast
                                                address. Also described in IETF RFC 2819 MIB etherStatsMulticastPkts.*/
    uint32_t RxStatsCrcAlignErrors;       /*!< The total number of packets received that had a length of bertween 64 and
                                            1518 octets that had either a bad Frame Check Sequence (FCS) with an integral
                                            number of octets (FCS Error) or a bad FCS with a non-integral number of octets
                                            (Alignment Error). Also described in IETF RFC 2819 MIB etherStatsCRCAlignErrors */
    uint32_t RxStatsUndersizePkts;        /*!< The total number of packets received that were less than 64 octets long
                                               (excluding framing bits, but including FCS octets) and were otherwise well
                                               formed. Also described in IETF RFC 2819 MIB etherStatsUndersizePkts. */
    uint32_t RxStatsOversizePkts;         /*!< The total number of packets received that were longer than 1518 octets
                                               (excluding framing bits, but including FCS octets) and were otherwise well
                                               formed. Also described in IETF RFC 2819 MIB etherStatsOversizePkts */
    uint32_t RxStatsFragments;            /*!< The total number of packets received that were less than 64 octets in length
                                               (excluding framing bits but including FCS octets) and had either a bad Frame Check
                                               Sequence (FCS) with an integral number of octets (FCS Error) or a bad FCS with a
                                               non-integral number of octets (Alignment Error). Also described in IETF RFC 2819 MIB
                                               etherStatsFragments. */
    uint32_t RxStatsJabbers;              /*!< The total number of packets received that were longer than 1518 octets, and had
                                                 either a bad Frame Check Sequence (FCS) with an integral number of octets (FCS Error)
                                                 or a bad FCS with a non-integral number of octets (Alignment Error). Also described in
                                                 IETF RFC 2819 MIB etherStatsJabbers. */
    uint32_t RxStatsCollisions;           /*!< The best estimate of the total number of collisions on this Ethernet segment.
                                                 Also described in IETF RFC 2819 MIB etherStatsCollisions*/
    uint32_t RxStatsPkts64Octets;         /*!< The total number of packets (including bad packets) received that were 64
                                          octets in length. Also described in IETF RFC 2819 MIB etherStatsPkts64Octets  */
    uint32_t RxStatsPkts65to127Octets;    /*!< The total number of packets (including bad packets) received that were
                                                  between 65 and 127 octets in length. Also described in IETF RFC 2819 MIB
                                                  etherStatsPkts65to127Octets */
    uint32_t RxStatsPkts128to255Octets;   /*!< The total number of packets (including bad packets) received that were
                                                between 128 and 255 octets in length. Also described in IETF RFC 2819 MIB
                                                etherStatsPkts128to255Octets */
    uint32_t RxStatsPkts256to511Octets;   /*!< The total number of packets (including bad packets) received that were
                                                between 256 and 511 octets in length. Also described in IETF RFC 2819 MIB
                                                etherStatsPkts256to511Octets */
    uint32_t RxStatsPkts512to1023Octets;  /*!<  The total number of packets (including bad packets) received that were
                                                between 512 and 1023 octets in length. Also described in IETF RFC 2819 MIB
                                                etherStatsPkts512to1023Octets */
    uint32_t RxStatsPkts1024to1518Octets; /*!< The total number of packets (including bad packets) received that were
                                               between 1024 and 1518 octets in length. Also described in IETF RFC 2819
                                               MIB etherStatsPkts1024to1518Octets*/
    uint32_t RxUnicastFrames;             /*!< The number of subnetwork-unicast packets delivered to a higher-layer protocol. Also
                                               described in IETF RFC1213 MIB ifInUcastPkts*/
} Eth_RxStatsType;

/**
 *  @brief   Statistic counter for diagnostics.
 *  @details SRS ID:     SWS_Eth_91003
 *           Design ID:  
 */
typedef struct
{
    uint32_t TxNumberOfOctets; /*!< The total number of octets transmitted out of the interface, including framing
                                    characters. Also described in IETF RFC1213 MIB ifOutOctets. */
    uint32_t TxNUcastPkts;     /*!< The total number of packets that higher level protocols requested be transmitted to a
                                  non-unicast (i.e., a subnetwork-broadcast or subnetwork-multicast) address, including those
                                  that were discarded or not sent. Also described in IETF RFC1213 MIB ifOutNUcastPkts. */
    uint32_t TxUniCastPkts;    /*!< The total number of packets that higher-level protocols requested be transmitted to a
                                 subnetwork-unicast address, including those that were discarded or not sent. Also
                                 described in IETF RFC1213 MIB ifOutUcastPkts. */
} Eth_TxStatsType;

/**
 *  @brief   Statistic counter for diagnostics.
 *  @details SRS ID:     SWS_Eth_91004
 *           Design ID:  
 */
typedef struct
{
    uint32_t TxDroppedNoErrorPkts; /*!< The number of outbound packets which were chosen to be discarded even though
                                        no errors had been detected to prevent their being transmitted. One possible
                                        reason for discarding such a packet could be to free up buffer space. Also
                                        described in IETF RFC1213 MIB ifOutDiscards */
    uint32_t TxDroppedErrorPkts;   /*!< transmitted because of errors. Also described in IETF RFC1213 MIB ifOutErrors */
    uint32_t TxDeferredTrans;      /*!< A count of frames for which the first transmission attempt on a particular
                                        interface is delayed because the medium is busy. The count represented by an instance
                                        of this object does not include frames involved in collisions. Also described in IETF
                                        RFC1643 MIB dot3StatsDeferredTransmissions */
    uint32_t TxSingleCollision;    /*!< A count of successfully transmitted frames on a particular interface for which
                                        transmission is inhibited by exactly one collision. A frame that is counted by an
                                        instance of this object is also counted by the corresponding instance of either the
                                        TxUniCastPkts and TxNUcastPkts and is not counted by the corresponding instance of
                                        the TxMultipleCollision object. Also described in IETF RFC1643 MIB
                                        dot3StatsSingleCollisionFrames*/
    uint32_t TxMultipleCollision;  /*!< A count of successfully transmitted frames on a particular interface for which
                                        transmission is inhibited by more than one collision. A frame that is counted by
                                        an instance of this object is also counted by the corresponding instance of
                                        either the TxUniCastPkts and TxNUcastPkts and is not counted by the corresponding
                                        instance of the TxSingleCollision object. Also described in IETF RFC1643 MIB
                                        dot3StatsMultipleCollisionFrames. */
    uint32_t TxLateCollision;      /*!< The number of times that a collision is detected on a particular interface later
                                        than 512 bit-times into the transmission of a packet. Five hundred and twelve
                                        bit-times corresponds to 51.2 microseconds on a 10 Mbit/s system. A (late) collision
                                        included in a count represented by an instance of this object is also considered as a
                                        (generic) collision for purposes of other collision-related statistics. Also
                                        described in IETF RFC1643 MIB dot3StatsLateCollisions */
    uint32_t TxExcessiveCollison; /*!<  A count of frames for which transmission on a particular interface fails due
                                        to excessive collisions. Also described in IETF RFC1643 MIB
                                        dot3StatsExcessiveCollisions */
} Eth_TxErrorCounterValuesType;

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#if defined(__cplusplus)
}
#endif
/*! @}*/

#endif /*!< _ETH_GENERAL_TYPES_H_ */
