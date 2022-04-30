/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>

#include "rtthread.h"
#include "rthw.h"

#include "cpu.h"
#include "mips.h"

#include "console.h"            /* always include */

//-----------------------------------------------------------------------------

extern void ls1x_hw_initialize(void);

extern void except_common_entry(void);
extern void mips_init_isr_table(void);
extern unsigned int get_memory_size(void);
extern void Clock_initialize(void);

//-----------------------------------------------------------------------------

extern unsigned char __bss_end;

void rt_hw_board_init(void)
{
    unsigned int mem_size;

    /*
     * install exec vec. Data ==> Instruction must use K1 address
     */
	memcpy((void *)K0_TO_K1(T_VEC), except_common_entry, 40);
	memcpy((void *)K0_TO_K1(C_VEC), except_common_entry, 40);
	memcpy((void *)K0_TO_K1(E_VEC), except_common_entry, 40);
	
    /* initialize isr table */
    mips_init_isr_table();
    
#ifdef RT_USING_HEAP
    mem_size = get_memory_size();
    rt_system_heap_init((void *)&__bss_end, (void *)PHYS_TO_K0(mem_size));
#endif

#ifdef RT_USING_SERIAL
    /* init hardware UART device */
    rt_hw_uart_init();
#endif

#ifdef RT_USING_CONSOLE
  #ifdef RT_USING_SERIAL
    /* set console device */
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
  #else
    console_init(115200);
  #endif
#endif

    /* init operating system timer */
    Clock_initialize();

#ifdef RT_USING_FPU
    /* init hardware fpu */
    rt_hw_fpu_init();
#endif

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

    mips_unmask_interrupt(SR_CU0 | SR_IMASK);

}

/*
 * @@ END
 */
