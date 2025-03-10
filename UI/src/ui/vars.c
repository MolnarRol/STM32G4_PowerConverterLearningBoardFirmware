/*
 * vars.c
 *
 *  Created on: Feb 28, 2025
 *      Author: molnar
 */
#include "vars.h"
#include <app_types.h>
#include "backlight_ctrl.h"

float brightness__per_cent__f32 = 50.0f;

float get_var_brightness__per_cent__f32() {
    return brightness__per_cent__f32;
}

void set_var_brightness__per_cent__f32(float value) {
    brightness__per_cent__f32 = value;
    LCD_BacklightSetBrightness_v(brightness__per_cent__f32);
}

/*
 * PCC params
 */
float* pcc_param_duty_cycle_pf32    = NULL;
float* pcc_param_sw_freq_pf32       = NULL;
float* pcc_param_deadtime_pf32      = NULL;
float* pcc_param_phase_shift_pf32   = NULL;
float* pcc_param_mod_freq_pf32      = NULL;
float* pcc_param_amplitude_pf32     = NULL;
float* pcc_param_firing_angle_pf32  = NULL;
float* pcc_param_pulse_len_pf32     = NULL;

float get_var_pcc_param_duty_cycle_f32() {
    if(pcc_param_duty_cycle_pf32)
        return *pcc_param_duty_cycle_pf32;
   return 0.0f;
}

void set_var_pcc_param_duty_cycle_f32(float value) {
    if(pcc_param_duty_cycle_pf32)
        *pcc_param_duty_cycle_pf32 = value;
}

int32_t get_var_pcc_param_duty_cycle_i32() {
    if(pcc_param_duty_cycle_pf32)
        return (int32_t)(*pcc_param_duty_cycle_pf32 * 100.0f);
    return 0;
}

void set_var_pcc_param_duty_cycle_i32(int32_t value) {
    if(pcc_param_duty_cycle_pf32)
        *pcc_param_duty_cycle_pf32 = (float)value / 100.0f;
}

int32_t get_var_pcc_param_sw_freq_i32() {
    if(pcc_param_sw_freq_pf32)
        return *pcc_param_sw_freq_pf32 * 100.0f;
    return 0.0f;
}

void set_var_pcc_param_sw_freq_i32(int32_t value) {
    if(pcc_param_sw_freq_pf32)
        *pcc_param_sw_freq_pf32 = (float)value / 100.0f;
}

int32_t get_var_pcc_param_deadtime_i32() {
    if(pcc_param_deadtime_pf32)
        return (int32_t)(*pcc_param_deadtime_pf32 * 1.0e9f);

    return 0;
}

void set_var_pcc_param_deadtime_i32(int32_t value) {
    if(pcc_param_deadtime_pf32)
        *pcc_param_deadtime_pf32 = (float)value * 1.0e-9f;
}

float get_var_pcc_param_phase_shift_f32() {
   if(pcc_param_phase_shift_pf32)
        return *pcc_param_phase_shift_pf32;
   return 0.0f;
}

void set_var_pcc_param_phase_shift_f32(float value) {
    if(pcc_param_phase_shift_pf32)
        *pcc_param_phase_shift_pf32 = value;
}

int32_t get_var_pcc_param_phase_shift_i32() {
    if(pcc_param_phase_shift_pf32)
         return (int32_t)(*pcc_param_phase_shift_pf32 * 100.0f);
    return 0;
}

void set_var_pcc_param_phase_shift_i32(int32_t value) {
    if(pcc_param_phase_shift_pf32)
        *pcc_param_phase_shift_pf32 = (float)value / 100.0f;
}

float get_var_pcc_param_mod_freq_f32() {
    if(pcc_param_mod_freq_pf32)
         return *pcc_param_mod_freq_pf32;
    return 0.0f;
}

void set_var_pcc_param_mod_freq_f32(float value) {
    if(pcc_param_mod_freq_pf32)
        *pcc_param_mod_freq_pf32 = value;
}

int32_t get_var_pcc_param_mod_freq_i32() {
    if(pcc_param_mod_freq_pf32)
         return (int32_t)(*pcc_param_mod_freq_pf32 * 100.0f);
    return 0;
}

void set_var_pcc_param_mod_freq_i32(int32_t value) {
    if(pcc_param_mod_freq_pf32)
        *pcc_param_mod_freq_pf32 = (float)value / 100.0f;
}

float get_var_pcc_param_amplitude_f32() {
    if(pcc_param_amplitude_pf32)
         return *pcc_param_amplitude_pf32;
    return 0.0f;
}

void set_var_pcc_param_amplitude_f32(float value) {
    if(pcc_param_amplitude_pf32)
        *pcc_param_amplitude_pf32 = value;
}

int32_t get_var_pcc_param_amplitude_i32() {
    if(pcc_param_amplitude_pf32)
         return (int32_t)(*pcc_param_amplitude_pf32 * 100.0f);
    return 0;
}

void set_var_pcc_param_amplitude_i32(int32_t value) {
    if(pcc_param_amplitude_pf32)
        *pcc_param_amplitude_pf32 = (float)value / 100.0f;
}

int32_t get_var_pcc_param_pulse_len_i32() {
    if(pcc_param_pulse_len_pf32)
        return (int32_t)*pcc_param_pulse_len_pf32;
    return 0;
}

void set_var_pcc_param_pulse_len_i32(int32_t value) {
    if(pcc_param_pulse_len_pf32)
        *pcc_param_pulse_len_pf32 = (float)value;
}

float get_var_pcc_param_firing_angle_f32() {
    if(pcc_param_firing_angle_pf32)
        return *pcc_param_firing_angle_pf32;
    return 0.0f;
}

void set_var_pcc_param_firing_angle_f32(float value) {
    if(pcc_param_firing_angle_pf32)
        *pcc_param_firing_angle_pf32 = (float)value;
}

int32_t get_var_pcc_param_firing_angle_i32() {
    if(pcc_param_firing_angle_pf32)
        return (int32_t)(*pcc_param_firing_angle_pf32 * 100.0f);
    return 0;
}

void set_var_pcc_param_firing_angle_i32(int32_t value) {
    if(pcc_param_firing_angle_pf32)
        *pcc_param_firing_angle_pf32 = (float)value / 100.0f;
}

extern volatile f32 PCC_LC_ZC_LineFreq__Hz__f32;
char pcc_param_line_freq_str[100] = { 0 };

const char *get_var_pcc_param_line_freq_str() {
    snprintf(pcc_param_line_freq_str, 1000, "%.2f", PCC_LC_ZC_LineFreq__Hz__f32);
    return pcc_param_line_freq_str;
}

void set_var_pcc_param_line_freq_str(const char *value) {

}

/*
 * PCC edit params enable.
 */
bool pcc_param_duty_edit_en;

bool get_var_pcc_param_duty_edit_en() {
    return pcc_param_duty_edit_en;
}

void set_var_pcc_param_duty_edit_en(bool value) {
    pcc_param_duty_edit_en = value;
}

bool pcc_param_sw_freq_edit_en;

bool get_var_pcc_param_sw_freq_edit_en() {
    return pcc_param_sw_freq_edit_en;
}

void set_var_pcc_param_sw_freq_edit_en(bool value) {
    pcc_param_sw_freq_edit_en = value;
}

bool pcc_param_mod_freq_edit_en;

bool get_var_pcc_param_mod_freq_edit_en() {
    return pcc_param_mod_freq_edit_en;
}

void set_var_pcc_param_mod_freq_edit_en(bool value) {
    pcc_param_mod_freq_edit_en = value;
}

bool pcc_param_amplitude_edit_en;

bool get_var_pcc_param_amplitude_edit_en() {
    return pcc_param_amplitude_edit_en;
}

void set_var_pcc_param_amplitude_edit_en(bool value) {
    pcc_param_amplitude_edit_en = value;
}

bool pcc_param_phase_shift_edit_en;

bool get_var_pcc_param_phase_shift_edit_en() {
    return pcc_param_phase_shift_edit_en;
}

void set_var_pcc_param_phase_shift_edit_en(bool value) {
    pcc_param_phase_shift_edit_en = value;
}

bool pcc_param_pulse_len_edit_en;

bool get_var_pcc_param_pulse_len_edit_en() {
    return pcc_param_pulse_len_edit_en;
}

void set_var_pcc_param_pulse_len_edit_en(bool value) {
    pcc_param_pulse_len_edit_en = value;
}

bool pcc_param_firing_angle_edit_en;

bool get_var_pcc_param_firing_angle_edit_en() {
    return pcc_param_firing_angle_edit_en;
}

void set_var_pcc_param_firing_angle_edit_en(bool value) {
    pcc_param_firing_angle_edit_en = value;
}
