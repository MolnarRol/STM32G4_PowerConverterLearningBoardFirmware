/*
 * pui.c
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */
#include <pui.h>

#ifdef __cplusplus
extern "C"
{
#endif

void PUI_Init(void)
{
    /*
     *  Incremental encoder initialization.
     */
    RCC->APB1RSTR1  |= RCC_APB1RSTR1_TIM4RST;                                   /* Reset TIM4 to default. */
    RCC->APB1RSTR1  &= ~RCC_APB1RSTR1_TIM4RST;                                  /* Release reset of TIM4. */
    RCC->APB1ENR1   |= RCC_APB1ENR1_TIM4EN;                                     /* Enable clocks for TIM4 */

    TIM4->SMCR      |= TIM_SMCR_SMS_0;                                          /* Encoder mode 1 - Counter counts up/down on tim_ti1fp1 edge depending on tim_ti2fp2 level.  */

    TIM4->CCMR1     |=  TIM_CCMR1_CC1S_0 |                                      /* Set channel 1 as input. */
                        TIM_CCMR1_CC2S_0;                                       /* Set channel 2 as input. */

    TIM4->CCER      |=  TIM_CCER_CC1E |                                         /* Enable capture channel 1. */
                        TIM_CCER_CC2E ;                                         /* Enable capture channel 2. */

    GPIOB->AFR[0]   |= GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL7_1;                 /* Set alternate function 2 to pins PB6 and PB7. */
    GPIOB->MODER    &=  ~(GPIO_MODER_MODE6_Msk  |
                        GPIO_MODER_MODE7_Msk    |
                        GPIO_MODER_MODE13_Msk   );                              /* Clear any mode configuration on PB6, PB7 and PB13. */
    GPIOB->MODER    |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;                 /* Set GPIO pins PB6 and PB7 as alternate function pins. */

    TIM4->CR1       |= TIM_CR1_CEN;                                             /* Enable timer. */
}

void PUI_Handler(void)
{

}

#ifdef __cplusplus
}
#endif

