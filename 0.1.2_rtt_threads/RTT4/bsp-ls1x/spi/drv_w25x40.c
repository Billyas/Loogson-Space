/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_w25x40.c
 *
 * created: 2021/1/6
 * authour: Bian
 */

#include "bsp.h"

#if defined(W25X40_DRV)

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
#include "spi/w25x40.h"

#include "drv_w25x40.h"

//-------------------------------------------------------------------------------------------------
// RTThread device for Loongson 1x
//-------------------------------------------------------------------------------------------------

/*
 * These functions glue SPI0-W25X40 device to RTThread.
 */
static rt_err_t rt_w25x40_init(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_w25x40_init(dev->user_data, NULL) != 0)
        return RT_ERROR;

    return RT_EOK;
}

static rt_err_t rt_w25x40_open(struct rt_device *dev, rt_uint16_t oflag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_w25x40_open(dev->user_data, NULL) != 0)
        return RT_ERROR;

    dev->open_flag = (oflag | RT_DEVICE_FLAG_STREAM) & 0xff;    /* set open flags */

    return RT_EOK;
}

static rt_err_t rt_w25x40_close(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    ls1x_w25x40_close(dev->user_data, NULL);

    return RT_EOK;
}

static rt_size_t rt_w25x40_read(struct rt_device *dev,
                                rt_off_t          pos,
                                void             *buffer,
                                rt_size_t         size)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (size == 0)
        return 0;

    /*
     * buffer is unsigned char *, pos is nor-flash inner linear address
     */
    return ls1x_w25x40_read(dev->user_data, buffer, (int)size, (void *)pos);
}

static rt_size_t rt_w25x40_write(struct rt_device *dev,
                                 rt_off_t          pos,
                                 const void       *buffer,
                                 rt_size_t         size)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (size == 0)
        return 0;
        
    /*
     * buffer is unsigned char *, pos is nor-flash inner linear address
     */
    return ls1x_w25x40_write(dev->user_data, buffer, (int)size, (void *)pos);
}

static rt_err_t rt_w25x40_control(struct rt_device *dev,
                                  int               cmd,
                                  void             *args)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    ls1x_w25x40_ioctl(dev->user_data, (unsigned)cmd, args);

    return RT_EOK;
}

/******************************************************************************
 * SPI0-W25X40 devices
 */
static struct rt_device rt_ls1x_spi0_w25x40;

/******************************************************************************
 * SPI0-W25X40 register
 */
static rt_err_t rt_ls1x_w25x40_register(struct rt_device *dev, void *pSPI, rt_uint32_t flag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(pSPI != RT_NULL);

    dev->type        = RT_Device_Class_SPIDevice;
    dev->rx_indicate = RT_NULL;
    dev->tx_complete = RT_NULL;

    dev->init        = rt_w25x40_init;
    dev->open        = rt_w25x40_open;
    dev->close       = rt_w25x40_close;
    dev->read        = rt_w25x40_read;
    dev->write       = rt_w25x40_write;
    dev->control     = rt_w25x40_control;

    dev->user_data   = pSPI;

    /* register a spi-device device
     */
    return rt_device_register(dev, W25X40_DEVICE_NAME, flag);
}

//-----------------------------------------------------------------------------
// Register Loongson SPI0-W25X40 devices
//-----------------------------------------------------------------------------

void rt_ls1x_w25x40_install(void)
{
    rt_ls1x_w25x40_register(&rt_ls1x_spi0_w25x40, busSPI0, RT_DEVICE_FLAG_RDWR);
}

#endif


