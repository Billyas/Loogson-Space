/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_mcp4725.c
 *
 * created: 2021/1/7
 *  author: Bian
 */

#include "bsp.h"

#if defined(MCP4725_DRV)

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
#include "i2c/mcp4725.h"

#include "drv_mcp4725.h"

//-------------------------------------------------------------------------------------------------
// RTThread device for Loongson 1x
//-------------------------------------------------------------------------------------------------

/*
 * These functions glue I2C0-MCP4725 device to RTThread.
 */
static rt_size_t rt_mcp4725_write(struct rt_device *dev,
                                  rt_off_t          pos,
                                  const void       *buffer,
                                  rt_size_t         size)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (size == 0)
        return 0;

    /*
     * buffer is unsigned short *
     */
    return ls1x_mcp4725_write(dev->user_data, (void *)buffer, (int)size, NULL);
}

static rt_err_t rt_mcp4725_control(struct rt_device *dev,
                                   int               cmd,
                                   void             *args)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    ls1x_mcp4725_ioctl(dev->user_data, (unsigned)cmd, args);

    return RT_EOK;
}

/******************************************************************************
 * I2C0-MCP4725 devices
 */
static struct rt_device rt_ls1x_i2c0_mcp4725;

/******************************************************************************
 * I2C0-MCP4725 register
 */
static rt_err_t rt_ls1x_mcp4725_register(struct rt_device *dev, void *pIIC, rt_uint32_t flag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(pIIC != RT_NULL);

    dev->type        = RT_Device_Class_I2CSlave;
    dev->rx_indicate = RT_NULL;
    dev->tx_complete = RT_NULL;

    dev->init        = RT_NULL;
    dev->open        = RT_NULL;
    dev->close       = RT_NULL;
    dev->read        = RT_NULL;
    dev->write       = rt_mcp4725_write;
    dev->control     = rt_mcp4725_control;

    dev->user_data   = pIIC;

    /* register a i2c-device device
     */
    return rt_device_register(dev, MCP4725_DEVICE_NAME, flag);
}

//-----------------------------------------------------------------------------
// Register Loongson I2C0-MCP4725 devices
//-----------------------------------------------------------------------------

void rt_ls1x_mcp4725_install(void)
{
    rt_ls1x_mcp4725_register(&rt_ls1x_i2c0_mcp4725, busI2C0, RT_DEVICE_FLAG_RDWR);
}

#endif


