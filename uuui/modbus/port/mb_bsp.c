/**************************************************************************************************
 * Modbus Board Support Package
 *
 * Filename: mb_bsp.C
 * Version:
 **************************************************************************************************/

/**************************************************************************************************
 * INCLUDE FILES
 **************************************************************************************************/

#include <stdint.h>
#include <string.h>

//-----------------------------------------------------------------------------
// PORTING FOR LS1x BARE/RTOS PROGRAMMING
//-----------------------------------------------------------------------------

#include "bsp.h"

#if defined(LS1B)
#include "ls1b.h"
#include "ls1b_irq.h"
#include "ls1x_rtc.h"
#elif defined(LS1C)
#include "ls1c.h"
#include "ls1c_irq.h"
#include "ls1x_pwm.h"
#else
#error "No Loongson1x SoC defined."
#endif

#include "termios.h"
#include "ns16550.h"
#include "../../ls1x-drv/uart/ns16550_p.h"

//-----------------------------------------------------------------------------

#include "../app/mb_cfg.h"
#include "../src/mb.h"

#include "mb_bsp.h"

/**************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

uint8_t  mb_devices_count = 0;                  /* Modbus channel counter (0..MODBUS_MAX_CH) */
MODBUS_t mb_devices_tbl[MODBUS_CFG_CHNL_MAX];   /* Modbus channels */

/**************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/

/**************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/**************************************************************************************************
 * function:    modbus_hw_port_exit()
 * Description: This function is called to terminate Modbus communications.
 *              All Modbus channels are close.
 * Argument(s): none
 * Return(s):   none.
 *
 * Caller(s):   modbus_exit()
 * Note(s):     none.
 **************************************************************************************************/

void modbus_hw_port_exit(void)
{
    uint8_t   i;
    MODBUS_t *p_mb = &mb_devices_tbl[0];

    for (i = 0; i < MODBUS_CFG_CHNL_MAX; i++)
    {
        if (NULL != p_mb->PtrUART)
        {
            ls1x_uart_close(p_mb->PtrUART, NULL);
            p_mb->PtrUART = NULL;
        }

        p_mb++;
    }
}

/**************************************************************************************************
 * function:    modbus_hw_port_config()
 * Description: This function initializes the serial port to the desired baud rate and the UART
 *              will be configured for N, 8, 1 (No parity, 8 bits, 1 stop).
 * Argument(s): p_mb       is a pointer to the modbus channel
 *              baud       is the desired baud rate for the serial port.
 *              parity     is the desired parity and can be either:
 *                         MODBUS_PARITY_NONE
 *                         MODBUS_PARITY_ODD
 *                         MODBUS_PARITY_EVEN
 *              bits       specifies the number of bit and can be either 7 or 8.
 *              stops      specifies the number of stop bits and can either be 1 or 2
 * Return(s):   none.
 *
 * Caller(s):   modbus_config_node()
 * Note(s):     none.
 **************************************************************************************************/

void modbus_hw_port_config(MODBUS_t *p_mb,
                           uint32_t  baud,
                           uint8_t   bits,
                           uint8_t   parity,
                           uint8_t   stops)
{
    struct termios t;

    if ((NULL == p_mb) || (NULL == p_mb->PtrUART))
        return;

    p_mb->BaudRate = baud;
    p_mb->Parity   = parity;
    p_mb->Bits     = bits;
    p_mb->Stops    = stops;

    ls1x_uart_init(p_mb->PtrUART, NULL);            /* 1st Initialize the UART */

    memset(&t, 0, sizeof(struct termios));

    /* Baudrate */
    switch (baud)
    {
    	case 1200:	 t.c_cflag |= B1200;   break;
    	case 2400:	 t.c_cflag |= B2400;   break;
    	case 4800:	 t.c_cflag |= B4800;   break;
    	case 9600:	 t.c_cflag |= B9600;   break;
    	case 19200:	 t.c_cflag |= B19200;  break;
    	case 38400:	 t.c_cflag |= B38400;  break;
    	case 57600:	 t.c_cflag |= B57600;  break;
    	case 115200: t.c_cflag |= B115200; break;
    	default:     t.c_cflag |= B9600;   break;
    }

    /* Parity */
    if (parity == 'N')              /* None */
    {
        t.c_cflag &=~ PARENB;
    }
    else if (parity == 'E')         /* Even */
    {
        t.c_cflag |= PARENB;
        t.c_cflag &=~ PARODD;
    }
    else                            /* Odd */
    {
        t.c_cflag |= PARENB;
        t.c_cflag |= PARODD;
    }

    /* Character Size */
    switch (bits)
    {
    	case 5:  t.c_cflag |= CS5; break;
    	case 6:  t.c_cflag |= CS6; break;
    	case 7:  t.c_cflag |= CS7; break;
    	case 8:
    	default: t.c_cflag |= CS8; break;
    }

    /* Stop Bits */
    if (stops == 1)                 /* Stop bit = 1 */
        t.c_cflag &= ~CSTOPB;
    else                            /* Stop bit = 2 */
        t.c_cflag |= CSTOPB;

    ls1x_uart_open(p_mb->PtrUART, (void *)&t);      /* 2nd Open the UART */

    /* Now modbus begin receiving. */
}

/**************************************************************************************************
 * function:    modbus_tx_bytes()
 * Description: This function Tx data buffer.
 * Argument(s): p_mb    is a pointer to the modbus channel
 * Return(s):   none.
 *
 * Caller(s):   modbus_tx()
 *
 * Note(s):     none.
 **************************************************************************************************/

void modbus_tx_bytes(MODBUS_t *p_mb)
{
    if (p_mb->TxBufByteCtr > 0)
    {
        int txcount;
        int count = (int)p_mb->TxBufByteCtr;

        while (count > 0)
        {
            txcount = ls1x_uart_write(p_mb->PtrUART,
                                      (void *)p_mb->TxBufPtr,
                                      (int)p_mb->TxBufByteCtr,
                                      NULL);

            if (txcount > 0)
            {
                count -= txcount;
                p_mb->TxBufByteCtr -= txcount;

                p_mb->TxCtr += txcount;
                p_mb->TxBufPtr += txcount;
            }
        }

#if (MODBUS_CFG_MASTER_EN == 1)
        if (p_mb->MasterSlave == MODBUS_MASTER)
        {
    #if (MODBUS_CFG_RTU_EN == 1)
            p_mb->RTU_TimeoutEn = false;            /* Disable RTU timeout timer until we start receiving */
    #endif
            p_mb->RxBufByteCtr = 0;                 /* Flush Rx buffer */
        }
#endif
    }
    else                                            /* If there is nothing to do end transmission */
    {
    	p_mb->TxBufPtr = &p_mb->TxBuf[0];           /* Reset at beginning of buffer */
    }
}

/**************************************************************************************************
 * function:    modbus_rx_1byte()
 * Description: This function Rx a byte.
 * Argument(s): p_mb    is a pointer to the modbus channel
 * Return(s):   none.
 *
 * Caller(s):   modbus_os_rx_wait()
 *              modbus_os_rx_task()
 *
 * Note(s):     none.
 **************************************************************************************************/

int modbus_rx_1byte(MODBUS_t *p_mb, uint8_t *rx_byte, int timeout)
{
    return ls1x_uart_read(p_mb->PtrUART,
                          (void *)rx_byte,
                          1,
                          (void *)timeout);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#if (MODBUS_CFG_RTU_EN == 1)

//-------------------------------------------------------------------------------------------------

uint16_t mb_rtu_frequency = 0;      /* Frequency at which RTU timer is running */
uint32_t mb_rtu_timer_count = 0;    /* Incremented every Modbus RTU timer interrupt */

#ifdef BSP_USE_RTC
static void modbus_rtu_timer_callback(int device, unsigned match, int *stop)
{
    *stop = 0;
    mb_rtu_timer_count++;           /* Indicate that we had activities on this interrupt. */
    modbus_rtu_timer_update();      /* Check for RTU timers that have expired */
}

#elif defined(BSP_USE_PWM0) || defined(BSP_USE_PWM1) || defined(BSP_USE_PWM2) || defined(BSP_USE_PWM3)

static void modbus_rtu_timer_callback(void *pwm, int *stopit)
{
    *stopit = 0;
    mb_rtu_timer_count++;
    modbus_rtu_timer_update();
}
#endif

/**************************************************************************************************
 * function:    modbus_rtu_timer_init()
 * Description: This function is called to initialize the RTU timeout timer.
 * Argument(s): freq          Is the frequency of the modbus RTU timer interrupt.
 * Return(s):   none.
 *
 * Caller(s):   modbus_init().
 * Note(s):     none.
 **************************************************************************************************/

void modbus_rtu_timer_init(void)
{
    if (mb_rtu_frequency == 0)
        mb_rtu_frequency = 100;                         /* 100HZ = 10ms */

#if defined(LS1B)

    rtc_cfg_t cfg;

    memset((void *)&cfg, 0, sizeof(rtc_cfg_t));
    cfg.interval_ms = 1000 / mb_rtu_frequency;          /* TODO 这个参数 */
    if (cfg.interval_ms == 0)
        cfg.interval_ms = 1;

    cfg.cb = modbus_rtu_timer_callback;                 /* called by match-isr */

    ls1x_rtc_timer_start(DEVICE_RTCMATCH0, &cfg);       // DEVICE_RTCMATCH0 用作定时器

#elif defined(LS1C)

    pwm_cfg_t cfg;

    memset((void *)&cfg, 0, sizeof(pwm_cfg_t));
    cfg.hi_ns = (1000 / mb_rtu_frequency) * 1000000;    /* ns */
    if (cfg.hi_ns == 0)
        cfg.hi_ns = 10000000;

    cfg.cb = modbus_rtu_timer_callback;
    cfg.mode = PWM_CONTINUE_TIMER;

    ls1x_pwm_timer_start(devPWM0, &cfg);                // PWM0 用作定时器

#endif

    /*
     * Reset all the RTU timers.
     */
    modbus_rtu_timer_reset_all();
}

/**************************************************************************************************
 * function:    modbus_rtu_timer_exit()
 * Description: This function is called to disable the RTU timeout timer.
 * Argument(s): none.
 * Return(s):   none.
 *
 * Caller(s):   modbus_exit()
 * Note(s):     none.
 **************************************************************************************************/

void modbus_rtu_timer_exit(void)
{
#if defined(LS1B)
    ls1x_rtc_timer_stop(DEVICE_RTCMATCH0);
#elif defined(LS1C)
    ls1x_pwm_timer_stop(devPWM0);
#endif
}

#endif // #if (MODBUS_CFG_RTU_EN == 1)

//-----------------------------------------------------------------------------
/*
 * @@ END
 */

