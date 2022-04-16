/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _BSP_H
#define _BSP_H

//-----------------------------------------------------------------------------

#define BSP_USE_OS          (!defined(OS_NONE))     // Is using RTOS?

#define BSP_USE_LWMEM       (!defined(OS_RTTHREAD)) // Need memory manager?

//-----------------------------------------------------------------------------
// cpu �ⲿ����Ƶ��: HZ
//-----------------------------------------------------------------------------

#define CPU_XTAL_FREQUENCY  24000000

#ifndef __ASSEMBLER__

/*
 * ��ӡ����ʹ�� printk
 */
#if defined(OS_RTTHREAD)
extern void rt_kprintf(const char *fmt, ...);
#define printk      rt_kprintf
#endif

#if 1
#define DBG_OUT(...)    printk(__VA_ARGS__)
#else
#define DBG_OUT(...)
#endif

//-----------------------------------------------------------------------------
// ��ʱ����
//-----------------------------------------------------------------------------

/*
 * in "xxx/port/clock.c"
 */
extern void delay_us(unsigned int us);
extern void delay_ms(unsigned int ms);

//-----------------------------------------------------------------------------
// Ƭ���豸ʹ�� 
//-----------------------------------------------------------------------------

//#define BSP_USE_SPI0
//#define BSP_USE_SPI1

//#define BSP_USE_I2C0
//#define BSP_USE_I2C1
//#define BSP_USE_I2C2

//#define BSP_USE_UART2
#define BSP_USE_UART3
//#define BSP_USE_UART4
#define BSP_USE_UART5           // Console_Port
//#define BSP_USE_UART0
//#define BSP_USE_UART01
//#define BSP_USE_UART02
//#define BSP_USE_UART03
//#define BSP_USE_UART1
//#define BSP_USE_UART11
//#define BSP_USE_UART12
//#define BSP_USE_UART13

//#define BSP_USE_CAN0
//#define BSP_USE_CAN1

//#define BSP_USE_NAND

//#define BSP_USE_FB              // framebuffer

//#define BSP_USE_GMAC0
#ifdef LS1B
//#define BSP_USE_GMAC1
#endif

//-----------------------------------------------------------------------------
// �ⲿ�豸 
//-----------------------------------------------------------------------------

/*
 * I2C �豸
 */
#ifdef BSP_USE_I2C0
#ifdef BSP_USE_FB
#define GP7101_DRV              // LCD ���ȿ���
#endif
#define PCA9557_DRV             // GPIO оƬ
#define ADS1015_DRV             // 4· 12bit ADC
#define MCP4725_DRV             // 1· 12bit DAC
#define RX8010_DRV              // RTC оƬ
#endif

/*
 * SPI �豸
 */
#ifdef BSP_USE_SPI0
#define W25X40_DRV
#ifdef BSP_USE_FB
#define XPT2046_DRV             // ������оƬ
#endif
#endif

//-----------------------------------------------------------------------------
// �����������
//-----------------------------------------------------------------------------

#ifdef BSP_USE_NAND
#define USE_YAFFS2              // yaffs �ļ�ϵͳ
#endif

#define USE_MODBUS              // modbus Э���

//-----------------------------------------------------------------------------
// in "irq.c"
//-----------------------------------------------------------------------------

extern void ls1x_install_irq_handler(int vector, void (*isr)(int, void *), void *arg);
extern void ls1x_remove_irq_handler(int vector);

#endif // __ASSEMBLER__

#endif // _BSP_H



