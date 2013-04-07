/*
 * Drivers for Universal Asynchronous Receiver/Transmitter (UART) peripherals.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include "../tte.h"

/**
 * Enables the given UART channel.
 *
 * \param[in]	channel		The UART channel to use.  This should be
 * 							either zero or one, by default.
 * \param[in]	as_irda		Specifies if the UART should be used for
 * 							the Infrared Data Association protocol.
 */
void UART_Enable(const uint8_t channel, const boolean_t as_irda);

/**
 * Disables the given UART channel.
 *
 * \param[in]	channel		The UART channel to use.  This should be
 * 							either zero or one, by default.
 */
void UART_Disable(const uint8_t channel);

/**
 * Initialises the given UART channel with the given baud rate, this
 * should be called before using any other functions in this driver.
 *
 * \param[in]	channel		The UART channel to use.  This should be
 * 							either zero or one, by default.
 * \param[in]	speed		The desired UART baud-rate.  This is used
 * 							to calculate an appropriate clock divider.
 */
void UART_Set_Baud_Rate(const uint8_t channel, const uint32_t speed,
		const uint32_t oscillator_frequency, const uint32_t clock_divider);

/**
 * Writes a single character to the given UART channel.  Please be sure
 * to call UART_Set_Baud_Rate before using this function!
 *
 * \param[in]	channel		The UART channel to use.  This should be
 * 							either zero or one, by default.
 * \param[in]	data		The character to send over the UART.
 *
 * \return	TRUE if the data was written successfully, FALSE if not.
 */
boolean_t UART_Write(const uint8_t channel, const char data);

/**
 * Reads a single Character from the given UART channel.  Please be sure
 * to call UART_Set_Baud_Rate before using this function!
 *
 * If there is no data available, then zero will be returned.  You can use
 * UART_Received_Data to determine if there is data available in the UART.
 *
 * \param[in]	channel		The UART channel to use.  This should be
 * 							either zero or one, by default.
 *
 * \return	A single character from the UART, or zero if there was no data.
 */
char UART_Read(const uint8_t channel);

/**
 * Determines if data is ready to be read from the UART channel.
 *
 * \param[in]	channel		The UART channel to use.  This should be
 * 							either zero or one, by default.
 *
 * \return	TRUE if there is data available, FALSE if not.
 */
boolean_t UART_Received_Data(const uint8_t channel);
/**
 * Determines if UART channel is busy in transmission process.
 *
 * \param[in]	channel		The UART channel to use.  This should be
 * 							either zero or one, by default.
 *
 * \return	TRUE if transmission is in process, FALSE if not.
 */
boolean_t UART_Transmitting_Data(const uint8_t channel);


#endif // !UART_H_INCLUDED
