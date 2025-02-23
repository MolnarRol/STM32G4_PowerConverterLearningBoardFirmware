/*
 * PCC_LC_CommonZeroCrossingDetection.h
 *
 *  Created on: Feb 22, 2025
 *      Author: molnar
 */

#ifndef PCC_POWERCONVERTERCONTROL_INC_PCC_LC_COMMONZEROCROSSINGDETECTION_H_
#define PCC_POWERCONVERTERCONTROL_INC_PCC_LC_COMMONZEROCROSSINGDETECTION_H_
#include "PCC_private_interface.h"


#define PCC_LC_ZC_MINIMUM_INPUT_FREQUENCY_df32      40.0f
#define PCC_LC_ZC_MAXIMUM_INPUT_FREQUENCY_df32      70.0f

#define ZC_LC_ZC_SENSITIVE_TO_RISING_EDGE_d         0UL
#define ZC_LC_ZC_SENSITIVE_TO_ANY_EDGE_d            1UL

typedef struct
{
    void (*start_pulses_pfv)(void);
    void (*inhibit_pulses_pfv)(void);
    void (*operation_handler_pfv)(void);
} PCC_LC_ZC_TopologyControlCallbacks_struct;

extern volatile f32 PCC_LC_ZC_LineFreq__Hz__f32;

void PCC_LC_ZC_Init_v(PCC_LC_ZC_TopologyControlCallbacks_struct* control_ps, u32 edge_sensitivity_u32);
void PCC_LC_ZC_DeInit_v(void);
void PCC_LC_ZC_IrqHandler_v(void);
void PCC_LC_ZC_Enable_v(void);
void PCC_LC_ZC_Disable_v(void);

#endif /* PCC_POWERCONVERTERCONTROL_INC_PCC_LC_COMMONZEROCROSSINGDETECTION_H_ */
