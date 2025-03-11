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

#define PROD_SW_VERSION_STR_d                       "0.1.0(alpha)"
#define PROD_HW_REVISION_STR_d                      "Rev. A"
#define PROD_SOURCE_URL_d                           "https://github.com/MolnarRol/STM32G4_PowerConverterLearningBoardFirmware"

#define GEN_DEF_PER_CENT_MAX_df32                   (f32)100.0f

#define SYS_CPU_CLOCK_FREQ__Hz__d					( 170000000UL )
#define SYS_AHB_CLOCK_FREQ__Hz__d                   ( 170000000UL )
#define SYS_APB1_CLOCK_FREQ__Hz__d                  ( 170000000UL )
#define SYS_APB2_CLOCK_FREQ__Hz__d                  ( 170000000UL )

#define ENABLE_d                                    1U
#define DISABLE_d                                   0U

#define CONFIG_ENABLE_DIGITAL_INPUTS_d              DISABLE_d

/* IWDG - Independent watchdog. */
#define CONFIG_SYS_IWDG_ENABLE_MODULE_d             ENABLE_d
#define CONFIG_SYS_IWDG_MAX_RESET_PERIOD__us__d     50000UL

#define CONFIG_PCC_IRQ_PRIORITY_d                   2

#endif /* GENERAL_CONFIG_H_ */
