/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_xpt2046.h
 *
 * created: 2021/1/6
 *  author: Bian
 */

#ifndef _DRV_XPT2046_H
#define _DRV_XPT2046_H

#ifdef __cplusplus
extern "C" {
#endif

#define XPT2046_DEVICE_NAME     "spi0.xpt2046"

/*
 * SPECIAL for LS1X XPT2046:
 *
 *   rt_device_read() parameter "buffer" is as "ts_message_t *"
 *
 */

void rt_ls1x_xpt2046_install(void);

#ifdef __cplusplus
}
#endif

#endif // _DRV_XPT2046_H

