/*
 * Drivers for Universal Asynchronous Receiver/Transmitter (UART) peripherals.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "../uart.h"
#include "../../tte.h"
#include "../../interrupt_control.h"

typedef struct
{
	uint32_t control;
	uint32_t clock_divider;
	uint32_t data;
	uint32_t irda_clock_divider;
	uint32_t padding[60];
} raw_uart_register_t;

///////////////////////////////////////////////////////////////////////////////

typedef volatile raw_uart_register_t * const uart_register_t;
static uart_register_t uart_register = (uart_register_t) 0x80000700;

///////////////////////////////////////////////////////////////////////////////

static const uint32_t control_enabled      = (1 << 0);
static const uint32_t control_irda_enabled = (1 << 1);
static const uint32_t control_transmitting = (1 << 2);
static const uint32_t control_receiving    = (1 << 3);

///////////////////////////////////////////////////////////////////////////////

// These allow us to route printf through UART 0
#include <reent.h>
#include <tte/device.h>
#include <stdio.h>

int uart_open_r(TTE_UNUSED struct _reent *r, TTE_UNUSED const char *path,
				TTE_UNUSED int flags, TTE_UNUSED int mode);
int uart_close_r(TTE_UNUSED struct _reent *r, TTE_UNUSED int fd);
long uart_write_r(TTE_UNUSED struct _reent *r, TTE_UNUSED int fd,
				  const char *ptr, int len);
long uart_read_r(TTE_UNUSED struct _reent *r, TTE_UNUSED int fd,
				 char *ptr, TTE_UNUSED int len);

static const device_t dev_uart = { "UART", uart_open_r, uart_close_r,
								   uart_write_r, uart_read_r };

///////////////////////////////////////////////////////////////////////////////

void UART_Enable(const uint8_t channel, const boolean_t as_irda)
{
	TTE_DEBUG_ASSERT(channel < 1);

	uart_register[channel].control |= control_enabled;

	if (as_irda)
	{
		uart_register[channel].control |= control_irda_enabled;
	}

	// Setup channel 0 for printf
	if (channel == 0) {
		Device_Remove_Loc(1);
		Device_Add_Loc(&dev_uart, 0);
		Device_Add_Loc(&dev_uart, 1);
		Device_Add_Loc(&dev_uart, 2);
	}
}

///////////////////////////////////////////////////////////////////////////////

void UART_Disable(const uint8_t channel)
{
	TTE_DEBUG_ASSERT(channel < 1);

	uart_register[channel].control &= ~control_enabled;
	uart_register[channel].control &= ~control_irda_enabled;
}

///////////////////////////////////////////////////////////////////////////////

void UART_Set_IrDA_Baud_Rate(const uint8_t channel, const uint32_t speed,
		const uint32_t oscillator_frequency, const uint32_t clock_divider)
{
	TTE_DEBUG_ASSERT(channel < 1);

	const uint32_t clock_frequency = oscillator_frequency;
	const uint32_t timer_frequency = clock_frequency / clock_divider;

	const uint32_t baud = timer_frequency/speed;
	const uint32_t baud3_16 = (((baud/16)*3)<<16);

	uart_register[channel].irda_clock_divider = (baud | baud3_16) ;
}
///////////////////////////////////////////////////////////////////////////////

void UART_Set_Baud_Rate(const uint8_t channel, const uint32_t speed,
		const uint32_t oscillator_frequency, const uint32_t clock_divider)
{
	TTE_DEBUG_ASSERT(channel < 1);

	const uint32_t clock_frequency = oscillator_frequency;
	const uint32_t timer_frequency = clock_frequency / clock_divider;
	const uint32_t uart_clock_divider = (timer_frequency / (speed));
	uart_register[channel].clock_divider = uart_clock_divider;

	UART_Set_IrDA_Baud_Rate(channel, speed, oscillator_frequency, clock_divider);
}

///////////////////////////////////////////////////////////////////////////////

boolean_t UART_Write(const uint8_t channel, const char data)
{
	TTE_DEBUG_ASSERT(channel < 1);

	if ((uart_register[channel].control & control_transmitting) != 0)
	{
		return FALSE;
	}

	uart_register[channel].data = (uint32_t) data;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

char UART_Read(const uint8_t channel)
{
	TTE_DEBUG_ASSERT(channel < 1);

	if ((uart_register[channel].control & control_receiving) == 0)
	{
		return 0;
	}

	const uint32_t data = uart_register[channel].data;
	uart_register[channel].control &= ~control_receiving;

	return (char) data;
}

///////////////////////////////////////////////////////////////////////////////

boolean_t UART_Received_Data(const uint8_t channel)
{
	TTE_DEBUG_ASSERT(channel < 1);

	return (uart_register[channel].control & control_receiving) != 0;
}

///////////////////////////////////////////////////////////////////////////////

boolean_t UART_Transmitting_Data(const uint8_t channel)
{
	TTE_DEBUG_ASSERT(channel < 1);

	return (uart_register[channel].control & control_transmitting) != 0;
}

///////////////////////////////////////////////////////////////////////////////

int uart_open_r(TTE_UNUSED struct _reent *r, TTE_UNUSED const char *path,
				TTE_UNUSED int flags, TTE_UNUSED int mode)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int uart_close_r(TTE_UNUSED struct _reent *r, TTE_UNUSED int fd)
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

long uart_write_r(TTE_UNUSED struct _reent *r, TTE_UNUSED int fd,
				  const char *ptr, int len)
{
	Disable_Interrupt();

	for (int i = 0; i < len; i++)
	{
		while (!UART_Write(0, ptr[i]))
		{
			// Just wait until the transmit is complete.
		}
	}

	Enable_Interrupt();

	return len;
}

///////////////////////////////////////////////////////////////////////////////

long uart_read_r(TTE_UNUSED struct _reent *r, TTE_UNUSED int fd,
				 char *ptr, TTE_UNUSED int len)
{
	Disable_Interrupt();

	boolean_t have_data = FALSE;

	for (int i = 0; i < len; i++)
	{
		char value = 0;
		while ((value = UART_Read(0)) == 0)
		{
			if (have_data)
			{
				Enable_Interrupt();
				return i;
			}
		}

		have_data = TRUE;
		ptr[i] = value;
	}

	Enable_Interrupt();

	return len;
}
