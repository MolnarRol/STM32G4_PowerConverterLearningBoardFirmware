/*
 * PCC_FC_SingleComplementaryPWM.c
 *
 *  Created on: Feb 20, 2025
 *      Author: molnar
 */

#include "PCC_private_interface.h"
#include "UTIL_public_interface.h"

static void PCC_FC_SingleComplementaryPWM_Init_v(void);
static void PCC_FC_SingleComplementaryPWM_Start_v(void);
static void PCC_FC_SingleComplementaryPWM_ActiveHandling_v(void);
static void PCC_FC_SingleComplementaryPWM_Stop_v(void);
static void PCC_FC_SingleComplementaryPWM_DeInit_v(void);

const PCC_TopologyHandle_struct PCC_Topology_SingleComplementaryPWM_s =
{
        .initialize_pfv     = PCC_FC_SingleComplementaryPWM_Init_v,
        .start_pf           = PCC_FC_SingleComplementaryPWM_Start_v,
        .active_handler_pfv = PCC_FC_SingleComplementaryPWM_ActiveHandling_v,
        .stop_pfv           = PCC_FC_SingleComplementaryPWM_Stop_v,
        .deinitalize_pfv    = PCC_FC_SingleComplementaryPWM_DeInit_v,
        .driver_enable_u    =
                            {
                              .drivers_s =
                                {
                                        .gd1_f1 = 1,
                                        .gd2_f1 = 0,
                                        .gd3_f1 = 0,
                                        .gd4_f1 = 1,
                                        .gd5_f1 = 0,
                                        .gd6_f1 = 0
                                }
                            }
};

PCC_FC_PWM_Params_s PCC_FC_SingleComplementaryPWM_NewParameters_s =
{
        .duty__per_cent__f32    = 0.0f,
        .frequency__Hz__f32     = 1000.0f,
        .deadtime__s__f32       = 75.0e-9f
};

static PCC_FC_PWM_Params_s s_PCC_FC_SingleComplementaryPWM_ActualParameters_s = {0};

f32 PCC_FC_SingleComplementaryPWM_freq__Hz__f32         = 1000.0f;
f32 PCC_FC_SingleComplementaryPWM_duty__per_cent__f32   = 0.0f;
f32 PCC_FC_SingleComplementaryPWM_DeadTime__s__f32      = 75.0e-9f;

static void PCC_FC_SingleComplementaryPWM_Init_v(void)
{
    /* TIM1 CH1 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;                                     /* Enable clocks for TIM1 module. */

    /* GPIO pin configuration:
     *      - PA7: CH1N     (AF6)
     *      - PA8: CH1      (AF6)
     */
    GPIOA->MODER                &=  (~GPIO_MODER_MODE8_Msk) &
                                    (~GPIO_MODER_MODE7_Msk);                /* Reset pin mode. */

    GPIOA->MODER                |=  (2UL << GPIO_MODER_MODE8_Pos) |
                                    (2UL << GPIO_MODER_MODE7_Pos);          /* Set mode to alternate function. */

    GPIOA->PUPDR                |=  GPIO_PUPDR_PUPD8_1 |
                                    GPIO_PUPDR_PUPD7_1;                     /* Enable pull down. */

    GPIOA->OTYPER               &=  (~GPIO_OTYPER_OT8_Msk) &
                                    (~GPIO_OTYPER_OT7_Msk);                 /* Set output type to push-pull. */

    GPIOA->OSPEEDR              &=  (~GPIO_OSPEEDR_OSPEED8_Msk) &
                                    (~GPIO_OSPEEDR_OSPEED7_Msk);            /* Reset pin speed. */

    GPIOA->OSPEEDR              |=  (2UL << GPIO_OSPEEDR_OSPEED8_Pos) |
                                    (2UL << GPIO_OSPEEDR_OSPEED7_Pos);      /* Set pin speed to high. */

    GPIOA->AFR[0]               &=  ~GPIO_AFRL_AFSEL7_Msk;
    GPIOA->AFR[1]               &=  ~GPIO_AFRH_AFSEL8_Msk;                  /* Reset alternate function. */

    GPIOA->AFR[0]               |=  6UL << GPIO_AFRL_AFSEL7_Pos;
    GPIOA->AFR[1]               |=  6UL << GPIO_AFRH_AFSEL8_Pos;            /* Set alternate function to AF6. */

    /* Reset timer 1 periphery. */
    RCC->APB2RSTR               |=  RCC_APB2RSTR_TIM1RST_Msk;               /* Force TIM1 peripheral reset. */
    RCC->APB2RSTR               &=  ~RCC_APB2RSTR_TIM1RST_Msk;              /* Release TIM1 peripheral reset. */
    RCC->APB2ENR                |=  RCC_APB2ENR_TIM1EN_Msk;                 /* Enable clocks for TIM1. */

    TIM1->ARR                   =   1;

    TIM1->CR1                   |=  TIM_CR1_ARPE |                          /* Auto reload register pre-load enable. */
                                    TIM_CR1_CMS_1;                          /* Center aligned mode 2. */

    /* Timer configuration. */
    TIM1->CCMR1                 |=  TIM_CCMR1_OC1M_2 |
                                    TIM_CCMR1_OC1M_1 |                      /* PWM mode 1. */
                                    TIM_CCMR1_OC1PE;                        /* Enable pre-load for compare value 1. */

    /* Dead time configuration. */
    TIM1->BDTR                  &= ~TIM_BDTR_DTG_Msk;
    TIM1->BDTR                  |= (u32)UTIL_TIM_SetMinumumDeadTimeValue_u8(170.0e6f, PCC_FC_SingleComplementaryPWM_DeadTime__s__f32);
}

static void PCC_FC_SingleComplementaryPWM_Start_v(void)
{
    TIM1->CNT                   =   0;
    TIM1->CCER                  |=  TIM_CCER_CC1E |
                                    TIM_CCER_CC1NE;                         /* Enable CH1. */
    TIM1->BDTR                  |=  TIM_BDTR_MOE;
    TIM1->CR1                   |=  TIM_CR1_CEN;
}

static void PCC_FC_SingleComplementaryPWM_ActiveHandling_v(void)
{
    UTIL_TIM_SetTimerOverflowFrequency_v(170.0e6f, PCC_FC_SingleComplementaryPWM_freq__Hz__f32, &TIM1->ARR, &TIM1->PSC);
    TIM1->CCR1 = (u16)((PCC_FC_SingleComplementaryPWM_duty__per_cent__f32 * (f32)TIM1->ARR)/100.0f);
}

static void PCC_FC_SingleComplementaryPWM_Stop_v(void)
{
    TIM1->CCER                  &=  (~TIM_CCER_CC1E) &
                                    (~TIM_CCER_CC1NE);
    TIM1->BDTR                  &=  ~TIM_BDTR_MOE;
    TIM1->CR1                   &=  ~TIM_CR1_CEN;
}

static void PCC_FC_SingleComplementaryPWM_DeInit_v(void)
{
    GPIOA->MODER                |= GPIO_MODER_MODE8_Msk |
                                   GPIO_MODER_MODE7_Msk;                    /* Reset pin mode. */

    /* Reset timer 1 periphery. */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM1RST_Msk;                /* Force TIM1 peripheral reset. */
    RCC->APB2RSTR               &= ~(RCC_APB2RSTR_TIM1RST_Msk);             /* Release TIM1 peripheral reset. */
    RCC->APB2ENR                |= RCC_APB2ENR_TIM1EN_Msk;                  /* Enable clocks for TIM1. */
}
