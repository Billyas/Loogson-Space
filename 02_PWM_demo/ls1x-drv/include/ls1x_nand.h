/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * ls1x_nanddrv.h
 *
 *  Created on: 2013-11-21
 *      Author: Bian
 */

#ifndef _LS1x_NANDDRV_H_
#define _LS1x_NANDDRV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

/*
 * NAND operate defination
 */
enum
{
	NAND_OP_MAIN  = 0x0001,
	NAND_OP_SPARE = 0x0002,
};

typedef struct
{
	unsigned int   pageNum;			// physcal page number
	unsigned int   colAddr;			// address in page
	unsigned int   opFlags;         // NAND_OP_MAIN / NAND_OP_SPARE
} NAND_PARAM_t;

/*
 * NAND K9F1G08 chip info
 */
#if (!defined(BYTES_OF_PAGE))
#define BYTES_OF_PAGE			0x800							// 2048 bytes/page
#define PAGES_OF_BLOCK			0x40							// 64   pages/block
#define BLOCKS_OF_CHIP			0x400							// 1024 blocks/chip
#define OOBBYTES_OF_PAGE		0x40							// 64   oobbytes/page
#define PAGES_OF_CHIP			(PAGES_OF_BLOCK*BLOCKS_OF_CHIP) //		pages/chip
#define BYTES_OF_CHIP			0x08000000						// 128M bytes/page
#endif

//-----------------------------------------------------------------------------
// NAND device
//-----------------------------------------------------------------------------

#ifdef BSP_USE_NAND
extern void *devNAND;  // LS1x_NAND_dev_t * 
#endif

//-----------------------------------------------------------------------------
// NAND io control command						param type
//-----------------------------------------------------------------------------

#define IOCTL_NAND_RESET			0x1000	    // none
#define IOCTL_NAND_GET_ID			0x2000	    // unsigned int *
#define IOCTL_NAND_GET_STATS		0x4000	    // XXX

#define IOCTL_NAND_ERASE_BLOCK      0x0100	    // unsigned int	  - block number
#define IOCTL_NAND_ERASE_CHIP		0x0200	    // none
#define IOCTL_NAND_PAGE_BLANK		0x0400	    // unsigned int   - page number
#define IOCTL_NAND_PAGE_VERIFY      0x0800	    // NAND_PARAM_t *

#define IOCTL_NAND_MARK_BAD_BLOCK   0x0010	    // unsigned int   - block number
#define IOCTL_NAND_IS_BAD_BLOCK     0x0020	    // unsigned int	  - block number

//-----------------------------------------------------------------------------
// NAND driver operators
//-----------------------------------------------------------------------------

#include "ls1x_io.h"

#if (PACK_DRV_OPS)

extern driver_ops_t *ls1x_nand_drv_ops;

#define ls1x_nand_init(nand, arg)             ls1x_nand_drv_ops->init_entry(nand, arg)
#define ls1x_nand_open(nand, arg)             ls1x_nand_drv_ops->open_entry(nand, arg)
#define ls1x_nand_close(nand, arg)            ls1x_nand_drv_ops->close_entry(nand, arg)
#define ls1x_nand_read(nand, buf, size, arg)  ls1x_nand_drv_ops->read_entry(nand, buf, size, arg)
#define ls1x_nand_write(nand, buf, size, arg) ls1x_nand_drv_ops->write_entry(nand, buf, size, arg)
#define ls1x_nand_ioctl(nand, cmd, arg)       ls1x_nand_drv_ops->ioctl_entry(nand, cmd, arg)

#else

int LS1x_NAND_initialize(void *dev, void *arg);
int LS1x_NAND_open(void *dev, void *arg);
int LS1x_NAND_close(void *dev, void *arg);
int LS1x_NAND_read(void *dev, void *buf, int size, void *arg);
int LS1x_NAND_write(void *dev, void *buf, int size, void *arg);
int LS1x_NAND_ioctl(void *dev, int cmd, void *arg);

#define ls1x_nand_init(nand, arg)             LS1x_NAND_initialize(nand, arg)
#define ls1x_nand_open(nand, arg)             LS1x_NAND_open(nand, arg)
#define ls1x_nand_close(nand, arg)            LS1x_NAND_close(nand, arg)
#define ls1x_nand_read(nand, buf, size, arg)  LS1x_NAND_read(nand, buf, size, arg)
#define ls1x_nand_write(nand, buf, size, arg) LS1x_NAND_write(nand, buf, size, arg)
#define ls1x_nand_ioctl(nand, cmd, arg)       LS1x_NAND_ioctl(nand, cmd, arg)

#endif

/*
 * for RT-Thread
 */
#if defined(OS_RTTHREAD)
#define LS1x_NAND_DEVICE_NAME   "nand0"
#endif


#ifdef __cplusplus
}
#endif

#endif /* LS1x_NANDDRV_H_ */

