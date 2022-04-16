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
/* �����߳̿��ƿ� */
static rt_thread_t thread1 = RT_NULL;

/* �߳� 1 ����ں���*/
static void thread1_entry(void *parameter)
{
    rt_uint32_t count = 0 ;
    
    while(1)
    {
        /* �߳� 1 ���õ����ȼ����У�һֱ��ӡ��ֵ */
        rt_kprintf("thread1 count: %d\r\n", count++);
        rt_thread_mdelay(50);
        if(count >= 10)
        {
            rt_thread_delete(thread1);
            /* �� �� 1 �� �� �� �� �� �� �� �� ɾ �� */
            rt_kprintf("thread1 exit\r\n");
        }
    }
}

/* �����߳̿��ƿ� */
static struct rt_thread thread2;
/* �����߳�ջ */
static unsigned char thread2_stack[1024];
/* �����߳̿�ջʱҪ�� RT_ALIGN_SIZE ���ֽڶ��� */
ALIGN(RT_ALIGN_SIZE)

/* �� �� �� �� */
static void thread2_entry(void *param)
{
    rt_uint32_t count = 0;
    /* �� �� 2 ӵ �� �� �ߵ����ȼ��� �� ��ռ�� �� 1 �� �� �� ִ �� */
    for (count = 0; count < 10 ; count++)
    {
        /* �� �� 2 �� ӡ �� �� ֵ */
        rt_kprintf("thread2 count: %d\r\n", count);
    }
    /* �� �� 2 �� �� �� �� �� Ҳ �� �� �� �� ϵ ͳ �� �� */
    rt_kprintf("thread2 exit\r\n");
}

int main(int argc, char** argv)
{
    rt_kprintf("\r\nWelcome to RT-Thread.\r\n\r\n");
    
    /* �� �� �� �� 1�� �� �� �� thread1�� �� �� �� thread1_entry*/
    thread1 = rt_thread_create("thread1",		//�߳�����
                                thread1_entry,	//�߳���ں���
                                RT_NULL,		//�߳���ں�������
                                1024,			//�߳�ջ��С
                                11,				//�̵߳����ȼ�
                                10);			//�߳�ʱ��Ƭ
    /* �� �� �� �� �� �� �� �� �飬 �� �� �� �� �� �� */
    if (thread1 != RT_NULL)
        rt_thread_startup(thread1);
    
    /* �� ʼ �� �� �� 2�� �� �� �� thread2�� �� �� �� thread2_entry */
    rt_thread_init( &thread2,					//�߳̿��ƿ�
                    "thread2",					//�߳�����
                    thread2_entry,				//�߳���ں���
                    RT_NULL,					//�߳���ں�������
                    &thread2_stack[0],			//�߳�ջ��ʼ��ַ
                    sizeof(thread2_stack),		//�߳�ջ��С
                    10,							//�̵߳����ȼ�
                    10);						//�߳�ʱ��Ƭ

    rt_thread_startup(&thread2);
    
    return 0;
}



