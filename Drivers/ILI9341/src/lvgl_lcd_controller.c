/*
 * lvgl_lcd_controller.c
 *
 *  Created on: Mar 2, 2025
 *      Author: molnar
 */

/*********************
 *      INCLUDES
 *********************/
#include "ili9341_lvgl_lcd_controller.h"
#include <stdbool.h>
#include <main.h>
#include <PUI_private_interface.h>
#include "ili9341.h"
#include "ui.h"
#include "screens.h"
#include "actions.h"
/*********************
 *      DEFINES
 *********************/
#define MY_DISP_HOR_RES         320
#define MY_DISP_VER_RES         240

#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    320
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_VER_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    240
#endif

#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) /*will be 2 for RGB565 */

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);

static void pui_lvgl_btn_long_press_cb_v(void);
static void pui_lvgl_btn_short_press_cb_v(void);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_display_t*    lcd_disp_ps;
lv_indev_t *            input_encoder_ps;
lv_indev_t *            input_push_btn_ps;
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lcd_disp_ps = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_display_set_flush_cb(lcd_disp_ps, disp_flush);

    /* Example 1
     * One buffer for partial rendering*/
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_1_1[MY_DISP_HOR_RES * 20 * BYTE_PER_PIXEL];            /*A buffer for 10 rows*/
    lv_display_set_buffers(lcd_disp_ps, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Example 2
     * Two buffers for partial rendering
     * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_2_1[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];
//
//    LV_ATTRIBUTE_MEM_ALIGN
//    static uint8_t buf_2_2[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];
//    lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /*
     * Input devices configuration.
     */
    input_encoder_ps = lv_indev_create();
    lv_indev_set_type(input_encoder_ps, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(input_encoder_ps, PUI_RotaryEncoderReadCallback_v);

    input_push_btn_ps = lv_indev_create();
    lv_indev_set_type(input_push_btn_ps, LV_INDEV_TYPE_BUTTON);
    lv_indev_set_mode(input_push_btn_ps, LV_INDEV_MODE_EVENT);

    /*
     * UI initialization.
     */
    ui_create_groups();
    lv_indev_set_group(input_encoder_ps, groups.MainGroup);
    ui_init();

    PUI_StartStopBtn_s.btn_short_press_callback_pvf = pui_lvgl_btn_short_press_cb_v;
    PUI_StartStopBtn_s.btn_long_press_callback_pvf = pui_lvgl_btn_long_press_cb_v;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void pui_lvgl_btn_long_press_cb_v(void)
{
    lv_indev_send_event(input_push_btn_ps, LV_EVENT_LONG_PRESSED, NULL);
}

static void pui_lvgl_btn_short_press_cb_v(void)
{
    lv_indev_send_event(input_push_btn_ps, LV_EVENT_PRESSED, NULL);
}

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    ILI9341_Init();
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_display_flush_ready()' has to be called when it's finished.*/
static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * px_map)
{
    //if(disp_flush_enabled) {
        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/
        ILI9341_SetWindow(area->x1, area->y1, area->x2, area->y2);

        int height = area->y2 - area->y1 + 1;
        int width = area->x2 - area->x1 + 1;

        ILI9341_DrawBitmap(width, height, px_map);
    //}

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_display_flush_ready(disp_drv);
}
