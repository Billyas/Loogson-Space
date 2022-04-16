/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include "arch/cc.h"

#if defined(OS_RTTHREAD)

//************************************************************************************************
// RT-Thread
//************************************************************************************************

#include <rtthread.h>

#define SYS_MBOX_NULL   RT_NULL
#define SYS_SEM_NULL    RT_NULL

#define SYS_MBOX_SIZE           8
#define SYS_LWIP_TIMER_NAME    "timer"
#define SYS_LWIP_MBOX_NAME     "mbox"
#define SYS_LWIP_SEM_NAME      "sem"
#define SYS_LWIP_MUTEX_NAME    "mu"

typedef rt_sem_t        sys_sem_t;
typedef rt_mutex_t      sys_mutex_t;
typedef rt_mailbox_t    sys_mbox_t;
typedef rt_thread_t     sys_thread_t;

#elif defined(OS_FREERTOS)

//************************************************************************************************
// FreeRTOS
//************************************************************************************************

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define SYS_MBOX_NULL       (xQueueHandle)0
#define SYS_SEM_NULL        (xSemaphoreHandle)0

typedef xSemaphoreHandle    sys_sem_t;
typedef xQueueHandle        sys_mbox_t;
typedef xTaskHandle         sys_thread_t;
typedef xSemaphoreHandle    sys_mutex_t;

/*
 * Message queue constants.
 */
#define archMESG_QUEUE_LENGTH	(6)

#define archPOST_BLOCK_TIME_MS	((unsigned portLONG)10000)

#elif defined(OS_UCOS)

//************************************************************************************************
// uCOSII
//************************************************************************************************

#include "os.h"

#define LWIP_MAX_TASKS  4           /* Number of LwIP tasks */
#define LWIP_STACK_SIZE 4096        /* Stack size for LwIP tasks */

#define LWIP_Q_SIZE     10          /* LwIP queue size */
#define LWIP_MAX_QS     20          /* Max. LwIP queues */

#define SYS_MBOX_NULL   NULL
#define SYS_SEM_NULL    NULL

typedef struct
{
    OS_EVENT *pQ;                       /* The mail queue itself. */
    void     *pvQEntries[LWIP_Q_SIZE];  /* The elements in the queue. */
    OS_EVENT *Q_full;                   /* The semaphore used to count the number of available slots. */
    int       is_valid;                 /* The validity flag. */
} sys_mbox_t;

typedef struct
{
    OS_EVENT *sem;
    int       is_valid;
} sys_sem_t;


typedef OS_EVENT *sys_mutex_t;
typedef INT8U     sys_thread_t;

#else // OS_NONE

//************************************************************************************************
// Bare Programming
//************************************************************************************************

/*
 * do nothing
 */

#endif

#endif /* __ARCH_SYS_ARCH_H__ */


