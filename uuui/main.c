/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Loongson 1B Bare Program, Sample main file
 */

#include <stdio.h>
#include <math.h>

#include "ls1b.h"
#include "mips.h"

//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"

#if defined(BSP_USE_CAN0) || defined(BSP_USE_CAN1)
  #include "ls1x_can.h"
#endif

#ifdef BSP_USE_FB
  #include "ls1x_fb.h"
  #include "gui/demo_gui.h"
  #include "gui/simple-gui/simple_gui.h"
  #ifdef XPT2046_DRV
    #include "spi/xpt2046.h"
    char LCD_display_mode[] = LCD_800x480;
  #elif defined(GT1151_DRV)
    #include "i2c/gt1151.h"
    #include "touch.h"
    char LCD_display_mode[] = LCD_480x800;
  #else
    #error "在bsp.h中选择配置 XPT2046_DRV 或者 GT1151_DRV"
           "XPT2046_DRV:  用于800*480 横屏的触摸屏."
           "GT1151_DRV:   用于480*800 竖屏的触摸屏."
           "如果都不选择, 注释掉本 error 信息, 然后自定义: LCD_display_mode[]"
  #endif
#endif

#if defined(BSP_USE_GMAC0)
  #include "lwip/netif.h"
  #include "lwip/init.h"
  extern struct netif *p_gmac0_netif;
#endif

//-------------------------------------------------------------------------------------------------

extern void start_my_gui(void);

extern int lx1x_can0_init_transmit(int gui);
extern int ls1x_can0_do_transmit(void *can);
extern int lx1x_can1_init_receive(int gui);
extern int ls1x_can1_do_receive(void *can);

extern int lx1x_i2c0_slaves_initialize(int gui);
extern int ls1x_i2c0_slaves_do_work(void);

extern void ethernetif_input(struct netif *netif);

//-------------------------------------------------------------------------------------------------
// 主循环
//-------------------------------------------------------------------------------------------------

int main(void)
{
    printf("\r\nmain() function.\r\n");

    ls1x_drv_init();            		/* Initialize device drivers */

    install_3th_libraries();      		/* Install 3th libraies */

#ifdef GP7101_DRV
	set_lcd_brightness(70);    			/* 设置 LCD 亮度 */
#endif

#ifdef BSP_USE_FB
	// do_touchscreen_calibrate();      /* 触摸屏校正 */
	start_my_gui();						/* start simple gui */
#endif

#ifdef BSP_USE_CAN0
    lx1x_can0_init_transmit(1);
#endif

#ifdef BSP_USE_CAN1
    lx1x_can1_init_receive(1);
#endif

#if defined(ADS1015_DRV) || defined(MCP4725_DRV) || defined(RX8010_DRV)
    lx1x_i2c0_slaves_initialize(1);
#endif

    /*
     * 裸机主循环
     */
    for (;;)
    {
#if defined(BSP_USE_GMAC0)
        ethernetif_input(p_gmac0_netif);
#endif

        delay_ms(10);

        /*
         * 触摸屏
         */
#ifdef XPT2046_DRV

        int x, y;

        if (bare_get_touch_point(&x, &y) == 0)
        {
            set_focused_button(x, y);
        }

#elif defined(GT1151_DRV)

        if (GT1151_Scan(0))
        {
            int x, y;
            x = tp_dev.x[0];
            y = tp_dev.y[0];

            set_focused_button(x, y);
        }

#endif

#ifdef BSP_USE_CAN0
        {
            ls1x_can0_do_transmit(devCAN0);
            delay_ms(10); // 50
        }
#endif

#ifdef BSP_USE_CAN1
        {
            ls1x_can1_do_receive(devCAN1);
            delay_ms(10); // 50
        }
#endif

#if defined(ADS1015_DRV) || defined(MCP4725_DRV)
        {
            ls1x_i2c0_slaves_do_work();
            delay_ms(20); // 100
        }
#endif
        
#if 0
        {
            tickcount = get_clock_ticks();
            printf("tick count = %i\r\n", tickcount);

            delay_ms(500);
        }
#endif
    }

    /*
     * Never goto here!
     */
    return 0;
}

/*
 * @@ End
 */

