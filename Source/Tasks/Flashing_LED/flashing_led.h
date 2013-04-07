/*
 * Simple flashing LED task created with a basic C project.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef FLASHING_LED_H_INCLUDED
#define FLASHING_LED_H_INCLUDED

/**
 * Initialises the GPIO pin and outputs the initial value to the LED.
 */
void Flashing_LED_Init(void);

/**
 * Toggles the state of the GPIO pin and its LED.
 */
void Flashing_LED_Toggle(void);

#endif // !FLASHING_LED_H_INCLUDED
