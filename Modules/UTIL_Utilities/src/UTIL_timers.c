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

u8 UTIL_TIM_SetMinumumDeadTimeValue_u8(f32 input_freq__Hz__f32, f32 minimum_deadtime__s__f32)
{
    const f32 t_dts_f32 = 1.0f / input_freq__Hz__f32;

    u8 return_deadtime_u8;

    if(minimum_deadtime__s__f32 < (127.0f * t_dts_f32))
    {
        return_deadtime_u8 = (u8)(minimum_deadtime__s__f32 / t_dts_f32);
    }
    else if (minimum_deadtime__s__f32 < (127.0f * 2.0f * t_dts_f32))
    {
        return_deadtime_u8  = (u8)((minimum_deadtime__s__f32 / (2.0f * t_dts_f32)) - 64.0f);
        return_deadtime_u8  |=  0b100 << 5;
    }
    else if (minimum_deadtime__s__f32 < (63.0f * 8.0f * t_dts_f32))
    {
        return_deadtime_u8  = (u8)((minimum_deadtime__s__f32 / (8.0f * t_dts_f32)) - 32.0f);
        return_deadtime_u8  |=  0b110 << 5;
    }
    else
    {
        return_deadtime_u8  = (u8)((minimum_deadtime__s__f32 / (16.0f * t_dts_f32)) - 32.0f);
        return_deadtime_u8  |=  0b111 << 5;
    }

    return return_deadtime_u8;
}

f32 UTIL_MapFloatToRange_f32(f32 map_intetval_low_f32, f32 map_interval_high_f32, f32 input_interval_low_f32, f32 input_interval_high_f32, f32 input_val_f32)
{
    if(input_val_f32 > input_interval_high_f32)     input_val_f32 = input_interval_high_f32;
    else if(input_val_f32 < input_interval_low_f32) input_val_f32 = input_interval_low_f32;

    return  (input_val_f32 - input_interval_low_f32) * (map_interval_high_f32 - map_intetval_low_f32) /
            (input_interval_high_f32 - input_interval_low_f32) +
            map_intetval_low_f32;
}
