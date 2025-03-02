#include "PCC_private_interface.h"
#include "UTIL_public_interface.h"

#define _SET_FREQ_d                 _s_set_params_s.PhaseShiftedPWM_struct.frequency__Hz__s.val_f32
#define _SET_PHASE_SHIFT_d          _s_set_params_s.PhaseShiftedPWM_struct.phase_shift__deg__s.val_f32
#define _SET_DEADTIME_d             _s_set_params_s.PhaseShiftedPWM_struct.dead_time__s__s.val_f32

static void PCC_FC_FullBridgePhaseShiftedPWM_Init_v(void);
static void PCC_FC_FullBridgePhaseShiftedPWM_Start_v(void);
static void PCC_FC_FullBridgePhaseShiftedPWM_Stop_v(void);
static void PCC_FC_FullBridgePhaseShiftedPWM_IrqHandler_v(void);
static void PCC_FC_FullBridgePhaseShiftedPWM_DeInit_v(void);

/* TODO: Remove active handling. */
static void PCC_FC_FullBridgePhaseShiftedPWM_ActiveHandling_v(void){};

/**********************************************************************************************************************
 * Topology control parameters.
 **********************************************************************************************************************/
static PCC_Params_struct _s_set_params_s = {
        .type_e = PCC_ParamType_PhaseShiftedPWM_e,
        .PhaseShiftedPWM_struct = {
                .frequency__Hz__s               = {.min_f32 = 10.0f, .max_f32 = 250.0e3f, .val_f32 = 100.0e3f},
                .phase_shift__deg__s            = {.min_f32 = 0.0f, .max_f32 = 180.0f, .val_f32 = 0.0f},
                .dead_time__s__s                = {.min_f32 = 0.0f, .max_f32 = 5000.0e-9f, .val_f32 = 75.0e-9f}
        }
};

static volatile f32 _s_freq__Hz__f32;
static volatile f32 _s_phase_shift__deg__f32;
static volatile f32 _s_dead_time__s__f32;

/**********************************************************************************************************************
 * Topology handler structure.
 **********************************************************************************************************************/
const PCC_TopologyHandle_struct PCC_Topology_FullBridgePhaseShiftedPWM_s =
{
    .ctrl_params_pv     = &_s_set_params_s,
    .initialize_pfv     = PCC_FC_FullBridgePhaseShiftedPWM_Init_v,
    .start_pf           = PCC_FC_FullBridgePhaseShiftedPWM_Start_v,
    .active_handler_pfv = PCC_FC_FullBridgePhaseShiftedPWM_ActiveHandling_v,
    .stop_pfv           = PCC_FC_FullBridgePhaseShiftedPWM_Stop_v,
    .deinitalize_pfv    = PCC_FC_FullBridgePhaseShiftedPWM_DeInit_v,
    .isr_handler_pfv    = PCC_FC_FullBridgePhaseShiftedPWM_IrqHandler_v,
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
/**
 *  @brief Initialization routine forphase shifted PWM generation with complementary signals.
 *  @details
 *      TIM1 configuration:
 *          - Counting mode: Up down
 *          - Interrupt on update event
 *          - Outputs:
 *              - TIM1_CH1:     Asymmetric PWM mode 2
 *              - TIM1_CH1N:    Complementary to TIM1_CH1
 *              - TIM1_CH3:     Asymmetric PWM mode 2
 *              - TIM1_CH3N:    Complementary to TIM1_CH1
 *      GPIO:
 *          - TIM1_CH1:     PA8     [AF6] - Push-pull, high speed, pull-down
 *          - TIM1_CH1N:    PA7     [AF6] - Push-pull, high speed, pull-down
 *          - TIM1_CH3:     PA10    [AF6] - Push-pull, high speed, pull-down
 *          - TIM1_CH3N:    PB1     [AF6] - Push-pull, high speed, pull-down
 */
static void PCC_FC_FullBridgePhaseShiftedPWM_Init_v(void)
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

    MODIFY_REG(GPIOA->PUPDR,
               GPIO_PUPDR_PUPD7_Msk |
               GPIO_PUPDR_PUPD8_Msk |
               GPIO_PUPDR_PUPD10_Msk,
               GPIO_PUPDR_PUPD7_1 |
               GPIO_PUPDR_PUPD8_1 |
               GPIO_PUPDR_PUPD10_1);                                                            /* Enable pull down. */

    CLEAR_BIT(GPIOA->OTYPER,
              GPIO_OTYPER_OT7_Msk |
              GPIO_OTYPER_OT8_Msk |
              GPIO_OTYPER_OT10_Msk);                                                            /* Set output type to push-pull. */

    MODIFY_REG(GPIOA->OSPEEDR,
               GPIO_OSPEEDR_OSPEED7_Msk |
               GPIO_OSPEEDR_OSPEED8_Msk |
               GPIO_OSPEEDR_OSPEED10_Msk,
               (2UL << GPIO_OSPEEDR_OSPEED7_Pos) |
               (2UL << GPIO_OSPEEDR_OSPEED8_Pos) |
               (2UL << GPIO_OSPEEDR_OSPEED10_Pos));                                             /* Set pin speed to high. */

    MODIFY_REG(GPIOA->AFR[0],
               GPIO_AFRL_AFSEL7_Msk,
               6UL << GPIO_AFRL_AFSEL7_Pos);                                                    /* Set alternate function to AF6. */

    MODIFY_REG(GPIOA->AFR[1],
               GPIO_AFRH_AFSEL8_Msk |
               GPIO_AFRH_AFSEL10_Msk,
               (6UL << GPIO_AFRH_AFSEL8_Pos) |
               (6UL << GPIO_AFRH_AFSEL10_Pos));                                                 /* Set alternate function to AF6. */

    MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODE1_Msk, 2UL << GPIO_MODER_MODE1_Pos);                /* Set mode to alternate function. */
    MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPD1_Msk, GPIO_PUPDR_PUPD1_1);                         /* Enable pull down. */
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT1_Msk);                                              /* Set output type to push-pull. */
    MODIFY_REG(GPIOB->OSPEEDR,
               GPIO_OSPEEDR_OSPEED1_Msk,
               2UL << GPIO_OSPEEDR_OSPEED1_Pos);                                                /* Set pin speed to high. */

    MODIFY_REG(GPIOB->AFR[0], GPIO_AFRL_AFSEL1_Msk, 6UL << GPIO_AFRL_AFSEL1_Pos);               /* Set alternate function to AF6. */

    /***********************************************************************************
     * Timer TIM1 configuration:
     *  - CH1/CH1N: Asymmetric PWM mode 2
     *  - CH2:      No output PWM mode 2
     *  - CH3/CH3N: Asymmetric PWM mode 2
     *  - CH4:      No output PWM mode 2
     ***********************************************************************************/
    /* Reset timer 1 periphery. */
    SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                           /* Force TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                         /* Release TIM1 peripheral reset. */
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN_Msk);                                              /* Enable clocks for TIM1. */

    SET_BIT(TIM1->CR1, TIM_CR1_CMS_1);                                                          /* Center aligned mode 2. */
    SET_BIT(TIM1->CCMR1,
            TIM_CCMR1_OC1M_3 |
            TIM_CCMR1_OC1M_2 |
            TIM_CCMR1_OC1M_1 |
            TIM_CCMR1_OC1M_0 |                                                                  /* OC1: Asymmetric PWM mode 2. */
            TIM_CCMR1_OC2M_2 |
            TIM_CCMR1_OC2M_1 |
            TIM_CCMR1_OC2M_0);                                                                  /* OC2: PWM mode 2. */

    SET_BIT(TIM1->CCMR2,
            TIM_CCMR2_OC3M_3 |
            TIM_CCMR2_OC3M_2 |
            TIM_CCMR2_OC3M_1 |
            TIM_CCMR2_OC3M_0 |                                                                  /* OC3: Asymmetric PWM mode 2. */
            TIM_CCMR2_OC4M_2 |
            TIM_CCMR2_OC4M_1 |
            TIM_CCMR2_OC4M_0);                                                                  /* OC4: PWM mode 2. */

    SET_BIT(TIM1->DIER, TIM_DIER_UIE);                                                          /* Enable update interrupt flag generation. */
    NVIC_SetPriority(TIM1_UP_TIM16_IRQn, CONFIG_PCC_IRQ_PRIORITY_d);                            /* Set correct NVIC interrupt priority. */
}

/**
 * @brief Start routine for phase shifted PWM generation with complementary signals.
 * @details
 *  - calculates and writes correct registers for reference frequency and phase shift
 *  - enables pre-load on ARR, CCR1, CCR2, CCR3 and CCR4
 *  - enables CC1, CC1N, CC3, CC3N
 *  - enables master output enable
 *  - clears timer count
 *  - enables timer and its overflow interrupt
 */
static void PCC_FC_FullBridgePhaseShiftedPWM_Start_v(void)
{
    PCC_CheckAndCorrectIncorrectParameters_v();

    /* Copy set parameters to actual parameters. */
    _s_freq__Hz__f32            = _SET_FREQ_d;
    _s_phase_shift__deg__f32    = _SET_PHASE_SHIFT_d;
    _s_dead_time__s__f32        = _SET_DEADTIME_d;

    /* Dead time calculation and write to dead time register. */
    MODIFY_REG(TIM1->BDTR,
               TIM_BDTR_DTG_Msk,
               (u32)UTIL_TIM_SetMinumumDeadTimeValue_u8((f32)SYS_APB1_CLOCK_FREQ__Hz__d, _s_dead_time__s__f32));

    /* Calculate and write register values for set frequency. */
    UTIL_TIM_SetTimerOverflowFrequency_v(
            (f32)SYS_APB1_CLOCK_FREQ__Hz__d,
            UTIL_TIM_UP_DOWN_COUNTER_MODE_FREQ_MULTIPLIER_df32 * _s_freq__Hz__f32,
            &TIM1->ARR,
            &TIM1->PSC
            );

    /* Set correct PWM duty and phase shift. */
    TIM1->CCR1 = 0;
    TIM1->CCR2 = TIM1->ARR - 1;

    u32 shift_u32 = (u32)(((f32)TIM1->ARR * (_s_phase_shift__deg__f32 / 180.0f)) + 0.5f);

    if(shift_u32 != (u32)0)
    {
        TIM1->CCR3 = shift_u32 - 1;
        TIM1->CCR4 = TIM1->ARR - shift_u32;
    }
    else
    {
        TIM1->CCR3 = 0;
        TIM1->CCR4 = TIM1->ARR;
    }

    SET_BIT(TIM1->CCMR1, TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);                                    /* Enable pre-load for 0C1 and OC2. */
    SET_BIT(TIM1->CCMR2, TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE);                                    /* Enable pre-load for 0C3 and OC4. */
    SET_BIT(TIM1->CCER,
            TIM_CCER_CC1E    |                                                                  /* Enable PWM channel: CH1. */
            TIM_CCER_CC1NE   |                                                                  /* Enable PWM channel: CH1N. */
            TIM_CCER_CC3E    |                                                                  /* Enable PWM channel: CH3. */
            TIM_CCER_CC3NE);                                                                    /* Enable PWM channel: CH3N. */

    CLEAR_REG(TIM1->CNT);                                                                       /* Clear timer counter. */
    NVIC_ClearPendingIRQ(TIM1_UP_TIM16_IRQn);                                                   /* Clear pending interrupt in NVIC. */
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);                                                         /* Enable update interrupt in NVIC. */
    SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);                                                          /* Enable master update. */
    SET_BIT(TIM1->CR1,
            TIM_CR1_ARPE |                                                                      /* Enable pre-load for ARR. */
            TIM_CR1_CEN);                                                                       /* Start the timer. */
}

/**
 * @brief Stop routine for phase shifted PWM generation with complementary signals.
 * @details
 *  - disables pre-load on ARR, CCR1, CCR2, CCR3 and CCR4
 *  - disables CC1, CC1N, CC3, CC3N
 *  - disables master output enable
 *  - clears timer count
 *  - disables timer and its overflow interrupt
 */
static void PCC_FC_FullBridgePhaseShiftedPWM_Stop_v(void)
{
    NVIC_ClearPendingIRQ(TIM1_UP_TIM16_IRQn);                                                   /* Clear pending interrupt in NVIC. */
    NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);                                                         /* Disable update interrupt in NVIC. */
    CLEAR_BIT(TIM1->BDTR, TIM_BDTR_MOE);                                                        /* Disable master update. */
    CLEAR_BIT(TIM1->CR1,
            TIM_CR1_ARPE |                                                                      /* Disable pre-load for ARR. */
            TIM_CR1_CEN);                                                                       /* Stop the timer. */

    CLEAR_REG(TIM1->CNT);                                                                       /* Clear timer counter. */

    CLEAR_BIT(TIM1->CCMR1, TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);                                  /* Disable pre-load for 0C1 and OC2. */
    CLEAR_BIT(TIM1->CCMR2, TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE);                                  /* Disable pre-load for 0C3 and OC4. */
    CLEAR_BIT(TIM1->CCER,
            TIM_CCER_CC1E    |                                                                  /* Disable PWM channel: CH1. */
            TIM_CCER_CC1NE   |                                                                  /* Disable PWM channel: CH1N. */
            TIM_CCER_CC3E    |                                                                  /* Disable PWM channel: CH3. */
            TIM_CCER_CC3NE);                                                                    /* Disable PWM channel: CH3N. */
}

/**
 * @brief Update interrupt handler for phase shifted PWM generation with complementary signals.
 * @details Writes new frequency, phase shift and dead time whenever they are changed
 */
static void PCC_FC_FullBridgePhaseShiftedPWM_IrqHandler_v(void)
{
    u32 shift_u32;

    /* Check if new PWM frequency was set. */
    if(_s_freq__Hz__f32 != _SET_FREQ_d)
    {
        PCC_CheckAndCorrectIncorrectParameters_v();

        /* Calculate and write register values for set frequency. */
        UTIL_TIM_SetTimerOverflowFrequency_v(
                (f32)SYS_APB1_CLOCK_FREQ__Hz__d,
                UTIL_TIM_UP_DOWN_COUNTER_MODE_FREQ_MULTIPLIER_df32 * _SET_FREQ_d,
                &TIM1->ARR,
                &TIM1->PSC
                );

        /* Set correct PWM duty and phase shift. */
        TIM1->CCR1 = 0;
        TIM1->CCR2 = TIM1->ARR - 1;

        shift_u32 = (u32)(((f32)TIM1->ARR * (_SET_PHASE_SHIFT_d / 180.0f)) + 0.5f);

        if(shift_u32 != (u32)0)
        {
            TIM1->CCR3 = shift_u32 - 1;
            TIM1->CCR4 = TIM1->ARR - shift_u32;
        }
        else
        {
            TIM1->CCR3 = 0;
            TIM1->CCR4 = TIM1->ARR;
        }
        /* Copy set parameters to actual parameters. */
        _s_freq__Hz__f32            = _SET_FREQ_d;
        _s_phase_shift__deg__f32    = _SET_PHASE_SHIFT_d;
    }

    /* Check if new PWM phase shift was set. */
    else if(_s_phase_shift__deg__f32 != _SET_PHASE_SHIFT_d)
    {
        PCC_CheckAndCorrectIncorrectParameters_v();

        shift_u32 = (u32)(((f32)TIM1->ARR * (_SET_PHASE_SHIFT_d / 180.0f)) + 0.5f);

        if(shift_u32 != (u32)0)
        {
            TIM1->CCR3 = shift_u32 - 1;
            TIM1->CCR4 = TIM1->ARR - shift_u32;
        }
        else
        {
            TIM1->CCR3 = 0;
            TIM1->CCR4 = TIM1->ARR;
        }

        /* Copy set parameters to actual parameters. */
        _s_phase_shift__deg__f32 = _SET_PHASE_SHIFT_d;
    }

    /* Check if new dead time was set. */
    if(_s_dead_time__s__f32 != _SET_DEADTIME_d)
    {
        PCC_CheckAndCorrectIncorrectParameters_v();

        /* Dead time calculation and write to dead time register. */
        MODIFY_REG(TIM1->BDTR,
                   TIM_BDTR_DTG_Msk,
                   (u32)UTIL_TIM_SetMinumumDeadTimeValue_u8((f32)SYS_APB1_CLOCK_FREQ__Hz__d, _SET_DEADTIME_d));

        /* Copy set parameters to actual parameters. */
        _s_dead_time__s__f32 = _SET_DEADTIME_d;
    }

    /* Clear interrupt flag. */
    CLEAR_BIT(TIM1->SR, TIM_SR_UIF);
}

/**
 *  @brief De-initialization routine for phase shifted PWM generation with complementary signals.
 *  @details
 *      TIM1 is reset.
 *      GPIO pins are set to analog.
 */
static void PCC_FC_FullBridgePhaseShiftedPWM_DeInit_v(void)
{
    /* Set pin modes to analog. */
    SET_BIT(GPIOA->MODER, GPIO_MODER_MODE7_Msk | GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE10_Msk);
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE1_Msk);

    /* Reset timer 1 periphery. */
    SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                           /* Force TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST_Msk);                                         /* Release TIM1 peripheral reset. */
    CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN_Msk);
}
