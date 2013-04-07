/*
 * Drivers for Pulse Width Modulation (PWM) peripherals.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED

#include "../tte.h"

/**
 * Enables the PWM peripheral.
 */
void PWM_Enable(void);

/**
 * Disables the PWM peripheral.
 */
void PWM_Disable(void);

/**
 * Resets the PWM peripheral.
 */
void PWM_Reset(void);

/**
 * Specifies the clock divider for the PWM peripheral.
 *
 * \param[in]	clock_divider	The clock divider to use.
 */
void PWM_Set_Clock_Divider(const uint32_t clock_divider);

/**
 * Specifies the duty cycle for the PWM peripheral.  The PWM
 * peripheral currently supports two output channels, which
 * are tied to separate pins in hardware (the exact details
 * depend on the hardware configuration used).
 *
 * \param[in]	channel		The PWM channel to use.
 * \param[in]	duty_cycle	Percentage of time that the output
 * 							should be high (0 - 100).
 */
void PWM_Set_Duty_Cycle(const uint8_t channel, const uint8_t duty_cycle);

#endif // !PWM_H_INCLUDED
