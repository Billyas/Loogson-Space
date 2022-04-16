/**************************************************************************************************
 * MODBUS CONFIGURATION
 *
 * Filename: mb_cfg.h
 * Version:
 **************************************************************************************************/

/*
 * DEBUG MODE
 */
#define MODBUS_DEBUG            1
#if (MODBUS_DEBUG)
#define DBGPRINT(fmt, ...)      printk(fmt, ##__VA_ARGS__ )
#else
#define DBGPRINT(fmt, ...)
#endif

/**************************************************************************************************
 * MODBUS MODULES CONFIGURATION
 **************************************************************************************************/

#define MODBUS_CFG_SLAVE_EN 		0 		/* Enable or Disable Modbus Slave */
#define MODBUS_CFG_MASTER_EN 		1  		/* Enable or Disable Modbus Master */

/**************************************************************************************************
 * MODBUS MODES CONFIGURATION
 **************************************************************************************************/

#define MODBUS_CFG_ASCII_EN  		1     	/* Modbus ASCII is supported when 1 */
#define MODBUS_CFG_RTU_EN    		0     	/* Modbus RTU   is supported when 1 */

/**************************************************************************************************
 * MODBUS COMMUNICATION CONFIGURATION
 **************************************************************************************************/

#define MODBUS_CFG_CHNL_MAX   		5    	/* Maximum number of Modbus channels. */
#define MODBUS_CFG_BUF_SIZE   		255   	/* Maximum outgoing message size. */

/**************************************************************************************************
 * MODBUS FLOATING POINT SUPPORT
 **************************************************************************************************/

#define MODBUS_CFG_FP_EN    		1     	/* Enable Floating-Point support. */
#if (MODBUS_CFG_FP_EN == 1)
#define MODBUS_CFG_FP_START_IX		5000  	/* Start address of Floating-Point registers */
#else
#define MODBUS_CFG_FP_START_IX		65500 	/* Floating point is disabled, set start of ... */
                                            /* ...FP very high */
#endif

/**************************************************************************************************
 * MODBUS FUNCTIONS CONFIGURATION
 **************************************************************************************************/

#define MODBUS_CFG_FC01_EN   		1     	/* Enable or Disable support for Modbus functions */
#define MODBUS_CFG_FC02_EN       	1
#define MODBUS_CFG_FC03_EN       	1
#define MODBUS_CFG_FC04_EN       	1
#define MODBUS_CFG_FC05_EN       	1
#define MODBUS_CFG_FC06_EN       	1
#define MODBUS_CFG_FC08_EN       	1
#define MODBUS_CFG_FC15_EN        	1
#define MODBUS_CFG_FC16_EN       	1
#define MODBUS_CFG_FC20_EN        	1
#define MODBUS_CFG_FC21_EN      	1

/*
 * @@ End of config
 */

