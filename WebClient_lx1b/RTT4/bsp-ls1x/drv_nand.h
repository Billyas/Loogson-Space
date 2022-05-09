/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_nand.h
 *
 * created: 2021/1/5
 *  author: Bian
 */

#ifndef _DRV_NAND_H
#define _DRV_NAND_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * SPECIAL for LS1X NAND:
 *
 *   rt_device_read() / rt_device_write() parameter "pos" is as "NAND_PARAM_t *"
 *
 */

void rt_ls1x_nand_install(void);

#ifdef __cplusplus
}
#endif

#endif // _DRV_NAND_H

