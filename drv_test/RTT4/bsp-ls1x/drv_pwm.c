/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_pwm.c
 *
 * created: 2021/3/7
 *  author: Bian
 */

#include "bsp.h"

#if defined(BSP_USE_PWM0) || defined(BSP_USE_PWM1) || defined(BSP_USE_PWM2) || defined(BSP_USE_PWM3)

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#include "ls1x_pwm.h"
#include "drv_pwm.h"

//-------------------------------------------------------------------------------------------------
// RTThread device for Loongson 1x
//-------------------------------------------------------------------------------------------------

/*
 * These functions glue PWM device to RTThread.
 */
static rt_err_t rt_pwm_init(struct rt_device *dev)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);

    if (ls1x_pwm_init(dev->user_data, NULL) != 0)
        return RT_ERROR;

    return RT_EOK;
}

static rt_err_t rt_pwm_control(struct rt_device *dev,
                               int               cmd,
                               void             *args)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);
    RT_ASSERT(args != RT_NULL);
    
    pwm_cfg_t *cfg = (pwm_cfg_t *)args;
    
    switch (cmd)
    {
        case IOCTL_PWM_START:
            if ((cfg->mode == PWM_SINGLE_PULSE) || (cfg->mode == PWM_CONTINUE_PULSE))
            {
                if (ls1x_pwm_pulse_start(dev->user_data, (pwm_cfg_t *)args) != 0)
                    return RT_EIO;
            }
            else if ((cfg->mode == PWM_SINGLE_TIMER) || (cfg->mode == PWM_CONTINUE_TIMER))
            {
                if (ls1x_pwm_timer_start(dev->user_data, (pwm_cfg_t *)args) != 0)
                    return RT_EIO;
            }
            else
                return RT_EINVAL;

            break;

        case IOCTL_PWM_STOP:
            if ((cfg->mode == PWM_SINGLE_PULSE) || (cfg->mode == PWM_CONTINUE_PULSE))
            {
                if (ls1x_pwm_pulse_stop(dev->user_data) != 0)
                    return RT_EIO;
            }
            else if ((cfg->mode == PWM_SINGLE_TIMER) || (cfg->mode == PWM_CONTINUE_TIMER))
            {
                if (ls1x_pwm_timer_stop(dev->user_data) != 0)
                    return RT_EIO;
            }
            else
                return RT_EINVAL;

            break;

        default:
            return RT_EINVAL;
    }

    return RT_EOK;
}

/******************************************************************************
 * PWM devices
 */
#ifdef BSP_USE_PWM0
static struct rt_device rt_ls1x_pwm0;
#endif
#ifdef BSP_USE_PWM1
static struct rt_device rt_ls1x_pwm1;
#endif
#ifdef BSP_USE_PWM2
static struct rt_device rt_ls1x_pwm2;
#endif
#ifdef BSP_USE_PWM3
static struct rt_device rt_ls1x_pwm3;
#endif

/******************************************************************************
 * PWM register
 */
static rt_err_t rt_ls1x_pwm_register(struct rt_device *dev, void *pwm, rt_uint32_t flag)
{
    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(pwm != RT_NULL);

    dev->type        = RT_Device_Class_Miscellaneous;  // RT_Device_Class_PWM
    dev->rx_indicate = RT_NULL;
    dev->tx_complete = RT_NULL;

    dev->init        = rt_pwm_init;
    dev->open        = NULL;
    dev->close       = NULL;
    dev->read        = NULL;
    dev->write       = NULL;
    dev->control     = rt_pwm_control;

    dev->user_data   = pwm;

    /* register a can device
     */
    return rt_device_register(dev, ls1x_pwm_get_device_name(pwm), flag);
}

//-----------------------------------------------------------------------------
// Register Loongson PWM devices
//-----------------------------------------------------------------------------

void rt_ls1x_pwm_install(void)
{
#ifdef BSP_USE_PWM0
    rt_ls1x_pwm_register(&rt_ls1x_pwm0, devPWM0, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_PWM1
    rt_ls1x_pwm_register(&rt_ls1x_pwm1, devPWM1, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_PWM2
    rt_ls1x_pwm_register(&rt_ls1x_pwm2, devPWM2, RT_DEVICE_FLAG_RDWR);
#endif
#ifdef BSP_USE_PWM3
    rt_ls1x_pwm_register(&rt_ls1x_pwm3, devPWM3, RT_DEVICE_FLAG_RDWR);
#endif
}

#endif

