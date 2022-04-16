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
 ** ���ܣ�  ��ʼ��I2C1
 ** ˵������ʼ��I2C1ʱҪ�Ƚ�GPIO38/39����ΪI2C1����
*************************************************************************/
 void I2C1_init(void)
 {
    //��GPIO38/39����Ϊ��ͨ����
    gpio_disable(38);
    gpio_disable(39);

    //��GPIO38/39����ΪI2C1����
    LS1B_MUX_CTRL0 |= 1 << 24;

    //��ʼ��I2C1������
    ls1x_i2c_initialize(busI2C1);
 }

/************************************************************************
** ���ܣ�  ��OPT3002д������
** ������
           @reg_buf:�Ĵ�����ַ
           @buf:���ݻ�������
           @len:д���ݳ���
** ����ֵ��0,�ɹ�;-1,ʧ��.
*************************************************************************/
static char HDC_WR_Data(unsigned char reg_addr, unsigned char *buf, int len)
{
	int ret=0;

	//��ʼ�ź�
	ret = ls1x_i2c_send_start(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_start error!!!\r\n");
        return -1;
    }

 	//���ʹӻ���ַ��д����
 	ret = ls1x_i2c_send_addr(busI2C1, OPT3002_ADDRESS, OPT3002_Write);
 	if(ret < 0)
    {
        printf("send_addr error!!!\r\n");
        return -1;
    }

    //���ͼĴ�����ַ
	ret = ls1x_i2c_write_bytes(busI2C1, &reg_addr, 1);
	if(ret < 0)
    {
        printf("write_bytes_reg error!!!\r\n");
        return -1;
    }

    //��������
	ret = ls1x_i2c_write_bytes(busI2C1, buf, len);
	if(ret < 0)
    {
        printf("write_bytes error!!!\r\n");
        return -1;
    }

    //����ֹͣ�ź�
    ret = ls1x_i2c_send_stop(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_stop error!!!\r\n");
        return -1;
    }

	return ret;
}

/************************************************************************
** ���ܣ�  ��OPT3002��������
** ������
           @reg_buf:�Ĵ����ĵ�ַ
           @buf:���ݻ�������
           @len:д���ݳ���
** ����ֵ��0,�ɹ�;-1,ʧ��.
*************************************************************************/
static char HDC_RD_Data(unsigned char reg_addr,unsigned char *buf,int len)
{
	int ret=0;

 	//��ʼ�ź�
	ret = ls1x_i2c_send_start(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_start error!!!\r\n");
        return -1;
    }

 	//���ʹӻ���ַ��д����
 	ret = ls1x_i2c_send_addr(busI2C1, OPT3002_ADDRESS, OPT3002_Write);
 	if(ret < 0)
    {
        printf("send_addr_W error!!!\r\n");
        return -1;
    }

    //���ͼĴ�����ַ
	ret = ls1x_i2c_write_bytes(busI2C1, &reg_addr, 1);
	if(ret < 0)
    {
        printf("write_bytes_reg error!!!\r\n");
        return -1;
    }

    //����ֹͣ�ź�
    ret = ls1x_i2c_send_stop(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_stop error!!!\r\n");
        return -1;
    }

    //��ʼ�ź�
	ret = ls1x_i2c_send_start(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_start error!!!\r\n");
        return -1;
    }

    //���ʹӻ���ַ�Ͷ�����
 	ret = ls1x_i2c_send_addr(busI2C1, OPT3002_ADDRESS, OPT3002_Read);
 	if(ret < 0)
    {
        printf("send_addr_R error!!!\r\n");
        return -1;
    }

    //��ȡ����
    ls1x_i2c_read_bytes(busI2C1,buf,len);
    if(ret < 0)
    {
        printf("read_bytes_Data error!!!\r\n");
        return -1;
    }

    //����ֹͣ�ź�
    ret = ls1x_i2c_send_stop(busI2C1,NULL);
    if(ret < 0)
    {
        printf("send_stop error!!!\r\n");
        return -1;
    }

    return 0;
}

/************************************************************************
 ** ���ܣ���ȡOPT�豸��ID
 ** ˵�����豸����ID��0x5449,Ҳ����ACSII���"TI",�Ĵ�����ַΪ0X7E;
          �豸ID��0x3001,�Ĵ�����ַ��0X7F
*************************************************************************/
 void Get_OPT_ID(void)
 {
     unsigned char Device_ID[2];
     uint16_t Device_id,Device_id_H,Device_id_L;

     HDC_RD_Data(Device_addr, Device_ID, 2);
     Device_id_H = Device_ID[0];
     Device_id_L = Device_ID[1];
     Device_id = (Device_id_H << 8) | Device_id_L;
     printf("OPT3002���豸IDΪ��%#x\r\n",Device_id);
 }
 
 /***************************************************************************************************
 ** ���ܣ�����OPT3001�Ĺ���ģʽ
 ** ˵����
    12:15 RN  	    - ���ò������յķ�Χ ���ֲ��7.6.1.1.2  ������λ1100������������Χ�Զ�ѡ��
	11    CT  	    - ����ʱ������ 0- 100Ms  1-800Ms
	10:9  M[1:0]	- ת��ģʽ 00-�ر�ģʽ  01 - ����ת��  10��11 - �������ת��
	8     OVF       - �������ճ����趨�ķ�Χ��������ֵ �����־
	7     CRF		- ת�������ֶ� 1-ת�����
	6     FH		- ת���Ĺ���ֵ ��������ֵ ��λ
	5     FL		- ת���Ĺ���ֵ С������ֵ ��λ
	4     L		    - �ж����������ģʽ  1-����ģʽ ����ģʽ�¸�����λ�͵�����λINT���
                    0-�ͺ�ģʽ ������λINT��� ���忴�ֲ�
	3     POL		- INT �жϱ������������ 0-����  1-����
	2     ME 		- �����ֶ�
	0:1   FC		- �������޷�Χ���ϼ���  ����������� ���ڵ��ڼ����趨����  INT����ж�
****************************************************************************************************/
 void OPT3002_Conf(void)
 {
     //�Զ��ɼ�ת������,���������ϱ�,ת����ʽ����Ϊ����ת��
     unsigned char conf_buf[2] = {0xCE,0x10};
     HDC_WR_Data(REG_Config,conf_buf,2);
     
     gpio_enable(LED9_IO, DIR_OUT);
     gpio_write(LED9_IO, 1);
 }
 
 /************************************************************************
 ** ���ܣ���ȡ����ǿ��
 ** ˵����ǿ�ȴ���120000ʱ��LED9������֮��LED9��
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
