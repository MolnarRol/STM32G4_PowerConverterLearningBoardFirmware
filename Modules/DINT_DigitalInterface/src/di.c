/*
 * di.c
 *
 *  Created on: Nov 10, 2024
 *      Author: roland
 */
#include "di.h"

static DI_Handler_struct DI_inputs_as[] =
{
    {
        .digital_pin_s =
        {
            .port_ps            = DI0_GPIO_PORT_dps,
            .gpio_pin_num_u16   = DI0_GPIO_PIN_d
        }
    },
    {
        .digital_pin_s =
        {
            .port_ps            = DI1_GPIO_PORT_dps,
            .gpio_pin_num_u16   = DI1_GPIO_PIN_d
        }
    }
};

static inline u8 DI_GetDigitalPinState(DI_Handler_struct* handle_ps)
{
    register u16 state_u16 = handle_ps->digital_pin_s.port_ps->IDR &= (u16)1 << handle_ps->digital_pin_s.gpio_pin_num_u16;
    return (state_u16 != (u16)0 ? (u8)1 : (u8)0);
}

void DINT_SetDigitalInputActivationCallback(DINT_DI_pin_enum pin_e, void (*callback_pf)(void))
{
    DI_inputs_as[pin_e].activation_callback_pF = callback_pf;
}

void DINT_SetDigitalInputState(DINT_DI_pin_enum pin_e, DINT_pin_enable_state_enum new_state_e)
{
    DI_inputs_as[pin_e].enabled_state = (u8)new_state_e;
}

void DINT_SetDigitalInputPolarity(DINT_DI_pin_enum pin_e, DINT_DI_pin_polarity_enum polarity_e)
{
    DI_inputs_as[pin_e].activation_polarity = (u8)polarity_e;
}

inline void DI_HandleInputs(void)
{
    u8 input_index_iu8;
    u8 tmp_pin_state_u8;

    for(input_index_iu8 = (u8)0; input_index_iu8 < (u8)(sizeof DI_inputs_as / sizeof DI_inputs_as[0]); input_index_iu8++)
    {
        if(DI_inputs_as[input_index_iu8].enabled_state == (u8)0) continue;

        tmp_pin_state_u8 = DI_GetDigitalPinState(&DI_inputs_as[input_index_iu8]);

        switch((DINT_DI_pin_polarity_enum)DI_inputs_as[input_index_iu8].activation_polarity)
        {
            case DI_ACTIVATION_POLARITY_RISING_EDGE_e:
                if( (DI_inputs_as[input_index_iu8].interface_state_flag == (u8)0) &&
                    (tmp_pin_state_u8 == (u8)1) )
                {
                    DI_CALL_CALLBACK(DI_inputs_as[input_index_iu8]);
                }
                break;

            case DI_ACTIVATION_POLARITY_FALLING_EDGE_e:
                if( (DI_inputs_as[input_index_iu8].interface_state_flag == (u8)1) &&
                    (tmp_pin_state_u8 == (u8)0) )
                {
                    DI_CALL_CALLBACK(DI_inputs_as[input_index_iu8]);
                }
                break;

            case DI_ACTIVATION_POLARITY_BOTH_EDGES_e:
            default:
                if( DI_inputs_as[input_index_iu8].interface_state_flag != tmp_pin_state_u8 )
                {
                    DI_CALL_CALLBACK(DI_inputs_as[input_index_iu8]);
                }
                break;

        }
        DI_inputs_as[input_index_iu8].interface_state_flag = tmp_pin_state_u8;

    }
}
