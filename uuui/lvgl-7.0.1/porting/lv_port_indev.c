/**
 * @file lv_port_indev.c
 *
 */

#include "bsp.h"

#ifdef USE_LVGL

//-------------------------------------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------------------------------------
 
#include "lv_port_indev.h"

#ifdef GT1151_DRV
#include "ls1x_i2c_bus.h"
#include "touch.h"
#elif defined(XPT2046_DRV)
#include "ls1x_spi_bus.h"
#include "spi/xpt2046.h"
/*
 * RTOS running
 */
#if BSP_USE_OS

/* 用 touch_msg_queue 响应?
 * 现在用的 callback
 */
#endif
#endif

//-------------------------------------------------------------------------------------------------
// DEFINES
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// TYPEDEFS
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// STATIC PROTOTYPES
//-------------------------------------------------------------------------------------------------

static void touchpad_init(void);
static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t *x, lv_coord_t *y);

static void mouse_init(void);
static bool mouse_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static bool mouse_is_pressed(void);
static void mouse_get_xy(lv_coord_t *x, lv_coord_t *y);

static void keypad_init(void);
static bool keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static unsigned int keypad_get_key(void);

static void encoder_init(void);
static bool encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static void encoder_handler(void);

static void button_init(void);
static bool button_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static char button_get_pressed_id(void);
static bool button_is_pressed(unsigned char id);

//-------------------------------------------------------------------------------------------------
// STATIC VARIABLES
//-------------------------------------------------------------------------------------------------

lv_indev_t *indev_touchpad;
lv_indev_t *indev_mouse;
lv_indev_t *indev_keypad;
lv_indev_t *indev_encoder;
lv_indev_t *indev_button;

static int32_t encoder_diff;
static lv_indev_state_t encoder_state;

//-------------------------------------------------------------------------------------------------
// MACROS
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-------------------------------------------------------------------------------------------------

void lv_port_indev_init(void)
{
    /* Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    lv_indev_drv_t indev_drv;

    /*-----------------------------------------------------
     * Touchpad
     *----------------------------------------------------*/

    /* Initialize your touchpad if you have */
    touchpad_init();

    /* Register a touchpad input device */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);

    /*-----------------------------------------------------
     * Mouse
     *----------------------------------------------------*/

    /* Initialize your touchpad if you have */
    mouse_init();

    /* Register a mouse input device */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;
    indev_mouse = lv_indev_drv_register(&indev_drv);

    /* Set cursor. For simplicity set a HOME symbol now. */
    lv_obj_t * mouse_cursor = lv_img_create(lv_disp_get_scr_act(NULL), NULL);
    lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
    lv_indev_set_cursor(indev_mouse, mouse_cursor);

    /*-----------------------------------------------------
     * Keypad
     *----------------------------------------------------*/

    /* Initialize your keypad or keyboard if you have */
    keypad_init();

    /* Register a keypad input device */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);

    /* Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     * add objects to the group with `lv_group_add_obj(group, obj)`
     * and assign this input device to group to navigate in it:
     * `lv_indev_set_group(indev_keypad, group);` */

    /*-----------------------------------------------------
     * Encoder
     *----------------------------------------------------*/

    /* Initialize your encoder if you have */
    encoder_init();

    /* Register a encoder input device */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_encoder = lv_indev_drv_register(&indev_drv);

    /* Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     * add objects to the group with `lv_group_add_obj(group, obj)`
     * and assign this input device to group to navigate in it:
     * `lv_indev_set_group(indev_encoder, group);` */

    /*-----------------------------------------------------
     * Button
     *----------------------------------------------------*/

    /* Initialize your button if you have */
    button_init();

    /* Register a button input device */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_BUTTON;
    indev_drv.read_cb = button_read;
    indev_button = lv_indev_drv_register(&indev_drv);

    /* Assign buttons to points on the screen */
    static const lv_point_t btn_points[2] =
    {
        {10, 10},   /* Button 0 -> x:10; y:10  */
        {40, 100},  /* Button 1 -> x:40; y:100 */
    };

    lv_indev_set_button_points(indev_button, btn_points);
}

//-------------------------------------------------------------------------------------------------
// STATIC FUNCTIONS
//-------------------------------------------------------------------------------------------------

/**********************************************************
 * Touchpad
 **********************************************************/
 
#ifdef GT1151_DRV

static int pixelsx = 0;
static int pixelsy = 0;

#elif defined(XPT2046_DRV)

static int touch_x = 0;
static int touch_y = 0;

// If RTOS running
#if BSP_USE_OS

static int is_xpt2046_clicked = 0;  // 标记有触摸事件

static void xpt2046_click_callback(int x, int y)
{
    touch_x = x;
    touch_y = y;
    is_xpt2046_clicked = 1;     
}
#endif // #if BSP_USE_OS

#endif // GT1151_DRV or XPT2046_DRV

/*
 * Initialize your touchpad
 */
static void touchpad_init(void)
{
    /* Your code comes here */
#ifdef GT1151_DRV

    extern int fb_get_pixelsx(void);
    extern int fb_get_pixelsy(void);

    pixelsx = fb_get_pixelsx();
	pixelsy = fb_get_pixelsy();
	
    tp_dev.init();
    
#elif defined(XPT2046_DRV)

    extern int load_touch_calibrate_values(void);

    ls1x_xpt2046_init(busSPI0, NULL);
    load_touch_calibrate_values();      /* 重新载入校正数据 */
    
  #if BSP_USE_OS
    start_touchscreen_task(xpt2046_click_callback); /* xpt2046 触摸任务 */
  #endif
  
#endif
}

/*
 * Will be called by the library to read the touchpad
 */
static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /* Save the pressed coordinates and the state */
    if (touchpad_is_pressed())
    {
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /* Set the last pressed coordinates */
    data->point.x = last_x;
    data->point.y = last_y;

    /* Return `false` because we are not buffering and no more data to read */
    return false;
}

/*
 * Return true is the touchpad is pressed
 */
static bool touchpad_is_pressed(void)
{
    /* Your code comes here */
#ifdef GT1151_DRV

    if (tp_dev.scan(0))
        return true;
        
#elif defined(XPT2046_DRV)

  #if BSP_USE_OS
    if (is_xpt2046_clicked)
        return true;
  #else
    if (bare_get_touch_point(&touch_x, &touch_y) == 0)
        return true;
  #endif
  
#endif

    return false;
}

/*
 * Get the x and y coordinates if the touchpad is pressed
 */
static void touchpad_get_xy(lv_coord_t *x, lv_coord_t *y)
{
    /* Your code comes here */
#ifdef GT1151_DRV
    int i;
    for (i=0; i<5; i++)
    {
	    if (((tp_dev.sta) & (1 << i)) &&
             (tp_dev.x[i] < pixelsx) && (tp_dev.y[i] < pixelsy))
		{
		    printk("pressed at x=%i, y=%i\r\n", tp_dev.x[i], tp_dev.y[i]);
		    *x = (lv_coord_t) tp_dev.x[i];
            *y = (lv_coord_t) tp_dev.y[i];
            break;
		}
    }

#elif defined(XPT2046_DRV)

    *x = (lv_coord_t) touch_x;
    *y = (lv_coord_t) touch_y;

  #if BSP_USE_OS
    is_xpt2046_clicked = 0;
  #endif

#else
    (*x) = 0;
    (*y) = 0;
    
#endif
}

/**********************************************************
 * Mouse
 **********************************************************/

/*
 * Initialize your mouse
 */
static void mouse_init(void)
{
    /*Your code comes here*/
}

/*
 * Will be called by the library to read the mouse
 */
static bool mouse_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    /* Get the current x and y coordinates */
    mouse_get_xy(&data->point.x, &data->point.y);

    /* Get whether the mouse button is pressed or released */
    if (mouse_is_pressed())
    {
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /* Return `false` because we are not buffering and no more data to read
     */
    return false;
}

/*
 * Return true is the mouse button is pressed
 */
static bool mouse_is_pressed(void)
{
    /* Your code comes here */

    return false;
}

/*
 * Get the x and y coordinates if the mouse is pressed
 */
static void mouse_get_xy(lv_coord_t *x, lv_coord_t *y)
{
    /* Your code comes here */

    (*x) = 0;
    (*y) = 0;
}

/**********************************************************
 * Keypad
 **********************************************************/

/*
 * Initialize your keypad
 */
static void keypad_init(void)
{
    /* Your code comes here */
}

/*
 * Will be called by the library to read the mouse
 */
static bool keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static unsigned int last_key = 0;

    /* Get the current x and y coordinates */
    mouse_get_xy(&data->point.x, &data->point.y);

    /* Get whether the a key is pressed and save the pressed key */
    unsigned int act_key = keypad_get_key();

    if (act_key != 0)
    {
        data->state = LV_INDEV_STATE_PR;

        /* Translate the keys to LVGL control characters according to your key definitions
         */
        switch (act_key)
        {
            case 1:
                act_key = LV_KEY_NEXT;
                break;
            case 2:
                act_key = LV_KEY_PREV;
                break;
            case 3:
                act_key = LV_KEY_LEFT;
                break;
            case 4:
                act_key = LV_KEY_RIGHT;
                break;
            case 5:
                act_key = LV_KEY_ENTER;
                break;
        }

        last_key = act_key;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;

    /* Return `false` because we are not buffering and no more data to read
     */
    return false;
}

/*
 * Get the currently being pressed key.  0 if no key is pressed
 */
static unsigned int keypad_get_key(void)
{
    /* Your code comes here */

    return 0;
}

/**********************************************************
 * Encoder
 **********************************************************/

/*
 * Initialize your keypad
 */
static void encoder_init(void)
{
    /* Your code comes here */
}

/*
 * Will be called by the library to read the encoder
 */
static bool encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    data->enc_diff = encoder_diff;
    data->state = encoder_state;

    /* Return `false` because we are not buffering and no more data to read
     */
    return false;
}

/*
 * Call this function in an interrupt to process encoder events (turn, press)
 */
static void encoder_handler(void)
{
    /* Your code comes here */

    encoder_diff += 0;
    encoder_state = LV_INDEV_STATE_REL;
}

/**********************************************************
 * Button
 **********************************************************/

/*
 * Initialize your buttons
 */
static void button_init(void)
{
    /* Your code comes here */
}

/*
 * Will be called by the library to read the button
 */
static bool button_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static unsigned char last_btn = 0;

    /*
     * Get the pressed button's ID
     */
    char btn_act = button_get_pressed_id();

    if (btn_act >= 0)
    {
        data->state = LV_INDEV_STATE_PR;
        last_btn = btn_act;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /* Save the last pressed button's ID */
    data->btn_id = last_btn;

    /* Return `false` because we are not buffering and no more data to read
     */
    return false;
}

/*
 * Get ID  (0, 1, 2 ..) of the pressed button
 */
static char button_get_pressed_id(void)
{
    unsigned char i;

    /* Check to buttons see which is being pressed (assume there are 2 buttons)
     */
    for (i = 0; i < 2; i++)
    {
        /* Return the pressed button's ID
         */
        if (button_is_pressed(i))
        {
            return i;
        }
    }

    /* No button pressed */
    return -1;
}

/*
 * Test if `id` button is pressed or not
 */
static bool button_is_pressed(unsigned char id)
{
    /* Your code comes here */

    return false;
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;

#endif // #ifdef USE_LVGL

