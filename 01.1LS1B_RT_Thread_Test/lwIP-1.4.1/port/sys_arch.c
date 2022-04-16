/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
 
#include "arch/sys_arch.h"

#if defined(OS_RTTHREAD)

//************************************************************************************************
// RT-Thread
//************************************************************************************************

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    static unsigned short counter = 0;
    char tname[RT_NAME_MAX];
    sys_mbox_t tmpmbox;

    RT_DEBUG_NOT_IN_INTERRUPT;

    rt_snprintf(tname, RT_NAME_MAX, "%s%d", SYS_LWIP_MBOX_NAME, counter);
    counter ++;

    tmpmbox = rt_mb_create(tname, size, RT_IPC_FLAG_FIFO);
    if (tmpmbox != RT_NULL)
    {
        *mbox = tmpmbox;

        return ERR_OK;
    }

    return ERR_MEM;
}

/*-----------------------------------------------------------------------------------*/

void sys_mbox_free(sys_mbox_t *mbox)
{
    RT_DEBUG_NOT_IN_INTERRUPT;

    rt_mb_delete(*mbox);

    return;
}

/*-----------------------------------------------------------------------------------*/

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    RT_DEBUG_NOT_IN_INTERRUPT;

    rt_mb_send_wait(*mbox, (rt_uint32_t)msg, RT_WAITING_FOREVER);

    return;
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    if (rt_mb_send(*mbox, (rt_uint32_t)msg) == RT_EOK)
        return ERR_OK;

    return ERR_MEM;
}

/*-----------------------------------------------------------------------------------*/

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
    rt_err_t ret;
    s32_t t;
    u32_t tick;

    RT_DEBUG_NOT_IN_INTERRUPT;

    /* get the begin tick */
    tick = rt_tick_get();

    if(timeout == 0)
        t = RT_WAITING_FOREVER;
    else
    {
        /* convirt msecond to os tick */
        if (timeout < (1000/RT_TICK_PER_SECOND))
            t = 1;
        else
            t = timeout / (1000/RT_TICK_PER_SECOND);
    }

    ret = rt_mb_recv(*mbox, (rt_ubase_t *)msg, t);
    if (ret == -RT_ETIMEOUT)
    {
        return SYS_ARCH_TIMEOUT;
    }
    else
    {
        LWIP_ASSERT("rt_mb_recv returned with error!", ret == RT_EOK);
    }

    /* get elapse msecond */
    tick = rt_tick_get() - tick;

    /* convert tick to msecond */
    tick = tick * (1000 / RT_TICK_PER_SECOND);
    if (tick == 0)
        tick = 1;

    return tick;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    int ret;

    ret = rt_mb_recv(*mbox, (rt_ubase_t *)msg, 0);
    if (ret == -RT_ETIMEOUT)
    {
        return SYS_ARCH_TIMEOUT;
    }
    else
    {
        if (ret == RT_EOK)
            ret = 1;
    }

    return ret;
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
    return (int)(*mbox);
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    *mbox = RT_NULL;
}

/*-----------------------------------------------------------------------------------*/

err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    static unsigned short counter = 0;
    char tname[RT_NAME_MAX];
    sys_sem_t tmpsem;

    RT_DEBUG_NOT_IN_INTERRUPT;

    rt_snprintf(tname, RT_NAME_MAX, "%s%d", SYS_LWIP_SEM_NAME, counter);
    counter ++;

    tmpsem = rt_sem_create(tname, count, RT_IPC_FLAG_FIFO);
    if (tmpsem == RT_NULL)
        return ERR_MEM;
    else
    {
        *sem = tmpsem;

        return ERR_OK;
    }
}

/*-----------------------------------------------------------------------------------*/

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    rt_err_t ret;
    s32_t t;
    u32_t tick;

    RT_DEBUG_NOT_IN_INTERRUPT;

    /* get the begin tick */
    tick = rt_tick_get();
    if (timeout == 0)
        t = RT_WAITING_FOREVER;
    else
    {
        /* convert msecond to os tick */
        if (timeout < (1000/RT_TICK_PER_SECOND))
            t = 1;
        else
            t = timeout / (1000/RT_TICK_PER_SECOND);
    }

    ret = rt_sem_take(*sem, t);

    if (ret == -RT_ETIMEOUT)
        return SYS_ARCH_TIMEOUT;
    else
    {
        if (ret == RT_EOK)
            ret = 1;
    }

    /* get elapse msecond */
    tick = rt_tick_get() - tick;

    /* convert tick to msecond */
    tick = tick * (1000 / RT_TICK_PER_SECOND);
    if (tick == 0)
        tick = 1;

    return tick;
}

/*-----------------------------------------------------------------------------------*/

void sys_sem_signal(sys_sem_t *sem)
{
    rt_sem_release(*sem);
}

/*-----------------------------------------------------------------------------------*/

void sys_sem_free(sys_sem_t *sem)
{
    RT_DEBUG_NOT_IN_INTERRUPT;
    rt_sem_delete(*sem);
}

int sys_sem_valid(sys_sem_t *sem)
{
    return (int)(*sem);
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
    *sem = RT_NULL;
}

/*-----------------------------------------------------------------------------------*/

err_t sys_mutex_new(sys_mutex_t *mutex)
{
    static unsigned short counter = 0;
    char tname[RT_NAME_MAX];
    sys_mutex_t tmpmutex;

    RT_DEBUG_NOT_IN_INTERRUPT;

    rt_snprintf(tname, RT_NAME_MAX, "%s%d", SYS_LWIP_MUTEX_NAME, counter);
    counter ++;

    tmpmutex = rt_mutex_create(tname, RT_IPC_FLAG_FIFO);
    if (tmpmutex == RT_NULL)
        return ERR_MEM;
    else
    {
        *mutex = tmpmutex;

        return ERR_OK;
    }
}

void sys_mutex_lock(sys_mutex_t *mutex)
{
    RT_DEBUG_NOT_IN_INTERRUPT;
    rt_mutex_take(*mutex, RT_WAITING_FOREVER);

    return;
}

void sys_mutex_unlock(sys_mutex_t *mutex)
{
    rt_mutex_release(*mutex);
}

void sys_mutex_free(sys_mutex_t *mutex)
{
    RT_DEBUG_NOT_IN_INTERRUPT;

    rt_mutex_delete(*mutex);
}

int sys_mutex_valid(sys_mutex_t *mutex)
{
    return (int)(*mutex);
}

void sys_mutex_set_invalid(sys_mutex_t *mutex)
{
    *mutex = RT_NULL;
}

/*-----------------------------------------------------------------------------------*/

sys_prot_t sys_arch_protect(void)
{
    rt_base_t level;

    level = rt_hw_interrupt_disable();

    return level;
}

void sys_arch_unprotect(sys_prot_t pval)
{
    rt_hw_interrupt_enable(pval);
}

/*-----------------------------------------------------------------------------------*/

void sys_init(void)
{
    /* nothing on RT-Thread porting */
}

/*-----------------------------------------------------------------------------------*/

sys_thread_t sys_thread_new(const char *name,
                            lwip_thread_fn function,
                            void *arg,
                            int stacksize,
                            int prio)
{
    rt_thread_t t;

    RT_DEBUG_NOT_IN_INTERRUPT;

    /* create thread */
    t = rt_thread_create(name, function, arg, stacksize, prio, 20);
    RT_ASSERT(t != RT_NULL);

    /* startup thread */
    rt_thread_startup(t);

    return t;
}

/*-----------------------------------------------------------------------------------*/

u32_t sys_now(void)
{
	return rt_tick_get() * (1000 / RT_TICK_PER_SECOND);
}

u32_t sys_jiffies(void)
{
    return rt_tick_get();
}

#elif defined(OS_FREERTOS)

//************************************************************************************************
// FreeRTOS
//************************************************************************************************

/*-----------------------------------------------------------------------------------*/

int sys_mbox_valid(sys_mbox_t *mbox)
{
	return (*mbox != SYS_MBOX_NULL);
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	*mbox = SYS_MBOX_NULL;
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
	*sem = SYS_SEM_NULL;
}

int sys_sem_valid(sys_sem_t *sem)
{
	return (*sem != SYS_SEM_NULL);
}

/*-----------------------------------------------------------------------------------*/
/*
 * Creates an empty mailbox.
 */
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	*mbox = xQueueCreate(size, sizeof(void *));

#if SYS_STATS
      ++lwip_stats.sys.mbox.used;
      if (lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used)
      {
         lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
	  }
#endif /* SYS_STATS */

    if (0 == *mbox)
    	return ERR_MEM;

	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
/*
 * Deallocates a mailbox. If there are messages still present in the
 * mailbox when the mailbox is deallocated, it is an indication of a
 * programming error in lwIP and the developer should be notified.
 */
void sys_mbox_free(sys_mbox_t *mbox)
{
	if (uxQueueMessagesWaiting(*mbox))
	{
		/* Line for breakpoint. Should never break here! */
		portNOP();
#if SYS_STATS
	    lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */

		// TODO notify the user of failure.
	}

	vQueueDelete(*mbox);

#if SYS_STATS
    --lwip_stats.sys.mbox.used;
#endif /* SYS_STATS */
}

/*-----------------------------------------------------------------------------------*/
/*
 * Posts the "msg" to the mailbox.
 */
void sys_mbox_post(sys_mbox_t *mbox, void *data)
{
	xQueueSendToBack(*mbox, &data, portMAX_DELAY);
}

/*-----------------------------------------------------------------------------------*/
/*
 * Try to post the "msg" to the mailbox.
 */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	err_t result;

	if (xQueueSend(*mbox, &msg, 0) == pdPASS)
	{
        result = ERR_OK;
	}
	else
    {
        // could not post, queue must be full
        result = ERR_MEM;
#if SYS_STATS
        lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */
	}

	return result;
}

/*-----------------------------------------------------------------------------------*/
/*
 * Blocks the thread until a message arrives in the mailbox, but does
 * not block the thread longer than "timeout" milliseconds (similar to
 * the sys_arch_sem_wait() function). The "msg" argument is a result
 * parameter that is set by the function (i.e., by doing "*msg =
 * ptr"). The "msg" parameter maybe NULL to indicate that the message
 * should be dropped.
 *
 * The return values are the same as for the sys_arch_sem_wait() function:
 * Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 * timeout.
 *
 * Note that a function with a similar name, sys_mbox_fetch(), is
 * implemented by lwIP.
 */
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	void *dummyptr;
	portTickType StartTime, EndTime, Elapsed;
	StartTime = xTaskGetTickCount();

	if (msg == NULL)
		msg = &dummyptr;

	if (timeout != 0)
	{
		if (pdTRUE == xQueueReceive(*mbox, &(*msg), timeout / portTICK_RATE_MS))
		{
			EndTime = xTaskGetTickCount();
			Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
			if (Elapsed == 0)
				Elapsed = 1;
			return (Elapsed);
		}
		else    // timed out blocking for message
		{
			*msg = NULL;
			return SYS_ARCH_TIMEOUT;
		}
	}
	else        // block forever for a message.
	{
		while (pdTRUE != xQueueReceive(*mbox, &(*msg), portMAX_DELAY))  // time is arbitrary
            ;
		EndTime = xTaskGetTickCount();
		Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
		if (Elapsed == 0)
			Elapsed = 1;
		return (Elapsed);   // return time blocked TODO test
	}
}

/*-----------------------------------------------------------------------------------*/
/*
 * Similar to sys_arch_mbox_fetch, but if message is not ready immediately, we'll
 * return with SYS_MBOX_EMPTY.  On success, 0 is returned.
 */
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	void *dummyptr;
	if (msg == NULL)
		msg = &dummyptr;
    if (pdTRUE == xQueueReceive(*mbox, &(*msg), 0))
        return ERR_OK;
    else
        return SYS_MBOX_EMPTY;
}

/*-----------------------------------------------------------------------------------*/

err_t sys_mutex_new(sys_mutex_t* mutex)
{
	*mutex = xSemaphoreCreateMutex();

	if (0 != *mutex)
		return ERR_OK;
	else
		return ERR_MEM;
}

void sys_mutex_lock(sys_mutex_t* mutex)
{
	xSemaphoreTake(*mutex, portMAX_DELAY);
}

void sys_mutex_unlock(sys_mutex_t* mutex)
{
	xSemaphoreGive(*mutex);
}

/*-----------------------------------------------------------------------------------*/
/*
 * Creates and returns a new semaphore. The "count" argument specifies
 *  the initial state of the semaphore.
 */
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	vSemaphoreCreateBinary((*sem));

	if (NULL == *sem)
	{
#if SYS_STATS
        ++lwip_stats.sys.sem.err;
#endif /* SYS_STATS */
		return ERR_MEM;
	}
	if (count == 0)
	{
		xSemaphoreTake(*sem, 1);
	}

#if SYS_STATS
	++lwip_stats.sys.sem.used;
 	if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used)
    {
		lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
	}
#endif /* SYS_STATS */

    return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
/*
 * Blocks the thread while waiting for the semaphore to be
 * signaled. If the "timeout" argument is non-zero, the thread should
 * only be blocked for the specified time (measured in
 * milliseconds).
 *
 * If the timeout argument is non-zero, the return value is the number of
 * milliseconds spent waiting for the semaphore to be signaled. If the
 * semaphore wasn't signaled within the specified time, the return value is
 * SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 * (i.e., it was already signaled), the function may return zero.
 *
 * Notice that lwIP implements a function with a similar name,
 * sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 */
u32_t sys_arch_sem_wait(sys_sem_t* sem, u32_t timeout)
{
	portTickType StartTime, EndTime, Elapsed;
	StartTime = xTaskGetTickCount();

	if (timeout != 0)
	{
		if (xSemaphoreTake(*sem, timeout / portTICK_RATE_MS) == pdTRUE)
		{
			EndTime = xTaskGetTickCount();
			Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
			if (Elapsed==0)
				Elapsed = 1;
			return (Elapsed);   // return time blocked TODO test
		}
		else
		{
			return SYS_ARCH_TIMEOUT;
		}
	}
	else    // must block without a timeout
	{
		while (xSemaphoreTake(*sem, portMAX_DELAY) != pdTRUE)
            ;
		EndTime = xTaskGetTickCount();
		Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
		if (Elapsed == 0)
			Elapsed = 1;
		return (Elapsed);   // return time blocked
	}
}

/*-----------------------------------------------------------------------------------*/
/*
 * Signals a semaphore
 */
void sys_sem_signal(sys_sem_t* sem)
{
	xSemaphoreGive(*sem);
}

/*-----------------------------------------------------------------------------------*/
/*
 * Deallocates a semaphore
 */
void sys_sem_free(sys_sem_t* sem)
{
#if SYS_STATS
    --lwip_stats.sys.sem.used;
#endif /* SYS_STATS */
    vSemaphoreDelete(*sem);
}

/*-----------------------------------------------------------------------------------*/
/*
 * Initialize sys arch
 */
void sys_init(void)
{
    //
}

/*-----------------------------------------------------------------------------------*/

/*
 * Starts a new thread with priority "prio" that will begin its execution in the
 * function "thread()". The "arg" argument will be passed as an argument to the
 * thread() function. The id of the new thread is returned. Both the id and
 * the priority are system dependent.
 */
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
	xTaskHandle CreatedTask = NULL;
	int result;

    /*
     * 创建一个 FreeRTOS 线程
     */
    result = xTaskCreate(thread,
                         (signed portCHAR *)name,
                         stacksize,
                         arg,
                         prio,
                         &CreatedTask);

    if (result != pdPASS)
	    CreatedTask = NULL;

   return CreatedTask;
}

/*-----------------------------------------------------------------------------------*/

/*
 * This optional function does a "fast" critical region protection and returns
 * the previous protection level. This function is only called during very short
 * critical regions. An embedded system which supports ISR-based drivers might
 * want to implement this function by disabling interrupts. Task-based systems
 * might want to implement this by using a mutex or disabling tasking. This
 * function should support recursive calls from the same task or interrupt. In
 * other words, sys_arch_protect() could be called while already protected. In
 * that case the return value indicates that it is already protected.
 *
 * sys_arch_protect() is only required if your port is supporting an operating
 * system.
 */
sys_prot_t sys_arch_protect(void)
{
	portENTER_CRITICAL();
	return 1;
}

/*
 * This optional function does a "fast" set of critical region protection to the
 * value specified by pval. See the documentation for sys_arch_protect() for
 * more information. This function is only required if your port is supporting
 * an operating system.
 */
void sys_arch_unprotect(sys_prot_t pval)
{
	(void) pval;
	portEXIT_CRITICAL();
}

#elif defined(OS_UCOS)

//************************************************************************************************
// uCOSII
//************************************************************************************************

/*-----------------------------------------------------------------------------------*/

OS_STK sys_stack[LWIP_MAX_TASKS][LWIP_STACK_SIZE];

static INT8U sys_thread_no;

/*-----------------------------------------------------------------------------------*/

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    mbox->pQ = OSQCreate(mbox->pvQEntries, LWIP_Q_SIZE);
    LWIP_ASSERT("OSQCreate", mbox->pQ != NULL );
    mbox->Q_full = OSSemCreate(LWIP_Q_SIZE);
    LWIP_ASSERT("OSSemCreate", mbox->Q_full != NULL );

    mbox->is_valid = 1;
    return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/

void sys_mbox_free(sys_mbox_t *mbox)
{
    INT8U     ucErr;

    LWIP_ASSERT("sys_mbox_free", mbox != NULL);
    OSQFlush(mbox->pQ);

    OSQDel(mbox->pQ, OS_DEL_NO_PEND, &ucErr);
    LWIP_ASSERT("OSQDel", ucErr == OS_ERR_NONE);

    OSSemDel(mbox->Q_full, OS_DEL_NO_PEND, &ucErr);
    LWIP_ASSERT("OSSemDel", ucErr == OS_ERR_NONE);
}

/*-----------------------------------------------------------------------------------*/

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    INT8U status;

    /* Wait for an available slot in the queue. */
    OSSemPend(mbox->Q_full, 0, &status);

    /* Posts the message to the queue. */
    status = OSQPost(mbox->pQ, msg);
    LWIP_ASSERT("OSQPost", status == OS_ERR_NONE);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    INT8U status;

    if (OSSemAccept(mbox->Q_full))
    {
        status = OSQPost(mbox->pQ, msg);
        LWIP_ASSERT("OSQPost", status == OS_ERR_NONE);
    }
    else
    {
        return ERR_MEM;
    }

    return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
    INT8U ucErr;
    INT32U ucos_timeout;
    void *temp;

    /* convert LwIP timeout (in milliseconds) to uC/OS-II timeout (in OS_TICKS) */
    if (timeout)
    {
        ucos_timeout = (timeout * OS_TICKS_PER_SEC)/1000;
        if (ucos_timeout < 1)
            ucos_timeout = 1;
        else if (ucos_timeout > 65535)
            ucos_timeout = 65535;
    }
    else
    {
        ucos_timeout = 0;
    }

    temp = OSQPend(mbox->pQ, ucos_timeout, &ucErr);

    if (ucErr == OS_ERR_TIMEOUT)
    {
        timeout = SYS_ARCH_TIMEOUT;
    }
    else
    {
        LWIP_ASSERT("OSQPend ", ucErr == OS_ERR_NONE);

        /* Tells tasks waiting because of a full buffer that the buffer is not full
         * anymore. */
        OSSemPost(mbox->Q_full);

        /* If there is a destination pointer, store the message in it. */
        if (msg)
        {
            *msg = temp;
        }

        /* Calculate time we waited for the message to arrive. */
        /* XXX: we cheat and just pretend that we waited for long! */
        timeout = 1;
    }

    return timeout;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    INT8U ucErr;
    void *temp;

    temp = OSQAccept(mbox->pQ, &ucErr);

    if (temp == NULL && ucErr == OS_ERR_Q_EMPTY)
        return SYS_MBOX_EMPTY;

    /* Tells tasks waiting because of a full buffer that the buffer is not full
     * anymore. */
    OSSemPost(mbox->Q_full);

    *msg = temp;

    return 0;
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
    LWIP_ASSERT("sys_mbox_valid", mbox != NULL);
    return mbox->is_valid;
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    LWIP_ASSERT("sys_mbox_valid", mbox != NULL);
    mbox->is_valid = 0;
}

/*-----------------------------------------------------------------------------------*/

err_t sys_sem_new(sys_sem_t *pSem, u8_t count)
{
    pSem->sem = OSSemCreate((INT16U)count);
    LWIP_ASSERT("OSSemCreate ", pSem->sem != NULL);
    pSem->is_valid = 1;
    return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    INT8U  ucErr;
    INT32U ucos_timeout;

    /* Convert lwIP timeout (in milliseconds) to uC/OS-II timeout (in OS_TICKS) */
    if (timeout)
    {
        ucos_timeout = (timeout * OS_TICKS_PER_SEC)/1000;
        if (ucos_timeout < 1)
            ucos_timeout = 1;
        else if (ucos_timeout > 65535)
            ucos_timeout = 65535;
    }
    else
    {
        ucos_timeout = 0;
    }

    OSSemPend(sem->sem, ucos_timeout, &ucErr);
    if (ucErr == OS_ERR_TIMEOUT)
    {
        timeout = SYS_ARCH_TIMEOUT;
    }
    else
    {
        /* Calculate time we waited for the message to arrive. */
        /* TODO: we cheat and just pretend that we waited for long! */
        timeout = 1;
    }

    return timeout;
}

/*-----------------------------------------------------------------------------------*/

void sys_sem_signal(sys_sem_t *sem)
{
    OSSemPost(sem->sem);
}

/*-----------------------------------------------------------------------------------*/

void sys_sem_free(sys_sem_t *sem)
{
    INT8U     ucErr;

    OSSemDel(sem->sem, OS_DEL_NO_PEND, &ucErr);
    LWIP_ASSERT("OSSemDel ", ucErr == OS_ERR_NONE);
}

int sys_sem_valid(sys_sem_t *sem)
{
    return sem->is_valid;
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
    sem->is_valid = 0;
}

/*-----------------------------------------------------------------------------------*/

void sys_init(void)
{
    sys_thread_no = 0;
}

/*-----------------------------------------------------------------------------------*/

sys_thread_t sys_thread_new(const char *name,
                            lwip_thread_fn function,
                            void *arg,
                            int stacksize,
                            int prio)
{
    INT8U bTemp;

    LWIP_ASSERT("sys_thread_new: Max Sys. Tasks reached.", sys_thread_no < LWIP_MAX_TASKS);

    ++sys_thread_no; /* next task created will be one lower to this one */

    if ((bTemp = OSTaskCreate(function,
                              arg,
                              &sys_stack[sys_thread_no - 1][LWIP_STACK_SIZE - 1],
                              prio)))
    {
        printk("sys_thread_new: Task creation error (prio=%d) [%d]\n",prio,bTemp);
        --sys_thread_no;
//      panic("lwip thread creation failed.");
    }

    return sys_thread_no;
}

u32_t sys_now(void)
{
    return (1000/OS_TICKS_PER_SEC)*OSTimeGet();
}

u32_t sys_jiffies(void)
{
    return OSTimeGet();
}

#else // OS_NONE

//************************************************************************************************
// Bare Programming
//************************************************************************************************

extern unsigned int get_clock_ticks(void);

unsigned int sys_now(void)
{
	return get_clock_ticks();
}

#endif // OS

//-----------------------------------------------------------------------------

/*
 * @@ END
 */


