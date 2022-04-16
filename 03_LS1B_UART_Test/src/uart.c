/*
 * uart.c
 *
 * created: 2021/4/30
 *  author: 
 */
#include "uart.h"
#include "ls1b.h"
#include "ls1b_gpio.h"
#include "ns16550.h"
#include "stdio.h"
#include "led.h"
#include "string.h"
#include "uart.h"
/*******************************************************************
 **函数名：UART5_set_IO
 **函数功能：初始化uart的IO口
 **形参：无
 **返回值：无
 **说明：   UART5_RX:60 -- 数据接收
            UART5_TX:61 -- 数据发送
 *******************************************************************/
void UART5_Config_Init(void)
{
    unsigned int BaudRate = 9600;
    ls1x_uart_init(devUART5,(void *)BaudRate); //初始化串口
    ls1x_uart_open(devUART5,NULL); //打开串口
}

int count;
char buff[256];

//测试
void UART5_Test(void)
{
    //接收数据
    count = ls1x_uart_read(devUART5,buff,256,NULL);
    if(count > 0)
    {
        //发送数据
        ls1x_uart_write(devUART5,buff,8,NULL);
    }
    
    delay_ms(500);

    if(strncmp(buff,"led_on",6) == 0) //比较前n个字节
    {
        LED_All_ON();// 开启LED
    }
    if(strncmp(buff,"led_off",6) == 0)
    {
        LED_All_OFF();//关闭LED
    }
        if(strncmp(buff,"led_demo",6)==0)
    {
        LED_Waterfall();//流水灯
        LED_Waterfall();
    }

}

