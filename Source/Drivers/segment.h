/*
 * Drivers for seven segment display peripherals.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef SEGMENT_H_INCLUDED
#define SEGMENT_H_INCLUDED

#include "../tte.h"

/**
 * The driver uses an object of this type to specify individual digits.
 */
typedef struct
{
	uint8_t segment;
	uint8_t digit;
} segment_digit_t;

/**
 * Enables the given seven segment display digit.
 *
 * \param[in]	digit	The seven-segment digit to use.
 */
void Segment_Enable(const segment_digit_t digit);

/**
 * Disables the given seven segment display digit.
 *
 * \param[in]	digit	The seven-segment digit to use.
 */
void Segment_Disable(const segment_digit_t digit);

/**
 * Writes the given hexadecimal value to the given digit.
 *
 * \param[in]	digit	The seven-segment digit to use.
 * \param[in]	value	The hexadecimal value to write to the digit
 * 						(it should be in the range 0x0 - 0xF).
 */
void Segment_Write(const segment_digit_t digit, const uint8_t value);

/**
 * Sets the decimal point for the given digit.
 *
 * \param[in]	digit	The seven-segment digit to use.
 */
void Segment_Set_Decimal(const segment_digit_t digit);

/**
 * Clears the decimal point for the given digit.
 *
 * \param[in]	digit	The seven-segment digit to use.
 */
void Segment_Clear_Decimal(const segment_digit_t digit);

#endif // !SEGMENT_H_INCLUDED
