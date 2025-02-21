/*
 * PCC_LC_CommonZeroCrossingDetection.c
 *
 *  Created on: Feb 21, 2025
 *      Author: molnar
 */


void PCC_LC_ZC_Init_v(void)
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
