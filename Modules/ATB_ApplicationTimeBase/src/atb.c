/*
 * ATB.c
 *
 *  Created on: Dec 15, 2024
 *      Author: molnar
 */
#include <atb.h>
#include "stm32g4xx_ll_gpio.h"

static u32 s_ATB_TimerOverflowCounter_u32 = (u32)0;

void ATB_Init_v(void)
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;							/* Enable clocks for basic timer 6. */

	TIM6->PSC = (u16)(SYS_CPU_CLOCK_FREQ__Hz__d / 1000000UL);
	TIM6->ARR = (u16)ATB_RELOAD_VALUE_d;
	TIM6->CNT = (u16)0;
	TIM6->DIER |= TIM_DIER_UIE;										/* Enable timer update/overflow interrupt. */
	NVIC_ClearPendingIRQ(TIM6_DAC_IRQn);
	NVIC_SetPriority(TIM6_DAC_IRQn, 15);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	TIM6->CR1 |= TIM_CR1_CEN;										/* Enable timer. */
}

RAM_FUNC void TIM6_DAC_IRQHandler(void)
{
	s_ATB_TimerOverflowCounter_u32 += (u32)1;
	TIM6->SR &= ~(TIM_SR_UIF);										/* Clear interrupt flag. */
	LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_8);
}

RAM_FUNC u32 ATB_GetCurrentTicks_u32(void)
{
	volatile u32 current_systick_val_u32;
	current_systick_val_u32 = TIM6->CNT;
	return (u32)s_ATB_TimerOverflowCounter_u32 * (u32)ATB_RELOAD_VALUE_d + current_systick_val_u32;
}

RAM_FUNC boolean ATB_CheckIfPeriodHasElapsed_b(u32* reference_tick_mark_pu32, const u32 tick_delta_u32)
{
	boolean return_value_b = False_b;
	const u32 current_tick_mark_u32 = ATB_GetCurrentTicks_u32();
	const u32 delta_tick_marks_u32 = current_tick_mark_u32 - *reference_tick_mark_pu32;

	if(delta_tick_marks_u32 >= tick_delta_u32)
	{
		*reference_tick_mark_pu32 = current_tick_mark_u32;
		return_value_b = True_b;
	}
	return (return_value_b);
}
