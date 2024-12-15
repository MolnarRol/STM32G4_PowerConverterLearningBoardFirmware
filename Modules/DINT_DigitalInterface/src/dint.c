/*
 * dint.c
 *
 *  Created on: Nov 10, 2024
 *      Author: roland
 */
#include "dint.h"

void DINT_Handler(void)
{
    DI_HandleInputs();
}

