#include <stdio.h>

#include "ls1b.h"
#include "mips.h"


#include "bsp.h"
#include "ls1x_spi_bus.h"
#include "spi/w25x40.h"
#include "ls1x_fb.h"

char LCD_display_mode[] = LCD_480x800;

//-------------------------------------------------------------------------------------------------
// 主程序
//-------------------------------------------------------------------------------------------------

int main(void)
{
    //内存堆初始化
    lwmem_initialize(0);
    //打开显示
    fb_open();
    //SPI0控制器初始化
    ls1x_spi_initialize(busSPI0);
    
    unsigned int id,wroffset = 0x51000,rdoffset = 0x51000;
	char sbuf[20];
	//初始化W25X40芯片
	ls1x_w25x40_init(busSPI0, NULL);
	//获取W25X40芯片ID
	ls1x_w25x40_ioctl(busSPI0, IOCTL_W25X40_READ_ID, &id);
    //printk("W25X40 ID:%x\n\n",id);
    sprintf((char *)sbuf,"W25X40 ID:%x\n",id);
    
    unsigned char rbuf[8],tbuf[8]="中国";
	unsigned char tx_count = 0;
	unsigned char s1[65],s2[65];

/*
 * 备注：W25X40芯片内部写有PMON，占用空间约268K，因此测试W25X40读写功能时，必须在268K空间以外
 * 本例程测试地址为300-512K空间
 * nor flash特性，写数据之前必须要先擦除，且芯片擦除次数是有寿命限制的
 */
    
    /*
     * 裸机主循环
     */
    while(1){
        ls1x_w25x40_ioctl(busSPI0, IOCTL_W25X40_ERASE_4K, wroffset);  //spinor flash写之前都要先擦除
        memset(rbuf, 0, 8);

        //往W25X40芯片写入数据
	    ls1x_w25x40_write(busSPI0, tbuf, 8, &wroffset);

	    ls1x_w25x40_read(busSPI0, rbuf, 8, &rdoffset);

        fb_textout(10, 80, "W25X40 read data");
        fb_fillrect(10, 100, 480, 116, cidxBLACK);
	    fb_textout(10, 100, rbuf);
	    delay_ms(500);
    }

    return 0;
}



