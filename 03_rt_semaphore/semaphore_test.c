/*
 * semaphore_test.c
 *
 * created: 2021/3/18
 *  author: 
 */

#include "ls1b_gpio.h"
#include "semaphore_test.h"

/*
 * 初始化LED&KEY对应的gpio模式
 */
void Test_Gpio_Init(void)
{  
    gpio_enable(LED1, DIR_OUT);
    gpio_enable(LED2, DIR_OUT);
    gpio_enable(LED3, DIR_OUT);
    gpio_enable(LED4, DIR_OUT);
}


/*
 * 4个led全部点亮
 */
void Led_All_On(void)
{
    gpio_write(LED1, ON);
    gpio_write(LED2, ON);
    gpio_write(LED3, ON);
    gpio_write(LED4, ON);
}


/*
 * 4个led全部关闭
 */
void Led_All_Off(void)
{
    gpio_write(LED1, OFF);
    gpio_write(LED2, OFF);
    gpio_write(LED3, OFF);
    gpio_write(LED4, OFF);
}


/*
 * 8个led实现跑马灯
 */
void Led_Test(void)
{
    gpio_write(LED1, ON);
    gpio_write(LED4, OFF);
    delay_ms(500);
    gpio_write(LED2, ON);
    gpio_write(LED1, OFF);
    delay_ms(500);
    gpio_write(LED3, ON);
    gpio_write(LED2, OFF);
    delay_ms(500);
    gpio_write(LED4, ON);
    gpio_write(LED3, OFF);
    delay_ms(500);
}










