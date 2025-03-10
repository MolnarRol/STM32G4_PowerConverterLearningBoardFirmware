#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _groups_t {
    lv_group_t *MainGroup;
    lv_group_t *PCC_topo_select_group;
    lv_group_t *pcc_simple_pwm_mode;
    lv_group_t *param_selector;
    lv_group_t *param_selector_sw_freq;
    lv_group_t *param_selector_duty;
    lv_group_t *pcc_topology_select_grp;
    lv_group_t *settings_group;
} groups_t;

extern groups_t groups;

void ui_create_groups();

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *power_topology_menu;
    lv_obj_t *topo_ctrl_parameter_scr;
    lv_obj_t *settings;
    lv_obj_t *obj0;
    lv_obj_t *settings_btn;
    lv_obj_t *ctrl_param__sw_freq_edit_en_btn;
    lv_obj_t *ctrl_param__sw_freq_spinbox;
    lv_obj_t *ctrl_param__phase_shift_edit_en_btn;
    lv_obj_t *ctrl_param__phase_shift_spinbox;
    lv_obj_t *ctrl_param__phase_shift_slider;
    lv_obj_t *ctrl_param__mod_freq_edit_en_btn;
    lv_obj_t *ctrl_param__mod_freq_spinbox;
    lv_obj_t *ctrl_param__mod_freq_slider;
    lv_obj_t *ctrl_param_1__duty_edit_en_btn;
    lv_obj_t *ctrl_param_1__duty_spinbox;
    lv_obj_t *ctrl_param_1__duty_slider;
    lv_obj_t *ctrl_param__amplitude_edit_en_btn;
    lv_obj_t *ctrl_param__amplitude_spinbox;
    lv_obj_t *ctrl_param__amplitude_slider;
    lv_obj_t *ctrl_param__deadtime_spinbox;
    lv_obj_t *ctrl_param__firing_angle_edit_en_btn;
    lv_obj_t *ctrl_param__firing_angle_spinbox;
    lv_obj_t *ctrl_param__firing_angle_slider;
    lv_obj_t *ctrl_param__pulse_len_edit_en_btn;
    lv_obj_t *ctrl_param__pulse_len_spinbox;
    lv_obj_t *brightness_slider;
    lv_obj_t *obj1;
    lv_obj_t *power_topology_control_btn;
    lv_obj_t *topo_name_label;
    lv_obj_t *topo_state_label;
    lv_obj_t *ctrl_param_main_cnt;
    lv_obj_t *ctrl_param_sw_freq_cnt;
    lv_obj_t *ctrl_param__sw_freq_cnt_label;
    lv_obj_t *ctrl_param__sw_freq_min_val_label;
    lv_obj_t *ctrl_param__sw_freq_max_val_label;
    lv_obj_t *ctrl_param__sw_freq_min_val_placeholder_label;
    lv_obj_t *ctrl_param__sw_freq_max_val_placeholder_label;
    lv_obj_t *ctrl_param_phase_shift_cnt;
    lv_obj_t *ctrl_param__phase_shift_name_label;
    lv_obj_t *ctrl_param__phase_shift_slider_cnt;
    lv_obj_t *ctrl_param__phase_shift_max_val_palceholder_label;
    lv_obj_t *ctrl_param__phase_shift_min_val_palceholder_label;
    lv_obj_t *ctrl_param_mod_freq_cnt;
    lv_obj_t *ctrl_param__mod_freq_name_label;
    lv_obj_t *ctrl_param__mod_freq_slider_cnt;
    lv_obj_t *ctrl_param__mod_freq_max_val_palceholder_label;
    lv_obj_t *ctrl_param__mod_freq_min_val_palceholder_label;
    lv_obj_t *ctrl_param_duty_cnt;
    lv_obj_t *ctrl_param_1__duty_name_label;
    lv_obj_t *ctrl_param_1__duty_slider_cnt;
    lv_obj_t *ctrl_param_1__duty_max_label;
    lv_obj_t *ctrl_param_1__duty_min_label;
    lv_obj_t *ctrl_param_amplitude_cnt;
    lv_obj_t *ctrl_param__amplitude_name_label;
    lv_obj_t *ctrl_param__amplitude_slider_cnt;
    lv_obj_t *ctrl_param__amplitude_max_val_palceholder_label;
    lv_obj_t *ctrl_param__amplitude_min_val_palceholder_label;
    lv_obj_t *ctrl_param_deadtime_cnt;
    lv_obj_t *ctrl_param__deadtime_label;
    lv_obj_t *ctrl_param_firing_angle_cnt;
    lv_obj_t *ctrl_param__firing_angle_name_label;
    lv_obj_t *ctrl_param__firing_angle_slider_cnt;
    lv_obj_t *ctrl_param__firing_angle_max_val_palceholder_label;
    lv_obj_t *ctrl_param__firing_angle_min_val_palceholder_label;
    lv_obj_t *ctrl_param_pulse_len_cnt;
    lv_obj_t *ctrl_param__pulse_len_cnt_label;
    lv_obj_t *ctrl_param__pulse_len_min_val_label;
    lv_obj_t *ctrl_param__pulse_len_max_val_label;
    lv_obj_t *ctrl_param__pulse_len_min_val_placeholder_label;
    lv_obj_t *ctrl_param__pulse_len_max_val_placeholder_label;
    lv_obj_t *pcc_param__sw_freq_edit_disabled_val_label;
    lv_obj_t *pcc_param__phase_shift_edit_disabled_val_label;
    lv_obj_t *pcc_param__mod_freq_edit_disabled_val_label;
    lv_obj_t *pcc_param__duty_cycle_edit_disabled_val_label;
    lv_obj_t *pcc_param__amplitude_edit_disabled_val_label;
    lv_obj_t *pcc_param__deadtime_edit_disabled_val_label;
    lv_obj_t *pcc_param__firing_angle_edit_disabled_val_label;
    lv_obj_t *pcc_param__pulse_len_edit_disabled_val_label;
    lv_obj_t *param_cnt_label_4;
    lv_obj_t *obj2;
    lv_obj_t *sw_info_container;
    lv_obj_t *device_info;
    lv_obj_t *sw_version_label;
    lv_obj_t *hw_revision_label;
    lv_obj_t *sw_version_str_label;
    lv_obj_t *hw_revision_str_label;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_POWER_TOPOLOGY_MENU = 2,
    SCREEN_ID_TOPO_CTRL_PARAMETER_SCR = 3,
    SCREEN_ID_SETTINGS = 4,
};

void create_screen_main();
void tick_screen_main();

void create_screen_power_topology_menu();
void tick_screen_power_topology_menu();

void create_screen_topo_ctrl_parameter_scr();
void tick_screen_topo_ctrl_parameter_scr();

void create_screen_settings();
void tick_screen_settings();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/