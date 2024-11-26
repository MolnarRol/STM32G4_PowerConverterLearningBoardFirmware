/*
 * AINT_private_interface.h
 *
 *  Created on: Nov 24, 2024
 *      Author: roland
 *      AI1 - PA1
 *      AI2 - PA3
 *      AO1 - PA4
 *      AO2 - PA6
 */

#ifndef AINT_ANALOGINTERFACE_INTERFACE_AINT_PRIVATE_INTERFACE_H_
#define AINT_ANALOGINTERFACE_INTERFACE_AINT_PRIVATE_INTERFACE_H_
#include <AINT_public_interface.h>
#include <app_types.h>
#include <general_config.h>
#include <stm32g4xx.h>

#define AINT_REF_VOLTAGE__V__df32                       ( 2.48f )

#ifdef __cplusplus
extern "C"
{
#endif

static void AINT_AI_InitializeInputs(void);

#ifdef __cplusplus
}
#endif
#endif /* AINT_ANALOGINTERFACE_INTERFACE_AINT_PRIVATE_INTERFACE_H_ */
