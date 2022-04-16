/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_uart.c
 *
 * created: 2021/1/5
 * authour: Bian
 */

#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#define UART_BAUDRATE   115200      // default

void rt_ls1x_uart_install(void);

extern const char *NS16550_get_device_name(void *pUART);

#endif  // __DRV_UART_H__



