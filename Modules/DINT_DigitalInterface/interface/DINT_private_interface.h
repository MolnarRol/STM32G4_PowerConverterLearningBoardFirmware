/*
 * DINT_private_interface.h
 *
 *  Created on: Nov 10, 2024
 *      Author: roland
 */

#ifndef DINT_DIGITALINTERFACE_INTERFACE_DINT_PRIVATE_INTERFACE_H_
#define DINT_DIGITALINTERFACE_INTERFACE_DINT_PRIVATE_INTERFACE_H_

#include "app_types.h"
#include "stm32g4xx.h"
#include "stm32g4xx_ll_gpio.h"
#include "digital_interface_config.h"
#include "DINT_public_interface.h"

void DI_HandleInputs(void);


#endif /* DINT_DIGITALINTERFACE_INTERFACE_DINT_PRIVATE_INTERFACE_H_ */
