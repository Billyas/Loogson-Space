/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * ls1x_yaffs2.h
 *
 *  Created on: 2014-3-18
 *      Author: Bian
 */

/*
 *	XXX IN PMON ORIGAN
 *	add_mtd_device(ls1g_soc_mtd, 0,          0x00e00000, "kernel");  //   0, 112
 *	add_mtd_device(ls1g_soc_mtd, 0x00e00000, 0x06700000, "os"); 	 // 112, 824
 *	add_mtd_device(ls1g_soc_mtd, 0x07500000, 0x00b00000, "data");	 // 936,  88
 *
 *	FIXME NEW IN PMON:
 *	add_mtd_device(ls1g_soc_mtd, 0,             8*1024*1024, "kernel");  //  0,  64
 *	add_mtd_device(ls1g_soc_mtd, 8*1024*1024, 120*1024*1024, "rtems"); 	 // 64, 960
 *
 */

#ifndef LS1X_YAFFS2_H_
#define LS1X_YAFFS2_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RYFS_MOUNTED_FS_NAME	"/ndd"

/******************************************************************************
 * NAND flash chip type - 128M
 */
#define K9F1G08U0C      0x0C    // NOP=4
#define K9F1G08U0D      0x0D    // NOP=4
#define K9F1G08U0E      0x0E    // NOP=1

/******************************************************************************
 * struct for initialize
 */
typedef struct YAFFS_config
{
	/* likely "struct yaffs_param" */
	unsigned bytes_of_page;             // total_bytes_per_chunk = BYTES_OF_PAGE;
	int      pages_of_block;            // chunks_per_block      = PAGES_OF_BLOCK;
	int      oobbytes_of_page;          // spare_bytes_per_chunk = OOBBYTES_OF_PAGE;
	int      start_block;               // start_block           = 112;
	int      end_block;                 // end_block             = 935;
	int      reserved_blocks;           // n_reserved_blocks     = 5;
	int      cache_blocks;              // n_caches              = 8;
	/* other */
	int      nand_flash_chip;           // K9F1G08U0C/D/E etc
	int      nand_rw_fullpage;          // 1: yes, by nand_flash_chip
	int      register_and_mount_fs;     // 1: mounted as "/ndd"
	int      initialized;               // 1: yes
} YAFFS_config_t;

//*****************************************************************************
//
// 初始化并挂载文件系统   /ndd
//
// 注意: YAFFS_config_t 参数与芯片紧密相关
//
//*****************************************************************************

int yaffs_startup_and_mount(const char *mount_name);

int yaffs_is_running(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LS1X_YAFFS2_H_ */


