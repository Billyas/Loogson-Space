/*
 * interrupt.c
 *
 * created: 2021/2/11
 * authour: 
 */


#include "ls1b_gpio.h"
#include "ls1b_irq.h"
#include "interrupt.h"


//LED初始化与关闭LED
void Led_All_Off(void)
{
    gpio_enable(LED1, DIR_OUT);
    gpio_enable(LED2, DIR_OUT);
    gpio_enable(LED3, DIR_OUT);
    gpio_enable(LED4, DIR_OUT);
    gpio_write(LED1, OFF);
    gpio_write(LED2, OFF);
    gpio_write(LED3, OFF);
    gpio_write(LED4, OFF);

    return;
}

//外部中断服务函数
int on = 0;
static void gpio_interrupt_isr(int vector, void * param)
{

    printk("gpio_interrupt_isr\n");
    gpio_write(LED4, on);
    on = !on;
    //delay_us(500000);       //中断中不建议加延时函数，增加延时函数，可使实验效果更明显
    return;
}

//外部中断初始化
void Gpio_Interrupt_Init(void)
{
    //ls1x_install_gpio_isr(KEY_UP, INT_TRIG_EDGE_UP, gpio_interrupt_isr, 0);    /* 上升沿触发 */
    ls1x_install_gpio_isr(KEY_UP, INT_TRIG_EDGE_DOWN, gpio_interrupt_isr, 0);  /* 下降沿触发 */
    //ls1x_install_gpio_isr(KEY_UP, INT_TRIG_LEVEL_HIGH, gpio_interrupt_isr, 0);  /* 高电平触发 */
    //ls1x_install_gpio_isr(KEY_UP, INT_TRIG_LEVEL_LOW, gpio_interrupt_isr, 0);  /* 低电平触发 */
    ls1x_enable_gpio_interrupt(KEY_UP);    //使能按键的中断

    return;
}

