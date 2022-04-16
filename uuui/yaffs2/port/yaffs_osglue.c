/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <errno.h>

#include "../yaffs_trace.h"
#include "../direct/yaffs_osglue.h"

#include "bsp.h"

/*
 * Mutext support
 */
#if defined(OS_RTTHREAD)
#include "rtthread.h"
static rt_mutex_t yaffs_mutex = NULL;
#elif defined(OS_FREERTOS)
#include "FreeRTOS.h"
#include "semphr.h"
static SemaphoreHandle_t yaffs_mutex = NULL;
#elif defined(OS_UCOS)
#include "os.h"
static OS_EVENT *yaffs_mutex = NULL;
#else
static int yaffs_mutex = 0;
#endif

/*
 * Dynamic memory alloc
 */
#if defined(OS_RTTHREAD)
#define MALLOC  rt_malloc
#define FREE    rt_free
#else
#define MALLOC  malloc
#define FREE    free
#endif

extern struct yaffs_dev* p_YAFFS_device;

unsigned int yaffs_trace_mask = YAFFS_TRACE_BAD_BLOCKS | YAFFS_TRACE_ALWAYS;

unsigned int yaffs_wr_attempts;

void *yaffsfs_malloc(unsigned int size)
{
	return MALLOC(size);
}

void yaffsfs_free(void *ptr)
{
	FREE(ptr);
}

u32 yaffsfs_CurrentTime(void)
{
    /*
     * 与文件日期有关, TODO read from rtc
     */
#if defined(OS_RTTHREAD)
    return rt_tick_get();
#elif defined(OS_FREERTOS)
    return xTaskGetTickCount();
#elif defined(OS_UCOS)
    return OSTimeGet();
#else
    return get_clock_ticks();
#endif
}

/**********************************************************
 * extern dev & lock & unlock.
 **********************************************************/

void yaffsfs_Lock(void)
{
#if defined(OS_RTTHREAD)
    rt_mutex_take(yaffs_mutex, RT_WAITING_FOREVER);
#elif defined(OS_FREERTOS)
    xSemaphoreTake(yaffs_mutex, portMAX_DELAY);
#elif defined(OS_UCOS)
    unsigned char err;
    if (OSRunning == OS_TRUE)
        OSMutexPend(yaffs_mutex, ~0, &err);
#else
    yaffs_mutex++;
#endif
}

void yaffsfs_Unlock(void)
{
#if defined(OS_RTTHREAD)
    rt_mutex_release(yaffs_mutex);
#elif defined(OS_FREERTOS)
    xSemaphoreGive(yaffs_mutex);
#elif defined(OS_UCOS)
    if (OSRunning == OS_TRUE)
        OSMutexPost(yaffs_mutex);
#else
    yaffs_mutex--;
#endif
}

void yaffsfs_SetError(int err)
{
	errno = err;
}

int yaffsfs_GetLastError(void)
{
	return errno;
}

//-------------------------------------------------------------------------------------------------
// initialize mutex
//-------------------------------------------------------------------------------------------------

int yaffsfs_init_mutex(void)
{
#if defined(OS_RTTHREAD)
    if (yaffs_mutex == NULL)
        yaffs_mutex = rt_mutex_create("yaffs2", RT_IPC_FLAG_FIFO);
    return yaffs_mutex == NULL ? 1 : 0;
#elif defined(OS_FREERTOS)
    if (yaffs_mutex == NULL)
        yaffs_mutex = xSemaphoreCreateMutex();
    return yaffs_mutex == NULL ? 1 : 0;
#elif defined(OS_UCOS)
    unsigned char err;
    if (yaffs_mutex == NULL)
        yaffs_mutex = OSMutexCreate(6, &err);
    return yaffs_mutex == NULL ? 1 : 0;
#else
    yaffs_mutex = 0;
    return 0;
#endif
}


