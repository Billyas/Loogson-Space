/*
 * interrupt.h
 *
 * created: 2021/2/11
 * authour: 
 */

#ifndef _INTERRUPT_H
#define _INTERRUPT_H


            //GPIO_num
#define KEY_UP    0     //默认低电平，按下按键为高电平
#define LED1    54
#define LED2    55
#define LED3    2
#define LED4    3

#define ON  0
#define OFF 1

void Gpio_Interrupt_Init(void);

void Led_All_Off(void);


#endif // _INTERRUPT_H

