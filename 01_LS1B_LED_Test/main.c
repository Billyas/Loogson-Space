#include <stdio.h>

#include "ls1b.h"
#include "mips.h"

//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"
#include "ls1b_gpio.h"
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#define LED1 54
#define LED2 55
#define LED3 2
#define LED4 3



#define ON  0
#define OFF 1


//LED��ʼ��
 void LED_IO_Config(void)
 {
    //����GPIOΪ���״̬

    gpio_enable(LED4,DIR_OUT);
    gpio_enable(LED3,DIR_OUT);
    gpio_enable(LED2,DIR_OUT);
    gpio_enable(LED1,DIR_OUT);

    gpio_enable(8, DIR_IN);    // Pin: LCD_B3
    gpio_enable(9, DIR_IN);    // Pin: LCD_B4
    gpio_enable(10, DIR_IN);    // Pin: LCD_B5
    
    gpio_write(8, ON);
    gpio_write(9, ON);
    gpio_write(10, ON);
    
    gpio_write(LED4,OFF);//�ر�LED
    gpio_write(LED3,OFF);//�ر�LED
    gpio_write(LED2,OFF);//�ر�LED
    gpio_write(LED1,OFF);//�ر�LED
 }


//LED���Ժ���
 void LED_Test(void)
 {
    gpio_write(LED4,ON);//����LED
    printk("open led4\n");
    delay_ms(500);

    gpio_write(LED4,OFF);//�ر�LED
    printk("close led4\n");
    delay_ms(500);
 }



int main(void)
{
    printk("\r\nmain() function.\r\n");
    LED_IO_Config();

    for (;;)
    {
        LED_Test();
    }

    return 0;
}



