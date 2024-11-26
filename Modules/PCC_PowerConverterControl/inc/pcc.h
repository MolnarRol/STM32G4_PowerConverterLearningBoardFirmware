/*
 * pcc.h
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */

#ifndef PCC_POWERCONVERTERCONTROL_INC_PCC_H_
#define PCC_POWERCONVERTERCONTROL_INC_PCC_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <PCC_private_interface.h>

typedef enum
{
    PCC_UNINITIALIZED_e,
    PCC_INITIALIZATION_e,
    PCC_INACTIVE_e,
    PCC_ACTIVE_e,
    PCC_UNINITIALIZATION_e
} PCC_HandleStates_enum;

#ifdef __cplusplus
}
#endif
#endif /* PCC_POWERCONVERTERCONTROL_INC_PCC_H_ */
