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

    SET_BIT(SPI3->CR2,
            (7UL << SPI_CR2_DS_Pos) |                                   /* Data size - 8 bit. */
            SPI_CR2_FRXTH);/* Not sure */

    SET_BIT(SPI3->CR1,
            SPI_CR1_SSM |
            SPI_CR1_SSI |
            SPI_CR1_SPE |
            (2UL << SPI_CR1_BR_Pos) |
            SPI_CR1_MSTR);

}

void TD_SPI_TransferInBlockingMode_v(u8* data_pu8, u32 n_of_bytes_u32)
{
    while(n_of_bytes_u32 > (u32)0)
    {
        if(n_of_bytes_u32 > 1U)
        {
            SPI3->DR = *(u16*)data_pu8;
            data_pu8 += sizeof(u16);
            n_of_bytes_u32 -= (u32)2;
        }
        else
        {
            *(u8*)&SPI3->DR = (u8)*(data_pu8++);
            n_of_bytes_u32 -= (u32)1;
        }
        while((SPI3->SR & SPI_SR_BSY))
            __NOP();

    }
}
