/*
 * pcc.c
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */
#include <pcc.h>


void PCC_Handler(void)
{

}

void PCC_SetGateDriverPowerStates(PCC_driver_enable_union enable_states_u)
{
	/* PC0 - PC3 */
	GPIOC->BSRR				|=	(1UL << (16UL * !enable_states_u.drivers_s.gd1_f1))			|
								(1UL << (1UL + 16UL * !enable_states_u.drivers_s.gd2_f1))	|
								(1UL << (2UL + 16UL * !enable_states_u.drivers_s.gd3_f1))	|
								(1UL << (3UL + 16UL * !enable_states_u.drivers_s.gd4_f1));
	/* PB2 */
	GPIOB->BSRR				|= 1UL << (2UL + 16UL * !enable_states_u.drivers_s.gd5_f1);

	/* PA5 */
	GPIOA->BSRR				|= 1UL << (5UL + 16UL * !enable_states_u.drivers_s.gd6_f1);
}
