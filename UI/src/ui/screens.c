#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

groups_t groups;
static bool groups_created = false;

objects_t objects;
lv_obj_t *tick_value_change_obj;

static void event_handler_cb_main_main(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: MainGroup
        lv_group_remove_all_objs(groups.MainGroup);
        lv_group_add_obj(groups.MainGroup, objects.obj0);
        lv_group_add_obj(groups.MainGroup, objects.settings_btn);
        // group: param_selector
        lv_group_remove_all_objs(groups.param_selector);
        // group: pcc_topology_select_grp
        lv_group_remove_all_objs(groups.pcc_topology_select_grp);
        // group: settings_group
        lv_group_remove_all_objs(groups.settings_group);
    }
}

static void event_handler_cb_power_topology_menu_power_topology_menu(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: MainGroup
        lv_group_remove_all_objs(groups.MainGroup);
        // group: param_selector
        lv_group_remove_all_objs(groups.param_selector);
        // group: pcc_topology_select_grp
        lv_group_remove_all_objs(groups.pcc_topology_select_grp);
        // group: settings_group
        lv_group_remove_all_objs(groups.settings_group);
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_topo_ctrl_parameter_scr(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: MainGroup
        lv_group_remove_all_objs(groups.MainGroup);
        // group: param_selector
        lv_group_remove_all_objs(groups.param_selector);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__sw_freq_edit_en_btn);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__sw_freq_spinbox);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__phase_shift_edit_en_btn);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__phase_shift_spinbox);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__phase_shift_slider);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__mod_freq_edit_en_btn);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__mod_freq_spinbox);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__mod_freq_slider);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param_1__duty_edit_en_btn);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param_1__duty_spinbox);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param_1__duty_slider);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__amplitude_edit_en_btn);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__amplitude_spinbox);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__amplitude_slider);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__deadtime_spinbox);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__firing_angle_edit_en_btn);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__firing_angle_spinbox);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__firing_angle_slider);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__pulse_len_edit_en_btn);
        lv_group_add_obj(groups.param_selector, objects.ctrl_param__pulse_len_spinbox);
        // group: pcc_topology_select_grp
        lv_group_remove_all_objs(groups.pcc_topology_select_grp);
        // group: settings_group
        lv_group_remove_all_objs(groups.settings_group);
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__sw_freq_edit_en_btn(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_pcc_param_sw_freq_edit_en(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__sw_freq_spinbox(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_spinbox_get_value(ta);
            set_var_pcc_param_sw_freq_i32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__phase_shift_edit_en_btn(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_pcc_param_phase_shift_edit_en(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__phase_shift_spinbox(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_spinbox_get_value(ta);
            set_var_pcc_param_phase_shift_i32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__phase_shift_slider(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            set_var_pcc_param_phase_shift_f32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__mod_freq_edit_en_btn(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_pcc_param_mod_freq_edit_en(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__mod_freq_spinbox(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_spinbox_get_value(ta);
            set_var_pcc_param_mod_freq_i32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__mod_freq_slider(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            set_var_pcc_param_mod_freq_f32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param_1__duty_edit_en_btn(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_pcc_param_duty_edit_en(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param_1__duty_spinbox(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_spinbox_get_value(ta);
            set_var_pcc_param_duty_cycle_i32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param_1__duty_slider(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            set_var_pcc_param_duty_cycle_f32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__amplitude_edit_en_btn(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_pcc_param_amplitude_edit_en(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__amplitude_spinbox(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_spinbox_get_value(ta);
            set_var_pcc_param_amplitude_i32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__amplitude_slider(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            set_var_pcc_param_amplitude_f32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__deadtime_spinbox(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_spinbox_get_value(ta);
            set_var_pcc_param_deadtime_i32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__firing_angle_edit_en_btn(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_pcc_param_firing_angle_edit_en(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__firing_angle_spinbox(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_spinbox_get_value(ta);
            set_var_pcc_param_firing_angle_i32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__firing_angle_slider(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            set_var_pcc_param_firing_angle_f32(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__pulse_len_edit_en_btn(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_pcc_param_pulse_len_edit_en(value);
        }
    }
}

static void event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__pulse_len_spinbox(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_spinbox_get_value(ta);
            set_var_pcc_param_pulse_len_i32(value);
        }
    }
}

static void event_handler_cb_settings_settings(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: MainGroup
        lv_group_remove_all_objs(groups.MainGroup);
        // group: param_selector
        lv_group_remove_all_objs(groups.param_selector);
        // group: pcc_topology_select_grp
        lv_group_remove_all_objs(groups.pcc_topology_select_grp);
        // group: settings_group
        lv_group_remove_all_objs(groups.settings_group);
        lv_group_add_obj(groups.settings_group, objects.brightness_slider);
    }
}

static void event_handler_cb_settings_brightness_slider(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_slider_get_value(ta);
            set_var_brightness__per_cent__f32(value);
        }
    }
}

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_main_screen_loaded, LV_EVENT_SCREEN_LOADED, (void *)0);
    lv_obj_add_event_cb(obj, event_handler_cb_main_main, LV_EVENT_ALL, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), 25);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_label_set_text(obj, "Menu");
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4c4c4c), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 0, 25);
            lv_obj_set_size(obj, 320, 215);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_pos(obj, 32, 40);
                    lv_obj_set_size(obj, 300, 50);
                    lv_obj_add_event_cb(obj, action_go_to_power_topology_menu, LV_EVENT_PRESSED, (void *)0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    add_style_default_btn(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // PowerTopologyControlBtn
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.power_topology_control_btn = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Power topology control");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // SettingsBtn
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.settings_btn = obj;
                    lv_obj_set_pos(obj, LV_PCT(33), 121);
                    lv_obj_set_size(obj, 300, 50);
                    lv_obj_add_event_cb(obj, action_go_to_settings, LV_EVENT_PRESSED, (void *)0);
                    add_style_default_btn(obj);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4c4c4c), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_left(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Settings");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_power_topology_menu() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.power_topology_menu = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_pcc_topology_menu_loaded, LV_EVENT_SCREEN_LOADED, (void *)0);
    lv_obj_add_event_cb(obj, action_pcc_topology_menu_unloaded, LV_EVENT_SCREEN_UNLOADED, (void *)0);
    lv_obj_add_event_cb(obj, event_handler_cb_power_topology_menu_power_topology_menu, LV_EVENT_ALL, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    tick_screen_power_topology_menu();
}

void tick_screen_power_topology_menu() {
}

void create_screen_topo_ctrl_parameter_scr() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.topo_ctrl_parameter_scr = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_topology_screen_loaded, LV_EVENT_SCREEN_LOADED, (void *)0);
    lv_obj_add_event_cb(obj, action_topology_screen_unloaded, LV_EVENT_SCREEN_UNLOADED, (void *)0);
    lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_topo_ctrl_parameter_scr, LV_EVENT_ALL, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // topo_name_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.topo_name_label = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(70), 20);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_label_set_text(obj, "Name");
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4c4c4c), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // topo_state_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.topo_state_label = obj;
            lv_obj_set_pos(obj, 224, 0);
            lv_obj_set_size(obj, LV_PCT(30), 20);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_label_set_text(obj, "");
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4c4c4c), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // ctrl_param_main_cnt
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.ctrl_param_main_cnt = obj;
            lv_obj_set_pos(obj, LV_PCT(1), 20);
            lv_obj_set_size(obj, LV_PCT(98), 220);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_scroll_dir(obj, LV_DIR_VER);
            lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_START);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ctrl_param_sw_freq_cnt
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ctrl_param_sw_freq_cnt = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), -19);
                    lv_obj_set_size(obj, LV_PCT(100), 74);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ctrl_param__sw_freq_cnt_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__sw_freq_cnt_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Switching frequency [Hz]");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__sw_freq_edit_en_btn
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ctrl_param__sw_freq_edit_en_btn = obj;
                            lv_obj_set_pos(obj, -4, 4);
                            lv_obj_set_size(obj, 40, 20);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__sw_freq_edit_en_btn, LV_EVENT_ALL, 0);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_margin_top(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__sw_freq_spinbox
                            lv_obj_t *obj = lv_spinbox_create(parent_obj);
                            objects.ctrl_param__sw_freq_spinbox = obj;
                            lv_obj_set_pos(obj, -4, -4);
                            lv_obj_set_size(obj, 95, 36);
                            lv_spinbox_set_digit_format(obj, 8, 6);
                            lv_spinbox_set_range(obj, 0, 18000);
                            lv_spinbox_set_rollover(obj, false);
                            lv_spinbox_set_step(obj, 1);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__sw_freq_spinbox, LV_EVENT_ALL, 0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__sw_freq_min_val_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__sw_freq_min_val_label = obj;
                            lv_obj_set_pos(obj, 4, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Min:");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__sw_freq_max_val_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__sw_freq_max_val_label = obj;
                            lv_obj_set_pos(obj, 4, -4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Max:");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__sw_freq_min_val_placeholder_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__sw_freq_min_val_placeholder_label = obj;
                            lv_obj_set_pos(obj, 40, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "<placeholder>");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__sw_freq_max_val_placeholder_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__sw_freq_max_val_placeholder_label = obj;
                            lv_obj_set_pos(obj, 40, -4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "<placeholder>");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // ctrl_param_phase_shift_cnt
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ctrl_param_phase_shift_cnt = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), 74);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ctrl_param__phase_shift_name_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__phase_shift_name_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Phase shift [degree]");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__phase_shift_edit_en_btn
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ctrl_param__phase_shift_edit_en_btn = obj;
                            lv_obj_set_pos(obj, -4, 4);
                            lv_obj_set_size(obj, 40, 20);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__phase_shift_edit_en_btn, LV_EVENT_ALL, 0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__phase_shift_spinbox
                            lv_obj_t *obj = lv_spinbox_create(parent_obj);
                            objects.ctrl_param__phase_shift_spinbox = obj;
                            lv_obj_set_pos(obj, -4, -4);
                            lv_obj_set_size(obj, 67, 36);
                            lv_spinbox_set_digit_format(obj, 5, 3);
                            lv_spinbox_set_range(obj, 0, 18000);
                            lv_spinbox_set_rollover(obj, false);
                            lv_spinbox_set_step(obj, 1);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__phase_shift_spinbox, LV_EVENT_ALL, 0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__phase_shift_slider_cnt
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.ctrl_param__phase_shift_slider_cnt = obj;
                            lv_obj_set_pos(obj, 4, 20);
                            lv_obj_set_size(obj, LV_PCT(75), 49);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // ctrl_param__phase_shift_slider
                                    lv_obj_t *obj = lv_slider_create(parent_obj);
                                    objects.ctrl_param__phase_shift_slider = obj;
                                    lv_obj_set_pos(obj, LV_PCT(5), 9);
                                    lv_obj_set_size(obj, LV_PCT(90), 12);
                                    lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__phase_shift_slider, LV_EVENT_ALL, 0);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                                }
                                {
                                    // ctrl_param__phase_shift_max_val_palceholder_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.ctrl_param__phase_shift_max_val_palceholder_label = obj;
                                    lv_obj_set_pos(obj, -4, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "<placeholder>");
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // ctrl_param__phase_shift_min_val_palceholder_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.ctrl_param__phase_shift_min_val_palceholder_label = obj;
                                    lv_obj_set_pos(obj, 4, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "<placeholder>");
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                            }
                        }
                    }
                }
                {
                    // ctrl_param_mod_freq_cnt
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ctrl_param_mod_freq_cnt = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), 74);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ctrl_param__mod_freq_name_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__mod_freq_name_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Modulation frequency [Hz]");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__mod_freq_edit_en_btn
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ctrl_param__mod_freq_edit_en_btn = obj;
                            lv_obj_set_pos(obj, -4, 4);
                            lv_obj_set_size(obj, 40, 20);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__mod_freq_edit_en_btn, LV_EVENT_ALL, 0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__mod_freq_spinbox
                            lv_obj_t *obj = lv_spinbox_create(parent_obj);
                            objects.ctrl_param__mod_freq_spinbox = obj;
                            lv_obj_set_pos(obj, -4, -4);
                            lv_obj_set_size(obj, 67, 36);
                            lv_spinbox_set_digit_format(obj, 5, 3);
                            lv_spinbox_set_range(obj, 0, 18000);
                            lv_spinbox_set_rollover(obj, false);
                            lv_spinbox_set_step(obj, 1);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__mod_freq_spinbox, LV_EVENT_ALL, 0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__mod_freq_slider_cnt
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.ctrl_param__mod_freq_slider_cnt = obj;
                            lv_obj_set_pos(obj, 4, 20);
                            lv_obj_set_size(obj, LV_PCT(75), 49);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // ctrl_param__mod_freq_slider
                                    lv_obj_t *obj = lv_slider_create(parent_obj);
                                    objects.ctrl_param__mod_freq_slider = obj;
                                    lv_obj_set_pos(obj, LV_PCT(5), 9);
                                    lv_obj_set_size(obj, LV_PCT(90), 12);
                                    lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__mod_freq_slider, LV_EVENT_ALL, 0);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                                }
                                {
                                    // ctrl_param__mod_freq_max_val_palceholder_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.ctrl_param__mod_freq_max_val_palceholder_label = obj;
                                    lv_obj_set_pos(obj, -4, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "<placeholder>");
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // ctrl_param__mod_freq_min_val_palceholder_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.ctrl_param__mod_freq_min_val_palceholder_label = obj;
                                    lv_obj_set_pos(obj, 4, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "<placeholder>");
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                            }
                        }
                    }
                }
                {
                    // ctrl_param_duty_cnt
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ctrl_param_duty_cnt = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), 74);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ctrl_param_1__duty_name_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param_1__duty_name_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Duty cycle [%]");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param_1__duty_edit_en_btn
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ctrl_param_1__duty_edit_en_btn = obj;
                            lv_obj_set_pos(obj, -4, 4);
                            lv_obj_set_size(obj, 40, 20);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param_1__duty_edit_en_btn, LV_EVENT_ALL, 0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param_1__duty_spinbox
                            lv_obj_t *obj = lv_spinbox_create(parent_obj);
                            objects.ctrl_param_1__duty_spinbox = obj;
                            lv_obj_set_pos(obj, -4, -4);
                            lv_obj_set_size(obj, 67, 36);
                            lv_spinbox_set_digit_format(obj, 5, 3);
                            lv_spinbox_set_range(obj, 0, 18000);
                            lv_spinbox_set_rollover(obj, false);
                            lv_spinbox_set_step(obj, 1);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param_1__duty_spinbox, LV_EVENT_ALL, 0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param_1__duty_slider_cnt
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.ctrl_param_1__duty_slider_cnt = obj;
                            lv_obj_set_pos(obj, 4, 20);
                            lv_obj_set_size(obj, LV_PCT(75), 49);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // ctrl_param_1__duty_slider
                                    lv_obj_t *obj = lv_slider_create(parent_obj);
                                    objects.ctrl_param_1__duty_slider = obj;
                                    lv_obj_set_pos(obj, LV_PCT(5), 9);
                                    lv_obj_set_size(obj, LV_PCT(90), 12);
                                    lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param_1__duty_slider, LV_EVENT_ALL, 0);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                                }
                                {
                                    // ctrl_param_1__duty_max_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.ctrl_param_1__duty_max_label = obj;
                                    lv_obj_set_pos(obj, -4, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "<placeholder>");
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // ctrl_param_1__duty_min_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.ctrl_param_1__duty_min_label = obj;
                                    lv_obj_set_pos(obj, 4, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "<placeholder>");
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                            }
                        }
                    }
                }
                {
                    // ctrl_param_amplitude_cnt
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ctrl_param_amplitude_cnt = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), 74);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ctrl_param__amplitude_name_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__amplitude_name_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Amplitude [%]");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__amplitude_edit_en_btn
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ctrl_param__amplitude_edit_en_btn = obj;
                            lv_obj_set_pos(obj, -4, 4);
                            lv_obj_set_size(obj, 40, 20);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__amplitude_edit_en_btn, LV_EVENT_ALL, 0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__amplitude_spinbox
                            lv_obj_t *obj = lv_spinbox_create(parent_obj);
                            objects.ctrl_param__amplitude_spinbox = obj;
                            lv_obj_set_pos(obj, -4, -4);
                            lv_obj_set_size(obj, 67, 36);
                            lv_spinbox_set_digit_format(obj, 5, 3);
                            lv_spinbox_set_range(obj, 0, 18000);
                            lv_spinbox_set_rollover(obj, false);
                            lv_spinbox_set_step(obj, 1);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__amplitude_spinbox, LV_EVENT_ALL, 0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__amplitude_slider_cnt
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.ctrl_param__amplitude_slider_cnt = obj;
                            lv_obj_set_pos(obj, 4, 20);
                            lv_obj_set_size(obj, LV_PCT(75), 49);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // ctrl_param__amplitude_slider
                                    lv_obj_t *obj = lv_slider_create(parent_obj);
                                    objects.ctrl_param__amplitude_slider = obj;
                                    lv_obj_set_pos(obj, LV_PCT(5), 9);
                                    lv_obj_set_size(obj, LV_PCT(90), 12);
                                    lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__amplitude_slider, LV_EVENT_ALL, 0);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                                }
                                {
                                    // ctrl_param__amplitude_max_val_palceholder_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.ctrl_param__amplitude_max_val_palceholder_label = obj;
                                    lv_obj_set_pos(obj, -4, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "<placeholder>");
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // ctrl_param__amplitude_min_val_palceholder_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.ctrl_param__amplitude_min_val_palceholder_label = obj;
                                    lv_obj_set_pos(obj, 4, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "<placeholder>");
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                            }
                        }
                    }
                }
                {
                    // ctrl_param_deadtime_cnt
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ctrl_param_deadtime_cnt = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), 0);
                    lv_obj_set_size(obj, LV_PCT(100), 46);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ctrl_param__deadtime_spinbox
                            lv_obj_t *obj = lv_spinbox_create(parent_obj);
                            objects.ctrl_param__deadtime_spinbox = obj;
                            lv_obj_set_pos(obj, -4, 0);
                            lv_obj_set_size(obj, 67, 36);
                            lv_spinbox_set_digit_format(obj, 4, 0);
                            lv_spinbox_set_range(obj, 0, 5000);
                            lv_spinbox_set_rollover(obj, false);
                            lv_spinbox_set_step(obj, 1);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__deadtime_spinbox, LV_EVENT_ALL, 0);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_RIGHT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__deadtime_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__deadtime_label = obj;
                            lv_obj_set_pos(obj, 4, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Dead time [ns] (0-5000)");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // ctrl_param_firing_angle_cnt
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ctrl_param_firing_angle_cnt = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), 74);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ctrl_param__firing_angle_name_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__firing_angle_name_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Firing angle [degree]");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__firing_angle_edit_en_btn
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ctrl_param__firing_angle_edit_en_btn = obj;
                            lv_obj_set_pos(obj, -4, 4);
                            lv_obj_set_size(obj, 40, 20);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__firing_angle_edit_en_btn, LV_EVENT_ALL, 0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                            lv_obj_add_state(obj, LV_STATE_FOCUSED);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__firing_angle_spinbox
                            lv_obj_t *obj = lv_spinbox_create(parent_obj);
                            objects.ctrl_param__firing_angle_spinbox = obj;
                            lv_obj_set_pos(obj, -4, -4);
                            lv_obj_set_size(obj, 67, 36);
                            lv_spinbox_set_digit_format(obj, 5, 3);
                            lv_spinbox_set_range(obj, 0, 18000);
                            lv_spinbox_set_rollover(obj, false);
                            lv_spinbox_set_step(obj, 1);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__firing_angle_spinbox, LV_EVENT_ALL, 0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__firing_angle_slider_cnt
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.ctrl_param__firing_angle_slider_cnt = obj;
                            lv_obj_set_pos(obj, 4, 20);
                            lv_obj_set_size(obj, LV_PCT(75), 49);
                            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // ctrl_param__firing_angle_slider
                                    lv_obj_t *obj = lv_slider_create(parent_obj);
                                    objects.ctrl_param__firing_angle_slider = obj;
                                    lv_obj_set_pos(obj, LV_PCT(5), 9);
                                    lv_obj_set_size(obj, LV_PCT(90), 12);
                                    lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__firing_angle_slider, LV_EVENT_ALL, 0);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                                }
                                {
                                    // ctrl_param__firing_angle_max_val_palceholder_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.ctrl_param__firing_angle_max_val_palceholder_label = obj;
                                    lv_obj_set_pos(obj, -4, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "<placeholder>");
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                                {
                                    // ctrl_param__firing_angle_min_val_palceholder_label
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.ctrl_param__firing_angle_min_val_palceholder_label = obj;
                                    lv_obj_set_pos(obj, 4, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "<placeholder>");
                                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                            }
                        }
                    }
                }
                {
                    // ctrl_param_line_freq_cnt
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ctrl_param_line_freq_cnt = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), 30);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ctrl_param_line_freq_name_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param_line_freq_name_label = obj;
                            lv_obj_set_pos(obj, 4, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Line frequency [Hz]");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // pcc__line_freq_text
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.pcc__line_freq_text = obj;
                            lv_obj_set_pos(obj, -4, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_RIGHT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // ctrl_param_pulse_len_cnt
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.ctrl_param_pulse_len_cnt = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), -19);
                    lv_obj_set_size(obj, LV_PCT(100), 74);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // ctrl_param__pulse_len_cnt_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__pulse_len_cnt_label = obj;
                            lv_obj_set_pos(obj, 4, 4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Pulse length [degree]");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__pulse_len_edit_en_btn
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.ctrl_param__pulse_len_edit_en_btn = obj;
                            lv_obj_set_pos(obj, -4, 4);
                            lv_obj_set_size(obj, 40, 20);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__pulse_len_edit_en_btn, LV_EVENT_ALL, 0);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_margin_top(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__pulse_len_spinbox
                            lv_obj_t *obj = lv_spinbox_create(parent_obj);
                            objects.ctrl_param__pulse_len_spinbox = obj;
                            lv_obj_set_pos(obj, -4, -4);
                            lv_obj_set_size(obj, 40, 36);
                            lv_spinbox_set_digit_format(obj, 2, 0);
                            lv_spinbox_set_range(obj, 0, 18000);
                            lv_spinbox_set_rollover(obj, false);
                            lv_spinbox_set_step(obj, 1);
                            lv_obj_add_event_cb(obj, event_handler_cb_topo_ctrl_parameter_scr_ctrl_param__pulse_len_spinbox, LV_EVENT_ALL, 0);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__pulse_len_min_val_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__pulse_len_min_val_label = obj;
                            lv_obj_set_pos(obj, 4, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Min:");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__pulse_len_max_val_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__pulse_len_max_val_label = obj;
                            lv_obj_set_pos(obj, 4, -4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Max:");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__pulse_len_min_val_placeholder_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__pulse_len_min_val_placeholder_label = obj;
                            lv_obj_set_pos(obj, 40, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "<placeholder>");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // ctrl_param__pulse_len_max_val_placeholder_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.ctrl_param__pulse_len_max_val_placeholder_label = obj;
                            lv_obj_set_pos(obj, 40, -4);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "<placeholder>");
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // pcc_param__firing_angle_edit_disabled_val_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pcc_param__firing_angle_edit_disabled_val_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, -4, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // pcc_param__pulse_len_edit_disabled_val_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pcc_param__pulse_len_edit_disabled_val_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, -4, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // pcc_param__sw_freq_edit_disabled_val_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pcc_param__sw_freq_edit_disabled_val_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Switching frequency: <>Hz");
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, -4, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // pcc_param__phase_shift_edit_disabled_val_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pcc_param__phase_shift_edit_disabled_val_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Phase shift: <> deg");
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, -4, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // pcc_param__mod_freq_edit_disabled_val_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pcc_param__mod_freq_edit_disabled_val_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Modulation frequency: <>Hz");
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, -4, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // pcc_param__duty_cycle_edit_disabled_val_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pcc_param__duty_cycle_edit_disabled_val_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Duty cycle: <>%");
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, -4, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // pcc_param__amplitude_edit_disabled_val_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pcc_param__amplitude_edit_disabled_val_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Amplitude: <>%");
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, -4, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // pcc_param__deadtime_edit_disabled_val_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pcc_param__deadtime_edit_disabled_val_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Dead time: <> ns");
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_margin_top(obj, -4, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    
    tick_screen_topo_ctrl_parameter_scr();
}

void tick_screen_topo_ctrl_parameter_scr() {
    {
        bool new_val = get_var_pcc_param_sw_freq_edit_en();
        bool cur_val = lv_obj_has_state(objects.ctrl_param__sw_freq_edit_en_btn, LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__sw_freq_edit_en_btn;
            if (new_val) lv_obj_add_state(objects.ctrl_param__sw_freq_edit_en_btn, LV_STATE_CHECKED);
            else lv_obj_clear_state(objects.ctrl_param__sw_freq_edit_en_btn, LV_STATE_CHECKED);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_sw_freq_i32();
        int32_t cur_val = lv_spinbox_get_value(objects.ctrl_param__sw_freq_spinbox);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__sw_freq_spinbox;
            lv_spinbox_set_value(objects.ctrl_param__sw_freq_spinbox, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_pcc_param_phase_shift_edit_en();
        bool cur_val = lv_obj_has_state(objects.ctrl_param__phase_shift_edit_en_btn, LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__phase_shift_edit_en_btn;
            if (new_val) lv_obj_add_state(objects.ctrl_param__phase_shift_edit_en_btn, LV_STATE_CHECKED);
            else lv_obj_clear_state(objects.ctrl_param__phase_shift_edit_en_btn, LV_STATE_CHECKED);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_phase_shift_i32();
        int32_t cur_val = lv_spinbox_get_value(objects.ctrl_param__phase_shift_spinbox);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__phase_shift_spinbox;
            lv_spinbox_set_value(objects.ctrl_param__phase_shift_spinbox, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_phase_shift_f32();
        int32_t cur_val = lv_slider_get_value(objects.ctrl_param__phase_shift_slider);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__phase_shift_slider;
            lv_slider_set_value(objects.ctrl_param__phase_shift_slider, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_pcc_param_mod_freq_edit_en();
        bool cur_val = lv_obj_has_state(objects.ctrl_param__mod_freq_edit_en_btn, LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__mod_freq_edit_en_btn;
            if (new_val) lv_obj_add_state(objects.ctrl_param__mod_freq_edit_en_btn, LV_STATE_CHECKED);
            else lv_obj_clear_state(objects.ctrl_param__mod_freq_edit_en_btn, LV_STATE_CHECKED);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_mod_freq_i32();
        int32_t cur_val = lv_spinbox_get_value(objects.ctrl_param__mod_freq_spinbox);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__mod_freq_spinbox;
            lv_spinbox_set_value(objects.ctrl_param__mod_freq_spinbox, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_mod_freq_f32();
        int32_t cur_val = lv_slider_get_value(objects.ctrl_param__mod_freq_slider);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__mod_freq_slider;
            lv_slider_set_value(objects.ctrl_param__mod_freq_slider, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_pcc_param_duty_edit_en();
        bool cur_val = lv_obj_has_state(objects.ctrl_param_1__duty_edit_en_btn, LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param_1__duty_edit_en_btn;
            if (new_val) lv_obj_add_state(objects.ctrl_param_1__duty_edit_en_btn, LV_STATE_CHECKED);
            else lv_obj_clear_state(objects.ctrl_param_1__duty_edit_en_btn, LV_STATE_CHECKED);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_duty_cycle_i32();
        int32_t cur_val = lv_spinbox_get_value(objects.ctrl_param_1__duty_spinbox);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param_1__duty_spinbox;
            lv_spinbox_set_value(objects.ctrl_param_1__duty_spinbox, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_duty_cycle_f32();
        int32_t cur_val = lv_slider_get_value(objects.ctrl_param_1__duty_slider);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param_1__duty_slider;
            lv_slider_set_value(objects.ctrl_param_1__duty_slider, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_pcc_param_amplitude_edit_en();
        bool cur_val = lv_obj_has_state(objects.ctrl_param__amplitude_edit_en_btn, LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__amplitude_edit_en_btn;
            if (new_val) lv_obj_add_state(objects.ctrl_param__amplitude_edit_en_btn, LV_STATE_CHECKED);
            else lv_obj_clear_state(objects.ctrl_param__amplitude_edit_en_btn, LV_STATE_CHECKED);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_amplitude_i32();
        int32_t cur_val = lv_spinbox_get_value(objects.ctrl_param__amplitude_spinbox);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__amplitude_spinbox;
            lv_spinbox_set_value(objects.ctrl_param__amplitude_spinbox, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_amplitude_f32();
        int32_t cur_val = lv_slider_get_value(objects.ctrl_param__amplitude_slider);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__amplitude_slider;
            lv_slider_set_value(objects.ctrl_param__amplitude_slider, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_deadtime_i32();
        int32_t cur_val = lv_spinbox_get_value(objects.ctrl_param__deadtime_spinbox);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__deadtime_spinbox;
            lv_spinbox_set_value(objects.ctrl_param__deadtime_spinbox, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_pcc_param_firing_angle_edit_en();
        bool cur_val = lv_obj_has_state(objects.ctrl_param__firing_angle_edit_en_btn, LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__firing_angle_edit_en_btn;
            if (new_val) lv_obj_add_state(objects.ctrl_param__firing_angle_edit_en_btn, LV_STATE_CHECKED);
            else lv_obj_clear_state(objects.ctrl_param__firing_angle_edit_en_btn, LV_STATE_CHECKED);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_firing_angle_i32();
        int32_t cur_val = lv_spinbox_get_value(objects.ctrl_param__firing_angle_spinbox);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__firing_angle_spinbox;
            lv_spinbox_set_value(objects.ctrl_param__firing_angle_spinbox, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_firing_angle_f32();
        int32_t cur_val = lv_slider_get_value(objects.ctrl_param__firing_angle_slider);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__firing_angle_slider;
            lv_slider_set_value(objects.ctrl_param__firing_angle_slider, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_pcc_param_line_freq_str();
        const char *cur_val = lv_label_get_text(objects.pcc__line_freq_text);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.pcc__line_freq_text;
            lv_label_set_text(objects.pcc__line_freq_text, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_pcc_param_pulse_len_edit_en();
        bool cur_val = lv_obj_has_state(objects.ctrl_param__pulse_len_edit_en_btn, LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__pulse_len_edit_en_btn;
            if (new_val) lv_obj_add_state(objects.ctrl_param__pulse_len_edit_en_btn, LV_STATE_CHECKED);
            else lv_obj_clear_state(objects.ctrl_param__pulse_len_edit_en_btn, LV_STATE_CHECKED);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_pcc_param_pulse_len_i32();
        int32_t cur_val = lv_spinbox_get_value(objects.ctrl_param__pulse_len_spinbox);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.ctrl_param__pulse_len_spinbox;
            lv_spinbox_set_value(objects.ctrl_param__pulse_len_spinbox, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_settings_screen_loaded, LV_EVENT_SCREEN_LOADED, (void *)0);
    lv_obj_add_event_cb(obj, action_settings_screen_unloaded, LV_EVENT_SCREEN_UNLOADED, (void *)0);
    lv_obj_add_event_cb(obj, event_handler_cb_settings_settings, LV_EVENT_ALL, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2a2a2a), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, LV_PCT(1), 27);
            lv_obj_set_size(obj, LV_PCT(98), 52);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // param_cnt_label_4
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.param_cnt_label_4 = obj;
                    lv_obj_set_pos(obj, 4, 4);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Display brightness");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // brightness_slider
                    lv_obj_t *obj = lv_slider_create(parent_obj);
                    objects.brightness_slider = obj;
                    lv_obj_set_pos(obj, LV_PCT(5), 28);
                    lv_obj_set_size(obj, LV_PCT(90), 12);
                    lv_slider_set_range(obj, 10, 100);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_brightness_slider, LV_EVENT_ALL, 0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffa8a8a8), LV_PART_KNOB | LV_STATE_DISABLED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffa8a8a8), LV_PART_INDICATOR | LV_STATE_DISABLED);
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), 25);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_label_set_text(obj, "Settings");
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff4c4c4c), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // sw_info_container
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.sw_info_container = obj;
            lv_obj_set_pos(obj, LV_PCT(1), 79);
            lv_obj_set_size(obj, LV_PCT(98), 161);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // device_info
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.device_info = obj;
                    lv_obj_set_pos(obj, 112, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Device info");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // sw_version_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.sw_version_label = obj;
                    lv_obj_set_pos(obj, 4, 22);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "SW version:");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // hw_revision_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.hw_revision_label = obj;
                    lv_obj_set_pos(obj, 4, 63);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "HW revision:");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // sw_version_str_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.sw_version_str_label = obj;
                    lv_obj_set_pos(obj, 17, 40);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // hw_revision_str_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.hw_revision_str_label = obj;
                    lv_obj_set_pos(obj, 17, 81);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    
    tick_screen_settings();
}

void tick_screen_settings() {
    {
        int32_t new_val = get_var_brightness__per_cent__f32();
        int32_t cur_val = lv_slider_get_value(objects.brightness_slider);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.brightness_slider;
            lv_slider_set_value(objects.brightness_slider, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
}


void ui_create_groups() {
    if (!groups_created) {
        groups.MainGroup = lv_group_create();
        groups.param_selector = lv_group_create();
        groups.pcc_topology_select_grp = lv_group_create();
        groups.settings_group = lv_group_create();
        groups_created = true;
    }
}


typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_power_topology_menu,
    tick_screen_topo_ctrl_parameter_scr,
    tick_screen_settings,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    ui_create_groups();
    
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_power_topology_menu();
    create_screen_topo_ctrl_parameter_scr();
    create_screen_settings();
}
