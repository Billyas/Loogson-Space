/**************************************************************************************************
 * MODBUS Utilities
 *
 *   Filename: mb_util.h
 *   Version:
 **************************************************************************************************/

#include "mb.h"

/**************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

//-------------------------------------------------------------------------------------------------

#if (MODBUS_CFG_ASCII_EN == 1)

/**************************************************************************************************
 * function:    modbus_ascii_bin_to_hex()
 * Description: Converts a byte into two ASCII characters into the given buffer.
 * Argument(s): value      The byte of data to be converted.
 *              pbuf       A pointer to the buffer to store the ASCII chars.
 * Return(s):   The buffer pointer which has been updated to point to the next char in the buffer.
 *
 * Caller(s):   MB_ASCII_Tx().
 * Note(s):     The function ONLY converts the byte to ASCII and DOES NOT null terminate the string.
 **************************************************************************************************/

uint8_t *modbus_ascii_bin_to_hex(uint8_t value, uint8_t *pbuf)
{
    uint8_t nibble;

    nibble = (value >> 4) & 0x0F;               /* Upper Nibble */
    if (nibble <= 9)
        *pbuf++ = (uint8_t)(nibble + '0');
    else
        *pbuf++ = (uint8_t)(nibble - 10 + 'A');

    nibble = value & 0x0F;                    	/* Lower Nibble */
    if (nibble <= 9)
        *pbuf++ = (uint8_t)(nibble + '0');
    else
        *pbuf++ = (uint8_t)(nibble - 10 + 'A');

    return pbuf;
}

/**************************************************************************************************
 * function:    modbus_ascii_hex_to_bin()
 * Description: Converts the first two ASCII hex characters in the buffer into one byte.
 * Argument(s): phex     Pointer to the buffer that contains the two ascii chars.
 * Return(s):   value of the two ASCII HEX digits pointed to by 'phex'.
 *
 * Caller(s):   MB_ASCII_RxByte(),
 *              MB_ASCII_Rx(),
 *              modbus_ascii_rx_calculate_lrc(),
 *              modbus_ascii_tx_calculate_lrc().
 * Note(s):     none.
 **************************************************************************************************/

uint8_t modbus_ascii_hex_to_bin (uint8_t *phex)
{
    uint8_t value, high, low;

    high = *phex;                            	/* Get upper nibble */
    phex++;
    low  = *phex;                             	/* Get lower nibble */

    if (high <= '9')                            /* Upper Nibble */
        value  = (uint8_t)(high - '0');
    else if (high <= 'F')
        value  = (uint8_t)(high - 'A' + 10);
    else
        value  = (uint8_t)(high - 'a' + 10);
    value <<= 4;

    if (low <= '9')                             /* Lower Nibble */
        value += (uint8_t)(low - '0');
    else if (low <= 'F')
        value += (uint8_t)(low - 'A' + 10);
    else
        value += (uint8_t)(low - 'a' + 10);

    return value;
}

/**************************************************************************************************
 * function:    modbus_ascii_rx_calculate_lrc()
 * Description: The function calculates an 8-bit Longitudinal Redundancy Check on a MODBUS_FRAME
 *              structure.
 * Argument(s): none.
 * Return(s):   The calculated LRC value.
 *
 * Caller(s):   MBS_ASCII_Task().
 * Note(s):     The LRC is calculated on the ADDR, FC and Data fields, not the ':', CR/LF and LRC
 *              placed in the message by the sender.  We thus need to subtract 5 'ASCII' characters
 *              from the received message to exclude these.
 **************************************************************************************************/

uint8_t modbus_ascii_rx_calculate_lrc(MODBUS_t *p_mb)
{
    uint8_t *pblock, lrc;
    uint16_t len;

    len    = (p_mb->RxBufByteCtr - 5) / 2;          /* LRC to include Addr + FC + Data */
    pblock = (uint8_t *)&p_mb->RxBuf[1];
    lrc    = 0;

    while (len-- > 0)
    {                          					    /* For each byte of data in the data block... */
        lrc    += modbus_ascii_hex_to_bin(pblock);  /* Add the data byte to LRC, increment data pointer. */
        pblock += 2;
    }

    lrc = ~lrc + 1;                                 /* Two complement the binary sum */
    return lrc;                                     /* Return LRC for all data in block. */
}

/**************************************************************************************************
 * function:    modbus_ascii_tx_calculate_lrc()
 * Description: The function calculates an 8-bit Longitudinal Redundancy Check on a MODBUS_FRAME
 *              structure.
 * Argument(s): none.
 * Return(s):   The calculated LRC value.
 *
 * Caller(s):   MB_ASCII_Tx().
 * Note(s):     (1) The LRC is calculated on the ADDR, FC and Data fields, not the ':' which was
 *                  inserted in the TxBuf[].  Thus we subtract 1 ASCII character from the LRC.
 *              (2) The LRC and CR/LF bytes are not YET in the .RxBuf[].
 **************************************************************************************************/

uint8_t modbus_ascii_tx_calculate_lrc(MODBUS_t *p_mb, uint16_t tx_bytes)
{
    uint8_t *pblock, lrc;
    uint16_t len;

    len    = (tx_bytes - 1) / 2;          		    /* LRC to include Addr + FC + Data (exclude ':') */
    pblock = (uint8_t *)&p_mb->TxBuf[1];
    lrc    = 0;

    while (len-- > 0)
    {
        lrc    += modbus_ascii_hex_to_bin(pblock);  /* Add the data byte to LRC, increment data pointer. */
        pblock += 2;
    }

    lrc = ~lrc + 1;                                 /* Two complement the binary sum */
    return lrc;                                     /* Return LRC for all data in block. */
}

#endif // #if (MODBUS_CFG_ASCII_EN == 1)

//-------------------------------------------------------------------------------------------------

#if (MODBUS_CFG_RTU_EN == 1)

/**************************************************************************************************
 * function:    modbus_rtu_rx_calculate_crc()
 * Description: The polynomial is a CRC-16 found for 'MBS_RxFrameNDataBytes' number of characters
 *              starting at 'MBS_RxFrameAddr'.
 * Argument(s): none.
 * Return(s):   An unsigned 16-bit value representing the CRC-16 of the data.
 *
 * Caller(s):   MBS_RTU_Task().
 * Note(s):     none.
 **************************************************************************************************/

uint16_t modbus_rtu_rx_calculate_crc(MODBUS_t *p_mb)
{
    uint16_t crc, length;
    uint8_t *pblock, shiftctr;
    bool     flag;

    pblock = (uint8_t *)&p_mb->RxFrameData[0];      /* Starting address of where the CRC data starts */
    length = p_mb->RxFrameNDataBytes + 2;          	/* Include the address and function code in the CRC */
    crc    = 0xFFFF;                            	/* Initialize CRC to all ones. */

    while (length > 0)
    {
        length--;
        crc ^= (uint16_t)*pblock++;
        shiftctr = 8;
        do
        {
            flag = (crc & 0x0001) ? true : false; 	/* Determine if the shift out of rightmost bit is 1 */
            crc >>= 1;                           	/* Shift CRC to the right one bit. */
            if (flag == true)                    	/* If (bit shifted out of rightmost bit was a 1) */
                crc ^= MODBUS_CRC16_POLY;         	/* Exclusive OR the CRC with the generating polynomial. */

            shiftctr--;
        } while (shiftctr > 0);
    }

    p_mb->RxFrameCRC_Calc = crc;
    return crc;
}

/**************************************************************************************************
 * function:    modbus_rtu_tx_calculate_crc()
 * Description: The polynomial is a CRC-16 found for 'MBS_TxFrameNDataBytes' number of characters
 *              starting at 'MBS_TxFrameAddr'.
 * Argument(s): none.
 * Return(s):   An unsigned 16-bit value representing the CRC-16 of the data.
 *
 * Caller(s):   MB_RTU_Tx().
 * Note*(s):    none.
 **************************************************************************************************/

uint16_t modbus_rtu_tx_calculate_crc(MODBUS_t *p_mb)
{
    uint16_t crc, length;
    uint8_t *pblock, shiftctr;
    bool     flag;

    pblock = (uint8_t *)&p_mb->TxFrameData[0];      /* Starting address of where the CRC data starts */
    length = p_mb->TxFrameNDataBytes + 2;           /* Include the address and function code in the CRC */
    crc    = 0xFFFF;                               	/* Initialize CRC to all ones. */

    while (length > 0)
    {
        length--;
        crc ^= (uint16_t)*pblock++;
        shiftctr = 8;

        do
        {
            flag = (crc & 0x0001) ? true : false; 	/* Determine if the shift out of rightmost bit is 1. */
            crc >>= 1;                            	/* Shift CRC to the right one bit. */

            if (flag == true)                    	/* If (bit shifted out of rightmost bit was a 1) */
                crc ^= MODBUS_CRC16_POLY;        	/* Exclusive OR the CRC with the generating polynomial */

            shiftctr--;
        } while (shiftctr > 0);
    }

    return crc;                                	    /* Return CRC for all data in block. */
}

#endif // #if (MODBUS_CFG_RTU_EN == 1)

//-----------------------------------------------------------------------------
/*
 * @@ END
 */


