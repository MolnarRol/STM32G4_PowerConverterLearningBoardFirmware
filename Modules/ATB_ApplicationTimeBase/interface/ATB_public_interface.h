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
u64 ATB_GetCurrentTicks_u64(void);
boolean ATB_CheckIfPeriodHasElapsed_b(u64* reference_tick_mark_pu64, const u64 tick_delta_u64);

u64 ATB__ms__TO__ticks__u64(u64 ms);
u64 ATB__us__TO__ticks__u64(u64 us);
u64 ATB__s__TO__ticks__u64(u64 s);

#ifdef __cplusplus
}
#endif
#endif /* ATB_APPLICATIONTIMEBASE_INTERFACE_ATB_PUBLIC_INTERFACE_H_ */
