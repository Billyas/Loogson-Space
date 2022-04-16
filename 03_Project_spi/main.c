#include <stdio.h>

#include "ls1b.h"
#include "mips.h"


#include "bsp.h"
#include "ls1x_spi_bus.h"
#include "spi/w25x40.h"
#include "ls1x_fb.h"

char LCD_display_mode[] = LCD_480x800;

//-------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------

int main(void)
{
    //�ڴ�ѳ�ʼ��
    lwmem_initialize(0);
    //����ʾ
    fb_open();
    //SPI0��������ʼ��
    ls1x_spi_initialize(busSPI0);
    
    unsigned int id,wroffset = 0x51000,rdoffset = 0x51000;
	char sbuf[20];
	//��ʼ��W25X40оƬ
	ls1x_w25x40_init(busSPI0, NULL);
	//��ȡW25X40оƬID
	ls1x_w25x40_ioctl(busSPI0, IOCTL_W25X40_READ_ID, &id);
    //printk("W25X40 ID:%x\n\n",id);
    sprintf((char *)sbuf,"W25X40 ID:%x\n",id);
    
    unsigned char rbuf[8],tbuf[8]="�й�";
	unsigned char tx_count = 0;
	unsigned char s1[65],s2[65];

/*
 * ��ע��W25X40оƬ�ڲ�д��PMON��ռ�ÿռ�Լ268K����˲���W25X40��д����ʱ��������268K�ռ�����
 * �����̲��Ե�ַΪ300-512K�ռ�
 * nor flash���ԣ�д����֮ǰ����Ҫ�Ȳ�������оƬ�������������������Ƶ�
 */
    
    /*
     * �����ѭ��
     */
    while(1){
        ls1x_w25x40_ioctl(busSPI0, IOCTL_W25X40_ERASE_4K, wroffset);  //spinor flashд֮ǰ��Ҫ�Ȳ���
        memset(rbuf, 0, 8);

        //��W25X40оƬд������
	    ls1x_w25x40_write(busSPI0, tbuf, 8, &wroffset);

	    ls1x_w25x40_read(busSPI0, rbuf, 8, &rdoffset);

        fb_textout(10, 80, "W25X40 read data");
        fb_fillrect(10, 100, 480, 116, cidxBLACK);
	    fb_textout(10, 100, rbuf);
	    delay_ms(500);
    }

    return 0;
}



