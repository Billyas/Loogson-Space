/*
 * traffic.c
 *
 * created: 2021/5/21
 *  author:
 */
 #include "Timing.h"
 #include "led.h"
 #include "ls1x_rtc.h"
 #include "ls1x_pwm.h"
 
 extern void *devPWM2;
 
 volatile unsigned char NS_Go_flag = 1;
 volatile unsigned char WE_Go_flag = 0;
 volatile unsigned char Y_flag = 0;


 //用于计时
 volatile unsigned int Second = 0;
 volatile unsigned int Timer = 0,time_cnt = 0;
 
 //定义一个PWM的结构体
 pwm_cfg_t pwm2_cfg;

 //PWM2中断处理函数
 static void pwmtimer_callback(void *pwm, int *stopit)
 {
    time_cnt++;
    Timer++;
 }

 /*************************************************************************************
 //**函数功能：使用PWM2实现约16.777216ms定时
 //**说明：此定时器主要用于切换人行道信号和实现跑马灯信号功能
 //**************************************************************************************/
 //static void PWM2_Time_Init(void)
 //{
    //pwm2_cfg.hi_ns = 16777216;
    //pwm2_cfg.lo_ns = 0;
    //pwm2_cfg.mode = PWM_CONTINUE_TIMER;//脉冲持续产生
    //pwm2_cfg.cb = pwmtimer_callback;
    //pwm2_cfg.isr = NULL; //工作在定时器模式

    //ls1x_pwm_init(devPWM2,NULL);
    //ls1x_pwm_open(devPWM2,(void *)&pwm2_cfg);
    //ls1x_pwm_timer_start(devPWM2,(void *)&pwm2_cfg);
 //}
 
 //RTC中断处理函数
 static void rtctimer_callback(int device, unsigned match, int *stop)
 {
    Second++;
 }

/*************************************************************************************
 **函数功能：使用RTC1秒定时
 **说明：此定时器主要使用于交通灯倒计时。
 **************************************************************************************/
static void Timer1ms(void)
{
    //初始化RTC
    ls1x_rtc_init(NULL,NULL);

    //开启RTC定时中断功能
    rtc_cfg_t rtc;
    rtc.cb = rtctimer_callback;
    rtc.isr = NULL;
    rtc.interval_ms = 1000;
    rtc.trig_datetime = NULL;
    //开启定时器
    ls1x_rtc_timer_start(DEVICE_RTCMATCH0,&rtc);
    
}

/*************************************************************************************
 **函数功能：初始化IO口
 **说明：
 **************************************************************************************/
 void Timing_init(void)
 {
    Timer1ms();         //1秒进一次中断
    LED_init();         //LED灯IO初始化
 }

/*************************************************************************************
 **函数功能：LED灯闪烁
 **说明：
 **************************************************************************************/
 void TimingDis(void)
 {
    
    if(Second < 2)
    {
        LED_1_ON;    //灯亮
        printk("LED_1_ON\n");
    }
    else
    {
        LED_1_OFF;   //灯灭
        printk("LED_1_OFF\n");
    }
    if(Second == 3)
    {
        Second = 1;
        printk("Back\n");
    }

 }
 


