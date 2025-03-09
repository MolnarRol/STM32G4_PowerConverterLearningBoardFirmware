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
    FLOW_GLOBAL_VARIABLE_BRIGHTNESS__PER_CENT__F32 = 0,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_DUTY_CYCLE_F32 = 1,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_DUTY_CYCLE_I32 = 2,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_SW_FREQ_I32 = 3,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_DUTY_EDIT_EN = 4,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_SW_FREQ_EDIT_EN = 5,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_MOD_FREQ_EDIT_EN = 6,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_AMPLITUDE_EDIT_EN = 7,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_PHASE_SHIFT_EDIT_EN = 8,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_DEADTIME_I32 = 9,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_PHASE_SHIFT_I32 = 10,
    FLOW_GLOBAL_VARIABLE_PCC_PARAM_PHASE_SHIFT_F32 = 11
};

// Native global variables

extern float get_var_brightness__per_cent__f32();
extern void set_var_brightness__per_cent__f32(float value);
extern float get_var_pcc_param_duty_cycle_f32();
extern void set_var_pcc_param_duty_cycle_f32(float value);
extern int32_t get_var_pcc_param_duty_cycle_i32();
extern void set_var_pcc_param_duty_cycle_i32(int32_t value);
extern int32_t get_var_pcc_param_sw_freq_i32();
extern void set_var_pcc_param_sw_freq_i32(int32_t value);
extern bool get_var_pcc_param_duty_edit_en();
extern void set_var_pcc_param_duty_edit_en(bool value);
extern bool get_var_pcc_param_sw_freq_edit_en();
extern void set_var_pcc_param_sw_freq_edit_en(bool value);
extern bool get_var_pcc_param_mod_freq_edit_en();
extern void set_var_pcc_param_mod_freq_edit_en(bool value);
extern bool get_var_pcc_param_amplitude_edit_en();
extern void set_var_pcc_param_amplitude_edit_en(bool value);
extern bool get_var_pcc_param_phase_shift_edit_en();
extern void set_var_pcc_param_phase_shift_edit_en(bool value);
extern int32_t get_var_pcc_param_deadtime_i32();
extern void set_var_pcc_param_deadtime_i32(int32_t value);
extern int32_t get_var_pcc_param_phase_shift_i32();
extern void set_var_pcc_param_phase_shift_i32(int32_t value);
extern float get_var_pcc_param_phase_shift_f32();
extern void set_var_pcc_param_phase_shift_f32(float value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/