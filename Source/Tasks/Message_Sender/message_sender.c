/**
 * Message_Sender.c
 *
 * SPI message send and receive.
 * ---
 * Spec: Transmit CAN messages via SPI protocol.
 * ---
 * Period: 2 ticks
 * Offset: 103 ticks
 * Modes: GO
 * ---
 */
#include "../../global.h"
#include <stdio.h>
#include <stdlib.h>
#include "message_sender.h"
#include "../../Drivers/spi.h"
#include "../../ports.h"
#include "../../config.h"


/* Motor CAN ID */
extern uint32_t canIDLf;
extern uint32_t canIDRt;

/* Motor CAN data array */
extern uint8_t candataLf[8];
extern uint8_t candataRt[8];

/* Servo CAN data array*/
extern uint8_t candataEnc[8];

/* System Mode */
extern Mode System_Mode_G;

uint8_t EXT_ID_Bytes[5];
uint8_t RX_Buffer_ID_Bytes[5];
uint8_t RX_Buffer_Data_array[8];
uint8_t RX0_ID_Bytes[5];
uint8_t RX1_ID_Bytes[5];
uint32_t PAD_ID;

/* Initialisation flag */
Bool init;

/*example data array*/
uint8_t Data[8] = {0x50,0x20,0x45,0x80,0xFF,0x67,0x36,0x23};
uint8_t speedPin;

extern Receive_Buffers bufferRX0;
extern Receive_Buffers bufferRX1;

uint16_t ticked;

/**
 * Initialisation for the SPI package.
 * This will be called from Tasks_Init by default.
 */
void Message_Sender_Init(void)
{
	/* Task initialisation */
	// SPI  0
	// Mode 0
	// SCLK 1KHz = (50MHz / 1000)
	const uint32_t clock_divider = Config_Oscillator_Frequency / 10000000;
	SPI_Init(SPI0, 0, clock_divider);

	speedPin = 0x33;

	bufferRX0.status = Used;
	bufferRX1.status = Used;

	GPIO_Set_Direction(CS_PIN, GPIO_OUTPUT);
	GPIO_Write(CS_PIN, GPIO_HIGH);

	GPIO_Set_Direction(RST_PIN, GPIO_OUTPUT);
	GPIO_Write(RST_PIN, GPIO_HIGH);

	init = False;

	ticked = 0;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Specifies messages to send by SPI
 */
void Message_Sender_Update(void)
{

	/* First Update ensures the CONFIG values are correct */
	if(init == False)
	{
		/* Soft Reset */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0xC0);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Set into CONFIG mode */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x02);
		SPI_Swap(SPI0, 0x0F);
		SPI_Swap(SPI0, 0x80);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Verify CONFIG mode */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x03);
		SPI_Swap(SPI0, 0x0E);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Set values in the CONFIG registers for 250 kbps CAN */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x02);
		SPI_Swap(SPI0, 0x28);
		SPI_Swap(SPI0, 0x02);
		SPI_Swap(SPI0, 0x90);
		SPI_Swap(SPI0, 0x03);
		SPI_Swap(SPI0, 0x00);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Verify set values */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x03);
		SPI_Swap(SPI0, 0x2A);
		speedPin = SPI_Swap(SPI0, 0xFF);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* BFPCTRL register cleared */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x02);
		SPI_Swap(SPI0, 0x0C);
		SPI_Swap(SPI0, 0x00);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* TXRTSCTRL register cleared */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x02);
		SPI_Swap(SPI0, 0x0D);
		SPI_Swap(SPI0, 0x00);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* RXB0CTRL */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x02);
		SPI_Swap(SPI0, 0x60);
		SPI_Swap(SPI0, 0x60);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* RXB1CTRL */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x02);
		SPI_Swap(SPI0, 0x70);
		SPI_Swap(SPI0, 0x60);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Filter for impossible CAN ID */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x02);
		SPI_Swap(SPI0, 0x08);
		SPI_Swap(SPI0, 0x3F);
		SPI_Swap(SPI0, 0x0F);
		SPI_Swap(SPI0, 0xFF);
		SPI_Swap(SPI0, 0xFF);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Mask for impossible CAN ID */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x02);
		SPI_Swap(SPI0, 0x24);
		SPI_Swap(SPI0, 0xFF);
		SPI_Swap(SPI0, 0xFF);
		SPI_Swap(SPI0, 0xFF);
		SPI_Swap(SPI0, 0xFF);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* CANCTRL - Set to Normal Mode */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, 0x02);
		SPI_Swap(SPI0, 0x0F);
		SPI_Swap(SPI0, 0x00);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		init = True;
	}

	/* Transmit "proper" CAN messages only when the Mode is GO */
	//if(System_Mode_G == Go || System_Mode_G == Game_Over)
	{
		/* Pull CS pin low - indicates start of transmission */
		/* Load CAN message into TX0: Left Motor */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, LOAD_TX0_INS);
		spi_buffer_CAN(canIDLf, 8, candataLf);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Load CAN message into TX1: Right Motor */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, LOAD_TX1_INS);
		spi_buffer_CAN(canIDRt, 8, candataRt);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Load CAN message into TX2: Servo controls */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, LOAD_TX2_INS);
		spi_buffer_CAN(0x05A5EEEE, 8, candataEnc);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Tx0 buffer full so request to send */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, RTS_TX0_INS);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Tx1 buffer full so request to send */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, RTS_TX1_INS);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Tx2 buffer full so request to send */
		GPIO_Write(CS_PIN, GPIO_LOW);
		SPI_Swap(SPI0, RTS_TX2_INS);
		GPIO_Write(CS_PIN, GPIO_HIGH);

		/* Check Rx buffers for incoming message */
		spi_receive_CAN(READ_RX0_INS, &bufferRX0);
	}

}

/**
 * Send CAN message into SPI buffer
 * @param EXT_ID Transmission ID for the CAN message.
 * @param length The size of the CAN data message.
 * @param Data_array The CAN data.
 */
void spi_buffer_CAN(uint32_t EXT_ID, uint8_t length, uint8_t Data_array[])
{
	uint32_t Byte_counter;

	/* Pad ID with LOAD_TX0_INS info (see MCP2515 datasheet) */
	build_EXT_ID_bytes(EXT_ID, EXT_ID_Bytes, length);

	/* Send padded CAN ID */
	for (Byte_counter = 0; Byte_counter < 5; Byte_counter++)
	{
		SPI_Swap(SPI0, EXT_ID_Bytes[Byte_counter]);
	}

	/* Send CAN Data */
	for (Byte_counter = 0; Byte_counter < length; Byte_counter++)
	{
		SPI_Swap(SPI0, Data_array[Byte_counter]);
	}
}

/**
 * Processes received CAN messages
 * @param buffer_ins The SPI buffer receive instruction.
 */
void spi_receive_CAN(uint32_t buffer_ins, Receive_Buffers * buffer)
{
	uint32_t Byte_counter, length;// displayID;

	PAD_ID = 0x0;

	/* Pull CS pin low - indicates start of transmission */
	GPIO_Write(CS_PIN, GPIO_LOW);
	SPI_Swap(SPI0, buffer_ins);

	/* Burn 5 Bytes: EXT_ID and Length */
	SPI_Swap(SPI0, 0xFF);
	SPI_Swap(SPI0, 0xFF);
	SPI_Swap(SPI0, 0xFF);
	buffer->ID = SPI_Swap(SPI0, 0xFF);
	SPI_Swap(SPI0, 0xFF);

	/* Known length */
	length = 8;

	/* Extract the CAN data and insert into the Receive Buffer */
	for (Byte_counter = 0; Byte_counter < length; Byte_counter++)
	{
		buffer->CAN_Data[Byte_counter] = SPI_Swap(SPI0, 0xFF);
	}

	GPIO_Write(CS_PIN, GPIO_HIGH);
}

/**
 * Builds the SPI CAN ID
 * @param CAN_ID The CAN ID to be converted
 * @param ID_Array The SPI CAN ID array
 * @param DL The Data Length
 */
void build_EXT_ID_bytes(uint32_t CAN_ID, uint8_t ID_Array[], uint8_t DL)
{
	uint32_t SID, EID, PAD_ID, MaskedID;
	int8_t Byte;

	PAD_ID = 0x0;

	/* Mask 4 bytes of CAN ID */
	MaskedID = CAN_ID & 0xFFFFFFFF;

	/* Separate Standard ID portion and shift for extra 3 bits */
	SID = MaskedID & 0x1FFC0000;
	SID <<= 3;

	/* Separate Extended ID portion */
	EID = MaskedID & 0x3FFFF;

	/* Combine Standard ID, 3 padding bits and Extended ID */
	PAD_ID = ((SID | 0x80000) | EID);

	/* Write to byte array */
	for(Byte = 3; Byte >= 0; Byte--)
	{
		ID_Array[Byte] = (PAD_ID & 0xFF);
		PAD_ID >>= 8;
	}

	/* add data length to byte array */
	ID_Array[4] = (DL & 0x0F);

	return;
}
