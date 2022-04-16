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
 **��������UART5_set_IO
 **�������ܣ���ʼ��uart��IO��
 **�βΣ���
 **����ֵ����
 **˵����   UART5_RX:60 -- ���ݽ���
            UART5_TX:61 -- ���ݷ���
 *******************************************************************/
void UART5_Config_Init(void)
{
    unsigned int BaudRate = 9600;
    ls1x_uart_init(devUART5,(void *)BaudRate); //��ʼ������
    ls1x_uart_open(devUART5,NULL); //�򿪴���
}

int count;
char buff[256];

//����
void UART5_Test(void)
{
    //��������
    count = ls1x_uart_read(devUART5,buff,256,NULL);
    if(count > 0)
    {
        //��������
        ls1x_uart_write(devUART5,buff,8,NULL);
    }
    
    delay_ms(500);

    if(strncmp(buff,"led_on",6) == 0) //�Ƚ�ǰn���ֽ�
    {
        LED_All_ON();// ����LED
    }
    if(strncmp(buff,"led_off",6) == 0)
    {
        LED_All_OFF();//�ر�LED
    }
        if(strncmp(buff,"led_demo",6)==0)
    {
        LED_Waterfall();//��ˮ��
        LED_Waterfall();
    }

}

