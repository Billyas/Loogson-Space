/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * drv_can.h
 *
 * created: 2021/1/4
 * authour: Bian
 */

#ifndef _DRV_CAN_H
#define _DRV_CAN_H

/*
 * SPECIAL for LS1X CAN:
 *
 *   rt_device_read() / rt_device_write() parameter "buffer" is as "CANMsg_t *"
 *
 */

void rt_ls1x_can_install(void);

extern const char *LS1x_CAN_get_device_name(void *pCAN);

#endif // _DRV_CAN_H

