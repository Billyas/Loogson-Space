/*
 * MODBUS TARGET SPECIFIC DATA ACCESS FUNCTIONS (Template)
 *
 *   Filename: mb_data.c
 *   Version:
 */

/**************************************************************************************************
 * 用户的从机代码在这里实现
 */

#include <stdint.h>

#include "mb_cfg.h"
#include "../src/mb.h"

#if (MODBUS_CFG_SLAVE_EN)

//-------------------------------------------------------------------------------------------------
// CPU Critical
//-------------------------------------------------------------------------------------------------

#if defined(OS_RTTHREAD)
#include "rtthread.h"
#define CPU_CRITICAL_ENTER()   do { rt_enter_critical(); } while (0)
#define CPU_CRITICAL_EXIT()    do { rt_exit_critical(); } while (0)
#elif defined(OS_UCOS)
#include "os.h"
#define CPU_CRITICAL_ENTER()   do { OS_ENTER_CRITICAL(); } while (0)
#define CPU_CRITICAL_EXIT()    do { OS_EXIT_CRITICAL(); } while (0)
#elif defined(OS_FREERTOS)
#include "FreeRTOS.h"
#define CPU_CRITICAL_ENTER()   do { vTaskEnterCritical(); } while (0)
#define CPU_CRITICAL_EXIT()    do { vTaskExitCritical(); } while (0)
#else // BARE PROGRAM
#define CPU_CRITICAL_ENTER()
#define CPU_CRITICAL_EXIT()
#endif

//-------------------------------------------------------------------------------------------------

/**************************************************************************************************
 * function:    GET THE VALUE OF A SINGLE COIL
 * Description: This function returns the value of a single coil.
 *              It is called by 'modbus_slave_fc01_read_coil()'.
 *              You must 'map' the 'coil' to the actual application's coil.
 * Arguments:   coil     is the coil number that is being requested.
 *              perr     is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified coil is valid and you are returning its value.
 *                MODBUS_ERR_RANGE    the specified coil is an invalid coil number in your
 *                                    application (i.e. product).  YOUR product defines what the
 *                                    valid range of values is for the 'coil' argument.
 *
 * Note(s):     You can perform the mapping of coil number to application coils directly in this
 *              function or via a table lookup.  A table lookup would make sense if you had a lot
 *              of coils in your product.
 **************************************************************************************************/

#if (MODBUS_CFG_FC01_EN == 1)
bool modbus_read_coil(uint16_t coil, uint16_t *perr)
{
    bool coil_val;

    switch (coil)
    {
        case 0:
             coil_val = true;
             break;

        case 1:
             coil_val = false;
             break;

        case 2:
             coil_val = true;
             break;

        default:
        case 3:
             coil_val = false;
             break;
    }

    *perr = MODBUS_ERR_NONE;
    return coil_val;
}
#endif

/**************************************************************************************************
 * function:    SET THE VALUE OF A SINGLE COIL
 * Description: This function changes the value of a single coil. It is called by
 *              'modbus_slave_fc05_write_coil()' and 'modbus_slave_fc15_write_coil_multiple()'.
 *              You must 'map' the 'coil' to the actual application's coil.
 * Arguments:   coil      is the coil number that needs to be changed.
 *              coil_val  is the desired value of the coil.  This value can be either true or false
 *                        with true indicating an energized coil.
 *              perr      is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified coil is valid and your code changed the value
 *                                    of the coil.
 *                MODBUS_ERR_RANGE    the specified coil is an invalid coil number in your
 *                                    application (i.e. product).  YOUR product defines what the
 *                                    valid range of values is for the 'coil' argument.
 *                MODBUS_ERR_WR       if the device is not able to write or accept the value
 *
 * Note(s):     You can perform the mapping of coil number to application coils directly in this
 *              function or via a table lookup.  A table lookup would make sense if you had a lot
 *              of coils in your product.
 **************************************************************************************************/

#if (MODBUS_CFG_FC05_EN == 1) || (MODBUS_CFG_FC15_EN == 1)
void modbus_write_coil(uint16_t coil, bool coil_val, uint16_t *perr)
{
    (void)coil;
    (void)coil_val;
    *perr = MODBUS_ERR_NONE;
}
#endif

/**************************************************************************************************
 * function:    GET THE VALUE OF A SINGLE DISCRETE INPUT
 * Description: This function reads the value of a single DI (DI means Discrete Input).
 *              It is called by 'modbus_slave_fc02_read_di()'.
 *              You must 'map' the 'di'  to the actual application's DI.
 * Arguments:   di        is the Discrete Input number that needs to be read.
 *              perr      is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified DI is valid and your code is returning its
 *                                    current value.
 *                MODBUS_ERR_RANGE    the specified DI is an invalid Discrete Input number in your
 *                                    application (i.e. product).  YOUR product defines what the
 *                                    valid range of values is for the 'di' argument.
 *
 * Note(s):     You can perform the mapping of DI number to the application DIs directly in this
 *              function or via a table lookup.  A table lookup would make sense if you had
 *              a lot of Discrete Inputs in your product.
 **************************************************************************************************/

#if (MODBUS_CFG_FC02_EN == 1)
bool modbus_read_di(uint16_t di, uint16_t *perr)
{
    (void)di;
    *perr = MODBUS_ERR_NONE;
    return false;
}
#endif

/**************************************************************************************************
 * function:    GET THE VALUE OF A SINGLE INPUT REGISTER
 * Description: This function reads the value of a single Input Register.
 *              It is called by 'modbus_slave_fc04_read_in_register()' when the argument 'reg' is
 *              BELOW the value set by the configuration constant MODBUS_CFG_FP_START_IX
 *              (see MB_CFG.H).
 *              You must 'map' the Input Register to the actual application's corresponding
 *              integer register.
 * Arguments:   reg       is the Input Register number that needs to be read.
 *              perr      is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified input register is valid and your code is
 *                                    returning its current value.
 *                MODBUS_ERR_RANGE    the specified input register is an invalid number in your
 *                                    application (i.e. product).  YOUR product defines what the
 *                                    valid range of values is for the 'reg' argument.
 *
 * Note(s):     1) You can perform the mapping of input register number to the application's
 *                 input registers directly in this function or via a table lookup.
 *                 A table lookup would make sense if you had a lot of Input Registers
 *                 in your product.
 *              2) If your product doesn't have input registers, you could simply set '*err' to
 *                 MODBUS_ERR_NONE and return 0.
 **************************************************************************************************/

#if (MODBUS_CFG_FC04_EN == 1)
uint16_t modbus_read_in_register(uint16_t reg, uint16_t *perr)
{
    uint16_t val;
#if defined(OS_UCOS)
    uint32_t cpu_sr;
#endif

    switch (reg)
    {
        case 0:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)0x5555;
             CPU_CRITICAL_EXIT();
             break;

        case 1:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)0xAAAA;
             CPU_CRITICAL_EXIT();
             break;

        case 2:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)0x5A5A;
             CPU_CRITICAL_EXIT();
             break;

        case 3:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)0xA5A5;
             CPU_CRITICAL_EXIT();
             break;

        case 4:
             CPU_CRITICAL_ENTER();
             val = sizeof(MODBUS_t);
             CPU_CRITICAL_EXIT();
             break;

        case 5:
             CPU_CRITICAL_ENTER();
             val = 0;
             CPU_CRITICAL_EXIT();
             break;

        default:
             val = 0;
             break;
    }

    *perr = MODBUS_ERR_NONE;
    return val;
}
#endif

/**************************************************************************************************
 * function:    GET THE VALUE OF A SINGLE 'FLOATING-POINT' INPUT REGISTER
 * Description: This function reads the value of a single Input Register.
 *              It is called by 'modbus_slave_fc04_read_in_register()' when the argument 'reg' is
 *              ABOVE or equal to the value set the configuration constant MODBUS_CFG_FP_START_IX
 *              (see MB_CFG.H).
 *              You must 'map' the Input Register to the actual application's corresponding
 *              floating-point register.
 * Arguments:   reg       is the Input Register number that needs to be read.
 *              perr      is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified input register is valid and your code is
 *                                    returning its current value.
 *                MODBUS_ERR_RANGE    the specified input register is an invalid number in your
 *                                    application (i.e. product).  YOUR product defines what the
 *                                    valid range of values is for the 'reg' argument.
 *
 * Note(s):     1) You can perform the mapping of input register number to the application's input
 *                 registers directly in this function or via a table lookup.  A table lookup would
 *                 make sense if you had a lot of Input Registers in your product.
 *              2) If your product doesn't have input registers, you could simply set '*err' to
 *                 MODBUS_ERR_NONE and return (float)0.
 **************************************************************************************************/

#if (MODBUS_CFG_FC04_EN == 1) && (MODBUS_CFG_FP_EN == 1)
float modbus_read_in_register_float(uint16_t reg, uint16_t *perr)
{
    (void)reg;
    *perr = MODBUS_ERR_NONE;
    return (float)0;
}
#endif

/**************************************************************************************************
 * function:    GET THE VALUE OF A SINGLE HOLDING REGISTER
 * Description: This function reads the value of a single Holding Register.
 *              It is called by 'modbus_slave_fc03_read_holding_register()' when the argument 'reg'
 *              is BELOW the value set by the configuration constant MODBUS_CFG_FP_START_IX
 *              (see MB_CFG.H).
 *              You must 'map' the Holding Register to the actual application's corresponding
 *              integer register.
 * Arguments:   reg       is the Holding Register number that needs to be read.
 *              perr      is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified holding register is valid and your code is
 *                                    returning its current value.
 *                MODBUS_ERR_RANGE    the specified holding register is an invalid number in your
 *                                    application (i.e. product).  YOUR product defines what the
 *                                    valid range of values is for the 'reg' argument.
 *
 * Note(s):     1) You can perform the mapping of holding register number to the application's
 *                 holding registers directly in this function or via a table lookup.  A table
 *                 lookup would make sense if you had a lot of Holding Registers in your product.
 *              2) If your product doesn't have holding registers, you could simply set '*err' to
 *                 MODBUS_ERR_NONE and return 0.
 **************************************************************************************************/

#if (MODBUS_CFG_FC03_EN == 1)
uint16_t modbus_read_holding_register(uint16_t reg, uint16_t *perr)
{
    uint16_t val;
#if defined(OS_UCOS)
    uint32_t cpu_sr;
#endif

    switch (reg)
    {
        case 0:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)0xDEAD;
             CPU_CRITICAL_EXIT();
             break;

        case 1:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)0xBEAF;
             CPU_CRITICAL_EXIT();
             break;

        case 2:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)0x55AA;
             CPU_CRITICAL_EXIT();
             break;

        case 3:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)0xAA55;
             CPU_CRITICAL_EXIT();
             break;

        case 4:
             CPU_CRITICAL_ENTER();
             val = sizeof(MODBUS_t);
             CPU_CRITICAL_EXIT();
             break;

        case 5:
             CPU_CRITICAL_ENTER();
             val = 0;
             CPU_CRITICAL_EXIT();
             break;

        case 6:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)((mb_devices_tbl[0].RxCtr / 1000) & 0x0000FFFF);
             CPU_CRITICAL_EXIT();
             break;

        case 7:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)((mb_devices_tbl[0].RxCtr % 1000) & 0x0000FFFF);
             CPU_CRITICAL_EXIT();
             break;

        case 8:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)((mb_devices_tbl[0].TxCtr / 1000) & 0x0000FFFF);
             CPU_CRITICAL_EXIT();
             break;

        case 9:
             CPU_CRITICAL_ENTER();
             val = (uint16_t)((mb_devices_tbl[0].TxCtr % 1000) & 0x0000FFFF);
             CPU_CRITICAL_EXIT();
             break;

        default:
             val = 0;
             break;
    }

    *perr = MODBUS_ERR_NONE;
    return (val);
}
#endif

/**************************************************************************************************
 * function:    GET THE VALUE OF A SINGLE 'FLOATING-POINT' HOLDING REGISTER
 * Description: This function reads the value of a single Floating-Point Holding Register.
 *              It is called by 'modbus_slave_fc03_read_holding_register()' when the argument
 *              'reg' is ABOVE or equal to the value set by the configuration constant
 *              MODBUS_CFG_FP_START_IX (see MB_CFG.H).
 *              You must 'map' the Holding Register to the actual application's corresponding
 *              floating-point register.
 * Arguments:   reg       is the Holding Register number that needs to be read.
 *              perr      is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified holding register is valid and your code is
 *                                    returning its current value.
 *                MODBUS_ERR_RANGE    the specified holding register is an invalid number in your
 *                                    application (i.e. product).  YOUR product defines what the
 *                                    valid range of values is for the 'reg' argument.
 *
 * Note(s):     1) You can perform the mapping of holding register number to the application's
 *                 holding registers directly in this function or via a table lookup.  A table
 *                 lookup would make sense if you had a lot of Holding Registers in your product.
 *              2) If your product doesn't have holding registers, you could simply set '*err' to
 *                 MODBUS_ERR_NONE and return 0.
 **************************************************************************************************/

#if (MODBUS_CFG_FC03_EN == 1) && (MODBUS_CFG_FP_EN == 1)
float modbus_read_holding_register_float(uint16_t reg, uint16_t *perr)
{
    (void)reg;
    *perr = MODBUS_ERR_NONE;
    return ((float)0);
}
#endif

/**************************************************************************************************
 * function:    SET THE VALUE OF A SINGLE HOLDING REGISTER
 * Description: This function is called to change the value of a single Integer Holding Register.
 *              It is called by 'modbus_slave_fc06_write_holding_register()' and
 *              'modbus_slave_fc16_write_holding_register_multiple()' when the argument 'reg' is
 *              BELOW to the value set by the configuration constant MODBUS_CFG_FP_START_IX
 *              (see MB_CFG.H).
 *              You must 'map' the Holding Register to the actual application's corresponding
 *              integer register.
 * Arguments:   reg       is the Holding Register number that needs to be read.
 *              reg_val   is the desired value of the holding register.
 *                        The value is specified as an unsigned integer even though it could
 *                        actually be represented by a signed integer.
 *              perr      is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified holding register is valid and your code is
 *                                    returning its current value.
 *                MODBUS_ERR_RANGE    the specified holding register is an invalid number in your
 *                                    application (i.e. product).  YOUR product defines what the
 *                                    valid range of values is for the 'reg' argument.
 *                MODBUS_ERR_WR       if the device is not able to write or accept the value
 *
 * Note(s):     1) You can perform the mapping of holding register number to the application's
 *                 holding registers directly in this function or via a table lookup. A table
 *                 lookup would make sense if you had a lot of Holding Registers in your product.
 *              2) If your product doesn't have holding registers, you could simply set '*err' to
 *                 MODBUS_ERR_NONE and return 0.
 **************************************************************************************************/

#if (MODBUS_CFG_FC06_EN == 1) || (MODBUS_CFG_FC16_EN == 1)
void modbus_write_holding_register(uint16_t reg, uint16_t reg_val_16, uint16_t *perr)
{
    /* Access to your variable here! */
    (void)reg;
    (void)reg_val_16;
    *perr = MODBUS_ERR_NONE;
}
#endif

/**************************************************************************************************
 * function:    SET THE VALUE OF A SINGLE 'FLOATING-POINT' HOLDING REGISTER
 * Description: This function is called to change the value of a single Floating-Point Holding
 *              Register. It is called by 'modbus_slave_fc06_write_holding_register()' and
 *              'modbus_slave_fc16_write_holding_register_multiple()' when the argument 'reg' is
 *              ABOVE or equal to the value set by the configuration constant MODBUS_CFG_FP_START_IX
 *              (see MB_CFG.H).
 *              You must 'map' the Holding Register to the actual application's corresponding
 *              floating-point register.
 * Arguments:   reg       is the Holding Register number that needs to be read.
 *              reg_val   is the desired value of the holding register.
 *                        The value is specified as an unsigned integer even though it could
 *                        actually be represented by a signed integer.
 *              perr      is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified holding register is valid and your code is
 *                                    returning its current value.
 *                MODBUS_ERR_RANGE    the specified holding register is an invalid number in your
 *                                    application (i.e. product).  YOUR product defines what the
 *                                    valid range of values is for the 'reg' argument.
 *                MODBUS_ERR_WR       if the device is not able to write or accept the value
 *
 * Note(s):     1) You can perform the mapping of holding register number to the application's
 *                 holding registers directly in this function or via a table lookup. A table
 *                 lookup would make sense if you had a lot of Holding Registers in your product.
 *              2) If your product doesn't have holding registers, you could simply set '*err'
 *                 to MODBUS_ERR_NONE and return 0.
 **************************************************************************************************/

#if ((MODBUS_CFG_FC06_EN == 1) || (MODBUS_CFG_FC16_EN == 1)) && (MODBUS_CFG_FP_EN == 1)
void modbus_write_holding_register_float(uint16_t reg, float reg_val_fp, uint16_t *perr)
{
    (void)reg;
    (void)reg_val_fp;
    *perr = MODBUS_ERR_RANGE;
}
#endif

/**************************************************************************************************
 * function:    GET A SINGLE ENTRY FROM A RECORD IN A FILE
 * Description: This function is called to read a single integer from a file.
 *              As mentionned in the Modbus specifications, a file is an organization of records.
 *              Each file can contain up to 10,000 records (addressed from 0 to 9999).
 *              You must 'map' the File/Record/Ix to the actual application's corresponding data.
 * Arguments:   file_nbr    is the number of the desired file.
 *              record_nbr  is the desired record within the file
 *              ix          is the desired entry in the specified record.
 *              record_len  is the desired length of the record.  Note that this parameter is
 *                          passed to this function to provide the 'requested' requested length
 *                          from the MODBUS command.
 *              perr        is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified file/record/entry is valid and your code is
 *                                    returning its current value.
 *                MODBUS_ERR_FILE     if the specified 'file_nbr' is not a valid file number in
 *                                    your product.
 *                MODBUS_ERR_RECORD   if the specified 'record_nbr' is not a valid record in the
 *                                    specified file.
 *                MODBUS_ERR_IX       if the specified 'ix' is not a valid index into the specified
 *                                    record.
 *
 * Note(s):     You can perform the mapping of file/record/ix to the application's data directly
 *              in this function or via a table lookup.  A table lookup would make sense if you
 *              had a lot data in your files.
 **************************************************************************************************/

#if (MODBUS_CFG_FC20_EN == 1)
uint16_t modbus_read_file(uint16_t file_nbr, uint16_t record_nbr, uint16_t ix,
                          uint8_t record_len, uint16_t *perr)
{
    (void)file_nbr;
    (void)record_nbr;
    (void)ix;
    (void)record_len;
    *perr  = MODBUS_ERR_NONE;
    return (0);
}
#endif

/**************************************************************************************************
 * function:    SET A SINGLE ENTRY OF A RECORD IN A FILE
 * Description: This function is called to change a single integer value in a file.
 *              As mentionned in the Modbus specifications, a file is an organization of records.
 *              Each file can contain up to 10,000 records (addressed from 0 to 9999).
 *              You must 'map' the File/Record/Ix to the actual application's corresponding data.
 * Arguments  : file_nbr    is the number of the desired file.
 *              record_nbr  is the desired record within the file
 *              ix          is the desired entry in the specified record.
 *              record_len  is the desired length of the record.  Note that this parameter is
 *                          passed to this function to provide the 'requested' requested length
 *                          from the MODBUS command.
 *              val         is the new value to place in the file.
 *              perr        is a pointer to an error code variable.  You must either return:
 *
 *                MODBUS_ERR_NONE     the specified file/record/entry is valid and your code is
 *                                    returning its current value.
 *                MODBUS_ERR_FILE     if the specified 'file_nbr' is not a valid file number in
 *                                    your product.
 *                MODBUS_ERR_RECORD   if the specified 'record_nbr' is not a valid record in the
 *                                    specified file.
 *                MODBUS_ERR_IX       if the specified 'ix' is not a valid index into the specified
 *                                    record.
 *
 * Note(s):     You can perform the mapping of file/record/ix to the application's data directly
 *              in this function or via a table lookup.  A table lookup would make sense if you
 *              had a lot data in your files.
 **************************************************************************************************/

#if (MODBUS_CFG_FC21_EN == 1)
void modbus_write_file(uint16_t file_nbr, uint16_t record_nbr, uint16_t ix,
                       uint8_t record_len, uint16_t val, uint16_t *perr)
{
    (void)file_nbr;
    (void)record_nbr;
    (void)ix;
    (void)record_len;
    (void)val;
    *perr = MODBUS_ERR_NONE;
}
#endif

#endif // #if (MODBUS_CFG_SLAVE_EN)

//-----------------------------------------------------------------------------
/*
 * @@ END
 */

