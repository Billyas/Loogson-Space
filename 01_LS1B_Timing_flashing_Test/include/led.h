/*
 * rgb.h
 *
 * created: 2021/5/21
 *  author:
 */

#ifndef _LED_H
#define _LED_H

#include "ls1b_gpio.h"

#define LED_1 14



#define LED_ON  1
#define LED_OFF 0

//���µ���
#define LED_1_ON gpio_write(LED_1,LED_ON)
//���µ���
#define LED_1_OFF gpio_write(LED_1,LED_OFF)


void LED_init(void);
void LED_ALL_ON(void);
void LED_ALL_OFF(void);


#endif 


