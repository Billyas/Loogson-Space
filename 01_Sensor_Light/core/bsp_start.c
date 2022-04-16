/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>

#include "bsp.h"

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

extern void except_common_entry(void);

/*
 * LS1x bsp start
 */
void bsp_start(void)
{
    mips_interrupt_disable();

    /*
     * install exec vec. Data <==> Instruction must use K1 address
     */
	memcpy((void *)K0_TO_K1(T_VEC), except_common_entry, 40);
	memcpy((void *)K0_TO_K1(C_VEC), except_common_entry, 40);
	memcpy((void *)K0_TO_K1(E_VEC), except_common_entry, 40);

    mips_init_isr_table();          /* initialize isr table */
    
    console_init(115200);           /* initialize console */

    Clock_initialize();             /* initialize ticker */

    /*
     * Enable all interrupts, FPU
     */
#if __mips_hard_float
    mips_unmask_interrupt(SR_CU1 | SR_IMASK | SR_IE);
#else
    mips_unmask_interrupt(SR_IMASK | SR_IE);
#endif

    /* goto main function */
    main();
}


