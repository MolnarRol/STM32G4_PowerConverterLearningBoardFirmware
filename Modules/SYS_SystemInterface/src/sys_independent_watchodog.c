/*
 * @file        sys_independent_watchodg.c
 * @brief      Configuration and handling of independent watchdog timer for resetting the system if firmware malfunction occurs.
 *
 *
 *  Created on: Nov 21, 2024
 *      Author: roland
 */
#include <sys_independent_watchdog.h>

/**
    @brief      Initialization routine for independent watchdog.
    @details    Watchdog does not use windowing feature. Reset period is set by CONFIG_SYS_IWDG_MAX_RESET_PERIOD__us__d define.
*/
void SYS_IWDG_Init_v(void)
{
#if ( SYS_IWDG_ENABLE_MODULE_d == ENABLE_d )
    DBGMCU->APB1FZR1    |= DBGMCU_APB1FZR1_DBG_IWDG_STOP;               /* Stop IWDG on core halt in debugging. */
    IWDG->KR            = SYS_IWDG_ENABLE_KEY_d;                        /* Enable the IWDG. */
    IWDG->KR            = SYS_IWDG_ENABLE_REGISTER_ACCESS_KEY_d;        /* Enable register access. */
    IWDG->PR            = SYS_IWDG_PRESCALER_d;                         /* No clock prescaling. */
    IWDG->RLR           = SYS_IWDG_RELOAD_VALUE_d;                      /* Set reload value for watchdog. */
    IWDG->WINR          = SYS_IWDG_WINDOW_COMPARE_VALUE_d;              /* Set maximum window - window feature not active. */
    while(IWDG->SR != 0x00UL)                                           /* Wait for the registers to be updated (IWDG_SR = 0x00000000. */
    {
        __NOP();
    }
    IWDG->KR            = SYS_IWDG_FEED_THE_DOG_KEY_d;                  /* Start timer. */
#endif
}

/**
    @brief      Reset watchdog timer to prevent system reset.
    @details    Call this function before reset period time elapses. Reset period is set by CONFIG_SYS_IWDG_MAX_RESET_PERIOD__us__d define.
*/
inline void SYS_IWDG_FeedTheDog_v(void)
{
    IWDG->KR = SYS_IWDG_FEED_THE_DOG_KEY_d;                             /* Reload watchog timer with reload value. */
}
