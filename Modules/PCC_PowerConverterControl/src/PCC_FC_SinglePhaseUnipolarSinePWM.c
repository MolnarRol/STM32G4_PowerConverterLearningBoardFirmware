/*
 * PCC_FC_SinglePhaseSinePWM.c
 *
 *  Created on: Feb 21, 2025
 *      Author: molnar
 */
#include "PCC_private_interface.h"
#include "UTIL_public_interface.h"

/**********************************************************************************************************************
 * Defines.
 **********************************************************************************************************************/
#define _SET_SW_FREQ_d              _s_set_params_s.SinePWM_struct.switching_frequency__Hz__s.val_f32
#define _SET_MOD_FREQ_d             _s_set_params_s.SinePWM_struct.modulation_frequency__Hz__s.val_f32
#define _SET_AMPLIDTUDE_d           _s_set_params_s.SinePWM_struct.amplitude__per_cent__s.val_f32
#define _SET_DEADTIME_d             _s_set_params_s.SinePWM_struct.dead_time__s__s.val_f32

/**********************************************************************************************************************
 * Static prototypes.
 **********************************************************************************************************************/
static void _s_init_v(void);
static void _s_start_v(void);
static void _s_stop_v(void);
static void _s_irq_handler_v(void);
static void _s_deinit_v(void);

/**********************************************************************************************************************
 * Topology control parameters.
 **********************************************************************************************************************/
static PCC_Params_struct _s_set_params_s = {
    .type_e = PCC_ParamType_SinePWM_e,
    .SinePWM_struct = {
        .switching_frequency__Hz__s = {.min_f32 = 1000.0f, .max_f32 = 100e3f, .val_f32 = 10.0e3f},
        .modulation_frequency__Hz__s = {.min_f32 = 0.0f, .max_f32 = 1000.0f, .val_f32 = 0.0f},
        .amplitude__per_cent__s = {.min_f32 = 0.0f, .max_f32 = 100.0f, .val_f32 = 0.0f},
        .dead_time__s__s = {.min_f32 = 0.0f, .max_f32 = 5000.0e-9f, .val_f32 = 200.0e-9f}
    }
};

static volatile f32 _s_switching_freq__Hz__f32;
static volatile f32 _s_modulation_freq__Hz__f32;
static volatile f32 _s_amplitude__per_cent__f32;
static volatile f32 _s_dead_time__s__f32;
static volatile f32 _s_commutation_angle__rad__f32;
static volatile f32 _s_commutation_angle_step_per_period__rad__f32;

/**********************************************************************************************************************
 * Topology handler structure.
 **********************************************************************************************************************/
const PCC_TopologyHandle_struct PCC_Topology_SinglePhaseUnipolarSinePWM_s =
{
    .ctrl_params_pv     = &_s_set_params_s,
    .initialize_pfv     = _s_init_v,
    .start_pf           = _s_start_v,
    .stop_pfv           = _s_stop_v,
    .deinitalize_pfv    = _s_deinit_v,
    .isr_handler_pfv    = _s_irq_handler_v,
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

/**********************************************************************************************************************
 * Topology control routines.
 **********************************************************************************************************************/
static void _s_init_v(void)
{
    /***********************************************************************************
     * GPIO pin configuration:
     *  - CH1:  PA8     [AF6]
     *  - CH1N: PA7     [AF6]
     *  - CH3:  PA10    [AF6]
     *  - CH3N: PB1     [AF6]
     ***********************************************************************************/
    MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODE7_Msk |
               GPIO_MODER_MODE8_Msk |
               GPIO_MODER_MODE10_Msk,
               (2UL << GPIO_MODER_MODE7_Pos) |
               (2UL << GPIO_MODER_MODE8_Pos) |
               (2UL << GPIO_MODER_MODE10_Pos));                                                 /* Set mode to alternate function. */

    MODIFY_REG(GPIOB->MODER,
               GPIO_MODER_MODE1_Msk,
               (2UL << GPIO_MODER_MODE1_Pos));                                                  /* Set mode to alternate function. */

    MODIFY_REG(GPIOA->PUPDR,
               GPIO_PUPDR_PUPD7_Msk |
               GPIO_PUPDR_PUPD8_Msk |
               GPIO_PUPDR_PUPD10_Msk,
               GPIO_PUPDR_PUPD7_1 |
               GPIO_PUPDR_PUPD8_1 |
               GPIO_PUPDR_PUPD10_1);                                                            /* Enable pull down. */

    MODIFY_REG(GPIOB->PUPDR,
               GPIO_PUPDR_PUPD1_Msk,
               GPIO_PUPDR_PUPD1_1);                                                             /* Enable pull down. */

    CLEAR_BIT(GPIOA->OTYPER,
              GPIO_OTYPER_OT7_Msk |
              GPIO_OTYPER_OT8_Msk |
              GPIO_OTYPER_OT10_Msk);                                                            /* Set output type to push-pull. */

    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT1_Msk);                                              /* Set output type to push-pull. */

    MODIFY_REG(GPIOA->OSPEEDR,
               GPIO_OSPEEDR_OSPEED7_Msk |
               GPIO_OSPEEDR_OSPEED8_Msk |
               GPIO_OSPEEDR_OSPEED10_Msk,
               (2UL << GPIO_OSPEEDR_OSPEED7_Pos) |
               (2UL << GPIO_OSPEEDR_OSPEED8_Pos) |
               (2UL << GPIO_OSPEEDR_OSPEED10_Pos));                                             /* Set pin speed to high. */

    MODIFY_REG(GPIOB->OSPEEDR,
               GPIO_OSPEEDR_OSPEED1_Msk,
               (2UL << GPIO_OSPEEDR_OSPEED1_Pos));                                              /* Set pin speed to high. */

    MODIFY_REG(GPIOA->AFR[0],
               GPIO_AFRL_AFSEL7_Msk,
               (6UL << GPIO_AFRL_AFSEL7_Pos));

    MODIFY_REG(GPIOA->AFR[1],
            GPIO_AFRH_AFSEL8_Msk |
            GPIO_AFRH_AFSEL10_Msk,
            (6UL << GPIO_AFRH_AFSEL8_Pos) |
            (6UL << GPIO_AFRH_AFSEL10_Pos));                                                    /* Set alternate function to AF6. */

    MODIFY_REG(GPIOB->AFR[0],
               GPIO_AFRL_AFSEL1_Msk,
               (6UL << GPIO_AFRL_AFSEL1_Pos));                                                  /* Set alternate function to AF6. */

    /***********************************************************************************
     * Timer TIM1 configuration:
     *  - CH1/CH1N: PWM mode 1.
     *  - CH3/CH3N: PWM mode 1.
     ***********************************************************************************/

    /* Reset timer 1 periphery. */
    SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                           /* Force TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                         /* Release TIM1 peripheral reset. */
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN_Msk);                                              /* Enable clocks for TIM1. */

    SET_BIT(TIM1->CR1, TIM_CR1_CMS_1);                                                          /* Center aligned mode 2. */
    SET_BIT(TIM1->CCMR1, TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);                                  /* PWM mode 1. */
    SET_BIT(TIM1->CCMR2, TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);                                  /* PWM mode 1. */
    SET_BIT(TIM1->DIER, TIM_DIER_UIE);                                                          /* Enable update interrupt flag generation. */
    NVIC_SetPriority(TIM1_UP_TIM16_IRQn, CONFIG_PCC_IRQ_PRIORITY_d);                            /* Set correct NVIC interrupt priority. */
}

static void _s_start_v(void)
{
    PCC_CheckAndCorrectIncorrectParameters_v();

    /* Copy set parameters to actual parameters. */
    _s_switching_freq__Hz__f32      = _SET_SW_FREQ_d;
    _s_modulation_freq__Hz__f32     = _SET_MOD_FREQ_d;
    _s_amplitude__per_cent__f32     = _SET_AMPLIDTUDE_d;
    _s_dead_time__s__f32            = _SET_DEADTIME_d;

    /* Dead time calculation and write to dead time register. */
    MODIFY_REG(TIM1->BDTR,
               TIM_BDTR_DTG_Msk,
               (u32)UTIL_TIM_SetMinumumDeadTimeValue_u8((f32)SYS_APB1_CLOCK_FREQ__Hz__d, _s_dead_time__s__f32));

    /* Calculate and set timer register values for set switching frequency. */
    UTIL_TIM_SetTimerOverflowFrequency_v(
        (f32)SYS_APB1_CLOCK_FREQ__Hz__d,
        UTIL_TIM_UP_DOWN_COUNTER_MODE_FREQ_MULTIPLIER_df32 * _s_switching_freq__Hz__f32,
        &TIM1->ARR,
        &TIM1->PSC
        );

    const f32 sin_val_f32 = _SET_AMPLIDTUDE_d * arm_sin_f32(_s_commutation_angle__rad__f32);
    const u32 compare_val_u32 = UTIL_MapFloatToRange_f32(0.0f, (f32)TIM1->ARR, -100.0f, 100.0f, sin_val_f32);
    TIM1->CCR1 = TIM1->ARR - compare_val_u32;
    TIM1->CCR3 = compare_val_u32;

    _s_commutation_angle_step_per_period__rad__f32 = (PI_d * _SET_MOD_FREQ_d) / _SET_SW_FREQ_d;

    _s_commutation_angle__rad__f32 += _s_commutation_angle_step_per_period__rad__f32;
    if(_s_commutation_angle__rad__f32 >= TWO_PI_d) _s_commutation_angle__rad__f32 -= TWO_PI_d;


    SET_BIT(TIM1->CCMR1, TIM_CCMR1_OC1PE);                                                      /* Enable pre-load for capture compare 1. */
    SET_BIT(TIM1->CCMR2, TIM_CCMR2_OC3PE);                                                      /* Enable pre-load for capture compare 3. */

    SET_BIT(TIM1->CCER,
            TIM_CCER_CC1E    |                                                                  /* Enable PWM channel: CH1. */
            TIM_CCER_CC1NE   |                                                                  /* Enable PWM channel: CH1N. */
            TIM_CCER_CC3E    |                                                                  /* Enable PWM channel: CH3. */
            TIM_CCER_CC3NE);                                                                    /* Enable PWM channel: CH3N. */

    SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);                                                          /* Enable master output. */
    CLEAR_REG(TIM1->CNT);                                                                       /* Reset timer count. */


    NVIC_ClearPendingIRQ(TIM1_UP_TIM16_IRQn);                                                   /* Clear pending interrupt in NVIC. */
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);                                                         /* Enable interrupt in NVIC. */

    SET_BIT(TIM1->CR1,
            TIM_CR1_ARPE |                                                                      /* Enable pre-load for ARR. */
            TIM_CR1_CEN);                                                                       /* Enable counter. */
}

static void _s_irq_handler_v(void)
{
    PCC_CheckAndCorrectIncorrectParameters_v();

    if(_s_switching_freq__Hz__f32 != _SET_SW_FREQ_d)
    {
        /* Calculate and set timer register values for set switching frequency. */
        UTIL_TIM_SetTimerOverflowFrequency_v(
            (f32)SYS_APB1_CLOCK_FREQ__Hz__d,
            UTIL_TIM_UP_DOWN_COUNTER_MODE_FREQ_MULTIPLIER_df32 * _SET_SW_FREQ_d,
            &TIM1->ARR,
            &TIM1->PSC
            );

        _s_switching_freq__Hz__f32 = _SET_SW_FREQ_d;

        /* Update commutation angle step. */
        _s_commutation_angle_step_per_period__rad__f32 = (PI_d * _SET_MOD_FREQ_d) / _SET_SW_FREQ_d;
    }

    if(_s_modulation_freq__Hz__f32 != _SET_MOD_FREQ_d)
    {
        _s_modulation_freq__Hz__f32 = _SET_MOD_FREQ_d;

        /* Update commutation angle step. */
        _s_commutation_angle_step_per_period__rad__f32 = (PI_d * _SET_MOD_FREQ_d) / _SET_SW_FREQ_d;
    }

    _s_amplitude__per_cent__f32 = _SET_AMPLIDTUDE_d;

    const f32 sin_val_f32 = _SET_AMPLIDTUDE_d * arm_sin_f32(_s_commutation_angle__rad__f32);
    const u32 compare_val_u32 = UTIL_MapFloatToRange_f32(0.0f, (f32)TIM1->ARR, -100.0f, 100.0f, sin_val_f32);
    TIM1->CCR1 = TIM1->ARR - compare_val_u32;
    TIM1->CCR3 = compare_val_u32;

    if(_s_dead_time__s__f32 != _SET_DEADTIME_d)
    {

        /* Calculate and write new dead time. */
        MODIFY_REG(TIM1->BDTR,
                   TIM_BDTR_DTG_Msk,
                   (u32)UTIL_TIM_SetMinumumDeadTimeValue_u8((f32)SYS_APB1_CLOCK_FREQ__Hz__d, _SET_DEADTIME_d));

        /* Copy set parameters to actual parameters. */
        _s_dead_time__s__f32 = _SET_DEADTIME_d;
    }

    /* Increment current angle and handle 2pi overflow. */
    _s_commutation_angle__rad__f32 += _s_commutation_angle_step_per_period__rad__f32;
    if(_s_commutation_angle__rad__f32 >= TWO_PI_d) _s_commutation_angle__rad__f32 -= TWO_PI_d;

    /* Clear interrupt flag. */
    CLEAR_BIT(TIM1->SR, TIM_SR_UIF);
}

static void _s_stop_v(void)
{
    NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);                                                        /* Disable interrupt in NVIC. */

    CLEAR_BIT(TIM1->CCER,
              TIM_CCER_CC1E |                                                                   /* Enable PWM channel: CH1. */
              TIM_CCER_CC1NE |                                                                  /* Enable PWM channel: CH1N. */
              TIM_CCER_CC3E |                                                                   /* Enable PWM channel: CH3. */
              TIM_CCER_CC3NE);                                                                  /* Enable PWM channel: CH3N. */
    CLEAR_BIT(TIM1->CCMR1, TIM_CCMR1_OC1PE);                                                    /* Disable pre-load for capture compare 1. */
    CLEAR_BIT(TIM1->CCMR2, TIM_CCMR2_OC3PE);                                                    /* Disable pre-load for capture compare 3. */

    CLEAR_BIT(TIM1->BDTR, TIM_BDTR_MOE);                                                        /* Disable master output. */
    CLEAR_REG(TIM1->CNT);                                                                       /* Reset timer count. */
    CLEAR_BIT(TIM1->CR1,
            TIM_CR1_ARPE |                                                                      /* Disable pre-load for ARR. */
            TIM_CR1_CEN);
}

static void _s_deinit_v(void)
{
    /* Set pin modes to analog. */
    SET_BIT(GPIOA->MODER,
            GPIO_MODER_MODE7_Msk |
            GPIO_MODER_MODE8_Msk |
            GPIO_MODER_MODE10_Msk);

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE1_Msk);

    /* Reset timer 1 periphery. */
    SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                           /* Force TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                         /* Release TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN_Msk);                                            /* Stop clocks for TIM1. */
}
