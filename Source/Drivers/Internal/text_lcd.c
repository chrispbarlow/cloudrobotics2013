/*
 * Drivers for accessing a text-based LCD through GPIO.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "../text_lcd.h"
#include "../gpio.h"

#include "../../tte.h"

void Send_Byte(const text_lcd_t* const lcd, const uint8_t byte,
			   const boolean_t cmd);
void Long_Delay(const uint32_t time);
void Short_Delay(void);

///////////////////////////////////////////////////////////////////////////////

static const uint8_t font_select   = 0x38;
static const uint8_t scroll_select = 0x06;

static const uint8_t display_off   = 0x08;
static const uint8_t display_on    = 0x0C;
static const uint8_t clear_display = 0x01;

static const uint8_t row_select[]  = { 0x80, 0xC0 };

///////////////////////////////////////////////////////////////////////////////

void Text_LCD_Init(const text_lcd_t* const lcd)
{
	for (uint32_t i = 0; i < 8; i++)
	{
		GPIO_Set_Direction(lcd->data[i], GPIO_OUTPUT);
		GPIO_Clear(lcd->data[i]);
	}

	GPIO_Set_Direction(lcd->power, GPIO_OUTPUT);
	GPIO_Set_Direction(lcd->command, GPIO_OUTPUT);
	GPIO_Set_Direction(lcd->enable, GPIO_OUTPUT);
	GPIO_Set_Direction(lcd->write, GPIO_OUTPUT);

	GPIO_Clear(lcd->power);
	GPIO_Clear(lcd->command);
	GPIO_Clear(lcd->enable);
	GPIO_Clear(lcd->write);

	Long_Delay(10);
	GPIO_Set(lcd->power);
	Long_Delay(30);

	for (uint32_t i = 0; i < 2; i++)
	{
		Send_Byte(lcd, font_select, FALSE);
		Long_Delay(1);
	}

	Send_Byte(lcd, scroll_select, FALSE);
	Long_Delay(1);
	Send_Byte(lcd, display_off, FALSE);
	Long_Delay(1);
	Send_Byte(lcd, clear_display, FALSE);
	Long_Delay(5);
	Send_Byte(lcd, display_on, FALSE);
	Long_Delay(1);
}

///////////////////////////////////////////////////////////////////////////////

void Text_LCD_Send_String(const text_lcd_t* const lcd,
						  const char * const str)
{
	TTE_DEBUG_ASSERT(str != NULL);

	uint32_t i = 0;
	while (str[i] != 0)
	{
		Send_Byte(lcd, str[i++], TRUE);
	}
}

///////////////////////////////////////////////////////////////////////////////

void Text_LCD_Send_Char(const text_lcd_t* const lcd,
						const char value)
{
	Send_Byte(lcd, value, TRUE);
}

///////////////////////////////////////////////////////////////////////////////

void Text_LCD_Set_Row(const text_lcd_t* const lcd, const uint8_t row)
{
	TTE_DEBUG_ASSERT(row < 2);

	Send_Byte(lcd, row_select[row], FALSE);
}

///////////////////////////////////////////////////////////////////////////////

void Send_Byte(const text_lcd_t* const lcd, const uint8_t byte,
			   const boolean_t cmd)
{
	GPIO_Clear(lcd->enable);
	GPIO_Clear(lcd->write);
	GPIO_Write(lcd->command, cmd);

	for (uint32_t i = 0; i < 8; i++)
	{
		GPIO_Write(lcd->data[i], (byte & (1 << i)) != 0);
	}

	Short_Delay();
	GPIO_Set(lcd->enable);
	Short_Delay();
	GPIO_Clear(lcd->enable);
	Short_Delay();
}

///////////////////////////////////////////////////////////////////////////////

void Long_Delay(const uint32_t time)
{
	for (volatile uint32_t i = 0; i < time; i++)
	{
		for (volatile uint32_t j = 0; j < 2250; j++)
		{
			// Dummy loops used for a software delay
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void Short_Delay(void)
{
	for (volatile uint32_t i = 0; i < 90; i++)
	{
		// Dummy loops used for a software delay
	}
}
