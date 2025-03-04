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
    const u32           minimum_valid_press_time__ticks__u32;
    const u32           minimum_long_press_time__ticks__u32;
    u32                 (* const tick_src_u32)(void);
    void                (*pressed_callback_pfv)(void);
    void                (*long_pressed_callback_pfv)(void);

    u32                 last_state_change__ticks__u32;
    boolean             state_b;
    boolean             long_press_detected_b;

} PUI_Button_struct;

extern PUI_Button_struct start_stop_btn_s;

void PUI_Init(void);
void PUI_Handler(void);

#ifdef __cplusplus
}
#endif
#endif /* PUI_PHYSICALUSERINPUT_INTERFACE_PUI_PUBLIC_INTERFACE_H_ */
