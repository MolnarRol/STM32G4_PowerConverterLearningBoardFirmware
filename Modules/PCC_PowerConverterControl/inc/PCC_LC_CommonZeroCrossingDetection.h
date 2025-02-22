/*
 * PCC_LC_CommonZeroCrossingDetection.h
 *
 *  Created on: Feb 22, 2025
 *      Author: molnar
 */

#ifndef PCC_POWERCONVERTERCONTROL_INC_PCC_LC_COMMONZEROCROSSINGDETECTION_H_
#define PCC_POWERCONVERTERCONTROL_INC_PCC_LC_COMMONZEROCROSSINGDETECTION_H_
#include "PCC_private_interface.h"

#define ZC_MIN_INPUT_FREQ_Hz_def                    40UL
#define ZC_MAX_INPUT_FREQ_Hz_def                    70.0f

#define ZC_TIMER_INPUT_INCREMENT_FREQ_Hz_def        170000000UL
#define ZC_TIMER_RESOLUTION_def                     (1UL << 16UL)

#define ZC_TIMER_FREQUENCY_DIVISION_def             ZC_TIMER_INPUT_INCREMENT_FREQ_Hz_def / ZC_MIN_INPUT_FREQ_Hz_def
#define ZC_TIMER_PRESCALER_def                      ZC_TIMER_FREQUENCY_DIVISION_def / ZC_TIMER_RESOLUTION_def

#define ZC_TIMER_INCREMENT_FREQ_HZ_def              ZC_TIMER_INPUT_INCREMENT_FREQ_Hz_def / ZC_TIMER_PRESCALER_def

typedef struct
{
    void (*start_pulses_pfv)(void);
    void (*inhibit_pulses_pfv)(void);
    void (*operation_handler_pfv)(void);
} PCC_LC_ZC_TopologyControlCallbacks_struct;

void PCC_LC_ZC_Init_v(void);
void PCC_LC_ZC_IrqHandler_v(void);
void PCC_LC_ZC_Enable_v(void);
void PCC_LC_ZC_Disable_v(void);
volatile f32* PCC_LC_ZC_GetLineFreq__Hz__pf32(void);
void PCC_LC_ZC_SetActiveLineCommutatedTopology_v(PCC_LC_ZC_TopologyControlCallbacks_struct* control_ps);

#endif /* PCC_POWERCONVERTERCONTROL_INC_PCC_LC_COMMONZEROCROSSINGDETECTION_H_ */
