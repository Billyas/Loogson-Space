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


/*定义邮箱控制块*/
static rt_mailbox_t test_mail = RT_NULL;

/*定义全局邮箱消息*/
char test_mb1[] = "receive mail test1";     //邮箱消息test1
char test_mb2[] = "receive mail test2";     //邮箱消息test2
char test_mb3[] = "receive mail test3";     //邮箱消息test3

/*定义线程控制块*/
static rt_thread_t send_thread = RT_NULL;
static rt_thread_t receive_thread = RT_NULL;

/*函数声明*/
static void send_thread_entry(void *arg);
static void receive_thread_entry(void *arg);

//-------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	rt_kprintf("\r\nWelcome to RT-Thread.\r\n\r\n");
	
	fb_open();
	Key_Init();    //初始化按键
	ADC_DAC_Init();

    /*创建邮箱*/
    test_mail = rt_mb_create("test_mail",           /*邮箱名称*/
                              10,                   /*邮箱大小*/
                              RT_IPC_FLAG_FIFO);    /*先进先出*/
    if(test_mail != RT_NULL)
        rt_kprintf("test_mail create ok\n");
        
    /*创建线程*/
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
        if(!gpio_read(KEY1)){   //KEY1被按下，邮件1发送
            rt_kprintf("KEY1 pressed\n");
            ret = rt_mb_send(test_mail, (rt_uint32_t)&test_mb1);
            if(RT_EOK == ret)
                rt_kprintf("test_mb1 send successfully\n");
            else
                rt_kprintf("test_mb1 send failed\n");
        }

        if(!gpio_read(KEY2)){   //KEY2被按下，邮件2发送
            rt_kprintf("KEY2 pressed\n");
            ret = rt_mb_send(test_mail, (rt_uint32_t)&test_mb2);
            if(RT_EOK == ret)
                rt_kprintf("test_mb2 send successfully\n");
            else
                rt_kprintf("test_mb2 send failed\n");
        }
        
        if(!gpio_read(KEY3)){   //KEY3被按下，邮件3发送
            rt_kprintf("KEY3 pressed\n");
            ret = rt_mb_send(test_mail, (rt_uint32_t)&test_mb3);
            if(RT_EOK == ret)
                rt_kprintf("test_mb3 send successfully\n");
            else
                rt_kprintf("test_mb3 send failed\n");
        }

        delay_ms(500);  //500ms扫描一次
    }
}


static void receive_thread_entry(void *arg)
{
    rt_err_t ret;
    char *rev_str;
    
    while(1){
        ret = rt_mb_recv(test_mail,                 //邮箱对象
                         (rt_uint32_t *)&rev_str,   //接收邮箱消息
                         RT_WAITING_FOREVER);       //指定超时时间，一直等，直到有邮件
        if(RT_EOK == ret){
            rt_kprintf("received mail: %s\n",rev_str);
            if(0 == strcmp(rev_str,test_mb1)){          //收到邮件1
                display_pic();
                delay_ms(3000);
                fb_cons_clear();
            }else if(0 == strcmp(rev_str,test_mb2)){    //收到邮件2
                display_pic_flag();
                delay_ms(3000);
                fb_cons_clear();
            }else if(0 == strcmp(rev_str,test_mb3)){    //收到邮件3
                ADC_DAC_Test();
                delay_ms(500);
                fb_cons_clear();
            }
        }else
            rt_kprintf("mailbox received failed! ERROR_CODE:0x%x\n",ret);
    }
}




