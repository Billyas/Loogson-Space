/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_uart.c
 *
 * created: 2021/1/5
 *  author: Bian
 */

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

#include "ns16550.h"

#include "drv_uart.h"

//-------------------------------------------------------------------------------------------------
// RTThread device for Loongson 1x
//-------------------------------------------------------------------------------------------------

/*
 * These functions glue UART device to RTThread.
 */
static rt_err_t rt_uart_init(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_uart_init(dev->user_data, NULL) != 0)
        return RT_ERROR;

    return RT_EOK;
}

static rt_err_t rt_uart_open(struct rt_device *dev, rt_uint16_t oflag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_uart_open(dev->user_data, NULL) != 0)              // arg: struct termios *
        return RT_ERROR;

    dev->open_flag = (oflag | RT_DEVICE_FLAG_STREAM) & 0xff;    /* set open flags */

    return RT_EOK;
}

static rt_err_t rt_uart_close(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);
    
    ls1x_uart_close(dev->user_data, NULL);
    
    return RT_EOK;
}

static rt_size_t rt_uart_read(struct rt_device *dev,
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
    return ls1x_uart_read(dev->user_data, buffer, (int)size, 0);
}

static rt_size_t rt_uart_write(struct rt_device *dev,
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
    return ls1x_uart_write(dev->user_data, (void *)buffer, (int)size, 0);
}

static rt_err_t rt_uart_control(struct rt_device *dev,
                                int               cmd,
                                void             *args)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    ls1x_uart_ioctl(dev->user_data, (unsigned)cmd, args);
 
    return RT_EOK;
}

/******************************************************************************
 * uart devices
 */
#if defined(LS1B)

#ifdef BSP_USE_UART2
static struct rt_device rt_ls1b_uart2;
#endif
#ifdef BSP_USE_UART3
static struct rt_device rt_ls1b_uart3;
#endif
#ifdef BSP_USE_UART4
static struct rt_device rt_ls1b_uart4;
#endif
#ifdef BSP_USE_UART5
static struct rt_device rt_ls1b_uart5;
#endif
#ifdef BSP_USE_UART0
static struct rt_device rt_ls1b_uart0;
#endif
#ifdef BSP_USE_UART01
static struct rt_device rt_ls1b_uart01;
#endif
#ifdef BSP_USE_UART02
static struct rt_device rt_ls1b_uart02;
#endif
#ifdef BSP_USE_UART03
static struct rt_device rt_ls1b_uart03;
#endif
#ifdef BSP_USE_UART1
static struct rt_device rt_ls1b_uart1;
#endif
#ifdef BSP_USE_UART11
static struct rt_device rt_ls1b_uart11;
#endif
#ifdef BSP_USE_UART12
static struct rt_device rt_ls1b_uart12;
#endif
#ifdef BSP_USE_UART13
static struct rt_device rt_ls1b_uart13;
#endif

#elif defined(LS1C)

#ifdef BSP_USE_UART0
static struct rt_device rt_ls1c_uart0;
#endif
#ifdef BSP_USE_UART1
static struct rt_device rt_ls1c_uart1;
#endif
#ifdef BSP_USE_UART2
static struct rt_device rt_ls1c_uart2;
#endif
#ifdef BSP_USE_UART3
static struct rt_device rt_ls1c_uart3;
#endif
#ifdef BSP_USE_UART4
static struct rt_device rt_ls1c_uart4;
#endif
#ifdef BSP_USE_UART5
static struct rt_device rt_ls1c_uart5;
#endif
#ifdef BSP_USE_UART6
static struct rt_device rt_ls1c_uart6;
#endif
#ifdef BSP_USE_UART7
static struct rt_device rt_ls1c_uart7;
#endif
#ifdef BSP_USE_UART8
static struct rt_device rt_ls1c_uart8;
#endif
#ifdef BSP_USE_UART9
static struct rt_device rt_ls1c_uart9;
#endif
#ifdef BSP_USE_UART10
static struct rt_device rt_ls1c_uart10;
#endif
#ifdef BSP_USE_UART11
static struct rt_device rt_ls1c_uart11;
#endif

#endif

/******************************************************************************
 * uart register
 */
static rt_err_t rt_ls1x_uart_register(struct rt_device *dev, void *pUART, rt_uint32_t flag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(pUART != RT_NULL);

    if (pUART == ConsolePort)
        return 0;
    
    dev->type        = RT_Device_Class_Char;
    dev->rx_indicate = RT_NULL;
    dev->tx_complete = RT_NULL;

    dev->init        = rt_uart_init;
    dev->open        = rt_uart_open;
    dev->close       = rt_uart_close;
    dev->read        = rt_uart_read;
    dev->write       = rt_uart_write;
    dev->control     = rt_uart_control;
    
    dev->user_data   = pUART;

    /* register a character device
     */
    return rt_device_register(dev, ls1x_uart_get_device_name(pUART), flag);
}

//-----------------------------------------------------------------------------
// Register Loongson uart devices
//-----------------------------------------------------------------------------

void rt_ls1x_uart_install(void)
{
#if defined(LS1B)

#ifdef BSP_USE_UART2
    rt_ls1x_uart_register(&rt_ls1b_uart2, devUART2, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART3
    rt_ls1x_uart_register(&rt_ls1b_uart3, devUART3, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART4
    rt_ls1x_uart_register(&rt_ls1b_uart4, devUART4, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART5
    rt_ls1x_uart_register(&rt_ls1b_uart5, devUART5, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART0
    rt_ls1x_uart_register(&rt_ls1b_uart0, devUART0, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART01
    rt_ls1x_uart_register(&rt_ls1b_uart01, devUART01, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART02
    rt_ls1x_uart_register(&rt_ls1b_uart02, devUART02, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART03
    rt_ls1x_uart_register(&rt_ls1b_uart03, devUART03, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART1
    rt_ls1x_uart_register(&rt_ls1b_uart1, devUART1, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART11
    rt_ls1x_uart_register(&rt_ls1b_uart11, devUART11, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART12
    rt_ls1x_uart_register(&rt_ls1b_uart12, devUART12, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART13
    rt_ls1x_uart_register(&rt_ls1b_uart13, devUART13, RT_DEVICE_FLAG_RDWR);
#endif

#elif defined(LS1C)

#ifdef BSP_USE_UART0
    rt_ls1x_uart_register(&rt_ls1c_uart0, devUART0, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART1
    rt_ls1x_uart_register(&rt_ls1c_uart1, devUART1, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART2
    rt_ls1x_uart_register(&rt_ls1c_uart2, devUART2, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART3
    rt_ls1x_uart_register(&rt_ls1c_uart3, devUART3, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART4
    rt_ls1x_uart_register(&rt_ls1c_uart4, devUART4, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART5
    rt_ls1x_uart_register(&rt_ls1c_uart5, devUART5, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART6
    rt_ls1x_uart_register(&rt_ls1c_uart6, devUART6, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART7
    rt_ls1x_uart_register(&rt_ls1c_uart7, devUART7, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART8
    rt_ls1x_uart_register(&rt_ls1c_uart8, devUART8, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART9
    rt_ls1x_uart_register(&rt_ls1c_uart9, devUART9, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART10
    rt_ls1x_uart_register(&rt_ls1c_uart10, devUART10, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_UART11
    rt_ls1x_uart_register(&rt_ls1c_uart11, devUART11, RT_DEVICE_FLAG_RDWR);
#endif

#endif
}



