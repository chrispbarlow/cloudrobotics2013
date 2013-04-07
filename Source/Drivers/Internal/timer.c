/*
 * Drivers for timer peripherals.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "../timer.h"

typedef struct
{
	uint32_t control;
	uint32_t prescalar_counter;
	uint32_t prescalar_match;
	uint32_t timer_counter;
	uint32_t timer_match;
	uint32_t padding[59];
} raw_timer_register_t;

///////////////////////////////////////////////////////////////////////////////

typedef volatile raw_timer_register_t * const timer_register_t;
static timer_register_t timer_register = (timer_register_t) 0x80000000;

///////////////////////////////////////////////////////////////////////////////

static const uint32_t control_start    = (1 << 0);
static const uint32_t control_overflow = (1 << 1);

///////////////////////////////////////////////////////////////////////////////

uint32_t Timer_Init(const uint8_t timer, const uint32_t tick_us,
		const uint32_t oscillator_frequency, const uint32_t clock_divider)
{
	Timer_Stop(timer);
	Timer_Set_Prescalar(timer, 0);
	Timer_Clear(timer);
	Timer_Overflow_Clear(timer);

	const uint32_t clock_frequency = oscillator_frequency;
	const uint32_t timer_frequency = clock_frequency / clock_divider;
	Timer_Set_Prescalar(timer, timer_frequency / 1000000);

	const uint32_t match = timer_frequency / (timer_frequency / tick_us);
	Timer_Set_Match(timer, match);

	return match;
}

///////////////////////////////////////////////////////////////////////////////

void Timer_Stop(const uint8_t timer)
{
	timer_register[timer].control &= ~control_start;
}

///////////////////////////////////////////////////////////////////////////////

void Timer_Start(const uint8_t timer)
{
	timer_register[timer].control |= control_start;
}

///////////////////////////////////////////////////////////////////////////////

void Timer_Clear(const uint8_t timer)
{
	timer_register[timer].timer_counter = 0;
}

///////////////////////////////////////////////////////////////////////////////

void Timer_Overflow_Clear(const uint8_t timer)
{
	timer_register[timer].control &= ~control_overflow;
}

///////////////////////////////////////////////////////////////////////////////

void Timer_Set_Prescalar(const uint8_t timer, const uint32_t prescalar)
{
	const uint32_t value = prescalar > 0 ? prescalar - 1 : 0;
	timer_register[timer].prescalar_match = value;
}

///////////////////////////////////////////////////////////////////////////////

void Timer_Set_Match(const uint8_t timer, const uint32_t match)
{
	const uint32_t value = match > 0 ? match - 1 : 0;
	timer_register[timer].timer_match = value;
}

///////////////////////////////////////////////////////////////////////////////

boolean_t Timer_Overflowed(const uint8_t timer)
{
	return (timer_register[timer].control & control_overflow) != 0;
}

///////////////////////////////////////////////////////////////////////////////

uint32_t Timer_Frequency(const uint8_t timer,
		const uint32_t oscillator_frequency, const uint32_t clock_divider)
{
	const uint32_t clock_frequency = oscillator_frequency;
	const uint32_t timer_frequency = clock_frequency / clock_divider;
	return timer_frequency / (timer_register[timer].prescalar_match + 1);
}

///////////////////////////////////////////////////////////////////////////////

uint32_t Timer_Value(const uint8_t timer)
{
	return timer_register[timer].timer_counter;
}

///////////////////////////////////////////////////////////////////////////////

void Timer_Wait(const uint8_t timer, const uint32_t delay_us,
		const uint32_t oscillator_frequency, const uint32_t clock_divider)
{
	Timer_Init(timer, delay_us, oscillator_frequency, clock_divider);
	Timer_Start(timer);

	while (!Timer_Overflowed(timer))
	{
		// Loop until we have an overflow
	}

	Timer_Stop(timer);
}
