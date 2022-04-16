/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_xpt2046.c
 *
 * created: 2021/1/6
 *  author: Bian
 */

#include "bsp.h"

#if defined(XPT2046_DRV)

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#if defined(LS1B)
#include "ls1b.h"
#include "ls1b_irq.h"
#include "ls1b_gpio.h"
#elif defined(LS1C)
#include "ls1c.h"
#include "ls1c_irq.h"
#include "ls1c_gpio.h"
#else
#error "No Loongson SoC defined."
#endif

#include "ls1x_spi_bus.h"
#include "spi/xpt2046.h"

#include "drv_xpt2046.h"

//-------------------------------------------------------------------------------------------------
// RTThread device for Loongson 1x
//-------------------------------------------------------------------------------------------------

/*
 * These functions glue SPI0-XPT2046 device to RTThread.
 */
static rt_err_t rt_xpt2046_init(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_xpt2046_init(dev->user_data, NULL) != 0)
        return RT_ERROR;

    return RT_EOK;
}

static rt_size_t rt_xpt2046_read(struct rt_device *dev,
                                 rt_off_t          pos,
                                 void             *buffer,
                                 rt_size_t         size)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (size == 0)
        return 0;
        
    /*
     * buffer is ts_message_t *
     */
    return ls1x_xpt2046_read(dev->user_data, buffer, (int)size, NULL);
}

/******************************************************************************
 * SPI0-XPT2046 devices
 */
static struct rt_device rt_ls1x_spi0_xpt2046;

/******************************************************************************
 * SPI0-XPT2046 register
 */
static rt_err_t rt_ls1x_xpt2046_register(struct rt_device *dev, void *pSPI, rt_uint32_t flag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(pSPI != RT_NULL);

    dev->type        = RT_Device_Class_SPIDevice;
    dev->rx_indicate = RT_NULL;
    dev->tx_complete = RT_NULL;

    dev->init        = rt_xpt2046_init;
    dev->open        = RT_NULL;
    dev->close       = RT_NULL;
    dev->read        = rt_xpt2046_read;
    dev->write       = RT_NULL;
    dev->control     = RT_NULL;

    dev->user_data   = pSPI;

    /* register a spi-device device
     */
    return rt_device_register(dev, XPT2046_DEVICE_NAME, flag);
}

//-----------------------------------------------------------------------------
// Register Loongson SPI0-XPT2046 devices
//-----------------------------------------------------------------------------

void rt_ls1x_xpt2046_install(void)
{
    rt_ls1x_xpt2046_register(&rt_ls1x_spi0_xpt2046, busSPI0, RT_DEVICE_FLAG_RDWR);
}

#endif

