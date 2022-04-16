/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * w25x40.h
 *
 * this file contains the ls1b SPI Flash driver definations
 *
 *  Created on: 2014-2-26
 *      Author: Bian
 */

#ifndef _W25X40_H
#define _W25X40_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// io control command
//-----------------------------------------------------------------------------

#define IOCTL_W25X40_READ_ID			0x0001
#define IOCTL_W25X40_READ_JDECID		0x0002
#define IOCTL_W25X40_READ_UNIQUEID		0x0004
#define IOCTL_W25X40_ERASE_4K			0x0008		/* sector erase, 4KB */
#define IOCTL_W25X40_ERASE_32K			0x0010		/* block erase, 32KB */
#define IOCTL_W25X40_ERASE_64K			0x0020		/* block erase, 64KB */
#define IOCTL_W25X40_SECTOR_ERASE		0x0040		/* sector erase */
#define IOCTL_W25X40_BULK_ERASE			0x0080		/* chip erase */
#define IOCTL_W25X40_WRITE_PROTECT		0x0100		/* write protect */
#define IOCTL_W25X40_IS_BLANK			0x0200		/* sector empty check */

//-----------------------------------------------------------------------------
// SPI0-W25X40 driver operators
//-----------------------------------------------------------------------------

#include "ls1x_io.h"

#if (PACK_DRV_OPS)

extern driver_ops_t *ls1x_w25x40_drv_ops;

#define ls1x_w25x40_init(spi, arg)             ls1x_w25x40_drv_ops->init_entry(spi, arg)
#define ls1x_w25x40_open(spi, arg)             ls1x_w25x40_drv_ops->open_entry(spi, arg)
#define ls1x_w25x40_close(spi, arg)            ls1x_w25x40_drv_ops->close_entry(spi, arg)
#define ls1x_w25x40_read(spi, buf, size, arg)  ls1x_w25x40_drv_ops->read_entry(spi, buf, size, arg)
#define ls1x_w25x40_write(spi, buf, size, arg) ls1x_w25x40_drv_ops->write_entry(spi, buf, size, arg)
#define ls1x_w25x40_ioctl(spi, cmd, arg)       ls1x_w25x40_drv_ops->ioctl_entry(spi, cmd, arg)

#else

int W25X40_init(void *bus, void *arg);
int W25X40_open(void *bus, void *arg);
int W25X40_close(void *bus, void *arg);
int W25X40_read(void *bus, void *buf, int size, void *arg);
int W25X40_write(void *bus, void *buf, int size, void *arg);
int W25X40_ioctl(void *bus, int cmd, void *arg);

#define ls1x_w25x40_init(spi, arg)             W25X40_init(spi, arg)
#define ls1x_w25x40_open(spi, arg)             W25X40_open(spi, arg)
#define ls1x_w25x40_close(spi, arg)            W25X40_close(spi, arg)
#define ls1x_w25x40_read(spi, buf, size, arg)  W25X40_read(spi, buf, size, arg)
#define ls1x_w25x40_write(spi, buf, size, arg) W25X40_write(spi, buf, size, arg)
#define ls1x_w25x40_ioctl(spi, cmd, arg)       W25X40_ioctl(spi, cmd, arg)

#endif

//-----------------------------------------------------------------------------
// user api
//-----------------------------------------------------------------------------

int save_touchscreen_calibrate_values_to_w25x40(int *calibrate_coords, int len);
int load_touchscreen_calibrate_values_from_w25x40(int *calibrate_coords, int len);

#ifdef __cplusplus
}
#endif

#endif /* _W25X40_H */


