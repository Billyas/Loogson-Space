/**************************************************************************************************
 * MODBUS Header File
 *
 * Filename: mb.h
 * Version:
 **************************************************************************************************/

/*
 * MODULE
 *
 * Note(s): This main modbus header file is protected from multiple pre-processor inclusion
 *          through use of the modbus module present pre-processor macro definition.
 */

#ifndef MODBUS_MODULE_H          /* See Note #1. */
#define MODBUS_MODULE_H

/**************************************************************************************************
 * MODBUS VERSION NUMBER
 *
 * Note(s): (a) The Modbus software version is denoted as follows:
 *              Vx.yy.zz
 *              where
 *                V     denotes 'Version' label
 *                x     denotes major software version revision number
 *                yy    denotes minor software version revision number
 *                zz    minor revision
 *
 *          (b) The software version label #define is formatted as follows:
 *              ver = x.yy.zz * 10000
 *              where
 *                ver     denotes software version number scaled as an integer value
 *                x.yy.zz denotes software version number
 **************************************************************************************************/

#define MODBUS_VERSION		21400u   	/* See Note #1. */

/**************************************************************************************************
 * EXTERNS
 **************************************************************************************************/

/**************************************************************************************************
 * MODBUS PROTOCOL INCLUDE FILES
 *
 * Note(s): (1) The modbus protocol suite files are located in the following directories:
 *              (a) (1) \<Your Product Application>\app_cfg.h
 *                  (2)                            \mb_cfg.h
 *                  (3)                            \mb_data.c
 *              (b) \<Modbus Protocol Suite>\Source\mb.h
 *                                                 \mb.c
 *                                                 \mb_def.c
 *                                                 \mb_util.c
 *                                                 \mbm_core.c
 *                                                 \mbs_core.c
 *              (c) \<Modbus Protocol Suite>\Ports\<cpu>\mb_bsp.*
 *              (d) \<Modbus Protocol Suite>\OS\<os>\mb_os.*
 *                  where
 *                    <Your Product Application> directory path for Your Product's Application
 *                    <Modbus Protocol Suite>    directory path for modbus protocol suite
 *                    <cpu>                      directory name for specific processor        (CPU)
 *                    <compiler>                 directory name for specific compiler
 *                    <os>                       directory name for specific operating system (OS)
 *
 *          (2) CPU-configuration software files are located in the following directories:
 *              (a) \<CPU-Compiler Directory>\cpu_def.h
 *              (b) \<CPU-Compiler Directory>\<cpu>\<compiler>\cpu*.*
 *                  where
 *                    <CPU-Compiler Directory>   directory path for common   CPU-compiler software
 *                    <cpu>                      directory name for specific processor (CPU)
 *                    <compiler>                 directory name for specific compiler
 *
 *          (3) NO compiler-supplied standard library functions are used by the modbus protocol suite.
 *              (a) Standard library functions are implemented in the custom library module(s):
 *                  \<Custom Library Directory>\lib*.*
 *                  where
 *                    <Custom Library Directory> directory path for custom library software
 *
 *          (4) Compiler MUST be configured to include the '\<Custom Library Directory>\uC-LIB\',
 *              '\<CPU-Compiler Directory>\' directory, & the specific CPU-compiler directory as
 *              additional include path directories.
 **************************************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "../app/mb_cfg.h"
#include "mb_def.h"

/**************************************************************************************************
 * DATA TYPES
 **************************************************************************************************/

typedef struct modbus_
{
    uint8_t    Channel;                             /* Channel number */
    bool       WrEn;                                /* Indicates whether MODBUS writes are enabled for the channel */
    uint32_t   WrCtr;                               /* Incremented each time a write command is performed */

    uint8_t    NodeAddr;                            /* Modbus node address of the channel */
    void      *PtrUART;                             /* UART is as NS16550_t* */

    uint32_t   BaudRate;                            /* Baud Rate */
    uint8_t    Parity;                              /* UART's parity settings (MODBUS_PARITY_NONE, _ODD or _EVEN) */
    uint8_t    Bits;                                /* UART's number of bits (7 or 8) */
    uint8_t    Stops;                               /* UART's number of stop bits (1 or 2) */

    uint8_t    Mode;                                /* Modbus mode: MODBUS_MODE_ASCII or MODBUS_MODE_RTU */
    uint8_t    MasterSlave;                         /* Slave when set to MODBUS_SLAVE, Master when set to MODBUS_MASTER */

    uint16_t   Err;                                 /* Internal code to indicate the source of MBS_ErrRespSet() */

#if (MODBUS_CFG_RTU_EN == 1)
    uint16_t   RTU_TimeoutCnts;                     /* Counts to reload in .RTU_TimeoutCtr when byte received */
    uint16_t   RTU_TimeoutCtr;                      /* Counts left before RTU timer times out for the channel */
    bool       RTU_TimeoutEn;                       /* Enable (when TRUE) or Disable (when FALSE) RTU timer */
#endif

#if (MODBUS_CFG_FC08_EN == 1)
    uint16_t   StatMsgCtr;                          /* Statistics */
    uint16_t   StatCRCErrCtr;
    uint16_t   StatExceptCtr;
    uint16_t   StatSlaveMsgCtr;
    uint16_t   StatNoRespCtr;
#endif

    uint32_t   RxTimeout;                           /* Amount of time Master is willing to wait for response from slave */
    volatile int RxDoneFlag;                        /* Flag the one transmit done */
    
    uint32_t   RxCtr;                               /* Incremented every time a character is received */
    uint16_t   RxBufByteCtr;                        /* Number of bytes received or to send */
    uint8_t   *RxBufPtr;                            /* Pointer to current position in buffer */
    uint8_t    RxBuf[MODBUS_CFG_BUF_SIZE];          /* Storage of received characters or characters to send */

    uint32_t   TxCtr;                               /* Incremented every time a character is transmitted */
    uint16_t   TxBufByteCtr;                        /* Number of bytes received or to send */
    uint8_t   *TxBufPtr;                            /* Pointer to current position in buffer */
    uint8_t    TxBuf[MODBUS_CFG_BUF_SIZE];          /* Storage of received characters or characters to send */

    uint8_t    RxFrameData[MODBUS_CFG_BUF_SIZE];    /* Additional data for function requested. */
    uint16_t   RxFrameNDataBytes;                   /* Number of bytes in the data field. */
    uint16_t   RxFrameCRC;                          /* Error check value (LRC or CRC-16). */
    uint16_t   RxFrameCRC_Calc;                     /* Error check value computed from packet received */

    uint8_t    TxFrameData[MODBUS_CFG_BUF_SIZE];    /* Additional data for function requested. */
    uint16_t   TxFrameNDataBytes;                   /* Number of bytes in the data field. */
    uint16_t   TxFrameCRC;                          /* Error check value (LRC or CRC-16). */
} MODBUS_t;

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

#if (MODBUS_CFG_RTU_EN == 1)
extern uint16_t mb_rtu_frequency;                       /* Frequency at which RTU timer is running */
extern uint32_t mb_rtu_timer_count;                     /* Incremented every Modbus RTU timer interrupt */
#endif

extern uint8_t  mb_devices_count;                       /* Modbus channel counter (0..MODBUS_MAX_CH) */
extern MODBUS_t mb_devices_tbl[MODBUS_CFG_CHNL_MAX];    /* Modbus channels */

/**************************************************************************************************
 * MODBUS INTERFACE FUNCTION PROTOTYPES
 * (MB.C)
 **************************************************************************************************/

void modbus_init(uint32_t freq);

void modbus_exit(void);

MODBUS_t *modbus_config_node(uint8_t  node_addr,
                             uint8_t  master_slave,
                             uint32_t rx_timeout,
                             uint8_t  modbus_mode,
                             void    *uart_device,  // uint8_t  port_nbr,
                             uint32_t baud,
                             uint8_t  bits,
                             uint8_t  parity,
                             uint8_t  stops,
                             uint8_t  wr_en);

void modbus_master_timeout_set(MODBUS_t *p_mb, uint32_t timeout);
void modbus_set_mode(MODBUS_t *p_mb, uint8_t master_slave, uint8_t mode);
void modbus_set_address(MODBUS_t *p_mb, uint8_t addr);
void modbus_set_write_enable(MODBUS_t *p_mb, uint8_t wr_en);
void modbus_set_channel_device(MODBUS_t *p_mb, void *device);

#if (MODBUS_CFG_ASCII_EN == 1)
void modbus_ascii_rx_byte(MODBUS_t *p_mb, uint8_t rx_byte);
#endif

#if (MODBUS_CFG_RTU_EN == 1)
void modbus_rtu_rx_byte(MODBUS_t *p_mb, uint8_t rx_byte);
void modbus_rtu_timer_reset(MODBUS_t *p_mb);                /* Resets the Frame Sync timer. */
void modbus_rtu_timer_reset_all(void);                      /* Resets all the RTU timers */
void modbus_rtu_timer_update(void);
#endif

void modbus_rx_byte(MODBUS_t *p_mb, uint8_t rx_byte);
void modbus_rx_task(MODBUS_t *p_mb);
int modbus_rx_1byte(MODBUS_t *p_mb, uint8_t *rx_byte, int timeout);

void modbus_tx(MODBUS_t *p_mb);
//void modbus_tx_byte(MODBUS_t *p_mb);
void modbus_tx_bytes(MODBUS_t *p_mb);

#if (MODBUS_CFG_ASCII_EN == 1)
bool modbus_ascii_rx(MODBUS_t *p_mb);
void modbus_ascii_tx(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_RTU_EN == 1)
bool modbus_rtu_rx(MODBUS_t *p_mb);
void modbus_rtu_tx(MODBUS_t *p_mb);
#endif

/**************************************************************************************************
 * RTOS INTERFACE FUNCTION PROTOTYPES
 * (defined in mb_os.c)
 **************************************************************************************************/

void modbus_os_init(void);
void modbus_os_exit(void);
void modbus_os_rx_signal(MODBUS_t *p_mb);
void modbus_os_rx_wait(MODBUS_t *p_mb, uint16_t *perr);

/**************************************************************************************************
 * COMMON MODBUS ASCII INTERFACE FUNCTION PROTOTYPES
 * (defined in mb_util.c)
 **************************************************************************************************/

#if (MODBUS_CFG_ASCII_EN == 1)
uint8_t *modbus_ascii_bin_to_hex(uint8_t value, uint8_t *pbuf);
uint8_t modbus_ascii_hex_to_bin(uint8_t *phex);
uint8_t modbus_ascii_rx_calculate_lrc(MODBUS_t *p_mb);
uint8_t modbus_ascii_tx_calculate_lrc(MODBUS_t *p_mb, uint16_t tx_bytes);
#endif

/**************************************************************************************************
 * COMMON MODBUS RTU INTERFACE FUNCTION PROTOTYPES
 * (defined in mb_util.C)
 **************************************************************************************************/

#if (MODBUS_CFG_RTU_EN == 1)
uint16_t modbus_rtu_tx_calculate_crc(MODBUS_t *p_mb);
uint16_t modbus_rtu_rx_calculate_crc(MODBUS_t *p_mb);
#endif

/**************************************************************************************************
 * INTERFACE TO APPLICATION DATA
 * (defined in mb_data.C)
 **************************************************************************************************/

#if (MODBUS_CFG_FC01_EN == 1)
bool modbus_read_coil(uint16_t coil, uint16_t *perr);
#endif

#if (MODBUS_CFG_FC05_EN == 1)
void modbus_write_coil(uint16_t coil, bool coil_val, uint16_t *perr);
#endif

#if (MODBUS_CFG_FC02_EN == 1)
bool modbus_read_di(uint16_t di, uint16_t *perr);
#endif

#if (MODBUS_CFG_FC04_EN == 1)
uint16_t modbus_read_in_register(uint16_t reg, uint16_t *perr);
float modbus_read_in_register_float(uint16_t reg, uint16_t *perr);
#endif

#if (MODBUS_CFG_FC03_EN == 1)
uint16_t modbus_read_holding_register(uint16_t reg, uint16_t *perr);
float modbus_read_holding_register_float(uint16_t reg, uint16_t  *perr);
#endif

#if (MODBUS_CFG_FC06_EN == 1) || (MODBUS_CFG_FC16_EN == 1)
void modbus_write_holding_register(uint16_t reg, uint16_t reg_val_16, uint16_t *perr);
void modbus_write_holding_register_float(uint16_t reg, float reg_val_fp, uint16_t *perr);
#endif

#if (MODBUS_CFG_FC20_EN == 1)
uint16_t modbus_read_file(uint16_t file_nbr, uint16_t record_nbr, uint16_t ix,
                          uint8_t record_len, uint16_t *perr);
#endif

#if (MODBUS_CFG_FC21_EN == 1)
void modbus_write_file(uint16_t file_nbr, uint16_t record_nbr, uint16_t ix,
                       uint8_t record_len, uint16_t value, uint16_t *perr);
#endif

/**************************************************************************************************
 * BSP FUNCTION PROTOTYPES
 **************************************************************************************************/

void modbus_hw_port_exit(void);                     /* Exit Modbus Communications */

void modbus_hw_port_config(MODBUS_t *p_mb,
                           uint32_t  baud,
                           uint8_t   bits,
                           uint8_t   parity,
                           uint8_t   stops);

void modbus_rx_irq_enable(MODBUS_t *p_mb);          /* Enable  Rx interrupts */
void modbus_rx_irq_disable(MODBUS_t *p_mb);         /* Disable Rx interrupts */
void modbus_tx_1byte(MODBUS_t *p_mb, uint8_t c);
void modbus_tx_irq_enable(MODBUS_t *p_mb);          /* Enable  Tx interrupts */
void modbus_tx_irq_disable(MODBUS_t *p_mb);         /* Disable Tx interrupts */

#if (MODBUS_CFG_RTU_EN == 1)
void modbus_rtu_timer_init(void);                   /* Initialize the timer used for RTU framing */
void modbus_rtu_timer_exit(void);
#endif

/**************************************************************************************************
 * MODBUS SLAVE
 * GLOBAL FUNCTION PROTOTYPES
 **************************************************************************************************/

#if (MODBUS_CFG_SLAVE_EN == 1)
bool modbus_slave_fcxx_handler(MODBUS_t *p_mb);
void modbus_slave_rx_task(MODBUS_t *p_mb);
#if (MODBUS_CFG_FC08_EN == 1)
void modbus_slave_stat_init(MODBUS_t *p_mb);
#endif
#endif

/**************************************************************************************************
 * MODBUS MASTER
 * GLOBAL FUNCTION PROTOTYPES
 **************************************************************************************************/

#if (MODBUS_CFG_MASTER_EN == 1)

#if (MODBUS_CFG_FC01_EN == 1)
uint16_t modbus_master_fc01_read_coil(MODBUS_t *p_mb,
                                      uint8_t   slave_addr,
                                      uint16_t  start_addr,
                                      uint8_t  *p_coil_tbl,
                                      uint16_t  nbr_coils);
#endif

#if (MODBUS_CFG_FC02_EN == 1)
uint16_t modbus_master_fc02_read_di(MODBUS_t *p_mb,
                                    uint8_t   slave_node,
                                    uint16_t  slave_addr,
                                    uint8_t  *p_di_tbl,
                                    uint16_t  nbr_di);
#endif

#if (MODBUS_CFG_FC03_EN == 1)
uint16_t modbus_master_fc03_read_holding_register(MODBUS_t *p_mb,
                                                  uint8_t   slave_node,
                                                  uint16_t  slave_addr,
                                                  uint16_t *p_reg_tbl,
                                                  uint16_t  nbr_regs);
#endif

#if (MODBUS_CFG_FC03_EN == 1) && (MODBUS_CFG_FP_EN == 1)
uint16_t modbus_master_fc03_read_holding_register_float(MODBUS_t *p_mb,
                                                        uint8_t   slave_node,
                                                        uint16_t  slave_addr,
                                                        float    *p_reg_tbl,
                                                        uint16_t  nbr_regs);
#endif

#if (MODBUS_CFG_FC04_EN == 1)
uint16_t modbus_master_fc04_read_in_register(MODBUS_t *p_mb,
                                             uint8_t   slave_node,
                                             uint16_t  slave_addr,
                                             uint16_t *p_reg_tbl,
                                             uint16_t  nbr_regs);
#endif

#if (MODBUS_CFG_FC05_EN == 1)
uint16_t modbus_master_fc05_write_coil(MODBUS_t *p_mb,
                                       uint8_t   slave_node,
                                       uint16_t  slave_addr,
                                       bool      coil_val);
#endif

#if (MODBUS_CFG_FC06_EN == 1)
uint16_t modbus_master_fc06_write_holding_register(MODBUS_t *p_mb,
                                                   uint8_t   slave_node,
                                                   uint16_t  slave_addr,
                                                   uint16_t  reg_val);
#endif

#if (MODBUS_CFG_FC06_EN == 1) && (MODBUS_CFG_FP_EN == 1)
uint16_t modbus_master_fc06_write_holding_register_float(MODBUS_t *p_mb,
                                                         uint8_t   slave_node,
                                                         uint16_t  slave_addr,
                                                         float     reg_val_fp);
#endif

#if (MODBUS_CFG_FC08_EN == 1)
uint16_t modbus_master_fc08_diag(MODBUS_t *p_mb,
                                 uint8_t   slave_node,
                                 uint16_t  fnct,
                                 uint16_t  fnct_data,
                                 uint16_t  *pval);
#endif

#if (MODBUS_CFG_FC15_EN == 1)
uint16_t modbus_master_fc15_write_coil(MODBUS_t *p_mb,
                                       uint8_t   slave_node,
                                       uint16_t  slave_addr,
                                       uint8_t  *p_coil_tbl,
                                       uint16_t  nbr_coils);
#endif

#if (MODBUS_CFG_FC16_EN == 1)
uint16_t modbus_master_fc16_write_holding_register_number(MODBUS_t *p_mb,
                                                          uint8_t   slave_node,
                                                          uint16_t  slave_addr,
                                                          uint16_t *p_reg_tbl,
                                                          uint16_t  nbr_regs);
#endif

#if (MODBUS_CFG_FC16_EN == 1) && (MODBUS_CFG_FP_EN   == 1)
uint16_t modbus_master_fc16_write_holding_register_number_float(MODBUS_t *p_mb,
                                                                uint8_t   slave_node,
                                                                uint16_t  slave_addr,
                                                                float    *p_reg_tbl,
                                                                uint16_t  nbr_regs);
#endif

#endif

/**************************************************************************************************
 * SYMBOL ERRORS
 **************************************************************************************************/

#ifndef MODBUS_CFG_MASTER_EN
#error "MODBUS_CFG_MASTER_EN        not #defined"
#error "... Defines wheteher your product will support Modbus Master"
#endif

#ifndef MODBUS_CFG_SLAVE_EN
#error "MODBUS_CFG_SLAVE_EN         not #defined"
#error "... Defines wheteher your product will support Modbus Slave"
#endif

#ifndef MODBUS_CFG_CHNL_MAX
#error "MODBUS_CFG_CHNL_MAX         not #defined"
#error "... Defines the number of Modbus ports supported.  Should be 1 to N."
#endif

#ifndef MODBUS_CFG_ASCII_EN
#error "MODBUS_CFG_ASCII_EN         not #defined "
#error "... Defines whether your product will support Modbus ASCII. "
#endif

#ifndef MODBUS_CFG_RTU_EN
#error "MODBUS_CFG_RTU_EN           not #defined "
#error "... Defines whether your product will support Modbus RTU. "
#endif

#ifndef MODBUS_CFG_FP_EN
#error "MODBUS_CFG_FP_EN            not #defined "
#error "... Defines whether your product will support Daniels Flow Meter Floating-Point extensions. "
#endif

#ifndef MODBUS_CFG_FP_START_IX
#error "MODBUS_CFG_FP_START_IX      not #defined "
#error "... Defines the starting register number for floating-point registers. "
#endif

#ifndef MODBUS_CFG_FC01_EN
#error "MODBUS_CFG_FC01_EN          not #defined "
#endif

#ifndef MODBUS_CFG_FC02_EN
#error "MODBUS_CFG_FC02_EN          not #defined "
#endif

#ifndef MODBUS_CFG_FC03_EN
#error "MODBUS_CFG_FC03_EN          not #defined "
#endif

#ifndef MODBUS_CFG_FC04_EN
#error "MODBUS_CFG_FC04_EN          not #defined "
#endif

#ifndef MODBUS_CFG_FC05_EN
#error "MODBUS_CFG_FC05_EN          not #defined "
#endif

#ifndef MODBUS_CFG_FC06_EN
#error "MODBUS_CFG_FC06_EN          not #defined "
#endif

#ifndef MODBUS_CFG_FC08_EN
#error "MODBUS_CFG_FC08_EN          not #defined "
#endif

#ifndef MODBUS_CFG_FC15_EN
#error "MODBUS_CFG_FC15_EN          not #defined "
#endif

#ifndef MODBUS_CFG_FC16_EN
#error "MODBUS_CFG_FC16_EN          not #defined "
#endif

#ifndef MODBUS_CFG_FC20_EN
#error "MODBUS_CFG_FC20_EN          not #defined "
#endif

#ifndef MODBUS_CFG_FC21_EN
#error "MODBUS_CFG_FC21_EN          not #defined "
#endif


/**************************************************************************************************
 * MODULE END
 **************************************************************************************************/

#endif  /* End of modbus module include. */

