#include "PCC_private_interface.h"
#include "UTIL_public_interface.h"

static void PCC_FC_FullBridgePhaseShiftedPWM_Init_v(void);
static void PCC_FC_FullBridgePhaseShiftedPWM_Start_v(void);
static void PCC_FC_FullBridgePhaseShiftedPWM_ActiveHandling_v(void);
static void PCC_FC_FullBridgePhaseShiftedPWM_Stop_v(void);
static void PCC_FC_FullBridgePhaseShiftedPWM_DeInit_v(void);

const PCC_TopologyHandle_struct PCC_Topology_FullBridgePhaseShiftedPWM_s =
{
        .initialize_pfv     = PCC_FC_FullBridgePhaseShiftedPWM_Init_v,
        .start_pf           = PCC_FC_FullBridgePhaseShiftedPWM_Start_v,
        .active_handler_pfv = PCC_FC_FullBridgePhaseShiftedPWM_ActiveHandling_v,
        .stop_pfv           = PCC_FC_FullBridgePhaseShiftedPWM_Stop_v,
        .deinitalize_pfv    = PCC_FC_FullBridgePhaseShiftedPWM_DeInit_v,
        .driver_enable_u    =
                            {
                                .drivers_s =
                                {
                                        .gd1_f1 = 1,
                                        .gd2_f1 = 1,
                                        .gd3_f1 = 1,
                                        .gd4_f1 = 1,
                                        .gd5_f1 = 0,
                                        .gd6_f1 = 0
                                }
                            }
};

f32 PCC_FC_FullBridgePhaseShiftedPWM_freq__Hz__f32 = 1000.0f;
f32 PCC_FC_FullBridgePhaseShiftedPWM_duty__per_cent__f32 = 0.0f;

static void PCC_FC_FullBridgePhaseShiftedPWM_Init_v(void)
{
	/* TIM1 CH1 */
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;								        /* Enable clocks for TIM1 module. */

	/* GPIO pin configuration: PA8, PA10 - AF6 */
	GPIOA->MODER				&= (~GPIO_MODER_MODE7_Msk)  &
	                               (~GPIO_MODER_MODE8_Msk)  &
	                               (~GPIO_MODER_MODE10_Msk);			    /* Reset pin mode. */

	GPIOA->MODER				|= (2UL << GPIO_MODER_MODE7_Pos) |
                                   (2UL << GPIO_MODER_MODE8_Pos) |
                                   (2UL << GPIO_MODER_MODE10_Pos);		    /* Set mode to alternate function. */

	GPIOA->PUPDR        		|= GPIO_PUPDR_PUPD7_1 |
	                               GPIO_PUPDR_PUPD8_1 |
	                               GPIO_PUPDR_PUPD10_1;                     /* Enable pull down. */

	GPIOA->OTYPER				&= (~GPIO_OTYPER_OT7_Msk) |
	                               (~GPIO_OTYPER_OT8_Msk) |
	                               (~GPIO_OTYPER_OT10_Msk);			        /* Set output type to push-pull. */

	GPIOA->OSPEEDR				&= (~GPIO_OSPEEDR_OSPEED7_Msk) &
	                               (~GPIO_OSPEEDR_OSPEED8_Msk) &
	                               (~GPIO_OSPEEDR_OSPEED10_Msk);	        /* Reset pin speed. */

	GPIOA->OSPEEDR				|= (2UL << GPIO_OSPEEDR_OSPEED7_Pos) |
	                               (2UL << GPIO_OSPEEDR_OSPEED8_Pos) |
	                               (2UL << GPIO_OSPEEDR_OSPEED10_Pos);	    /* Set pin speed to high. */

	GPIOA->AFR[0]               &= (~GPIO_AFRL_AFSEL7_Msk);
	GPIOA->AFR[1]				&= (~GPIO_AFRH_AFSEL8_Msk) &
	                               (~GPIO_AFRH_AFSEL10_Msk);			    /* Reset alternate function. */

	GPIOA->AFR[0]               |= (6UL << GPIO_AFRL_AFSEL7_Pos);
	GPIOA->AFR[1]				|= (6UL << GPIO_AFRH_AFSEL8_Pos) |
	                               (6UL << GPIO_AFRH_AFSEL8_Pos);		        /* Set alternate function to AF6. */
	GPIOA->MODER                &= (~GPIO_MODER_MODE7_Msk)  &
	                                   (~GPIO_MODER_MODE8_Msk)  &
	                                   (~GPIO_MODER_MODE10_Msk);                /* Reset pin mode. */

	/* PB01 - AF6 */
	GPIOB->MODER                &= GPIO_MODER_MODE1_Msk;
    GPIOB->MODER                |= (2UL << GPIO_MODER_MODE1_Pos);          /* Set mode to alternate function. */
    GPIOB->PUPDR                |= GPIO_PUPDR_PUPD1_1;                     /* Enable pull down. */
    GPIOB->OTYPER               &= (~GPIO_OTYPER_OT1_Msk);                 /* Set output type to push-pull. */
    GPIOB->OSPEEDR              &= (~GPIO_OSPEEDR_OSPEED1_Msk);            /* Reset pin speed. */
    GPIOB->OSPEEDR              |= (2UL << GPIO_OSPEEDR_OSPEED1_Pos);      /* Set pin speed to high. */
    GPIOB->AFR[0]               &= (~GPIO_AFRL_AFSEL1_Msk);
    GPIOB->AFR[0]               |= (6UL << GPIO_AFRL_AFSEL1_Pos);


	/* Reset timer 1 periphery. */
	RCC->APB2RSTR               |= RCC_APB2RSTR_TIM1RST_Msk;                /* Force TIM1 peripheral reset. */
	RCC->APB2RSTR               &= ~(RCC_APB2RSTR_TIM1RST_Msk);             /* Release TIM1 peripheral reset. */
	RCC->APB2ENR                |= RCC_APB2ENR_TIM1EN_Msk;                  /* Enable clocks for TIM1. */

	/* Timer configuration. */
	TIM1->CCMR1                 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;     /* PWM mode 1. */
	TIM1->CCER                  |= TIM_CCER_CC1E;                           /* Enable CH1. */
	TIM1->CNT                   = 0;
}

static void PCC_FC_FullBridgePhaseShiftedPWM_Start_v(void)
{
    TIM1->BDTR                  |= TIM_BDTR_MOE;
    TIM1->CR1                   |= TIM_CR1_CEN;
}

static void PCC_FC_FullBridgePhaseShiftedPWM_ActiveHandling_v(void)
{
    UTIL_TIM_SetTimerOverflowFrequency_v(170.0e6f, PCC_FC_FullBridgePhaseShiftedPWM_freq__Hz__f32, &TIM1->ARR, &TIM1->PSC);
    TIM1->CCR1 = (u16)((PCC_FC_FullBridgePhaseShiftedPWM_duty__per_cent__f32 * (f32)TIM1->ARR)/100.0f);
}

static void PCC_FC_FullBridgePhaseShiftedPWM_Stop_v(void)
{
    TIM1->BDTR                  &= ~TIM_BDTR_MOE;
    TIM1->CR1                   &= ~TIM_CR1_CEN;
}

static void PCC_FC_FullBridgePhaseShiftedPWM_DeInit_v(void)
{
    GPIOA->MODER                |= GPIO_MODER_MODE8_Msk;                    /* Set pin mode to analog. */

    /* Reset timer 1 periphery. */
    RCC->APB2RSTR               |= RCC_APB2RSTR_TIM1RST_Msk;                /* Force TIM1 peripheral reset. */
    RCC->APB2RSTR               &= ~(RCC_APB2RSTR_TIM1RST_Msk);             /* Release TIM1 peripheral reset. */
    RCC->APB2ENR                |= RCC_APB2ENR_TIM1EN_Msk;                  /* Enable clocks for TIM1. */
}
