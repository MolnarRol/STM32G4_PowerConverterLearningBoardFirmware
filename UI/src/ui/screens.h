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
    lv_obj_t *simple_pwm;
    lv_obj_t *simple_complementary_pwm;
    lv_obj_t *sine_pwm;
    lv_obj_t *settings;
    lv_obj_t *obj0;
    lv_obj_t *settings_btn;
    lv_obj_t *freq_editable_btn;
    lv_obj_t *param_slider;
    lv_obj_t *param_spinbox;
    lv_obj_t *freq_editable_btn_1;
    lv_obj_t *param_slider_1;
    lv_obj_t *param_spinbox_1;
    lv_obj_t *param_sw_freq_edit_enable_btn;
    lv_obj_t *param_sw_freq_spinbox;
    lv_obj_t *obj1;
    lv_obj_t *freq_editable_btn_3;
    lv_obj_t *param_slider_3;
    lv_obj_t *param_spinbox_3;
    lv_obj_t *dead_time_spin_box;
    lv_obj_t *sine_pwm__amplitude_edit_en_btn;
    lv_obj_t *sine_pwm__amplitude_spinbox;
    lv_obj_t *sine_pwm__amplitude_slider_1;
    lv_obj_t *sine_pwm__mod_freq_edit_en_btn;
    lv_obj_t *sine_pwm__mod_freq_spinbox;
    lv_obj_t *sine_pwm__mod_freq_slider;
    lv_obj_t *sine_pwm__sw_freq_edit_en_btn;
    lv_obj_t *sine_pwm__freq_spinbox;
    lv_obj_t *sine_pwm__freq_unit_dropdown;
    lv_obj_t *sine_pwm__deadtime_spinbox;
    lv_obj_t *brightness_slider;
    lv_obj_t *obj2;
    lv_obj_t *power_topology_control_btn;
    lv_obj_t *simple_pwm__name;
    lv_obj_t *param_cnt_freq;
    lv_obj_t *param_cnt_label;
    lv_obj_t *param_max;
    lv_obj_t *param_min;
    lv_obj_t *param_cnt_duty;
    lv_obj_t *param_cnt_label_1;
    lv_obj_t *param_max_1;
    lv_obj_t *param_min_1;
    lv_obj_t *simple_complementary_pwm__name;
    lv_obj_t *param_sw_freq_cnt;
    lv_obj_t *minimum_sw_freq_value_label;
    lv_obj_t *minimum_sw_freq_label;
    lv_obj_t *param_cnt_label_2;
    lv_obj_t *maximum_sw_freq_label;
    lv_obj_t *maximum_sw_freq_value_label;
    lv_obj_t *param_duty_cnt;
    lv_obj_t *param_cnt_label_3;
    lv_obj_t *param_duty_slider_cnt;
    lv_obj_t *param_max_3;
    lv_obj_t *param_min_3;
    lv_obj_t *param_dead_time_cnt;
    lv_obj_t *obj3;
    lv_obj_t *sine_pwm__name;
    lv_obj_t *sine_pwm__main_cnt;
    lv_obj_t *sine_pwm__amplitude_cnt;
    lv_obj_t *sine_pwm__amplitude_cnt_label;
    lv_obj_t *sine_pwm__amplitude_slider;
    lv_obj_t *sine_pwm__amplitude_min_value;
    lv_obj_t *sine_pwm__amplitude_max_value;
    lv_obj_t *sine_pwm__mod_freq_cnt;
    lv_obj_t *sine_pwm__mod_freq_label;
    lv_obj_t *sine_pwm__mod_freq_edit_en_btn_1;
    lv_obj_t *sine_pwm__mod_freq_min_value;
    lv_obj_t *sine_pwm__mod_freq_max_value;
    lv_obj_t *sine_pwm__sw_freq_cnt;
    lv_obj_t *sine_pwm__sw_freq_cnt_label;
    lv_obj_t *sine_pwm__min_val_label;
    lv_obj_t *sine_pwm__max_val_label;
    lv_obj_t *sine_pwm__min_val_placeholder_label;
    lv_obj_t *sine_pwm__max_val_placeholder_label;
    lv_obj_t *sine_pwm__deadtime_cnt;
    lv_obj_t *sine_pwm__deadtime_label;
    lv_obj_t *sine_pwm__amplitude_edit_disabled_val_label;
    lv_obj_t *sine_pwm__mod_freq_edit_disabled_val_label;
    lv_obj_t *sine_pwm__sw_freq_edit_disabled_val_label;
    lv_obj_t *sine_pwm__deadtime_edit_disabled_val_label;
    lv_obj_t *param_cnt_label_4;
    lv_obj_t *obj4;
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
    SCREEN_ID_SIMPLE_PWM = 3,
    SCREEN_ID_SIMPLE_COMPLEMENTARY_PWM = 4,
    SCREEN_ID_SINE_PWM = 5,
    SCREEN_ID_SETTINGS = 6,
};

void create_screen_main();
void tick_screen_main();

void create_screen_power_topology_menu();
void tick_screen_power_topology_menu();

void create_screen_simple_pwm();
void tick_screen_simple_pwm();

void create_screen_simple_complementary_pwm();
void tick_screen_simple_complementary_pwm();

void create_screen_sine_pwm();
void tick_screen_sine_pwm();

void create_screen_settings();
void tick_screen_settings();

void create_user_widget_status_bar(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_status_bar(int startWidgetIndex);

void create_user_widget_param_slider_and_spinbox(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_param_slider_and_spinbox(int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/