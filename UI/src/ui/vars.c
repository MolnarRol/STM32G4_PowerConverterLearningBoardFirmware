/*
 * vars.c
 *
 *  Created on: Feb 28, 2025
 *      Author: molnar
 */
#include "vars.h"
#include <app_types.h>

float slider_val;

float get_var_slider_val() {
    return slider_val;
}

void set_var_slider_val(float value) {
    slider_val = value;
}

bool simple_pwm_freq_edit_enable_b = true;

bool get_var_simple_pwm_freq_edit_enable_b() {
    return simple_pwm_freq_edit_enable_b;
}

void set_var_simple_pwm_freq_edit_enable_b(bool value) {
    simple_pwm_freq_edit_enable_b = value;
}

bool simple_pwm_freq_freq_edit_enable_flag_b = false;

bool get_var_simple_pwm_freq_freq_edit_enable_flag_b() {
    simple_pwm_freq_freq_edit_enable_flag_b = !simple_pwm_freq_edit_enable_b;
    return simple_pwm_freq_freq_edit_enable_flag_b;
}

void set_var_simple_pwm_freq_freq_edit_enable_flag_b(bool value) {
    simple_pwm_freq_freq_edit_enable_flag_b = value;
}

char status_var_str[100] = { 0 };

const char *get_var_status_var_str() {
    return status_var_str;
}

void set_var_status_var_str(const char *value) {
    strncpy(status_var_str, value, sizeof(status_var_str) / sizeof(char));
    status_var_str[sizeof(status_var_str) / sizeof(char) - 1] = 0;
}
