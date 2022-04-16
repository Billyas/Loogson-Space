

#include <stdio.h>

#include "ls1b.h"
#include "mips.h"

//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"
#include "libc/lwmem.h"
#include "ls1x_fb.h"
#include "ls1b_gpio.h"
#include "src/opt3002/opt3002.h"

char LCD_display_mode[] = LCD_480x800;

int main(void)
{
    printk("\r\nmain() function.\r\n");
    
    //��ʼ���ڴ��
    lwmem_initialize(0);
    
    unsigned char sta = 0;
    float temp = 0,hum = 0,press = 0,eleva = 0,lx = 0;
    static char cnt = 0;
    unsigned char Key = 0;
    unsigned char buf[20] = {0};
    //I2C���߳�ʼ��
    I2C1_init();
    //��ȡ���նȴ�����ID
    Get_OPT_ID();
    //���ù��նȴ���������ģʽ
    OPT3002_Conf();
    //����ʾ
    fb_open();
    delay_ms(200);
    fb_textout(50, 30, "IIC���նȴ���������");
    /*
     * �����ѭ��
     */
    for (;;)
    {

            delay_ms(1000);
            fb_fillrect(50, 50, 600, 150, cidxBLACK);
            fb_textout(50, 50, "���մ�����");
            fb_textout(50, 70, "��ǰ����ǿ��: ");
            lx = OPT3002_RD_Data();
            /* ��LCD����ʾ����ǿ�� */
            sprintf((char *)buf,"%.02fmW/CM2", lx);
	        fb_textout(160, 70, buf);
	        memset(buf, 0, sizeof(buf));
            if(lx >= 150000)
            {
                LED9_ON();
                fb_textout(50, 90, "ָʾ��״̬��LED9_ON ");
            }
            else
            {
                LED9_OFF();
                fb_textout(50, 90, "ָʾ��״̬��LED9_OFF");
            }
    }

    return 0;
}



