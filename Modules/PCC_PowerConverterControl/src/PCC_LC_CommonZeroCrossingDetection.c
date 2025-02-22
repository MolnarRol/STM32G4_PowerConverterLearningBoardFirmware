/*
 * PCC_LC_CommonZeroCrossingDetection.c
 *
 *  Created on: Feb 21, 2025
 *      Author: molnar
 */
#include "PCC_LC_CommonZeroCrossingDetection.h"

static volatile f32 PCC_LC_ZC_LineFreq__Hz__f32 = 0.0f;
static PCC_LC_ZC_TopologyControlCallbacks_struct* s_PCC_LC_ZC_TopologyCallbacks_ps = NULL;
static u32 s_PCC_LC_ZC_MaximumFrequencyCompareValue_u32;

inline volatile f32* PCC_LC_ZC_GetLineFreq__Hz__pf32(void)
{
    return &PCC_LC_ZC_LineFreq__Hz__f32;
}

void PCC_LC_ZC_Init_v(void)
{
    s_PCC_LC_ZC_MaximumFrequencyCompareValue_u32 = (u32)((f32)ZC_TIMER_INPUT_INCREMENT_FREQ_Hz_def / ((f32)ZC_MAX_INPUT_FREQ_Hz_def * ZC_TIMER_PRESCALER_def));

    /* Timer TIM15 configuration. */
    RCC->APB2ENR                |= RCC_APB2ENR_TIM15EN;                                             /* Enable clocks for TIM15 */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM15RST;                                           /* Force reset to defaults on TIM15. */
    RCC->APB2RSTR               &= ~RCC_APB2RSTR_TIM15RST;                                          /* Release reset on TIM15. */

    TIM15->PSC                  = (u16)(ZC_TIMER_PRESCALER_def - 1UL);                              /* Set prescaler to calculated value. */
    TIM15->CR1                  |= TIM_CR1_OPM;                                                     /* Enable one pulse mode - stop counter on overflow. */

    LL_TIM_IC_SetPolarity(TIM15, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);                    /* Capture rising edge. LL_TIM_IC_POLARITY_BOTHEDGE / LL_TIM_IC_POLARITY_RISING */
    LL_TIM_IC_SetActiveInput(TIM15, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);

    LL_TIM_SetTriggerInput(TIM15, LL_TIM_TS_TI1FP1);                                                /* Trigger on input capture edge. */
    LL_TIM_SetSlaveMode(TIM15, LL_TIM_SLAVEMODE_COMBINED_RESETTRIGGER);                             /* Reset + Trigger timer on input. */

    TIM15->DIER                 |= TIM_DIER_CC1IE;                                                  /* Enable interrupt generation on capture/compare 1. */
}

static inline f32 PCC_LC_ZC_CalculateFrequencyFromCaptureValue__Hz__f32(u16 capture_val_u16)
{
    if(capture_val_u16 == 0) return 0.0f;
    return ((f32)ZC_TIMER_INPUT_INCREMENT_FREQ_Hz_def / (f32)((u32)ZC_TIMER_PRESCALER_def)) / (f32)capture_val_u16;
}

void PCC_LC_ZC_SetActiveLineCommutatedTopology_v(PCC_LC_ZC_TopologyControlCallbacks_struct* control_ps)
{
    assert_param(control_ps != NULL);
    s_PCC_LC_ZC_TopologyCallbacks_ps = control_ps;
}

void PCC_LC_ZC_IrqHandler_v(void)
{
    static u8 good_input_freq_count_u8 = 0;
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
            s_PCC_LC_ZC_TopologyCallbacks_ps->operation_handler_pfv();
        }
    }
    else
    {
        good_input_freq_count_u8 = (u8)0;
        s_PCC_LC_ZC_TopologyCallbacks_ps->inhibit_pulses_pfv();
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
