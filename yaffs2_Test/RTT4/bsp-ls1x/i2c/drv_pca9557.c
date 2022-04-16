/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_pca9557.c
 *
 * created: 2021/1/7
 *  author: Bian
 */

#include "bsp.h"

#if defined(PCA9557_DRV)

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

#include "ls1x_i2c_bus.h"
#include "i2c/pca9557.h"

#include "drv_pca9557.h"

//-------------------------------------------------------------------------------------------------
// RTThread device for Loongson 1x
//-------------------------------------------------------------------------------------------------

/*
 * These functions glue I2C0-PCA9557 device to RTThread.
 */
static rt_err_t rt_pca9557_init(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_pca9557_init(dev->user_data, NULL) != 0)
        return RT_ERROR;

    return RT_EOK;
}

static rt_size_t rt_pca9557_read(struct rt_device *dev,
                                 rt_off_t          pos,
                                 void             *buffer,
                                 rt_size_t         size)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (size == 0)
        return 0;

    /*
     * buffer is unsigned char *
     */
    return ls1x_pca9557_read(dev->user_data, buffer, (int)size, 0);
}

static rt_size_t rt_pca9557_write(struct rt_device *dev,
                                  rt_off_t          pos,
                                  const void       *buffer,
                                  rt_size_t         size)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (size == 0)
        return 0;

    /*
     * buffer is unsigned char *
     */
    return ls1x_pca9557_write(dev->user_data, (void *)buffer, (int)size, 0);
}

/******************************************************************************
 * I2C0-PCA9557 devices
 */
static struct rt_device rt_ls1x_i2c0_pca9557;

/******************************************************************************
 * I2C0-PCA9557 register
 */
static rt_err_t rt_ls1x_pca9557_register(struct rt_device *dev, void *pIIC, rt_uint32_t flag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(pIIC != RT_NULL);

    dev->type        = RT_Device_Class_I2CSlave;
    dev->rx_indicate = RT_NULL;
    dev->tx_complete = RT_NULL;

    dev->init        = rt_pca9557_init;
    dev->open        = RT_NULL;
    dev->close       = RT_NULL;
    dev->read        = rt_pca9557_read;
    dev->write       = rt_pca9557_write;
    dev->control     = RT_NULL;

    dev->user_data   = pIIC;

    /* register a i2c-device device
     */
    return rt_device_register(dev, PCA9557_DEVICE_NAME, flag);
}

//-----------------------------------------------------------------------------
// Register Loongson I2C0-PCA9557 devices
//-----------------------------------------------------------------------------

void rt_ls1x_pca9557_install(void)
{
    rt_ls1x_pca9557_register(&rt_ls1x_i2c0_pca9557, busI2C0, RT_DEVICE_FLAG_RDWR);
}

#endif


