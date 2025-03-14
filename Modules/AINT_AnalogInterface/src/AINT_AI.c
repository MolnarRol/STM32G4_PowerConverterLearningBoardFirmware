/*
 * AINT_AI.c
 *
 *  Created on: Nov 24, 2024
 *      Author: roland
 */
#include <AINT_AI.h>

/*
 *  *      AI1 - PA1
 *      AI2 - PA3
 * */

inline void AINT_InitializeInterfaces_v(void)
{
    /*
     * Analog voltage reference buffer setup:
     *  - Internal buffer is used.
     *  - Outputs reference voltage on VREF pin of the MCU.
     *  - Reference voltage is set to be around 2048 mV
     */
    VREFBUF->CSR    = 0UL;                              /* Set register to 0. Default: 0x2. */
    VREFBUF->CSR    |= VREFBUF_CSR_ENVR;                /* Internal voltage reference mode. */
    VREFBUF->CSR	|= 0UL << VREFBUF_CSR_VRS_Pos;		/* Reference voltage level: 2048 mV. */
    while((VREFBUF->CSR & VREFBUF_CSR_VRR) == 0)        /* Wait for reference voltage to stabilize at selected level. */
    {
        __NOP();
    }


}

static inline void AINT_InitAnalogSamplingTimers(void)
{
	RCC->AHB1ENR	|= RCC_APB1ENR1_TIM6EN | RCC_APB1ENR1_TIM7EN;	/* Enable clocks for timer 6 and timer 7. */

}

static inline void AINT_AI_InitializeInputs(void)
{
	/*
	 * AI1 - PA1 -> ADC12_IN2
	 * AI2 - PA3 -> ADC1_IN4
	 * */
	RCC->CCIPR	|=	RCC_CCIPR_ADC12SEL_1;

}
