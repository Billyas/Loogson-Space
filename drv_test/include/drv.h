#ifndef _DRV_CAN_H
#define _DRV_CAN_H

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

rt_err_t rt_hw_led_register(rt_device_t device, const char* name, rt_uint32_t flag, void *user_data);
void test_driver(void);
#endif