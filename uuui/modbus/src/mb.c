/**************************************************************************************************
 * MODBUS Source Code
 *
 * Filename: mb.c
 * Version:
 **************************************************************************************************/

/**************************************************************************************************
 * INCLUDE FILES
 **************************************************************************************************/

#if MODBUS_DEBUG
#include <stdio.h>
#endif

#include "mb.h"

/**************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 **************************************************************************************************/

/**************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/**************************************************************************************************
 * function:    modbus_init()
 * Description: Handle either Modbus ASCII or Modbus RTU received packets.
 * Argument(s): freq       Specifies the Modbus RTU timer frequency (in Hz)
 * Return(s):   none.
 *
 * Caller(s):   Application
 * Note(s):     none.
 **************************************************************************************************/

static int is_modbus_initialized = 0;

void modbus_init(uint32_t freq)
{
    uint8_t   i;
    MODBUS_t *p_mb;

    if (is_modbus_initialized)
        return 0;

#if (MODBUS_CFG_RTU_EN == 1)
    mb_rtu_frequency = freq;                            /* Save the RTU frequency */
#endif

    p_mb = &mb_devices_tbl[0];                          /* Save Modbus channel number in data structure */
    for (i = 0; i < MODBUS_CFG_CHNL_MAX; i++)
    {
    	/* Initialize default values
    	 */
    	p_mb->Channel       = i;
    	p_mb->NodeAddr      = 1;
    	p_mb->MasterSlave   = MODBUS_SLAVE;             /* Channel defaults to MODBUS_SLAVE mode */
    	p_mb->Mode          = MODBUS_MODE_ASCII;
    	p_mb->RxBufByteCtr  = 0;
    	p_mb->RxBufPtr      = &p_mb->RxBuf[0];
    	p_mb->WrEn          = MODBUS_WR_EN;
    	p_mb->WrCtr         = 0;
#if (MODBUS_CFG_RTU_EN == 1)
    	p_mb->RTU_TimeoutEn = true;
#endif

#if (MODBUS_CFG_SLAVE_EN == 1) && (MODBUS_CFG_FC08_EN  == 1)
        modbus_slave_stat_init(p_mb);
#endif
        p_mb++;
    }

    mb_devices_count = 0;

    modbus_os_init();                	    /* Initialize OS interface functions */

#if (MODBUS_CFG_RTU_EN == 1)         	    /* MODBUS 'RTU' Initialization */
    modbus_rtu_timer_init();
#else
    (void)&freq;
#endif

    is_modbus_initialized = 1;
}

/**************************************************************************************************
 * function:    modbus_exit()
 * Description: This function is called to terminate all Modbus communications
 * Argument(s): none.
 * Return(s):   none.
 *
 * Caller(s):   Application.
 * Note(s):     none.
 **************************************************************************************************/

void modbus_exit(void)
{
#if (MODBUS_CFG_RTU_EN == 1)
    modbus_rtu_timer_exit();          	    /* Stop the RTU timer interrupts */
#endif

    modbus_hw_port_exit();                  /* Disable all communications */

    modbus_os_exit();                 	    /* Stop RTOS services */
}

/**************************************************************************************************
 * function:    modbus_config_node()
 * Description: This function must be called after calling modbus_init() to initialize each of
 *              the Modbus channels in your system.
 * Argument(s): node_addr     is the Modbus node address that the channel is assigned to.
 *              master_slave  specifies whether the channel is a MODBUS_MASTER or a MODBUS_SLAVE
 *              rx_timeout    amount of time Master will wait for a response from the slave.
 *              modbus_mode   specifies the type of modbus channel.  The choices are:
 *                            MODBUS_MODE_ASCII
 *                            MODBUS_MODE_RTU
 *              port_nbr      is the UART port number associated with the channel
 *              baud          is the desired baud rate
 *              parity        is the UART's parity setting:
 *                            MODBUS_PARITY_NONE
 *                            MODBUS_PARITY_ODD
 *                            MODBUS_PARITY_EVEN
 *              bits          UART's number of bits (7 or 8)
 *              stops         Number of stops bits (1 or 2)
 *              wr_en         This argument determines whether a Modbus WRITE request will be
 *                            accepted. The choices are:
 *                            MODBUS_WR_EN
 *                            MODBUS_WR_DIS
 * Return(s):   none.
 *
 * Caller(s):   Application.
 * Note(s):     none.
 **************************************************************************************************/

MODBUS_t *modbus_config_node(uint8_t  node_addr,
                             uint8_t  master_slave,
                             uint32_t rx_timeout,
                             uint8_t  modbus_mode,
                             void    *uart_device, // uint8_t  port_nbr,
                             uint32_t baud,
                             uint8_t  bits,
                             uint8_t  parity,
                             uint8_t  stops,
                             uint8_t  wr_en)
{
    MODBUS_t *p_mb;
#if (MODBUS_CFG_RTU_EN == 1)
    uint16_t cnts;
#endif

    if (mb_devices_count < MODBUS_CFG_CHNL_MAX)
    {
    	p_mb = &mb_devices_tbl[mb_devices_count];               // The last channel
        modbus_master_timeout_set(p_mb, rx_timeout);
        modbus_set_address(p_mb, node_addr);
        modbus_set_mode(p_mb, master_slave, modbus_mode);
        modbus_set_write_enable(p_mb, wr_en);
        modbus_set_channel_device(p_mb, uart_device);           // 1st: set UART device
        modbus_hw_port_config(p_mb, baud, bits, parity, stops); // 2nd: set uart by PortNbr

#if (MODBUS_CFG_RTU_EN == 1)
        if (p_mb->MasterSlave == MODBUS_MASTER)
        {
        	p_mb->RTU_TimeoutEn = false;
        }

        /*
         * freq * 5(char) * 10 (bits/char) / BaudRate <== 8N1
         */
        cnts = ((uint32_t)mb_rtu_frequency * 5L * 10L) / baud;
        if (cnts <= 1)
        {
            cnts = 2;
        }

        p_mb->RTU_TimeoutCnts = cnts;
        p_mb->RTU_TimeoutCtr  = cnts;
        
#endif
        mb_devices_count++;
        return p_mb;
    }
    else
    {
        return (MODBUS_t *)0;
    }
}

/**************************************************************************************************
 * function:    modbus_master_timeout_set()
 * Description: This function is called to change the operating mode of a Modbus channel.
 * Argument(s): p_mb        is a pointer to the Modbus channel to change
 *              timeout     amount of time Master will wait for a response from the slave.
 * Return(s):   none.
 *
 * Caller(s):   Application.
 * Note(s):     none.
 **************************************************************************************************/

void modbus_master_timeout_set(MODBUS_t *p_mb, uint32_t timeout)
{
    if (p_mb != (MODBUS_t *)0)
    {
    	p_mb->RxTimeout = timeout;
    }
}

/**************************************************************************************************
 * function:    modbus_set_mode()
 * Description: This function is called to change the operating mode of a Modbus channel.
 * Argument(s): p_mb         is a pointer to the Modbus channel to change
 *              modbus_mode  specifies the type of modbus channel.  The choices are:
 *                           MODBUS_MODE_ASCII
 *                           MODBUS_MODE_RTU
 * Return(s):   none.
 *
 * Caller(s):   Application.
 * Note(s):     none.
 **************************************************************************************************/

void modbus_set_mode(MODBUS_t *p_mb, uint8_t master_slave, uint8_t mode)
{
    if (p_mb != (MODBUS_t *)0)
    {
        switch (master_slave)
        {
            case MODBUS_MASTER:
            	p_mb->MasterSlave = MODBUS_MASTER;
                break;

            case MODBUS_SLAVE:
            default:
            	p_mb->MasterSlave = MODBUS_SLAVE;
                break;
        }

        switch (mode)
        {
		#if (MODBUS_CFG_ASCII_EN == 1)
            case MODBUS_MODE_ASCII:
            	p_mb->Mode = MODBUS_MODE_ASCII;
                break;
		#endif

		#if (MODBUS_CFG_RTU_EN == 1)
            case MODBUS_MODE_RTU:
            	p_mb->Mode = MODBUS_MODE_RTU;
                break;
		#endif

            default:
		#if (MODBUS_CFG_RTU_EN == 1)
            	p_mb->Mode = MODBUS_MODE_RTU;
		#else
                p_mb->Mode = MODBUS_MODE_ASCII;
		#endif
                break;
        }
    }
}

/**************************************************************************************************
 * function:    modbus_set_address()
 * Description: This function is called to change the Modbus node address that the channel
 *              will respond to.
 * Argument(s): p_mb        is a pointer to the Modbus channel to change
 *              node_addr   is the Modbus node address that the channel is assigned to.
 * Return(s):   none.
 *
 * Caller(s):   Application.
 * Note(s):     none.
 **************************************************************************************************/

void modbus_set_address(MODBUS_t *p_mb, uint8_t node_addr)
{
    if (p_mb != (MODBUS_t *)0)
    {
    	p_mb->NodeAddr = node_addr;
    }
}

/**************************************************************************************************
 * function:    modbus_set_write_enable()
 * Description: This function is called to enable or disable write accesses to the data.
 * Argument(s): p_mb    is the Modbus channel to change
 *              wr_en   This argument determines whether a Modbus WRITE request will be accepted.
 *                      The choices are:
 *                      MODBUS_WR_EN
 *                      MODBUS_WR_DIS
 * Return(s):   none.
 *
 * Caller(s):   Application.
 * Note(s):     none.
 **************************************************************************************************/

void modbus_set_write_enable(MODBUS_t *p_mb, uint8_t wr_en)
{
    if (p_mb != (MODBUS_t *)0)
    {
    	p_mb->WrEn = wr_en;
    }
}

/**************************************************************************************************
 * function:    modbus_set_channel_device()
 * Description: This function is called to change the physical port number of the Modbus channel.
 * Argument(s): p_mb        is a pointer to the Modbus channel to change
 *              port_nbr    This argument determines the physical port number of the Modbus channel
 * Return(s):   none.
 *
 * Caller(s):   Application.
 * Note(s):     none.
 **************************************************************************************************/

void modbus_set_channel_device(MODBUS_t *p_mb, void *device)
{
    if (p_mb != (MODBUS_t *)0)
    {
    	p_mb->PtrUART = device;
    }
}

/**************************************************************************************************
 * function:    modbus_rx_byte()
 * Description: A byte has been received from a serial port.  We just store it in the buffer for
 *              processing when a complete packet has been received.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 *              rx_byte     Is the byte received.
 * Return(s):   none.
 *
 * Caller(s):   modbus_rx_tx_irq_handler().
 * Note(s):     none.
 **************************************************************************************************/

void modbus_rx_byte(MODBUS_t *p_mb, uint8_t rx_byte)
{
    switch (p_mb->Mode)
    {
	#if (MODBUS_CFG_ASCII_EN == 1)
        case MODBUS_MODE_ASCII:
             modbus_ascii_rx_byte(p_mb, rx_byte & 0x7F);
             break;
	#endif

	#if (MODBUS_CFG_RTU_EN == 1)
        case MODBUS_MODE_RTU:
             modbus_rtu_rx_byte(p_mb, rx_byte);
             break;
	#endif

        default:
             break;
    }
}

/**************************************************************************************************
 * function:    modbus_rx_task()
 * Description: This function is called when a packet needs to be processed.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   none.
 *
 * Caller(s):   modbus_os_rx_task().
 * Note(s):     none.
 **************************************************************************************************/

void modbus_rx_task(MODBUS_t *p_mb)
{
#if (MODBUS_CFG_SLAVE_EN == 1)
    if (p_mb != (MODBUS_t *)0)
    {
        if (p_mb->MasterSlave == MODBUS_SLAVE)
        {
            modbus_slave_rx_task(p_mb);
        }
    }
#endif
}

/**************************************************************************************************
 * function:    modbus_tx()
 * Description: This function is called to start transmitting a packet to a modbus channel.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   none.
 *
 * Caller(s):   modbus_ascii_tx(),
 *              modbus_rtu_tx().
 * Note(s):     none.
 **************************************************************************************************/

void modbus_tx(MODBUS_t *p_mb)
{
	p_mb->TxBufPtr = &p_mb->TxBuf[0];
    modbus_tx_bytes(p_mb);
}

/**************************************************************************************************
 * function:    modbus_ascii_rx_byte()
 * Description: A byte has been received from a serial port.  We just store it in the buffer for
 *              processing when a complete packet has been received.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 *              rx_byte     Is the byte received.
 * Return(s):   none.
 *
 * Caller(s):   modbus_rx_byte().
 * Return(s):   none.
 **************************************************************************************************/

#if (MODBUS_CFG_ASCII_EN == 1)
void modbus_ascii_rx_byte(MODBUS_t *p_mb, uint8_t rx_byte)
{
    uint8_t *phex, node_addr;

    p_mb->RxCtr++;                                      /* Increment the number of bytes received */
    if (rx_byte == ':')                                 /* Is it the start of frame character? */
    {         
    	p_mb->RxBufPtr     = &p_mb->RxBuf[0];           /* Yes, Restart a new frame */
    	p_mb->RxBufByteCtr = 0;
    }

    if (p_mb->RxBufByteCtr < MODBUS_CFG_BUF_SIZE)       /* No, add received byte to buffer */
    {
        *p_mb->RxBufPtr++  = rx_byte;
        p_mb->RxBufByteCtr++;                           /* Increment byte counter to see if we have Rx ... */
                                                        /* ... activity */
    }

    if (rx_byte == MODBUS_ASCII_END_FRAME_CHAR2)        /* See if we received a complete ASCII frame */
    {
        phex      = &p_mb->RxBuf[1];
        node_addr = modbus_ascii_hex_to_bin(phex);

        /* Is the address for us? ... or a 'broadcast'?
         */
        if ((node_addr == p_mb->NodeAddr) || (node_addr == 0))
        {
            modbus_os_rx_signal(p_mb);                  /* Yes, Let task handle reply */
        }
        else
        {
        	p_mb->RxBufPtr     = &p_mb->RxBuf[0];       /* No,  Wipe out anything, we have to re-synchronize. */
        	p_mb->RxBufByteCtr = 0;
        }
    }
}
#endif

/**************************************************************************************************
 * function:    modbus_ascii_rx()
 * Description: Parses and converts an ASCII style message into a Modbus frame.  A check is
 *              performed to verify that the Modbus packet is valid.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If all checks pass.
 *              false       If any checks fail.
 *
 * Caller(s):   modbus_master_rx_reply().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_ASCII_EN == 1)
bool modbus_ascii_rx(MODBUS_t *p_mb)
{
    uint8_t *pmsg, *prx_data;
    uint16_t rx_size;

    pmsg     = &p_mb->RxBuf[0];
    rx_size  =  p_mb->RxBufByteCtr;
    prx_data = &p_mb->RxFrameData[0];

    if ((rx_size & 0x01) &&                                     /* Message should have an ODD nbr of bytes. */
        (rx_size  > MODBUS_ASCII_MIN_MSG_SIZE) &&               /* Check if message is long enough */
        (pmsg[0] == MODBUS_ASCII_START_FRAME_CHAR) &&           /* Check the first char. */
        (pmsg[rx_size - 2] == MODBUS_ASCII_END_FRAME_CHAR1) &&  /* Check the last two. */
        (pmsg[rx_size - 1] == MODBUS_ASCII_END_FRAME_CHAR2))
    {
        rx_size -= 3;                                           /* Take away for the ':', CR, and LF */
        pmsg++;                                                 /* Point past the ':' to the address. */
        p_mb->RxFrameNDataBytes = 0;                            /* Get the data from the message */

        while (rx_size > 2)
        {
            *prx_data++ = modbus_ascii_hex_to_bin(pmsg);
            pmsg    += 2;
            rx_size -= 2;
            p_mb->RxFrameNDataBytes++;      		    /* Increment the number of Modbus packets received */
        }

        p_mb->RxFrameNDataBytes -= 2;                   /* Subtract the Address and function code */
        p_mb->RxFrameCRC = (uint16_t)modbus_ascii_hex_to_bin(pmsg);  /* Extract the message's LRC */
        return true;
    }
    else
    {
        return false;
    }
}
#endif

/**************************************************************************************************
 * function:    modbus_ascii_tx()
 * Description: The format of the message is ASCII.  The actual information is taken from the
 *              given MODBUS frame.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   none.
 *
 * Caller(s):   modbus_master_tx_command(),
 *              modbus_slave_ascii_task().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_ASCII_EN == 1)
void modbus_ascii_tx(MODBUS_t *p_mb)
{
    uint8_t *ptx_data, *pbuf, lrc;
    uint16_t i, tx_bytes;

    ptx_data = &p_mb->TxFrameData[0];
    pbuf     = &p_mb->TxBuf[0];
    *pbuf++  = MODBUS_ASCII_START_FRAME_CHAR;               /* Place the start-of-frame character into output buffer */
    pbuf     = modbus_ascii_bin_to_hex(*ptx_data++, pbuf);
    pbuf     = modbus_ascii_bin_to_hex(*ptx_data++, pbuf);
    tx_bytes = 5;
    i        = (uint8_t)p_mb->TxFrameNDataBytes;            /* Transmit the actual data */

    while (i > 0)
    {
        pbuf      = modbus_ascii_bin_to_hex(*ptx_data++, pbuf);
        tx_bytes += 2;
        i--;
    }

    lrc  = modbus_ascii_tx_calculate_lrc(p_mb, tx_bytes);   /* Compute outbound packet LRC */
    pbuf = modbus_ascii_bin_to_hex(lrc, pbuf);              /* Add the LRC checksum in the packet */
    *pbuf++ = MODBUS_ASCII_END_FRAME_CHAR1;                 /* Add 1st end-of-frame character (0x0D) to output buffer */
    *pbuf++ = MODBUS_ASCII_END_FRAME_CHAR2;                 /* Add 2nd end-of-frame character (0x0A) to output buffer */
    tx_bytes += 4;
    p_mb->TxFrameCRC   = (uint16_t)lrc;                     /* Save the computed LRC into the channel */
    p_mb->TxBufByteCtr = tx_bytes;                          /* Update the total number of bytes to send */
    modbus_tx(p_mb);                                        /* Send it out the communication driver. */
}
#endif

/**************************************************************************************************
 * function:    modbus_rtu_rx_byte()
 * Description: A byte has been received from a serial port.  We just store it in the buffer
 *              for processing when a complete packet has been received.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 *              rx_byte     Is the byte received.
 * Return(s):   none.
 *
 * Caller(s):   modbus_rx_byte().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_RTU_EN == 1)
void modbus_rtu_rx_byte(MODBUS_t *p_mb, uint8_t rx_byte)
{
    modbus_rtu_timer_reset(p_mb);                       /* Reset the timeout timer on a new character */

#if (MODBUS_CFG_MASTER_EN == 1)
    if (p_mb->MasterSlave == MODBUS_MASTER)
    {
        p_mb->RTU_TimeoutEn = true;
    }
#endif

    if (p_mb->RxBufByteCtr < MODBUS_CFG_BUF_SIZE)       /* No, add received byte to buffer */
    {
    	p_mb->RxCtr++;                                  /* Increment the number of bytes received */
        *p_mb->RxBufPtr++ = rx_byte;
        p_mb->RxBufByteCtr++;                           /* Increment byte counter to see if we have Rx activity */
    }
}
#endif

/**************************************************************************************************
 * function:    modbus_rtu_rx()
 * Description: Parses a Modbus RTU packet and processes the request if the packet is valid.
 * Argument(s): p_mb    Is a pointer to the Modbus channel's data structure.
 * Return(s):   true    If all checks pass.
 *              false   If any checks fail.
 * Caller(s):   modbus_master_rx_reply(),
 *              modbus_slave_rtu_task().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_RTU_EN == 1)
bool modbus_rtu_rx(MODBUS_t *p_mb)
{
    uint8_t *prx_data, *pmsg;
    uint16_t rx_size, crc;

    pmsg    = &p_mb->RxBuf[0];
    rx_size =  p_mb->RxBufByteCtr;

    if (rx_size >= MODBUS_RTU_MIN_MSG_SIZE)             /* Is the message long enough? */
    {
        if (rx_size <= MODBUS_CFG_BUF_SIZE)
        {
            prx_data    = &p_mb->RxFrameData[0];
            *prx_data++ = *pmsg++;                      /* Transfer the node address */
            rx_size--;

            *prx_data++ = *pmsg++;                      /* Transfer the function code */
            rx_size--;

            p_mb->RxFrameNDataBytes = 0;                /* Transfer the data */
            while (rx_size > 2)
            {
                *prx_data++ = *pmsg++;
                p_mb->RxFrameNDataBytes++;
                rx_size--;
            }

            crc  = (uint16_t)*pmsg++;                   /* Transfer the CRC over. It's LSB first, then MSB. */
            crc += (uint16_t)*pmsg << 8;
            p_mb->RxFrameCRC = crc;

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
#endif

/**************************************************************************************************
 * function:    modbus_rtu_tx()
 * Description: A MODBUS message is formed into a buffer and sent to the appropriate communication
 *              port. The actual reply is taken from the given MODBUS Frame.
 * Argument(s): p_mb    Is a pointer to the Modbus channel's data structure.
 * Return(s):   none.
 *
 * Caller(s):   modbus_master_tx_command(),
 *              modbus_slave_rtu_task().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_RTU_EN == 1)
void modbus_rtu_tx(MODBUS_t *p_mb)
{
    uint8_t *ptx_data, *pbuf, i;
    uint16_t tx_bytes, crc;

    tx_bytes = 0;
    pbuf     = &p_mb->TxBuf[0];                         /* Point to the beginning of the output buffer. */
    ptx_data = &(p_mb->TxFrameData[0]);
    i        = (uint8_t)p_mb->TxFrameNDataBytes + 2;    /* Include the actual data in the buffer */

    while (i > 0)
    {
        *pbuf++ = *ptx_data++;
        tx_bytes++;
        i--;
    }

    crc       = modbus_rtu_tx_calculate_crc(p_mb);
    *pbuf++   = (uint8_t)(crc & 0x00FF);                /* Add in the CRC checksum.  Low byte first! */
    *pbuf     = (uint8_t)(crc >> 8);
    tx_bytes += 2;
    p_mb->TxFrameCRC   = crc;                           /* Save the calculated CRC in the channel */
    p_mb->TxBufByteCtr = tx_bytes;

    modbus_tx(p_mb);                                    /* Send it out the communication driver. */
}
#endif

/**************************************************************************************************
 * function:    modbus_rtu_timer_reset()
 * Description: This function is called when a byte a received and thus, we reset the RTU timeout
 *              timer value indicating that we are not done receiving a complete RTU packet.
 * Argument(s): p_mb    Is a pointer to the Modbus channel's data structure.
 * Return(s):   none.
 *
 * Caller(s):   modbus_rtu_timer_reset_all().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_RTU_EN == 1)
void modbus_rtu_timer_reset(MODBUS_t *p_mb)
{
	p_mb->RTU_TimeoutCtr = p_mb->RTU_TimeoutCnts;
}
#endif

/**************************************************************************************************
 * function:    modbus_rtu_timer_reset_all()
 * Description: This function is used to reset all the RTU timers for all Modbus channels.
 * Argument(s): none.
 * Return(s):   none.
 *
 * Caller(s):   modbus_master_rtu_timer_init().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_RTU_EN == 1)
void modbus_rtu_timer_reset_all(void)
{
    uint8_t   i;
    MODBUS_t *p_mb;

    p_mb = &mb_devices_tbl[0];
    for (i = 0; i < MODBUS_CFG_CHNL_MAX; i++)
    {
        if (p_mb->Mode == MODBUS_MODE_RTU)
            modbus_rtu_timer_reset(p_mb);
        p_mb++;
    }
}
#endif

/**************************************************************************************************
 * function:    modbus_rtu_timer_update()
 * Description: This function is called when the application supplied RTU framing timer expires.
 * Argument(s): none.
 * Return(s):   none.
 *
 * Caller(s):   modbus_rtu_timer_irq_handler().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_RTU_EN == 1)
void modbus_rtu_timer_update(void)
{
    MODBUS_t *p_mb;
    uint8_t i;

    p_mb = &mb_devices_tbl[0];

    for (i = 0; i < MODBUS_CFG_CHNL_MAX; i++)
    {
        if (p_mb->Mode == MODBUS_MODE_RTU)
        {
            if (p_mb->RTU_TimeoutEn == true)
            {
                if (p_mb->RTU_TimeoutCtr > 0)
                {
                	p_mb->RTU_TimeoutCtr--;

                    if (p_mb->RTU_TimeoutCtr == 0)
                    {
                        if (p_mb->MasterSlave == MODBUS_MASTER)
                        {
                        	p_mb->RTU_TimeoutEn = false;
                        }

                        /*
                         * RTU Timer expired for this Modbus channel
                         */
                        modbus_os_rx_signal(p_mb);
                    }
                }
            }
            else
            {
            	p_mb->RTU_TimeoutCtr = p_mb->RTU_TimeoutCnts;
            }
        }

        p_mb++;
    }
}
#endif

//-----------------------------------------------------------------------------
/*
 * @@ END
 */


