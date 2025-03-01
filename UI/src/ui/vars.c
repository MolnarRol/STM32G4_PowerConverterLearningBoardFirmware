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


