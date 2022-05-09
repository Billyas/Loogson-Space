/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_pca9557.h
 *
 * created: 2021/1/7
 *  author: Bian
 */

#ifndef _DRV_PCA9557_H
#define _DRV_PCA9557_H

#ifdef __cplusplus
extern "C" {
#endif

#define PCA9557_DEVICE_NAME     "i2c0.pca9557"

/*
 * SPECIAL for LS1X PCA9557:
 *
 *   rt_device_read() / rt_device_write() parameter "buf" is 1 byte as unsigned char *
 *
 */
 
void rt_ls1x_pca9557_install(void);

#ifdef __cplusplus
}
#endif

#endif // _DRV_PCA9557_H

