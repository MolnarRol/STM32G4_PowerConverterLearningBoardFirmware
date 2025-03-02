#include "ui.h"
#include "actions.h"
#include "screens.h"
#include <stm32g4xx.h>
#include <lvgl.h>
#include <app_types.h>
#include <PCC_public_interface.h>

extern lv_indev_t* encoder_input_device_ps;
extern lv_indev_t* btn_input_device_ps;

void action_back_to_main_menu(lv_event_t *e) {
    loadScreen(SCREEN_ID_MAIN);
}

void action_go_to_power_topology_menu(lv_event_t * e)
{
    lv_indev_set_group(encoder_input_device_ps, groups.param_selector);

    PCC_SetTopology_b(PCC_TOPO_SinglePWM_e);
    PCC_InitializeActiveTopology_b();
    PCC_StartActiveTopology_v();

    loadScreen(SCREEN_ID_SIMPLE_PWM);
}
