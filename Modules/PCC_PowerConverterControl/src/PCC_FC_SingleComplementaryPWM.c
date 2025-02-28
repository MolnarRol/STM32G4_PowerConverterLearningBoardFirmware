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
static void PCC_FC_SingleComplementaryPWM_IrqHandler_v(void);
static void PCC_FC_SingleComplementaryPWM_Stop_v(void);
static void PCC_FC_SingleComplementaryPWM_DeInit_v(void);

/* TODO: Remove active handling. */
static void PCC_FC_SingleComplementaryPWM_ActiveHandling_v(void) {};

/**********************************************************************************************************************
 * Topology handler structure.
 **********************************************************************************************************************/
const PCC_TopologyHandle_struct PCC_Topology_SingleComplementaryPWM_s =
{
        .initialize_pfv     = PCC_FC_SingleComplementaryPWM_Init_v,
        .start_pf           = PCC_FC_SingleComplementaryPWM_Start_v,
        .active_handler_pfv = PCC_FC_SingleComplementaryPWM_ActiveHandling_v,
        .stop_pfv           = PCC_FC_SingleComplementaryPWM_Stop_v,
        .deinitalize_pfv    = PCC_FC_SingleComplementaryPWM_DeInit_v,
        .isr_handler_pfv    = PCC_FC_SingleComplementaryPWM_IrqHandler_v,
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

/**********************************************************************************************************************
 * Topology control parameters.
 **********************************************************************************************************************/
PCC_FC_PWM_Params_s PCC_FC_SingleComplementaryPWM_SetParams_s =
{
        .duty__per_cent__f32    = 0.0f,
        .frequency__Hz__f32     = 1000.0f,
        .deadtime__s__f32       = 75.0e-9f
};
static PCC_FC_PWM_Params_s s_PCC_FC_SingleComplementaryPWM_ActualParams_s;

/**********************************************************************************************************************
 * Topology control routines.
 **********************************************************************************************************************/
/**
 *  @brief Initialization routine for single PWM generation without complementary signal.
 *  @details
 *      TIM1 configuration:
 *          - Counting mode: Up down
 *          - Interrupt on update event
 *          - Outputs:
 *              - TIM1_CH1: PWM mode 1
 *              - TIM1_CH1N: Complementary to TIM1_CH1
 *      GPIO:
 *          - TIM1_CH1:     PA8 [AF6] - Push-pull, high speed, pull-down
 *          - TIM1_CH1N:    PA7 [AF6] - Push-pull, high speed, pull-down
 */
static void PCC_FC_SingleComplementaryPWM_Init_v(void)
{
    /***********************************************************************************
     * GPIO pin configuration:
     *  - CH1:  PA8     [AF6]
     *  - CH1N: PA7     [AF6]
     ***********************************************************************************/
    MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODE7_Msk | GPIO_MODER_MODE8_Msk,
               (2UL << GPIO_MODER_MODE7_Pos) | (2UL << GPIO_MODER_MODE8_Pos));                  /* Set mode to alternate function. */

    MODIFY_REG(GPIOA->PUPDR,
               GPIO_PUPDR_PUPD7_Msk | GPIO_PUPDR_PUPD8_Msk,
               GPIO_PUPDR_PUPD7_1 | GPIO_PUPDR_PUPD8_1);                                        /* Enable pull down. */

    CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT7_Msk | GPIO_OTYPER_OT8_Msk);                        /* Set output type to push-pull. */
    MODIFY_REG(GPIOA->OSPEEDR,
            GPIO_OSPEEDR_OSPEED7_Msk | GPIO_OSPEEDR_OSPEED8_Msk,
            (2UL << GPIO_OSPEEDR_OSPEED7_Pos) | (2UL << GPIO_OSPEEDR_OSPEED8_Pos));             /* Set pin speed to high. */

    MODIFY_REG(GPIOA->AFR[0], GPIO_AFRL_AFSEL7_Msk, 6UL << GPIO_AFRL_AFSEL7_Pos);               /* PA7 - Set alternate function to AF6. */
    MODIFY_REG(GPIOA->AFR[1], GPIO_AFRH_AFSEL8_Msk, 6UL << GPIO_AFRH_AFSEL8_Pos);               /* PA7 - Set alternate function to AF6. */

    /***********************************************************************************
     * Timer TIM1 configuration:
     *  - CH1/CH1N: PWM mode 1.
     ***********************************************************************************/
    /* Reset timer 1 periphery. */
    SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                           /* Force TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                         /* Release TIM1 peripheral reset. */
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN_Msk);                                              /* Enable clocks for TIM1. */

    SET_BIT(TIM1->CR1, TIM_CR1_CMS_1);                                                          /* Center aligned mode 2. */
    SET_BIT(TIM1->CCMR1, TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);                                  /* PWM mode 1 */
    SET_BIT(TIM1->DIER, TIM_DIER_UIE);                                                          /* Enable update interrupt flag generation. */
    NVIC_SetPriority(TIM1_UP_TIM16_IRQn, CONFIG_PCC_IRQ_PRIORITY_d);                            /* Set correct NVIC interrupt priority. */
}

/**
 * @brief Start routine for single PWM generation without complementary signal.
 * @details
 *  - calculates and writes correct registers for reference frequency and duty cycle
 *  - enables pre-load on ARR and CCR1
 *  - enables CC1 and CC1N
 *  - enables master output enable
 *  - clears timer count
 *  - enables timer and its overflow interrupt
 */
static void PCC_FC_SingleComplementaryPWM_Start_v(void)
{
    /* Copy set parameters to actual parameters. */
    s_PCC_FC_SingleComplementaryPWM_ActualParams_s.frequency__Hz__f32 =
            PCC_FC_SingleComplementaryPWM_SetParams_s.frequency__Hz__f32;
    s_PCC_FC_SingleComplementaryPWM_ActualParams_s.duty__per_cent__f32 =
                PCC_FC_SingleComplementaryPWM_SetParams_s.duty__per_cent__f32;
    s_PCC_FC_SingleComplementaryPWM_ActualParams_s.deadtime__s__f32 =
                PCC_FC_SingleComplementaryPWM_SetParams_s.deadtime__s__f32;

    /* Dead time calculation and write to dead time register. */
    MODIFY_REG(TIM1->BDTR,
               TIM_BDTR_DTG_Msk,
               (u32)UTIL_TIM_SetMinumumDeadTimeValue_u8((f32)SYS_APB1_CLOCK_FREQ__Hz__d,
                                                        PCC_FC_SingleComplementaryPWM_SetParams_s.deadtime__s__f32));

    UTIL_TIM_SetTimerOverflowFrequency_v(
        (f32)SYS_APB1_CLOCK_FREQ__Hz__d,
        UTIL_TIM_UP_DOWN_COUNTER_MODE_FREQ_MULTIPLIER_df32 * PCC_FC_SingleComplementaryPWM_SetParams_s.frequency__Hz__f32,
        &TIM1->ARR,
        &TIM1->PSC
        );

    /* Set correct PWM duty. */
    TIM1->CCR1 = (u16)((s_PCC_FC_SingleComplementaryPWM_ActualParams_s.duty__per_cent__f32 *
                 ((f32)TIM1->ARR + 1.0f))/GEN_DEF_PER_CENT_MAX_df32);

    SET_BIT(TIM1->CCMR1, TIM_CCMR1_OC1PE);                                                      /* Enable pre-load for capture compare 1. */
    SET_BIT(TIM1->CCER, TIM_CCER_CC1E | TIM_CCER_CC1NE);                                        /* Enable CH1. */
    SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);                                                          /* Enable master output. */
    CLEAR_REG(TIM1->CNT);                                                                       /* Reset timer count. */

    NVIC_ClearPendingIRQ(TIM1_UP_TIM16_IRQn);                                                   /* Clear pending interrupt in NVIC. */
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);                                                         /* Enable interrupt in NVIC. */

    SET_BIT(TIM1->CR1,
            TIM_CR1_ARPE |                                                                      /* Enable pre-load for ARR. */
            TIM_CR1_CEN);
}

/**
 * @brief Stop routine for single PWM generation with complementary signal.
 * @details
 *  - disables pre-load on ARR and CCR1
 *  - disables CC1 and CC1N
 *  - disables master output enable
 *  - clears timer count
 *  - disables timer and its overflow interrupt
 */
static void PCC_FC_SingleComplementaryPWM_Stop_v(void)
{
    NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);                                                        /* Enable interrupt in NVIC. */

    CLEAR_BIT(TIM1->CCMR1, TIM_CCMR1_OC1PE);                                                    /* Disable pre-load for capture compare 1. */
    CLEAR_BIT(TIM1->CCER, TIM_CCER_CC1E | TIM_CCER_CC1NE);                                      /* Disable CH1. */
    CLEAR_BIT(TIM1->BDTR, TIM_BDTR_MOE);                                                        /* Disable master output. */
    CLEAR_REG(TIM1->CNT);                                                                       /* Reset timer count. */
    CLEAR_BIT(TIM1->CR1,
            TIM_CR1_ARPE |                                                                      /* Disable pre-load for ARR. */
            TIM_CR1_CEN);
}

/**
 * @brief Update interrupt handler for single PWM generation with complementary signal.
 * @details Writes new frequency, duty cycle and dead time whenever they are changed
 */
static void PCC_FC_SingleComplementaryPWM_IrqHandler_v(void)
{
    /* Check if new PWM frequency was set. */
    if(s_PCC_FC_SingleComplementaryPWM_ActualParams_s.frequency__Hz__f32 !=
       PCC_FC_SingleComplementaryPWM_SetParams_s.frequency__Hz__f32)
    {
        /* Set new frequency. */
        UTIL_TIM_SetTimerOverflowFrequency_v(
            (f32)SYS_APB1_CLOCK_FREQ__Hz__d,
            UTIL_TIM_UP_DOWN_COUNTER_MODE_FREQ_MULTIPLIER_df32 * PCC_FC_SingleComplementaryPWM_SetParams_s.frequency__Hz__f32,
            &TIM1->ARR,
            &TIM1->PSC
            );

        /* Set correct PWM duty. */
        TIM1->CCR1 = (u16)((PCC_FC_SingleComplementaryPWM_SetParams_s.duty__per_cent__f32 *
                     ((f32)TIM1->ARR + 1.0f))/GEN_DEF_PER_CENT_MAX_df32);

        /* Copy set parameters to actual parameters. */
        s_PCC_FC_SingleComplementaryPWM_ActualParams_s.frequency__Hz__f32 =
                PCC_FC_SingleComplementaryPWM_SetParams_s.frequency__Hz__f32;
        s_PCC_FC_SingleComplementaryPWM_ActualParams_s.duty__per_cent__f32 =
                PCC_FC_SingleComplementaryPWM_SetParams_s.duty__per_cent__f32;
    }

    /* Check if new PWM duty was set. */
    else if(s_PCC_FC_SingleComplementaryPWM_ActualParams_s.duty__per_cent__f32 !=
            PCC_FC_SingleComplementaryPWM_SetParams_s.duty__per_cent__f32)
    {
        /* Set correct PWM duty. */
        TIM1->CCR1 = (u16)((PCC_FC_SingleComplementaryPWM_SetParams_s.duty__per_cent__f32 *
                 ((f32)TIM1->ARR + 1.0f))/GEN_DEF_PER_CENT_MAX_df32);

        /* Copy set parameters to actual parameters. */
        s_PCC_FC_SingleComplementaryPWM_ActualParams_s.duty__per_cent__f32 =
                PCC_FC_SingleComplementaryPWM_SetParams_s.duty__per_cent__f32;
    }

    /* Check if new dead time was set. */
    if(s_PCC_FC_SingleComplementaryPWM_ActualParams_s.deadtime__s__f32 !=
       PCC_FC_SingleComplementaryPWM_SetParams_s.deadtime__s__f32)
    {
        /* Calculate and write new dead time. */
        MODIFY_REG(TIM1->BDTR,
                   TIM_BDTR_DTG_Msk,
                   (u32)UTIL_TIM_SetMinumumDeadTimeValue_u8((f32)SYS_APB1_CLOCK_FREQ__Hz__d,
                                                            PCC_FC_SingleComplementaryPWM_SetParams_s.deadtime__s__f32));

        /* Copy set parameters to actual parameters. */
        s_PCC_FC_SingleComplementaryPWM_ActualParams_s.deadtime__s__f32 =
                        PCC_FC_SingleComplementaryPWM_SetParams_s.deadtime__s__f32;
    }

    /* Clear interrupt flag. */
    CLEAR_BIT(TIM1->SR, TIM_SR_UIF);
}

/**
 *  @brief De-initialization routine for single PWM generation with complementary signal.
 *  @details
 *      TIM1 is reset.
 *      GPIO pins are set to analog.
 */
static void PCC_FC_SingleComplementaryPWM_DeInit_v(void)
{
    SET_BIT(GPIOA->MODER, GPIO_MODER_MODE7_Msk | GPIO_MODER_MODE8_Msk);                         /* Set pin modes to analog. */

    /* Reset timer 1 periphery. */
    SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                           /* Force TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                         /* Release TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN_Msk);                                            /* Stop clocks for TIM1. */
}
