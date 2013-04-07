/**
 * Simple flashing LED heartbeat.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include <tte.h>
#include "../../global.h"
#include "../../ports.h"
#include "flashing_led.h"

/* The current state of the LED connected to the GPIO pin. */
static gpio_value_t led_state = GPIO_LOW;

extern Mode System_Mode_G;

///////////////////////////////////////////////////////////////////////////////

/**
 * Initialises the GPIO pin and outputs the initial value to the LED.
 */
void Flashing_LED_Init(void)
{
	GPIO_Set_Direction(LED_Pin, GPIO_OUTPUT);
	GPIO_Write(LED_Pin, led_state);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Toggles the state of the GPIO pin and its LED.
 */
void Flashing_LED_Toggle(void)
{
	GPIO_Write(LED_Pin, led_state);
	if(System_Mode_G == Set)
	{
		GPIO_Write(Error_Lamp, led_state);
	}
	else
	{
		GPIO_Write(Error_Lamp, GPIO_LOW);
	}
	led_state = !led_state;
}
