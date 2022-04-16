/*
 * touch.c
 *
 * created: 2021/3/15
 *  author: 
 */

#include "bsp.h"

#ifdef GT1151_DRV

#include "ls1x_i2c_bus.h"
#include "touch.h"
#include "i2c/gt1151.h"

// 触摸屏初始化
void TP_Init(void)
{
    ls1x_gt1151_init(busI2C0, NULL);
}

_m_tp_dev tp_dev =
{
    .init = TP_Init,           // 初始化触摸屏控制器
    .scan = GT1151_Scan,	   // 扫描触摸屏
    .x    = { 0 },		       // 当前坐标
    .y    = { 0 },		       // 电容屏有最多5组坐标
    .sta  = 0,
};

#endif
