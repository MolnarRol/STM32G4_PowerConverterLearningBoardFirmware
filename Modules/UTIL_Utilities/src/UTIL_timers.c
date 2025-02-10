/*
 * UTIL_timers.c
 *
 *  Created on: Dec 15, 2024
 *      Author: molnar
 */
#include <UTIL_timers.h>

UTIL_Status_enum UTIL_TIM_SetTimerOverflowFrequency_v(f32 input_freq__Hz__f32, f32 desired_ovf_freq__Hz__f32, volatile u16* arr_reg_pu16, volatile u16* psc_reg_pu16)
{
	assert_param( (input_freq__Hz__f32 > 0.0f) && (desired_ovf_freq__Hz__f32 > 0.0f) );
	assert_param( (arr_reg_pu16 != NULL) && (psc_reg_pu16 != NULL) );

	const f32 inv_16bit_max_f32 	= 1.525878906e-5f;												/* 1 / 2ˆ16. */
	const f32 input_div_target_f32 	= input_freq__Hz__f32 / desired_ovf_freq__Hz__f32;
	f32 calculated_psc_f32			= input_div_target_f32 * inv_16bit_max_f32;
	assert_param(calculated_psc_f32 <= (f32)U16_MAX);

	*psc_reg_pu16					= (u16)calculated_psc_f32;
	*arr_reg_pu16					= (u16)(input_div_target_f32 / (f32)(*psc_reg_pu16 + (u16)1));
	return UTIL_OK_e;
}

