/*
 * sys_independent_watchdog.h
 *
 *  Created on: Nov 21, 2024
 *      Author: roland
 */

#ifndef SYS_SYSTEMINTERFACE_INC_SYS_INDEPENDENT_WATCHDOG_H_
#define SYS_SYSTEMINTERFACE_INC_SYS_INDEPENDENT_WATCHDOG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <sys_private_interface.h>

#define SYS_IDWG_CLOCK_SOURCE_FREQ__Hz__d           ( 32000UL )
#define SYS_IWDG_ENABLE_KEY_d                       ( 0x0000CCCCUL )
#define SYS_IWDG_ENABLE_REGISTER_ACCESS_KEY_d       ( 0x00005555UL )
#define SYS_IWDG_FEED_THE_DOG_KEY_d                 ( 0x0000AAAAUL )
#define SYS_IWDG_WINDOW_COMPARE_VALUE_d             ( 0x00000FFFUL )
#define SYS_IWDG_PRESCALER_d                        ( 0x0UL )

#if !defined(CONFIG_SYS_IWDG_ENABLE_MODULE_d)
#warning "[SYS_IWDG] Missing configuration definition <CONFIG_SYS_IWDG_ENABLE_MODULE_d>.\
          Independent watchdog will not be active by default."
#define SYS_IWDG_ENABLE_MODULE_d                    ( DISABLE_d )
#else
#define SYS_IWDG_ENABLE_MODULE_d                    ( CONFIG_SYS_IWDG_ENABLE_MODULE_d )
#if !defined(CONFIG_SYS_IWDG_MAX_RESET_PERIOD__us__d)
#warning "[SYS_IWDG] Missing configuration definition <CONFIG_SYS_IWDG_MAX_RESET_PERIOD__us__d>.\
          Independent watchdog will have 1000 us maximum reset period before system reset."
#define SYS_IWDG_MAX_RESET_PERIOD__us__d            ( 1000UL )
#else
#define SYS_IWDG_MAX_RESET_PERIOD__us__d            ( CONFIG_SYS_IWDG_MAX_RESET_PERIOD__us__d )
#endif
#endif

#define SYS_IWDG_RELOAD_VALUE_d                     ( (SYS_IDWG_CLOCK_SOURCE_FREQ__Hz__d * SYS_IWDG_MAX_RESET_PERIOD__us__d) / 1000000UL )

#ifdef __cplusplus
}
#endif
#endif /* SYS_SYSTEMINTERFACE_INC_SYS_INDEPENDENT_WATCHDOG_H_ */
