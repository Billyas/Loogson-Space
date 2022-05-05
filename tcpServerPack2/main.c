/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Loongson 1B RT-Thread Application
 */

#include <time.h>

#include "rtthread.h"

//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"

#if defined(BSP_USE_FB)
  #include "ls1x_fb.h"
  #ifdef XPT2046_DRV
    char LCD_display_mode[] = LCD_800x480;
  #elif defined(GT1151_DRV)
    char LCD_display_mode[] = LCD_480x800;
  #else
    #error "在bsp.h中选择配置 XPT2046_DRV 或者 GT1151_DRV"
           "XPT2046_DRV:  用于800*480 横屏的触摸屏."
           "GT1151_DRV:   用于480*800 竖屏的触摸屏."
           "如果都不选择, 注释掉本 error 信息, 然后自定义: LCD_display_mode[]"
  #endif
#endif

//-------------------------------------------------------------------------------------------------
// Simple demo of task
//-------------------------------------------------------------------------------------------------

static rt_thread_t m_demo_thread = NULL;

static void demo_thread(void *arg)
{
	unsigned int tickcount;

    for ( ; ; )
    {
        tickcount = rt_tick_get();

        rt_kprintf("tick count = %i\r\n", tickcount);

        rt_thread_delay(500);
    }
}

 static const char http_html_hdr[] =
     "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
 static const char http_index_html[] =
 "<html><head><title>Congrats!</title></head>\
 <body><h1 align=\"center\">Hello World!</h1>\
 <h2 align=\"center\">Welcome to  lwIP HTTP Server!</h1>\
 <p align=\"center\">This is a small test page, served by httpserver-netconn on lx1b200.</p>\
 <p align=\"center\"><a href=\"https://www.cnblogs.com/billyme/\">\
 <font size=\"6\"> Billyme </font> </a></p>\
 <a href=\"https://www.cnblogs.com/billyme/\">\
 <p align=\"center\"><img src=\"https://cdn.jsdelivr.net/gh/Billyas/myimg/img/color-success.png\" /></a>\
 </body></html>";
 
static void tcpservertest()
{
    rt_int32_t timeout = 100000;
    struct tcpserver* server = tcpserver_create("192.168.5.64", 80);
    struct tcpclient * client = tcpserver_accept(server, timeout);
    if(client){
        rt_kprintf("get client");
      tcpserver_send(client, http_html_hdr, sizeof(http_html_hdr), timeout);
      tcpserver_send(client, http_index_html, sizeof(http_index_html), timeout);
      tcpserver_close(client);
    }
    rt_kprintf("no client");
    tcpserver_destroy(server);
}

//-------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	rt_kprintf("\r\nWelcome to RT-Thread.\r\n\r\n");

    ls1x_drv_init();            /* Initialize device drivers */

    rt_ls1x_drv_init();         /* Initialize device drivers for RTT */

    install_3th_libraries(); 	/* Install 3th libraies */

    unsigned char  a[4]={192,168,5,64};

    ls1x_initialize_lwip(NULL,NULL);
    
    tcpservertest();
    
    return 0;
}

/*
 * @@ End
 */
