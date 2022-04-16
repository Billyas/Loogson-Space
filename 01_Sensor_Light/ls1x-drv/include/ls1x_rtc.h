/*
 * ls1x_rtc.h
 *
 * created: 2020/6/30
 * authour: 
 */

/*
 * LS1B: ����ʹ��
 *       1. ��д toywritehi/toywritelo��, һֱ��� rtc_ctrl_ts λ. 
 *          ��ʹ�ܺ�: ���Զ�������, ����ȷ�ؼ�ʱ; toymatch ��ʱ����ʹ��
 *       2. ��д rtcwrite��, һֱ��� rtc_ctrl_rs λ. 
 *          ��ʹ�ܺ�: rtcmatch ��������ʹ��
 *
 * LS1C: ����ʹ��. ����Ϊ: ��д toytrim/rtctrim ���������, �����ܷ�.
 *
 */

#ifndef _LS1X_RTC_H
#define _LS1X_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------

#include <time.h>

#include "ls1x_io.h"

//-----------------------------------------------------------------------------

#define LS1X_RTC                0x0500
#define LS1X_TOY                0x0A00

/*
 * Virtual Sub-Device
 */
#define DEVICE_RTCMATCH0       (LS1X_RTC | 1)
#define DEVICE_RTCMATCH1       (LS1X_RTC | 2)
#define DEVICE_RTCMATCH2       (LS1X_RTC | 3)

#define DEVICE_TOYMATCH0       (LS1X_TOY | 1)
#define DEVICE_TOYMATCH1       (LS1X_TOY | 2)
#define DEVICE_TOYMATCH2       (LS1X_TOY | 3)

//-----------------------------------------------------------------------------
// ��ʱ���жϻص�����
//-----------------------------------------------------------------------------

typedef void (*rtctimer_callback_t)(int device, unsigned match, int *stop);

//-----------------------------------------------------------------------------
// RTC Ioctl parameter
//-----------------------------------------------------------------------------

typedef struct rtc_cfg
{
    /*
     * This parameter's unit is millisecond(ms). After interrupt ocurred,
     * this value will auto loaded for next match interrupt.
     * if arrive_datetime is NULL, toymatch use this parameter.
     *
     */
    int        interval_ms;

    /*
     * This parameter is used by toymatch. When toymatch arrives this datetime
     * toymatch interrupt will be triggered. This interrupt ocurred only once.
     *
     * if This parameter is NULL, use interval_ms as irq trigger interval
     *
     */
    struct tm *trig_datetime;

    irq_handler_t       isr;            /* User defined match-isr */
    rtctimer_callback_t cb;             /* called by match-isr */
#if BSP_USE_OS
    void               *event;          /* RTOS event created by user */
#endif
} rtc_cfg_t;

//-----------------------------------------------------------------------------
// ioctl command
//-----------------------------------------------------------------------------

/**
 * control rtc
 */
#define IOCTL_RTC_SET_TRIM          0x0801      // unsigned int *
#define IOCTL_RTC_GET_TRIM          0x0802      // unsigned int *

#define IOCTL_RTCMATCH_START        0x0805      // DEVICE_XXX & rtc_cfg_t *
#define IOCTL_RTCMATCH_STOP         0x0806      // DEVICE_XXX

/**
 * control toy
 */
#define IOCTL_TOY_SET_DATETIME      0x8011      // struct tm *
#define IOCTL_TOY_GET_DATETIME      0x8012      // struct tm *

#define IOCTL_TOY_SET_TRIM          0x0813      // unsigned int *
#define IOCTL_TOY_GET_TRIM          0x0814      // unsigned int *

#define IOCTL_TOYMATCH_START        0x0815      // DEVICE_XXX & rtc_cfg_t *
#define IOCTL_TOYMATCH_STOP         0x0816      // DEVICE_XXX

//-----------------------------------------------------------------------------
// RTC driver operators
//-----------------------------------------------------------------------------

#if (PACK_DRV_OPS)

extern driver_ops_t *ls1x_rtc_drv_ops;

#define ls1x_rtc_init(rtc, arg)             ls1x_rtc_drv_ops->init_entry(rtc, arg)
#define ls1x_rtc_open(rtc, arg)             ls1x_rtc_drv_ops->open_entry(rtc, arg)
#define ls1x_rtc_close(rtc, arg)            ls1x_rtc_drv_ops->close_entry(rtc, arg)
#define ls1x_rtc_read(rtc, buf, size, arg)  ls1x_rtc_drv_ops->read_entry(rtc, buf, size, arg)
#define ls1x_rtc_write(rtc, buf, size, arg) ls1x_rtc_drv_ops->write_entry(rtc, buf, size, arg)
#define ls1x_rtc_ioctl(rtc, cmd, arg)       ls1x_rtc_drv_ops->ioctl_entry(rtc, cmd, arg)

#else

/*
 * dev:  NULL
 * arg:  struct tm *, if not NULL then initialize datetime => toywrite
 */
int LS1x_RTC_initialize(void *dev, void *arg);

/*
 * dev:  DEVICE_XXX
 * arg:  rtc_cfg_t *
 */
int LS1x_RTC_open(void *dev, void *arg);

/*
 * dev:  DEVICE_XXX
 * arg:  NULL
 */
int LS1x_RTC_close(void *dev, void *arg);

/*
 * get current datetime <= toyread
 *
 * dev:  NULL
 * buf:  struct tm *
 * size: sizeof(struct tm)
 * arg:  NULL
 */
int LS1x_RTC_read(void *dev, void *buf, int size, void *arg);

/*
 * set current datetime => toywrite
 *
 * dev:  NULL
 * buf:  struct tm *
 * size: sizeof(struct tm)
 * arg:  NULL
 */
int LS1x_RTC_write(void *dev, void *buf, int size, void *arg);

/*
 * dev:  NULL or DEVICE_XXX
 * arg:  if dev==NULL then do TRIM access
 *       else is as rtc_cfg_t *
 */
int LS1x_RTC_ioctl(void *dev, int cmd, void *arg);                 

#define ls1x_rtc_init(rtc, arg)             LS1x_RTC_initialize(rtc, arg)
#define ls1x_rtc_open(rtc, arg)             LS1x_RTC_open(rtc, arg)
#define ls1x_rtc_close(rtc, arg)            LS1x_RTC_close(rtc, arg)
#define ls1x_rtc_read(rtc, buf, size, arg)  LS1x_RTC_read(rtc, buf, size, arg)
#define ls1x_rtc_write(rtc, buf, size, arg) LS1x_RTC_write(rtc, buf, size, arg)
#define ls1x_rtc_ioctl(rtc, cmd, arg)       LS1x_RTC_ioctl(rtc, cmd, arg)

#endif

//-----------------------------------------------------------------------------
// User API
//-----------------------------------------------------------------------------

int ls1x_rtc_set_datetime(struct tm *dt);                   /* ���õ�ǰʱ�� */
int ls1x_rtc_get_datetime(struct tm *dt);                   /* ��ȡ��ǰʱ�� */

/*
 * device is Virtual Sub Device
 */
int ls1x_rtc_timer_start(unsigned device, rtc_cfg_t *cfg);  /* ������ʱ�� */
int ls1x_rtc_timer_stop(unsigned device);                   /* �رն�ʱ�� */

/*
 * LS1x toymatch ���ڸ�ʽת��
 */
void ls1x_tm_to_toymatch(struct tm *dt, unsigned int *match);
void ls1x_toymatch_to_tm(struct tm *dt, unsigned int match);

unsigned int ls1x_seconds_to_toymatch(unsigned int seconds);
unsigned int ls1x_toymatch_to_seconds(unsigned int match);

/*
 * struct tm ���ڸ�ʽת��, +1900/-1900
 */
void normalize_tm(struct tm *tm, bool tm_format);

#ifdef __cplusplus
}
#endif

#endif // _LS1X_RTC_H

