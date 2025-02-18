/*
 * uart_api.c
 *
 *  Created on: Feb 18, 2025
 *      Author: molnar
 */

#include "uart_api.h"

#define UART_API_RS485_BAUD_RATE_d              115200UL

void UART_API_RS485_Init_v(void)
{
    /* Reset USART1 periphery. */
    RCC->APB2RSTR           |=  RCC_APB2RSTR_USART1RST;
    RCC->APB2RSTR           &=  ~RCC_APB2RSTR_USART1RST_Msk;

    /* Enable clocks for USART1. */
    RCC->APB2ENR            |=  RCC_APB2ENR_USART1EN;

    USART1->CR1             |=  (4UL << USART_CR1_DEAT_Pos) |
                                (4UL << USART_CR1_DEDT_Pos) |
                                USART_CR1_TE                |           /* Enable transmitter. */
                                USART_CR1_RE;                           /* Enable receiver. */
    USART1->CR3             |= USART_CR3_DEM;                           /* Enable hardware data enable pin. */
    USART1->BRR             = 1476;                                     /* Baud: 115200 */

    USART1->CR1             |= USART_CR1_UE;
}
