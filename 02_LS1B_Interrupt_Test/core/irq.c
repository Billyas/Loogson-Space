/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include "cpu.h"
#include "mips.h"

#if defined(LS1B)
#include "ls1b.h"
#include "ls1b_irq.h"
#elif defined(LS1C)
#include "ls1c.h"
#include "ls1c_irq.h"
#else
#error "No Loongson1x Soc defined."
#endif

//-----------------------------------------------------------------------------
// 初始化中断 
//-----------------------------------------------------------------------------

/* 中断向量表 */
typedef struct isr_tbl
{
	void (*handler)(int, void *);       // 中断句柄
	unsigned int arg;                   // 参数 
} isr_tbl_t;

static isr_tbl_t isr_table[BSP_INTERRUPT_VECTOR_MAX];

/*
 * 默认中断 
 */
static void mips_default_isr(int vector, void *arg)
{
	unsigned int sr;
	unsigned int cause;

	mips_get_sr(sr);
	mips_get_cause(cause);

	printf("Unhandled isr exception: vector 0x%02x, cause 0x%08X, sr 0x%08X\n",
	        vector, cause, sr);

	while (1)
	    ;
}

/* 
 * 初始化
 */ 
void mips_init_isr_table(void)
{
	unsigned int i;
	
	for (i=0; i<BSP_INTERRUPT_VECTOR_MAX; i++) 
    {
		isr_table[i].handler = mips_default_isr;
		isr_table[i].arg = i;
	}
}

//-----------------------------------------------------------------------------
// 中断分发 
//-----------------------------------------------------------------------------

static void bsp_irq_handler_dispatch(int vector)
{
	if ((vector >= 0) && (vector < BSP_INTERRUPT_VECTOR_MAX))
	{
	    if (isr_table[vector].handler)
        {
            /* 
             * 关中断由中断程序自己处理 
             */
             
            isr_table[vector].handler(vector, (void *)isr_table[vector].arg);
            
            /* 
             * 开中断由中断程序自己处理 
             */
        }
        else
        {
            mips_default_isr(vector, NULL);
        }
	}
	else
	{
		mips_default_isr(vector, NULL);
	}
}

//-----------------------------------------------------------------------------

static void call_vectored_isr(void *, unsigned int, void *);

void c_interrupt_handler(void *p)
{
	unsigned int sr;
	unsigned int cause;

	mips_get_sr(sr);
	mips_get_cause(cause);

	cause &= (sr & SR_IMASK);
	cause >>= CAUSE_IPSHIFT;

	/******************************************************************
	 * XXX use as bsp system tick generator.
	 */
	if (cause & 0x80)			/* count/compare interrupt */
	{
		bsp_irq_handler_dispatch(LS1x_IRQ_CNT);
	}

	if (cause & 0x04)			/* Interrupt controller 0 */
	{
		call_vectored_isr(p, cause, (void *)LS1x_INTC0_BASE);
	}

	if (cause & 0x08)			/* Interrupt controller 1 */
	{
		call_vectored_isr(p, cause, (void *)LS1x_INTC1_BASE);
	}

	if (cause & 0x10)			/* Interrupt controller 2 */
	{
		call_vectored_isr(p, cause, (void *)LS1x_INTC2_BASE);
	}

	if (cause & 0x20)			/* Interrupt controller 3 */
	{
		call_vectored_isr(p, cause, (void *)LS1x_INTC3_BASE);
	}

#if defined(LS1B)

	if (cause & 0x40)			/* Performance counter */
	{
		bsp_irq_handler_dispatch(LS1x_IRQ_PERF);
	}

#elif defined(LS1C)

	if (cause & 0x40)			/* Interrupt controller 4 */
	{
		call_vectored_isr(p, cause, (void *)LS1x_INTC4_BASE);
	}

#endif

	if (cause & 0x02)			/* Soft Interrupt SW[1] */
	{
		bsp_irq_handler_dispatch(LS1x_IRQ_SW1);
	}
	
	if (cause & 0x01)			/* Soft Interrupt SW[0] */
	{
		bsp_irq_handler_dispatch(LS1x_IRQ_SW0);
	}
	
	// mips_set_cause(0);
}

//-----------------------------------------------------------------------------

static void call_vectored_isr(void *p, unsigned int cause, void *ctrlr)
{
    unsigned int src;
    int index;

    /* check request */
    src = LS1x_INTC_ISR((unsigned int)ctrlr);
    index = 0;

    while (src) 
    {
        /* check LSB */
        if (src & 1) 
        {
            /* clear */
            LS1x_INTC_CLR((unsigned int)ctrlr) = (1 << index);
            asm volatile ("sync");
            
            if ((unsigned int)ctrlr == LS1x_INTC0_BASE)
            {
            	bsp_irq_handler_dispatch(LS1x_IRQ0_BASE + index);
            }
            else if ((unsigned int)ctrlr == LS1x_INTC1_BASE)
            {
            	bsp_irq_handler_dispatch(LS1x_IRQ1_BASE + index);
            }
            else if ((unsigned int)ctrlr == LS1x_INTC2_BASE)
            {
            	bsp_irq_handler_dispatch(LS1x_IRQ2_BASE + index);
            }
            else if ((unsigned int)ctrlr == LS1x_INTC3_BASE)
            {
            	bsp_irq_handler_dispatch(LS1x_IRQ3_BASE + index);
            }
    #if defined(LS1C)
            else if ((unsigned int)ctrlr == LS1x_INTC4_BASE)
            {
            	bsp_irq_handler_dispatch(LS1x_IRQ4_BASE + index);
            }
    #endif
        }

        index++;

        /* shift, and make sure MSB is clear */
        src = (src >> 1) & 0x7fffffff;
    }
}

//-----------------------------------------------------------------------------

/* Generate a software interrupt */
int assert_sw_irq(unsigned int irqnum)
{
    unsigned int cause;

    if (irqnum <= 1) 
    {
        mips_get_cause(cause);
        cause = cause | ((irqnum + 1) << CAUSE_IPSHIFT);
        mips_set_cause(cause);

        return irqnum;
    } 

    return -1;
}

/* Clear a software interrupt */
int negate_sw_irq(unsigned int irqnum)
{
	unsigned int cause;

    if (irqnum <= 1)
    {
        mips_get_cause(cause);
        cause = cause & ~((irqnum + 1) << CAUSE_IPSHIFT);
        mips_set_cause(cause);

        return irqnum;
    }

    return -1;
}

//-----------------------------------------------------------------------------

#define LS1X_SR_IPMASK  0x7F00      // IPMASK 0:6 (but counter/compare)

void ls1x_install_irq_handler(int vector, void (*isr)(int, void *), void *arg)
{
    if ((vector >= 0) && (vector < BSP_INTERRUPT_VECTOR_MAX))
    {
        mips_interrupt_disable();
        isr_table[vector].handler = isr;
        isr_table[vector].arg = (unsigned int)arg;
        mips_interrupt_enable();
    }
}

void ls1x_remove_irq_handler(int vector)
{
    if ((vector >= 0) && (vector < BSP_INTERRUPT_VECTOR_MAX))
    {
        mips_interrupt_disable();
        isr_table[vector].handler = mips_default_isr;
        isr_table[vector].arg = vector;
        mips_interrupt_enable();
    }
}


