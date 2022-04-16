/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * xpt2046.h
 *
 *  Created on: 2014-9-1
 *      Author: Bian
 */

#ifndef _XPT2046_H_
#define _XPT2046_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TC-1B200开发板, 触摸中断信号通过 GPIO54(UART2_RX实现).
 */
#if defined(LS1B)
#if BSP_USE_OS
#define	XPT2046_USE_GPIO_INT	1			/* touchscreen use interrupt */
#endif
#define XPT2046_USE_GPIO_NUM	54 			/* interrupt gpio port */
#endif // #if BSP_USE_OS

#define TOUCHSCREEN_USE_MESSAGE 1
#if (TOUCHSCREEN_USE_MESSAGE)
typedef struct touch_msg
{
	unsigned short x;
	unsigned short y;
	unsigned short z;
} ts_message_t;
#endif

/******************************************************************************
 * SPI0-XPT2046 driver operators
 */

#include "ls1x_io.h"

#if (PACK_DRV_OPS)

extern driver_ops_t *ls1x_xpt2046_drv_ops;

#define ls1x_xpt2046_init(spi, arg)             ls1x_xpt2046_drv_ops->init_entry(spi, arg)
#define ls1x_xpt2046_read(spi, buf, size, arg)  ls1x_xpt2046_drv_ops->read_entry(spi, buf, size, arg)

#else

int XPT2046_initialize(void *bus, void *arg);
int XPT2046_read(void *bus, void *buf, int size, void *arg);

#define ls1x_xpt2046_init(spi, arg)             XPT2046_initialize(spi, arg)
#define ls1x_xpt2046_read(spi, buf, size, arg)  XPT2046_read(spi, buf, size, arg)

#endif

/******************************************************************************
 * user api in "touch_utils.c"
 */
typedef void (*touch_callback_t)(int x, int y);     /* 触摸事件回调函数 */

extern int do_touchscreen_calibrate(void);
extern int start_touchscreen_task(touch_callback_t cb);
extern int stop_touchscreen_task(void);

/*
 * touch screen calibrate file name. XXX 长文件名不能使用
 */
#define TOUCH_CALIBRATE_DIR        "/ndd/config"
#define TOUCH_CALIBRATE_INIFILE    "/ndd/config/touchcal.ini"
#define TOUCH_CALIBRATE_DATFILE    "/ndd/config/touchcal.dat"

#ifdef __cplusplus
}
#endif

#endif /* _XPT2046_H_ */


