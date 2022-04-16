/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_mcp4725.h
 *
 * created: 2021/1/7
 * authour: Bian
 */

#ifndef _DRV_MCP4725_H
#define _DRV_MCP4725_H

#define MCP4725_DEVICE_NAME     "i2c0.mcp4725"

/*
 * SPECIAL for LS1X MCP4725:
 *
 *   rt_device_write() parameter "buf" is as unsigned short *
 *
 */

void rt_ls1x_mcp4725_install(void);

#endif // _DRV_MCP4725_H

