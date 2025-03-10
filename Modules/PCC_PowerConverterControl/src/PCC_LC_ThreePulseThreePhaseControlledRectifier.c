/*
 * PCC_LC_ThreePulseThreePhaseControlledRectifier.c
 *
 *  Created on: Feb 23, 2025
 *      Author: molnar
 */
#include "PCC_private_interface.h"
#include "PCC_LC_CommonZeroCrossingDetection.h"
#include "UTIL_public_interface.h"

/**********************************************************************************************************************
 * Defines.
 **********************************************************************************************************************/
#define _SET_ALPHA_d                            (_s_set_params_s.LineCommutation_struct.firing_angle__deg__s.val_f32)
#define _SET_PULSE_LEN_d                        (_s_set_params_s.LineCommutation_struct.pulse_len__deg__s.val_f32)

/**********************************************************************************************************************
 * Static prototypes.
 **********************************************************************************************************************/
static void _s_init_v(void);
static void _s_deinit_v(void);
static void _s_start_pulses_v(void);
static void _s_inhibit_pulses_v(void);
static void _s_operational_handler_v(void);

/**********************************************************************************************************************
 * Topology control parameters.
 **********************************************************************************************************************/
static PCC_Params_struct _s_set_params_s = {
    .type_e = PCC_ParamType_LineCommutated_e,
    .LineCommutation_struct = {
        .firing_angle__deg__s = {.min_f32 = 0.0f, .max_f32 = 90.0f, .val_f32 = 90.0f},
        .pulse_len__deg__s = {.min_f32 = 2.0f, .max_f32 = 30.0f, .val_f32 = 10.0f},
    }
};

/**********************************************************************************************************************
 * Topology handler structure.
 **********************************************************************************************************************/
const PCC_TopologyHandle_struct PCC_Topology_ThreePulseThreePhaseControlledRectifier_s =
{
    .ctrl_params_pv     = &_s_set_params_s,
    .initialize_pfv     = _s_init_v,
    .start_pf           = PCC_LC_ZC_Enable_v,
    .stop_pfv           = PCC_LC_ZC_Disable_v,
    .deinitalize_pfv    = _s_deinit_v,
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
                        },
    .ctrl_name_str      = "3 impulse AC/DC\0"
};

/**********************************************************************************************************************
 * Topology control routines.
 **********************************************************************************************************************/
static void _s_init_v(void)
{
    static const PCC_LC_ZC_TopologyControlCallbacks_struct s_zc_control_s =
    {
        .start_pulses_pfv       = _s_start_pulses_v,
        .inhibit_pulses_pfv     = _s_inhibit_pulses_v,
        .operation_handler_pfv  = _s_operational_handler_v
    };

    /*************************************************************************************************
     * GPIO configuration
     *************************************************************************************************/
    /* Set mode to alternate function. */
    MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODE0_Msk |
               GPIO_MODER_MODE8_Msk,
               (2UL << GPIO_MODER_MODE0_Pos) |
               (2UL << GPIO_MODER_MODE8_Pos));

    MODIFY_REG(GPIOB->MODER,
               GPIO_MODER_MODE4_Msk,
               (2UL << GPIO_MODER_MODE4_Pos));

    /* Enable pull down. */
    MODIFY_REG(GPIOA->PUPDR,
               GPIO_PUPDR_PUPD0_Msk |
               GPIO_PUPDR_PUPD8_Msk,
               GPIO_PUPDR_PUPD0_1 |
               GPIO_PUPDR_PUPD8_1);

    MODIFY_REG(GPIOB->PUPDR,
               GPIO_PUPDR_PUPD4_Msk,
               GPIO_PUPDR_PUPD4_1);

    /* Set output type to push-pull. */
    CLEAR_BIT(GPIOA->OTYPER,
              GPIO_OTYPER_OT0_Msk |
              GPIO_OTYPER_OT8_Msk);

    CLEAR_BIT(GPIOB->OTYPER,
              GPIO_OTYPER_OT4_Msk);

    /* Set pin speed to high. */
    MODIFY_REG(GPIOA->OSPEEDR,
               GPIO_OSPEEDR_OSPEED0_Msk |
               GPIO_OSPEEDR_OSPEED8_Msk,
               (2UL << GPIO_OSPEEDR_OSPEED0_Pos) |
               (2UL << GPIO_OSPEEDR_OSPEED8_Pos));

    MODIFY_REG(GPIOB->OSPEEDR,
               GPIO_OSPEEDR_OSPEED4_Msk,
               (2UL << GPIO_OSPEEDR_OSPEED4_Pos));


    MODIFY_REG(GPIOA->AFR[0],
               GPIO_AFRL_AFSEL0_Msk,
               (1UL << GPIO_AFRL_AFSEL0_Pos));

    MODIFY_REG(GPIOA->AFR[1],
               GPIO_AFRH_AFSEL8_Msk,
               (6UL << GPIO_AFRH_AFSEL8_Pos));

    MODIFY_REG(GPIOB->AFR[0],
               GPIO_AFRL_AFSEL4_Msk,
               (2UL << GPIO_AFRL_AFSEL4_Pos));

    /***********************************************************************************
     * Timer TIM1 configuration:
     ***********************************************************************************/
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN);

    /* Reset timer 1 periphery. */
    SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);
    CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);

    /* Reset timer 2 and 3 periphery. */
    SET_BIT(RCC->APB1RSTR1, RCC_APB1RSTR1_TIM2RST_Msk | RCC_APB1RSTR1_TIM3RST_Msk);
    CLEAR_BIT(RCC->APB1RSTR1, RCC_APB1RSTR1_TIM2RST_Msk | RCC_APB1RSTR1_TIM3RST_Msk);


    /* Timer configuration 1. */
    SET_BIT(TIM1->CR1, TIM_CR1_OPM);
    SET_BIT(TIM1->CCMR1,
            TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0 |        /* Combined PWM mode 2. */
            TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);                           /* PWM mode 1. */

    SET_BIT(TIM1->CR2, TIM_CR2_MMS_2);
    SET_BIT(TIM1->SMCR, TIM_SMCR_SMS_3 | TIM_SMCR_TS_3 | TIM_SMCR_TS_1);

    /* Timer configuration 2. */
    SET_BIT(TIM2->CR1, TIM_CR1_OPM);
    SET_BIT(TIM2->CCMR1,
            TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0 |        /* Combined PWM mode 2. */
            TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);                           /* PWM mode 1. */

    SET_BIT(TIM2->CR2, TIM_CR2_MMS_2);                                      /* Set oc3refc as trigger output. */
    SET_BIT(TIM2->SMCR, TIM_SMCR_SMS_3);

    /* Timer configuration 3. */
    SET_BIT(TIM3->CR1, TIM_CR1_OPM);
    SET_BIT(TIM3->CCMR1,
            TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0 |        /* Combined PWM mode 2. */
            TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);                           /* PWM mode 1. */

    SET_BIT(TIM3->SMCR, TIM_SMCR_SMS_3 | TIM_SMCR_TS_0);                    /* Set TIM3 as slave for TIM2_TRGO */

    PCC_LC_ZC_Init_v(&s_zc_control_s, ZC_LC_ZC_SENSITIVE_TO_RISING_EDGE_d);
}

static void _s_deinit_v(void)
{
    /* Set pin mode to analog. */
    SET_BIT(GPIOA->MODER,
            GPIO_MODER_MODE0_Msk |
            GPIO_MODER_MODE8_Msk);

    SET_BIT(GPIOB->MODER,
            GPIO_MODER_MODE4_Msk);

    /* Reset timer 1 periphery. */
    SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                   /* Force TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                 /* Release TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN_Msk);                                    /* Disable clocks for TIM1. */

    /* Reset timer 2 and 3 periphery. */
    SET_BIT(RCC->APB1RSTR1, RCC_APB1RSTR1_TIM2RST_Msk | RCC_APB1RSTR1_TIM3RST_Msk);
    CLEAR_BIT(RCC->APB1RSTR1, RCC_APB1RSTR1_TIM2RST_Msk | RCC_APB1RSTR1_TIM3RST_Msk);
    CLEAR_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM2EN | RCC_APB1ENR1_TIM3EN);
}

static void _s_start_pulses_v(void)
{
    _s_operational_handler_v();

    /* Enable auto-reload preload. */
    SET_BIT(TIM1->CR1, TIM_CR1_ARPE);
    SET_BIT(TIM2->CR1, TIM_CR1_ARPE);
    SET_BIT(TIM3->CR1, TIM_CR1_ARPE);

    /* Enable output compare channel preloads. */
    SET_BIT(TIM1->CCMR1,TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
    SET_BIT(TIM2->CCMR1,TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
    SET_BIT(TIM3->CCMR1,TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);

    /* Enable timer channels. */
    SET_BIT(TIM1->CCER,
            TIM_CCER_CC1E |
            TIM_CCER_CC2E);

    SET_BIT(TIM2->CCER,
            TIM_CCER_CC1E |
            TIM_CCER_CC2E);

    SET_BIT(TIM3->CCER,
            TIM_CCER_CC1E |
            TIM_CCER_CC2E);

    /* Enable master output on timer1. */
    SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);
}

static void _s_inhibit_pulses_v(void)
{
    /* Disable master output on timer1. */
    CLEAR_BIT(TIM1->BDTR, TIM_BDTR_MOE);

    /* Disable timer channels. */
    CLEAR_BIT(TIM1->CCER,
            TIM_CCER_CC1E |
            TIM_CCER_CC2E);

    CLEAR_BIT(TIM2->CCER,
                TIM_CCER_CC1E |
                TIM_CCER_CC2E);

    CLEAR_BIT(TIM3->CCER,
                TIM_CCER_CC1E |
                TIM_CCER_CC2E);

    /* Disable auto-reload preload. */
    CLEAR_BIT(TIM1->CR1, TIM_CR1_ARPE);
    CLEAR_BIT(TIM2->CR1, TIM_CR1_ARPE);
    CLEAR_BIT(TIM3->CR1, TIM_CR1_ARPE);

    /* Disable output compare channel preloads. */
    CLEAR_BIT(TIM1->CCMR1,TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
    CLEAR_BIT(TIM2->CCMR1,TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
    CLEAR_BIT(TIM3->CCMR1,TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
}

static void _s_operational_handler_v(void)
{
    PCC_CheckAndCorrectIncorrectParameters_v();
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
                    _SET_ALPHA_d);

    TIM1->CCR2 = (u16)UTIL_MapFloatToRange_f32(
                    0.0f,
                    (f32)TIM1->ARR,
                    0.0f,
                    360.0f,
                    _SET_ALPHA_d + _SET_PULSE_LEN_d);

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
                    120.0f + _SET_PULSE_LEN_d);

    TIM3->CCR1 = TIM2->CCR1;
    TIM3->CCR2 = TIM2->CCR2;
}
