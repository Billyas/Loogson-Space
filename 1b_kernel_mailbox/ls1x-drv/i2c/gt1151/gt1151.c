/*
 * gt1151.c
 *
 * created: 2021/3/15
 *  author: 
 */

#include "bsp.h"

#ifdef GT1151_DRV

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "ls1x_io.h"
#include "ls1x_i2c_bus.h"
#include "../ls1x_i2c_bus_hw.h"

#if defined(LS1B)
#include "ls1b.h"
#include "ls1b_gpio.h"
#elif defined(LS1C)
#include "ls1c.h"
#else
#error "No Loongson1x SoC defined."
#endif

#include "i2c/gt1151.h"
#include "touch.h"
#include "ls1x_fb.h"

static int pixelsx = 0;
static int pixelsy = 0;

//-----------------------------------------------------------------------------

#define CHECK_DONE(rt) \
	do {               \
        if (0 != rt)   \
            goto lbl_done; \
    } while (0);

//-----------------------------------------------------------------------------


// GT1151���ò���
// ��һ���ֽ�Ϊ�汾�ţ�0x60�������뱣֤�µİ汾�Ŵ��ڵ���GT1151�ڲ�
// flashԭ�а汾�ţ��Ż��������

const unsigned char GT1151_CFG_TBL[]=
{
	0x63,0xE0,0x01,0x20,0x03,0x05,0x3D,0x04,0x00,0x08,
	0x09,0x0F,0x55,0x37,0x33,0x11,0x00,0x03,0x08,0x56,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x48,0x00,0x00,
	0x3C,0x08,0x0A,0x28,0x1E,0x50,0x00,0x00,0x82,0xB4,
	0xD2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x85,0x25,0x10,0x41,0x43,0x31,
	0x0D,0x00,0xAD,0x22,0x24,0x7D,0x1D,0x1D,0x32,0xDF,
	0x4F,0x44,0x0F,0x80,0x2C,0x50,0x50,0x00,0x00,0x00,
	0x00,0xD3,0x00,0x00,0x00,0x00,0x0F,0x28,0x1E,0xFF,
	0xF0,0x37,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x50,0xB4,0xC0,0x94,0x53,0x2D,
	0x0A,0x02,0xBE,0x60,0xA2,0x71,0x8F,0x82,0x80,0x92,
	0x74,0xA3,0x6B,0x01,0x0F,0x14,0x03,0x1E,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x0D,0x0E,0x0F,0x10,0x12,
	0x13,0x14,0x15,0x1F,0x1D,0x1B,0x1A,0x19,0x18,0x17,
	0x16,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x06,0x08,0x0C,
	0x12,0x13,0x14,0x15,0x17,0x18,0x19,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
	0xC4,0x09,0x23,0x23,0x50,0x5D,0x54,0x4B,0x3C,0x0F,
	0x32,0xFF,0xE4,0x04,0x40,0x00,0x8A,0x05,0x40,0x00,
	0xAA,0x00,0x22,0x22,0x00,0x00,0x73,0x22,0x01
};

void check_sum(void)
{
	unsigned checksum=0;
	unsigned char checksumH,checksumL;
	unsigned char i=0;
	for (i=0;i<(sizeof(GT1151_CFG_TBL)-3);i+=2)
	checksum +=((GT1151_CFG_TBL[i]<<8)|GT1151_CFG_TBL[i+1]); // ����У���
	//checksum +=(GT1151_CFG_TBL[i]<<8)+GT1151_CFG_TBL[i+1];
	//checksum =0-checksum;
	checksum =(~checksum)+1;
	checksumH=checksum>>8;
	checksumL=checksum;
	printf("chksum:0x%X,\r\n",checksum);
	printf("chksumH:0x%X,\r\n",checksumH);
	printf("chksumL:0x%X,\r\n",checksumL);
}

// ��GT1151д��һ������
// @reg����ʼ�Ĵ�����ַ
// @buf�����ݻ�����
// @len��д���ݳ���
// ����ֵ���ɹ�����д���������

static int GT1151_WR_Reg(LS1x_I2C_bus_t *pIIC, unsigned reg, unsigned char *buf, unsigned char len)
{
	int rt=0,rw_cnt=0;
	unsigned char addr[2];
	unsigned char *data = buf;
	
    /* start transfer */
	rt = ls1x_i2c_send_start(pIIC, GT1151_ADDRESS);
	CHECK_DONE(rt);
	
	/* set transfer mode */
	rt = ls1x_i2c_ioctl(pIIC, IOCTL_SPI_I2C_SET_TFRMODE, (void *)GT1151_BAUDRATE);
	CHECK_DONE(rt);
	
 	/* address device, FALSE(0) for write */
	rt = ls1x_i2c_send_addr(pIIC, GT1151_ADDRESS, false);
	CHECK_DONE(rt);
	
	//send reg addr
	addr[0] = reg >> 8;
	addr[1] = reg & 0xFF;
	rw_cnt = ls1x_i2c_write_bytes(pIIC, addr, 2);
	if (rw_cnt < 0)
        rt = rw_cnt;
	CHECK_DONE(rt);
	
	//send data
	rw_cnt = ls1x_i2c_write_bytes(pIIC, data, len);
	if (rw_cnt < 0)
        rt = rw_cnt;
	CHECK_DONE(rt);
	
	rt = rw_cnt;

lbl_done:
	/* terminate transfer */
	ls1x_i2c_send_stop(pIIC, GT1151_ADDRESS);
	
	return rt;
}

// ��GT1151����һ������
// @reg����ʼ�Ĵ�����ַ
// @buf�����ݻ�����
// @len�������ݳ���

static int GT1151_RD_Reg(LS1x_I2C_bus_t *pIIC, unsigned reg, unsigned char *buf, unsigned char len)
{
	int rt=0,rw_cnt=0;
	unsigned char addr[2];
	
	/* start transfer */
	rt = ls1x_i2c_send_start(pIIC, GT1151_ADDRESS);
	CHECK_DONE(rt);

	/* set transfer mode */
	rt = ls1x_i2c_ioctl(pIIC, IOCTL_SPI_I2C_SET_TFRMODE, (void *)GT1151_BAUDRATE);
	CHECK_DONE(rt);
	
	/* address device, FALSE(0) for write */
	rt = ls1x_i2c_send_addr(pIIC, GT1151_ADDRESS, false);
	CHECK_DONE(rt);

	// send reg addr
	addr[0] = reg >> 8;
	addr[1] = reg & 0xFF;
	rw_cnt = ls1x_i2c_write_bytes(pIIC, addr, 2);
	if (rw_cnt < 0)
        rt = rw_cnt;
	CHECK_DONE(rt);
	
	/* terminate transfer */
	ls1x_i2c_send_stop(pIIC, GT1151_ADDRESS);  // ����Ҫ��ֹͣ�źţ��������Ķ�ȡ������
	
    /* restart: start transfer */
	rt = ls1x_i2c_send_start(pIIC, GT1151_ADDRESS);
	CHECK_DONE(rt);
	
 	/* address device, TRUE(1) for READ */
	rt = ls1x_i2c_send_addr(pIIC, GT1151_ADDRESS, true);
	CHECK_DONE(rt);
	
	//-----------------------------------------------------
	// read out data
	//-----------------------------------------------------
	rw_cnt = ls1x_i2c_read_bytes(pIIC, buf, len);
	if (rw_cnt < 0)
		rt = rw_cnt;
	CHECK_DONE(rt);

	rt = rw_cnt;

lbl_done:

	/* terminate transfer */
	ls1x_i2c_send_stop(pIIC, GT1151_ADDRESS);
	
	return rt;
}

// ����GT1151���ò���
// @mode��0-���������浽flash
//        1-�������浽flash

unsigned char GT1151_Send_Cfg(unsigned char mode)
{
    unsigned checksum=0;
	unsigned char buf[3];
	unsigned char i=0;
	for (i=0;i<(sizeof(GT1151_CFG_TBL)-3);i+=2)
	checksum +=((GT1151_CFG_TBL[i]<<8)|GT1151_CFG_TBL[i+1]);//����У���
	//checksum +=(GT1151_CFG_TBL[i]<<8)+GT1151_CFG_TBL[i+1];
	//checksum =0-checksum;
	checksum =(~checksum)+1;
	printf("chksum:0x%x,\r\n",checksum);
	buf[0]= checksum>>8;
	buf[1]= checksum;
	buf[2]= mode;	//�Ƿ�д�뵽GT1151 FLASH�����Ƿ���籣��
	GT1151_WR_Reg(busI2C0,GT_CFGS_REG,(unsigned char*)GT1151_CFG_TBL,sizeof(GT1151_CFG_TBL));//���ͼĴ�������
	return 0;
}

/******************************************************************************
 * driver routine
 ******************************************************************************/

STATIC_DRV int GT1151_init(void *bus, void *arg)
{
    unsigned char temp[5];

    if (bus == NULL)
        return -1;

    //GT_RST - GPIO2
    gpio_enable(GT_RST, DIR_OUT);
    gpio_write(GT_RST, 0);    //��λ
    delay_ms(10);
    gpio_write(GT_RST, 1);    //�ͷŸ�λ
    delay_ms(10);

    //GT_INT - GPIO3
    gpio_enable(GT_INT, DIR_IN);
    delay_ms(100);
    
    GT1151_RD_Reg((LS1x_I2C_bus_t *)bus,GT_PID_REG,temp,4);//��ȡ��ƷID
	temp[4]=0;
	printk("CTP ID:GT%s\r\n",temp);	//��ӡID
	check_sum();
	
	pixelsx = fb_get_pixelsx();
	pixelsy = fb_get_pixelsy();
	
	if (strcmp((char*)temp,"1158")==0)//ID==1158
	{
		temp[0]=0X02;
		GT1151_WR_Reg((LS1x_I2C_bus_t *)bus,GT_CTRL_REG,temp,1);//��λT1151
		GT1151_RD_Reg((LS1x_I2C_bus_t *)bus,GT_CFGS_REG,temp,1);//��ȡGT_CFGS_REG�Ĵ���
		if (temp[0]<0X60)//Ĭ�ϰ汾�Ƚϵͣ���Ҫ����flash����
		{
			printk("Default Ver:0x%x\r\n",temp[0]);
			GT1151_Send_Cfg(1);//���²���������
		}
		delay_ms(10);
		temp[0]=0X00;
		GT1151_WR_Reg((LS1x_I2C_bus_t *)bus,GT_CTRL_REG,temp,1);//������λ

	#if 0
        int i;
        unsigned char Cfg_Info[239] = {0};
        
		GT1151_RD_Reg((LS1x_I2C_bus_t *)bus,GT_CFGS_REG,Cfg_Info,239);
		printk("Config Info:\r\n");
		for (i = 0; i < 239; i++ )
		{
			printk("0x%02x,",Cfg_Info[i]);
			if ((i+1)%10==0)
			printk("\r\n");
		}
		printk("\n");
	#endif

		return 0;
	}
	
	return 1;
}

STATIC_DRV int GT1151_write(void *bus, void *buf, int size, void *arg)
{
    if ((bus == NULL) || (buf == NULL))
        return -1;
        
    unsigned reg = (unsigned)arg;

    return GT1151_WR_Reg((LS1x_I2C_bus_t *)bus, reg, (unsigned char *)buf, size);
}

STATIC_DRV int GT1151_read(void *bus, void *buf, int size, void *arg)
{
    if ((bus == NULL) || (buf == NULL))
        return -1;

    unsigned reg = (unsigned)arg;

    /* input register */
    return GT1151_RD_Reg((LS1x_I2C_bus_t *)bus, reg, (unsigned char *)buf, size);
}

#if (PACK_DRV_OPS)
/******************************************************************************
 * GT1151 driver operators
 */
static driver_ops_t LS1x_GT1151_drv_ops =
{
    .init_entry  = GT1151_init,
    .open_entry  = NULL,
    .close_entry = NULL,
    .read_entry  = GT1151_read,
    .write_entry = GT1151_write,
    .ioctl_entry = NULL,
};
driver_ops_t *ls1x_gt1151_drv_ops = &LS1x_GT1151_drv_ops;
#endif

//--------------------------------------------------------------------------------------------

const unsigned GT1151_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

// ɨ�败���������ò�ѯ��ʽ��
// mode��0-����ɨ��
// ����ֵ����ǰ����״̬ 0-�����޴���  1-�����д���

unsigned char GT1151_Scan(unsigned char mode)
{
    unsigned char buf[4],i=0,res=0,temp,tempsta;
    static unsigned char t=0;   // ���Ʋ�ѯ������Ӷ�����CPUռ����
    t++;
    
    if ((t%10)==0||t<10)     	// ����ʱ��ÿ����10��CTP_Scan�����ż��1�Σ��Ӷ���ʡCPUʹ����
    {
        GT1151_RD_Reg(busI2C0,GT_GSTID_REG,&mode,1);	//��ȡ�������״̬(GT1151��ǰ��⵽�Ĵ������)
        if ((mode&0X80) && ((mode&0XF)<6))
		{
			temp=0;
			GT1151_WR_Reg(busI2C0,GT_GSTID_REG,&temp,1);//���־
		}
		if ((mode&0XF)&&((mode&0XF)<6))
		{
		    temp = 0xFF << (mode & 0xF);      	//����ĸ���ת��Ϊ1��λ����ƥ��tp_dev.sta����
		    tempsta = tp_dev.sta;             	//���浱ǰ��tp_dev.staֵ
		    tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES;
			tp_dev.x[4]=tp_dev.x[0];			//���津��0������
			tp_dev.y[4]=tp_dev.y[0];
			for (i=0;i<5;i++)
			{
				if (tp_dev.sta&(1<<i))			//������Ч?
				{
					GT1151_RD_Reg(busI2C0,GT1151_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
					tp_dev.x[i]=((unsigned)buf[1]<<8)+buf[0];
					tp_dev.y[i]=((unsigned)buf[3]<<8)+buf[2];
//					printk("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}
			}
			res=1;
			if (tp_dev.x[0]>pixelsx || tp_dev.y[0]>pixelsy)//�Ƿ����ݣ����곬���ˣ�
			{
				if ((mode&0XF)>1)		//�������������ݣ��򸴵ڶ�����������ݵ���һ������
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//����һ�Σ���������������10�Σ��Ӷ����������
				}
				else					//�Ƿ����ݣ�����Դ˴����ݣ���ԭԭ���ģ�
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					mode=0X80;
					tp_dev.sta=tempsta;	//�ָ�tp_dev.sta
				}
			}
			else
				t=0;					//����һ�Σ���������������10�Σ��Ӷ����������
        }
    }
    
    if ((mode&0X8F)==0X80)				//�޴����㰴��
	{
		if (tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ�����
		{
			tp_dev.sta&=~(1<<7);		//��־�����ɿ�
		}
		else							//֮ǰ��û�б�����
		{
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;			//�������Ч��־
		}
	}
	
	if (t>240)
        t=10;	//���´�10��ʼ����
	
	return res;
}

#endif

