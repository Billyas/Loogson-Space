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
#include "ls1x_fb.h"
#include "src/mailbox.h"

char LCD_display_mode[] = LCD_480x800;


/*����������ƿ�*/
static rt_mailbox_t test_mail = RT_NULL;

/*����ȫ��������Ϣ*/
char test_mb1[] = "receive mail test1";     //������Ϣtest1
char test_mb2[] = "receive mail test2";     //������Ϣtest2
char test_mb3[] = "receive mail test3";     //������Ϣtest3

/*�����߳̿��ƿ�*/
static rt_thread_t send_thread = RT_NULL;
static rt_thread_t receive_thread = RT_NULL;

/*��������*/
static void send_thread_entry(void *arg);
static void receive_thread_entry(void *arg);

//-------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	rt_kprintf("\r\nWelcome to RT-Thread.\r\n\r\n");
	
	fb_open();
	Key_Init();    //��ʼ������
	ADC_DAC_Init();

    /*��������*/
    test_mail = rt_mb_create("test_mail",           /*��������*/
                              10,                   /*�����С*/
                              RT_IPC_FLAG_FIFO);    /*�Ƚ��ȳ�*/
    if(test_mail != RT_NULL)
        rt_kprintf("test_mail create ok\n");
        
    /*�����߳�*/
    send_thread = rt_thread_create("send_thread",
                                    send_thread_entry,
                                    RT_NULL,
                                    1024,
                                    5,
                                    20);
    if(send_thread != RT_NULL)
        rt_thread_startup(send_thread);
    else
        rt_kprintf("send_thread create failed.\n");
        
    receive_thread = rt_thread_create("receive_thread",
                                    receive_thread_entry,
                                    RT_NULL,
                                    1024,
                                    5,
                                    20);
    if(receive_thread != RT_NULL)
        rt_thread_startup(receive_thread);
    else
        rt_kprintf("receive_thread create failed.\n");
    
    return 0;
}


static void send_thread_entry(void *arg)
{
    rt_err_t ret;
    
    while(1){
        if(!gpio_read(KEY1)){   //KEY1�����£��ʼ�1����
            rt_kprintf("KEY1 pressed\n");
            ret = rt_mb_send(test_mail, (rt_uint32_t)&test_mb1);
            if(RT_EOK == ret)
                rt_kprintf("test_mb1 send successfully\n");
            else
                rt_kprintf("test_mb1 send failed\n");
        }

        if(!gpio_read(KEY2)){   //KEY2�����£��ʼ�2����
            rt_kprintf("KEY2 pressed\n");
            ret = rt_mb_send(test_mail, (rt_uint32_t)&test_mb2);
            if(RT_EOK == ret)
                rt_kprintf("test_mb2 send successfully\n");
            else
                rt_kprintf("test_mb2 send failed\n");
        }
        
        if(!gpio_read(KEY3)){   //KEY3�����£��ʼ�3����
            rt_kprintf("KEY3 pressed\n");
            ret = rt_mb_send(test_mail, (rt_uint32_t)&test_mb3);
            if(RT_EOK == ret)
                rt_kprintf("test_mb3 send successfully\n");
            else
                rt_kprintf("test_mb3 send failed\n");
        }

        delay_ms(500);  //500msɨ��һ��
    }
}


static void receive_thread_entry(void *arg)
{
    rt_err_t ret;
    char *rev_str;
    
    while(1){
        ret = rt_mb_recv(test_mail,                 //�������
                         (rt_uint32_t *)&rev_str,   //����������Ϣ
                         RT_WAITING_FOREVER);       //ָ����ʱʱ�䣬һֱ�ȣ�ֱ�����ʼ�
        if(RT_EOK == ret){
            rt_kprintf("received mail: %s\n",rev_str);
            if(0 == strcmp(rev_str,test_mb1)){          //�յ��ʼ�1
                display_pic();
                delay_ms(3000);
                fb_cons_clear();
            }else if(0 == strcmp(rev_str,test_mb2)){    //�յ��ʼ�2
                display_pic_flag();
                delay_ms(3000);
                fb_cons_clear();
            }else if(0 == strcmp(rev_str,test_mb3)){    //�յ��ʼ�3
                ADC_DAC_Test();
                delay_ms(500);
                fb_cons_clear();
            }
        }else
            rt_kprintf("mailbox received failed! ERROR_CODE:0x%x\n",ret);
    }
}




