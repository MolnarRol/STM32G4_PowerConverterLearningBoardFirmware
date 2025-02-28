/*
 * UTIL_public_interface.h
 *
 *  Created on: Dec 15, 2024
 *      Author: molnar
 */

#ifndef UTIL_UTILITIES_INTERFACE_UTIL_PUBLIC_INTERFACE_H_
#define UTIL_UTILITIES_INTERFACE_UTIL_PUBLIC_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define UTIL_TIM_UP_DOWN_COUNTER_MODE_FREQ_MULTIPLIER_df32              (f32)2.0f

typedef enum
{
	UTIL_OK_e,
	UTIL_ERR_e
} UTIL_Status_enum;

UTIL_Status_enum UTIL_TIM_SetTimerOverflowFrequency_v(f32 input_freq__Hz__f32, f32 desired_ovf_freq__Hz__f32, volatile u16* arr_reg_pu16, volatile u16* psc_reg_pu16);
u8 UTIL_TIM_SetMinumumDeadTimeValue_u8(f32 input_freq__Hz__f32, f32 minimum_deadtime__s__f32);
f32 UTIL_MapFloatToRange_f32(f32 map_intetval_low_f32, f32 map_interval_high_f32, f32 input_interval_low_f32, f32 input_interval_high_f32, f32 input_val_f32);
#ifdef __cplusplus
}
#endif
#endif /* UTIL_UTILITIES_INTERFACE_UTIL_PUBLIC_INTERFACE_H_ */
