/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * RT-Thread Application
 */

#include <time.h>

#include "rtthread.h"

//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"
#include "semaphore_test.h"

//-------------------------------------------------------------------------------------------------

/*定义线程控制块*/
static rt_thread_t led_test_thread = RT_NULL;
static rt_thread_t led_all_thread = RT_NULL;

/*定义信号量控制块*/
static rt_sem_t test_sem = RT_NULL;

static void led_test_thread_entry(void * arg);
static void led_all_thread_entry(void * arg);


int main(int argc, char** argv)
{
	rt_kprintf("\r\nWelcome to RT-Thread.\r\n\r\n");
	
	Test_Gpio_Init();
	Led_All_Off();

    /*创建一个信号量*/
    test_sem = rt_sem_create("test_sem",            /*信号量名称*/
                              1,                    /*信号量初始值，默认有一个信号量(二进制值信号量)*/
                              RT_IPC_FLAG_FIFO);    /*信号量模式 先来先得的顺序获取资源 FIFO(0x00)*/

    if(test_sem != RT_NULL)
        rt_kprintf("test semaphore create ok!\n");
        
    /*创建线程*/
    led_test_thread = rt_thread_create("led_test",
                                        led_test_thread_entry,
                                        RT_NULL,
                                        1024,
                                        5,
                                        20);
    if(led_test_thread != RT_NULL)
        rt_thread_startup(led_test_thread);
    else
        rt_kprintf("led_test_thread create failed.\n");

    led_all_thread = rt_thread_create("led_all",
                                        led_all_thread_entry,
                                        RT_NULL,
                                        1024,
                                        5,
                                        20);
    if(led_all_thread != RT_NULL)
        rt_thread_startup(led_all_thread);
    else
        rt_kprintf("led_all_thread create failed.\n");
                                        
    return 0;
}


static void led_test_thread_entry(void * arg)
{
    rt_err_t ret = -1;
    int i=0;
    
    while(1){
        ret = rt_sem_take(test_sem,               /*获取二进制值信号量*/
                        RT_WAITING_FOREVER);    /*等待时间：一直等，直到信号量可用*/
                        
        if(ret == RT_EOK){   /*成功获取到信号量*/
            rt_kprintf("%s: take sem successful\n",__func__);
            for(i=0;i<5;i++)
                Led_Test();

            rt_sem_release(test_sem);   /*释放二进制值信号量*/
        }
        else{
            rt_kprintf("%s: take sem failed.\n",__func__);
            /*删除信号量*/
            ret = rt_sem_delete(test_sem);
            if(RT_EOK == ret)
                rt_kprintf("test semaphore delete ok!\n");
        }
    }
}


static void led_all_thread_entry(void * arg)
{
    rt_err_t ret = -1;
    int i=0;
    
    while(1){
        ret = rt_sem_take(test_sem,               /*获取二进制值信号量*/
                        RT_WAITING_FOREVER);    /*等待时间：一直等，直到信号量可用*/
                        
        if(ret == RT_EOK){   /*成功获取到信号量*/
            rt_kprintf("%s: take sem successful\n",__func__);
            for(i=0;i<10;i++){
                Led_All_On();
                delay_ms(500);
                Led_All_Off();
                delay_ms(500);
            }
            
            rt_sem_release(test_sem);   /*释放二进制值信号量*/
        }else{
            rt_kprintf("%s: take sem failed.\n",__func__);
            /*删除信号量*/
            ret = rt_sem_delete(test_sem);
            if(RT_EOK == ret)
                rt_kprintf("test semaphore delete ok!\n");
        }
    }
}







