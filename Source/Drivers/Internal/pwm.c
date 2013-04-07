/*
 * Drivers for Pulse Width Modulation (PWM) peripherals.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "../pwm.h"

#include "../../tte.h"

typedef volatile uint32_t * const pwm_register_t;
static pwm_register_t pwm_register         = (pwm_register_t) 0x80000B00;
static pwm_register_t pwm_divider_register = (pwm_register_t) 0x80000B04;

///////////////////////////////////////////////////////////////////////////////

static const uint32_t pwm_enabled = (1 << 0);
static const uint32_t pwm_reset   = (1 << 1);

static const uint32_t channel_inverted[] = { (1 << 10), (1 << 20) };
static const uint32_t channel_value[]    = { (1 << 11), (1 << 21) };

static const uint32_t channel_duty_cycle[] = { (1 <<  2) | (1 <<  3) |
											   (1 <<  4) | (1 <<  5) |
											   (1 <<  6) | (1 <<  7) |
											   (1 <<  8) | (1 <<  9),

											   (1 << 12) | (1 << 13) |
											   (1 << 14) | (1 << 15) |
											   (1 << 16) | (1 << 17) |
											   (1 << 18) | (1 << 19) };

///////////////////////////////////////////////////////////////////////////////

void PWM_Enable(void)
{
	*pwm_register |= pwm_enabled;
}

///////////////////////////////////////////////////////////////////////////////

void PWM_Disable(void)
{
	*pwm_register &= ~pwm_enabled;
}

///////////////////////////////////////////////////////////////////////////////

void PWM_Reset(void)
{
	*pwm_register |=  pwm_reset;
	*pwm_register &= ~pwm_reset;
}

///////////////////////////////////////////////////////////////////////////////

void PWM_Set_Clock_Divider(const uint32_t clock_divider)
{
	*pwm_divider_register = clock_divider;
}

///////////////////////////////////////////////////////////////////////////////

void PWM_Set_Duty_Cycle(const uint8_t channel, const uint8_t duty_cycle)
{
	TTE_DEBUG_ASSERT(channel < 2);
	TTE_DEBUG_ASSERT(duty_cycle <= 100);

	*pwm_register &= ~channel_duty_cycle[channel];
	*pwm_register |= (uint32_t) duty_cycle << (channel == 0 ? 2 : 12);
}
