/*
 * DINT_public_interface.h
 *
 *  Created on: Nov 10, 2024
 *      Author: roland
 */

#ifndef DINT_DIGITALINTERFACE_INTERFACE_DINT_PUBLIC_INTERFACE_H_
#define DINT_DIGITALINTERFACE_INTERFACE_DINT_PUBLIC_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
    DINT_DISABLE_e                          = 0,
    DINT_ENABLE_e                           = 1
} DINT_pin_enable_state_enum;
#define DI_STATE_DISABLE_d                          0x02U
#define DI_STATE_ENABLE_d                           0x01U

typedef enum
{
    DI0_e                                   = 0,
    DI1_e                                   = 1
} DINT_DI_pin_enum;

typedef enum
{
    DI_ACTIVATION_POLARITY_RISING_EDGE_e    = 0,
    DI_ACTIVATION_POLARITY_FALLING_EDGE_e   = 1,
    DI_ACTIVATION_POLARITY_BOTH_EDGES_e     = 2
} DINT_DI_pin_polarity_enum;

typedef enum
{
    DO0_e                                   = 0,
    DO1_e                                   = 1
} DINT_DO_pin_enum;

void DINT_Handler(void);

void DINT_SetDigitalInputActivationCallback(DINT_DI_pin_enum pin_e, void (*callback_pf)(void));
void DINT_SetDigitalInputState(DINT_DI_pin_enum pin_e, DINT_pin_enable_state_enum new_state_e);
void DINT_SetDigitalInputPolarity(DINT_DI_pin_enum pin_e, DINT_DI_pin_polarity_enum polarity_e);

#ifdef __cplusplus
}
#endif
#endif /* DINT_DIGITALINTERFACE_INTERFACE_DINT_PUBLIC_INTERFACE_H_ */
