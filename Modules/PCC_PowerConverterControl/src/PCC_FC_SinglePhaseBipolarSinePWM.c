/*
 * PCC_FC_SinglePhaseBipolarSinePWM.c
 *
 *  Created on: Feb 21, 2025
 *      Author: molnar
 */
#include "PCC_private_interface.h"
#include "UTIL_public_interface.h"

static void PCC_FC_SinglePhaseBipolarSinePWM_Init_v(void);
static void PCC_FC_SinglePhaseBipolarSinePWM_Start_v(void);
static void PCC_FC_SinglePhaseBipolarSinePWM_ActiveHandling_v(void);
static void PCC_FC_SinglePhaseBipolarSinePWM_Stop_v(void);
static void PCC_FC_SinglePhaseBipolarSinePWM_DeInit_v(void);
static void PCC_FC_SinglePhaseBipolarSinePWM_InterruptHandler_v(void);

const PCC_TopologyHandle_struct PCC_Topology_SinglePhaseBipolarSinePWM_s =
{
        .initialize_pfv     = PCC_FC_SinglePhaseBipolarSinePWM_Init_v,
        .start_pf           = PCC_FC_SinglePhaseBipolarSinePWM_Start_v,
        .active_handler_pfv = PCC_FC_SinglePhaseBipolarSinePWM_ActiveHandling_v,
        .stop_pfv           = PCC_FC_SinglePhaseBipolarSinePWM_Stop_v,
        .deinitalize_pfv    = PCC_FC_SinglePhaseBipolarSinePWM_DeInit_v,
        .isr_handler_pfv    = PCC_FC_SinglePhaseBipolarSinePWM_InterruptHandler_v,
        .driver_enable_u    =
                            {
                                .drivers_s =
                                {
                                        .gd1_f1 = 1,
                                        .gd2_f1 = 1,
                                        .gd3_f1 = 0,
                                        .gd4_f1 = 1,
                                        .gd5_f1 = 1,
                                        .gd6_f1 = 0
                                }
                            }
};

PCC_SinePWM_Parameters_s PCC_FC_SinglePhaseBipolarSinePWM_ActualParameters_s =
{
        .amplitude_f32                  = 0.0f,
        .modulation_freq__Hz__f32       = 50.0f,
        .switching_freq__Hz__f32        = 10000.0f,
};
f32 PCC_FC_SinglePhaseBipolarSinePWM_DeadTime__s__f32 = 75.0e-9f;

static f32 s_PCC_FC_SinglePhaseBipolarSinePWM_CommutationAngle__rad__f32 = 0.0f;
static f32 s_PCC_FC_SinglePhaseBipolarSinePWM_CommutationAngleStepPerTimerPeriod__rad__f32 = 0.0f;

static void PCC_FC_SinglePhaseBipolarSinePWM_Init_v(void)
{
    /* TIM1 CH1 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;                                     /* Enable clocks for TIM1 module. */

    /* GPIO pin configuration: PA8, PA10 - AF6 */
    GPIOA->MODER                &= (~GPIO_MODER_MODE7_Msk)  &
                                   (~GPIO_MODER_MODE8_Msk)  &
                                   (~GPIO_MODER_MODE10_Msk);                /* Reset pin mode. */

    GPIOA->MODER                |= (2UL << GPIO_MODER_MODE7_Pos) |
                                   (2UL << GPIO_MODER_MODE8_Pos) |
                                   (2UL << GPIO_MODER_MODE10_Pos);          /* Set mode to alternate function. */

    GPIOA->PUPDR                |= GPIO_PUPDR_PUPD7_1 |
                                   GPIO_PUPDR_PUPD8_1 |
                                   GPIO_PUPDR_PUPD10_1;                     /* Enable pull down. */

    GPIOA->OTYPER               &= (~GPIO_OTYPER_OT7_Msk) |
                                   (~GPIO_OTYPER_OT8_Msk) |
                                   (~GPIO_OTYPER_OT10_Msk);                 /* Set output type to push-pull. */

    GPIOA->OSPEEDR              &= (~GPIO_OSPEEDR_OSPEED7_Msk) &
                                   (~GPIO_OSPEEDR_OSPEED8_Msk) &
                                   (~GPIO_OSPEEDR_OSPEED10_Msk);            /* Reset pin speed. */

    GPIOA->OSPEEDR              |= (2UL << GPIO_OSPEEDR_OSPEED7_Pos) |
                                   (2UL << GPIO_OSPEEDR_OSPEED8_Pos) |
                                   (2UL << GPIO_OSPEEDR_OSPEED10_Pos);      /* Set pin speed to high. */

    GPIOA->AFR[0]               &= (~GPIO_AFRL_AFSEL7_Msk);
    GPIOA->AFR[1]               &= (~GPIO_AFRH_AFSEL8_Msk) &
                                   (~GPIO_AFRH_AFSEL10_Msk);                /* Reset alternate function. */

    GPIOA->AFR[0]               |= (6UL << GPIO_AFRL_AFSEL7_Pos);
    GPIOA->AFR[1]               |= (6UL << GPIO_AFRH_AFSEL8_Pos) |
                                   (6UL << GPIO_AFRH_AFSEL10_Pos);              /* Set alternate function to AF6. */

    /* PB01 - AF6 */
    GPIOB->MODER                &= ~GPIO_MODER_MODE1_Msk;
    GPIOB->MODER                |= (2UL << GPIO_MODER_MODE1_Pos);          /* Set mode to alternate function. */
    GPIOB->PUPDR                |= GPIO_PUPDR_PUPD1_1;                     /* Enable pull down. */
    GPIOB->OTYPER               &= (~GPIO_OTYPER_OT1_Msk);                 /* Set output type to push-pull. */
    GPIOB->OSPEEDR              &= (~GPIO_OSPEEDR_OSPEED1_Msk);            /* Reset pin speed. */
    GPIOB->OSPEEDR              |= (2UL << GPIO_OSPEEDR_OSPEED1_Pos);      /* Set pin speed to high. */
    GPIOB->AFR[0]               &= (~GPIO_AFRL_AFSEL1_Msk);
    GPIOB->AFR[0]               |= (6UL << GPIO_AFRL_AFSEL1_Pos);

    /* Reset timer 1 periphery. */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM1RST_Msk;                /* Force TIM1 peripheral reset. */
    RCC->APB2RSTR               &= ~(RCC_APB2RSTR_TIM1RST_Msk);             /* Release TIM1 peripheral reset. */
    RCC->APB2ENR                |= RCC_APB2ENR_TIM1EN_Msk;                  /* Enable clocks for TIM1. */

    TIM1->ARR                   = 1;

    TIM1->CR1                   |= TIM_CR1_CMS_1;                           /* Center aligned mode 2. */

    /* Timer configuration. */
    TIM1->CCMR1                 |= TIM_CCMR1_OC1M_2 |
                                   TIM_CCMR1_OC1M_1;

    TIM1->CCMR2                 |= TIM_CCMR2_OC3M_2 |
                                   TIM_CCMR2_OC3M_1 |
                                   TIM_CCMR2_OC3M_0;

    NVIC_ClearPendingIRQ(TIM1_UP_TIM16_IRQn);
    NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 1);
    TIM1->DIER                  |=  TIM_DIER_UIE;                           /* Enable update interrupt. */
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}

static void PCC_FC_SinglePhaseBipolarSinePWM_Start_v(void)
{
    /* Dead time configuration. */
    TIM1->BDTR                  &= ~TIM_BDTR_DTG_Msk;
    TIM1->BDTR                  |= (u32)UTIL_TIM_SetMinumumDeadTimeValue_u8(170.0e6f, PCC_FC_SinglePhaseBipolarSinePWM_DeadTime__s__f32);

    UTIL_TIM_SetTimerOverflowFrequency_v(   170.0e6f,
                                            2.0f * PCC_FC_SinglePhaseBipolarSinePWM_ActualParameters_s.switching_freq__Hz__f32,
                                            &TIM1->ARR,
                                            &TIM1->PSC);
    TIM1->CCR1                  = 0;
    TIM1->CCR3                  = 0;

    TIM1->CCMR1                 |= TIM_CCMR1_OC1PE;
    TIM1->CCMR2                 |= TIM_CCMR2_OC3PE;
    TIM1->CR1                   |= TIM_CR1_ARPE;

    TIM1->CCER                  |= TIM_CCER_CC1E    |                       /* Enable PWM channel: CH1. */
                                   TIM_CCER_CC1NE   |                       /* Enable PWM channel: CH1N. */
                                   TIM_CCER_CC3E    |                       /* Enable PWM channel: CH3. */
                                   TIM_CCER_CC3NE;                          /* Enable PWM channel: CH3N. */
    TIM1->BDTR                  |= TIM_BDTR_MOE;
    TIM1->CR1                   |= TIM_CR1_CEN;
}

static void PCC_FC_SinglePhaseBipolarSinePWM_ActiveHandling_v(void)
{
}

static void PCC_FC_SinglePhaseBipolarSinePWM_InterruptHandler_v(void)
{
    UTIL_TIM_SetTimerOverflowFrequency_v(   170.0e6f,
                                            2.0f * PCC_FC_SinglePhaseBipolarSinePWM_ActualParameters_s.switching_freq__Hz__f32,
                                            &TIM1->ARR,
                                            &TIM1->PSC);

    const f32 sin_val_f32 = PCC_FC_SinglePhaseBipolarSinePWM_ActualParameters_s.amplitude_f32 * arm_sin_f32(s_PCC_FC_SinglePhaseBipolarSinePWM_CommutationAngle__rad__f32);
    const u32 compare_val_u32 = UTIL_MapFloatToRange_f32(0.0f, (f32)TIM1->ARR, -1.0f, 1.0f, sin_val_f32);
    TIM1->CCR1 = compare_val_u32;
    TIM1->CCR3 = compare_val_u32;

    s_PCC_FC_SinglePhaseBipolarSinePWM_CommutationAngleStepPerTimerPeriod__rad__f32 =   (PI_d * PCC_FC_SinglePhaseBipolarSinePWM_ActualParameters_s.modulation_freq__Hz__f32) /
                                                                                        PCC_FC_SinglePhaseBipolarSinePWM_ActualParameters_s.switching_freq__Hz__f32;
    s_PCC_FC_SinglePhaseBipolarSinePWM_CommutationAngle__rad__f32 += s_PCC_FC_SinglePhaseBipolarSinePWM_CommutationAngleStepPerTimerPeriod__rad__f32;
    if(s_PCC_FC_SinglePhaseBipolarSinePWM_CommutationAngle__rad__f32 >= TWO_PI_d) s_PCC_FC_SinglePhaseBipolarSinePWM_CommutationAngle__rad__f32 -= TWO_PI_d;

    TIM1->SR &= ~TIM_SR_UIF;
}

static void PCC_FC_SinglePhaseBipolarSinePWM_Stop_v(void)
{
    TIM1->BDTR                  &= ~TIM_BDTR_MOE;
    TIM1->CR1                   &= ~TIM_CR1_CEN;
    TIM1->CCMR1                 &= ~TIM_CCMR1_OC1PE;
    TIM1->CCMR2                 &= ~TIM_CCMR2_OC3PE;
    TIM1->CR1                   &= ~TIM_CR1_ARPE;
}

static void PCC_FC_SinglePhaseBipolarSinePWM_DeInit_v(void)
{
    GPIOA->MODER                |= GPIO_MODER_MODE8_Msk;                    /* Set pin mode to analog. */

    /* Reset timer 1 periphery. */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM1RST_Msk;                /* Force TIM1 peripheral reset. */
    RCC->APB2RSTR               &= ~(RCC_APB2RSTR_TIM1RST_Msk);             /* Release TIM1 peripheral reset. */
    RCC->APB2ENR                |= RCC_APB2ENR_TIM1EN_Msk;                  /* Enable clocks for TIM1. */
    NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
}
