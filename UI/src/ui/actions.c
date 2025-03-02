#include "ui.h"
#include "actions.h"
#include "screens.h"
#include <stm32g4xx.h>
#include <lvgl.h>
#include <app_types.h>
#include <PCC_public_interface.h>

//extern lv_indev_t* encoder_input_device_ps;
//extern lv_indev_t* btn_input_device_ps;

extern lv_indev_t *            input_encoder_ps;
extern lv_indev_t *            input_push_btn_ps;

void action_start_topology(lv_event_t *e);
void action_stop_topology(lv_event_t *e);

void remove_all_indev_events(lv_indev_t * indev_p)
{
    for(u32 event_count_u32 = lv_indev_get_event_count(input_push_btn_ps); event_count_u32 > 0; event_count_u32--)
    {
        lv_indev_remove_event(indev_p, event_count_u32 - 1);
    }
}

void action_back_to_main_menu(lv_event_t *e) {
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_set_group(input_encoder_ps, groups.MainGroup);
    loadScreen(SCREEN_ID_MAIN);
}

void action_stop_topology(lv_event_t *e)
{
    PCC_StopActiveTopology_v();
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_add_event_cb(input_push_btn_ps, action_back_to_main_menu, LV_EVENT_PRESSED, NULL);
    lv_indev_add_event_cb(input_push_btn_ps, action_start_topology, LV_EVENT_LONG_PRESSED, NULL);
}

void action_start_topology(lv_event_t *e) {
    remove_all_indev_events(input_push_btn_ps);
    PCC_StartActiveTopology_v();

    lv_indev_add_event_cb(input_push_btn_ps, action_stop_topology, LV_EVENT_PRESSED, NULL);
}

void action_go_to_power_topology_menu(lv_event_t * e)
{
    lv_indev_add_event_cb(input_push_btn_ps, action_back_to_main_menu, LV_EVENT_PRESSED, NULL);
    lv_indev_add_event_cb(input_push_btn_ps, action_start_topology, LV_EVENT_LONG_PRESSED, NULL);

    lv_indev_set_group(input_encoder_ps, groups.param_selector);

    PCC_SetTopology_b(PCC_TOPO_SinglePWM_e);
    PCC_InitializeActiveTopology_b();


    loadScreen(SCREEN_ID_SIMPLE_PWM);
}
