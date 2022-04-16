/**
 * \file            lwmem_sys_template.c
 * \brief           System functions template file
 */

/*
 * Copyright (c) 2020 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LwMEM - Lightweight dynamic memory manager library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v1.3.0
 */

#include "bsp.h"

#if (BSP_USE_LWMEM)

#include "lwmem_sys.h"

#if LWMEM_CFG_OS || __DOXYGEN__

#if defined(OS_RTTHREAD)
#include "rtthread.h"
#elif defined(OS_UCOS)
#include "os.h"
#elif defined(OS_FREERTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#else
#error "no OS defined!"
#endif

/**
 * \brief           Create a new mutex and assign value to handle
 * \param[out]      m: Output variable to save mutex handle
 * \return          `1` on success, `0` otherwise
 */
uint8_t lwmem_sys_mutex_create(LWMEM_CFG_OS_MUTEX_HANDLE* m)
{
#if defined(OS_RTTHREAD)
    *m = (LWMEM_CFG_OS_MUTEX_HANDLE *)rt_mutex_create("lwmem", RT_IPC_FLAG_FIFO);
#elif defined(OS_UCOS)
    unsigned char err;
    *m = (LWMEM_CFG_OS_MUTEX_HANDLE *)OSMutexCreate(1, &err);
#elif defined(OS_FREERTOS)
    *m = (LWMEM_CFG_OS_MUTEX_HANDLE *)xSemaphoreCreateMutex();
#else
    *m = NULL;
#endif

    return *m == NULL ? 0 : 1;
}

/**
 * \brief           Check if mutex handle is valid
 * \param[in]       m: Mutex handle to check if valid
 * \return          `1` on success, `0` otherwise
 */
uint8_t lwmem_sys_mutex_isvalid(LWMEM_CFG_OS_MUTEX_HANDLE* m)
{
    return *m == NULL ? 0 : 1;
}

/**
 * \brief           Wait for a mutex until ready (unlimited time)
 * \param[in]       m: Mutex handle to wait for
 * \return          `1` on success, `0` otherwise
 */
uint8_t lwmem_sys_mutex_wait(LWMEM_CFG_OS_MUTEX_HANDLE* m)
{
#if defined(OS_RTTHREAD)
    rt_mutex_take((rt_mutex_t)m, RT_WAITING_FOREVER);
#elif defined(OS_UCOS)
    unsigned char err;
    OSMutexPend(p->dev_mutex, ~0, &err);
#elif defined(OS_FREERTOS)
    xSemaphoreTake((SemaphoreHandle_t)m, portMAX_DELAY);
#else

#endif

    return 1;
}

/**
 * \brief           Release already locked mutex
 * \param[in]       m: Mutex handle to release
 * \return          `1` on success, `0` otherwise
 */
uint8_t lwmem_sys_mutex_release(LWMEM_CFG_OS_MUTEX_HANDLE* m)
{
#if defined(OS_RTTHREAD)
    rt_mutex_release((rt_mutex_t)m);
#elif defined(OS_UCOS)
	OSMutexPost(p->dev_mutex);
#elif defined(OS_FREERTOS)
    xSemaphoreGive((SemaphoreHandle_t)m);
#else

#endif

    return 1;
}

#endif /* LWMEM_CFG_OS || __DOXYGEN__ */

//-------------------------------------------------------------------------------------
// Initialize Application HEAP
//-------------------------------------------------------------------------------------

#include "cpu.h"
#include "lwmem.h"

static lwmem_region_t m_region;

/*
 * 把全部剩余空间用作 heap, 可以改成部分RAM...
 */
void lwmem_initialize(unsigned int size)
{
    extern char end[];              /* end in "ld.script" */
    unsigned int endAddr;

    endAddr             = ((unsigned int)end + 4) & ~0x03;
    m_region.start_addr = (void *)endAddr;
    m_region.size       = get_memory_size() - K0_TO_PHYS(endAddr);

    lwmem_assignmem(&m_region, 1);
}

#endif // #if (BSP_USE_LWMEM)


