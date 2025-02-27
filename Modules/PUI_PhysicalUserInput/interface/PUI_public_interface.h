/*
 * PUI_public_interface.h
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */

#ifndef PUI_PHYSICALUSERINPUT_INTERFACE_PUI_PUBLIC_INTERFACE_H_
#define PUI_PHYSICALUSERINPUT_INTERFACE_PUI_PUBLIC_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    const GPIO_TypeDef* port_ps;
    const u32           pin_number_u32;

    u32                 last_btn_down__ticks__u32;
    u32                 last_btn_up__ticks__u32;

    u32                 long_btn_press_minimum_hold_time__ticks__u32;

    void                (*btn_short_press_callback_pvf)(void);
    void                (*btn_long_press_callback_pvf)(void);
    boolean             long_press_handled_b;
    boolean             state_b;
}  PUI_IrqButton_struct;

void PUI_Init(void);
void PUI_Handler(void);

#ifdef __cplusplus
}
#endif
#endif /* PUI_PHYSICALUSERINPUT_INTERFACE_PUI_PUBLIC_INTERFACE_H_ */
