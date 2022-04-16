/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * ls1b_gpio.h
 *
 *  Created on: 2014-8-31
 *      Author: Bian
 */

#ifndef LS1B_GPIO_H_
#define LS1B_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "ls1b.h"

#define DIR_IN      1
#define DIR_OUT     0

#define GPIO_COUNT  62

/*
 * Enable GPIO, Mode and Direction
 */
static inline void gpio_enable(int ioNum, int dir)
{
    if ((ioNum >= 0) && (ioNum < GPIO_COUNT))
    {
	    int register regIndex = ioNum / 32;
	    int register bitVal   = 1 << (ioNum % 32);

        LS1B_GPIO_CFG(regIndex) |= bitVal;
        if (dir)
            LS1B_GPIO_EN(regIndex) |= bitVal;
        else
            LS1B_GPIO_EN(regIndex) &= ~bitVal;
    }
}

/*
 * Read From GPIO
 */
static inline int gpio_read(int ioNum)
{
    if ((ioNum >= 0) && (ioNum < GPIO_COUNT))
        return ((LS1B_GPIO_IN(ioNum / 32) >> (ioNum % 32)) & 0x1);
    else
        return -1;
}

/*
 * Write To GPIO
 */
static inline void gpio_write(int ioNum, bool val)
{
    if ((ioNum >= 0) && (ioNum < GPIO_COUNT))
    {
        int register regIndex = ioNum / 32;
	    int register bitVal   = 1 << (ioNum % 32);
        if (val)
            LS1B_GPIO_OUT(regIndex) |= bitVal;
        else
            LS1B_GPIO_OUT(regIndex) &= ~bitVal;
    }
}

/*
 * Disable GPIO
 */
static inline void gpio_disable(int ioNum)
{
    if ((ioNum >= 0) && (ioNum < GPIO_COUNT))
        LS1B_GPIO_CFG(ioNum / 32) &= ~(1 << (ioNum % 32));
}

//-------------------------------------------------------------------------------------------------
// in "ls1x_gpio.c"
//-------------------------------------------------------------------------------------------------

#define INT_TRIG_EDGE_UP        0x01        /* 上升沿触发 gpio 中断 */
#define INT_TRIG_EDGE_DOWN      0x02        /* 下降沿触发 gpio 中断 */
#define INT_TRIG_LEVEL_HIGH     0x04        /* 高电平触发 gpio 中断 */
#define INT_TRIG_LEVEL_LOW      0x08        /* 低电平触发 gpio 中断 */

extern int ls1x_enable_gpio_interrupt(int gpio);
extern int ls1x_disable_gpio_interrupt(int gpio);

extern int ls1x_install_gpio_isr(int gpio, int trigger_mode, void (*isr)(int, void *), void *arg);
extern int ls1x_remove_gpio_isr(int gpio);

#ifdef __cplusplus
}
#endif

#endif /* LS1B_GPIO_H_ */

