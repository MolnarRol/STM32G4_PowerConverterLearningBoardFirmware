/*
 * ATB_public_interface.h
 *
 *  Created on: Dec 15, 2024
 *      Author: molnar
 */

#ifndef ATB_APPLICATIONTIMEBASE_INTERFACE_ATB_PUBLIC_INTERFACE_H_
#define ATB_APPLICATIONTIMEBASE_INTERFACE_ATB_PUBLIC_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif


void ATB_Init_v(void);
u32 ATB_GetCurrentTicks_u32(void);
boolean ATB_CheckIfPeriodHasElapsed_b(u32* reference_tick_mark_pu32, const u32 tick_delta_u32);

#define ATB__ms__TO__ticks__du32(ms) 	(u32)(ms * 1000UL)
#define ATB__us__TO__ticks__du32(us) 	(u32)(us)
#define ATB__s__TO__ticks__du32(s) 		(u32)(s * 1000000UL)

#ifdef __cplusplus
}
#endif
#endif /* ATB_APPLICATIONTIMEBASE_INTERFACE_ATB_PUBLIC_INTERFACE_H_ */
