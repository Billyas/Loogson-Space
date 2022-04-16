/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Loongson 1B RT-Thread Sample main file
 */

#include <time.h>

#include "rtthread.h"

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

extern void start_my_gui(void);
extern int can0_test_start(void);
extern int can1_test_start(void);
extern int i2c0_slaves_test_start(void);

//-------------------------------------------------------------------------------------------------

static void touchme(int x, int y)
{
    printk("touched at: x=%i, y=%i\r\n", x, y);
}

/******************************************************************************
 * ��ȷ���������� ?
 ******************************************************************************/

int main(int argc, char** argv)
{
	rt_kprintf("\r\nWelcome to RT-Thread.\r\n\r\n");

    ls1x_drv_init();            		/* Initialize device drivers */

    rt_ls1x_drv_init();         		/* Initialize device drivers for RTT */

    install_3th_libraries();      		/* Install 3th libraies */

#ifdef GP7101_DRV
    set_lcd_brightness(70);    			/* ���� LCD ���� */
#endif

#ifdef XPT2046_DRV
	// do_touchscreen_calibrate();      /* ������У�� */
	start_touchscreen_task(touchme);    /* touch_callback_t */
#endif

#ifdef BSP_USE_FB
	start_my_gui(); 					/* start simple gui */
#endif

#ifdef BSP_USE_CAN0
	can0_test_start();
#endif

#ifdef BSP_USE_CAN1
	can1_test_start();
#endif

#if defined(ADS1015_DRV) || defined(MCP4725_DRV)
	i2c0_slaves_test_start();
#endif

    rt_kprintf("main() is exit!\r\n");

	/*
	 * Finsh as another thread...
	 */
    return 0;
}

/*
 * @@ End
 */
