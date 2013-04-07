/**
 * Message_Sender.h.
 */

#ifndef MESSAGE_SENDER_H_INCLUDED
#define MESSAGE_SENDER_H_INCLUDED

/*= Instruction values for the SPI =*/
/* Transmission Buffer loader instructions */
#define LOAD_TX0_INS (0x40)
#define LOAD_TX1_INS (0x42)
#define LOAD_TX2_INS (0x44)

/* Receive Buffer read instructions */
#define READ_RX0_INS (0x90)
#define READ_RX1_INS (0x94)

/* Request to send buffered Transmit messages */
#define RTS_TX0_INS (0x81)
#define RTS_TX1_INS (0x82)
#define RTS_TX2_INS (0x84)

/* Check Receive Buffer instruction*/
#define RX_STATUS_INS (0xB0)

/* SPI Port */
#define SPI0 (0)

#include "../../global.h"


/**
 * Initialisation for the Message_Sender package.
 * This will be called from Tasks_Init by default.
 */
void Message_Sender_Init(void);

/**
 * Currently empty task, to be filled in.
 */
void Message_Sender_Update(void);

void spi_buffer_CAN(uint32_t, uint8_t, uint8_t *);
void spi_RX_buffer_status(void);
void spi_receive_CAN(uint32_t, Receive_Buffers *);
int spi_receive_byte(void);
void spi_send_byte(int Byte_to_send);
void build_EXT_ID_bytes(uint32_t, uint8_t *, uint8_t);

#endif // !MESSAGE_SENDER_H_INCLUDED
