/*
 * Drivers for SPI peripherals.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

#include "../tte.h"

/**
 * Initialises the SPI to a given clock rate.
 *
 * \param[in]	spi 	The zero-based index of the spi.
 * \param[in]	mode	The SPI mode to use.
 * \param[in]	clk_div	The clock divider to use
 */
void SPI_Init(const uint8_t spi, const uint32_t mode, const uint32_t clock_divider);

/**
 * Returns TRUE if the SPI channel has finished the last operation
 *
 * \param[in]	spi 	The zero-based index of the spi.
 *
 * \return	The status of last operation.
 */
boolean_t SPI_Ready(const uint8_t spi);

/**
 * Send the given data over the SPI channel and return the value received.
 *
 * \param[in]	spi 	The zero-based index of the spi.
 * \param[in]	data	The SPI data to send.
 *
 * \return	The value received by the SPI channel.
 */
uint8_t SPI_Swap(const uint8_t spi, const uint8_t data);

#endif // !SPI_H_INCLUDED
