/*
 * PCC_public_interface.h
 *
 *  Created on: Nov 12, 2024
 *      Author: roland
 */

#ifndef PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PUBLIC_INTERFACE_H_
#define PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PUBLIC_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef union
{
	struct
	{
		u8 gd1_f1			: 1;
		u8 gd2_f1			: 1;
		u8 gd3_f1			: 1;
		u8 gd4_f1			: 1;
		u8 gd5_f1			: 1;
		u8 gd6_f1			: 1;
		u8 _reserved		: 2;
	} drivers_s;
	u8 byte_val_u8;
} PCC_driver_enable_union;

void PCC_Handler(void);
void PCC_SetGateDriverPowerStates(PCC_driver_enable_union enable_states_u);


#ifdef __cplusplus
}
#endif
#endif /* PCC_POWERCONVERTERCONTROL_INTERFACE_PCC_PUBLIC_INTERFACE_H_ */
