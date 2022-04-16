/*
 * led.h
 *
 * created: 2021/4/29
 *  author: 
 */

#ifndef _LED_H
#define _LED_H

#define u8 unsigned char

#define LED1 54
#define LED2 55
#define LED3 2
#define LED4 3

#define ON 0
#define OFF 1

void LED_IO_Config_Init(void);
void LEDx_Set_Status(u8 LEDx, u8 status);
void LED_All_ON(void);
void LED_All_OFF(void);
void LED_Waterfall(void);

#endif // _LED_H

