/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_can.c
 *
 * created: 2021/1/4
 *  author: Bian
 */

#include "bsp.h"

#if defined(BSP_USE_CAN0) || defined(BSP_USE_CAN1)

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

#include "ls1x_can.h"

#include "drv_can.h"

//-------------------------------------------------------------------------------------------------
// RTThread device for Loongson 1x
//-------------------------------------------------------------------------------------------------

/*
 * These functions glue CAN device to RTThread.
 */
static rt_err_t rt_can_init(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_can_init(dev->user_data, NULL) != 0)
        return RT_ERROR;

    return RT_EOK;
}

static rt_err_t rt_can_open(struct rt_device *dev, rt_uint16_t oflag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_can_open(dev->user_data, NULL) != 0)
        return RT_ERROR;

    dev->open_flag = (oflag | RT_DEVICE_FLAG_STREAM) & 0xff;    /* set open flags */

    return RT_EOK;
}

static rt_err_t rt_can_close(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    ls1x_can_close(dev->user_data, NULL);

    return RT_EOK;
}

static rt_size_t rt_can_read(struct rt_device *dev,
                             rt_off_t          pos,
                             void             *buffer,
                             rt_size_t         size)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (size == 0)
        return 0;

    /*
     * buffer is CANMsg_t *
     */
    return ls1x_can_read(dev->user_data, buffer, (int)size, 0);
}

static rt_size_t rt_can_write(struct rt_device *dev,
                              rt_off_t          pos,
                              const void       *buffer,
                              rt_size_t         size)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (size == 0)
        return 0;

    /*
     * buffer is CANMsg_t *
     */
    return ls1x_can_write(dev->user_data, (void *)buffer, (int)size, 0);
}

static rt_err_t rt_can_control(struct rt_device *dev,
                               int               cmd,
                               void             *args)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    ls1x_can_ioctl(dev->user_data, (unsigned)cmd, args);

    return RT_EOK;
}

/******************************************************************************
 * CAN devices
 */
#ifdef BSP_USE_CAN0
static struct rt_device rt_ls1x_can0;
#endif
#ifdef BSP_USE_CAN1
static struct rt_device rt_ls1x_can1;
#endif

/******************************************************************************
 * CAN register
 */
static rt_err_t rt_ls1x_can_register(struct rt_device *dev, void *pCAN, rt_uint32_t flag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(pCAN != RT_NULL);

    dev->type        = RT_Device_Class_CAN;
    dev->rx_indicate = RT_NULL;
    dev->tx_complete = RT_NULL;

    dev->init        = rt_can_init;
    dev->open        = rt_can_open;
    dev->close       = rt_can_close;
    dev->read        = rt_can_read;
    dev->write       = rt_can_write;
    dev->control     = rt_can_control;

    dev->user_data   = pCAN;

    /* register a can device
     */
    return rt_device_register(dev, ls1x_can_get_device_name(pCAN), flag);
}

//-----------------------------------------------------------------------------
// Register Loongson CAN devices
//-----------------------------------------------------------------------------

void rt_ls1x_can_install(void)
{
#ifdef BSP_USE_CAN0
    rt_ls1x_can_register(&rt_ls1x_can0, devCAN0, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_CAN1
    rt_ls1x_can_register(&rt_ls1x_can1, devCAN1, RT_DEVICE_FLAG_RDWR);
#endif
}

#endif


