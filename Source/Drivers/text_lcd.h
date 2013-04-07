/*
 * Drivers for accessing a text-based LCD through GPIO.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef TEXT_LCD_H_INCLUDED
#define TEXT_LCD_H_INCLUDED

#include "gpio.h"

#include "../tte.h"

/**
 * The driver uses an object of this type to specify the LCD interface.
 */
typedef struct
{
	gpio_pin_t data[8];
	gpio_pin_t power;
	gpio_pin_t command;
	gpio_pin_t enable;
	gpio_pin_t write;
} text_lcd_t;

/**
 * Initialises the given LCD.
 *
 * \param[in]	lcd		The Text-based LCD to use.
 */
void Text_LCD_Init(const text_lcd_t* const lcd);

/**
 * Sends a string to the current location on the LCD.
 *
 * \param[in]	lcd		The Text-based LCD to use.
 * \param[in]	str		A zero-terminated string to send to the LCD.
 */
void Text_LCD_Send_String(const text_lcd_t* const lcd,
						  const char * const str);

/**
 * Sends a single character to the current location on the LCD.
 *
 * \param[in]	lcd		The Text-based LCD to use.
 * \param[in]	value	A single character to send to the LCD.
 */
void Text_LCD_Send_Char(const text_lcd_t* const lcd,
						const char value);

/**
 * Sets the current location of the LCD to the start of the given row.
 *
 * \param[in]	lcd		The Text-based LCD to use.
 * \param[in]	row		The row to use for the new cursor location.
 * 						Currently, this should be in the range 0 - 1.
 */
void Text_LCD_Set_Row(const text_lcd_t* const lcd, const uint8_t row);

#endif // !TEXT_LCD_H_INCLUDED
