/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Loongson 1B RT-Thread Application
 */

#include <time.h>

#include "rtthread.h"
//#include "webclient_get_sample.c"
//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"

#if defined(BSP_USE_FB)
  #include "ls1x_fb.h"
  #ifdef XPT2046_DRV
    char LCD_display_mode[] = LCD_800x480;
  #elif defined(GT1151_DRV)
    char LCD_display_mode[] = LCD_480x800;
  #else
    #error "��bsp.h��ѡ������ XPT2046_DRV ���� GT1151_DRV"
           "XPT2046_DRV:  ����800*480 �����Ĵ�����."
           "GT1151_DRV:   ����480*800 �����Ĵ�����."
           "�������ѡ��, ע�͵��� error ��Ϣ, Ȼ���Զ���: LCD_display_mode[]"
  #endif
#endif

//-------------------------------------------------------------------------------------------------
// Simple demo of task
//-------------------------------------------------------------------------------------------------

static rt_thread_t m_demo_thread = NULL;

static void demo_thread(void *arg)
{
	unsigned int tickcount;

    for ( ; ; )
    {
        tickcount = rt_tick_get();

        rt_kprintf("tick count = %i\r\n", tickcount);

        rt_thread_delay(500);
    }
}

//-------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	rt_kprintf("\r\nWelcome to RT-Thread.\r\n\r\n");

    ls1x_drv_init();            /* Initialize device drivers */

    rt_ls1x_drv_init();         /* Initialize device drivers for RTT */

    install_3th_libraries(); 	/* Install 3th libraies */

    unsigned char  a[4];
    a[0] = 192;
    a[1] = 168;
    a[2] = 5;
    a[3] = 64;

    ls1x_initialize_lwip(NULL,NULL);
	/*
	 * Finsh as another thread...
	 */
	//webclient_get_comm("http://d.billyme.top/ssh.txt");
    return 0;
}

/*
 * @@ End
 */
