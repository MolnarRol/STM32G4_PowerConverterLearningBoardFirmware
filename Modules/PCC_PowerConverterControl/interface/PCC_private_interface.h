/*
 * PCC_private_interface.h
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */

#ifndef PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PRIVATE_INTERFACE_H_
#define PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PRIVATE_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <app_types.h>
#include <stm32g4xx.h>
#include <stm32g4xx_ll_tim.h>
#include <stm32g4xx_ll_dma.h>
#include <PCC_public_interface.h>
#include <stdio.h>

typedef struct
{
    void (*initialize_pf)(void);
    void (*active_handler_pf)(void);
    void (*deinitalize_pf)(void);
} PCC_TopologyHandle_struct;

#ifdef __cplusplus
}
#endif
#endif /* PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PRIVATE_INTERFACE_H_ */
