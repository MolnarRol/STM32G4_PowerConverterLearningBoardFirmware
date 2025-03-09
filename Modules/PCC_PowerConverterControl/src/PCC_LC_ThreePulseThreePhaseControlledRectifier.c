/*
 * PCC_LC_ThreePulseThreePhaseControlledRectifier.c
 *
 *  Created on: Feb 23, 2025
 *      Author: molnar
 */
#include "PCC_private_interface.h"
#include "PCC_LC_CommonZeroCrossingDetection.h"
#include "UTIL_public_interface.h"

static void PCC_LC_ThreePulseThreePhaseControlledRectifier_Init_v(void);
static void PCC_LC_ThreePulseThreePhaseControlledRectifier_DeInit_v(void);

static void PCC_LC_ThreePulseThreePhaseControlledRectifier_StartPulses_v(void);
static void PCC_LC_ThreePulseThreePhaseControlledRectifier_InhibitPulses_v(void);
static void PCC_LC_ThreePulseThreePhaseControlledRectifier_OperationalHandler_v(void);

const PCC_TopologyHandle_struct PCC_Topology_ThreePulseThreePhaseControlledRectifier_s =
{
        .initialize_pfv     = PCC_LC_ThreePulseThreePhaseControlledRectifier_Init_v,
        .start_pf           = PCC_LC_ZC_Enable_v,
        .stop_pfv           = PCC_LC_ZC_Disable_v,
        .deinitalize_pfv    = PCC_LC_ThreePulseThreePhaseControlledRectifier_DeInit_v,
        .isr_handler_pfv    = PCC_LC_ZC_IrqHandler_v,
        .driver_enable_u    =
                            {
                                .drivers_s =
                                {
                                        .gd1_f1 = 1,
                                        .gd2_f1 = 0,
                                        .gd3_f1 = 1,
                                        .gd4_f1 = 0,
                                        .gd5_f1 = 1,
                                        .gd6_f1 = 0
                                }
                            }
};

PCC_LC_CommutationParams_s PCC_LC_ThreePulseThreePhaseControlledRectifier_ActualParams_s =
{
        .alpha__deg__f32        = 120.0f,
        .pulse_len__deg__f32    = 10.0f
};

static void PCC_LC_ThreePulseThreePhaseControlledRectifier_Init_v(void)
{
    static const PCC_LC_ZC_TopologyControlCallbacks_struct s_zc_control_s =
    {
        .start_pulses_pfv       = PCC_LC_ThreePulseThreePhaseControlledRectifier_StartPulses_v,
        .inhibit_pulses_pfv     = PCC_LC_ThreePulseThreePhaseControlledRectifier_InhibitPulses_v,
        .operation_handler_pfv  = PCC_LC_ThreePulseThreePhaseControlledRectifier_OperationalHandler_v
    };

/*************************************************************************************************
 * GPIO configuration
 *************************************************************************************************/
    GPIOA->MODER                &= (~GPIO_MODER_MODE0_Msk) &
                                   (~GPIO_MODER_MODE8_Msk);                /* Reset pin mode. */

    GPIOB->MODER                &= (~GPIO_MODER_MODE4_Msk);

    GPIOA->MODER                |= (2UL << GPIO_MODER_MODE0_Pos) |
                                   (2UL << GPIO_MODER_MODE8_Pos);          /* Set mode to alternate function. */

    GPIOB->MODER                |= (2UL << GPIO_MODER_MODE4_Pos);

    GPIOA->PUPDR                |= GPIO_PUPDR_PUPD0_1 |
                                   GPIO_PUPDR_PUPD8_1 ;                     /* Enable pull down. */

    GPIOB->PUPDR                |= GPIO_PUPDR_PUPD4_1;

    GPIOA->OTYPER               &= (~GPIO_OTYPER_OT0_Msk) &
                                   (~GPIO_OTYPER_OT8_Msk);                 /* Set output type to push-pull. */

    GPIOB->OTYPER               &= (~GPIO_OTYPER_OT4_Msk);

    GPIOA->OSPEEDR              &= (~GPIO_OSPEEDR_OSPEED0_Msk) &
                                   (~GPIO_OSPEEDR_OSPEED8_Msk);            /* Reset pin speed. */

    GPIOB->OSPEEDR              &= (~GPIO_OSPEEDR_OSPEED4_Msk);

    GPIOA->OSPEEDR              |= (2UL << GPIO_OSPEEDR_OSPEED0_Pos) |
                                   (2UL << GPIO_OSPEEDR_OSPEED8_Pos);      /* Set pin speed to high. */

    GPIOB->OSPEEDR              |= (2UL << GPIO_OSPEEDR_OSPEED4_Pos);

    GPIOA->AFR[0]               &= (~GPIO_AFRL_AFSEL0_Msk);

    GPIOA->AFR[0]               |= (1UL << GPIO_AFRL_AFSEL0_Pos);

    GPIOB->AFR[0]               &= (~GPIO_AFRL_AFSEL4_Msk);

    GPIOB->AFR[0]               |= (2UL << GPIO_AFRL_AFSEL4_Pos);

    GPIOA->AFR[1]               &= (~GPIO_AFRH_AFSEL8_Msk);                /* Reset alternate function. */

    GPIOA->AFR[1]               |= (6UL << GPIO_AFRH_AFSEL8_Pos);          /* Set alternate function to AF6. */

    /* TIM1 CH1 */
    RCC->APB2ENR                |= RCC_APB2ENR_TIM1EN;                                     /* Enable clocks for TIM1 module. */
    RCC->APB1ENR1               |= RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN;

    /* Reset timer 1 periphery. */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM1RST_Msk;                /* Force TIM1 peripheral reset. */
    RCC->APB2RSTR               &= ~(RCC_APB2RSTR_TIM1RST_Msk);             /* Release TIM1 peripheral reset. */

    /* Reset timer 2 periphery. */
    RCC->APB1RSTR1              |= RCC_APB1RSTR1_TIM2RST_Msk | RCC_APB1RSTR1_TIM3RST_Msk;
    RCC->APB1RSTR1              &= (~RCC_APB1RSTR1_TIM2RST_Msk) & (~RCC_APB1RSTR1_TIM3RST_Msk);


    /* Timer configuration 1. */
    TIM1->CR1                   |= TIM_CR1_OPM;                             /* One pulse mode. */

    TIM1->CCMR1                 |= TIM_CCMR1_OC1M_3 |
                                   TIM_CCMR1_OC1M_2 |
                                   TIM_CCMR1_OC1M_0 |                       /* Combined PWM mode 2 */
                                   TIM_CCMR1_OC2M_2 |
                                   TIM_CCMR1_OC2M_1;

    TIM1->CR2                   |= TIM_CR2_MMS_2;                           /* Set oc3refc as trigger output. */

    TIM1->SMCR                  |= TIM_SMCR_SMS_3 | TIM_SMCR_TS_3 | TIM_SMCR_TS_1;

    /* Timer configuration 2. */
    TIM2->CR1                   |= TIM_CR1_OPM;                             /* One pulse mode. */

    TIM2->CCMR1                 |= TIM_CCMR1_OC1M_3 |
                                   TIM_CCMR1_OC1M_2 |
                                   TIM_CCMR1_OC1M_0 |                       /* Combined PWM mode 2 */
                                   TIM_CCMR1_OC2M_2 |
                                   TIM_CCMR1_OC2M_1;

    TIM2->CR2                   |= TIM_CR2_MMS_2;                           /* Set oc3refc as trigger output. */

    TIM2->SMCR                  |= TIM_SMCR_SMS_3;                          /* Set TIM2 as slave for TIM1_TRGO */

    /* Timer configuration 3. */
    TIM3->CR1                   |= TIM_CR1_OPM;                             /* One pulse mode. */

    TIM3->CCMR1                 |= TIM_CCMR1_OC1M_3 |
                                   TIM_CCMR1_OC1M_2 |
                                   TIM_CCMR1_OC1M_0 |                       /* Combined PWM mode 2 */
                                   TIM_CCMR1_OC2M_2 |
                                   TIM_CCMR1_OC2M_1;

    TIM3->SMCR                  |= TIM_SMCR_SMS_3 | TIM_SMCR_TS_0;         /* Set TIM3 as slave for TIM2_TRGO */

    PCC_LC_ZC_Init_v(&s_zc_control_s, ZC_LC_ZC_SENSITIVE_TO_RISING_EDGE_d);
}

static void PCC_LC_ThreePulseThreePhaseControlledRectifier_DeInit_v(void)
{
    GPIOA->MODER                |= GPIO_MODER_MODE0_Msk |
                                   GPIO_MODER_MODE8_Msk;                    /* Set pin mode to analog. */

    GPIOB->MODER                |= GPIO_MODER_MODE4_Msk;

    /* Reset timer 1 periphery. */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM1RST_Msk;                /* Force TIM1 peripheral reset. */
    RCC->APB2RSTR               &= ~(RCC_APB2RSTR_TIM1RST_Msk);             /* Release TIM1 peripheral reset. */
    RCC->APB2ENR                &= ~RCC_APB2ENR_TIM1EN_Msk;                  /* Enable clocks for TIM1. */

    /* Reset timer 2 periphery. */
    RCC->APB1RSTR1              |= RCC_APB1RSTR1_TIM2RST_Msk | RCC_APB1RSTR1_TIM3RST_Msk;
    RCC->APB1RSTR1              &= (~RCC_APB1RSTR1_TIM2RST_Msk) & (~RCC_APB1RSTR1_TIM3RST_Msk);
    RCC->APB1ENR1               &= (~RCC_APB1ENR1_TIM2EN) & (~RCC_APB1ENR1_TIM3EN);
}

static void PCC_LC_ThreePulseThreePhaseControlledRectifier_StartPulses_v(void)
{
    PCC_LC_ThreePulseThreePhaseControlledRectifier_OperationalHandler_v();
    TIM1->CR1                   |= TIM_CR1_ARPE;
    TIM1->CCMR1                 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
    TIM1->CCER                  |= TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM1->BDTR                  |= TIM_BDTR_MOE;                                            /* Master output enable */

    TIM2->CR1                   |= TIM_CR1_ARPE;
    TIM2->CCMR1                 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
    TIM2->CCER                  |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    TIM3->CR1                   |= TIM_CR1_ARPE;
    TIM3->CCMR1                 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
    TIM3->CCER                  |= TIM_CCER_CC1E | TIM_CCER_CC2E;
}

static void PCC_LC_ThreePulseThreePhaseControlledRectifier_InhibitPulses_v(void)
{
    TIM1->BDTR                  &= ~TIM_BDTR_MOE;                                           /* Master output enable */
    TIM1->CR1                   &= ~TIM_CR1_ARPE;
    TIM1->CCMR1                 &= (~TIM_CCMR1_OC1PE) & (~TIM_CCMR1_OC2PE);
    TIM1->CCER                  &= (~TIM_CCER_CC1E) & (~TIM_CCER_CC2E);

    TIM2->CR1                   &= ~TIM_CR1_ARPE;
    TIM2->CCMR1                 &= (~TIM_CCMR1_OC1PE) & (~TIM_CCMR1_OC2PE);
    TIM2->CCER                  &= (~TIM_CCER_CC1E) & (~TIM_CCER_CC2E);

    TIM3->CR1                   &= ~TIM_CR1_ARPE;
    TIM3->CCMR1                 &= (~TIM_CCMR1_OC1PE) & (~TIM_CCMR1_OC2PE);
    TIM3->CCER                  &= (~TIM_CCER_CC1E) & (~TIM_CCER_CC2E);
}

static void PCC_LC_ThreePulseThreePhaseControlledRectifier_OperationalHandler_v(void)
{
    UTIL_TIM_SetTimerOverflowFrequency_v(170.0e6f, PCC_LC_ZC_LineFreq__Hz__f32, &TIM1->ARR, &TIM1->PSC);
    TIM2->ARR = TIM1->ARR;
    TIM2->PSC = TIM1->PSC;
    TIM3->ARR = TIM1->ARR;
    TIM3->PSC = TIM1->PSC;

    TIM1->CCR1 = (u16)UTIL_MapFloatToRange_f32(
                    0.0f,
                    (f32)TIM1->ARR,
                    0.0f,
                    360.0f,
                    PCC_LC_ThreePulseThreePhaseControlledRectifier_ActualParams_s.alpha__deg__f32);

    TIM1->CCR2 = (u16)UTIL_MapFloatToRange_f32(
                    0.0f,
                    (f32)TIM1->ARR,
                    0.0f,
                    360.0f,
                    PCC_LC_ThreePulseThreePhaseControlledRectifier_ActualParams_s.alpha__deg__f32 + PCC_LC_ThreePulseThreePhaseControlledRectifier_ActualParams_s.pulse_len__deg__f32);


    TIM2->CCR1 = (u16)UTIL_MapFloatToRange_f32(
                    0.0f,
                    (f32)TIM1->ARR,
                    0.0f,
                    360.0f,
                    120.0f);

    TIM2->CCR2 = (u16)UTIL_MapFloatToRange_f32(
                    0.0f,
                    (f32)TIM1->ARR,
                    0.0f,
                    360.0f,
                    120.0f + PCC_LC_ThreePulseThreePhaseControlledRectifier_ActualParams_s.pulse_len__deg__f32);


    TIM3->CCR1 = TIM2->CCR1;
    TIM3->CCR2 = TIM2->CCR2;
}
