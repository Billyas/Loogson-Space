#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

//-----------------------------------------------------------------------------

#ifdef __rtems__
#error "Cann't use RTEMS gcc, because of newlibc library."
#endif

//-----------------------------------------------------------------------------

//#define CPU_OSC_FREQ              24000000  // 24MHZ CPU_XTAL_FREQUENCY in "bsp.h"

#define RT_USING_NEWLIB

//-----------------------------------------------------------------------------

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

/* RT-Thread Kernel */

#define RT_NAME_MAX                 10
#define RT_ALIGN_SIZE               8
#define RT_THREAD_PRIORITY_MAX      32
#define RT_TICK_PER_SECOND          1000
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE      4
#define IDLE_THREAD_STACK_SIZE      1024
#define RT_DEBUG
#define RT_DEBUG_COLOR

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_MEMHEAP
#define RT_USING_SMALL_MEM
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_INTERRUPT_INFO
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE          128
//#define RT_CONSOLE_DEVICE_NAME    "uart5"
#define RT_VER_NUM                  0x40002

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE   (1024*8)      // default is 2048,
#define RT_MAIN_THREAD_PRIORITY     10

/* C++ features */


/* Command shell */

#define RT_USING_FINSH
#define FINSH_THREAD_NAME           "tshell"
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES         5
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_THREAD_PRIORITY       20
#define FINSH_THREAD_STACK_SIZE     (1024*8)
#define FINSH_CMD_SIZE              80
#define FINSH_USING_MSH
#define FINSH_USING_MSH_DEFAULT
#define FINSH_ARG_MAX               10

/* Device virtual file system */

#define RT_USING_DFS
#define DFS_USING_WORKDIR
#define DFS_FILESYSTEMS_MAX         2
#define DFS_FILESYSTEM_TYPES_MAX    2
#define DFS_FD_MAX                  4

/* Device Drivers */

//#define RT_USING_DEVICE_OPS
#define RT_USING_DEVICE_IPC
#define RT_PIPE_BUFSZ               512
//#define RT_USING_SERIAL
//#define RT_SERIAL_USING_DMA
//#define RT_SERIAL_RB_BUFSZ        64
//#define RT_USING_CAN
//#define RT_USING_I2C
//#define RT_USING_I2C_BITOPS
//#define RT_USING_PIN
//#define RT_USING_SPI
//#define RT_USING_SPI_MSD

/* Using USB */


/* POSIX layer and C standard library */

#define RT_USING_LIBC
#define RT_USING_PTHREADS
#define PTHREAD_NUM_MAX             8

/* Network */

/* Socket abstraction layer */


/* Network interface device */


/* light weight TCP/IP stack */

#define RT_LWIP_ETHTHREAD_PRIORITY  5

#define RT_LWIP_IPADDR              "192.168.5.64"
#define RT_LWIP_GWADDR              "192.168.5.1"
#define RT_LWIP_MSKADDR             "255.255.255.0"

#define RT_LWIP_DNS
#define RT_LWIP_ICMP
#define RT_LWIP_TCP
#define RT_LWIP_UDP

#define RT_LWIP_ETH_PAD_SIZE        2

#define RT_LWIP_DEBUG
#define RT_LWIP_ETHARP_DEBUG
#define RT_LWIP_TCPIP_DEBUG
#define RT_LWIP_NETIF_DEBUG
#define RT_LWIP_SOCKETS_DEBUG
#define RT_LWIP_DNS_DEBUG
//#define RT_LWIP_DHCP_DEBUG
#define RT_LWIP_IP_DEBUG
#define RT_LWIP_ICMP_DEBUG
#define RT_LWIP_UDP_DEBUG
#define RT_LWIP_TCP_DEBUG
#define RT_LWIP_TCP_INPUT_DEBUG
#define RT_LWIP_TCP_OUTPUT_DEBUG

/* AT commands */


/* VBUS(Virtual Software BUS) */


/* Utilities */


/* RT-Thread MIPS CPU */


/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* IoT Cloud */


/* security packages */


/* language packages */


/* multimedia packages */


/* tools packages */


/* system packages */


/* peripheral libraries and drivers */


/* miscellaneous packages */


/* samples: kernel and components samples */

//#define SOC_1B200
//#define RT_USING_SELF_BOOT
//#define RT_SELF_BOOT_DEBUG
//#define RT_USING_UART2
//#define RT_USING_UART3
//#define RT_USING_UART4
#define RT_USING_UART5
#define RT_UART_RX_BUFFER_SIZE      64
#define RT_USING_GMAC_INT_MODE
#define RT_USING_I2C1
#define RT_USING_I2C2
#define NO_TOUCH

#endif
