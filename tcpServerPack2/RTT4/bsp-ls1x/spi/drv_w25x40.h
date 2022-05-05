/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_w25x40.h
 *
 * created: 2021/1/6
 *  author: Bian
 */

#ifndef _DRV_W25X40_H
#define _DRV_W25X40_H

#ifdef __cplusplus
extern "C" {
#endif

#define W25X40_DEVICE_NAME      "spi0.w25x40"

void rt_ls1x_w25x40_install(void);

#ifdef __cplusplus
}
#endif

#endif // _DRV_W25X40_H

