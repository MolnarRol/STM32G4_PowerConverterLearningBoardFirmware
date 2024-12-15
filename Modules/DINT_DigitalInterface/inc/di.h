/*
 * di.h
 *
 *  Created on: Nov 10, 2024
 *      Author: roland
 */

#ifndef DINT_DIGITALINTERFACE_INC_DI_H_
#define DINT_DIGITALINTERFACE_INC_DI_H_
#include "DINT_private_interface.h"

typedef struct
{
    struct
    {
        u8 enabled_state                           :1;
        u8 interface_state_flag                     :1;
        u8 activation_polarity                     :2;
    };
    const struct
    {
        GPIO_TypeDef*                               port_ps;
        u16                                         gpio_pin_num_u16;
    } digital_pin_s;
    void (*activation_callback_pF)(void);
} DI_Handler_struct;

#define DI_CALL_CALLBACK(handle)    { if(handle.activation_callback_pF != NULL) handle.activation_callback_pF(); }

static u8 DI_GetDigitalPinState(DI_Handler_struct* handle_ps);

#endif /* DINT_DIGITALINTERFACE_INC_DI_H_ */
