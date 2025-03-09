/*
 * PCC_LC_DoubleImpulseControlledRectifier.c
 *
 *  Created on: Feb 23, 2025
 *      Author: molnar
 */
#include "PCC_private_interface.h"
#include "PCC_LC_CommonZeroCrossingDetection.h"
#include "UTIL_public_interface.h"

static void PCC_LC_DoubleImpulseControlledRectifier_Init_v(void);
static void PCC_LC_DoubleImpulseControlledRectifier_DeInit_v(void);

static void PCC_LC_DoubleImpulseControlledRectifier_StartPulses_v(void);
static void PCC_LC_DoubleImpulseControlledRectifier_InhibitPulses_v(void);
static void PCC_LC_DoubleImpulseControlledRectifier_OperationalHandler_v(void);

const PCC_TopologyHandle_struct PCC_Topology_DoubleImpulseControlledRectifier_s =
{
        .initialize_pfv     = PCC_LC_DoubleImpulseControlledRectifier_Init_v,
        .start_pf           = PCC_LC_ZC_Enable_v,
        .stop_pfv           = PCC_LC_ZC_Disable_v,
        .deinitalize_pfv    = PCC_LC_DoubleImpulseControlledRectifier_DeInit_v,
        .isr_handler_pfv    = PCC_LC_ZC_IrqHandler_v,
        .driver_enable_u    =
                            {
                                .drivers_s =
                                {
                                        .gd1_f1 = 1,
                                        .gd2_f1 = 1,
                                        .gd3_f1 = 1,
                                        .gd4_f1 = 1,
                                        .gd5_f1 = 0,
                                        .gd6_f1 = 0
                                }
                            }
};

PCC_LC_CommutationParams_s PCC_LC_DoubleImpulseControlledRectifier_ActualParams_s =
{
        .alpha__deg__f32        = 180.0f,
        .pulse_len__deg__f32    = 10.0f
};

static void PCC_LC_DoubleImpulseControlledRectifier_Init_v(void)
{
    static const PCC_LC_ZC_TopologyControlCallbacks_struct s_zc_control_s =
    {
        .start_pulses_pfv       = PCC_LC_DoubleImpulseControlledRectifier_StartPulses_v,
        .inhibit_pulses_pfv     = PCC_LC_DoubleImpulseControlledRectifier_InhibitPulses_v,
        .operation_handler_pfv  = PCC_LC_DoubleImpulseControlledRectifier_OperationalHandler_v
    };

    /* GPIO pin configuration: PA8, PA10 - AF6 */
    GPIOA->MODER                &= (~GPIO_MODER_MODE0_Msk) &
                                   (~GPIO_MODER_MODE2_Msk) &
                                   (~GPIO_MODER_MODE8_Msk) &
                                   (~GPIO_MODER_MODE10_Msk);                /* Reset pin mode. */

    GPIOA->MODER                |= (2UL << GPIO_MODER_MODE0_Pos) |
                                   (2UL << GPIO_MODER_MODE2_Pos) |
                                   (2UL << GPIO_MODER_MODE8_Pos) |
                                   (2UL << GPIO_MODER_MODE10_Pos);          /* Set mode to alternate function. */

    GPIOA->PUPDR                |= GPIO_PUPDR_PUPD0_1 |
                                   GPIO_PUPDR_PUPD2_1 |
                                   GPIO_PUPDR_PUPD8_1 |
                                   GPIO_PUPDR_PUPD10_1;                     /* Enable pull down. */

    GPIOA->OTYPER               &= (~GPIO_OTYPER_OT0_Msk) &
                                   (~GPIO_OTYPER_OT2_Msk) &
                                   (~GPIO_OTYPER_OT8_Msk) &
                                   (~GPIO_OTYPER_OT10_Msk);                 /* Set output type to push-pull. */

    GPIOA->OSPEEDR              &= (~GPIO_OSPEEDR_OSPEED0_Msk) &
                                   (~GPIO_OSPEEDR_OSPEED2_Msk) &
                                   (~GPIO_OSPEEDR_OSPEED8_Msk) &
                                   (~GPIO_OSPEEDR_OSPEED10_Msk);            /* Reset pin speed. */

    GPIOA->OSPEEDR              |= (2UL << GPIO_OSPEEDR_OSPEED0_Pos) |
                                   (2UL << GPIO_OSPEEDR_OSPEED2_Pos) |
                                   (2UL << GPIO_OSPEEDR_OSPEED8_Pos) |
                                   (2UL << GPIO_OSPEEDR_OSPEED10_Pos);      /* Set pin speed to high. */

    GPIOA->AFR[0]               &= (~GPIO_AFRL_AFSEL0_Msk) &
                                   (~GPIO_AFRL_AFSEL2_Msk);

    GPIOA->AFR[0]               |= (1UL << GPIO_AFRL_AFSEL0_Pos) |
                                   (1UL << GPIO_AFRL_AFSEL2_Pos);

    GPIOA->AFR[1]               &= (~GPIO_AFRH_AFSEL8_Msk) &
                                   (~GPIO_AFRH_AFSEL10_Msk);                /* Reset alternate function. */

    GPIOA->AFR[1]               |= (6UL << GPIO_AFRH_AFSEL8_Pos) |
                                   (6UL << GPIO_AFRH_AFSEL10_Pos);          /* Set alternate function to AF6. */

    /* TIM1 CH1 */
    RCC->APB2ENR                |= RCC_APB2ENR_TIM1EN;                                     /* Enable clocks for TIM1 module. */
    RCC->APB1ENR1               |= RCC_APB1ENR1_TIM2EN;

    /* Reset timer 1 periphery. */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM1RST_Msk;                /* Force TIM1 peripheral reset. */
    RCC->APB2RSTR               &= ~(RCC_APB2RSTR_TIM1RST_Msk);             /* Release TIM1 peripheral reset. */

    /* Reset timer 2 periphery. */
    RCC->APB1RSTR1              |= RCC_APB1RSTR1_TIM2RST_Msk;
    RCC->APB1RSTR1              &= ~RCC_APB1RSTR1_TIM2RST_Msk;

    TIM1->CR1                   |= TIM_CR1_OPM;                             /* One pulse mode. */
    TIM2->CR1                   |= TIM_CR1_OPM;                             /* One pulse mode. */

    /* Timer configuration 1. */
    TIM1->CCMR1                 |= TIM_CCMR1_OC1M_3 |
                                   TIM_CCMR1_OC1M_2 |
                                   TIM_CCMR1_OC1M_0 |                       /* Combined PWM mode 2 */
                                   TIM_CCMR1_OC2M_2 |
                                   TIM_CCMR1_OC2M_1;

    TIM1->CCMR2                 |= TIM_CCMR2_OC3M_3 |
                                   TIM_CCMR2_OC3M_2 |
                                   TIM_CCMR2_OC3M_0 |
                                   TIM_CCMR2_OC4M_2 |
                                   TIM_CCMR2_OC4M_1;

    TIM1->CR2                   |= TIM_CR2_MMS_2;                           /* Set oc1refc as trigger output. */

    TIM1->SMCR                  |= TIM_SMCR_SMS_3 | TIM_SMCR_TS_3 | TIM_SMCR_TS_1;

    /* Timer configuration 2. */
    TIM2->CCMR1                 |= TIM_CCMR1_OC1M_3 |
                                   TIM_CCMR1_OC1M_2 |
                                   TIM_CCMR1_OC1M_0 |                       /* Combined PWM mode 2 */
                                   TIM_CCMR1_OC2M_2 |
                                   TIM_CCMR1_OC2M_1;

    TIM2->CCMR2                 |= TIM_CCMR2_OC3M_3 |
                                   TIM_CCMR2_OC3M_2 |
                                   TIM_CCMR2_OC3M_0 |
                                   TIM_CCMR2_OC4M_2 |
                                   TIM_CCMR2_OC4M_1;

    TIM2->SMCR                  |= TIM_SMCR_SMS_3;                          /* Set TIM2 as slave for TIM1_TRGO */

    PCC_LC_ZC_Init_v(&s_zc_control_s, ZC_LC_ZC_SENSITIVE_TO_RISING_EDGE_d);
}

static void PCC_LC_DoubleImpulseControlledRectifier_DeInit_v(void)
{
    GPIOA->MODER                |= GPIO_MODER_MODE0_Msk |
                                   GPIO_MODER_MODE2_Msk |
                                   GPIO_MODER_MODE8_Msk |
                                   GPIO_MODER_MODE10_Msk;                    /* Set pin mode to analog. */

    /* Reset timer 1 periphery. */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM1RST_Msk;                /* Force TIM1 peripheral reset. */
    RCC->APB2RSTR               &= ~(RCC_APB2RSTR_TIM1RST_Msk);             /* Release TIM1 peripheral reset. */
    RCC->APB2ENR                &= ~RCC_APB2ENR_TIM1EN_Msk;                  /* Enable clocks for TIM1. */

    /* Reset timer 2 periphery. */
    RCC->APB1RSTR1              |= RCC_APB1RSTR1_TIM2RST_Msk;
    RCC->APB1RSTR1              &= ~RCC_APB1RSTR1_TIM2RST_Msk;
    RCC->APB1ENR1               &= ~RCC_APB1ENR1_TIM2EN;
}

static void PCC_LC_DoubleImpulseControlledRectifier_StartPulses_v(void)
{
    PCC_LC_DoubleImpulseControlledRectifier_OperationalHandler_v();
    TIM1->CR1                   |= TIM_CR1_ARPE;
    TIM1->CCMR1                 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
    TIM1->CCMR2                 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE;
    TIM1->CCER                  |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;
    TIM1->BDTR                  |= TIM_BDTR_MOE;                                            /* Master output enable */

    TIM2->CR1                   |= TIM_CR1_ARPE;
    TIM2->CCMR1                 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
    TIM2->CCMR2                 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE;
    TIM2->CCER                  |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;                                          /* Master output enable */
}

static void PCC_LC_DoubleImpulseControlledRectifier_InhibitPulses_v(void)
{
    TIM1->BDTR                  &= ~TIM_BDTR_MOE;                                           /* Master output enable */
    TIM1->CR1                   &= ~TIM_CR1_ARPE;
    TIM1->CCMR1                 &= (~TIM_CCMR1_OC1PE) & (~TIM_CCMR1_OC2PE);
    TIM1->CCMR2                 &= (~TIM_CCMR2_OC3PE) & (~TIM_CCMR2_OC4PE);
    TIM1->CCER                  &= (~TIM_CCER_CC1E) & (~TIM_CCER_CC2E) & (~TIM_CCER_CC3E) & (~TIM_CCER_CC4E);

    TIM2->CR1                   &= ~TIM_CR1_ARPE;
    TIM2->CCMR1                 &= (~TIM_CCMR1_OC1PE) & (~TIM_CCMR1_OC2PE);
    TIM2->CCMR2                 &= (~TIM_CCMR2_OC3PE) & (~TIM_CCMR2_OC4PE);
    TIM2->CCER                  &= (~TIM_CCER_CC1E) & (~TIM_CCER_CC2E) & (~TIM_CCER_CC3E) & (~TIM_CCER_CC4E);
}

static void PCC_LC_DoubleImpulseControlledRectifier_OperationalHandler_v(void)
{
    UTIL_TIM_SetTimerOverflowFrequency_v(170.0e6f, PCC_LC_ZC_LineFreq__Hz__f32, &TIM1->ARR, &TIM1->PSC);
    TIM2->ARR = TIM1->ARR;
    TIM2->PSC = TIM1->PSC;

    TIM1->CCR1 = (u16)UTIL_MapFloatToRange_f32(
                    0.0f,
                    (f32)TIM1->ARR,
                    0.0f,
                    360.0f,
                    PCC_LC_DoubleImpulseControlledRectifier_ActualParams_s.alpha__deg__f32);
    TIM1->CCR2 = (u16)UTIL_MapFloatToRange_f32(
                    0.0f,
                    (f32)TIM1->ARR,
                    0.0f,
                    360.0f,
                    PCC_LC_DoubleImpulseControlledRectifier_ActualParams_s.alpha__deg__f32 + PCC_LC_DoubleImpulseControlledRectifier_ActualParams_s.pulse_len__deg__f32);

    TIM1->CCR3 = TIM1->CCR1;
    TIM1->CCR4 = TIM1->CCR2;


    TIM2->CCR1 = (u16)UTIL_MapFloatToRange_f32(
                    0.0f,
                    (f32)TIM1->ARR,
                    0.0f,
                    360.0f,
                    180.0f);
    TIM2->CCR2 = (u16)UTIL_MapFloatToRange_f32(
                    0.0f,
                    (f32)TIM1->ARR,
                    0.0f,
                    360.0f,
                    180.0f + PCC_LC_DoubleImpulseControlledRectifier_ActualParams_s.pulse_len__deg__f32);

    TIM2->CCR3 = TIM2->CCR1;
    TIM2->CCR4 = TIM2->CCR2;
}
