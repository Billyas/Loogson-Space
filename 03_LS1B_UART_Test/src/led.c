/*
 * led.c
 *
 * created: 2021/4/29
 *  author: 
 */
 #include "led.h"
 #include "ls1b_gpio.h"
 #include "ls1b.h"

 /*******************************************************************
 **��������LED_IO_Config
 **�������ܣ���ʼ��LED��IO��
 **�βΣ���
 **����ֵ����
 **˵����
 *******************************************************************/
 void LED_IO_Config_Init(void)
 {
    //�⿪��
    gpio_enable(LED1,DIR_OUT);
    gpio_enable(LED2,DIR_OUT);
    gpio_enable(LED3,DIR_OUT);
    gpio_enable(LED4,DIR_OUT);
 }

/*******************************************************************
 **��������LEDx_Status
 **�������ܣ����õ���LED�Ƶ�״̬
 **�βΣ�
    @int ledx -- x:1,2,3,4
    @bool status -- ON��OFF
 **����ֵ����
 **˵����
 *******************************************************************/
 void LEDx_Set_Status(u8 LEDx, u8 status)
 {
    gpio_write(LEDx,status);
 }
 
/*******************************************************************
 **��������LED_All_ON
 **�������ܣ��������еĵ�
 **�βΣ���
 **����ֵ����
 **˵����
 *******************************************************************/
 void LED_All_ON(void)
 {
    gpio_write(LED1,ON);
    gpio_write(LED2,ON);
    gpio_write(LED3,ON);
    gpio_write(LED4,ON);
 }
 
 /*******************************************************************
 **��������LED_All_OFF
 **�������ܣ��ر����еĵ�
 **�βΣ���
 **����ֵ����
 **˵����
 *******************************************************************/
 void LED_All_OFF(void)
 {
    gpio_write(LED1,OFF);
    gpio_write(LED2,OFF);
    gpio_write(LED3,OFF);
    gpio_write(LED4,OFF);
 }
 
 /*******************************************************************
 **��������LED_Waterfall
 **�������ܣ���ˮ��
 **�βΣ���
 **����ֵ����
 **˵����
 *******************************************************************/
 void LED_Waterfall(void)
 {
    gpio_write(LED1,ON);
    gpio_write(LED2,OFF);
    gpio_write(LED3,OFF);
    gpio_write(LED4,OFF);
    delay_ms(500);
    
    gpio_write(LED1,OFF);
    gpio_write(LED2,ON);
    gpio_write(LED3,OFF);
    gpio_write(LED4,OFF);
    delay_ms(500);
    
    gpio_write(LED1,OFF);
    gpio_write(LED2,OFF);
    gpio_write(LED3,ON);
    gpio_write(LED4,OFF);
    delay_ms(500);
    
    gpio_write(LED1,OFF);
    gpio_write(LED2,OFF);
    gpio_write(LED3,OFF);
    gpio_write(LED4,ON);
    delay_ms(500);
 }


