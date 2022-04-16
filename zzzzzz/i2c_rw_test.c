/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * demo 需要打开 framebuffer, 使用simple gui进行显示
 */

#include "bsp.h"

#if defined(ADS1015_DRV) || defined(MCP4725_DRV)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ls1x_rtc.h"
#include "ls1x_i2c_bus.h"

#include "i2c/pca9557.h"
#include "i2c/mcp4725.h"
#include "i2c/ads1015.h"
#include "i2c/rx8010.h"

#include "app_os_priority.h"

//-----------------------------------------------------------------------------
// 任务/线程测试
//-----------------------------------------------------------------------------

#if defined(OS_RTTHREAD)
#include "rtthread.h"
#include "i2c/drv_ads1015.h"
#include "i2c/drv_mcp4725.h"
#include "i2c/drv_rx8010.h"
#define SLEEP           rt_thread_sleep
static  rt_thread_t     i2c0_test_thread;
#elif defined(OS_UCOS)
#include "os.h"
#define SLEEP           OSTimeDly
static  OS_STK          i2c0_slaves_stack[I2C0_STK_SIZE];
#elif defined(OS_FREERTOS)
#include "FreeRTOS.h"
#define SLEEP           vTaskDelay
#else
#define SLEEP(x)        delay_ms(x)
#endif

//-----------------------------------------------------------------------------

#if defined(OS_RTTHREAD)
#define SPRINTF         rt_sprintf
#else
#define SPRINTF         sprintf
#endif

#define S_LEN   64
static unsigned char info_buf[S_LEN+1];

//-----------------------------------------------------------------------------

#ifdef XPT2046_DRV
#define DATA_COL    3
#elif defined(GT1151_DRV)
#define DATA_COL    2
#else
#define DATA_COL    3
#endif

//-----------------------------------------------------------------------------

#if (BSP_USE_OS)

#ifndef I2C0_STK_SIZE
//#define I2C0_STK_SIZE   2048
#endif

/*
 * I2C0 任务
 */
static void i2c0_slaves_test_task(void *arg)
{
    uint16_t dac = 0;      /* 需要累计然后输出 */
    
#if defined(OS_RTTHREAD)

  #ifdef MCP4725_DRV
    rt_device_t devDAC;
    
    //rt_ls1x_mcp4725_install();  // if no install, install it
    devDAC = rt_device_find(MCP4725_DEVICE_NAME);
    if (devDAC == NULL)
        return;
    rt_device_open(devDAC, RT_DEVICE_FLAG_WRONLY);
  #endif
  
  #ifdef ADS1015_DRV
    rt_device_t devADC;
    
    //rt_ls1x_ads1015_install();  // if no install, install it
    devADC = rt_device_find(ADS1015_DEVICE_NAME);
    if (devADC == NULL)
        return;
    rt_device_open(devADC, RT_DEVICE_FLAG_RDONLY);
  #endif

  #ifdef RX8010_DRV
    rt_device_t devRTC;
    
    //rt_ls1x_rx8010_install();   // if no install, install it
    devRTC = rt_device_find(RX8010_DEVICE_NAME);
    if (devRTC == NULL)
        return;
    rt_device_open(devRTC, RT_DEVICE_FLAG_RDWR);
    
  #endif
  
#endif
  
    /*
     * 初始化 RTC 时间
     */
    struct tm t;

    t.tm_year = 2021-1900;
    t.tm_mon  = 8-1;
    t.tm_mday = 19;
    t.tm_hour = 8;
    t.tm_min  = 0;
    t.tm_sec  = 0;
     
#ifdef BSP_USE_RTC
    ls1x_rtc_set_datetime(&t);
    
#elif defined(RX8010_DRV)

  #if defined(OS_RTTHREAD)
    rt_device_write(devRTC, 0, (const void *)&t, sizeof(struct tm));
  #else
    ls1x_rx8010_init(busI2C0, NULL);
    set_rx8010_datetime(busI2C0, &t);
  #endif

#endif

    SLEEP(100);
#ifdef BSP_USE_FB
    gui_drawtext_in_grid(3, 0, "DAC");  gui_drawtext_in_grid(3, 1, "set");
    gui_drawtext_in_grid(5, 0, "ADC0"); gui_drawtext_in_grid(5, 1, "get");
    gui_drawtext_in_grid(6, 0, "ADC1"); gui_drawtext_in_grid(6, 1, "get");
    gui_drawtext_in_grid(7, 0, "ADC2"); gui_drawtext_in_grid(7, 1, "get");
    gui_drawtext_in_grid(8, 0, "ADC3"); gui_drawtext_in_grid(8, 1, "get");
    gui_drawtext_in_grid(10, 0, "RTC"); gui_drawtext_in_grid(10, 1, "time");
#endif
    DBG_OUT("i2c0_test_task started.\r\n");
    
    for ( ; ; )
    {
        uint16_t adc[4];

    #ifdef MCP4725_DRV
        /*
         * 写 mcp4725
         */
      #if defined(OS_RTTHREAD)
        rt_device_write(devDAC, 0, (const void *)&dac, 2);
      #else
        ls1x_mcp4725_write(busI2C0, (unsigned char *)&dac, 2, NULL);
      #endif

        SPRINTF(info_buf, " %i", dac);
      #if defined(XPT2046_DRV) || defined(GT1151_DRV)
        gui_drawtext_in_grid(3, DATA_COL, info_buf);
      #else
        printk("MCP4725 DAC set: %s\r\n", info_buf);
      #endif
        
        dac += 5;
        if (dac > 4096)         // 12 bits limited
            dac = 0;
    #endif

    #ifdef ADS1015_DRV
        /*
         * 读 ADS1015
         */
      #if defined(OS_RTTHREAD)
        rt_device_read(devADC, ADS1015_CHANNEL_S0, (void *)&adc[0], 2);
        rt_device_read(devADC, ADS1015_CHANNEL_S1, (void *)&adc[1], 2);
        rt_device_read(devADC, ADS1015_CHANNEL_S2, (void *)&adc[2], 2);
        rt_device_read(devADC, ADS1015_CHANNEL_S3, (void *)&adc[3], 2);
      #else
        adc[0] = get_ads1015_adc(busI2C0, ADS1015_REG_CONFIG_MUX_SINGLE_0);
        adc[1] = get_ads1015_adc(busI2C0, ADS1015_REG_CONFIG_MUX_SINGLE_1);
        adc[2] = get_ads1015_adc(busI2C0, ADS1015_REG_CONFIG_MUX_SINGLE_2);
        adc[3] = get_ads1015_adc(busI2C0, ADS1015_REG_CONFIG_MUX_SINGLE_3);
      #endif

      #if defined(XPT2046_DRV) || defined(GT1151_DRV)
        SPRINTF(info_buf, " %i", adc[0]);
        gui_drawtext_in_grid(5, DATA_COL, info_buf);
        SPRINTF(info_buf, " %i", adc[1]);
        gui_drawtext_in_grid(6, DATA_COL, info_buf);
        SPRINTF(info_buf, " %i", adc[2]);
        gui_drawtext_in_grid(7, DATA_COL, info_buf);
        SPRINTF(info_buf, " %i", adc[3]);
        gui_drawtext_in_grid(8, DATA_COL, info_buf);
      #else
        printk("ADS1015 ADC0 get: %i\r\n", adc[0]);
        printk("ADS1015 ADC1 get: %i\r\n", adc[1]);
        printk("ADS1015 ADC2 get: %i\r\n", adc[2]);
        printk("ADS1015 ADC3 get: %i\r\n", adc[3]);
      #endif
    #endif
        
    #if defined(BSP_USE_RTC) || defined(RX8010_DRV)
        /*
         * 读 RTC 时间
         */
      #ifdef BSP_USE_RTC
        ls1x_rtc_get_datetime(&t);
      #elif defined(RX8010_DRV)

        #if defined(OS_RTTHREAD)
        rt_device_read(devRTC, 0, (void *)&t, sizeof(struct tm));
        #else
        get_rx8010_datetime(busI2C0, &t);
        #endif
      #endif
      
        SPRINTF(info_buf, " %i.%i.%i-%i.%i.%i",
                t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

      #if defined(XPT2046_DRV) || defined(GT1151_DRV)
        gui_drawtext_in_grid(10, DATA_COL, info_buf);
      #else
        printk("RTC time: %s\r\n", info_buf);
      #endif

    #endif

        /* abandon cpu time to run other task */
        SLEEP(200);                     // task sleep 100 ms
    }
}

/*
 * 创建 I2C0 任务
 */
int i2c0_slaves_test_start(void)
{
#if defined(OS_RTTHREAD)

    i2c0_test_thread = rt_thread_create("i2c0slavestask",
                                         i2c0_slaves_test_task,
                                         NULL,              // arg
                                         I2C0_STK_SIZE*4,   // statck size
                                         I2C0_TASK_PRIO,    // priority
                                         I2C0_TASK_SLICE);  // slice ticks

    if (i2c0_test_thread == NULL)
    {
        printk("create i2c0 devices test thread fail!\r\n");
		return -1;
	}

	rt_thread_startup(i2c0_test_thread);

#elif defined(OS_UCOS)

    OSTaskCreate(i2c0_slaves_test_task,
                 NULL,
        #if OS_STK_GROWTH == 1
                 (void *)&i2c0_slaves_stack[I2C0_STK_SIZE-1],
        #else
                 (void *)&i2c0_slaves_stack[0];
        #endif
                 I2C0_TASK_PRIO);

#elif defined(OS_FREERTOS)

    xTaskCreate(i2c0_slaves_test_task,
                "i2c0slavestask",
                I2C0_STK_SIZE, NULL,
                I2C0_TASK_PRIO, NULL);
                
#else

#endif

    return 0;
}

#else // #if (BSP_USE_OS)

/*
 * OS_NONE
 */
int lx1x_i2c0_slaves_initialize(int gui)
{
    /*
     * 设备初始化, 仅需要初始化 RX8010
     */
    struct tm t;

    t.tm_year = 2021-1900;
    t.tm_mon  = 8-1;
    t.tm_mday = 19;
    t.tm_hour = 8;
    t.tm_min  = 0;
    t.tm_sec  = 0;

#ifdef BSP_USE_RTC
    ls1x_rtc_set_datetime(&t);
#elif defined(RX8010_DRV)
    //ls1x_rx8010_init(busI2C0, NULL);
    set_rx8010_datetime(busI2C0, &t);
#endif

#ifdef BSP_USE_FB
    if (gui)
    {
        gui_drawtext_in_grid(3, 0, "DAC");  gui_drawtext_in_grid(3, 1, "set");
        gui_drawtext_in_grid(5, 0, "ADC0"); gui_drawtext_in_grid(5, 1, "get");
        gui_drawtext_in_grid(6, 0, "ADC1"); gui_drawtext_in_grid(6, 1, "get");
        gui_drawtext_in_grid(7, 0, "ADC2"); gui_drawtext_in_grid(7, 1, "get");
        gui_drawtext_in_grid(8, 0, "ADC3"); gui_drawtext_in_grid(8, 1, "get");
        gui_drawtext_in_grid(10, 0, "RTC"); gui_drawtext_in_grid(10, 1, "time");
    }
#endif

    DBG_OUT("i2c0 slaves initialized.\r\n");
    
    return 0;
}

//-----------------------------------------------------------------------------

static uint16_t dac = 0;

int ls1x_i2c0_slaves_do_work(void)
{
    uint16_t adc[4];

#ifdef MCP4725_DRV
    /*
     * 写 mcp4725
     */
    ls1x_mcp4725_write(busI2C0, (unsigned char *)&dac, 2, NULL);

    SPRINTF(info_buf, " %i", dac);
  #if defined(XPT2046_DRV) || defined(GT1151_DRV)
    gui_drawtext_in_grid(3, DATA_COL, info_buf);
  #else
    printk("MCP4725 DAC set: %s\r\n", info_buf);
  #endif

    dac += 5;
    if (dac > 4096)         // 12 bits limited
        dac = 0;
#endif

#ifdef ADS1015_DRV
    /*
     * 读 ADS1015
     */
    adc[0] = get_ads1015_adc(busI2C0, ADS1015_REG_CONFIG_MUX_SINGLE_0);
    adc[1] = get_ads1015_adc(busI2C0, ADS1015_REG_CONFIG_MUX_SINGLE_1);
    adc[2] = get_ads1015_adc(busI2C0, ADS1015_REG_CONFIG_MUX_SINGLE_2);
    adc[3] = get_ads1015_adc(busI2C0, ADS1015_REG_CONFIG_MUX_SINGLE_3);

  #if defined(XPT2046_DRV) || defined(GT1151_DRV)
    SPRINTF(info_buf, " %i", adc[0]);
    gui_drawtext_in_grid(5, DATA_COL, info_buf);
    SPRINTF(info_buf, " %i", adc[1]);
    gui_drawtext_in_grid(6, DATA_COL, info_buf);
    SPRINTF(info_buf, " %i", adc[2]);
    gui_drawtext_in_grid(7, DATA_COL, info_buf);
    SPRINTF(info_buf, " %i", adc[3]);
    gui_drawtext_in_grid(8, DATA_COL, info_buf);
  #else
    printk("ADS1015 ADC0 get: %i\r\n", adc[0]);
    printk("ADS1015 ADC1 get: %i\r\n", adc[1]);
    printk("ADS1015 ADC2 get: %i\r\n", adc[2]);
    printk("ADS1015 ADC3 get: %i\r\n", adc[3]);
  #endif
#endif

    /*
     * 读 RTC 时间
     */
#if defined(BSP_USE_RTC) || defined(RX8010_DRV)
     
    struct tm t;
    
  #ifdef BSP_USE_RTC
    ls1x_rtc_get_datetime(&t);
  #elif defined(RX8010_DRV)
    get_rx8010_datetime(busI2C0, &t);
  #endif

    SPRINTF(info_buf, " %i.%i.%i-%i.%i.%i",
            t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

 #if defined(XPT2046_DRV) || defined(GT1151_DRV)
    gui_drawtext_in_grid(10, DATA_COL, info_buf);
 #else
    printk("RTC time: %s\r\n", info_buf);
 #endif

#endif

}

#endif // #if (BSP_USE_OS)

#endif

/*
 * @END
 */
 
 

