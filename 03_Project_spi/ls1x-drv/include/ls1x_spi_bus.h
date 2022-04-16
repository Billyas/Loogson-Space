/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * ls1x_spi.h
 *
 * this file contains the ls1x SPI driver declarations
 *
 *  Created on: 2013-11-1
 *      Author: Bian
 */

#ifndef _LS1x_SPI_H
#define _LS1x_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "ls1x_io.h"

#if defined(LS1B)
#include "ls1b.h"
#include "ls1b_irq.h"
#define LS1x_SPI0_BASE      LS1B_SPI0_BASE
#define LS1x_SPI1_BASE      LS1B_SPI1_BASE
#elif defined(LS1C)
#include "ls1c.h"
#include "ls1c_irq.h"
#define LS1x_SPI0_BASE      LS1C_SPI0_BASE
#define LS1x_SPI1_BASE      LS1C_SPI1_BASE
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
 * LS1x SPI Definations
 ******************************************************************************/

typedef struct LS1x_SPI_regs;

/* 
 * SPI bus
 */
typedef struct
{
	struct LS1x_SPI_regs *hwSPI;    /* pointer to HW registers */
	/* hardware parameters */
	unsigned int base_frq;          /* input frq for baud rate divider */
	unsigned int chipsel_nums;      /* total chip select numbers */
	unsigned int chipsel_high;      /* cs high level - XXX: value from tfr-mode */
	unsigned int dummy_char;        /* this character will be continuously transmitted in read only functions */
    /* interrupt support*/
	unsigned int irqNum;            /* interrupt num */
	unsigned int int_ctrlr;         /* interrupt controller */
	unsigned int int_mask;          /* interrupt mask */
	/* mutex */
#if defined(OS_RTTHREAD)
	rt_mutex_t spi_mutex;
#elif defined(OS_UCOS)
	OS_EVENT  *spi_mutex;
#elif defined(OS_FREERTOS)
	SemaphoreHandle_t spi_mutex;
#else // defined(OS_NONE)
    int  spi_mutex;
#endif
    int  initialized;
    char dev_name[16];
#if (PACK_DRV_OPS)
    libspi_ops_t *ops;              /* bus operators */
#endif
} LS1x_SPI_bus_t;

/* 
 * SPI device communication mode 
 */
typedef struct
{
    unsigned int  baudrate;         /* maximum bits per second */
    unsigned char bits_per_char;    /* how many bits per byte/word/longword? */
	bool          lsb_first;        /* true: send LSB first */
	bool          clock_pha;        /* clock phase    - spi mode */
	bool          clock_pol;        /* clock polarity - spi mode */
	bool          clock_inv;        /* true: inverted clock (low active) - cs high or low */
	bool          clock_phs;        /* true: clock starts toggling at start of data tfr - interface mode */
} LS1x_SPI_mode_t;

/******************************************************************************
 * LS1x SPI BUS
 */
#ifdef BSP_USE_SPI0
extern LS1x_SPI_bus_t *busSPI0;
#endif
#ifdef BSP_USE_SPI1
extern LS1x_SPI_bus_t *busSPI1;
#endif

/******************************************************************************
 * bus operators
 */
#if (PACK_DRV_OPS)

#define ls1x_spi_initialize(spi)            spi->ops->init(spi)
#define ls1x_spi_send_start(spi, addr)      spi->ops->send_start(spi, addr)
#define ls1x_spi_send_stop(spi, addr)       spi->ops->send_stop(spi, addr)
#define ls1x_spi_send_addr(spi, addr, rw)   spi->ops->send_addr(spi, addr, rw)
#define ls1x_spi_read_bytes(spi, buf, len)  spi->ops->read_bytes(spi, buf, len)
#define ls1x_spi_write_bytes(spi, buf, len) spi->ops->write_bytes(spi, buf, len)
#define ls1x_spi_ioctl(spi, cmd, arg)       spi->ops->ioctl(spi, cmd, arg)

#else

int LS1x_SPI_initialize(void *bus);
int LS1x_SPI_send_start(void *bus, unsigned int Addr);
int LS1x_SPI_send_stop(void *bus, unsigned int Addr);
int LS1x_SPI_send_addr(void *bus, unsigned int Addr, int rw);
int LS1x_SPI_read_bytes(void *bus, unsigned char *buf, int len);
int LS1x_SPI_write_bytes(void *bus, unsigned char *buf, int len);
int LS1x_SPI_ioctl(void *bus, int cmd, void *arg);

#define ls1x_spi_initialize(spi)            LS1x_SPI_initialize(spi)
#define ls1x_spi_send_start(spi, addr)      LS1x_SPI_send_start(spi, addr)
#define ls1x_spi_send_stop(spi, addr)       LS1x_SPI_send_stop(spi, addr)
#define ls1x_spi_send_addr(spi, addr, rw)   LS1x_SPI_send_addr(spi, addr, rw)
#define ls1x_spi_read_bytes(spi, buf, len)  LS1x_SPI_read_bytes(spi, buf, len)
#define ls1x_spi_write_bytes(spi, buf, len) LS1x_SPI_write_bytes(spi, buf, len)
#define ls1x_spi_ioctl(spi, cmd, arg)       LS1x_SPI_ioctl(spi, cmd, arg)

#endif

/******************************************************************************
 * bus api
 */
int LS1x_enable_spiflash_fastread(LS1x_SPI_bus_t *pSPI);
int LS1x_disable_spiflash_fastread(LS1x_SPI_bus_t *pSPI);

#ifdef __cplusplus
}
#endif

#endif /* _LS1x_I2C_H */
