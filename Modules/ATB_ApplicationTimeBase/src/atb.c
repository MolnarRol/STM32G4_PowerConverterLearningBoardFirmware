/*
 * ATB.c
 *
 *  Created on: Dec 15, 2024
 *      Author: molnar
 */
#include <atb.h>


u32 s_ATB_SysTickOverflowCounter_u32 = (u32)0;


void ATB_Init_v(void)
{
	SysTick->CTRL = (__IOM u32)0;						/* Disable SysTick operation. */
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		/* SysTick uses the processor clock. */
	SysTick->LOAD = ATB_RELOAD_VALUE_d;					/* SysTick overflow period to 24 bit unsigned maximum. */
	SysTick->VAL = (__IOM u32)0;						/* Reset SysTick current value to 0. */
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;			/* Enable SysTick operation. */
}

u64 ATB_GetCurrentTicks_u64(void)
{
	u64 current_systick_val_u64;
	__disable_irq();
	current_systick_val_u64 = (u64)SysTick->VAL;
	if((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != (u32)0)
	{
		s_ATB_SysTickOverflowCounter_u32 += (u32)1;
	}
	__enable_irq();
	return (u64)s_ATB_SysTickOverflowCounter_u32 * (u64)ATB_RELOAD_VALUE_d + current_systick_val_u64;
}

boolean ATB_CheckIfPeriodHasElapsed_b(u64* reference_tick_mark_pu64, const u64 tick_delta_u64)
{
	boolean return_value_b = False_b;
	const u64 current_tick_mark_u64 = ATB_GetCurrentTicks_u64();
	const u64 delta_tick_marks_u64 = current_tick_mark_u64 - *reference_tick_mark_pu64;

	if(delta_tick_marks_u64 >= tick_delta_u64)
	{
		*reference_tick_mark_pu64 = current_tick_mark_u64;
		return_value_b = True_b;
	}
	return (return_value_b);
}

FORCE_INLINE u64 ATB__ms__TO__ticks__u64(u64 ms)
{
	const u64 ms_in_ticks = SYS_CPU_CLOCK_FREQ__Hz__d / (u64)1000;
	return (ms * ms_in_ticks);
}

FORCE_INLINE u64 ATB__us__TO__ticks__u64(u64 us)
{
	const u64 us_in_ticks = SYS_CPU_CLOCK_FREQ__Hz__d / (u64)1000000;
	return (us * us_in_ticks);
}

FORCE_INLINE u64 ATB__s__TO__ticks__u64(u64 s)
{
	const u64 s_in_ticks = SYS_CPU_CLOCK_FREQ__Hz__d;
	return (s * s_in_ticks);
}
