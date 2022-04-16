/*
 * opt3002.c
 *
 * created: 2021/6/5
 *  author: 
 */
 #include "opt3002.h"
 #include "ls1x_i2c_bus.h"
 #include "ls1b_gpio.h"
 #include <stdint.h>
 #include <math.h>

 #define OPT3002_ADDRESS 0X44
 #define OPT3002_Write 0
 #define OPT3002_Read  1
 
 #define Device_addr 0x7f
 #define REG_Config  0x01
 #define REG_Result  0x00
 #define LED9_IO 51
 
 
  /************************************************************************
 ** 功能：  初始化I2C1
 ** 说明：初始化I2C1时要先将GPIO38/39复用为I2C1功能
*************************************************************************/
 void I2C1_init(void)
 {
    //将GPIO38/39复用为普通功能
    gpio_disable(38);
    gpio_disable(39);

    //将GPIO38/39复用为I2C1功能
    LS1B_MUX_CTRL0 |= 1 << 24;

    //初始化I2C1控制器
    ls1x_i2c_initialize(busI2C1);
 }

/************************************************************************
** 功能：  向OPT3002写入数据
** 参数：
           @reg_buf:寄存器地址
           @buf:数据缓缓存区
           @len:写数据长度
** 返回值：0,成功;-1,失败.
*************************************************************************/
static char HDC_WR_Data(unsigned char reg_addr, unsigned char *buf, int len)
{
	int ret=0;

	//起始信号
	ret = ls1x_i2c_send_start(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_start error!!!\r\n");
        return -1;
    }

 	//发送从机地址和写命令
 	ret = ls1x_i2c_send_addr(busI2C1, OPT3002_ADDRESS, OPT3002_Write);
 	if(ret < 0)
    {
        printf("send_addr error!!!\r\n");
        return -1;
    }

    //发送寄存器地址
	ret = ls1x_i2c_write_bytes(busI2C1, &reg_addr, 1);
	if(ret < 0)
    {
        printf("write_bytes_reg error!!!\r\n");
        return -1;
    }

    //发送数据
	ret = ls1x_i2c_write_bytes(busI2C1, buf, len);
	if(ret < 0)
    {
        printf("write_bytes error!!!\r\n");
        return -1;
    }

    //发送停止信号
    ret = ls1x_i2c_send_stop(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_stop error!!!\r\n");
        return -1;
    }

	return ret;
}

/************************************************************************
** 功能：  从OPT3002读出数据
** 参数：
           @reg_buf:寄存器的地址
           @buf:数据缓缓存区
           @len:写数据长度
** 返回值：0,成功;-1,失败.
*************************************************************************/
static char HDC_RD_Data(unsigned char reg_addr,unsigned char *buf,int len)
{
	int ret=0;

 	//起始信号
	ret = ls1x_i2c_send_start(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_start error!!!\r\n");
        return -1;
    }

 	//发送从机地址和写命令
 	ret = ls1x_i2c_send_addr(busI2C1, OPT3002_ADDRESS, OPT3002_Write);
 	if(ret < 0)
    {
        printf("send_addr_W error!!!\r\n");
        return -1;
    }

    //发送寄存器地址
	ret = ls1x_i2c_write_bytes(busI2C1, &reg_addr, 1);
	if(ret < 0)
    {
        printf("write_bytes_reg error!!!\r\n");
        return -1;
    }

    //发送停止信号
    ret = ls1x_i2c_send_stop(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_stop error!!!\r\n");
        return -1;
    }

    //起始信号
	ret = ls1x_i2c_send_start(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_start error!!!\r\n");
        return -1;
    }

    //发送从机地址和读命令
 	ret = ls1x_i2c_send_addr(busI2C1, OPT3002_ADDRESS, OPT3002_Read);
 	if(ret < 0)
    {
        printf("send_addr_R error!!!\r\n");
        return -1;
    }

    //读取数据
    ls1x_i2c_read_bytes(busI2C1,buf,len);
    if(ret < 0)
    {
        printf("read_bytes_Data error!!!\r\n");
        return -1;
    }

    //发送停止信号
    ret = ls1x_i2c_send_stop(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_stop error!!!\r\n");
        return -1;
    }

    return 0;
}

/************************************************************************
 ** 功能：获取OPT设备的ID
 ** 说明：设备厂商ID是0x5449,也就是ACSII码的"TI",寄存器地址为0X7E;
          设备ID是0x3001,寄存器地址是0X7F
*************************************************************************/
 void Get_OPT_ID(void)
 {
     unsigned char Device_ID[2];
     uint16_t Device_id,Device_id_H,Device_id_L;

     HDC_RD_Data(Device_addr, Device_ID, 2);
     Device_id_H = Device_ID[0];
     Device_id_L = Device_ID[1];
     Device_id = (Device_id_H << 8) | Device_id_L;
     printf("OPT3002的设备ID为：%#x\r\n",Device_id);
 }
 
 /***************************************************************************************************
 ** 功能：配置OPT3001的工作模式
 ** 说明：
    12:15 RN  	    - 配置测量光照的范围 见手册第7.6.1.1.2  当配置位1100传感器测量范围自动选择
	11    CT  	    - 测量时间配置 0- 100Ms  1-800Ms
	10:9  M[1:0]	- 转换模式 00-关闭模式  01 - 单次转换  10、11 - 连续多次转换
	8     OVF       - 测量光照超出设定的范围或最大测量值 溢出标志
	7     CRF		- 转换就绪字段 1-转换完成
	6     FH		- 转换的光照值 大于上限值 置位
	5     FL		- 转换的光照值 小于下限值 置位
	4     L		    - 中断输出的两种模式  1-窗口模式 这种模式下高限置位和低限置位INT输出
                    0-滞后模式 高限置位INT输出 具体看手册
	3     POL		- INT 中断被触发输出极性 0-拉低  1-拉高
	2     ME 		- 掩码字段
	0:1   FC		- 超出上限范围故障计数  如果超出次数 大于等于计数设定次数  INT输出中断
****************************************************************************************************/
 void OPT3002_Conf(void)
 {
     //自动采集转换数据,但不主动上报,转换方式配置为连续转换
     unsigned char conf_buf[2] = {0xCE,0x10};
     HDC_WR_Data(REG_Config,conf_buf,2);
     
     gpio_enable(LED9_IO, DIR_OUT);
     gpio_write(LED9_IO, 1);
 }
 
 /************************************************************************
 ** 功能：读取光照强度
 ** 说明：强度大于120000时，LED9亮，反之，LED9灭
*************************************************************************/
 float OPT3002_RD_Data(void)
 {
     unsigned char Result[2];
     float data;
     uint16_t res = 0;
     
     HDC_RD_Data(REG_Result, Result, 2);
     
     res = ((Result[0] & 0x0f) << 8) | Result[1];
     
     data = pow(2,Result[0] >> 4) * res * 1.2;
     
     return data;
     
 }
 
void LED9_ON(void)
{
    gpio_write(LED9_IO, 0);
}

void LED9_OFF(void)
{
    gpio_write(LED9_IO, 1);
}
