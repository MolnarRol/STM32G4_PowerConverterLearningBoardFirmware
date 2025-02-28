/*
 * TD_SPI.c
 *
 *  Created on: Feb 28, 2025
 *      Author: molnar
 */
#include "TD_SPI.h"

void TD_SPI_Init_v(void)
{
    SET_BIT(RCC->APB1RSTR1, RCC_APB1RSTR1_SPI3RST);                     /* Force reset for SPI3. */
    CLEAR_BIT(RCC->APB1RSTR1, RCC_APB1RSTR1_SPI3RST);                   /* Release reset for SPI3. */
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_SPI3EN);                        /* Enable clocks for SPI3. */



}
