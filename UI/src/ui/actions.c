#include "ui.h"
#include "actions.h"
#include "screens.h"
#include <stm32g4xx.h>
#include <lvgl.h>
#include <app_types.h>
#include <PCC_public_interface.h>
#include "pcc_menu.h"
#include "PCC_private_interface.h"
#include "general_config.h"

extern lv_indev_t *            input_encoder_ps;
extern lv_indev_t *            input_push_btn_ps;

void action_start_topology(lv_event_t *e);
void action_stop_topology(lv_event_t *e);

static void remove_all_indev_events(lv_indev_t * indev_p)
{
    for(u32 event_count_u32 = lv_indev_get_event_count(input_push_btn_ps); event_count_u32 > 0; event_count_u32--)
    {
        lv_indev_remove_event(indev_p, event_count_u32 - 1);
    }
}

void action_topology_simple_pwm_loaded(lv_event_t *e) {
    lv_indev_wait_release(input_encoder_ps);
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_add_event_cb(input_push_btn_ps, action_go_to_power_topology_menu, LV_EVENT_PRESSED, NULL);
    lv_indev_set_group(input_encoder_ps, groups.param_selector);
}


void action_back_to_main_menu(lv_event_t *e) {

    loadScreen(SCREEN_ID_MAIN);
}

void action_main_screen_loaded(lv_event_t *e) {
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_wait_release(input_encoder_ps);
    lv_indev_wait_release(input_push_btn_ps);
    lv_indev_set_group(input_encoder_ps, groups.MainGroup);
}

void action_settings_screen_loaded(lv_event_t *e) {
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_wait_release(input_encoder_ps);
    lv_indev_wait_release(input_push_btn_ps);

    lv_obj_t* qr = lv_qrcode_create(objects.sw_info_container);
    lv_qrcode_set_size(qr, lv_obj_get_height(objects.sw_info_container) - 300);
    const char* data = "https://github.com/MolnarRol/STM32G4_PowerConverterLearningBoardFirmware";
    lv_qrcode_update(qr, data, strlen(data));
    lv_obj_align(qr, LV_ALIGN_BOTTOM_RIGHT, -4, -4);

    lv_label_set_text(objects.sw_version_str_label, PROD_SW_VERSION_STR_d);
    lv_label_set_text(objects.hw_revision_str_label, PROD_HW_REVISION_STR_d);

    lv_indev_set_group(input_encoder_ps, groups.settings_group);
    lv_indev_add_event_cb(input_push_btn_ps, action_back_to_main_menu, LV_EVENT_PRESSED, NULL);
}

void action_stop_topology(lv_event_t *e)
{
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_add_event_cb(input_push_btn_ps, action_back_to_main_menu, LV_EVENT_PRESSED, NULL);
    lv_indev_add_event_cb(input_push_btn_ps, action_start_topology, LV_EVENT_LONG_PRESSED, NULL);
}

void action_start_topology(lv_event_t *e) {
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_add_event_cb(input_push_btn_ps, action_stop_topology, LV_EVENT_PRESSED, NULL);
}

void action_go_to_power_topology_menu(lv_event_t * e)
{
    lv_indev_wait_release(input_encoder_ps);
    loadScreen(SCREEN_ID_POWER_TOPOLOGY_MENU);
}

void action_pcc_topology_menu_loaded(lv_event_t *e) {
    lv_indev_wait_release(input_encoder_ps);

    ui_init_pcc_menu_v();

    lv_indev_set_group(input_encoder_ps, groups.pcc_topology_select_grp);
    lv_indev_wait_release(input_push_btn_ps);
    lv_indev_add_event_cb(input_push_btn_ps, action_back_to_main_menu, LV_EVENT_PRESSED, NULL);
}

void action_load_pcc_topology_ctrl_screen(lv_event_t *e) {
    PCC_TopologyHandle_struct* topo_handle_s = (PCC_TopologyHandle_struct*)lv_event_get_user_data(e);

    switch(topo_handle_s->ctrl_params_pv->type_e)
    {
        case PCC_ParamType_PWM_e:
            loadScreen(SCREEN_ID_SIMPLE_PWM);
            break;

        case PCC_ParamType_ComplementaryPWM_e:
            break;

        case PCC_ParamType_PhaseShiftedPWM_e:
            break;

        case PCC_ParamType_SinePWM_e:
            break;

        case PCC_ParamType_LineCommutated_e:
            break;

        default:
            break;
    }
}

void action_go_to_settings(lv_event_t *e) {
    loadScreen(SCREEN_ID_SETTINGS);
}

