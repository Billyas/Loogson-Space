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
 **函数名：LED_IO_Config
 **函数功能：初始化LED的IO口
 **形参：无
 **返回值：无
 **说明：
 *******************************************************************/
 void LED_IO_Config_Init(void)
 {
    //库开发
    gpio_enable(LED1,DIR_OUT);
    gpio_enable(LED2,DIR_OUT);
    gpio_enable(LED3,DIR_OUT);
    gpio_enable(LED4,DIR_OUT);
 }

/*******************************************************************
 **函数名：LEDx_Status
 **函数功能：设置单个LED灯的状态
 **形参：
    @int ledx -- x:1,2,3,4
    @bool status -- ON或OFF
 **返回值：无
 **说明：
 *******************************************************************/
 void LEDx_Set_Status(u8 LEDx, u8 status)
 {
    gpio_write(LEDx,status);
 }
 
/*******************************************************************
 **函数名：LED_All_ON
 **函数功能：点亮所有的灯
 **形参：无
 **返回值：无
 **说明：
 *******************************************************************/
 void LED_All_ON(void)
 {
    gpio_write(LED1,ON);
    gpio_write(LED2,ON);
    gpio_write(LED3,ON);
    gpio_write(LED4,ON);
 }
 
 /*******************************************************************
 **函数名：LED_All_OFF
 **函数功能：关闭所有的灯
 **形参：无
 **返回值：无
 **说明：
 *******************************************************************/
 void LED_All_OFF(void)
 {
    gpio_write(LED1,OFF);
    gpio_write(LED2,OFF);
    gpio_write(LED3,OFF);
    gpio_write(LED4,OFF);
 }
 
 /*******************************************************************
 **函数名：LED_Waterfall
 **函数功能：流水灯
 **形参：无
 **返回值：无
 **说明：
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


