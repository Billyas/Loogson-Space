/*
 * mailbox.h
 *
 * created: 2021/3/26
 *  author: 
 */

#ifndef _MAILBOX_H
#define _MAILBOX_H


#include "ls1b_gpio.h"
#include "ls1x_can.h"
#include "ls1x_fb.h"
#include "ls1x_i2c_bus.h"
#include "i2c/ads1015.h"
#include "i2c/mcp4725.h"


#define KEY1    1
#define KEY2    40
#define KEY3    41

void Key_Init(void);
void ADC_DAC_Init(void);
void ADC_DAC_Test(void);

#endif // _MAILBOX_H

