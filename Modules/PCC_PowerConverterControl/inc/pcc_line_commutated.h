/*
 * pcc_line_commutated.h
 *
 *  Created on: Nov 17, 2024
 *      Author: roland
 */

#ifndef PCC_POWERCONVERTERCONTROL_INC_PCC_LINE_COMMUTATED_H_
#define PCC_POWERCONVERTERCONTROL_INC_PCC_LINE_COMMUTATED_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "PCC_private_interface.h"


/*************************************************************************************************************************
        Zero crossing detection
*************************************************************************************************************************/
//#define ZC_MIN_INPUT_FREQ_Hz_def                    40UL
//#define ZC_TIMER_INPUT_INCREMENT_FREQ_Hz_def        170000000UL
//#define ZC_TIMER_RESOLUTION_def                     (1UL << 16UL)
//
//#define ZC_TIMER_FREQUENCY_DIVISION_def             ZC_TIMER_INPUT_INCREMENT_FREQ_Hz_def / ZC_MIN_INPUT_FREQ_Hz_def
//#define ZC_TIMER_PRESCALER_def                      ZC_TIMER_FREQUENCY_DIVISION_def / ZC_TIMER_RESOLUTION_def
//
//#define ZC_TIMER_INCREMENT_FREQ_HZ_def              ZC_TIMER_INPUT_INCREMENT_FREQ_Hz_def / ZC_TIMER_PRESCALER_def

void PCC_LC_InitZeroCrossingDetection_v(void);
void PCC_LC_ZeroCrossingDetection_Enable_v(void);
void PCC_LC_ZeroCrossingDetection_Disable_v(void);


void PCC_InitPulseTimer1_v(void);


#ifdef __cplusplus
}
#endif
#endif /* PCC_POWERCONVERTERCONTROL_INC_PCC_LINE_COMMUTATED_H_ */
