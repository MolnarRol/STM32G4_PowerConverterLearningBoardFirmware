/*
 * backlight_ctrl.c
 *
 *  Created on: Mar 4, 2025
 *      Author: molnar
 */
#include <app_types.h>
#include <stm32g4xx.h>
#include <UTIL_public_interface.h>
#include <general_config.h>

/* PB8 [AF1] - TIM16_CH1 */
void LCD_BacklightInit_v(void)
{
    MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODE8_Msk, (2UL << GPIO_MODER_MODE8_Pos));
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT8_Msk);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD8_Msk);
    MODIFY_REG(GPIOB->AFR[1], GPIO_AFRH_AFSEL8_Msk, (1UL << GPIO_AFRH_AFSEL8_Pos));

    SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM16RST);
    CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM16RST);
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM16EN);

    UTIL_TIM_SetTimerOverflowFrequency_v(SYS_APB2_CLOCK_FREQ__Hz__d, 5000.0f, &TIM16->ARR, &TIM16->PSC);

    SET_BIT(TIM16->CCMR1, TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    SET_BIT(TIM16->CCER, TIM_CCER_CC1E);
}

void LCD_BacklightEnable_v(void)
{
    SET_BIT(TIM16->BDTR, TIM_BDTR_MOE);
    SET_BIT(TIM16->CR1, TIM_CR1_CEN);
}

void LCD_BacklightDisable_v(void)
{
    SET_BIT(TIM16->CR1, TIM_CR1_CEN);
    SET_BIT(TIM16->BDTR, TIM_BDTR_MOE);
}

void LCD_BacklightSetBrightness_v(f32 brightness__per_cent__f32)
{
    TIM16->CCR1 = (u16)UTIL_MapFloatToRange_f32(0.0f, (f32)TIM16->ARR, 0.0f, 100.0f, brightness__per_cent__f32);
}
