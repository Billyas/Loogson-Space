/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _CLOCK_H
#define _CLOCK_H

#define TICKS_PER_SECOND    1000

/*
 * elapsed ticks since boot
 */
unsigned int get_clock_ticks(void);

/*
 * initialise the Clock
 */
void Clock_initialize(void);

void Clock_exit(void);

/*
 * delay with CP0 count/compare
 */
void delay_us(unsigned int us);
void delay_ms(unsigned int ms);

#endif // _CLOCK_H
