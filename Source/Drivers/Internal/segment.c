/*
 * Drivers for seven segment display peripherals.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "../segment.h"

#include "../../tte.h"

typedef struct
{
	uint32_t enabled;
	uint32_t values;
	uint32_t decimals;
	uint32_t padding[61];
} raw_segment_register_t;

///////////////////////////////////////////////////////////////////////////////

typedef volatile raw_segment_register_t * const segment_register_t;
static segment_register_t segment_register = (segment_register_t) 0x80000900;

///////////////////////////////////////////////////////////////////////////////

static const uint32_t segment_enabled[] = { (1 << 0), (1 << 1),
											(1 << 2), (1 << 3) };

static const uint32_t segment_value[]   = { (1 <<  0) | (1 <<  1) |
											(1 <<  2) | (1 <<  3),
											(1 <<  4) | (1 <<  5) |
											(1 <<  6) | (1 <<  7),
											(1 <<  8) | (1 <<  9) |
											(1 << 10) | (1 << 11),
											(1 << 12) | (1 << 13) |
											(1 << 14) | (1 << 15) };

static const uint32_t segment_decimal[] = { (1 << 0), (1 << 1),
											(1 << 2), (1 << 3) };

///////////////////////////////////////////////////////////////////////////////

void Segment_Enable(const segment_digit_t digit)
{
	TTE_DEBUG_ASSERT(digit.segment < 1);
	TTE_DEBUG_ASSERT(digit.digit < 4);

	segment_register[digit.segment].enabled |= segment_enabled[digit.digit];
}

///////////////////////////////////////////////////////////////////////////////

void Segment_Disable(const segment_digit_t digit)
{
	TTE_DEBUG_ASSERT(digit.segment < 1);
	TTE_DEBUG_ASSERT(digit.digit < 4);

	segment_register[digit.segment].enabled &= ~segment_enabled[digit.digit];
}

///////////////////////////////////////////////////////////////////////////////

void Segment_Write(const segment_digit_t digit, const uint8_t value)
{
	TTE_DEBUG_ASSERT(digit.segment < 1);
	TTE_DEBUG_ASSERT(digit.digit < 4);

	segment_register[digit.segment].values &= ~segment_value[digit.digit];
	segment_register[digit.segment].values |= (value << (digit.digit * 4));
}

///////////////////////////////////////////////////////////////////////////////

void Segment_Set_Decimal(const segment_digit_t digit)
{
	TTE_DEBUG_ASSERT(digit.segment < 1);
	TTE_DEBUG_ASSERT(digit.digit < 4);

	segment_register[digit.segment].decimals |= segment_decimal[digit.digit];
}

///////////////////////////////////////////////////////////////////////////////

void Segment_Clear_Decimal(const segment_digit_t digit)
{
	TTE_DEBUG_ASSERT(digit.segment < 1);
	TTE_DEBUG_ASSERT(digit.digit < 4);

	segment_register[digit.segment].decimals &= ~segment_decimal[digit.digit];
}
