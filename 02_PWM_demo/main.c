/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include "ls1b.h"
#include "mips.h"

//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"
#include "ls1x_pwm.h"
#include "ls1b_gpio.h"

//-------------------------------------------------------------------------------------------------
// 主程序
//-------------------------------------------------------------------------------------------------

int main(void)
{
    printk("\r\nmain() function.\r\n");

    gpio_enable(54,DIR_OUT);
    gpio_enable(3,DIR_OUT);
    gpio_write(54,1);
    gpio_write(3,1);
    gpio_disable(2);
    
    unsigned int hrc = 1, dir = 1;
    pwm_cfg_t cfg;
    cfg.isr = NULL;
    cfg.mode = PWM_CONTINUE_PULSE;
    cfg.cb = NULL;

    /*
     * 裸机主循环
     */
    //当DDR频率为100MHz时，pwm输入的高低脉冲宽度最低为20ns
    //当DDR频率为165MHz时，pwm输入的高低脉冲宽度最低为13ns
    while(1){
        if(dir)
            hrc++;
        else
            hrc--;
        printk("hrc=%d\n",hrc);
        cfg.hi_ns = 5000-hrc*100;
        cfg.lo_ns = hrc*100;
        ls1x_pwm_pulse_start(devPWM2, &cfg);
        delay_ms(20);
        ls1x_pwm_pulse_stop(devPWM2);

        if(hrc == 49)
            dir = 0;
        if(hrc == 1)
            dir = 1;
    }

    return 0;
}



