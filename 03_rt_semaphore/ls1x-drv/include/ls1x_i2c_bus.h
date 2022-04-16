/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * ls1x_i2cdrv.h
 *
 * this file contains the ls1x I2C driver declarations
 *
 *  Created on: 2013-11-1
 *      Author: Bian
 */

#ifndef _LS1x_I2C_H
#define _LS1x_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "ls1x_io.h"

#if defined(LS1B)
#include "ls1b.h"
#include "ls1b_irq.h"
#define LS1x_I2C0_BASE  LS1B_I2C0_BASE
#define LS1x_I2C1_BASE  LS1B_I2C1_BASE
#define LS1x_I2C2_BASE  LS1B_I2C2_BASE
#elif defined(LS1C)
#include "ls1c.h"
#include "ls1c_irq.h"
#define LS1x_I2C0_BASE  LS1C_I2C0_BASE
#define LS1x_I2C1_BASE  LS1C_I2C1_BASE
#define LS1x_I2C2_BASE  LS1C_I2C2_BASE
#else
#error "No Loongson1x SoC defined."
#endif

#if defined(OS_RTTHREAD)
#include "rtthread.h"
#elif defined(OS_UCOS)
#include "os.h"
#elif defined(OS_FREERTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#include "bsp.h"

/******************************************************************************
 * LS1x I2C Definations
 ******************************************************************************/

typedef struct LS1x_I2C_regs;

typedef struct
{
	struct LS1x_I2C_regs *hwI2C;    /* pointer to HW registers */
	unsigned int base_frq;    		/* input frq for baud rate divider */
	unsigned int baudrate;          /* work baud rate */
	unsigned int dummy_char;        /* dummy char */
    /* interrupt support*/
	unsigned int irqNum;            /* interrupt num */
	unsigned int int_ctrlr;         /* interrupt controller */
	unsigned int int_mask;          /* interrupt mask */
	/* mutex */
#if defined(OS_RTTHREAD)
	rt_mutex_t i2c_mutex;
#elif defined(OS_UCOS)
	OS_EVENT  *i2c_mutex;
#elif defined(OS_FREERTOS)
	SemaphoreHandle_t i2c_mutex;
#else // defined(OS_NONE)
    int  i2c_mutex;
#endif

    int  initialized;
    char dev_name[16];
#if (PACK_DRV_OPS)
    libi2c_ops_t *ops;              /* bus operators */
#endif
} LS1x_I2C_bus_t;

/******************************************************************************
 * LS1x I2C BUS
 */
#ifdef BSP_USE_I2C0
extern LS1x_I2C_bus_t *busI2C0;
#endif
#ifdef BSP_USE_I2C1
extern LS1x_I2C_bus_t *busI2C1;
#endif
#ifdef BSP_USE_I2C2
extern LS1x_I2C_bus_t *busI2C2;
#endif

/******************************************************************************
 * bus operators
 */
#if (PACK_DRV_OPS)

#define ls1x_i2c_initialize(i2c)            i2c->ops->init(i2c)
#define ls1x_i2c_send_start(i2c, addr)      i2c->ops->send_start(i2c, addr)
#define ls1x_i2c_send_stop(i2c, addr)       i2c->ops->send_stop(i2c, addr)
#define ls1x_i2c_send_addr(i2c, addr, rw)   i2c->ops->send_addr(i2c, addr, rw)
#define ls1x_i2c_read_bytes(i2c, buf, len)  i2c->ops->read_bytes(i2c, buf, len)
#define ls1x_i2c_write_bytes(i2c, buf, len) i2c->ops->write_bytes(i2c, buf, len)
#define ls1x_i2c_ioctl(i2c, cmd, arg)       i2c->ops->ioctl(i2c, cmd, arg)

#else

int LS1x_I2C_initialize(void *bus);
int LS1x_I2C_send_start(void *bus, unsigned int Addr);
int LS1x_I2C_send_stop(void *bus, unsigned int Addr);
int LS1x_I2C_send_addr(void *bus, unsigned int Addr, int rw);
int LS1x_I2C_read_bytes(void *bus, unsigned char *buf, int len);
int LS1x_I2C_write_bytes(void *bus, unsigned char *buf, int len);
int LS1x_I2C_ioctl(void *bus, int cmd, void *arg);

#define ls1x_i2c_initialize(i2c)            LS1x_I2C_initialize(i2c)
#define ls1x_i2c_send_start(i2c, addr)      LS1x_I2C_send_start(i2c, addr)
#define ls1x_i2c_send_stop(i2c, addr)       LS1x_I2C_send_stop(i2c, addr)
#define ls1x_i2c_send_addr(i2c, addr, rw)   LS1x_I2C_send_addr(i2c, addr, rw)
#define ls1x_i2c_read_bytes(i2c, buf, len)  LS1x_I2C_read_bytes(i2c, buf, len)
#define ls1x_i2c_write_bytes(i2c, buf, len) LS1x_I2C_write_bytes(i2c, buf, len)
#define ls1x_i2c_ioctl(i2c, cmd, arg)       LS1x_I2C_ioctl(i2c, cmd, arg)

#endif

#ifdef __cplusplus
}
#endif

#endif /* _LS1x_I2C_H */

