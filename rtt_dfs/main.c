/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Loongson 1B RT-Thread Application
 */

#include <time.h>

#include "rtthread.h"

//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"


#ifdef BSP_USE_NAND
    ls1x_nand_init(devNAND);
#endif
#ifdef USE_YAFFS2
yaffs_startup_and_mount(RYFS_MOUNTED_FS_NAME);
#endif
//---------------------------   ----------------------------------------------------------------------
// Simple demo of task
//-------------------------------------------------------------------------------------------------

static rt_thread_t m_demo_thread = NULL;

static void demo_thread(void *arg)
{
	unsigned int tickcount;

    for ( ; ; )
    {
        tickcount = rt_tick_get();

        rt_kprintf("tick count = %i\r\n", tickcount);

        rt_thread_delay(500);
    }
}

//-------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	rt_kprintf("\r\nWelcome to RT-Thread.\r\n\r\n");

    ls1x_drv_init();            /* Initialize device drivers */

    rt_ls1x_drv_init();         /* Initialize device drivers for RTT */

    install_3th_libraries(); 	/* Install 3th libraies */

    //dfs_mkfs("elm","nand0");
    //dfs_mount("nand0", "/", "elm", 0, 0);
	/*
	 * Finsh as another thread...
	 */
    return 0;
}

/*
 * @@ End
 */
