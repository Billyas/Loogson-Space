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

//-------------------------------------------------------------------------------------------------
// Simple demo of task
//-------------------------------------------------------------------------------------------------
/* 定义线程控制块 */
static rt_thread_t thread1 = RT_NULL;

/* 线程 1 的入口函数*/
static void thread1_entry(void *parameter)
{
    rt_uint32_t count = 0 ;
    
    while(1)
    {
        /* 线程 1 采用低优先级运行，一直打印数值 */
        rt_kprintf("thread1 count: %d\r\n", count++);
        rt_thread_mdelay(50);
        if(count >= 10)
        {
            rt_thread_delete(thread1);
            /* 线 程 1 运 行 结 束 后 将 手 动 删 除 */
            rt_kprintf("thread1 exit\r\n");
        }
    }
}

/* 定义线程控制块 */
static struct rt_thread thread2;
/* 定义线程栈 */
static unsigned char thread2_stack[1024];
/* 定义线程控栈时要求 RT_ALIGN_SIZE 个字节对齐 */
ALIGN(RT_ALIGN_SIZE)

/* 线 程 入 口 */
static void thread2_entry(void *param)
{
    rt_uint32_t count = 0;
    /* 线 程 2 拥 有 较 高的优先级， 以 抢占线 程 1 而 获 得 执 行 */
    for (count = 0; count < 10 ; count++)
    {
        /* 线 程 2 打 印 计 数 值 */
        rt_kprintf("thread2 count: %d\r\n", count);
    }
    /* 线 程 2 运 行 结 束 后 也 将 自 动 被 系 统 脱 离 */
    rt_kprintf("thread2 exit\r\n");
}

int main(int argc, char** argv)
{
    rt_kprintf("\r\nWelcome to RT-Thread.\r\n\r\n");
    
    /* 创 建 线 程 1， 名 称 是 thread1， 入 口 是 thread1_entry*/
    thread1 = rt_thread_create("thread1",		//线程名称
                                thread1_entry,	//线程入口函数
                                RT_NULL,		//线程入口函数参数
                                1024,			//线程栈大小
                                11,				//线程的优先级
                                10);			//线程时间片
    /* 如 果 获 得 线 程 控 制 块， 启 动 这 个 线 程 */
    if (thread1 != RT_NULL)
        rt_thread_startup(thread1);
    
    /* 初 始 化 线 程 2， 名 称 是 thread2， 入 口 是 thread2_entry */
    rt_thread_init( &thread2,					//线程控制块
                    "thread2",					//线程名称
                    thread2_entry,				//线程入口函数
                    RT_NULL,					//线程入口函数参数
                    &thread2_stack[0],			//线程栈起始地址
                    sizeof(thread2_stack),		//线程栈大小
                    10,							//线程的优先级
                    10);						//线程时间片

    rt_thread_startup(&thread2);
    
    return 0;
}



