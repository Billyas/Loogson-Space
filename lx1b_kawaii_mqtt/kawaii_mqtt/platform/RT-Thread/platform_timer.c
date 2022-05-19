/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-10 22:16:41
 * @LastEditTime : 2020-01-11 01:19:35
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include "platform_timer.h"

static uint32_t platform_uptime_ms(void)
{
#if (RT_TICK_PER_SECOND == 1000)
    return (uint32_t)rt_tick_get();
#else
    rt_tick_t tick = 0u;

    tick = rt_tick_get() * 1000;
    return (uint32_t)((tick + RT_TICK_PER_SECOND - 1) / RT_TICK_PER_SECOND);
#endif
}

void platform_timer_init(platform_timer_t* timer)
{
    timer->time = 0;
}

void platform_timer_cutdown(platform_timer_t* timer, unsigned int timeout)
{
	timer->time = platform_uptime_ms();
    timer->time += timeout;
}

char platform_timer_is_expired(platform_timer_t* timer)
{
	return platform_uptime_ms() > timer->time ? 1 : 0;
}

int platform_timer_remain(platform_timer_t* timer)
{
    uint32_t now;

    now = platform_uptime_ms();
    if (timer->time <= now) {
        return 0;
    }

    return timer->time - now;
}

unsigned long platform_timer_now(void)
{
    return (unsigned long) platform_uptime_ms();
}

void platform_timer_usleep(unsigned long usec)
{
    uint32_t ms = 0;
    if(usec != 0) {
        ms = usec / 1000;
        if (ms == 0) {
            ms = 1;
        }
    }
    rt_thread_mdelay(ms);
}
