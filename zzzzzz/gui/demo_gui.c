/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * demo_gui.c
 *
 *  Created on: 2015-1-26
 *      Author: Bian
 */

/*
 * 假定: XPT2046_DRV 使用800*480 触摸屏
 *       GT1151_DRV  使用480*800 触摸屏
 *
 * 其它分辨率, 需要修改 grid/button 的位置参数
 */
 
#include "bsp.h"

#ifdef BSP_USE_FB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ls1x_fb.h"
#include "simple-gui/simple_gui.h"

#define BUTTON_WIDTH		120
#define BUTTON_HEIGHT		42

/******************************************************************************
 * show message
 ******************************************************************************/

static volatile int last_x   = -1;
static volatile int last_y   = -1;

void gui_info(const char *str, int x, int y)
{
	if ((str == NULL) || (x < 0) || (y < 0))
		return;

	/* clear last */
	if ((last_x > 0) && (last_y > 0))
	{
		FillRect(last_x, last_y, fb_get_pixelsx() - 1, last_y + 16, cidxBLACK);
	}

	PutString(x, y, (char *)str, cidxBRTWHITE);

	last_x = x;
	last_y = y;
}

/******************************************************************************
 * defined functions
 ******************************************************************************/

int clear_screen(void)
{
	/* fill screen color silver
	 */
	ls1x_dc_ioctl(devDC, IOCTRL_FB_CLEAR_BUFFER, (void *)GetColor(cidxSILVER));

	return 0;
}
 
static int create_main_objects(void);

static void set_objects_active_group(int group);

/******************************************************************************
 * main frame
 ******************************************************************************/

#define MAIN_GROUP	0x00010000

static TGrid *grid_main = NULL;

static int create_grid_main(void)
{
	TColumn *p_column;
	TRect    rect;

	rect.left   = 20;
	rect.top    = 20;
	
#ifdef XPT2046_DRV
	rect.right  = 638;
	rect.bottom = 459;
	grid_main = create_grid(&rect, 12, 4, MAIN_GROUP | 0x0001, MAIN_GROUP);
#elif defined(GT1151_DRV)
	rect.right  = 460;
	rect.bottom = 492;
	grid_main = create_grid(&rect, 12, 3, MAIN_GROUP | 0x0001, MAIN_GROUP);
#endif

	if (grid_main == NULL)
		return -1;

	/* column 0 */
	p_column = grid_get_column(grid_main, 0);
	p_column->align = align_center;
	grid_set_column_title(p_column, "设备");
	grid_set_column_width(grid_main, 0, 56);

	/* column 1 */
	p_column = grid_get_column(grid_main, 1);
	p_column->align = align_center;
	grid_set_column_title(p_column, "协议");
	grid_set_column_width(grid_main, 1, 64);

#ifdef XPT2046_DRV
	/* column 2 */
	p_column = grid_get_column(grid_main, 2);
	p_column->align = align_center;
	grid_set_column_title(p_column, "下位机");
	grid_set_column_width(grid_main, 2, 92);

	/* column 3 */
	p_column = grid_get_column(grid_main, 3);
	grid_set_column_title(p_column, "当前活动数据");
	grid_set_column_width(grid_main, 3, 430);
	
#elif defined(GT1151_DRV)
	/* column 1 */
	p_column = grid_get_column(grid_main, 2);
	grid_set_column_title(p_column, "当前活动数据");
	grid_set_column_width(grid_main, 2, 316);
	
#endif

	return 0;
}

/*
 * right: 160*480, button to navigate
 */
static void main_onclick_0001(unsigned msg, void *param)
{
    TButton *btn = (TButton *)param;
    printk("print me \"%s\"\r\n", btn->caption);  // 实时曲线
}

static void main_onclick_0002(unsigned msg, void *param)
{
    TButton *btn = (TButton *)param;
    printk("print me \"%s\"\r\n", btn->caption); // 实时数据
}

static void main_onclick_0003(unsigned msg, void *param)
{
    TButton *btn = (TButton *)param;
    printk("print me \"%s\"\r\n", btn->caption); // 历史数据

    if (btn->tag == 0)
    {
//        MB_Master_suspend();
        strcpy(btn->caption, "Resume");
        btn->tag = 1;
    }
    else
    {
//        MB_Master_resume();
        strcpy(btn->caption, "Suspend");
        btn->tag = 0;  
    }
    
    btn->layout_changed = true;   // 立即重画
}

static void main_onclick_0004(unsigned msg, void *param)
{
    TButton *btn = (TButton *)param;
    printk("print me \"%s\"\r\n", btn->caption); // 重启系统
//    bsp_reset();
}

static int create_buttons_main(void)
{
	TRect rect;

#ifdef XPT2046_DRV
	rect.left   = 660;
	rect.top    = 15;
	rect.right  = rect.left + BUTTON_WIDTH;
	rect.bottom = rect.top + BUTTON_HEIGHT;
	new_button(&rect, MAIN_GROUP | 0x0001, MAIN_GROUP, "实时曲线", main_onclick_0001); 

	rect.left   = 660;
	rect.top    = 75;
	rect.right  = rect.left + BUTTON_WIDTH;
	rect.bottom = rect.top + BUTTON_HEIGHT;
	new_button(&rect, MAIN_GROUP | 0x0002, MAIN_GROUP, "实时数据", main_onclick_0002);  

	rect.left   = 660;
	rect.top    = 135;
	rect.right  = rect.left + BUTTON_WIDTH;
	rect.bottom = rect.top + BUTTON_HEIGHT;
	new_button(&rect, MAIN_GROUP | 0x0003, MAIN_GROUP, "Suspend", main_onclick_0003);  // 历史数据

	rect.left   = 660;
	rect.top    = 425;
	rect.right  = rect.left + BUTTON_WIDTH;
	rect.bottom = rect.top + BUTTON_HEIGHT;
	new_button(&rect, MAIN_GROUP | 0x0004, MAIN_GROUP, "重启系统", main_onclick_0004);

#elif defined(GT1151_DRV)
	rect.left   = 340;
	rect.top    = 520;
	rect.right  = rect.left + BUTTON_WIDTH;
	rect.bottom = rect.top + BUTTON_HEIGHT;
	new_button(&rect, MAIN_GROUP | 0x0001, MAIN_GROUP, "实时曲线", main_onclick_0001);

	rect.left   = 340;
	rect.top    = 580;
	rect.right  = rect.left + BUTTON_WIDTH;
	rect.bottom = rect.top + BUTTON_HEIGHT;
	new_button(&rect, MAIN_GROUP | 0x0002, MAIN_GROUP, "实时数据", main_onclick_0002);

	rect.left   = 340;
	rect.top    = 640;
	rect.right  = rect.left + BUTTON_WIDTH;
	rect.bottom = rect.top + BUTTON_HEIGHT;
	new_button(&rect, MAIN_GROUP | 0x0003, MAIN_GROUP, "Suspend", main_onclick_0003);  // 历史数据

	rect.left   = 340;
	rect.top    = 700;
	rect.right  = rect.left + BUTTON_WIDTH;
	rect.bottom = rect.top + BUTTON_HEIGHT;
	new_button(&rect, MAIN_GROUP | 0x0004, MAIN_GROUP, "重启系统", main_onclick_0004);

#endif

	return 0;
}

static int create_main_objects(void)
{
	if (clear_screen() != 0)
		return -1;

	if (get_buttons_count(MAIN_GROUP) == 0)
		create_buttons_main();

	if (grid_main == NULL)
		create_grid_main();

	set_objects_active_group(MAIN_GROUP);
	
	return 0;
}

/******************************************************************************
 * object set active
 ******************************************************************************/

static void set_objects_active_group(int group)
{
	set_gui_active_group(group);
}

/******************************************************************************
 * app with simple-gui & xyplot
 ******************************************************************************/

void start_my_gui(void)
{
	if (fb_open() != 0)
		return;

    if (!ls1x_dc_started())
        return;

    init_simple_gui_env();          /* gui enviroment */
    create_main_objects();	        /* main objects */

#if BSP_USE_OS
	start_gui_monitor_task();	    /* start button click event task */
#else
    paint_my_simple_gui();          /* just draw It */
#endif

#if defined(USE_YAFFS2) && defined(GT1151_DRV)
    fb_showbmp(20, 580, "/ndd/photo/logo_160.bmp");
#endif
}

void gui_drawtext_in_grid(int row, int col, const char *str)
{
    if (!ls1x_dc_started())
        return;
        
    if (grid_main != NULL)
        grid_set_cell_text(grid_main, row, col, str);
}

#endif

