/*
 * rt_ls1x_drv_init.c
 *
 */

#include "bsp.h"

#include "drv_uart.h"

#if defined(BSP_USE_CAN0) || defined(BSP_USE_CAN1)
#include "drv_can.h"
#endif

#ifdef BSP_USE_NAND
#include "drv_nand.h"
#endif

#ifdef BSP_USE_FB
#include "drv_fb.h"
#endif

#if defined(BSP_USE_PWM0) || defined(BSP_USE_PWM1) || \
    defined(BSP_USE_PWM2) || defined(BSP_USE_PWM3)
#include "drv_pwm.h"
#endif

/*
 * I2C ���豸
 */
#ifdef BSP_USE_I2C0
#ifdef PCA9557_DRV
#include "i2c/drv_pca9557.h"    // GPIO оƬ
#endif
#ifdef ADS1015_DRV
#include "i2c/drv_ads1015.h"    // 4· 12bit ADC
#endif
#ifdef MCP4725_DRV
#include "i2c/drv_mcp4725.h"    // 1· 12bit DAC
#endif
#ifdef RX8010_DRV
#include "i2c/drv_rx8010.h"     // RTC оƬ
#endif
#endif

/*
 * SPI ���豸
 */
#ifdef BSP_USE_SPI0
#ifdef W25X40_DRV
#include "spi/drv_w25x40.h"         // SPI Flash
#endif
#if defined(BSP_USE_FB) && defined(XPT2046_DRV)
#include "spi/drv_xpt2046.h"        // ������оƬ
#endif
#endif

//-----------------------------------------------------------------------------
// Initialize drivers according BSP's Configuration
//-----------------------------------------------------------------------------

int rt_ls1x_drv_init(void)
{
    rt_ls1x_uart_install();

    #if defined(BSP_USE_CAN0) || defined(BSP_USE_CAN1)
        rt_ls1x_can_install();
    #endif

    #ifdef BSP_USE_NAND
        rt_ls1x_nand_install();
    #endif

    #ifdef BSP_USE_FB
        rt_ls1x_fb_install();
    #endif

    #if defined(BSP_USE_PWM0) || defined(BSP_USE_PWM1) || \
        defined(BSP_USE_PWM2) || defined(BSP_USE_PWM3)
        rt_ls1x_pwm_install();
    #endif

    /*
     * I2C ���豸
     */
    #ifdef PCA9557_DRV
        rt_ls1x_pca9557_install();      // GPIO оƬ
    #endif
    #ifdef ADS1015_DRV
        rt_ls1x_ads1015_install();      // 4· 12bit ADC
    #endif
    #ifdef MCP4725_DRV
        rt_ls1x_mcp4725_install();      // 1· 12bit DAC
    #endif
    #ifdef RX8010_DRV
        rt_ls1x_rx8010_install();       // RTC оƬ
    #endif

    /*
     * SPI ���豸
     */
    #ifdef W25X40_DRV
        rt_ls1x_w25x40_install();       // SPI Flash
    #endif
    #ifdef XPT2046_DRV
        rt_ls1x_xpt2046_install();      // ������оƬ
    #endif

    return 0;
}

