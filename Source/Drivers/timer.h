/*
 * Drivers for timer peripherals.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "../tte.h"

/**
 * Initialises the timer to match at the given time, in microseconds.
 *
 * \param[in]	timer	The zero-based index of the timer.
 * \param[in]	tick_us	The tick period in microseconds.
 *
 * \return	The value used with the timer match register to give one tick.
 */
uint32_t Timer_Init(const uint8_t timer, const uint32_t tick_us,
		const uint32_t oscillator_frequency, const uint32_t clock_divider);

/**
 * Stops the given timer.
 *
 * \param[in]	timer	The zero-based index of the timer.
 */
void Timer_Stop(const uint8_t timer);

/**
 * Starts the given timer.
 *
 * \param[in]	timer	The zero-based index of the timer.
 */
void Timer_Start(const uint8_t timer);

/**
 * Clears the given timer.
 *
 * \param[in]	timer	The zero-based index of the timer.
 */
void Timer_Clear(const uint8_t timer);

/**
 * Clears the overflow flag for the given timer.
 *
 * \param[in]	timer	The zero-based index of the timer.
 */
void Timer_Overflow_Clear(const uint8_t timer);

/**
 * Sets the given timer's prescalar to the given value.
 *
 * \param[in]	timer		The zero-based index of the timer.
 * \param[in]	prescalar	The number of clock cycles per timer tick.
 */
void Timer_Set_Prescalar(const uint8_t timer, const uint32_t prescalar);

/**
 * Sets the given timer's match register to the given value.
 *
 * \param[in]	timer	The zero-based index of the timer.
 * \param[in]	match	The timer value at which it will overflow.
 */
void Timer_Set_Match(const uint8_t timer, const uint32_t match);

/**
 * Determines if the given timer has overflowed.
 *
 * \param[in]	timer	The zero-based index of the timer.
 *
 * \return	TRUE if the timer has overflowed, FALSE if not.
 */
boolean_t Timer_Overflowed(const uint8_t timer);

/**
 * Returns the frequency with which the given timer updates, after
 * taking the prescalar into account.
 *
 * \param[in]	timer	The zero-based index of the timer.
 *
 * \return	The update frequency for the timer's value.
 */
uint32_t Timer_Frequency(const uint8_t timer,
		const uint32_t oscillator_frequency, const uint32_t clock_divider);

/**
 * Returns the current value of the given timer.
 *
 * \param[in]	timer	The zero-based index of the timer.
 *
 * \return	The current value of the timer.
 */
uint32_t Timer_Value(const uint8_t timer);

/**
 * Uses the given timer to perform a hardware delay for the given time.
 *
 * \param[in]	timer		The zero-based index of the timer.
 * \param[in]	delay_us	The time to delay for, in microseconds.
 */
void Timer_Wait(const uint8_t timer, const uint32_t delay_us,
		const uint32_t oscillator_frequency, const uint32_t clock_divider);

#endif // !TIMER_H_INCLUDED
