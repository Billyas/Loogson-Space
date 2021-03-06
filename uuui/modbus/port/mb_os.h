/**************************************************************************************************
 * MODBUS uC/OS-II LAYER INTERFACE
 *
 * Filename: mb_os.h
 * Version:
 **************************************************************************************************/

#ifndef MB_OS_MODULE_H
#define MB_OS_MODULE_H

/**************************************************************************************************
 * INCLUDE FILES
 **************************************************************************************************/

/**************************************************************************************************
 * DEFINES
 **************************************************************************************************/

/**************************************************************************************************
 * EXTERNS
 **************************************************************************************************/

/**************************************************************************************************
 * CONFIGURATION ERRORS
 **************************************************************************************************/

#if (OS_Q_EN == 0)
#error "MODBUS Slave requires uC/OS-II Message Queue Services."
#endif

#ifndef MB_OS_CFG_RX_TASK_ID
#error "MODBUS Missing Rx Task's MB_OS_CFG_RX_TASK_ID."
#endif

#ifndef MB_OS_CFG_RX_TASK_PRIO
#error "MODBUS Missing Rx Task's MB_OS_CFG_RX_TASK_PRIO."
#endif

#ifndef  MB_OS_CFG_RX_TASK_STK_SIZE
#error "MODBUS Missing Rx Task's MB_OS_CFG_RX_TASK_STK_SIZE."
#endif

#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
#if (OS_SEM_EN  == 0)
#error "MODBUS Master requires uC/OS-II Semaphore Services."
#error "... It needs MODBUS_CFG_MAX_CH semaphores."
#endif
#endif

/**************************************************************************************************
 * MODULE END
 **************************************************************************************************/

#endif /* End of MB_OS module */



