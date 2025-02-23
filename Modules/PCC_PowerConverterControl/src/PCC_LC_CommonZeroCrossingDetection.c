/*
 * PCC_LC_CommonZeroCrossingDetection.c
 *
 *  Created on: Feb 21, 2025
 *      Author: molnar
 */
#include "PCC_LC_CommonZeroCrossingDetection.h"

volatile f32                                        PCC_LC_ZC_LineFreq__Hz__f32                     = 0.0f;
static PCC_LC_ZC_TopologyControlCallbacks_struct*   s_PCC_LC_ZC_TopologyCallbacks_ps                = NULL;
static u32                                          s_PCC_LC_ZC_MaximumFrequencyCompareValue_u32    = (u32)0;
static boolean                                      s_PCC_LC_ZC_LineFreqStable_b                    = False_b;

void PCC_LC_ZC_Init_v(PCC_LC_ZC_TopologyControlCallbacks_struct* control_ps, u32 edge_sensitivity_u32)
{
    s_PCC_LC_ZC_TopologyCallbacks_ps = control_ps;

    /* Timer TIM15 configuration. */
    RCC->APB2ENR                |= RCC_APB2ENR_TIM15EN;                                             /* Enable clocks for TIM15 */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM15RST;                                           /* Force reset to defaults on TIM15. */
    RCC->APB2RSTR               &= ~RCC_APB2RSTR_TIM15RST;                                          /* Release reset on TIM15. */

    UTIL_TIM_SetTimerOverflowFrequency_v(   (f32)SYS_APB2_CLOCK_FREQ__Hz__d,
                                            PCC_LC_ZC_MINIMUM_INPUT_FREQUENCY_df32 *
                                            (1.0f + (f32)edge_sensitivity_u32),
                                            &TIM15->ARR,
                                            &TIM15->PSC);

    s_PCC_LC_ZC_MaximumFrequencyCompareValue_u32 = (u32)((((f32)TIM15->ARR + 1.0f) * PCC_LC_ZC_MINIMUM_INPUT_FREQUENCY_df32)  / PCC_LC_ZC_MAXIMUM_INPUT_FREQUENCY_df32);

    TIM15->CR1                  |= TIM_CR1_OPM;                                                     /* Enable one pulse mode - stop counter on overflow. */

    TIM15->CCER                 |= (TIM_CCER_CC1P | TIM_CCER_CC1NP) * edge_sensitivity_u32;
    TIM15->CCMR1                |= TIM_CCMR1_CC1S_0;

    TIM15->SMCR                 |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 |                                  /* Trigger on input capture edge. */
                                   TIM_SMCR_SMS_3;                                                  /* Reset + Trigger timer on input. */

    TIM15->DIER                 |= TIM_DIER_CC1IE;                                                  /* Enable interrupt generation on capture/compare 1. */
}

void PCC_LC_ZC_DeInit_v(void)
{
    PCC_LC_ZC_Disable_v();

    /* Reset timer to defaults. */
    RCC->APB2ENR                |= RCC_APB2ENR_TIM15EN;                                             /* Enable clocks for TIM15 */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM15RST;                                           /* Force reset to defaults on TIM15. */
    RCC->APB2RSTR               &= ~RCC_APB2RSTR_TIM15RST;                                          /* Release reset on TIM15. */
}

static inline f32 PCC_LC_ZC_CalculateFrequencyFromCaptureValue__Hz__f32(u16 capture_val_u16)
{
    if(capture_val_u16 == 0) return 0.0f;
    f32 unrounded_freq__Hz__f32;
    f32 return_freq__Hz__f32;

    unrounded_freq__Hz__f32 = (((f32)TIM15->ARR + 1.0f) * PCC_LC_ZC_MINIMUM_INPUT_FREQUENCY_df32) / (f32)capture_val_u16;
    return_freq__Hz__f32 = (f32)((u32)(unrounded_freq__Hz__f32 * 100.0f + 0.5f)) / 100.0f;
    return return_freq__Hz__f32;
}


void PCC_LC_ZC_IrqHandler_v(void)
{
    static u8 good_input_freq_count_u8 = 0;
    if(s_PCC_LC_ZC_LineFreqStable_b)
    {
        s_PCC_LC_ZC_TopologyCallbacks_ps->operation_handler_pfv();
    }

    if(s_PCC_LC_ZC_MaximumFrequencyCompareValue_u32 < TIM15->CCR1)
    {
        if(good_input_freq_count_u8 < 10)
        {
            good_input_freq_count_u8 += (u8)1;
        }
        else
        {
            PCC_LC_ZC_LineFreq__Hz__f32 = PCC_LC_ZC_CalculateFrequencyFromCaptureValue__Hz__f32(TIM15->CCR1);
            s_PCC_LC_ZC_TopologyCallbacks_ps->start_pulses_pfv();
            s_PCC_LC_ZC_LineFreqStable_b = True_b;
        }
    }
    else
    {
        good_input_freq_count_u8 = (u8)0;
        s_PCC_LC_ZC_TopologyCallbacks_ps->inhibit_pulses_pfv();
        s_PCC_LC_ZC_LineFreqStable_b = False_b;
    }
    TIM15->SR   &= ~TIM_SR_CC1IF;
}

void PCC_LC_ZC_Enable_v(void)
{
    GPIOB->BSRR |= GPIO_BSRR_BS15;                                                                  /* Enable zero crossing circuit. */
    TIM15->SR &= ~TIM_SR_CC1IF;                                                                     /* Clear capture/compare 1 interrupt flag. */
    NVIC_ClearPendingIRQ(TIM1_BRK_TIM15_IRQn);                                                      /* Clear pending interrupt request on TIM15 IRQ line. */
    NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);                                                            /* Enable interrupt request on TIM15 IRQ line. */
    TIM15->CCER |= TIM_CCER_CC1E;                                                                   /* Enable capture/compare channel 1. */
}

void PCC_LC_ZC_Disable_v(void)
{
    GPIOB->BSRR |= GPIO_BSRR_BR15;                                                                  /* Disable zero crossing circuit. */
    TIM15->CCER &= ~(TIM_CCER_CC1E);                                                                /* Disable capture/compare channel 1. */
    NVIC_DisableIRQ(TIM1_BRK_TIM15_IRQn);                                                           /* Disable interrupt request on TIM15 IRQ line. */
}
