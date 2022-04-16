/*
 * semaphore_test.h
 *
 * created: 2021/3/18
 *  author: 
 */

#ifndef _SEMAPHORE_TEST_H
#define _SEMAPHORE_TEST_H


#define LED1    54
#define LED2    55
#define LED3    2
#define LED4    3     

//输出低电平灯亮，高电平灯灭
#define ON      0
#define OFF     1

void Test_Gpio_Init(void);
void Led_All_On(void);
void Led_All_Off(void);
void Led_Test(void);

#endif // _SEMAPHORE_TEST_H

