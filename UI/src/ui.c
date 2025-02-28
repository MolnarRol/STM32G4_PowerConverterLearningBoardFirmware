/*
 * ui.c
 *
 *  Created on: Feb 28, 2025
 *      Author: molnar
 */

#include <stm32g4xx.h>
#include <app_types.h>
#include <lvgl.h>
extern lv_indev_t* encoder_input_device_ps;

lv_obj_t* ScreenMain_ps;
lv_obj_t* ScreenControlTopology_ps;
lv_obj_t* ScreenSettings_ps;

//void UI_Init_v(void)
//{
//    lv_obj_t* label = lv_label_create(lv_scr_act());
//    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
//    lv_label_set_text(label, "Ahoj Dianka moja krasna");
//    lv_obj_set_width(label, 320);
//    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, 0);
//    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);
//
//    //  // Change the active screen's background color
//      lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);
//      lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
//
//      /*Create a spinner*/
//      lv_obj_t * spinner = lv_spinner_create(lv_scr_act(), 1000, 60);
//      lv_obj_set_size(spinner, 64, 64);
//      lv_obj_align(spinner, LV_ALIGN_BOTTOM_MID, 0, 0);
//}

static void set_topology_btn_event_handler_v(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        __NOP();
        lv_scr_load(ScreenControlTopology_ps);
    }
    else if(code == LV_EVENT_VALUE_CHANGED)
    {
        __NOP();
    }
}

static void back_to_main_menu_v(lv_event_t * e)
{
    lv_scr_load(ScreenMain_ps);
}

void UI_Init_v(void)
{
    ScreenMain_ps = lv_scr_act();

    ScreenControlTopology_ps = lv_label_create(NULL);
    lv_label_set_text(ScreenControlTopology_ps, "Topo");
    lv_obj_center(ScreenControlTopology_ps);
    lv_obj_add_event_cb(ScreenControlTopology_ps, back_to_main_menu_v, LV_EVENT_PRESSED, NULL);

    ScreenSettings_ps = lv_label_create(NULL);
    lv_label_set_text(ScreenSettings_ps, "Settings");
    lv_obj_center(ScreenSettings_ps);

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);

    lv_obj_t* btn_labels_ps;

    lv_obj_t* set_topology_btn_ps = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(set_topology_btn_ps, set_topology_btn_event_handler_v, LV_EVENT_ALL, NULL);
    lv_obj_align(set_topology_btn_ps, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_width(set_topology_btn_ps, 260);
    btn_labels_ps = lv_label_create(set_topology_btn_ps);
    lv_label_set_text(btn_labels_ps, "Control topologies");
    lv_obj_center(btn_labels_ps);

    lv_obj_t* settings_btn_ps = lv_btn_create(lv_scr_act());
    lv_obj_align(settings_btn_ps, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_width(settings_btn_ps, 260);
    btn_labels_ps = lv_label_create(settings_btn_ps);
    lv_label_set_text(btn_labels_ps, "Settings");
    lv_obj_center(btn_labels_ps);

    lv_group_t * btn_group_ps = lv_group_create();
    lv_group_add_obj(btn_group_ps, set_topology_btn_ps);
    lv_group_add_obj(btn_group_ps, settings_btn_ps);
    lv_indev_set_group(encoder_input_device_ps, btn_group_ps);
}
