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
#include "main.h"

extern lv_indev_t *     input_encoder_ps;
extern lv_indev_t *     input_push_btn_ps;

extern float*           pcc_param_duty_cycle_pf32;
extern float*           pcc_param_sw_freq_pf32;
extern float*           pcc_param_deadtime_pf32;
extern float*           pcc_param_phase_shift_pf32;
extern float*           pcc_param_mod_freq_pf32;
extern float*           pcc_param_amplitude_pf32;

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

    lv_label_set_text(objects.topo_state_label, "READY");
    lv_obj_set_style_text_color(objects.topo_state_label, (lv_color_t){.green = 255}, LV_PART_MAIN);

    set_var_pcc_param_duty_edit_en(true);
    set_var_pcc_param_sw_freq_edit_en(true);
    set_var_pcc_param_phase_shift_edit_en(true);
    set_var_pcc_param_amplitude_edit_en(true);
    set_var_pcc_param_mod_freq_edit_en(true);
    PCC_InitializeActiveTopology_b();
}

void action_topology_screen_unloaded(lv_event_t *e) {
    printf("%s\n", __func__);
    lv_group_set_editing(groups.param_selector, false);
    PCC_DeinitializeActiveTopology_v();
}

void start_topology_callback(lv_event_t *e) {
    printf("%s\n", __func__);
    char tmp_str[100];
    PCC_Params_struct* params_ps = PCC_GetActiveTopologyParameters_ps();
    lv_label_set_text(objects.topo_state_label, "RUNNING");
    lv_obj_set_style_text_color(objects.topo_state_label, (lv_color_t){.red = 255}, LV_PART_MAIN);

    remove_all_indev_events(input_push_btn_ps);
    lv_indev_add_event_cb(input_push_btn_ps, stop_topology_callback, LV_EVENT_PRESSED, NULL);

    switch(params_ps->type_e)
    {
        case PCC_ParamType_PWM_e:
            if(!get_var_pcc_param_duty_edit_en()) {
                lv_obj_clear_flag(objects.pcc_param__duty_cycle_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(objects.ctrl_param_duty_cnt, LV_OBJ_FLAG_HIDDEN);
                snprintf(tmp_str, sizeof(tmp_str), "Duty cycle: %.2f %%", params_ps->PWM_struct.duty_cycle__per_cent__s.val_f32);
                lv_label_set_text(objects.pcc_param__duty_cycle_edit_disabled_val_label, tmp_str);
            }

            if(!get_var_pcc_param_sw_freq_edit_en()) {
                lv_obj_clear_flag(objects.pcc_param__sw_freq_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
                snprintf(tmp_str, sizeof(tmp_str), "Switching frequency: %.2f Hz", params_ps->PWM_struct.frequency__Hz__s.val_f32);
                lv_label_set_text(objects.pcc_param__sw_freq_edit_disabled_val_label, tmp_str);
            }


            lv_obj_add_flag(objects.ctrl_param__sw_freq_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.ctrl_param_1__duty_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            break;

        case PCC_ParamType_ComplementaryPWM_e:
            if(!get_var_pcc_param_duty_edit_en()) {
                lv_obj_clear_flag(objects.pcc_param__duty_cycle_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(objects.ctrl_param_duty_cnt, LV_OBJ_FLAG_HIDDEN);
                snprintf(tmp_str, sizeof(tmp_str), "Duty cycle: %.2f %%", params_ps->ComplementaryPWM_struct.duty_cycle__per_cent__s.val_f32);
                lv_label_set_text(objects.pcc_param__duty_cycle_edit_disabled_val_label, tmp_str);
            }

            if(!get_var_pcc_param_sw_freq_edit_en()) {
                lv_obj_clear_flag(objects.pcc_param__sw_freq_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
                snprintf(tmp_str, sizeof(tmp_str), "Switching frequency: %.2f Hz", params_ps->ComplementaryPWM_struct.frequency__Hz__s.val_f32);
                lv_label_set_text(objects.pcc_param__sw_freq_edit_disabled_val_label, tmp_str);
            }

            lv_obj_add_flag(objects.ctrl_param__sw_freq_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.ctrl_param_1__duty_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.ctrl_param_deadtime_cnt, LV_OBJ_FLAG_HIDDEN);
            snprintf(tmp_str, sizeof(tmp_str), "Deadtime: %d ns", get_var_pcc_param_deadtime_i32());
            lv_label_set_text(objects.pcc_param__deadtime_edit_disabled_val_label, tmp_str);
            lv_obj_clear_flag(objects.pcc_param__deadtime_edit_disabled_val_label, LV_OBJ_FLAG_HIDDEN);
            break;

        case PCC_ParamType_PhaseShiftedPWM_e:
            if(!get_var_pcc_param_phase_shift_edit_en()) {
                lv_obj_clear_flag(objects.pcc_param__phase_shift_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(objects.ctrl_param_phase_shift_cnt, LV_OBJ_FLAG_HIDDEN);
                snprintf(tmp_str, sizeof(tmp_str), "Phase shift: %.2f degree", params_ps->PhaseShiftedPWM_struct.phase_shift__deg__s.val_f32);
                lv_label_set_text(objects.pcc_param__phase_shift_edit_disabled_val_label, tmp_str);
            }

            if(!get_var_pcc_param_sw_freq_edit_en()) {
                lv_obj_clear_flag(objects.pcc_param__sw_freq_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
                snprintf(tmp_str, sizeof(tmp_str), "Switching frequency: %.2f Hz", params_ps->PhaseShiftedPWM_struct.frequency__Hz__s.val_f32);
                lv_label_set_text(objects.pcc_param__sw_freq_edit_disabled_val_label, tmp_str);
            }

            lv_obj_add_flag(objects.ctrl_param__sw_freq_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.ctrl_param__phase_shift_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.ctrl_param_deadtime_cnt, LV_OBJ_FLAG_HIDDEN);
            snprintf(tmp_str, sizeof(tmp_str), "Deadtime: %d ns", get_var_pcc_param_deadtime_i32());
            lv_label_set_text(objects.pcc_param__deadtime_edit_disabled_val_label, tmp_str);
            lv_obj_clear_flag(objects.pcc_param__deadtime_edit_disabled_val_label, LV_OBJ_FLAG_HIDDEN);
            break;

        case PCC_ParamType_SinePWM_e:
            if(!get_var_pcc_param_sw_freq_edit_en()) {
                lv_obj_clear_flag(objects.pcc_param__sw_freq_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
                snprintf(tmp_str, sizeof(tmp_str), "Switching frequency: %.2f Hz", params_ps->SinePWM_struct.switching_frequency__Hz__s.val_f32);
                lv_label_set_text(objects.pcc_param__sw_freq_edit_disabled_val_label, tmp_str);
            }

            if(!get_var_pcc_param_mod_freq_edit_en()) {
                lv_obj_clear_flag(objects.pcc_param__mod_freq_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(objects.ctrl_param_mod_freq_cnt, LV_OBJ_FLAG_HIDDEN);
                snprintf(tmp_str, sizeof(tmp_str), "Modulating frequency: %.2f Hz", params_ps->SinePWM_struct.modulation_frequency__Hz__s.val_f32);
                lv_label_set_text(objects.pcc_param__mod_freq_edit_disabled_val_label, tmp_str);
            }

            if(!get_var_pcc_param_amplitude_edit_en()) {
                lv_obj_clear_flag(objects.pcc_param__amplitude_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(objects.ctrl_param_amplitude_cnt, LV_OBJ_FLAG_HIDDEN);
                snprintf(tmp_str, sizeof(tmp_str), "Amplitude: %.2f %%", params_ps->SinePWM_struct.amplitude__per_cent__s.val_f32);
                lv_label_set_text(objects.pcc_param__amplitude_edit_disabled_val_label, tmp_str);
            }

            lv_obj_add_flag(objects.ctrl_param__sw_freq_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.ctrl_param__mod_freq_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.ctrl_param__amplitude_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.ctrl_param_deadtime_cnt, LV_OBJ_FLAG_HIDDEN);
            snprintf(tmp_str, sizeof(tmp_str), "Deadtime: %d ns", get_var_pcc_param_deadtime_i32());
            lv_label_set_text(objects.pcc_param__deadtime_edit_disabled_val_label, tmp_str);
            lv_obj_clear_flag(objects.pcc_param__deadtime_edit_disabled_val_label, LV_OBJ_FLAG_HIDDEN);
            break;

        default:
            break;
    }
    PCC_StartActiveTopology_v();
}

void stop_topology_callback(lv_event_t *e) {
    printf("%s\n", __func__);

    PCC_StopActiveTopology_v();
    lv_indev_wait_release(input_push_btn_ps);
    lv_label_set_text(objects.topo_state_label, "READY");
    lv_obj_set_style_text_color(objects.topo_state_label, (lv_color_t){.green = 255}, LV_PART_MAIN);
    remove_all_indev_events(input_push_btn_ps);
    lv_indev_add_event_cb(input_push_btn_ps, action_go_to_power_topology_menu, LV_EVENT_PRESSED, NULL);
    lv_indev_add_event_cb(input_push_btn_ps, start_topology_callback, LV_EVENT_LONG_PRESSED, NULL);

    switch(PCC_GetActiveTopologyParameters_ps()->type_e)
    {
        case PCC_ParamType_PWM_e:
            lv_obj_clear_flag(objects.ctrl_param__sw_freq_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_1__duty_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_duty_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__duty_cycle_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__sw_freq_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);

            break;

        case PCC_ParamType_ComplementaryPWM_e:
            lv_obj_clear_flag(objects.ctrl_param__sw_freq_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_1__duty_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_duty_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_deadtime_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__duty_cycle_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__sw_freq_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__deadtime_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            break;

        case PCC_ParamType_PhaseShiftedPWM_e:
            lv_obj_clear_flag(objects.ctrl_param__sw_freq_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param__phase_shift_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_phase_shift_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_deadtime_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__phase_shift_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__sw_freq_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__deadtime_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            break;

        case PCC_ParamType_SinePWM_e:
            lv_obj_clear_flag(objects.ctrl_param__sw_freq_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param__amplitude_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param__mod_freq_edit_en_btn, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_mod_freq_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_amplitude_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_deadtime_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__sw_freq_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__mod_freq_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__amplitude_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(objects.pcc_param__deadtime_edit_disabled_val_label,LV_OBJ_FLAG_HIDDEN);
            break;

        default:
            break;
    }
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

    char tmp_str[100];
    lv_obj_t* child_ps;
    PCC_TopologyHandle_struct* topo_handle_s = (PCC_TopologyHandle_struct*)lv_event_get_user_data(e);

    if(PCC_SetTopology_b(topo_handle_s) == False_b)
    {
        Error_Handler();
    }

    lv_label_set_text(objects.topo_name_label, topo_handle_s->ctrl_name_str);

    for(u32 child_iu32 = 0; child_iu32 < lv_obj_get_child_count(objects.ctrl_param_main_cnt); child_iu32++) {
        child_ps = lv_obj_get_child(objects.ctrl_param_main_cnt, child_iu32);
        lv_obj_add_flag(child_ps, LV_OBJ_FLAG_HIDDEN);
    }

    switch(topo_handle_s->ctrl_params_pv->type_e)
    {
        case PCC_ParamType_PWM_e:
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->PWM_struct.frequency__Hz__s.min_f32);
            lv_label_set_text(objects.ctrl_param__sw_freq_min_val_placeholder_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->PWM_struct.frequency__Hz__s.max_f32);
            lv_label_set_text(objects.ctrl_param__sw_freq_max_val_placeholder_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->PWM_struct.duty_cycle__per_cent__s.min_f32);
            lv_label_set_text(objects.ctrl_param_1__duty_min_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->PWM_struct.duty_cycle__per_cent__s.max_f32);
            lv_label_set_text(objects.ctrl_param_1__duty_max_label, tmp_str);

            lv_slider_set_range(objects.ctrl_param_1__duty_slider,
                                topo_handle_s->ctrl_params_pv->PWM_struct.duty_cycle__per_cent__s.min_f32,
                                topo_handle_s->ctrl_params_pv->PWM_struct.duty_cycle__per_cent__s.max_f32);

            lv_spinbox_set_range(objects.ctrl_param_1__duty_spinbox,
                                 topo_handle_s->ctrl_params_pv->PWM_struct.duty_cycle__per_cent__s.min_f32 * 100.0f,
                                 topo_handle_s->ctrl_params_pv->PWM_struct.duty_cycle__per_cent__s.max_f32 * 100.0f);

            lv_spinbox_set_range(objects.ctrl_param__sw_freq_spinbox,
                                topo_handle_s->ctrl_params_pv->PWM_struct.frequency__Hz__s.min_f32 * 100.0f,
                                topo_handle_s->ctrl_params_pv->PWM_struct.frequency__Hz__s.max_f32 * 100.0f);


            lv_obj_clear_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_duty_cnt, LV_OBJ_FLAG_HIDDEN);

            pcc_param_duty_cycle_pf32 = &topo_handle_s->ctrl_params_pv->PWM_struct.duty_cycle__per_cent__s.val_f32;
            pcc_param_sw_freq_pf32 = &topo_handle_s->ctrl_params_pv->PWM_struct.frequency__Hz__s.val_f32;
            break;

        case PCC_ParamType_ComplementaryPWM_e:
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.frequency__Hz__s.min_f32);
            lv_label_set_text(objects.ctrl_param__sw_freq_min_val_placeholder_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.frequency__Hz__s.max_f32);
            lv_label_set_text(objects.ctrl_param__sw_freq_max_val_placeholder_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.duty_cycle__per_cent__s.min_f32);
            lv_label_set_text(objects.ctrl_param_1__duty_min_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.duty_cycle__per_cent__s.max_f32);
            lv_label_set_text(objects.ctrl_param_1__duty_max_label, tmp_str);

            lv_slider_set_range(objects.ctrl_param_1__duty_slider,
                                topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.duty_cycle__per_cent__s.min_f32,
                                topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.duty_cycle__per_cent__s.max_f32);

            lv_spinbox_set_range(objects.ctrl_param_1__duty_spinbox,
                                 topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.duty_cycle__per_cent__s.min_f32 * 100.0f,
                                 topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.duty_cycle__per_cent__s.max_f32 * 100.0f);

            lv_spinbox_set_range(objects.ctrl_param__sw_freq_spinbox,
                                topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.frequency__Hz__s.min_f32 * 100.0f,
                                topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.frequency__Hz__s.max_f32 * 100.0f);

            lv_spinbox_set_range(objects.ctrl_param__deadtime_spinbox,
                                 0,
                                 5000);

            lv_obj_clear_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_duty_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_deadtime_cnt, LV_OBJ_FLAG_HIDDEN);

            pcc_param_duty_cycle_pf32 = &topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.duty_cycle__per_cent__s.val_f32;
            pcc_param_sw_freq_pf32 = &topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.frequency__Hz__s.val_f32;
            pcc_param_deadtime_pf32 = &topo_handle_s->ctrl_params_pv->ComplementaryPWM_struct.dead_time__s__s.val_f32;
            break;

        case PCC_ParamType_PhaseShiftedPWM_e:
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.frequency__Hz__s.min_f32);
            lv_label_set_text(objects.ctrl_param__sw_freq_min_val_placeholder_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.frequency__Hz__s.max_f32);
            lv_label_set_text(objects.ctrl_param__sw_freq_max_val_placeholder_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.phase_shift__deg__s.min_f32);
            lv_label_set_text(objects.ctrl_param__phase_shift_min_val_palceholder_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.phase_shift__deg__s.max_f32);
            lv_label_set_text(objects.ctrl_param__phase_shift_max_val_palceholder_label, tmp_str);

            lv_slider_set_range(objects.ctrl_param__phase_shift_slider,
                                topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.phase_shift__deg__s.min_f32,
                                topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.phase_shift__deg__s.max_f32);

            lv_spinbox_set_range(objects.ctrl_param__phase_shift_spinbox,
                                 topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.phase_shift__deg__s.min_f32 * 100.0f,
                                 topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.phase_shift__deg__s.max_f32 * 100.0f);

            lv_spinbox_set_range(objects.ctrl_param__sw_freq_spinbox,
                                topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.frequency__Hz__s.min_f32 * 100.0f,
                                topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.frequency__Hz__s.max_f32 * 100.0f);

            lv_obj_clear_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_phase_shift_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_deadtime_cnt, LV_OBJ_FLAG_HIDDEN);

            pcc_param_phase_shift_pf32 = &topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.phase_shift__deg__s.val_f32;
            pcc_param_sw_freq_pf32 = &topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.frequency__Hz__s.val_f32;
            pcc_param_deadtime_pf32 = &topo_handle_s->ctrl_params_pv->PhaseShiftedPWM_struct.dead_time__s__s.val_f32;
            break;

        case PCC_ParamType_SinePWM_e:
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->SinePWM_struct.switching_frequency__Hz__s.min_f32);
            lv_label_set_text(objects.ctrl_param__sw_freq_min_val_placeholder_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->SinePWM_struct.switching_frequency__Hz__s.max_f32);
            lv_label_set_text(objects.ctrl_param__sw_freq_max_val_placeholder_label, tmp_str);

            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->SinePWM_struct.modulation_frequency__Hz__s.min_f32);
            lv_label_set_text(objects.ctrl_param__mod_freq_min_val_palceholder_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->SinePWM_struct.modulation_frequency__Hz__s.max_f32);
            lv_label_set_text(objects.ctrl_param__mod_freq_max_val_palceholder_label, tmp_str);

            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->SinePWM_struct.amplitude__per_cent__s.min_f32);
            lv_label_set_text(objects.ctrl_param__amplitude_min_val_palceholder_label, tmp_str);
            snprintf(tmp_str, sizeof(tmp_str), "%.0f", topo_handle_s->ctrl_params_pv->SinePWM_struct.amplitude__per_cent__s.max_f32);
            lv_label_set_text(objects.ctrl_param__amplitude_max_val_palceholder_label, tmp_str);

            lv_slider_set_range(objects.ctrl_param__mod_freq_slider,
                                topo_handle_s->ctrl_params_pv->SinePWM_struct.modulation_frequency__Hz__s.min_f32,
                                topo_handle_s->ctrl_params_pv->SinePWM_struct.modulation_frequency__Hz__s.max_f32);

            lv_slider_set_range(objects.ctrl_param__amplitude_slider,
                                 topo_handle_s->ctrl_params_pv->SinePWM_struct.amplitude__per_cent__s.min_f32,
                                 topo_handle_s->ctrl_params_pv->SinePWM_struct.amplitude__per_cent__s.max_f32);

            lv_spinbox_set_range(objects.ctrl_param__mod_freq_spinbox,
                                topo_handle_s->ctrl_params_pv->SinePWM_struct.modulation_frequency__Hz__s.min_f32 * 100.0f,
                                topo_handle_s->ctrl_params_pv->SinePWM_struct.modulation_frequency__Hz__s.max_f32 * 100.0f);

            lv_spinbox_set_range(objects.ctrl_param__amplitude_spinbox,
                                 topo_handle_s->ctrl_params_pv->SinePWM_struct.amplitude__per_cent__s.min_f32 * 100.0f,
                                 topo_handle_s->ctrl_params_pv->SinePWM_struct.amplitude__per_cent__s.max_f32 * 100.0f);

            lv_spinbox_set_range(objects.ctrl_param__sw_freq_spinbox,
                                topo_handle_s->ctrl_params_pv->SinePWM_struct.switching_frequency__Hz__s.min_f32 * 100.0f,
                                topo_handle_s->ctrl_params_pv->SinePWM_struct.switching_frequency__Hz__s.max_f32 * 100.0f);

            lv_obj_clear_flag(objects.ctrl_param_amplitude_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_sw_freq_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_mod_freq_cnt, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(objects.ctrl_param_deadtime_cnt, LV_OBJ_FLAG_HIDDEN);

            pcc_param_amplitude_pf32    = &topo_handle_s->ctrl_params_pv->SinePWM_struct.amplitude__per_cent__s.val_f32;
            pcc_param_sw_freq_pf32      = &topo_handle_s->ctrl_params_pv->SinePWM_struct.switching_frequency__Hz__s.val_f32;
            pcc_param_mod_freq_pf32     = &topo_handle_s->ctrl_params_pv->SinePWM_struct.modulation_frequency__Hz__s.val_f32;
            pcc_param_deadtime_pf32     = &topo_handle_s->ctrl_params_pv->SinePWM_struct.dead_time__s__s.val_f32;
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
