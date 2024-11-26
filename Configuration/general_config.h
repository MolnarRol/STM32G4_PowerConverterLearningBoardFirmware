/*
 * general_config.h
 *
 *  Created on: Nov 10, 2024
 *      Author: roland
 */

#ifndef GENERAL_CONFIG_H_
#define GENERAL_CONFIG_H_

#include "app_types.h"
#include "stm32g4xx.h"
#include "digital_interface_config.h"

#define ENABLE_d                                    1U
#define DISABLE_d                                   0U

#define CONFIG_ENABLE_DIGITAL_INPUTS_d              ENABLE_d

/* IWDG - Independent watchdog. */
#define CONFIG_SYS_IWDG_ENABLE_MODULE_d             ENABLE_d
#define CONFIG_SYS_IWDG_MAX_RESET_PERIOD__us__d     1500UL

#endif /* GENERAL_CONFIG_H_ */
