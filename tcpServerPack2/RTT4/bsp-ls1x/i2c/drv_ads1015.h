/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_ads1015.h
 *
 * created: 2021/1/7
 *  author: Bian
 */

#ifndef _DRV_ADS1015_H
#define _DRV_ADS1015_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * SPECIAL for LS1X ADS1015:
 *
 *   rt_device_read()
 *   parameter "buf" is as unsigned short *
 *   parameter "pos" ADS1015_CHANNEL_D0: Differential P = AIN0, N = AIN1
 *                   ADS1015_CHANNEL_D1: Differential P = AIN0, N = AIN3
 *                   ADS1015_CHANNEL_D2: Differential P = AIN1, N = AIN3
 *                   ADS1015_CHANNEL_D3: Differential P = AIN2, N = AIN3
 *                   ADS1015_CHANNEL_S0: Single-ended AIN0
 *                   ADS1015_CHANNEL_S1: Single-ended AIN1
 *                   ADS1015_CHANNEL_S2: Single-ended AIN2
 *                   ADS1015_CHANNEL_S3: Single-ended AIN3
 *
 */

#define ADS1015_DEVICE_NAME     "i2c0.ads1015"

void rt_ls1x_ads1015_install(void);

#ifdef __cplusplus
}
#endif

#endif // _DRV_ADS1015_H

