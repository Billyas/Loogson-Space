/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_fb.c
 *
 * created: 2021/1/6
 *  author: Bian
 */

#include "bsp.h"

#if defined(BSP_USE_FB)

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

#include "ls1x_fb.h"

#include "drv_fb.h"

//-------------------------------------------------------------------------------------------------
// RTThread device for Loongson 1x
//-------------------------------------------------------------------------------------------------

/*
 * These functions glue FrameBuffer device to RTThread.
 */
static rt_err_t rt_fb_init(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_dc_init(dev->user_data, NULL) != 0)
        return RT_ERROR;

    return RT_EOK;
}

static rt_err_t rt_fb_open(struct rt_device *dev, rt_uint16_t oflag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_dc_open(dev->user_data, NULL) != 0)
        return RT_ERROR;

    dev->open_flag = (oflag | RT_DEVICE_FLAG_STREAM) & 0xff;    /* set open flags */

    return RT_EOK;
}

static rt_err_t rt_fb_close(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    ls1x_dc_close(dev->user_data, NULL);

    return RT_EOK;
}

static rt_size_t rt_fb_read(struct rt_device *dev,
                            rt_off_t          pos,
                            void             *buffer,
                            rt_size_t         size)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (size == 0)
        return 0;

    /*
     * buffer is unsigned char *, pos is frame-buffer inner linear address
     */
    return ls1x_dc_read(dev->user_data, buffer, (int)size, (void *)pos);
}

static rt_size_t rt_fb_write(struct rt_device *dev,
                             rt_off_t          pos,
                             const void       *buffer,
                             rt_size_t         size)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (size == 0)
        return 0;
        
    /*
     * buffer is unsigned char *, pos is frame-buffer inner linear address
     */
    return ls1x_dc_write(dev->user_data, (void *)buffer, (int)size, (void *)pos);
}

static rt_err_t rt_fb_control(struct rt_device *dev,
                              int               cmd,
                              void             *args)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    ls1x_dc_ioctl(dev->user_data, (unsigned)cmd, args);

    return RT_EOK;
}

/******************************************************************************
 * FB devices
 */
static struct rt_device rt_ls1x_fb;

/******************************************************************************
 * FB register
 */
static rt_err_t rt_ls1x_fb_register(struct rt_device *dev, void *pDC, rt_uint32_t flag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(pDC != RT_NULL);

    dev->type        = RT_Device_Class_Graphic;
    dev->rx_indicate = RT_NULL;
    dev->tx_complete = RT_NULL;

    dev->init        = rt_fb_init;
    dev->open        = rt_fb_open;
    dev->close       = rt_fb_close;
    dev->read        = rt_fb_read;
    dev->write       = rt_fb_write;
    dev->control     = rt_fb_control;

    dev->user_data   = pDC;

    /* register a graphic device
     */
    return rt_device_register(dev, LS1x_FB_DEVICE_NAME, flag);
}

//-----------------------------------------------------------------------------
// Register Loongson FB devices
//-----------------------------------------------------------------------------

void rt_ls1x_fb_install(void)
{
    rt_ls1x_fb_register(&rt_ls1x_fb, devDC, RT_DEVICE_FLAG_RDWR);
}

#endif


