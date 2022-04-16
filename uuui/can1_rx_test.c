/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * demo 需要打开 framebuffer, 使用simple gui进行显示
 */

#include "bsp.h"

#ifdef BSP_USE_CAN1

#include <stdio.h>
#include <stdlib.h>

#include "ls1x_can.h"
#include "app_os_priority.h"

#if defined(OS_RTTHREAD)
#include "rtthread.h"
#define SLEEP           rt_thread_sleep
static  rt_thread_t     can1_thread;
#elif defined(OS_UCOS)
#include "os.h"
#define SLEEP           OSTimeDly
static  OS_STK          can1_stack[CAN1_STK_SIZE];
#elif defined(OS_FREERTOS)
#include "FreeRTOS.h"
#define SLEEP           vTaskDelay
#else
#define SLEEP(x)        delay_ms(x)
#endif

//-----------------------------------------------------------------------------

#if defined(OS_RTTHREAD)
#define SPRINTF         rt_sprintf
#else
#define SPRINTF         sprintf
#endif

#define S_LEN   64
static unsigned char info_buf[S_LEN+1];

//-----------------------------------------------------------------------------

#ifdef XPT2046_DRV
#define DATA_COL    3
#elif defined(GT1151_DRV)
#define DATA_COL    2
#else
#define DATA_COL    3
#endif

//-----------------------------------------------------------------------------

int lx1x_can1_init_receive(int gui)
{
    ls1x_can_init(devCAN1, NULL);
    ls1x_can_open(devCAN1, NULL);
    ls1x_can_ioctl(devCAN1, IOCTL_CAN_SET_CORE, (void *)CAN_CORE_20B); 	  /* set mode: CAN_CORE_20B */
	ls1x_can_ioctl(devCAN1, IOCTL_CAN_SET_SPEED, (void *)CAN_SPEED_250K); /* set speed: CAN_SPEED_500K */
    ls1x_can_ioctl(devCAN1, IOCTL_CAN_START, NULL);          	          /* start It */
    
#ifdef BSP_USE_FB
    if (gui)
    {
        gui_drawtext_in_grid(1, 0, "CAN1");
        gui_drawtext_in_grid(1, 1, "RX");
    }
#endif

    DBG_OUT("can1 initialized.\r\n");

    return 0;
}

//-----------------------------------------------------------------------------

#if defined(OS_RTTHREAD)
#define READ_CAN1(can, m)  rt_device_read(can, 0, (const void *)&m, sizeof(m))
#else
#define READ_CAN1(can, m)  ls1x_can_read(can, (void *)&m, sizeof(m), NULL)
#endif

int ls1x_can1_do_receive(void *can)
{
    int rd_cnt;
	CANMsg_t msg;

	/*
	 * 接收数据.
	 */
    rd_cnt = READ_CAN1(can, msg);

	if (rd_cnt > 0)
	{
        SPRINTF(info_buf, " %02x %02x %02x %02x %02x %02x %02x %02x",
		        msg.data[0], msg.data[1], msg.data[2], msg.data[3],
                msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
                
#if defined(XPT2046_DRV) || defined(GT1151_DRV)
        gui_drawtext_in_grid(1, DATA_COL, info_buf);
#else
        printk("CAN1 RX: %s\r\n", info_buf);
#endif
	}
}

//-----------------------------------------------------------------------------

#if (BSP_USE_OS)

#ifndef CAN1_STK_SIZE
//#define CAN1_STK_SIZE   2048
#endif

/*
 * CAN1 任务
 */
static void can1_test_task(void *arg)
{
    /*
     * Add CAN1 initialize code here.
     */
#if defined(OS_RTTHREAD)

    rt_device_t pCAN1;

    pCAN1 = rt_device_find(ls1x_can_get_device_name(devCAN1));
    if (pCAN1 == NULL)
        return;

    rt_device_open(pCAN1, RT_DEVICE_FLAG_WRONLY);
	rt_device_control(pCAN1, IOCTL_CAN_SET_CORE, (void *)CAN_CORE_20B);    /* set mode: CAN_CORE_20B */
	rt_device_control(pCAN1, IOCTL_CAN_SET_SPEED, (void *)CAN_SPEED_250K); /* set speed: CAN_SPEED_500K */
    rt_device_control(pCAN1, IOCTL_CAN_START, NULL);          	           /* start It */

#else
    lx1x_can1_init_receive(0);
#endif

    SLEEP(100);
#ifdef BSP_USE_FB
    gui_drawtext_in_grid(1, 0, "CAN1");
    gui_drawtext_in_grid(1, 1, "RX");
#endif
    DBG_OUT("can1_test_task started.\r\n");
    
    for ( ; ; )
    {
        /*
         * Add CAN1 task code here.
         */
    #if defined(OS_RTTHREAD)
		ls1x_can1_do_receive(pCAN1);
    #else
		ls1x_can1_do_receive(devCAN1);
    #endif

        /* abandon cpu time to run other task */
        SLEEP(100);                      // task sleep 50 ms
    }
    
#if defined(OS_RTTHREAD)
    rt_device_close(pCAN1);
#else
    ls1x_can_close(devCAN1, NULL);
#endif
}

/*
 * 创建 CAN1 任务
 */
int can1_test_start(void)
{
    ls1x_can_init(devCAN1, NULL);
    ls1x_can_open(devCAN1, NULL);
    
#if defined(OS_RTTHREAD)
    can1_thread = rt_thread_create("can1thread",
                                   can1_test_task,
                                   NULL,                // arg
                                   CAN1_STK_SIZE*4,     // statck size
                                   CAN1_TASK_PRIO,      // priority
                                   CAN1_TASK_SLICE);    // slice ticks

    if (can1_thread == NULL)
    {
        printk("create can1 test thread fail!\r\n");
		return -1;
	}

	rt_thread_startup(can1_thread);
	
#elif defined(OS_UCOS)

    OSTaskCreate(can1_test_task,
                 NULL,
        #if OS_STK_GROWTH == 1
                 (void *)&can1_stack[CAN1_STK_SIZE-1],
        #else
                 (void *)&can1_stack[0],
        #endif
                 CAN1_TASK_PRIO);

#elif defined(OS_FREERTOS)

    xTaskCreate(can1_test_task,
                "can1task",
                CAN1_STK_SIZE,
				NULL,
                CAN1_TASK_PRIO,
				NULL);
                
#else

#endif

    return 0;
}

#endif // #if (BSP_USE_OS)

#endif

/*
 * @END
 */


