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
#include "vars.h"
#include "stdio.h"

extern lv_indev_t *            input_encoder_ps;
extern lv_indev_t *            input_push_btn_ps;

void start_topology_callback(lv_event_t *e);
void stop_topology_callback(lv_event_t *e);

static void remove_all_indev_events(lv_indev_t * indev_p) {
    printf("%s\n", __func__);
    for(u32 event_count_u32 = lv_indev_get_event_count(input_push_btn_ps); event_count_u32 > 0; event_count_u32--)
    {
        lv_indev_remove_event(indev_p, event_count_u32 - 1);
    }
}

/***************************************************************************************************************
 *  Topology screens callbacks.
 ***************************************************************************************************************/
void action_topology_screen_loaded(lv_event_t *e) {
    printf("%s\n", __func__);
    lv_indev_set_group(input_encoder_ps, groups.param_selector);

    remove_all_indev_events(input_push_btn_ps);
    lv_indev_add_event_cb(input_push_btn_ps, action_go_to_power_topology_menu, LV_EVENT_PRESSED, NULL);
    lv_indev_add_event_cb(input_push_btn_ps, start_topology_callback, LV_EVENT_LONG_PRESSED, NULL);

    set_var_param_1_en_b(true);
    set_var_param_2_en_b(true);
    set_var_param_3_en_b(true);

    lv_label_set_text(objects.topo_name_label, PCC_GetCurrentTopologyHandle_ps()->ctrl_name_str);
}

void action_topology_screen_unloaded(lv_event_t *e) {
    printf("%s\n", __func__);
    lv_group_set_editing(groups.param_selector, false);
}

void start_topology_callback(lv_event_t *e) {
    printf("%s\n", __func__);
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_add_event_cb(input_push_btn_ps, stop_topology_callback, LV_EVENT_PRESSED, NULL);
}

void stop_topology_callback(lv_event_t *e) {
    printf("%s\n", __func__);
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_add_event_cb(input_push_btn_ps, action_go_to_power_topology_menu, LV_EVENT_PRESSED, NULL);
    lv_indev_add_event_cb(input_push_btn_ps, start_topology_callback, LV_EVENT_LONG_PRESSED, NULL);

}

/***************************************************************************************************************
 *  Settings screen callbacks.
 ***************************************************************************************************************/
void action_settings_screen_loaded(lv_event_t *e) {
    printf("%s\n", __func__);
    static lv_obj_t* project_url_qr_ps = NULL;
    static const char* project_url_p = PROD_SOURCE_URL_d;

    remove_all_indev_events(input_push_btn_ps);
    lv_indev_wait_release(input_encoder_ps);
    lv_indev_wait_release(input_push_btn_ps);

    if(project_url_qr_ps == NULL) {
        project_url_qr_ps = lv_qrcode_create(objects.sw_info_container);
        lv_qrcode_set_size(project_url_qr_ps, lv_obj_get_height(objects.sw_info_container) - 300);
        lv_qrcode_update(project_url_qr_ps, project_url_p, strlen(project_url_p));
        lv_obj_align(project_url_qr_ps, LV_ALIGN_BOTTOM_RIGHT, -4, -4);
    }

    lv_label_set_text(objects.sw_version_str_label, PROD_SW_VERSION_STR_d);
    lv_label_set_text(objects.hw_revision_str_label, PROD_HW_REVISION_STR_d);

    lv_indev_set_group(input_encoder_ps, groups.settings_group);
    lv_indev_add_event_cb(input_push_btn_ps, action_back_to_main_menu, LV_EVENT_PRESSED, NULL);
}

void action_settings_screen_unloaded(lv_event_t *e) {
    printf("%s\n", __func__);
    lv_group_set_editing(groups.settings_group, false);
}

/***************************************************************************************************************
 *  Main screen callbacks.
 ***************************************************************************************************************/
void action_main_screen_loaded(lv_event_t *e) {
    printf("%s\n", __func__);
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_set_group(input_encoder_ps, groups.MainGroup);
}

/***************************************************************************************************************
 *  Topology select screen.
 ***************************************************************************************************************/
void action_pcc_topology_menu_loaded(lv_event_t *e) {
    printf("%s\n", __func__);
    lv_indev_wait_release(input_encoder_ps);
    remove_all_indev_events(input_push_btn_ps);
    ui_init_pcc_menu_v();

    lv_indev_set_group(input_encoder_ps, groups.pcc_topology_select_grp);
    lv_indev_add_event_cb(input_push_btn_ps, action_back_to_main_menu, LV_EVENT_PRESSED, NULL);
}

void action_pcc_topology_menu_unloaded(lv_event_t *e) {
    printf("%s\n", __func__);
    lv_group_set_editing(groups.pcc_topology_select_grp, false);
}

void action_load_pcc_topology_ctrl_screen(lv_event_t *e) {
    printf("%s\n", __func__);
    PCC_TopologyHandle_struct* topo_handle_s = (PCC_TopologyHandle_struct*)lv_event_get_user_data(e);

    switch(topo_handle_s->ctrl_params_pv->type_e)
    {
        case PCC_ParamType_PWM_e:
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
    loadScreen(SCREEN_ID_TOPO_CTRL_PARAMETER_SCR);
}

/***************************************************************************************************************
 *  Screen switch events.
 ***************************************************************************************************************/
void action_go_to_settings(lv_event_t *e) {
    printf("%s\n", __func__);
    loadScreen(SCREEN_ID_SETTINGS);
}

void action_go_to_power_topology_menu(lv_event_t * e) {
    printf("%s\n", __func__);
    loadScreen(SCREEN_ID_POWER_TOPOLOGY_MENU);
}

void action_back_to_main_menu(lv_event_t *e) {
    printf("%s\n", __func__);
    loadScreen(SCREEN_ID_MAIN);
}
