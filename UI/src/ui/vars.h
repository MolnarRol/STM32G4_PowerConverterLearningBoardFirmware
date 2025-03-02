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
    FLOW_GLOBAL_VARIABLE_TOPOLOGY_CTRL_NAME = 1
};

// Native global variables

extern float get_var_slider_val();
extern void set_var_slider_val(float value);
extern const char *get_var_topology_ctrl_name();
extern void set_var_topology_ctrl_name(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/