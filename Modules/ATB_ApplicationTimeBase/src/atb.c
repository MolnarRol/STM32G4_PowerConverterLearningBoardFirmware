/*
 * ATB.c
 *
 *  Created on: Dec 15, 2024
 *      Author: molnar
 */
#include <atb.h>


void ATB_Init_v(void)
{
	//RCC->APB1ENR		|= RCC_APB1ENR1_LPTIM1EN;						/* Enable clock for low power timer. */
	UTIL_TIM_SetTimerOverflowFrequency_v(170.0e6f, 10000.0f, &LPTIM1->ARR, &LPTIM1->ARR);
}
