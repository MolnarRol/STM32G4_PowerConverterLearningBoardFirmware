/*
 * pui.c
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */
#include <pui.h>

#ifdef __cplusplus
extern "C"
{
#endif

u32 dbg_pui_enc_cnt_u32;

PUI_IrqButton_struct PUI_StartStopBtn_s =
{
    .port_ps                                        = GPIOB,
    .pin_number_u32                                 = 12,
    .long_btn_press_minimum_hold_time__ticks__u32   = ATB__ms__TO__ticks__du32(1500)

};

void PUI_IrqButtonIsrHandler_v(PUI_IrqButton_struct* btn_handle_ps)
{
    /* Button down was detected. */
    if((btn_handle_ps->port_ps->IDR & (1UL << btn_handle_ps->pin_number_u32)) == (u32)0 )
    {
        btn_handle_ps->last_btn_down__ticks__u32 = ATB_GetCurrentTicks_u32();                                   /* Mark button down time stamp. */
        btn_handle_ps->state_b = True_b;                                                                        /* Write new state. */
    }

    /* Button up was detected, */
    else
    {
        btn_handle_ps->last_btn_up__ticks__u32 = ATB_GetCurrentTicks_u32();                                     /* Mark button up time stamp. */
        btn_handle_ps->state_b = False_b;                                                                       /* Write new state. */
    }

    /* Check condition for short button press. */
    if((btn_handle_ps->last_btn_up__ticks__u32 - btn_handle_ps->last_btn_down__ticks__u32) <
        btn_handle_ps->long_btn_press_minimum_hold_time__ticks__u32)
    {
        /* Call short press callback if callback was registered. */
        if(btn_handle_ps->btn_short_press_callback_pvf != NULL)
            btn_handle_ps->btn_short_press_callback_pvf();
    }
    btn_handle_ps->long_press_handled_b = False_b;
}

void PUI_IrqButtonLongPressHandler_v(PUI_IrqButton_struct* btn_handle_ps)
{
    u32 actual_time__ticks__u32;                                                                                /* Actual time time stamp variable. */

    /* Check conditions for handling long press. Button must be held in down state.
     * long_press_handled_b is checked to prevent cyclic long press callback calling while the button is held down.*/
    if(btn_handle_ps->state_b && !btn_handle_ps->long_press_handled_b)
    {
        actual_time__ticks__u32 = ATB_GetCurrentTicks_u32();                                                    /* Get actual time stamp. */

        /* Check if enough time has elapsed for long press. */
        if((actual_time__ticks__u32 - btn_handle_ps->last_btn_down__ticks__u32) >
           btn_handle_ps->long_btn_press_minimum_hold_time__ticks__u32)
        {
            btn_handle_ps->long_press_handled_b = True_b;                                                       /* Write flag variable to prevent cyclic behavior. */

            /* If long press callback was registered call the callback. */
            if(btn_handle_ps->btn_long_press_callback_pvf != NULL)
                btn_handle_ps->btn_long_press_callback_pvf();
        }
    }
}

void PUI_Init(void)
{
    /**********************************************************************************************************************
     * Incremental encoder initialization.
     *  - Button:   PB5     [input]
     *  - ENC A:    PB6     [AF2]
     *  - ENC B:    PB7     [AF2]
     **********************************************************************************************************************/
    /* Rotary encoder GPIO pin configuration. */
    MODIFY_REG(GPIOB->MODER,
               GPIO_MODER_MODE5_Msk |                                               /* Set PB5 mode to input. */
               GPIO_MODER_MODE6_Msk |
               GPIO_MODER_MODE7_Msk,
               (2UL << GPIO_MODER_MODE6_Pos) |                                      /* Set PB6 mode to alternate function. */
               (2UL << GPIO_MODER_MODE7_Pos));                                      /* Set PB7 mode to alternate function. */

    MODIFY_REG(GPIOB->AFR[0],
               GPIO_AFRL_AFSEL6_Msk |
               GPIO_AFRL_AFSEL7_Msk,
               (2UL << GPIO_AFRL_AFSEL6_Pos) |                                      /* Set PB6 alternate function to AF2. */
               (2UL << GPIO_AFRL_AFSEL7_Pos));                                      /* Set PB7 alternate function to AF2. */

    /* Rotary encoder timer configuration. */
    SET_BIT(RCC->APB1RSTR1, RCC_APB1RSTR1_TIM4RST);                                 /* Reset TIM4 to default. */
    CLEAR_BIT(RCC->APB1RSTR1, RCC_APB1RSTR1_TIM4RST);                               /* Release reset of TIM4 */
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_TIM4EN);                                    /* Enable clocks for TIM4 */
    SET_BIT(TIM4->SMCR,
            TIM_SMCR_SMS_0 |
            TIM_SMCR_SMS_1 |
            TIM_SMCR_SMS_2 |
            TIM_SMCR_SMS_3);                                                        /* Quadrature encoder mode: x1 mode, counting on tim_ti2fp2 edges only, edge sensitivity is set by CC2P. */

    SET_BIT(TIM4->CCMR1,
            TIM_CCMR1_CC1S_0 |                                                      /* Set channel 1 as input. */
            TIM_CCMR1_CC2S_0);                                                      /* Set channel 2 as input. */

   SET_BIT(TIM4->CCER,
           TIM_CCER_CC1E |                                                          /* Enable capture channel 1. */
           TIM_CCER_CC2E);                                                          /* Enable capture channel 2. */

   SET_BIT(TIM4->CR1, TIM_CR1_CEN);                                                 /* Enable timer. */

    /**********************************************************************************************************************
    * External button with EXTI.
    *  - PB12
    **********************************************************************************************************************/
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);                                    /* Enable clocks for SYSCFG. */
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODE12_Msk);                                 /* Set GPIO pin mode to input. */
    SET_BIT(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI12_PB);                           /* Enable EXTI request from PB12 line. */
    SET_BIT(EXTI->IMR1, EXTI_IMR1_IM12);                                            /* Unmask interrupt in EXTI. */
    SET_BIT(EXTI->RTSR1, EXTI_RTSR1_RT12);                                          /* Enable rising trigger. */
    SET_BIT(EXTI->FTSR1, EXTI_FTSR1_FT12);                                          /* Enable falling trigger. */
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);                                           /* Clear potentionally pending interrupt in NVIC. */
    NVIC_SetPriority(EXTI15_10_IRQn, 0);                                            /* Set highest priority to this interrupt. Rationale: Button is used also as on/off. */
    NVIC_EnableIRQ(EXTI15_10_IRQn);                                                 /* Enable interrupt in NVIC. */

}

void PUI_RotaryEncoderReadCallback_v(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static u16 s_prev_enc_cnt_u16 = 0;
    data->enc_diff = (s16)TIM4->CNT - (s16)s_prev_enc_cnt_u16;
    s_prev_enc_cnt_u16 = TIM4->CNT;
    if((GPIOB->IDR & (1UL << 5)) == (u32)0) data->state = LV_INDEV_STATE_PRESSED;
    else data->state = LV_INDEV_STATE_RELEASED;
}

void PUI_PushBtnReadCallback_v(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    if(PUI_StartStopBtn_s.state_b)  data->state = LV_INDEV_STATE_PRESSED;
    else                            data->state = LV_INDEV_STATE_RELEASED;
    data->btn_id = 0;
}

void PUI_Handler(void)
{
#if 1
    if((GPIOB->IDR & (1UL << 5)) == (u32)0 )
    {
        CLEAR_REG(TIM4->CNT);
    }
    dbg_pui_enc_cnt_u32 = TIM4->CNT;
#endif

    PUI_IrqButtonLongPressHandler_v(&PUI_StartStopBtn_s);
}

#ifdef __cplusplus
}
#endif

