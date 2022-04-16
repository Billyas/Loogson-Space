/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_rx8010.h
 *
 * created: 2021/1/7
 *  author: Bian
 */

#ifndef _DRV_RX8010_H
#define _DRV_RX8010_H

#ifdef __cplusplus
extern "C" {
#endif

#define RX8010_DEVICE_NAME      "i2c0.rx8010"

/*
 * SPECIAL for LS1X RX8010:
 *
 *   rt_device_read() / rt_device_write() parameter "buf" is as "struct tm *"
 *
 */

void rt_ls1x_rx8010_install(void);

#ifdef __cplusplus
}
#endif

#endif // _DRV_RX8010_H

