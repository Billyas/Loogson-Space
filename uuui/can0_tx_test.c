/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * demo 需要打开 framebuffer, 使用simple gui进行显示
 */
 
#include "bsp.h"

#ifdef BSP_USE_CAN0

#include <stdio.h>
#include <stdlib.h>

#include "ls1x_can.h"
#include "app_os_priority.h"

#if defined(OS_RTTHREAD)
#include "rtthread.h"
#define SLEEP           rt_thread_sleep
static  rt_thread_t     can0_thread;
#elif defined(OS_UCOS)
#include "os.h"
#define SLEEP           OSTimeDly
static  OS_STK          can0_stack[CAN0_STK_SIZE];
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

int lx1x_can0_init_transmit(int gui)
{
    ls1x_can_init(devCAN0, NULL);
    ls1x_can_open(devCAN0, NULL);
    ls1x_can_ioctl(devCAN0, IOCTL_CAN_SET_CORE, (void *)CAN_CORE_20B); 	  /* set mode: CAN_CORE_20B */
	ls1x_can_ioctl(devCAN0, IOCTL_CAN_SET_SPEED, (void *)CAN_SPEED_250K); /* set speed: CAN_SPEED_500K */
    ls1x_can_ioctl(devCAN0, IOCTL_CAN_START, NULL);          	          /* start It */
    
#ifdef BSP_USE_FB
    if (gui)
    {
        gui_drawtext_in_grid(0, 0, "CAN0");
        gui_drawtext_in_grid(0, 1, "TX");
    }
#endif

    DBG_OUT("can0 initialized.\r\n");
    
    return 0;
}

//-----------------------------------------------------------------------------

#if defined(OS_RTTHREAD)
#define WRITE_CAN0(can, m)  rt_device_write(can, 0, (const void *)&m, sizeof(m))
#else
#define WRITE_CAN0(can, m)  ls1x_can_write(can, (void *)&m, sizeof(m), NULL)
#endif

static int tx_count = 0;

int ls1x_can0_do_transmit(void *can)
{
    int wr_cnt;
    CANMsg_t msg;

	/*
	 * 每隔 0.1秒发送一次数据.
	 */
	msg.id = 2; // MSG_ID;
	msg.extended = 1;
	msg.rtr = 0;

    tx_count++;
	msg.data[0] = (unsigned char)(tx_count >> 24);
	msg.data[1] = (unsigned char)(tx_count >> 16);
	msg.data[2] = (unsigned char)(tx_count >> 8);
	msg.data[3] = (unsigned char)(tx_count >> 0);

    tx_count++;
	msg.data[4] = (unsigned char)(tx_count >> 24);
	msg.data[5] = (unsigned char)(tx_count >> 16);
	msg.data[6] = (unsigned char)(tx_count >> 8);
	msg.data[7] = (unsigned char)(tx_count >> 0);

	msg.len = 8;

	/*
	 * Send Message
	 */
    wr_cnt = WRITE_CAN0(can, msg);

    if (wr_cnt > 0)
    {
        SPRINTF(info_buf, "%02x %02x %02x %02x %02x %02x %02x %02x",
		        msg.data[0], msg.data[1], msg.data[2], msg.data[3],
                msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
                
#if defined(XPT2046_DRV) || defined(GT1151_DRV)
        gui_drawtext_in_grid(0, DATA_COL, info_buf);
#else
        printk("CAN0 TX: %s\r\n", info_buf);
#endif
    }

    return 0;
}

//-----------------------------------------------------------------------------

#if (BSP_USE_OS)

#ifndef CAN0_STK_SIZE
//#define CAN0_STK_SIZE   2048
#endif

/*
 * CAN0 任务
 */
static void can0_test_task(void *arg)
{
    /*
     * Add CAN0 initialize code here.
     */
#if defined(OS_RTTHREAD)

    rt_device_t pCAN0;

    pCAN0 = rt_device_find(ls1x_can_get_device_name(devCAN0));
    if (pCAN0 == NULL)
        return;

    rt_device_open(pCAN0, RT_DEVICE_FLAG_WRONLY);
	rt_device_control(pCAN0, IOCTL_CAN_SET_CORE, (void *)CAN_CORE_20B);    /* set mode: CAN_CORE_20B */
	rt_device_control(pCAN0, IOCTL_CAN_SET_SPEED, (void *)CAN_SPEED_250K); /* set speed: CAN_SPEED_500K */
    rt_device_control(pCAN0, IOCTL_CAN_START, NULL);          	           /* start It */
    
#else
    lx1x_can0_init_transmit(0);
#endif

    SLEEP(100);
#ifdef BSP_USE_FB
    gui_drawtext_in_grid(0, 0, "CAN0");
    gui_drawtext_in_grid(0, 1, "TX");
#endif
    DBG_OUT("can0_test_task started.\r\n");
    
    for ( ; ; )
    {
        /*
         * Add CAN0 task code here.
         */

    #if defined(OS_RTTHREAD)
		ls1x_can0_do_transmit(pCAN0);
    #else
		ls1x_can0_do_transmit(devCAN0);
    #endif
    
        /* abandon cpu time to run other task */
        SLEEP(100);                     // task sleep 100 ms
    }

#if defined(OS_RTTHREAD)
    rt_device_close(pCAN0);
#else
    ls1x_can_close(devCAN0, NULL);
#endif
}

/*
 * 创建 CAN0 任务
 */
int can0_test_start(void)
{
    ls1x_can_init(devCAN0, NULL);
    ls1x_can_open(devCAN0, NULL);

#if defined(OS_RTTHREAD)
    can0_thread = rt_thread_create("can0thread",
                                   can0_test_task,
                                   NULL,                // arg
                                   CAN0_STK_SIZE*4,     // statck size
                                   CAN0_TASK_PRIO,      // priority
                                   CAN0_TASK_SLICE);    // slice ticks

    if (can0_thread == NULL)
    {
        printk("create can0 test thread fail!\r\n");
		return -1;
	}

	rt_thread_startup(can0_thread);
	
#elif defined(OS_UCOS)

    OSTaskCreate(can0_test_task,
                 NULL,
        #if OS_STK_GROWTH == 1
                 (void *)&can0_stack[CAN0_STK_SIZE-1],
        #else
                 (void *)&can0_stack[0],
        #endif
                 CAN0_TASK_PRIO);

#elif defined(OS_FREERTOS)

    xTaskCreate(can0_test_task,
                "can0task",
                CAN0_STK_SIZE, NULL,
                CAN0_TASK_PRIO, NULL);

#else

#endif

    return 0;
}

#endif // #if (BSP_USE_OS)

#endif

/*
 * @END
 */
 

