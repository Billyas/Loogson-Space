/*
 * interrupt.c
 *
 * created: 2021/2/11
 * authour: 
 */


#include "ls1b_gpio.h"
#include "ls1b_irq.h"
#include "interrupt.h"


//LED��ʼ����ر�LED
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

//�ⲿ�жϷ�����
int on = 0;
static void gpio_interrupt_isr(int vector, void * param)
{

    printk("gpio_interrupt_isr\n");
    gpio_write(LED4, on);
    on = !on;
    //delay_us(500000);       //�ж��в��������ʱ������������ʱ��������ʹʵ��Ч��������
    return;
}

//�ⲿ�жϳ�ʼ��
void Gpio_Interrupt_Init(void)
{
    //ls1x_install_gpio_isr(KEY_UP, INT_TRIG_EDGE_UP, gpio_interrupt_isr, 0);    /* �����ش��� */
    ls1x_install_gpio_isr(KEY_UP, INT_TRIG_EDGE_DOWN, gpio_interrupt_isr, 0);  /* �½��ش��� */
    //ls1x_install_gpio_isr(KEY_UP, INT_TRIG_LEVEL_HIGH, gpio_interrupt_isr, 0);  /* �ߵ�ƽ���� */
    //ls1x_install_gpio_isr(KEY_UP, INT_TRIG_LEVEL_LOW, gpio_interrupt_isr, 0);  /* �͵�ƽ���� */
    ls1x_enable_gpio_interrupt(KEY_UP);    //ʹ�ܰ������ж�

    return;
}

