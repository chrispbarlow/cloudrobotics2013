/*
 * Drivers for General Purpose Input/Output (GPIO) peripherals.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include "../tte.h"

/**
 * The driver uses an object of this type to specify individual pins.
 */
typedef struct
{
	uint8_t port;
	uint8_t pin;
} gpio_pin_t;

/**
 * Specifies the direction of each individual pin.
 */
typedef enum
{
	GPIO_INPUT  = 0,
	GPIO_OUTPUT = 1
} gpio_direction_t;

/**
 * Specifies the value for each individual pin.
 */
typedef enum
{
	GPIO_LOW  = 0,
	GPIO_HIGH = 1
} gpio_value_t;

/**
 * Sets the direction of a given pin.  This procedure should be called
 * before calling any of the following macros.
 *
 * \param[in]	gpio	The GPIO port and pin to use.
 * \param[in]	dir		The direction to set for the pin, which
 * 						should be either GPIO_INPUT or GPIO_OUTPUT.
 */
#define GPIO_Set_Direction(gpio, dir)									\
	do { 																\
		gpio_register[(gpio).port].direction &= ~(1 << (gpio).pin);		\
		gpio_register[(gpio).port].direction |= ((dir) << (gpio).pin);	\
	} while (0)

/**
 * Sets the pin.  After calling this, its value will be GPIO_HIGH.
 *
 * \param[in]	gpio	The GPIO port and pin to use.
 */
#define GPIO_Set(gpio)													\
	do {																\
		gpio_register[(gpio).port].value |= (1 << (gpio).pin);			\
	} while(0)

/**
 * Clears the pin.  After calling this, its value will be GPIO_LOW.
 *
 * \param[in]	gpio	The GPIO port and pin to use.
 */
#define GPIO_Clear(gpio)												\
	do {																\
		gpio_register[(gpio).port].value &= ~(1 << (gpio).pin);			\
	} while(0)

/**
 * Writes a given value to the given pin.
 *
 * \param[in]	gpio	The GPIO port and pin to use.
 * \param[in]	val		The value (GPIO_HIGH or GPIO_LOW) to write.
 */
#define GPIO_Write(gpio, val)											\
	do {																\
		const gpio_value_t value = (val);								\
		gpio_register[(gpio).port].value &= ~(!value << (gpio).pin);	\
		gpio_register[(gpio).port].value |=  ( value << (gpio).pin);	\
	} while(0)

/**
 * Returns the current value of the pin.
 *
 * \param[in]	gpio	The GPIO port and pin to use.
 *
 * \return	The current value of the pin; either GPIO_HIGH, or GPIO_LOW.
 */
#define GPIO_Get(gpio) 													\
	((gpio_register[(gpio).port].value >> (gpio).pin) & 0x00000001)

///////////////////////////////////////////////////////////////////////////////

// Internal GPIO implementation details are placed here, as they are required
// by the macro implementations seen above.  Macros are used for performance,
// and superior debugging to inlined functions (which often cannot be stepped
// over when debugging).

typedef struct
{
	uint32_t value;
	uint32_t direction;
	uint32_t padding[62];
} raw_gpio_register_t;

typedef volatile raw_gpio_register_t * const gpio_register_t;
static gpio_register_t gpio_register = (gpio_register_t) 0x80000200;

#endif // !GPIO_H_INCLUDED
