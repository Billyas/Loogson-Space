/**
 * @file
 *
 * lwIP Options Configuration
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#include "bsp.h"

//-----------------------------------------------------------------------------
// Thread Support
//-----------------------------------------------------------------------------

/**
 * TCPIP_THREAD_NAME: The name assigned to the main tcpip thread.
 */
#if defined(OS_RTTHREAD)
#define TCPIP_THREAD_NAME              "tcpip_rtt"
#elif defined(OS_FREERTOS)
#define TCPIP_THREAD_NAME              "tcpip_free"
#elif defined(OS_UCOS)
#define TCPIP_THREAD_NAME              "tcpip_ucos"
#endif

/**
 * TCPIP_THREAD_STACKSIZE: The stack size used by the main tcpip thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#define TCPIP_THREAD_STACKSIZE          (8*1024)

/**
 * TCPIP_THREAD_PRIO: The priority assigned to the main tcpip thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#if defined(OS_RTTHREAD)
#define TCPIP_THREAD_PRIO               7
#elif defined(OS_FREERTOS)
#define TCPIP_THREAD_PRIO               (configMAX_PRIORITIES - 2)
#elif defined(OS_UCOS)
#define TCPIP_THREAD_PRIO               12
#endif

/**
 * TCPIP_MBOX_SIZE: The mailbox size for the tcpip thread messages
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when tcpip_init is called.
 */
#define TCPIP_MBOX_SIZE                 4

#define DEFAULT_RAW_RECVMBOX_SIZE       4

#define DEFAULT_UDP_RECVMBOX_SIZE       4

#define DEFAULT_TCP_RECVMBOX_SIZE       4

#define DEFAULT_ACCEPTMBOX_SIZE         4

/**
 * DEFAULT_THREAD_STACKSIZE: The stack size used by any other lwIP thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#define DEFAULT_THREAD_STACKSIZE        (4*1024)

/**
 * DEFAULT_THREAD_PRIO: The priority assigned to any other lwIP thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#if defined(OS_RTTHREAD)
#define DEFAULT_THREAD_PRIO             8
#elif defined(OS_FREERTOS)
#define DEFAULT_THREAD_PRIO             (configMAX_PRIORITIES - 3)
#elif defined(OS_UCOS)
#define DEFAULT_THREAD_PRIO             13
#endif

//-----------------------------------------------------------------------------

/**
 * MEM_LIBC_MALLOC==1: Use malloc/free/realloc provided by your C-library
 * instead of the lwip internal allocator. Can save code size if you
 * already use it.
 */
#define MEM_LIBC_MALLOC                 1
#if defined(OS_RTTHREAD)
#define mem_malloc          rt_malloc
#define mem_calloc          rt_calloc
#define mem_free            rt_free
#else
#define mem_malloc          malloc
#define mem_calloc          calloc
#define mem_free            free
#endif

//-----------------------------------------------------------------------------

/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */
#if defined(OS_RTTHREAD)
#define NO_SYS                          0   // We are running RT-Thread
#elif defined(OS_FREERTOS)
#define NO_SYS                          0   // We are running FreeRTOS
#elif defined(OS_UCOS)
#define NO_SYS                          0   // We are running uCOSII
#else // OS_NONE
#define NO_SYS                          1   // We are bare-programming
#endif

/**
 * NO_SYS_NO_TIMERS==1: Drop support for sys_timeout when NO_SYS==1
 * Mainly for compatibility to old versions.
 */
#define NO_SYS_NO_TIMERS                0

/**
 * MEM_ALIGNMENT: should be set to the alignment of the CPU
 */
#define MEM_ALIGNMENT                   4

/**
 * MEM_SIZE: the size of the heap memory. If the application will send
 * a lot of data that needs to be copied, this should be set high.
 */
#define MEM_SIZE                        (32*1024)

/**
 * ETH_PAD_SIZE: number of bytes added before the ethernet header to ensure
 * alignment of payload after that header. Since the header is 14 bytes long,
 * without this padding e.g. addresses in the IP header will not be aligned
 * on a 32-bit boundary, so setting this to 2 can speed up 32-bit-platforms.
 */
#define ETH_PAD_SIZE					2

/**
 * LWIP_ARP==1: Enable ARP functionality.
 */
#define LWIP_ARP                        1

/**
 * LWIP_DHCP==1: Enable DHCP module.
 */
#define LWIP_DHCP                       0

/**
 * LWIP_ICMP==1: Enable ICMP module inside the IP stack.
 * Be careful, disable that make your product non-compliant to RFC1122
 */
#define LWIP_ICMP                       1

/**
 * LWIP_TCP==1: Turn on TCP.
 */
#define LWIP_TCP                        1
/**
 * TCP_MSS: TCP Maximum segment size. (default is 536, a conservative default,
 * you might want to increase this.)
 * For the receive side, this MSS is advertised to the remote side
 * when opening a connection. For the transmit size, this MSS sets
 * an upper limit on the MSS advertised by the remote host.
 */
#define TCP_MSS                 		(1500 - 40)

/**
 * LWIP_UDP==1: Turn on UDP.
 */
#define LWIP_UDP                        1

/**
 * LWIP_SNMP==1: Turn on SNMP module. UDP must be available for SNMP
 * transport.
 */
#define LWIP_SNMP                       0

/**
 * LWIP_IGMP==1: Turn on IGMP module.
 */
#define LWIP_IGMP                       0

/**
 * LWIP_DNS==1: Turn on DNS module. UDP must be available for DNS
 * transport.
 */
#define LWIP_DNS                        0

/**
 * LWIP_HAVE_LOOPIF==1: Support loop interface (127.0.0.1) and loopif.c
 */
#define LWIP_HAVE_LOOPIF                0

/**
 * LWIP_HAVE_SLIPIF==1: Support slip interface and slipif.c
 */
#define LWIP_HAVE_SLIPIF                0

/**
 * PPP_SUPPORT==1: Enable PPP.
 */
#define PPP_SUPPORT                     0

/**
 * LWIP_STATS==1: Enable statistics collection in lwip_stats.
 */
#define LWIP_STATS                      0

/**
 * LWIP_RAW==1: Enable application layer to hook into the IP layer itself.
 */
#define LWIP_RAW                        (!BSP_USE_OS)

/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET                     (BSP_USE_OS)

#define LWIP_SO_SNDTIMEO                1           // socket send timeout

#define LWIP_SO_RCVTIMEO                1           // socket receive timeout

/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN                    (BSP_USE_OS)

/**
 *  #define LWIP_DEBUG: Enable LWIP Debug
 */
#define LWIP_DEBUG
#define ETHARP_DEBUG                    LWIP_DBG_OFF
#define TCPIP_DEBUG                     LWIP_DBG_OFF
#define IP_DEBUG                        LWIP_DBG_OFF
#define TCP_DEBUG                       LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG                LWIP_DBG_OFF
#define TCP_INPUT_DEBUG                 LWIP_DBG_OFF
#define UDP_DEBUG                       LWIP_DBG_OFF
#define SOCKETS_DEBUG                   LWIP_DBG_OFF
#define INET_DEBUG                      LWIP_DBG_OFF

#endif /* __LWIPOPTS_H__ */

