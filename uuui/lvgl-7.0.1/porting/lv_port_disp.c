/**
 * @file lv_port_disp.c
 *
 */

#include "bsp.h"

#ifdef USE_LVGL

//-------------------------------------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------------------------------------

#include "ls1x_fb.h"

#include "lv_port_disp.h"

//-------------------------------------------------------------------------------------------------
// DEFINES
//-------------------------------------------------------------------------------------------------

#define EXAMPLE_1    0
#define EXAMPLE_2    1
#define EXAMPLE_3    0          // 可以速度最快

//-------------------------------------------------------------------------------------------------
// TYPEDEFS
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// STATIC PROTOTYPES
//-------------------------------------------------------------------------------------------------

static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv,
                       const lv_area_t *area,
                       lv_color_t *color_p);

#if LV_USE_GPU
static void gpu_blend(lv_disp_drv_t *disp_drv,
                      lv_color_t *dest,
                      const lv_color_t *src,
                      unsigned int length,
                      lv_opa_t opa);

static void gpu_fill(lv_disp_drv_t *disp_drv,
                     lv_color_t *dest_buf,
                     lv_coord_t dest_width,
                     const lv_area_t *fill_area,
                     lv_color_t color);
#endif

//-------------------------------------------------------------------------------------------------
// STATIC VARIABLES
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// MACROS
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-------------------------------------------------------------------------------------------------

void lv_port_disp_init(void)
{
    /*-----------------------------------------------------
     * Initialize your display
     *----------------------------------------------------*/
    disp_init();

    /*-----------------------------------------------------
     * Create a buffer for drawing
     *----------------------------------------------------*/

    /* LVGL requires a buffer where it draws the objects. The buffer's has to be greater than 1 display row
     *
     * There are three buffering configurations:
     * 1. Create ONE buffer with some rows: 
     *      LVGL will draw the display's content here and writes it to your display
     * 
     * 2. Create TWO buffer with some rows: 
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     * 
     * 3. Create TWO screen-sized buffer: 
     *      Similar to 2) but the buffer have to be screen sized. When LVGL is ready it will give the
     *      whole frame to display. This way you only need to change the frame buffer's address instead of
     *      copying the pixels.
     * */
     
    /* Example for 1) */
#if EXAMPLE_1
    static lv_disp_buf_t disp_buf_1;
  #if (BSP_USE_LWMEM)
    lv_color_t *buf1_1 = (lv_color_t *)malloc(LV_HOR_RES * 10 * sizeof(lv_color_t));
    lv_disp_buf_init(&disp_buf_1, buf1_1, NULL, LV_HOR_RES * 10);
  #else
    static lv_color_t buf1_1[LV_HOR_RES_MAX * 10];                        /* A buffer for 10 rows */
    lv_disp_buf_init(&disp_buf_1, buf1_1, NULL, LV_HOR_RES_MAX * 10);     /* Initialize the display buffer */
  #endif
#endif

    /* Example for 2) */
#if EXAMPLE_2
    static lv_disp_buf_t disp_buf_2;
  #if (BSP_USE_LWMEM)
    lv_color_t *buf2_1 = (lv_color_t *)malloc(LV_HOR_RES * 10 * sizeof(lv_color_t));
    lv_color_t *buf2_2 = (lv_color_t *)malloc(LV_HOR_RES * 10 * sizeof(lv_color_t));
    lv_disp_buf_init(&disp_buf_2, buf2_1, buf2_2, LV_HOR_RES * 10);
  #else
    static lv_color_t buf2_1[LV_HOR_RES_MAX * 10];                        /* A buffer for 10 rows */
    static lv_color_t buf2_2[LV_HOR_RES_MAX * 10];                        /* An other buffer for 10 rows */
    lv_disp_buf_init(&disp_buf_2, buf2_1, buf2_2, LV_HOR_RES_MAX * 10);   /* Initialize the display buffer */
  #endif
#endif

    /* Example for 3) */
#if EXAMPLE_3
    static lv_disp_buf_t disp_buf_3;
  #if (BSP_USE_LWMEM)
    lv_color_t *buf3_1 = (lv_color_t *)malloc(LV_HOR_RES * LV_VER_RES * sizeof(lv_color_t));
    lv_color_t *buf3_2 = (lv_color_t *)malloc(LV_HOR_RES * LV_VER_RES * sizeof(lv_color_t));
    lv_disp_buf_init(&disp_buf_3, buf3_1, buf3_2, LV_HOR_RES * LV_VER_RES);         /* Initialize the display buffer */
  #else
    static lv_color_t buf3_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];                      /* A screen sized buffer */
    static lv_color_t buf3_2[LV_HOR_RES_MAX * LV_VER_RES_MAX];                      /* An other screen sized buffer */
    lv_disp_buf_init(&disp_buf_3, buf3_1, buf3_2, LV_HOR_RES_MAX * LV_VER_RES_MAX); /* Initialize the display buffer */
  #endif
#endif

    /*-----------------------------------------------------
     * Register the display in LVGL
     *----------------------------------------------------*/

    lv_disp_drv_t disp_drv;                             /* Descriptor of a display driver */
    lv_disp_drv_init(&disp_drv);                        /* Basic initialization */

    /* Set up the functions to access to your display */

    /* Set the resolution of the display */
    disp_drv.hor_res = LV_HOR_RES_MAX;      // 800;
    disp_drv.ver_res = LV_VER_RES_MAX;      // 480;

    /* Used to copy the buffer's content to the display */
    disp_drv.flush_cb = disp_flush;

    /* Set a display buffer */
#if EXAMPLE_1
    disp_drv.buffer = &disp_buf_1;
#elif EXAMPLE_2
    disp_drv.buffer = &disp_buf_2;
#elif EXAMPLE_3
    disp_drv.buffer = &disp_buf_3;
#else
#error "please define the disp buffer."
#endif

#if LV_USE_GPU
    /*
     * Optionally add functions to access the GPU. (Only in buffered mode, LV_VDB_SIZE != 0)
     */

    /* Blend two color array using opacity */
    disp_drv.gpu_blend_cb = gpu_blend;

    /* Fill a memory array with a color */
    disp_drv.gpu_fill_cb = gpu_fill;
#endif

    /* Finally register the driver */
    lv_disp_drv_register(&disp_drv);
}

//-------------------------------------------------------------------------------------------------
// STATIC FUNCTIONS
//-------------------------------------------------------------------------------------------------

/*
 * Initialize your display and the required peripherals.
 */
static void disp_init(void)
{
    fb_open();
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_disp_flush_ready()' has to be called when finished.
 */
static void disp_flush(lv_disp_drv_t *disp_drv,
                       const lv_area_t *area,
                       lv_color_t *color_p)
{
    /*
     * The most simple case (but also the slowest) to put all pixels to the screen one-by-one
     */
    int x, y;
    
    for (y = area->y1; y <= area->y2; y++)
    {
        for (x = area->x1; x <= area->x2; x++)
        {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color_p) */

            ls1x_draw_rgb565_pixel(x, y, color_p->full);
            
            color_p++;
        }
    }

    flush_dcache();

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing
     */
    lv_disp_flush_ready(disp_drv);
}

/*
 * OPTIONAL: GPU INTERFACE
 */
#if LV_USE_GPU

/* If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)
 */
static void gpu_blend(lv_disp_drv_t *disp_drv,
                      lv_color_t *dest,
                      const lv_color_t *src,
                      unsigned int length,
                      lv_opa_t opa)
{
    /*
     * It's an example code which should be done by your GPU
     */
    unsigned int i;
    for (i = 0; i < length; i++)
    {
        dest[i] = lv_color_mix(dest[i], src[i], opa);
    }
}

/* If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)
 */
static void gpu_fill(lv_disp_drv_t *disp_drv,
                     lv_color_t *dest_buf,
                     lv_coord_t dest_width,
                     const lv_area_t *fill_area,
                     lv_color_t color)
{
    /*
     * It's an example code which should be done by your GPU
     */
    int x, y;
    dest_buf += dest_width * fill_area->y1;     /* Go to the first line */

    for (y = fill_area->y1; y <= fill_area->y2; y++)
    {
        for (x = fill_area->x1; x <= fill_area->x2; x++)
        {
            dest_buf[x] = color;
        }

        dest_buf+=dest_width;                   /* Go to the next line */
    }
}

#endif  /*LV_USE_GPU*/

#else /* Enable this file at the top */

/*
 * This dummy typedef exists purely to silence -Wpedantic.
 */
typedef int keep_pedantic_happy;

#endif // #ifdef USE_LVGL



