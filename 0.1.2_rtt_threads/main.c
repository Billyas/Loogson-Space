/*
 * RT-Thread Application
 */

#include <time.h>

#include "rtthread.h"

//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"
#include "ls1b_gpio.h"
//-------------------------------------------------------------------------------------------------
// Simple demo of task
//-------------------------------------------------------------------------------------------------



#define LED1 54
#define LED2 55
#define LED3 2
#define LED4 3

#define ON  0
#define OFF 1


//LED≥ı ºªØ
 void LED_IO_Config(void)
 {
    //Â∫ìÂáΩÊï∞Êìç‰Ωú
    gpio_enable(LED1,DIR_OUT);
    gpio_enable(LED2,DIR_OUT);
    gpio_enable(LED3,DIR_OUT);
    gpio_enable(LED4,DIR_OUT);
 }


//LEDøÿ÷∆
 void LED_Waterfall1(void)
 {
    gpio_write(LED1,ON);
    gpio_write(LED2,OFF);
    rt_thread_delay(500);

    gpio_write(LED1,OFF);
    gpio_write(LED2,ON);
    rt_thread_delay(500);

 }
//LEDøÿ÷∆
 void LED_Waterfall2(void)
 {

    gpio_write(LED3,ON);
    gpio_write(LED4,OFF);
    rt_thread_delay(200);

    gpio_write(LED3,OFF);
    gpio_write(LED4,ON);
    rt_thread_delay(200);
 }



static rt_thread_t m_demo_thread = NULL;
static rt_thread_t m_demo_thread2 = NULL;

static void demo_thread(void *arg)
{
	unsigned int tickcount;

    for ( ; ; )
    {
        LED_Waterfall1();
        rt_thread_delay(500);
    }
}

static void demo_thread2(void *arg)
{
	unsigned int tickcount;

    for ( ; ; )
    {
        LED_Waterfall2();
        rt_thread_delay(500);
    }
}

//-------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	rt_kprintf("\r\nWelcome to RT-Thread.\r\n\r\n");
LED_IO_Config();
	/*
     * Task initializing...
     */
    m_demo_thread = rt_thread_create("demothread",
                                      demo_thread,
                                      NULL,         // arg
                                      1024*4,       // statck size
                                      11,           // priority
                                      10);          // slice ticks

    if (m_demo_thread == NULL)
    {
    	rt_kprintf("create demo thread fail!\r\n");
	}
	else
    {
    	rt_thread_startup(m_demo_thread);
    }

    m_demo_thread2 = rt_thread_create("demothread2",
                                      demo_thread2,
                                      NULL,         // arg
                                      1024*4,       // statck size
                                      11,           // priority
                                      10);          // slice ticks

    if (m_demo_thread2 == NULL)
    {
    	rt_kprintf("create demo thread fail!\r\n");
	}
	else
    {
    	rt_thread_startup(m_demo_thread2);
    }


	/*
	 * Finsh as another thread...
	 */

    return 0;
}

