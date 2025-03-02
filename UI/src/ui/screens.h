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
} groups_t;

extern groups_t groups;

void ui_create_groups();

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *power_topology_menu;
    lv_obj_t *simple_pwm;
    lv_obj_t *obj0;
    lv_obj_t *settings_btn;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *freq_editable_btn;
    lv_obj_t *param_slider;
    lv_obj_t *param_spinbox;
    lv_obj_t *freq_editable_btn_1;
    lv_obj_t *param_slider_1;
    lv_obj_t *param_spinbox_1;
    lv_obj_t *obj5;
    lv_obj_t *power_topology_control_btn;
    lv_obj_t *obj6;
    lv_obj_t *label_topology_name;
    lv_obj_t *param_cnt_freq;
    lv_obj_t *param_cnt_label;
    lv_obj_t *param_max;
    lv_obj_t *param_min;
    lv_obj_t *param_cnt_duty;
    lv_obj_t *param_cnt_label_1;
    lv_obj_t *param_max_1;
    lv_obj_t *param_min_1;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_POWER_TOPOLOGY_MENU = 2,
    SCREEN_ID_SIMPLE_PWM = 3,
};

void create_screen_main();
void tick_screen_main();

void create_screen_power_topology_menu();
void tick_screen_power_topology_menu();

void create_screen_simple_pwm();
void tick_screen_simple_pwm();

void create_user_widget_status_bar(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_status_bar(int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/