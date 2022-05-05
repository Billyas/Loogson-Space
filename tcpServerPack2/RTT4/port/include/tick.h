/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _TICK_H
#define _TICK_H

void Clock_initialize(void);

void Clock_exit(void);

#define mips_init_ticker Clock_initialize

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);

#endif // _TICK_H
