/*
 * Unity unit testing library configuration file.
 */
#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

#include "stm32g4xx_hal.h"

#define UNITY_INCLUDE_PRINT_FORMATTED
#define UNITY_INCLUDE_EXEC_TIME
#define UNITY_CLOCK_MS  HAL_GetTick

#endif /* UNITY_CONFIG_H */
