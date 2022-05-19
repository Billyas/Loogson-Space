/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2020-04-29    springcity      the first version
 */

#ifndef _UMQTT_CFG_H__
#define _UMQTT_CFG_H__

#define PKG_UMQTT_PROTOCOL_NAME                         ("MQTC")
#define PKG_UMQTT_PROTOCOL_NAME_LEN                     (rt_strlen(PKG_UMQTT_PROTOCOL_NAME))
#define PKG_UMQTT_PROTOCOL_LEVEL                        (4)             /* MQTT3.1.1 ver_lvl:4;  MQTT3.1 ver_lvl:3 */

#ifdef PKG_UMQTT_WILL_TOPIC_STRING
#define UMQTT_WILL_TOPIC                                PKG_UMQTT_WILL_TOPIC_STRING
#else
#define UMQTT_WILL_TOPIC                                ("/umqtt/test")
#endif

#ifdef PKG_UMQTT_WILL_MESSAGE_STRING
#define UMQTT_WILL_MWSSAGE                              PKG_UMQTT_WILL_MESSAGE_STRING
#else
#define UMQTT_WILL_MESSAGE                              ("Goodbye!")
#endif

#define UMQTT_INFO_DEF_THREAD_TICK                      50
#define UMQTT_MAX_PACKET_ID                             65535
#define UMQTT_INFO_DEF_UPLINK_TIMER_TICK                1000

#ifndef PKG_UMQTT_PUBLISH_RECON_MAX
#define PKG_UMQTT_PUBLISH_RECON_MAX                     3
#endif
#ifndef PKG_UMQTT_QOS2_QUE_MAX
#define PKG_UMQTT_QOS2_QUE_MAX                          1
#endif
#define PKG_UMQTT_RECPUBREC_INTERVAL_TIME               (2 * UMQTT_INFO_DEF_UPLINK_TIMER_TICK)
#define PKG_UMQTT_SUBRECV_DEF_LENGTH                    4
#define PKG_UMQTT_CONNECT_KEEPALIVE_DEF_TIME            0xffff
#define PKG_UMQTT_MSG_QUEUE_ACK_DEF_SIZE                4
#define PKG_UMQTT_INFO_DEF_SENDSIZE                     1024
#define PKG_UMQTT_INFO_DEF_RECVSIZE                     1024
#define PKG_UMQTT_INFO_DEF_RECONNECT_MAX_NUM            5
#define PKG_UMQTT_INFO_DEF_RECONNECT_INTERVAL           60
#define PKG_UMQTT_INFO_DEF_KEEPALIVE_MAX_NUM            5
#define PKG_UMQTT_INFO_DEF_HEARTBEAT_INTERVAL           30
#define PKG_UMQTT_INFO_DEF_CONNECT_TIMEOUT              4
#define PKG_UMQTT_INFO_DEF_RECV_TIMEOUT_MS              100
#define PKG_UMQTT_INFO_DEF_SEND_TIMEOUT                 4
#define PKG_UMQTT_INFO_DEF_THREAD_STACK_SIZE            2048
#define PKG_UMQTT_INFO_DEF_THREAD_PRIORITY              8
#endif
