/*
 * vars.c
 *
 *  Created on: Feb 28, 2025
 *      Author: molnar
 */
#include "vars.h"
#include <app_types.h>

float slider_val;
bool simple_pwm_freq_edit_enable_b = true;
bool simple_pwm_freq_freq_edit_enable_flag_b = false;
char status_var_str[100] = { 0 };
bool simple_pwm_duty_edit_enable_b;
bool simple_pwm_duty_edit_enable_flag_b;

/*
 * PCC topology control variables.
 */
float pcc_param_freq_val_f32;

float get_var_slider_val() {
    return slider_val;
}

void set_var_slider_val(float value) {
    slider_val = value;
}

bool get_var_simple_pwm_freq_edit_enable_b() {
    return simple_pwm_freq_edit_enable_b;
}

void set_var_simple_pwm_freq_edit_enable_b(bool value) {
    simple_pwm_freq_edit_enable_b = value;
}

bool get_var_simple_pwm_freq_freq_edit_enable_flag_b() {
    simple_pwm_freq_freq_edit_enable_flag_b = !simple_pwm_freq_edit_enable_b;
    return simple_pwm_freq_freq_edit_enable_flag_b;
}

void set_var_simple_pwm_freq_freq_edit_enable_flag_b(bool value) {
    simple_pwm_freq_freq_edit_enable_flag_b = value;
}

const char *get_var_status_var_str() {
    return status_var_str;
}

void set_var_status_var_str(const char *value) {
    strncpy(status_var_str, value, sizeof(status_var_str) / sizeof(char));
    status_var_str[sizeof(status_var_str) / sizeof(char) - 1] = 0;
}

bool get_var_simple_pwm_duty_edit_enable_b() {
    return simple_pwm_duty_edit_enable_b;
}

void set_var_simple_pwm_duty_edit_enable_b(bool value) {
    simple_pwm_duty_edit_enable_b = value;
}

bool get_var_simple_pwm_duty_edit_enable_flag_b() {
    simple_pwm_duty_edit_enable_flag_b = !simple_pwm_duty_edit_enable_b;
    return simple_pwm_duty_edit_enable_flag_b;
}

void set_var_simple_pwm_duty_edit_enable_flag_b(bool value) {
    simple_pwm_duty_edit_enable_flag_b = value;
}

float get_var_pcc_param_freq_val_f32() {
    return pcc_param_freq_val_f32;
}

void set_var_pcc_param_freq_val_f32(float value) {
    pcc_param_freq_val_f32 = value;
}
