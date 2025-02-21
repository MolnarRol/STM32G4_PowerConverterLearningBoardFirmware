/*
 * pcc_line_commutated.c
 *
 *  Created on: Nov 17, 2024
 *      Author: roland
 */
#include "pcc_line_commutated.h"

#define ZC_CAPTURE_BUFFER_SIZE_def          32U
#define ZC_HALF_CAPTURE_BUFFER_SIZE_def_u16 (u16)(ZC_CAPTURE_BUFFER_SIZE_def / 2U)

static u16 ZC_DMA_input_capture_buffer_stat_arr_u16[ZC_CAPTURE_BUFFER_SIZE_def];

f32 PACZC_LineFrequency_Hz_f32;
u32 freq;
boolean freq_stabile_b = False_b;

f32 ZC_CalculateFrequencyFromCaptureValue_Hz_f32(u16 capture_val_u16)
{
    if(capture_val_u16 == 0) return 0.0f;
    return ((f32)ZC_TIMER_INPUT_INCREMENT_FREQ_Hz_def / (f32)((u32)ZC_TIMER_PRESCALER_def)) / (f32)capture_val_u16;
}

void PCC_LC_InitZeroCrossingDetection_v(void)
{
    /* Timer TIM15 configuration. */
    RCC->APB2ENR                |= RCC_APB2ENR_TIM15EN;                                             /* Enable clocks for TIM15 */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM15RST;                                           /* Force reset to defaults on TIM15. */
    RCC->APB2RSTR               &= ~RCC_APB2RSTR_TIM15RST;                                          /* Release reset on TIM15. */

    TIM15->PSC                  = (u16)(ZC_TIMER_PRESCALER_def - 1UL);                              /* Set prescaler to calculated value. */
    TIM15->CR1                  |= TIM_CR1_OPM;                                                     /* Enable one pulse mode - stop counter on overflow. */
    //TIM15->CR2                  |= TIM_CR2_MMS_2;
    LL_TIM_IC_SetPolarity(TIM15, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);                    /* Capture rising edge. LL_TIM_IC_POLARITY_BOTHEDGE / LL_TIM_IC_POLARITY_RISING */
    LL_TIM_IC_SetActiveInput(TIM15, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);

    LL_TIM_SetTriggerInput(TIM15, LL_TIM_TS_TI1FP1);                                                /* Trigger on input capture edge. */
    LL_TIM_SetSlaveMode(TIM15, LL_TIM_SLAVEMODE_COMBINED_RESETTRIGGER);                             /* Reset + Trigger timer on input. */

    TIM15->DIER                 |= TIM_DIER_CC1IE;                                                  /* Enable interrupt generation on capture/compare 1. */
}

inline void PCC_LC_ZeroCrossingDetection_Enable_v(void)
{
    GPIOB->BSRR |= GPIO_BSRR_BS15;                                                                  /* Enable zero crossing circuit. */
    TIM15->SR &= ~TIM_SR_CC1IF;                                                                     /* Clear capture/compare 1 interrupt flag. */
    NVIC_ClearPendingIRQ(TIM1_BRK_TIM15_IRQn);                                                      /* Clear pending interrupt request on TIM15 IRQ line. */
    NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);                                                            /* Enable interrupt request on TIM15 IRQ line. */
    TIM15->CCER |= TIM_CCER_CC1E;                                                                   /* Enable capture/compare channel 1. */
}

inline void PCC_LC_ZeroCrossingDetection_Disable_v(void)
{
    GPIOB->BSRR |= GPIO_BSRR_BR15;                                                                  /* Disable zero crossing circuit. */
    TIM15->CCER &= ~(TIM_CCER_CC1E);                                                                /* Disable capture/compare channel 1. */
    NVIC_DisableIRQ(TIM1_BRK_TIM15_IRQn);                                                           /* Disable interrupt request on TIM15 IRQ line. */
}

static f32 ZC_CalculateAvgFreq_Hz_f32(u16* buffered_capture_arr_u16)
{
    u8 capture_index_u8;
    u16 input_mean_u16;
    u32 input_sum_u32 = (u32)0;

    /* Calculate sum of capture values. */
    for(capture_index_u8 = (u8)0;
        capture_index_u8 < (u8)ZC_HALF_CAPTURE_BUFFER_SIZE_def_u16;
        capture_index_u8++)
    {
        input_sum_u32 += (u32)buffered_capture_arr_u16[capture_index_u8];                               /* Accumulate. */
    }
    input_mean_u16 = (u16)(input_sum_u32 / ZC_HALF_CAPTURE_BUFFER_SIZE_def_u16);                    /* Calculate mean from accumulated sum. */

    return ZC_CalculateFrequencyFromCaptureValue_Hz_f32(input_mean_u16);                            /* Return calculated frequency from mean input capture. */
}

void PCC_InitPulseTimer1_v(void)
{
    /* PWM output - PA8 */
    GPIOA->MODER        &= ~(GPIO_MODER_MODE8_Msk);                                 /* Reset mode on PA8. */
    GPIOA->MODER        |= (2UL << GPIO_MODER_MODE8_Pos);                           /* Set mode to alternate function. */
    GPIOA->AFR[1]       &= ~(GPIO_AFRH_AFSEL8_Msk);                                 /* Reset alternate function number. */
    GPIOA->AFR[1]       |= (6UL << GPIO_AFRH_AFSEL8_Pos);                           /* Set alternate function on PA8 to TIM1_CH1. */
    GPIOA->PUPDR        &= ~(GPIO_PUPDR_PUPD8_Msk);                                 /* Reset resistor pull-up/down. */
    GPIOA->PUPDR        |= GPIO_PUPDR_PUPD8_1;                                      /* Enable pull down. */
    GPIOA->OTYPER       &= ~(GPIO_OTYPER_OT8);                                      /* Output push-pull on PA8. */

    RCC->APB2RSTR       |= RCC_APB2RSTR_TIM1RST_Msk;                                /* Force TIM1 peripheral reset. */
    RCC->APB2RSTR       &= ~(RCC_APB2RSTR_TIM1RST_Msk);                             /* Release TIM1 peripheral reset. */
    RCC->APB2ENR        |= RCC_APB2ENR_TIM1EN_Msk;                                  /* Enable clocks for TIM1 */

    TIM1->CR1           |= TIM_CR1_OPM;                                             /* Set one pulse mode. */

    TIM1->CCMR1         |=                                        /* Output compare 1 fast enable. */
                           TIM_CCMR1_OC1M_3 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0 | /* Combined PWM mode 2 */
                           TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;                     /* PWM mode 1. */
    TIM1->CCER          |= TIM_CCER_CC1E | TIM_CCER_CC2E;                           /* Enable CH1 and CH2. */

    TIM1->PSC           = 169;
    TIM1->ARR           = 20000;

/* Slave mode selection and trigger:
 *  - Slave mode selection - Combined reset + trigger mode.
 *  - Trigger selection - ITR6 (TIM15 TRGO)
 */
    TIM1->SMCR          |= TIM_SMCR_SMS_3 | TIM_SMCR_TS_3 | TIM_SMCR_TS_1;

    TIM1->BDTR          |= TIM_BDTR_MOE;                                            /* Master output enable */
}

//void TIM1_BRK_TIM15_IRQHandler(void)
//{
//    if((TIM15->SR & TIM_SR_CC1IF) != 0)
//    {
//        PACZC_LineFrequency_Hz_f32 = ZC_CalculateFrequencyFromCaptureValue_Hz_f32(TIM15->CCR1);
//        TIM15->SR &= ~(TIM_SR_CC1IF);
//    }
//    NVIC_ClearPendingIRQ(TIM1_BRK_TIM15_IRQn);
//}

void DMA1_Channel1_IRQHandler(void)
{
    if(LL_DMA_IsActiveFlag_HT1(DMA1))
    {
        PACZC_LineFrequency_Hz_f32 = ZC_CalculateAvgFreq_Hz_f32(&ZC_DMA_input_capture_buffer_stat_arr_u16[0]);
        DMA1->IFCR |= DMA_IFCR_CHTIF1;
    }
    if(LL_DMA_IsActiveFlag_TC1(DMA1))
    {
        PACZC_LineFrequency_Hz_f32 = ZC_CalculateAvgFreq_Hz_f32(&ZC_DMA_input_capture_buffer_stat_arr_u16[16]);
        DMA1->IFCR |= DMA_IFCR_CTCIF1;
    }
    freq = PACZC_LineFrequency_Hz_f32 * 1000.0f;

    DMA1->IFCR |= DMA_IFCR_CGIF1;
    NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);
}
