/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include "rtthread.h"
#include "rthw.h"

#if defined(LS1B)
#include "ls1b.h"
#include "ls1b_irq.h"
#elif defined(LS1C)
#include "ls1c.h"
#include "ls1c_irq.h"
#else
#error "No Loongson1x Soc defined."
#endif

#include "cpu.h"
#include "mips.h"

#include "tick.h"

/*
 * CPU_XTAL_FREQUENCY
 */
#include "bsp.h"

#ifdef CPU_OSC_FREQ
#undef  CPU_OSC_FREQ
#endif

#define CPU_OSC_FREQ    CPU_XTAL_FREQUENCY

/*
 * In "mips_timer.S"
 */
extern void mips_set_timer(unsigned int timer_clock_interval);
extern unsigned int mips_get_timer(void);

/*
 * CPU 中断, 寄存器 sr 和 cause 的 bit(15), IP7 & IM7
 */
#define CLOCK_INT_MASK       0x8000
#define CLOCK_VECTOR        (MIPS_INTERRUPT_BASE+0x07)  /* LS1x_IRQ_CNT */

static unsigned int mips_timer_step = 0;         /* tick 每次累加计数的 */

static volatile unsigned int Clock_driver_ticks; /* Clock ticks since initialization */

//-----------------------------------------------------------------------------

unsigned int get_clock_ticks(void)
{
    return Clock_driver_ticks;
}

/*
 *  Clock_isr
 *
 *  This is the clock tick interrupt handler.
 */
static void Clock_isr(int vector, void *param)
{
	++Clock_driver_ticks;

    mips_set_timer(mips_timer_step);

	/* increase a RT-Thread tick  
     */
    rt_tick_increase();
}

/*
 * Clock_initialize
 */
void Clock_initialize(void)
{
	Clock_driver_ticks = 0;

	mips_mask_interrupt(CLOCK_INT_MASK);

    /*
     * divided by 2: LS1x-counter plus 1 by two instructions. 
     */
    mips_timer_step  = LS1x_CPU_FREQUENCY(CPU_OSC_FREQ) / 2 / RT_TICK_PER_SECOND;

    mips_set_timer(mips_timer_step * 25);
    
    rt_kprintf("\r\nClock_mask: %x, step=%i\n", CLOCK_INT_MASK, mips_timer_step); 

    /* install then Clock isr */
    rt_hw_interrupt_install(CLOCK_VECTOR, Clock_isr, NULL, "Tick");

    mips_unmask_interrupt(CLOCK_INT_MASK);  
    mips_enable_dc();         
}

void Clock_exit(void)
{
    mips_disable_dc();
    mips_mask_interrupt(CLOCK_INT_MASK);
}

/****************************************************************************** 
 * 延时程序 
 */
void delay_us(unsigned int us)
{
	register unsigned int startVal;
	register unsigned int endVal;
	register unsigned int curVal;
	asm volatile ("mfc0 %0, $9;" : "=&r"(startVal));
	endVal = startVal + us * (mips_timer_step / 1000);
	while (1) 
    {
		asm volatile ("mfc0 %0, $9;" : "=&r"(curVal));
		if (((endVal > startVal) && (curVal >= endVal)) ||
		    ((endVal < startVal) && (curVal < startVal) && (curVal >= endVal)))
			break;
	}
}

/*
 * 延时 1ms
 *
 * TODO 换算 TICK--->SECOND, 目前假设 tick==1ms
 */
void delay_ms(unsigned int ms)
{
    if (rt_tick_get() > 1000)
        rt_thread_delay(ms);
    else
	    delay_us(ms * 1000);
}


