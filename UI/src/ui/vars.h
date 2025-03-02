#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_SLIDER_VAL = 0,
    FLOW_GLOBAL_VARIABLE_TOPOLOGY_CTRL_NAME = 1,
    FLOW_GLOBAL_VARIABLE_SIMPLE_PWM_FREQ_EDIT_ENABLE_B = 2,
    FLOW_GLOBAL_VARIABLE_SIMPLE_PWM_FREQ_FREQ_EDIT_ENABLE_FLAG_B = 3,
    FLOW_GLOBAL_VARIABLE_STATUS_VAR_STR = 4
};

// Native global variables

extern float get_var_slider_val();
extern void set_var_slider_val(float value);
extern const char *get_var_topology_ctrl_name();
extern void set_var_topology_ctrl_name(const char *value);
extern bool get_var_simple_pwm_freq_edit_enable_b();
extern void set_var_simple_pwm_freq_edit_enable_b(bool value);
extern bool get_var_simple_pwm_freq_freq_edit_enable_flag_b();
extern void set_var_simple_pwm_freq_freq_edit_enable_flag_b(bool value);
extern const char *get_var_status_var_str();
extern void set_var_status_var_str(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/