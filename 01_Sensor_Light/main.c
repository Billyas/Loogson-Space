

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
    
    //初始化内存堆
    lwmem_initialize(0);
    
    unsigned char sta = 0;
    float temp = 0,hum = 0,press = 0,eleva = 0,lx = 0;
    static char cnt = 0;
    unsigned char Key = 0;
    unsigned char buf[20] = {0};
    //I2C总线初始化
    I2C1_init();
    //获取光照度传感器ID
    Get_OPT_ID();
    //配置光照度传感器工作模式
    OPT3002_Conf();
    //打开显示
    fb_open();
    delay_ms(200);
    fb_textout(50, 30, "IIC光照度传感器测量");
    /*
     * 裸机主循环
     */
    for (;;)
    {

            delay_ms(1000);
            fb_fillrect(50, 50, 600, 150, cidxBLACK);
            fb_textout(50, 50, "光照传感器");
            fb_textout(50, 70, "当前光照强度: ");
            lx = OPT3002_RD_Data();
            /* 在LCD上显示光照强度 */
            sprintf((char *)buf,"%.02fmW/CM2", lx);
	        fb_textout(160, 70, buf);
	        memset(buf, 0, sizeof(buf));
            if(lx >= 150000)
            {
                LED9_ON();
                fb_textout(50, 90, "指示灯状态：LED9_ON ");
            }
            else
            {
                LED9_OFF();
                fb_textout(50, 90, "指示灯状态：LED9_OFF");
            }
    }

    return 0;
}



