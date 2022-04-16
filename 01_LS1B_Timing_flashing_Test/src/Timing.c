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


 //���ڼ�ʱ
 volatile unsigned int Second = 0;
 volatile unsigned int Timer = 0,time_cnt = 0;
 
 //����һ��PWM�Ľṹ��
 pwm_cfg_t pwm2_cfg;

 //PWM2�жϴ�����
 static void pwmtimer_callback(void *pwm, int *stopit)
 {
    time_cnt++;
    Timer++;
 }

 /*************************************************************************************
 //**�������ܣ�ʹ��PWM2ʵ��Լ16.777216ms��ʱ
 //**˵�����˶�ʱ����Ҫ�����л����е��źź�ʵ��������źŹ���
 //**************************************************************************************/
 //static void PWM2_Time_Init(void)
 //{
    //pwm2_cfg.hi_ns = 16777216;
    //pwm2_cfg.lo_ns = 0;
    //pwm2_cfg.mode = PWM_CONTINUE_TIMER;//�����������
    //pwm2_cfg.cb = pwmtimer_callback;
    //pwm2_cfg.isr = NULL; //�����ڶ�ʱ��ģʽ

    //ls1x_pwm_init(devPWM2,NULL);
    //ls1x_pwm_open(devPWM2,(void *)&pwm2_cfg);
    //ls1x_pwm_timer_start(devPWM2,(void *)&pwm2_cfg);
 //}
 
 //RTC�жϴ�����
 static void rtctimer_callback(int device, unsigned match, int *stop)
 {
    Second++;
 }

/*************************************************************************************
 **�������ܣ�ʹ��RTC1�붨ʱ
 **˵�����˶�ʱ����Ҫʹ���ڽ�ͨ�Ƶ���ʱ��
 **************************************************************************************/
static void Timer1ms(void)
{
    //��ʼ��RTC
    ls1x_rtc_init(NULL,NULL);

    //����RTC��ʱ�жϹ���
    rtc_cfg_t rtc;
    rtc.cb = rtctimer_callback;
    rtc.isr = NULL;
    rtc.interval_ms = 1000;
    rtc.trig_datetime = NULL;
    //������ʱ��
    ls1x_rtc_timer_start(DEVICE_RTCMATCH0,&rtc);
    
}

/*************************************************************************************
 **�������ܣ���ʼ��IO��
 **˵����
 **************************************************************************************/
 void Timing_init(void)
 {
    Timer1ms();         //1���һ���ж�
    LED_init();         //LED��IO��ʼ��
 }

/*************************************************************************************
 **�������ܣ�LED����˸
 **˵����
 **************************************************************************************/
 void TimingDis(void)
 {
    
    if(Second < 2)
    {
        LED_1_ON;    //����
        printk("LED_1_ON\n");
    }
    else
    {
        LED_1_OFF;   //����
        printk("LED_1_OFF\n");
    }
    if(Second == 3)
    {
        Second = 1;
        printk("Back\n");
    }

 }
 


