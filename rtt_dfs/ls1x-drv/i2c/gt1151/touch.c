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

// ��������ʼ��
void TP_Init(void)
{
    ls1x_gt1151_init(busI2C0, NULL);
}

_m_tp_dev tp_dev =
{
    .init = TP_Init,           // ��ʼ��������������
    .scan = GT1151_Scan,	   // ɨ�败����
    .x    = { 0 },		       // ��ǰ����
    .y    = { 0 },		       // �����������5������
    .sta  = 0,
};

#endif
