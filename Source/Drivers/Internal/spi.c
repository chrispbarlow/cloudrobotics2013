/*
 * Drivers for SPI peripherals.
 *
 * Copyright (c) TTE Systems Limited 2012.  All rights reserved.
 */

#include "../spi.h"

typedef struct
{
	uint32_t control;
	uint32_t clk_div;
	uint32_t data;
	uint32_t padding[61];
} raw_spi_register_t;

///////////////////////////////////////////////////////////////////////////////

typedef volatile raw_spi_register_t * const spi_register_t;
static spi_register_t spi_register = (spi_register_t) 0x80000A00;

///////////////////////////////////////////////////////////////////////////////

static const uint32_t control_ready    = (1 << 0);
static const uint32_t control_cpha     = (1 << 1);
static const uint32_t control_cpol     = (1 << 2);

///////////////////////////////////////////////////////////////////////////////

void SPI_Init(const uint8_t spi, const uint32_t mode, const uint32_t clock_divider)
{
	spi_register[spi].control = ((mode & 0x03) << 1);
	spi_register[spi].clk_div = clock_divider & 0xFFFFFFFC;
}

///////////////////////////////////////////////////////////////////////////////

boolean_t SPI_Ready(const uint8_t spi)
{
	return spi_register[spi].control & 0x01;
}

///////////////////////////////////////////////////////////////////////////////

uint8_t SPI_Swap(const uint8_t spi, const uint8_t data)
{
	while(!SPI_Ready(spi));
	spi_register[spi].data = data;
	while(!SPI_Ready(spi));
	return spi_register[spi].data;
}
