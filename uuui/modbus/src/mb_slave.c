/*
 * MODBUS SLAVE COMMAND PROCESSOR
 *
 *   Filename: mb_slave.c
 *   Version:
 */

#include "mb.h"

#if (MODBUS_CFG_SLAVE_EN == 1)

/**************************************************************************************************
 * LOCAL MACRO'S
 **************************************************************************************************/

#define MBS_RX_DATA_START  		(((uint16_t)p_mb->RxFrameData[2] << 8) + \
		                          (uint16_t)p_mb->RxFrameData[3])
#define MBS_RX_DATA_START_H		(p_mb->RxFrameData[2])
#define MBS_RX_DATA_START_L		(p_mb->RxFrameData[3])

#define MBS_RX_DATA_POINTS 		(((uint16_t)p_mb->RxFrameData[4] << 8) + \
		                          (uint16_t)p_mb->RxFrameData[5])
#define MBS_RX_DATA_POINTS_H	(p_mb->RxFrameData[4])
#define MBS_RX_DATA_POINTS_L	(p_mb->RxFrameData[5])

#define MBS_RX_DATA_BYTES      	(p_mb->RxFrameData[6])

#define MBS_RX_DATA_COIL      	(((uint16_t)p_mb->RxFrameData[4] << 8) + \
		                          (uint16_t)p_mb->RxFrameData[5])
#define MBS_RX_DATA_COIL_H     	(p_mb->RxFrameData[4])
#define MBS_RX_DATA_COIL_L     	(p_mb->RxFrameData[5])

#define MBS_RX_DATA_REG       	(((uint16_t)p_mb->RxFrameData[4] << 8) + \
		                          (uint16_t)p_mb->RxFrameData[5])
#define MBS_RX_DATA_REG_H      	(p_mb->RxFrameData[4])
#define MBS_RX_DATA_REG_L      	(p_mb->RxFrameData[5])

#define MBS_RX_DIAG_CODE      	(((uint16_t)p_mb->RxFrameData[2] << 8) + \
		                          (uint16_t)p_mb->RxFrameData[3])
#define MBS_RX_DIAG_CODE_H     	(p_mb->RxFrameData[2])
#define MBS_RX_DIAG_CODE_L     	(p_mb->RxFrameData[3])
#define MBS_RX_DIAG_DATA      	(((uint16_t)p_mb->RxFrameData[4] << 8) + \
		                          (uint16_t)p_mb->RxFrameData[5])
#define MBS_RX_DIAG_DATA_H     	(p_mb->RxFrameData[4])
#define MBS_RX_DIAG_DATA_L     	(p_mb->RxFrameData[5])

#define MBS_RX_FRAME          	(&p_mb->RxFrame)
#define MBS_RX_FRAME_ADDR      	(p_mb->RxFrameData[0])
#define MBS_RX_FRAME_FC        	(p_mb->RxFrameData[1])
#define MBS_RX_FRAME_DATA      	(p_mb->RxFrameData[2])
#define MBS_RX_FRAME_NBYTES    	(p_mb->RxFrameNDataBytes)


#define MBS_TX_DATA_START_H    	(p_mb->TxFrameData[2])
#define MBS_TX_DATA_START_L    	(p_mb->TxFrameData[3])

#define MBS_TX_DATA_POINTS_H   	(p_mb->TxFrameData[4])
#define MBS_TX_DATA_POINTS_L   	(p_mb->TxFrameData[5])

#define MBS_TX_DATA_COIL_H     	(p_mb->TxFrameData[4])
#define MBS_TX_DATA_COIL_L     	(p_mb->TxFrameData[5])

#define MBS_TX_DATA_REG_H      	(p_mb->TxFrameData[4])
#define MBS_TX_DATA_REG_L      	(p_mb->TxFrameData[5])

#define MBS_TX_DIAG_CODE_H     	(p_mb->TxFrameData[2])
#define MBS_TX_DIAG_CODE_L     	(p_mb->TxFrameData[3])
#define MBS_TX_DIAG_DATA_H     	(p_mb->TxFrameData[4])
#define MBS_TX_DIAG_DATA_L     	(p_mb->TxFrameData[5])

#define MBS_TX_FRAME          	(&p_mb->TxFrame)
#define MBS_TX_FRAME_ADDR      	(p_mb->TxFrameData[0])
#define MBS_TX_FRAME_FC        	(p_mb->TxFrameData[1])
#define MBS_TX_FRAME_DATA      	(p_mb->TxFrameData[2])
#define MBS_TX_FRAME_NBYTES    	(p_mb->TxFrameNDataBytes)

/**************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

static void modbus_slave_set_response_error(MODBUS_t *p_mb, uint8_t errcode);

#if (MODBUS_CFG_FC01_EN == 1)
static bool modbus_slave_fc01_read_coil(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_FC02_EN == 1)
static bool modbus_slave_fc02_read_di(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_FC03_EN == 1)
static bool modbus_slave_fc03_read_holding_register(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_FC04_EN == 1)
static bool modbus_slave_fc04_read_in_register(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_FC05_EN == 1)
static bool modbus_slave_fc05_write_coil(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_FC06_EN == 1)
static bool modbus_slave_fc06_write_holding_register(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_FC08_EN == 1)
static bool modbus_slave_fc08_loopback(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_FC15_EN == 1)
static bool modbus_slave_fc15_write_coil_multiple(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_FC16_EN == 1)
static bool modbus_slave_fc16_write_holding_register_multiple(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_FC20_EN == 1)
static bool modbus_slave_fc20_read_file(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_FC21_EN == 1)
static bool modbus_slave_fc21_write_file(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_ASCII_EN == 1)
static void modbus_slave_ascii_task(MODBUS_t *p_mb);
#endif

#if (MODBUS_CFG_RTU_EN   == 1)
static void modbus_slave_rtu_task(MODBUS_t *p_mb);
#endif

/**************************************************************************************************
 * function:    modbus_slave_set_response_error()
 * Description: This function sets the indicated error response code into the response frame.
 *              Then the routine is called to calculate the error check value.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 *              errcode     An unsigned byte value containing the error code that is to be placed
 *                          in the response frame.
 * Return(s):   none.
 *
 * Caller(s):   modbus_slave_fcxx_handler(),
 *              Modbus Slave functions.
 * Note(s):     none.
 **************************************************************************************************/

static void modbus_slave_set_response_error(MODBUS_t *p_mb, uint8_t err_code)
{
#if (MODBUS_CFG_FC08_EN == 1)
    p_mb->StatExceptCtr++;
#endif
    MBS_TX_FRAME_ADDR   = MBS_RX_FRAME_ADDR;
    MBS_TX_FRAME_FC     = MBS_RX_FRAME_FC | 0x80;  	    /* Set the high order bit of the function code. */
    MBS_TX_FRAME_DATA   = err_code;                	    /* Set the high order bit of the function code. */
    MBS_TX_FRAME_NBYTES = 1;                      	    /* Nbr of data bytes in exception response is 1. */
}

/**************************************************************************************************
 * function:    modbus_slave_fcxx_handler()
 * Description: This is the main processing function for MODBUS commands. The message integrity is
 *              verified, and if valid, the function requested is processed. Unimplemented functions
 *              will generate an Illegal Function Exception Response code (01).
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   none.
 *
 * Caller(s):   modbus_slave_ascii_task(),
 *              modbus_slave_rtu_task().
 * Note(s):     none.
 **************************************************************************************************/

bool modbus_slave_fcxx_handler(MODBUS_t *p_mb)
{
    bool do_reply = false;

    /* Proper node address? (i.e. Is this message for us?)
     * ... or a 'broadcast' address? */
    if ((MBS_RX_FRAME_ADDR == p_mb->NodeAddr) || (MBS_RX_FRAME_ADDR == 0))
    {
#if (MODBUS_CFG_FC08_EN == 1)
        p_mb->StatSlaveMsgCtr++;
#endif
        switch (MBS_RX_FRAME_FC)
        {
		#if (MODBUS_CFG_FC01_EN == 1)
            case MODBUS_FC01_COIL_RD:
                 do_reply = modbus_slave_fc01_read_coil(p_mb);
                 break;
		#endif

		#if (MODBUS_CFG_FC02_EN == 1)
            case MODBUS_FC02_DI_RD:
                 do_reply = modbus_slave_fc02_read_di(p_mb);
                 break;
		#endif

		#if (MODBUS_CFG_FC03_EN == 1)
            case MODBUS_FC03_HOLDING_REG_RD:
                 do_reply = modbus_slave_fc03_read_holding_register(p_mb);
                 break;
		#endif

		#if (MODBUS_CFG_FC04_EN == 1)
            case MODBUS_FC04_IN_REG_RD:
                 do_reply = modbus_slave_fc04_read_in_register(p_mb);
                 break;
		#endif

		#if (MODBUS_CFG_FC05_EN == 1)
            case MODBUS_FC05_COIL_WR:
                 if (p_mb->WrEn == true)
                     do_reply = modbus_slave_fc05_write_coil(p_mb);
                 else
                     do_reply = false;
                 break;
		#endif

		#if (MODBUS_CFG_FC06_EN == 1)
            case MODBUS_FC06_HOLDING_REG_WR:
                 if (p_mb->WrEn == true)
                     do_reply = modbus_slave_fc06_write_holding_register(p_mb);
                 else
                     do_reply = false;
                 break;
		#endif

		#if (MODBUS_CFG_FC08_EN == 1)
            case MODBUS_FC08_LOOPBACK:
                 do_reply = modbus_slave_fc08_loopback(p_mb);	/* Process loopback command. */
                 break;
		#endif

		#if (MODBUS_CFG_FC15_EN == 1)
            case MODBUS_FC15_COIL_WR_MULTIPLE:
                 if (p_mb->WrEn == true)
                     do_reply = modbus_slave_fc15_write_coil_multiple(p_mb);
                 else
                     do_reply = false;
                 break;
		#endif

		#if (MODBUS_CFG_FC16_EN == 1)
            case MODBUS_FC16_HOLDING_REG_WR_MULTIPLE:
                 if (p_mb->WrEn == true)
                     do_reply = modbus_slave_fc16_write_holding_register_multiple(p_mb);
                 else
                     do_reply = false;
                 break;
		#endif

		#if (MODBUS_CFG_FC20_EN == 1)
            case MODBUS_FC20_FILE_RD:
                 do_reply = modbus_slave_fc20_read_file(p_mb);
                 break;
		#endif

		#if (MODBUS_CFG_FC21_EN == 1)
            case MODBUS_FC21_FILE_WR:
                 if (p_mb->WrEn == true)
                	 do_reply = modbus_slave_fc21_write_file(p_mb);
                 else
                     do_reply = false;
                 break;
		#endif

            default:                  	    /* Function code not implemented, set error response. */
                 p_mb->Err = MODBUS_ERR_ILLEGAL_FC;
                 modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_FC);
                 do_reply = true;
                 break;
        }
    }

    if (MBS_RX_FRAME_ADDR == 0)       	    /* Was the command received a 'broadcast'? */
        return false;                	    /* Yes, don't reply */
    else
        return do_reply;         	        /* No,  reply according to the outcome of the command */
}

/**************************************************************************************************
 * function:    modbus_slave_fc01_read_coil()
 * Description: Responds to a request to read the status of any number of coils.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 *
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     1) RX command format:             Example:
 *                 <slave address>                0x11
 *                 <function code>                0x01
 *                 <start address HI>             0x00
 *                 <start address LO>             0x13
 *                 <# coils HI>                   0x00
 *                 <# coils LO>                   0x25
 *                 <Error Check (LRC or CRC)>     0x??
 *              2) TX reply format:               Example:
 *                 <slave address>                0x11
 *                 <function code>                0x01
 *                 <byte count>                   0x05
 *                 <Data Coils>                   0xCD  (Bit set to 1 means ON, Bit cleared means == OFF)
 *                 <Data Coils>                   0x6B  (Bit set to 1 means ON, Bit cleared means == OFF)
 *                 <Data Coils>                   0xB2  (Bit set to 1 means ON, Bit cleared means == OFF)
 *                 <Data Coils>                   0x0E  (Bit set to 1 means ON, Bit cleared means == OFF)
 *                 <Data Coils>                   0x1B  (Bit set to 1 means ON, Bit cleared means == OFF)
 *                 <Error Check (LRC or CRC)>     0x??
 **************************************************************************************************/

#if (MODBUS_CFG_FC01_EN == 1)
static bool modbus_slave_fc01_read_coil(MODBUS_t *p_mb)
{
    uint8_t *presp, bit_mask;
    uint16_t coil, err, nbr_coils, nbr_bytes, ix;
    bool     coil_val;

    if (p_mb->RxFrameNDataBytes != 4)				/* 4 data bytes for this message. */
        return false;                            	/* Tell caller that we DON'T need to send a response */

    coil      = MBS_RX_DATA_START;             		/* Get the starting address of the desired coils */
    nbr_coils = MBS_RX_DATA_POINTS;               	/* Find out how many coils */
    if (nbr_coils == 0 || nbr_coils > 2000)         /* Make sure we don't exceed the allowed limit per request */
    {          
        p_mb->Err = MODBUS_ERR_FC01_01;
        modbus_slave_set_response_error(p_mb,  MODBUS_ERR_ILLEGAL_DATA_QTY);
        return true;                           	    /* Tell caller that we need to send a response */
    }

    nbr_bytes = ((nbr_coils - 1) / 8) + 1;          /* Find #bytes needed for response. */
    p_mb->TxFrameNDataBytes = nbr_bytes + 1;        /* Number of data bytes + byte count. */
    presp     = &p_mb->TxFrameData[0];          	/* Clear bytes in response */
    for (ix = 0; ix < (nbr_bytes + 3); ix++)
        *presp++ = 0x00;

    bit_mask = 0x01;                             	/* Start with bit 0 in response byte data mask. */
    ix       =    0;                            	/* Initialize loop counter. */
    presp    = &p_mb->TxFrameData[0];            	/* Reset the pointer to the start of the response */
    *presp++ = MBS_RX_FRAME_ADDR;               	/* Prepare response packet */
    *presp++ = MBS_RX_FRAME_FC;
    *presp++ = (uint8_t)nbr_bytes;                	/* Set number of data bytes in response message. */

    while (ix < nbr_coils)							/* Loop through each COIL requested. */
    {
        coil_val = modbus_read_coil(coil, &err);    /* Get the current value of the coil */

        switch (err)
        {
            case MODBUS_ERR_NONE:
                 if (coil_val == MODBUS_COIL_ON)	/* Only set data response bit if COIL is on. */
                     *presp |= bit_mask;
                 coil++;
                 ix++;                          	/* Increment COIL counter. */
                 if ((ix % 8) == 0)          		/* Determine if 8 data bits have been filled. */
                 {
                     bit_mask = 0x01;        		/* Reset the data mask. */
                     presp++;                    	/* Increment data frame index. */
                 }
                 else                           	/* Still in same data byte, so */
                     bit_mask <<= 1;              	/* Shift the data mask to the next higher bit position. */

                 break;

            case MODBUS_ERR_RANGE:
            default:
                 p_mb->Err = MODBUS_ERR_FC01_02;
                 modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                 return true;                  	    /* Tell caller that we need to send a response */
        }
    }

    p_mb->Err = MODBUS_ERR_NONE;
    return true;                               	    /* Tell caller that we need to send a response */
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_fc02_read_di()
 * Description: Responds to a request to read the status of any number of Discrete Inputs (DIs).
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 *
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     1) RX command format:             Example:
 *                 <slave address>                0x11
 *                 <function code>                0x02
 *                 <start address HI>             0x00
 *                 <start address LO>             0xC4
 *                 <# input statuses HI>          0x00
 *                 <# input statuses LO>          0x16
 *                 <Error Check (LRC or CRC)>     0x??
 *              2) TX reply format:               Example:
 *                 <slave address>                0x11
 *                 <function code>                0x02
 *                 <byte count>                   0x03
 *                 <Data Inputs>                  0xAC  (Bit set to 1 means ON, Bit cleared means == OFF)
 *                 <Data Inputs>                  0xDB  (Bit set to 1 means ON, Bit cleared means == OFF)
 *                 <Data Inputs>                  0x35  (Bit set to 1 means ON, Bit cleared means == OFF)
 *                 <Error Check (LRC or CRC)>     0x??
 **************************************************************************************************/

#if (MODBUS_CFG_FC02_EN == 1)
static bool modbus_slave_fc02_read_di(MODBUS_t *p_mb)
{
    uint8_t *presp, bit_mask;
    uint16_t di, err, nbr_di, nbr_bytes, ix;
    bool     di_val;

    if (p_mb->RxFrameNDataBytes != 4)				/* 4 data bytes for this message. */
        return false;                          	    /* Tell caller that we DON'T need to send a response */

    di     = MBS_RX_DATA_START;                  	/* Get the starting address of the desired DIs */
    nbr_di = MBS_RX_DATA_POINTS;                  	/* Find out how many DIs */
    if (nbr_di == 0 || nbr_di > 2000)          		/* Make sure we don't exceed the allowed limit per request */
    {
        p_mb->Err = MODBUS_ERR_FC02_01;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
        return true;                             	/* Tell caller that we need to send a response */
    }

    nbr_bytes = ((nbr_di - 1) / 8) + 1;         	/* Find #bytes needed for response. */
    p_mb->TxFrameNDataBytes = nbr_bytes + 1;   	    /* Number of data bytes + byte count. */
    presp     = &p_mb->TxFrameData[0];           	/* Clear bytes in response */
    for (ix = 0; ix < (nbr_bytes + 3); ix++)
        *presp++ = 0x00;

    bit_mask = 0x01;                             	/* Start with bit 0 in response byte data mask. */
    ix       = 0;                                	/* Initialize loop counter. */
    presp    = &p_mb->TxFrameData[0];            	/* Reset the pointer to the start of the response */
    *presp++ = MBS_RX_FRAME_ADDR;                	/* Prepare response packet */
    *presp++ = MBS_RX_FRAME_FC;
    *presp++ = (uint8_t)nbr_bytes;               	/* Set number of data bytes in response message. */

    while (ix < nbr_di)       						/* Loop through each DI requested. */
    {
        di_val = modbus_read_di(di, &err);          /* Get the current value of the DI */
        switch (err)
        {
            case MODBUS_ERR_NONE:
                 if (di_val == MODBUS_COIL_ON)		/* Only set data response bit if DI is on. */
                     *presp |= bit_mask;
                 di++;
                 ix++;                           	/* Increment DI counter. */
                 if ((ix % 8) == 0)             	/* Determine if 8 data bits have been filled. */
                 {
                     bit_mask   = 0x01;          	/* Reset the data mask. */
                     presp++;                     	/* Increment data frame index. */
                 }
                 else                           	/* Still in same data byte, so */
                     bit_mask <<= 1;             	/* Shift the data mask to the next higher bit position. */

                 break;

            case MODBUS_ERR_RANGE:
            default:
                 p_mb->Err = MODBUS_ERR_FC02_02;
                 modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                 return true;                  	    /* Tell caller that we need to send a response */
        }
    }

    p_mb->Err = MODBUS_ERR_NONE;
    return true;                              	    /* Tell caller that we need to send a response */
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_fc03_read_holding_register()
 * Description: Obtains the contents of the specified holding registers.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     1) RX command format:             Example:
 *                 <slave address>                0x11
 *                 <function code>                0x03
 *                 <start address HI>             0x00
 *                 <start address LO>             0x6B
 *                 <# registers HI>               0x00
 *                 <# registers LO>               0x03
 *                 <Error Check (LRC or CRC)>     0x??
 *              2) TX reply format:               Example:
 *                 <slave address>                0x11
 *                 <function code>                0x03
 *                 <byte count>                   0x06
 *                 <Data HI register>             0x02
 *                 <Data LO register>             0x2B
 *                 <Data HI register>             0x00
 *                 <Data LO register>             0x00
 *                 <Data HI register>             0x00
 *                 <Data LO register>             0x64
 *                 <Error Check (LRC or CRC)>     0x??
 **************************************************************************************************/

#if (MODBUS_CFG_FC03_EN == 1)
static bool modbus_slave_fc03_read_holding_register(MODBUS_t *p_mb)
{
    uint8_t *presp;
    uint16_t err, reg, nbr_regs, nbr_bytes, reg_val_16;
#if (MODBUS_CFG_FP_EN == 1)
    uint8_t *pfp, ix;
    float    reg_val_fp;
#endif

    if (p_mb->RxFrameNDataBytes != 4)  				        /* Nbr of data bytes must be 4. */
        return false;                          	            /* Tell caller that we DON'T need to send a response */

    reg      = MBS_RX_DATA_START;
    nbr_regs = MBS_RX_DATA_POINTS;

#if (MODBUS_CFG_FP_EN == 1)
    if (reg < MODBUS_CFG_FP_START_IX)          		        /* See if we want integer registers */
    {
        if (nbr_regs == 0 || nbr_regs > 125)		        /* Make sure we don't exceed the allowed limit per request */
        {
            p_mb->Err = MODBUS_ERR_FC03_03;
            modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
            return true;                       	            /* Tell caller that we need to send a response */
        }
        
        nbr_bytes = (uint8_t)(nbr_regs * sizeof(uint16_t)); /* Find #bytes needed for response. */
    }
    else
    {
        if (nbr_regs == 0 || nbr_regs > 62)			        /* Make sure we don't exceed the allowed limit per request */
        {
            p_mb->Err = MODBUS_ERR_FC03_04;
            modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
            return true;                       	            /* Tell caller that we need to send a response */
        }
        
        nbr_bytes = (uint8_t)(nbr_regs * sizeof(float));    /* Find #bytes needed for response. */
    }

#else
    if (nbr_regs == 0 || nbr_regs > 125)			        /* Make sure we don't exceed the allowed limit per request */
    {
        p_mb->Err = MODBUS_ERR_FC03_03;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
        return true;                           	            /* Tell caller that we need to send a response */
    }
    
    nbr_bytes = (uint8_t)(nbr_regs * sizeof(uint16_t));     /* Find #bytes needed for response. */

#endif

    p_mb->TxFrameNDataBytes = nbr_bytes + 1;     	        /* Number of data bytes + byte count. */
    presp    = &p_mb->TxFrameData[0];            	        /* Reset the pointer to the start of the response */
    *presp++ =  MBS_RX_FRAME_ADDR;
    *presp++ =  MBS_RX_FRAME_FC;
    *presp++ = (uint8_t)nbr_bytes;               	        /* Set number of data bytes in response message */

    while (nbr_regs > 0)                      		        /* Loop through each register requested. */
    {
        if (reg < MODBUS_CFG_FP_START_IX) 			        /* See if we want an integer register */
        {
            reg_val_16 = modbus_read_holding_register(reg, &err);       /* Yes, get its value */

            switch (err)
            {
                case MODBUS_ERR_NONE:
                     *presp++ = (uint8_t)((reg_val_16 >> 8) & 0x00FF); 	/* Get MSB first. */
                     *presp++ = (uint8_t)(reg_val_16 & 0x00FF);        	/* Get LSB next. */
                     break;

                case MODBUS_ERR_RANGE:
                default:
                     p_mb->Err = MODBUS_ERR_FC03_01;
                     modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                     return true;
            }
        }
        else
        {
#if (MODBUS_CFG_FP_EN == 1)
            reg_val_fp = modbus_read_holding_register_float(reg, &err); /* No,  get the value of the FP register */

            switch (err)
            {
                case MODBUS_ERR_NONE:
                     pfp = (uint8_t *)&reg_val_fp;          /* Point to the FP register */
                     pfp += sizeof(float) - 1;
                     for (ix = 0; ix < sizeof(float); ix++)
                         *presp++ = *pfp--;
                     break;

                case MODBUS_ERR_RANGE:
                default:
                     p_mb->Err = MODBUS_ERR_FC03_02;
                     modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                     return true;
            }
#endif
        }

        reg++;                           	/* Increment current register number */
        nbr_regs--;
    }

    p_mb->Err = MODBUS_ERR_NONE;
    return true;                  		    /* Tell caller that we need to send a response */
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_fc04_read_in_register()
 * Description: Obtains the contents of the specified input registers.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     1) RX command format:             Example:
 *                 <slave address>                0x11
 *                 <function code>                0x04
 *                 <start address HI>             0x00
 *                 <start address LO>             0x08
 *                 <# registers HI>               0x00
 *                 <# registers LO>               0x01
 *                 <Error Check (LRC or CRC)>     0x??
 *              2) TX reply format:               Example:
 *                 <slave address>                0x11
 *                 <function code>                0x04
 *                 <byte count>                   0x02
 *                 <Data HI register value>       0x00
 *                 <Data LO register value>       0x0A
 *                 <Error Check (LRC or CRC)>     0x??
 **************************************************************************************************/

#if (MODBUS_CFG_FC04_EN == 1)
static bool modbus_slave_fc04_read_in_register(MODBUS_t *p_mb)
{
    uint8_t *presp;
    uint16_t err, reg, nbr_regs, nbr_bytes, reg_val_16;
#if (MODBUS_CFG_FP_EN == 1)
    uint8_t *pfp, ix;
    float    reg_val_fp;
#endif

    if (p_mb->RxFrameNDataBytes != 4)   			        /* Nbr of data bytes must be 4. */
        return false;                          	            /* Tell caller that we DON'T need to send a response */

    reg      = MBS_RX_DATA_START;
    nbr_regs = MBS_RX_DATA_POINTS;

#if (MODBUS_CFG_FP_EN == 1)
    if (reg < MODBUS_CFG_FP_START_IX)              	        /* See if we want integer registers */
    {
        if (nbr_regs == 0 || nbr_regs > 125)  		        /* Make sure we don't exceed the allowed limit per request */
        {
            p_mb->Err = MODBUS_ERR_FC04_03;
            modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
            return true;                        	        /* Tell caller that we need to send a response */
        }

        nbr_bytes = (uint8_t)(nbr_regs * sizeof(uint16_t)); /* Find #bytes needed for response. */
    }
    else
    {
        if (nbr_regs == 0 || nbr_regs > 62)			        /* Make sure we don't exceed the allowed limit per request */
        {
            p_mb->Err = MODBUS_ERR_FC04_04;
            modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
            return true;                       	            /* Tell caller that we need to send a response */
        }

        nbr_bytes = (uint8_t)(nbr_regs * sizeof(float));	/* Find #bytes needed for response. */
    }

#else
    if (nbr_regs == 0 || nbr_regs > 125)                    /* Make sure we don't exceed the allowed limit per request */
    {
        p_mb->Err = MODBUS_ERR_FC04_03;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
        return true);                           	        /* Tell caller that we need to send a response */
    }

    nbr_bytes = (uint8_t)(nbr_regs * sizeof(uint16_t));     /* Find #bytes needed for response. */
#endif

    p_mb->TxFrameNDataBytes = nbr_bytes + 1;     	        /* Number of data bytes + byte count. */
    presp    = &p_mb->TxFrameData[0];            	        /* Reset the pointer to the start of the response */
    *presp++ =  MBS_RX_FRAME_ADDR;                          /* Prepare response packet */
    *presp++ =  MBS_RX_FRAME_FC;
    *presp++ = (uint8_t)nbr_bytes;               	        /* Set number of data bytes in response message */

    while (nbr_regs > 0)                   			        /* Loop through each register requested. */
    {
        if (reg < MODBUS_CFG_FP_START_IX)                   /* See if we want an integer register */
        {
            reg_val_16 = modbus_read_in_register(reg, &err);            /* Yes, get its value */
            switch (err)
            {
                case MODBUS_ERR_NONE:
                     *presp++ = (uint8_t)((reg_val_16 >> 8) & 0x00FF); 	/* Get MSB first. */
                     *presp++ = (uint8_t)(reg_val_16 & 0x00FF);        	/* Get LSB next. */
                     break;

                case MODBUS_ERR_RANGE:
                default:
                     p_mb->Err = MODBUS_ERR_FC04_01;
                     modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                     return true;
            }
        }
        else
        {
#if (MODBUS_CFG_FP_EN == 1)
            reg_val_fp = modbus_read_in_register_float(reg, &err); /* No,  get the value of the FP register */

            switch (err)
            {
                case MODBUS_ERR_NONE:
                     pfp = (uint8_t *)&reg_val_fp;          /* Point to the FP register */
                     pfp += sizeof(float) - 1;
                     for (ix = 0; ix < sizeof(float); ix++)
                         *presp++ = *pfp--;
                     break;

                case MODBUS_ERR_RANGE:
                default:
                     p_mb->Err = MODBUS_ERR_FC04_02;
                     modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                     return true;
            }
#endif
        }

        reg++;                              /* Increment current register number */
        nbr_regs--;
    }

    p_mb->Err = MODBUS_ERR_NONE;
    return true;                     		/* Tell caller that we need to send a response */
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_fc05_write_coil()
 * Description: Responds to a request to force a coil to a specified state.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     1) A value of 0xFF00 forces a coil ON and 0x0000 to OFF
 *              2) RX command format:             Example:
 *                 <slave address>                0x11
 *                 <function code>                0x05
 *                 <Coil address HI>              0x00
 *                 <Coil address LO>              0xAC
 *                 <Force coil value HI>          0xFF
 *                 <Force coil value LO>          0x00
 *                 <Error Check (LRC or CRC)>     0x??
 *              3) TX reply format:               Example:
 *                 <slave address>                0x11
 *                 <function code>                0x05
 *                 <Coil address HI>              0x00
 *                 <Coil address LO>              0xAC
 *                 <Force coil value HI>          0xFF
 *                 <Force coil value LO>          0x00
 *                 <Error Check (LRC or CRC)>     0x??
 **************************************************************************************************/

#if (MODBUS_CFG_FC05_EN == 1)
static bool modbus_slave_fc05_write_coil(MODBUS_t *p_mb)
{
    uint8_t *prx_data, *ptx_data, i;
    uint16_t coil, temp, err;;
    bool     coil_val;

    if (p_mb->RxFrameNDataBytes != 4)  				    /* Nbr of data bytes must be 4. */
        return false;                           	    /* Tell caller that we DON'T need to send a response */

    coil = MBS_RX_DATA_START;                     	    /* Get the desired coil number */
    temp = MBS_RX_DATA_COIL;
    if (p_mb->WrEn == true)
    {
        if (temp == MODBUS_COIL_OFF_CODE)			    /* See if coil needs to be OFF? */
            coil_val = 0;                          	    /* Yes, Turn coil OFF */
        else
            coil_val = 1;                        	    /* No,  Turn coil ON */

        modbus_write_coil(coil, coil_val, &err);        /* Force coil */

    }
    else
    {
        p_mb->Err = MODBUS_ERR_FC05_02;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_VAL);
        return true;                             	    /* Tell caller that we need to send a response */
    }

    p_mb->TxFrameNDataBytes = 4;
    MBS_TX_FRAME_ADDR = MBS_RX_FRAME_ADDR;        	    /* Prepare response packet */
    MBS_TX_FRAME_FC   = MBS_RX_FRAME_FC;
    prx_data = &p_mb->RxFrameData[2];                   /* Copy four data bytes from the receive packet */
    ptx_data = &p_mb->TxFrameData[2];

    for (i = 0; i < 4; i++)
    {
        *ptx_data++ = *prx_data++;
    }

    switch (err)
    {
        case MODBUS_ERR_NONE:                    	    /* We simply echoe back with the command received */
             p_mb->Err = MODBUS_ERR_NONE;
             p_mb->WrCtr++;
             break;

        case MODBUS_ERR_RANGE:
        default:
             p_mb->Err = MODBUS_ERR_FC05_01;
             modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
             break;
    }

    return true;                            /* Tell caller that we need to send a response */
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_fc06_write_holding_register()
 * Description: Change the value of a single register.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     1) RX command format:             Example:
 *                 <slave address>                0x11
 *                 <function code>                0x06
 *                 <start address HI>             0x00
 *                 <start address LO>             0x01
 *                 <Register value HI>            0x00
 *                 <Register value LO>            0x03
 *                 <Error Check (LRC or CRC)>     0x??
 *              2) TX reply format:               Example:
 *                 <slave address>                0x11
 *                 <function code>                0x06
 *                 <start address HI>             0x00
 *                 <start address LO>             0x01
 *                 <Register value HI>            0x00
 *                 <Register value LO>            0x03
 *                 <Error Check (LRC or CRC)>     0x??
 **************************************************************************************************/

#if (MODBUS_CFG_FC06_EN == 1)
static bool modbus_slave_fc06_write_holding_register(MODBUS_t *p_mb)
{
    uint8_t *prx_data, *ptx_data, i, max;
    uint16_t err, reg, reg_val_16;
#if (MODBUS_CFG_FP_EN == 1)
    float    reg_val_fp;
    uint8_t *pfp;
#endif

    if (p_mb->RxFrameNDataBytes != 4)       			      /* Nbr of data bytes must be 4. */
        return false;

    reg = MBS_RX_DATA_START;

#if (MODBUS_CFG_FP_EN == 1)
    if (reg < MODBUS_CFG_FP_START_IX)
    {
        reg_val_16 = MBS_RX_DATA_REG;
        modbus_write_holding_register(reg, reg_val_16, &err); /* Write to integer register */
    }
    else
    {
        prx_data = &p_mb->RxFrameData[4];        		      /* Point to data in the received frame. */
        pfp      = (uint8_t *)&reg_val_fp;
        prx_data += sizeof(float) - 1;
        for (i = 0; i < sizeof(float); i++)
            *pfp++ = *prx_data--;
        modbus_write_holding_register_float(reg, reg_val_fp, &err); /* Write to floating point register */
    }
#else
    reg_val_16 = MBS_RX_DATA_REG;
    modbus_write_holding_register(reg, reg_val_16, &err);           /* Write to integer register */

#endif

    p_mb->TxFrameNDataBytes = 4;
    MBS_TX_FRAME_ADDR = MBS_RX_FRAME_ADDR;            	/* Prepare response packet (duplicate Rx frame) */
    MBS_TX_FRAME_FC   = MBS_RX_FRAME_FC;
    prx_data = &p_mb->RxFrameData[2];               	/* Copy received register address and data to response */
    ptx_data = &p_mb->TxFrameData[2];
    if (reg < MODBUS_CFG_FP_START_IX)
        max = sizeof(uint16_t) + 2;
    else
        max = sizeof(float) + 2;

    for (i = 0; i < max; i++)
        *ptx_data++ = *prx_data++;

    switch (err)
    {
        case MODBUS_ERR_NONE:                       	/* Reply with echoe of command received */
             p_mb->Err = MODBUS_ERR_NONE;
             p_mb->WrCtr++;
             break;

        case MODBUS_ERR_RANGE:
        default:
             p_mb->Err = MODBUS_ERR_FC06_01;
             modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
             break;
    }

    return true;
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_fc08_loopback()
 * Description: The LOOPBACK function contains various diagnostic codes that perform specific actions.
 *              This function processes individual diagnostic requests and formats the response message
 *              frame accordingly.  Unimplemented diagnostic codes will return an Illegal Data Value
 *              Exception Response Code (03).
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 *
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_FC08_EN == 1)
static bool modbus_slave_fc08_loopback(MODBUS_t *p_mb)
{
    uint16_t diagcode;

    if (p_mb->RxFrameNDataBytes != 4)    				    /* Nbr of data bytes must be 4. */
        return false;

    diagcode           = MBS_RX_DIAG_CODE;
    MBS_TX_FRAME_ADDR  = MBS_RX_FRAME_ADDR;          	    /* Prepare response packet */
    MBS_TX_FRAME_FC    = MBS_RX_FRAME_FC;
    MBS_TX_DIAG_CODE_H = MBS_RX_DIAG_CODE_H;
    MBS_TX_DIAG_CODE_L = MBS_RX_DIAG_CODE_L;

    switch (diagcode)
    {
        case MODBUS_FC08_LOOPBACK_QUERY:             	    /* Return Query function code - no need to do anything. */
             MBS_TX_DIAG_DATA_H = MBS_RX_DIAG_DATA_H;
             MBS_TX_DIAG_DATA_L = MBS_RX_DIAG_DATA_L;
             p_mb->Err           = MODBUS_ERR_NONE;
             break;

        case MODBUS_FC08_LOOPBACK_CLR_CTR:
	#if (MODBUS_CFG_SLAVE_EN == 1) && (MODBUS_CFG_FC08_EN  == 1)
             modbus_slave_stat_init(p_mb);                  /* Initialize the system counters, echo response back. */
	#endif
             p_mb->Err = MODBUS_ERR_NONE;
             break;

        case MODBUS_FC08_LOOPBACK_BUS_MSG_CTR:       	    /* Return the message count in diag information field. */
             MBS_TX_DIAG_DATA_H = (uint8_t)((p_mb->StatMsgCtr & 0xFF00) >> 8);
             MBS_TX_DIAG_DATA_L = (uint8_t) (p_mb->StatMsgCtr & 0x00FF);
             p_mb->Err           = MODBUS_ERR_NONE;
             break;

        case MODBUS_FC08_LOOPBACK_BUS_CRC_CTR:        	    /* Return the CRC error count in diag information field. */
             MBS_TX_DIAG_DATA_H = (uint8_t)((p_mb->StatCRCErrCtr & 0xFF00) >> 8);
             MBS_TX_DIAG_DATA_L = (uint8_t) (p_mb->StatCRCErrCtr & 0x00FF);
             p_mb->Err           = MODBUS_ERR_NONE;
             break;

        case MODBUS_FC08_LOOPBACK_BUS_EXCEPT_CTR:     	    /* Return exception count in diag information field. */
             MBS_TX_DIAG_DATA_H = (uint8_t)((p_mb->StatExceptCtr & 0xFF00) >> 8);
             MBS_TX_DIAG_DATA_L = (uint8_t) (p_mb->StatExceptCtr & 0x00FF);
             p_mb->Err           = MODBUS_ERR_NONE;
             break;

        case MODBUS_FC08_LOOPBACK_SLAVE_MSG_CTR:      	    /* Return slave message count in diag information field. */
             MBS_TX_DIAG_DATA_H = (uint8_t)((p_mb->StatSlaveMsgCtr & 0xFF00) >> 8);
             MBS_TX_DIAG_DATA_L = (uint8_t) (p_mb->StatSlaveMsgCtr & 0x00FF);
             p_mb->Err           = MODBUS_ERR_NONE;
             break;

        case MODBUS_FC08_LOOPBACK_SLAVE_NO_RESP_CTR: 	    /* Return no response count in diag information field. */
             MBS_TX_DIAG_DATA_H = (uint8_t)((p_mb->StatNoRespCtr & 0xFF00) >> 8);
             MBS_TX_DIAG_DATA_L = (uint8_t) (p_mb->StatNoRespCtr & 0x00FF);
             p_mb->Err           = MODBUS_ERR_NONE;
             break;

        default:
             p_mb->Err = MODBUS_ERR_FC08_01;
             modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_VAL);
             break;
    }

    return true;          		    /* Tell caller that we need to send a response */
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_fc15_write_coil_multiple()
 * Description: Processes the MODBUS "Force Multiple COILS" command and writes the COIL states.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     1) RX command format:             Example:
 *                 <slave address>                0x11
 *                 <function code>                0x0F
 *                 <Coil address HI>              0x00
 *                 <Coil address LO>              0x13
 *                 <# coils HI>                   0x00
 *                 <# coils LO>                   0x0A
 *                 <byte count>                   0x02
 *                 <Force Data HI>                0xCD
 *                 <Force Data LO>                0x01
 *                 <Error Check (LRC or CRC)>     0x??
 *              2) TX reply format:               Example:
 *                 <slave address>                0x11
 *                 <function code>                0x0F
 *                 <Coil address HI>              0x00
 *                 <Coil address LO>              0x13
 *                 <# coils HI>                   0x00
 *                 <# coils LO>                   0x0A
 *                 <Error Check (LRC or CRC)>     0x??
 **************************************************************************************************/

#if (MODBUS_CFG_FC15_EN == 1)
static bool modbus_slave_fc15_write_coil_multiple(MODBUS_t *p_mb)
{
    uint16_t ix, coil, nbr_coils, nbr_bytes, err;
    uint8_t  data_ix, temp;
    bool     coil_val;

    if (p_mb->WrEn == true)
    {
        if (p_mb->RxFrameNDataBytes < 6)      			    /* Minimum Nbr of data bytes must be 6. */
            return false;                          		    /* Tell caller that we DON'T need to send a response */

        coil      = MBS_RX_DATA_START;
        nbr_coils = MBS_RX_DATA_POINTS;
        nbr_bytes = MBS_RX_DATA_BYTES;                    	/* Get the byte count for the data. */
        if (((((nbr_coils - 1) / 8) + 1) ==  nbr_bytes) && 	/* Be sure #bytes valid for number COILS. */
            (p_mb->RxFrameNDataBytes  == (nbr_bytes + 5)))
        {
            ix      = 0;                                  	/* Initialize COIL/loop counter variable. */
            data_ix = 7;                                   	/* The 1st COIL data byte is 5th element in data frame. */

            while (ix < nbr_coils)  						/* Loop through each COIL to be forced. */
            {
                if ((ix % 8) == 0)    						/* Move to the next data byte after every eight bits. */
                    temp = p_mb->RxFrameData[data_ix++];

                if (temp & 0x01)                          	/* Get LSBit */
                    coil_val = MODBUS_COIL_ON;
                else
                    coil_val = MODBUS_COIL_OFF;

                modbus_write_coil(coil + ix, coil_val, &err);

                switch (err)
                {
                    case MODBUS_ERR_NONE:
                         break;                         	/* Continue with the next coil if no error */

                    case MODBUS_ERR_RANGE:
                    default:
                         p_mb->Err = MODBUS_ERR_FC15_01;
                         modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                         return true;                 	    /* Tell caller that we need to send a response */
                }

                temp >>= 1;    				                /* Shift the data one bit position to the right. */
                ix++;             			                /* Increment the COIL counter. */
            }
        }
        else
        {
            p_mb->Err = MODBUS_ERR_FC15_02;
            modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_VAL);
            return true;                            		/* Tell caller that we need to send a response */
        }

        p_mb->TxFrameNDataBytes = 4;                    	/* Don't echo the whole message back! */
        MBS_TX_FRAME_ADDR      = MBS_RX_FRAME_ADDR;       	/* Prepare response packet */
        MBS_TX_FRAME_FC        = MBS_RX_FRAME_FC;
        MBS_TX_DATA_START_H    = MBS_RX_DATA_START_H;
        MBS_TX_DATA_START_L    = MBS_RX_DATA_START_L;
        MBS_TX_DATA_POINTS_H   = MBS_RX_DATA_POINTS_H;
        MBS_TX_DATA_POINTS_L   = MBS_RX_DATA_POINTS_L;
        p_mb->Err = MODBUS_ERR_NONE;
    }
    else
    {
        p_mb->Err = MODBUS_ERR_FC15_03;              		/* Number of bytes incorrect for number of COILS. */
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_VAL);
    }

    return true;    			    /* Tell caller that we need to send a response */
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_fc16_write_holding_register_multiple()
 * Description: This function is called to write to multiple holding registers.  If the address of the
 *              rquest exceeds or is equal to MODBUS_CFG_FP_START_IX, then the command would write to
 *              multiple 'floating-point' according to the 'Daniels Flow Meter' extensions.  This means
 *              that each register requested is considered as a 32-bit IEEE-754 floating-point format.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     1) RX command format:             Example:
 *                 <slave address>                0x11
 *                 <function code>                0x10
 *                 <start address HI>             0x00
 *                 <start address LO>             0x01
 *                 <# registers HI>               0x00
 *                 <# registers LO>               0x02
 *                 <byte count>                   0x04
 *                 <Register value HI>            0x00
 *                 <Register value LO>            0x0A
 *                 <Register value HI>            0x01
 *                 <Register value LO>            0x02
 *                 <Error Check (LRC or CRC)>     0x??
 *              2) TX reply format:               Example:
 *                 <slave address>                0x11
 *                 <function code>                0x10
 *                 <start address HI>             0x00
 *                 <start address LO>             0x01
 *                 <# registers HI>               0x00
 *                 <# registers LO>               0x02
 *                 <Error Check (LRC or CRC)>     0x??
 **************************************************************************************************/

#if (MODBUS_CFG_FC16_EN == 1)
static bool modbus_slave_fc16_write_holding_register_multiple(MODBUS_t *p_mb)
{
    uint8_t *prx_data, data_size;
    uint16_t err, reg, reg_val_16, nbr_regs, nbr_bytes;
#if (MODBUS_CFG_FP_EN == 1)
    uint8_t *pfp, i;
    float    reg_val_fp;
#endif

    reg      = MBS_RX_DATA_START;
    nbr_regs = MBS_RX_DATA_POINTS;

#if (MODBUS_CFG_FP_EN == 1)
    if (reg < MODBUS_CFG_FP_START_IX)
    {
        if (nbr_regs == 0 || nbr_regs > 125)			    /* Make sure we don't exceed the allowed limit per request */
        {
            p_mb->Err = MODBUS_ERR_FC16_04;
            modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
            return true;                      		        /* Tell caller that we need to send a response */
        }

        data_size  = sizeof(uint16_t);
    }
    else
    {
        if (nbr_regs == 0 || nbr_regs > 62)				    /* Make sure we don't exceed the allowed limit per request */
        {
            p_mb->Err = MODBUS_ERR_FC16_05;
            modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
            return true;                       		        /* Tell caller that we need to send a response */
        }

        data_size  = sizeof(float);
    }

#else
    if (nbr_regs == 0 || nbr_regs > 125) 				    /* Make sure we don't exceed the allowed limit per request */
    {
        p_mb->Err = MODBUS_ERR_FC16_04;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
        return true;                         			    /* Tell caller that we need to send a response */
    }

    data_size  = sizeof(uint16_t);

#endif

    prx_data  = &p_mb->RxFrameData[6];            		    /* Point to number of bytes in request frame */
    nbr_bytes = (uint16_t)*prx_data++;

    if ((p_mb->RxFrameNDataBytes - 5) != nbr_bytes) 	    /* Compare actual number of bytes to what they say. */
    {
        p_mb->Err = MODBUS_ERR_FC16_01;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
        return true;
    }

    if ((nbr_bytes / nbr_regs) != (uint16_t)data_size)
    {
        p_mb->Err = MODBUS_ERR_FC16_02;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_VAL);
        return true;                        			    /* Tell caller that we need to send a response */
    }

    while (nbr_regs > 0)
    {
#if (MODBUS_CFG_FP_EN == 1)
        if (reg < MODBUS_CFG_FP_START_IX)
        {
            reg_val_16  = ((uint16_t)*prx_data++) << 8;     /* Get MSB first. */
            reg_val_16 +=  (uint16_t)*prx_data++;           /* Add in the LSB. */
            modbus_write_holding_register(reg, reg_val_16, &err);
        }
        else
        {
            pfp = (uint8_t *)&reg_val_fp;
            pfp += sizeof(float) - 1;
            for (i = 0; i < sizeof(float); i++)
                *pfp--   = *prx_data++;
            modbus_write_holding_register_float(reg, reg_val_fp, &err);
        }

#else
        reg_val_16  = ((uint16_t)*prx_data++) << 8;         /* Get MSB first. */
        reg_val_16 +=  (uint16_t)*prx_data++;               /* Add in the LSB. */
        modbus_write_holding_register(reg, reg_val_16, &err);

#endif

        switch (err)                       				    /* See if any errors in writing the data */
        {
            case MODBUS_ERR_NONE:               		    /* Reply with echoe of command received */
                 p_mb->WrCtr++;
                 reg++;
                 nbr_regs--;
                 break;

            case MODBUS_ERR_RANGE:
            default:
                 p_mb->Err = MODBUS_ERR_FC16_03;
                 modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                 return true;                   		    /* Tell caller that we need to send a response */
        }
    }

    p_mb->TxFrameNDataBytes = 4;                    	    /* Don't echo the whole message back! */
    MBS_TX_FRAME_ADDR    = MBS_RX_FRAME_ADDR;       	    /* Prepare response packet */
    MBS_TX_FRAME_FC      = MBS_RX_FRAME_FC;
    MBS_TX_DATA_START_H  = MBS_RX_DATA_START_H;
    MBS_TX_DATA_START_L  = MBS_RX_DATA_START_L;
    MBS_TX_DATA_POINTS_H = MBS_RX_DATA_POINTS_H;
    MBS_TX_DATA_POINTS_L = MBS_RX_DATA_POINTS_L;
    
    return true;                /* Tell caller that we need to send a response */
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_fc20_read_file()
 * Description: Read a record from a file.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     1) The current version of this software only supports ONE Sub-request at a time.
 *              2) RX command format:             Example:
 *                 <slave address>                0x11
 *                 <function code>                0x14
 *                 <byte count>                   0x0E
 *                 <Reference Type>               0x06
 *                 <File number HI>               0x00
 *                 <File number LO>               0x04
 *                 <Starting address HI>          0x00
 *                 <Starting address LO>          0x01
 *                 <Register count HI>            0x00
 *                 <Register count LO>            0x02
 *                 <Error Check (LRC or CRC)>     0x??
 *              3) TX reply format:               Example:
 *                 <slave address>                0x11
 *                 <function code>                0x14
 *                 <byte count>                   0x0C
 *                 <start address HI>             0x00
 *                 <Reference Type>               0x06
 *                 <Register data HI>             0x0D
 *                 <Register data LO>             0xFE
 *                 <Register data HI>             0x00
 *                 <Register data LO>             0x20
 *                 <Error Check (LRC or CRC)>     0x??
 **************************************************************************************************/

#if (MODBUS_CFG_FC20_EN  == 1)
static bool modbus_slave_fc20_read_file(MODBUS_t *p_mb)
{
    uint8_t *presp, cmd_type;
    uint16_t file_nbr, record_nbr, record_len, cmd_len, err, reg_val, ix;

    cmd_len = p_mb->RxFrameData[2];                        	/* Get the number of bytes in the command received */
    if (cmd_len < 7 || cmd_len > 245)
    {                                      					/* Make sure the byte count Rx'd is within expected range */
        p_mb->Err = MODBUS_ERR_FC20_01;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
        return true;                                    	/* Tell caller that we need to send a response */
    }

    cmd_type   =  p_mb->RxFrameData[3];                		/* Get the reference type */
    file_nbr   = ((uint16_t)p_mb->RxFrameData[4] << 8) +   	/* Get the file number */
                  (uint16_t)p_mb->RxFrameData[5];
    record_nbr = ((uint16_t)p_mb->RxFrameData[6] << 8) +   	/* Get the record number */
                  (uint16_t)p_mb->RxFrameData[7];
    record_len = ((uint16_t)p_mb->RxFrameData[8] << 8) +  	/* Get the record length */
                  (uint16_t)p_mb->RxFrameData[9];
    presp      = &p_mb->TxFrameData[0];                  	/* Point to first location in response buffer */
    *presp++   = MBS_RX_FRAME_ADDR;                       	/* Reply back with the node address */
    *presp++   = MBS_RX_FRAME_FC;                         	/* Include the function code */

    if (cmd_type == 6)										/* File type should ALWAYS be 6. */
    {
        p_mb->TxFrameNDataBytes = record_len * sizeof(uint16_t) + 3;	/* Determine the total number of data bytes in the Tx frame */
        *presp++ = (uint8_t)(p_mb->TxFrameNDataBytes - 1);   			/* Total byte count (excluding byte count) */
        *presp++ = (uint8_t)(p_mb->TxFrameNDataBytes - 2);   			/* Sub request byte count (excluding sub-request byte cnt) */
        *presp++ = 6;                                          			/* Reference type is ALWAYS 6. */
        ix       = 0;                                          			/* Initialize the index into the record */

        while (record_len > 0)
        {
            reg_val = modbus_read_file(file_nbr, record_nbr, ix, record_len, &err);	/* Get one value from the file */
            switch (err)
            {
                case MODBUS_ERR_NONE:
                     *presp++ = (uint8_t)(reg_val >> 8);     	/* Store high byte of record data */
                     *presp++ = (uint8_t)(reg_val & 0x00FF);  	/* Store low  byte of record data */
                     break;

                case MODBUS_ERR_FILE:
                     p_mb->Err = MODBUS_ERR_FC20_02;
                     modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                     return true;                         	    /* Tell caller that we need to send a response */

                case MODBUS_ERR_RECORD:
                     p_mb->Err = MODBUS_ERR_FC20_03;
                     modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                     return true;                           	/* Tell caller that we need to send a response */

                case MODBUS_ERR_IX:
                default:
                     p_mb->Err = MODBUS_ERR_FC20_04;
                     modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                     return true;                           	/* Tell caller that we need to send a response */
            }

            ix++;
            record_len--;
        }
    }
    else
    {
        p_mb->Err = MODBUS_ERR_FC20_05;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
        return true;                      	/* Tell caller that we need to send a response */
    }

    p_mb->Err = MODBUS_ERR_NONE;
    return true;                          	/* Tell caller that we need to send a response */
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_fc21_write_file()
 * Description: Write a record to a file.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   true        If a response needs to be sent
 *              false       If not
 * Caller(s):   modbus_slave_fcxx_handler().
 * Note(s):     1) The current version of this software only supports ONE Sub-request at a time.
 *              2) RX command format:             Example:
 *                 <slave address>                0x11
 *                 <function code>                0x15
 *                 <byte count>                   0x0D
 *                 <Reference Type>               0x06
 *                 <File number HI>               0x00
 *                 <File number LO>               0x04
 *                 <Starting address HI>          0x00
 *                 <Starting address LO>          0x07
 *                 <Register count HI>            0x00
 *                 <Register count LO>            0x03
 *                 <Register data HI>             0x06
 *                 <Register data LO>             0xAF
 *                 <Register data HI>             0x04
 *                 <Register data LO>             0xBE
 *                 <Register data HI>             0x10
 *                 <Register data LO>             0x0D
 *                 <Error Check (LRC or CRC)>     0x??
 *              3) TX reply format:               Example:
 *                 <slave address>                0x11
 *                 <function code>                0x15
 *                 <byte count>                   0x0D
 *                 <Reference Type>               0x06
 *                 <File number HI>               0x00
 *                 <File number LO>               0x04
 *                 <Starting address HI>          0x00
 *                 <Starting address LO>          0x07
 *                 <Register count HI>            0x00
 *                 <Register count LO>            0x03
 *                 <Register data HI>             0x06
 *                 <Register data LO>             0xAF
 *                 <Register data HI>             0x04
 *                 <Register data LO>             0xBE
 *                 <Register data HI>             0x10
 *                 <Register data LO>             0x0D
 *                 <Error Check (LRC or CRC)>     0x??
 **************************************************************************************************/

#if (MODBUS_CFG_FC21_EN  == 1)
static bool modbus_slave_fc21_write_file (MODBUS_t *p_mb)
{
    uint8_t *prx_data, *pcmd, *presp, cmd_type, max;
    uint16_t file_nbr, record_nbr, record_len, cmd_len, err, reg_val, ix;

    cmd_len = p_mb->RxFrameData[2];
    if (cmd_len < 7 || cmd_len > 245) 						/* Make sure the byte count Rx'd is within expected range */
    {
        p_mb->Err = MODBUS_ERR_FC21_01;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_QTY);
        return true;                                    	/* Tell caller that we need to send a response */
    }

    cmd_type   =  p_mb->RxFrameData[3];                   	/* Get the reference type */
    file_nbr   = ((uint16_t)p_mb->RxFrameData[4] << 8) +  	/* Get the file number */
                  (uint16_t)p_mb->RxFrameData[5];
    record_nbr = ((uint16_t)p_mb->RxFrameData[6] << 8) +    /* Get the record number */
                  (uint16_t)p_mb->RxFrameData[7];
    record_len = ((uint16_t)p_mb->RxFrameData[8] << 8) +    /* Get the record length */
                  (uint16_t)p_mb->RxFrameData[9];
    prx_data   = &p_mb->RxFrameData[10];                    /* Point to first data byte */

    if (cmd_type == 6)                                     	/* File type should ALWAYS be 6. */
    {
        ix = 0;                                             /* Initialize the index into the record */
        while (record_len > 0)
        {
            reg_val  = ((uint16_t)*prx_data++ << 8) & 0xFF00; /* Get data to write to file */
            reg_val |=  (uint16_t)*prx_data++ & 0x00FF;

            modbus_write_file(file_nbr, record_nbr, ix, record_len, reg_val, &err); /* Write one value to the file */

            switch (err)
            {
                case MODBUS_ERR_NONE:
                     p_mb->WrCtr++;
                     break;

                case MODBUS_ERR_FILE:
                     p_mb->Err = MODBUS_ERR_FC21_02;
                     modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                     return true;                          	/* Tell caller that we need to send a response */

                case MODBUS_ERR_RECORD:
                     p_mb->Err = MODBUS_ERR_FC21_03;
                     modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                     return true;                          	/* Tell caller that we need to send a response */

                case MODBUS_ERR_IX:
                default:
                     p_mb->Err = MODBUS_ERR_FC21_04;
                     modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
                     return true;                          	/* Tell caller that we need to send a response */
            }

            ix++;
            record_len--;
        }
    }
    else
    {
        p_mb->Err = MODBUS_ERR_FC21_05;
        modbus_slave_set_response_error(p_mb, MODBUS_ERR_ILLEGAL_DATA_ADDR);
        return true;
    }

    record_len = ((uint16_t)p_mb->RxFrameData[8] << 8) + (uint16_t)p_mb->RxFrameData[9]; /* Get the record length */
    pcmd       = &p_mb->RxFrameData[0];
    presp      = &p_mb->TxFrameData[0];                 /* Point to first location in response buffer */
    max        = (record_len * 2) + 9;

    for (ix = 0; ix < max; ix++)              			/* Copy the request into the transmit packet */
        *presp++ = *pcmd++;

    p_mb->Err = MODBUS_ERR_NONE;
    return true;                /* Tell caller that we need to send a response */
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_stat_init()
 * Description: This function is used to initialize/reset the MODBUS statistics/communications counters.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   none.
 *
 * Caller(s):   modbus_init(),
 *              modbus_slave_fc08_loopback().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_FC08_EN  == 1)
void modbus_slave_stat_init(MODBUS_t *p_mb)
{
    p_mb->StatMsgCtr      = 0;                          /* Initialize all MODBUS event counters. */
    p_mb->StatCRCErrCtr   = 0;
    p_mb->StatExceptCtr   = 0;
    p_mb->StatSlaveMsgCtr = 0;
    p_mb->StatNoRespCtr   = 0;
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_rx_task()
 * Description: Handle either Modbus ASCII or Modbus RTU received packets.
 * Argument(s): p_mb        Specifies the Modbus channel that needs servicing.
 * Return(s):   none.
 *
 * Caller(s):   modbus_rx_task().
 * Note(s):     none.
 **************************************************************************************************/

void modbus_slave_rx_task(MODBUS_t *p_mb)
{
    if (p_mb != (MODBUS_t *)0)
    {
#if (MODBUS_CFG_ASCII_EN == 1)
        if (p_mb->Mode == MODBUS_MODE_ASCII)
            modbus_slave_ascii_task(p_mb);
#endif

#if (MODBUS_CFG_RTU_EN == 1)
        if (p_mb->Mode == MODBUS_MODE_RTU)
            modbus_slave_rtu_task(p_mb);
#endif
    }
}

/**************************************************************************************************
 * function:    modbus_slave_ascii_task()
 * Description: Received a packet that should be encoded for Modbus ASCII mode.  Process request.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   none.
 *
 * Caller(s):   modbus_slave_rx_task().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_ASCII_EN == 1)
static void modbus_slave_ascii_task(MODBUS_t *p_mb)
{
    uint16_t calc_lrc;                   					/* Used for LRC */
    bool     ok, do_reply;

#if (MODBUS_CFG_FC08_EN == 1)
    p_mb->StatMsgCtr++;
#endif
    if (p_mb->RxBufByteCtr >= MODBUS_ASCII_MIN_MSG_SIZE)
    {
        ok = modbus_ascii_rx(p_mb);                   	    /* Extract received command from .RxBuf[] & move to .RxFrameData[] */
        if (ok == true)
        {
            calc_lrc = modbus_ascii_rx_calculate_lrc(p_mb); /* Calculate LRC on received ASCII packet */
            if (calc_lrc != p_mb->RxFrameCRC)               /* If sum of all data plus received LRC is not the same */
            {
    #if (MODBUS_CFG_FC08_EN == 1)
                p_mb->StatCRCErrCtr++;            		    /* then the frame was not received properly. */
                p_mb->StatNoRespCtr++;
    #endif
            }
            else
            {
                do_reply = modbus_slave_fcxx_handler(p_mb); /* Execute received command and formulate a response */
                if (do_reply == true)
                {
                    modbus_ascii_tx(p_mb);             		/* Send back reply. */
                }
    #if (MODBUS_CFG_FC08_EN == 1)
                else
                    p_mb->StatNoRespCtr++;
    #endif
            }
        }
    #if (MODBUS_CFG_FC08_EN == 1)
        else
            p_mb->StatNoRespCtr++;
    #endif
    }

    p_mb->RxBufByteCtr = 0;
    p_mb->RxBufPtr     = &p_mb->RxBuf[0];
}
#endif

/**************************************************************************************************
 * function:    modbus_slave_rtu_task()
 * Description: This function processes a packet received on the Modbus channel assuming that
 *              it's an RTU packet.
 * Argument(s): p_mb        Is a pointer to the Modbus channel's data structure.
 * Return(s):   none.
 *
 * Caller(s):   modbus_slave_rtu_task().
 * Note(s):     none.
 **************************************************************************************************/

#if (MODBUS_CFG_RTU_EN == 1)
static void modbus_slave_rtu_task(MODBUS_t *p_mb)
{
    uint16_t calc_crc;                             			/* Used for CRC */
    bool     ok, do_reply;

#if (MODBUS_CFG_FC08_EN == 1)
    p_mb->StatMsgCtr++;
#endif

    if (p_mb->RxBufByteCtr >= MODBUS_RTU_MIN_MSG_SIZE)
    {
        ok = modbus_rtu_rx(p_mb);                       	/* Extract received command from .RxBuf[] & move to .RxFrameData[] */

        if (ok == true)
        {
            calc_crc = modbus_rtu_rx_calculate_crc(p_mb);   /* Do our own calculation of the CRC. */
            
            if (calc_crc != p_mb->RxFrameCRC)               /* If the calculated CRC does not match the CRC received, */
            {         
    #if (MODBUS_CFG_FC08_EN == 1)
                p_mb->StatCRCErrCtr++;                  	/* then the frame is bad. */
                p_mb->StatNoRespCtr++;
    #endif
            }
            else
            {
                do_reply = modbus_slave_fcxx_handler(p_mb); /* Execute received command and formulate a response */
                
                if (do_reply == true)
                    modbus_rtu_tx(p_mb);                    /* Send back reply. */
    #if (MODBUS_CFG_FC08_EN == 1)
                else
                    p_mb->StatNoRespCtr++;
    #endif
            }
        }
    }

    p_mb->RxBufByteCtr = 0;
    p_mb->RxBufPtr     = &p_mb->RxBuf[0];
}
#endif

#endif // #if (MODBUS_CFG_SLAVE_EN == 1)

//-----------------------------------------------------------------------------
/*
 * @@ END
 */

