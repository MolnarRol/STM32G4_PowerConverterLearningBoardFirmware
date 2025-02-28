/*
 * isr_handlers.c
 *
 *  Created on: Feb 28, 2025
 *      Author: molnar
 */
#include <stm32g4xx.h>
#include <app_types.h>

static u32 irq_count;

void EXTI15_10_IRQHandler(void)
{
    if(READ_BIT(EXTI->PR1, EXTI_PR1_PIF12) != 0){
//        PUI_IrqButtonIsrHandler_v(&PUI_StartStopBtn_s);
        SET_BIT(EXTI->PR1, EXTI_PR1_PIF12);
    }

    if(READ_BIT(EXTI->PR1, EXTI_PR1_PIF15) != 0){
//        PUI_IrqButtonIsrHandler_v(&PUI_StartStopBtn_s);
        irq_count++;
        SET_BIT(EXTI->PR1, EXTI_PR1_PIF15);
    }

    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
}

