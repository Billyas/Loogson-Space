/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * ls1x_yaffs2.c
 *
 *  Created on: 2014-3-18
 *      Author: Bian
 */

#include "bsp.h"

#if defined(BSP_USE_NAND) && defined(USE_YAFFS2)

#include <stdbool.h>

#include "../yaffs_guts.h"
#include "../yaffs_packedtags2.h"
#include "../yaffs_trace.h"
#include "ls1x_nand.h"
#include "ls1x_yaffs.h"

#include "../direct/yaffsfs.h"

enum
{
	NAND_READ_FLAG  = 0,
	NAND_WRITE_FLAG = 1,
};

extern void nand_correct_page_swecc(unsigned char *buf,
                                    unsigned char *ecc_code,
                                    unsigned int pagesize,
                                    unsigned int *ecc_fixed,
                                    unsigned int *ecc_unfixed);

extern void nand_calculate_page_swecc(const unsigned char *buf,
                                      unsigned char *ecc_code,
                                      unsigned int pagesize);

/**********************************************************************
 * pre-defination
 **********************************************************************/

/* position to mark a bad block in spare
 */
#define BAD_FLAG_OFFSET		0
#define BAD_FLAG_LEN		4		/* it's two, four just for aligned */

/**********************************************************************
 * rtems-yaffs variable
 **********************************************************************/

//static rtems_yaffs_default_os_context	ryfs_os_context;

static struct yaffs_dev     m_YAFFS_device;
struct yaffs_dev*			p_YAFFS_device = NULL;

static YAFFS_config_t       m_YAFFS_config;

/*
 * ls1x NAND device
 */
#define CHECK_NAND_DRV(rt) 	if (NULL == devNAND) return rt;

/**************************************************************************************************
 * ls1x NAND access functions for yaffs
 **************************************************************************************************/

static int ls1x_nand_ReadWrite(unsigned int pagenum,
                               unsigned int column,
                               unsigned int op_flags,
					           const unsigned char *buf,
                               unsigned int buf_size,
                               int rw)
{
    int rt = 0;
	NAND_PARAM_t param;
	
	CHECK_NAND_DRV(-1);

	param.pageNum = pagenum;
	param.colAddr = column;
	param.opFlags = op_flags;

	/*
	 * call nand driver r/w function
	 */
	if (0 == rw)
	{
	    rt = ls1x_nand_read(devNAND, (unsigned char *)buf, (int)buf_size, (void *)&param);
	}
	else
	{
		rt = ls1x_nand_write(devNAND, (unsigned char *)buf, (int)buf_size, (void *)&param);
	}

	return rt;
}

static int ls1x_nand_Initialise(struct yaffs_dev *dev)
{
	CHECK_NAND_DRV(YAFFS_FAIL);

    ls1x_nand_init(devNAND, NULL);    //\\ 注意: 可以放在外面初始化
    
	if (ls1x_nand_open(devNAND, NULL) == 0)
		return YAFFS_OK;
	else
		return YAFFS_FAIL;
}

static int ls1x_nand_Deinitialise(struct yaffs_dev *dev)
{
	CHECK_NAND_DRV(YAFFS_FAIL);

	if (ls1x_nand_close(devNAND, NULL) == 0)
		return YAFFS_OK;
	else
		return YAFFS_FAIL;
}

static int ls1x_nand_EraseBlock(struct yaffs_dev *dev, int flash_block)
{
	CHECK_NAND_DRV(YAFFS_FAIL);

	if (ls1x_nand_ioctl(devNAND, IOCTL_NAND_ERASE_BLOCK, (void *)flash_block) == 0)
		return YAFFS_OK;
	else
		return YAFFS_FAIL;
}

/**************************************************************************************************
 * ls1b yaffs2 mode functions
 **************************************************************************************************/

static int ls1x_Read_ChunkAndTags(struct yaffs_dev *dev,
                                  int nand_chunk,
                                  unsigned char *data,
					              struct yaffs_ext_tags *tags)
{
	unsigned char tempbuf[OOBBYTES_OF_PAGE];
	unsigned int  ecc_offset, ecc_len;
	unsigned int  pt2_offset, pt2_len;
	void 		 *pt2_ptr;
	int 		  rt = 0;
	unsigned int  ecc_fixed = 0, ecc_unfixed = 0;
	struct yaffs_packed_tags2 pt2;
	
	ecc_len = (BYTES_OF_PAGE / 256) * 3;
	ecc_offset = OOBBYTES_OF_PAGE - ecc_len;	// XXX: if no hardecc

	pt2_len = dev->param.no_tags_ecc ? sizeof(pt2.t) : sizeof(pt2);
	pt2_ptr = dev->param.no_tags_ecc ? (void *)&pt2.t : (void *)&pt2;
	pt2_offset = dev->param.inband_tags ?
				 dev->data_bytes_per_chunk : (BAD_FLAG_OFFSET + BAD_FLAG_LEN);

	if (data)
	{
	    if (m_YAFFS_config.nand_rw_fullpage)
	    {
	    	unsigned char fullpage[BYTES_OF_PAGE + OOBBYTES_OF_PAGE];

	    	/*
	    	 * read out main & spare data, include tags or not.
	    	 */
	    	rt = ls1x_nand_ReadWrite(nand_chunk,
	    							 0,
									 NAND_OP_MAIN | NAND_OP_SPARE,
									 fullpage,
									 BYTES_OF_PAGE + OOBBYTES_OF_PAGE,
									 NAND_READ_FLAG);

	    	if (rt != (BYTES_OF_PAGE + OOBBYTES_OF_PAGE))
	    	{
	    		return YAFFS_FAIL;
	    	}

	    	memcpy(data, fullpage, BYTES_OF_PAGE);
	    	memcpy(tempbuf, fullpage + BYTES_OF_PAGE, OOBBYTES_OF_PAGE);
	    }
	    else
	    {
	    	/*
	    	 * read out main data, include tags or not.
	    	 */
	    	rt = ls1x_nand_ReadWrite(nand_chunk,
	    							 0,
									 NAND_OP_MAIN,
									 data,
									 BYTES_OF_PAGE,
									 NAND_READ_FLAG);

	    	if (rt != BYTES_OF_PAGE)
	    	{
	    		return YAFFS_FAIL;
	    	}

	    	/* read out spare data
	    	 */
	    	rt = ls1x_nand_ReadWrite(nand_chunk,
	    							 0,
									 NAND_OP_SPARE,
									 tempbuf,
									 OOBBYTES_OF_PAGE,
									 NAND_READ_FLAG);

	    	if (rt != OOBBYTES_OF_PAGE)
	    	{
	    		return YAFFS_FAIL;
	    	}
	    }
	}
	else if (tags)
	{
		if (dev->param.inband_tags)
		{
			/* only read out tags from main
			 */
			rt = ls1x_nand_ReadWrite(nand_chunk,
					                 pt2_offset,
									 NAND_OP_MAIN,
									 tempbuf,
									 pt2_len,
									 NAND_READ_FLAG);
		}
		else
		{
			/* only read out tags from spare
			 */
			rt = ls1x_nand_ReadWrite(nand_chunk,
					                 pt2_offset,
									 NAND_OP_SPARE,
									 tempbuf,
									 pt2_len,
									 NAND_READ_FLAG);
		}

		if (rt != pt2_len)
		{
			return YAFFS_FAIL;
		}
	}
	else
	{
		return YAFFS_FAIL;
	}

	/* unpack the tags
	 */
	if (tags)
	{
		if (dev->param.inband_tags && data)
		{
			struct yaffs_packed_tags2_tags_only *pt2tp;
			pt2tp = (struct yaffs_packed_tags2_tags_only *)&data[pt2_offset];
			yaffs_unpack_tags2_tags_only(tags, pt2tp);
		}
		else
		{
			memcpy(pt2_ptr, tempbuf, pt2_len);
			yaffs_unpack_tags2(tags, &pt2, !dev->param.no_tags_ecc);
		}
	}

	/* do ecc check
	 */
	rt = 0;
	if (data)
	{
		nand_correct_page_swecc(data, (unsigned char *)(tempbuf + ecc_offset),
								BYTES_OF_PAGE, &ecc_fixed, &ecc_unfixed);
	}

	if (tags && (ecc_unfixed > 0) && (tags->ecc_result == YAFFS_ECC_RESULT_NO_ERROR))
	{
		tags->ecc_result = YAFFS_ECC_RESULT_UNFIXED;
		dev->n_ecc_unfixed += ecc_unfixed;
	}

	if (tags && (ecc_fixed > 0) && (tags->ecc_result == YAFFS_ECC_RESULT_NO_ERROR))
	{
		tags->ecc_result = YAFFS_ECC_RESULT_FIXED;
		dev->n_ecc_fixed += ecc_fixed;
	}

	if (rt == 0)
		return YAFFS_OK;
	else
		return YAFFS_FAIL;
}

static int ls1x_Write_ChunkAndTags(struct yaffs_dev *dev,
                                   int nand_chunk,
                                   const unsigned char *data,
						           const struct yaffs_ext_tags *tags)
{
	unsigned char tempbuf[OOBBYTES_OF_PAGE];
	unsigned int  ecc_offset, ecc_len;
	unsigned int  pt2_offset, pt2_len;
	void 		 *pt2_ptr;
	int 		  rt = 0;
	struct yaffs_packed_tags2 pt2;
	
	ecc_len = (BYTES_OF_PAGE / 256) * 3;
	ecc_offset = OOBBYTES_OF_PAGE - ecc_len;	// XXX: if no hardecc

	pt2_len = dev->param.no_tags_ecc ? sizeof(pt2.t) : sizeof(pt2);
	pt2_ptr = dev->param.no_tags_ecc ? (void *)&pt2.t : (void *)&pt2;
	pt2_offset = dev->param.inband_tags ?
				 dev->data_bytes_per_chunk : (BAD_FLAG_OFFSET + BAD_FLAG_LEN);

	memset((void *)tempbuf, 0xFF, OOBBYTES_OF_PAGE);

	/* For yaffs2 writing there must be both data and tags.
	 * If we're using inband tags, then the tags are stuffed into
	 * the end of the data buffer.
	 */
	if (!data || !tags)
	{
		BUG();
	}
	else if (dev->param.inband_tags)
	{
		struct yaffs_packed_tags2_tags_only *pt2tp;
		pt2tp = (struct yaffs_packed_tags2_tags_only *)&data[pt2_offset];
		yaffs_pack_tags2_tags_only(pt2tp, tags);
	}
	else
	{
		yaffs_pack_tags2(&pt2, tags, !dev->param.no_tags_ecc);

		/* copy pt2 to spare
		 */
		memcpy((void *)(tempbuf + pt2_offset), pt2_ptr, pt2_len);
	}

    if (m_YAFFS_config.nand_rw_fullpage)
	{
		unsigned char fullpage[BYTES_OF_PAGE + OOBBYTES_OF_PAGE];

		/* calculate ecc and write to spare */
		nand_calculate_page_swecc(data, (unsigned char *)(tempbuf + ecc_offset), BYTES_OF_PAGE);

		/* combine to full page. */
		memcpy(fullpage, data, BYTES_OF_PAGE);
		memcpy(fullpage + BYTES_OF_PAGE, tempbuf, OOBBYTES_OF_PAGE);

		/*
		 * write data to main and spare
		 */
		rt = ls1x_nand_ReadWrite(nand_chunk,
				                 0,
								 NAND_OP_MAIN | NAND_OP_SPARE,
								 fullpage,
								 BYTES_OF_PAGE + OOBBYTES_OF_PAGE,
								 NAND_WRITE_FLAG);

		if (rt != (BYTES_OF_PAGE + OOBBYTES_OF_PAGE))
		{
			return YAFFS_FAIL;
		}
	}
    else
    {
    	/*
    	 * write data to main
    	 */
    	rt = ls1x_nand_ReadWrite(nand_chunk,
    			                 0,
								 NAND_OP_MAIN,
								 data,
								 BYTES_OF_PAGE,
								 NAND_WRITE_FLAG);

    	if (rt != BYTES_OF_PAGE)
    	{
    		return YAFFS_FAIL;
    	}

    	/*
    	 * write ecc to spare
    	 */
    	nand_calculate_page_swecc(data, (unsigned char *)(tempbuf + ecc_offset), BYTES_OF_PAGE);

    	/*
    	 * write data to spare
    	 */
    	rt = ls1x_nand_ReadWrite(nand_chunk,
    			                 0,
								 NAND_OP_SPARE,
								 tempbuf,
								 OOBBYTES_OF_PAGE,
								 NAND_WRITE_FLAG);

    	if (rt != OOBBYTES_OF_PAGE)
    	{
    		return YAFFS_FAIL;
    	}
    }

	return YAFFS_OK;
}

static int ls1x_Mark_BadBlock(struct yaffs_dev *dev, int block_no)
{
	CHECK_NAND_DRV(YAFFS_FAIL);

	if (ls1x_nand_ioctl(devNAND, IOCTL_NAND_MARK_BAD_BLOCK, (void *)block_no) == 0)
		return YAFFS_OK;
	else
		return YAFFS_FAIL;
}

static int ls1x_Query_Block(struct yaffs_dev *dev,
                            int block_no,
				            enum yaffs_block_state *state,
				            unsigned int *seq_number)
{
	CHECK_NAND_DRV(YAFFS_FAIL);

	if (ls1x_nand_ioctl(devNAND, IOCTL_NAND_IS_BAD_BLOCK, (void *)block_no) != 0)
	{
		*state = YAFFS_BLOCK_STATE_DEAD;
		*seq_number = 0;
		
		return YAFFS_FAIL;
	}
	else
	{
		struct yaffs_ext_tags t;
		ls1x_Read_ChunkAndTags(dev, block_no * (PAGES_OF_BLOCK), NULL, &t);

		if (t.chunk_used)
		{
			*seq_number = t.seq_number;
			*state = YAFFS_BLOCK_STATE_NEEDS_SCAN;
		}
		else
		{
			*seq_number = 0;
			*state = YAFFS_BLOCK_STATE_EMPTY;
		}
		
		return YAFFS_OK;
	}
}

/**************************************************************************************************
 * initialize yaffs enviroment
 **************************************************************************************************/

static int LS1x_initialize_YAFFS(YAFFS_config_t *cfg)
{
	struct yaffs_dev *dev = &m_YAFFS_device;

	/*
	 * 已初始化
	 */
	if (cfg->initialized)
	{
		return 0;
	}

	p_YAFFS_device = dev;

	/*
	 * XXX 芯片型号, 主要是芯片参数 NOP 影响读写
	 */
	switch (cfg->nand_flash_chip)
	{
		case K9F1G08U0E:
			cfg->nand_rw_fullpage = 1;
			break;

		case K9F1G08U0C:
		case K9F1G08U0D:
		default:
			cfg->nand_rw_fullpage = 0;
			break;
	}
	
	/******************************************************
	 * 初始化 yaffs device
	 ******************************************************/

	/* extern to limit write failure.
	 */
	yaffs_wr_attempts = YAFFS_WR_ATTEMPTS;

	/* init yaffs_dev
	 */
	memset((void *)dev, 0, sizeof(struct yaffs_dev));

	dev->param.name = "/ndd";        // "yaffs2";
	dev->param.inband_tags = true;   // TRUE
	
	dev->param.total_bytes_per_chunk = cfg->bytes_of_page;
	dev->param.chunks_per_block      = cfg->pages_of_block;
	dev->param.spare_bytes_per_chunk = cfg->oobbytes_of_page;
	dev->param.start_block           = cfg->start_block;
	dev->param.end_block             = cfg->end_block;
	dev->param.n_reserved_blocks     = cfg->reserved_blocks;
	dev->param.n_caches              = cfg->cache_blocks;

	dev->param.use_nand_ecc = false;
	dev->param.tags_9bytes = false;
	dev->param.no_tags_ecc = true;
	dev->param.is_yaffs2 = true;
	dev->param.always_check_erased = true;		// XXX: NEEDED?

	/* NAND access functions for yaffs1 & yaffs2
	 */
	dev->param.write_chunk_fn = NULL;
	dev->param.read_chunk_fn = NULL;
	dev->param.erase_fn = ls1x_nand_EraseBlock;
	dev->param.initialise_flash_fn = ls1x_nand_Initialise;
	dev->param.deinitialise_flash_fn = ls1x_nand_Deinitialise;

	/* yaffs2 mode functions
	 */
	dev->param.write_chunk_tags_fn = ls1x_Write_ChunkAndTags;
	dev->param.read_chunk_tags_fn = ls1x_Read_ChunkAndTags;
	dev->param.bad_block_fn = ls1x_Mark_BadBlock;
	dev->param.query_block_fn = ls1x_Query_Block;

	dev->param.remove_obj_fn = NULL;
	dev->param.sb_dirty_fn = NULL;
	dev->param.gc_control = NULL;

	cfg->initialized = 1;

	return 0;
}

//***************************************************************************************
//
// 初始化并挂载文件系统   /ndd
//
// 注意: YAFFS_config_t 参数与芯片紧密相关
//
//***************************************************************************************

static int yaffs_is_mounted = 0;

extern int yaffsfs_init_mutex(void);

int yaffs_startup_and_mount(const char *mount_name)
{
    int rt;

    if (yaffs_is_mounted)
        return 0;

    if (yaffsfs_init_mutex() != 0)
        return -1;

    YAFFS_config_t *cfg = &m_YAFFS_config;

    memset((void *)cfg, 0, sizeof(YAFFS_config_t));
	cfg->bytes_of_page         = BYTES_OF_PAGE;
	cfg->pages_of_block        = PAGES_OF_BLOCK;
	cfg->oobbytes_of_page      = OOBBYTES_OF_PAGE;
	cfg->start_block           = 112;
	cfg->end_block             = 935;
	cfg->reserved_blocks       = 5;
	cfg->cache_blocks          = 8;
    cfg->nand_flash_chip       = K9F1G08U0C;

    LS1x_initialize_YAFFS(cfg);

    yaffs_add_device(p_YAFFS_device);

    rt = yaffs_mount(mount_name);

    yaffs_is_mounted = 1;

    printk("YAFFS: mounted NAND flash %s.\r\n", (rt == 0) ? "successful" : "fail");

    return rt;
}

int yaffs_is_running(void)
{
    return yaffs_is_mounted;
}

#endif // #if defined(BSP_USE_NAND) && defined(USE_YAFFS2)

/*
 * @@ END
 */
