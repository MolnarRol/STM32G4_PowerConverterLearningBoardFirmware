#include "ui.h"
#include "actions.h"
#include "screens.h"
#include <stm32g4xx.h>
#include <lvgl.h>
#include <PCC_public_interface.h>

extern lv_indev_t* encoder_input_device_ps;
extern lv_indev_t* btn_input_device_ps;

void action_back_to_main_menu(lv_event_t *e) {

}

void action_go_to_power_topology_menu(lv_event_t * e)
{
    lv_indev_set_group(encoder_input_device_ps, groups.param_selector);
    loadScreen(SCREEN_ID_SIMPLE_PWM);
//    PCC_
}

void action_change_active_state_of_frequency_parameters(lv_event_t *e) {
    lv_event_code_t event_code_e = lv_event_get_code(e);

    if(event_code_e == LV_EVENT_VALUE_CHANGED)
    {
        bool btn_state_b = lv_obj_has_state(objects.freq_editable_btn, LV_STATE_CHECKED);
        if(btn_state_b)
        {
            lv_obj_clear_state(objects.param_slider, LV_STATE_DISABLED);
            lv_obj_clear_state(objects.param_spinbox, LV_STATE_DISABLED);
        }
        else
        {
            lv_obj_add_state(objects.param_slider, LV_STATE_DISABLED);
            lv_obj_add_state(objects.param_spinbox, LV_STATE_DISABLED);
        }
    }
}
