/*
 * mailbox.c
 *
 * created: 2021/3/26
 *  author: 
 */


#include "mailbox.h"


void Key_Init(void)
{
    gpio_enable(KEY1, DIR_IN);
    gpio_enable(KEY2, DIR_IN);
    gpio_enable(KEY3, DIR_IN);
}


void ADC_DAC_Init(void)
{
    ls1x_i2c_initialize(busI2C0);
}


uint16_t dac=0;
void ADC_DAC_Test(void)
{
    char tbuf[40]={0},sbuf[40]={0},rt;
    uint16_t adc=0;
    int i=0;

    for(i=0;i<10;i++){
        sprintf((char *)tbuf,"MCP4725_write_adc: dac = %d\n",dac);
        set_mcp4725_dac(busI2C0, dac);
        dac += 5;
        if(dac > 4096)
            dac = 0;

        adc = get_ads1015_adc(busI2C0, ADS1015_REG_CONFIG_MUX_SINGLE_3);
        sprintf((char *)sbuf,"ADS1015_get_adc: adc = %d\n",adc);

        rt_kprintf("%s\n%s\n\n",tbuf,sbuf);

        fb_textout(10, 20, "MCP4725 DAC SET");
        fb_fillrect(10, 40, 480, 56, cidxBLACK);
        fb_textout(10, 40, tbuf);
        fb_textout(10, 60, "ADS1015 ADC GET");
        fb_fillrect(10, 80, 480, 96, cidxBLACK);
        fb_textout(10, 80, sbuf);
        delay_ms(500);
	}
}


