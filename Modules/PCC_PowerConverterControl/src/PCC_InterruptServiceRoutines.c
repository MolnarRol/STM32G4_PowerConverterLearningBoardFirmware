/*
 * PCC_InterruptServiceRoutines.c
 *
 *  Created on: Feb 20, 2025
 *      Author: molnar
 */
#include "pcc.h"

void TIM1_UP_TIM16_IRQHandler(void)
{
    PCC_InterruptHandler_v();
    NVIC_ClearPendingIRQ(TIM1_UP_TIM16_IRQn);
}

void TIM1_BRK_TIM15_IRQHandler(void)
{
    PCC_InterruptHandler_v();
    NVIC_ClearPendingIRQ(TIM1_BRK_TIM15_IRQn);
}
