/**
 *  Ports.h
 *
 *  Configuration file for all the Ports used.
 *
 */

#ifndef PORTS_H_
#define PORTS_H_
#include "Drivers/gpio.h"
#include "Drivers/segment.h"

/*=== INPUTS ===*/
/* Starter Inputs */
static const gpio_pin_t Red_Cord_SW = { .port = 3, .pin = 0 };			/* J4.02; Red Cord */
static const gpio_pin_t Blue_Cord_SW = { .port = 3, .pin = 1 };			/* J4.04; Blue Cord*/
static const gpio_pin_t Reset_Cord_SW = { .port = 1, .pin = 11 };

static const gpio_pin_t Left_SW = { .port = 3, .pin = 3 };				/* J4.06; Left Push switch*/
static const gpio_pin_t Right_SW = { .port = 3, .pin = 5 };				/* J4.08; Right Push switch*/


/* Encoder Inputs */
static const gpio_pin_t EncL_CH_A = { .port = 3, .pin = 10 };			/* J4.15; Left Encoder channel A */
static const gpio_pin_t EncL_CH_B = { .port = 3, .pin = 11 };			/* J4.16; Left Encoder channel B */
static const gpio_pin_t EncR_CH_A = { .port = 3, .pin = 19 };			/* J4.26; Right Encoder channel A */
static const gpio_pin_t EncR_CH_B = { .port = 3, .pin = 18 };			/* J4.25; Right Encoder channel B */

/* Strategy Switches */
static const gpio_pin_t Strategy0 = { .port = 1, .pin = 0 };
static const gpio_pin_t Strategy1 = { .port = 1, .pin = 1 };
static const gpio_pin_t Strategy2 = { .port = 1, .pin = 2 };
static const gpio_pin_t Trigger = { .port = 1, .pin = 10 };			/* Push Button 1 */

/*=== OUTPUTS ===*/
static const gpio_pin_t KILL_ALL = { .port = 3, .pin = 23 };			/* J4.32; Relay Kill switch */

/* SPI Controls */
static const gpio_pin_t CS_PIN = { .port = 4, .pin = 6};				/* J5.09; SPI CS Pin control */
static const gpio_pin_t RST_PIN = { .port = 4, .pin = 2};				/* J5.04; SPI Reset pin */

/* LED indicators */
static const gpio_pin_t LED_Pin = { .port = 0, .pin = 9 };				/* Flashing_LED Heartbeat task. */
static const gpio_pin_t Error_Lamp = { .port = 0, .pin = 8 };			/* Error Lamp */
static const gpio_pin_t Ready_Lamp_Bl = { .port = 0, .pin = 7 };		/* Blue Team Ready Status Lamp */
static const gpio_pin_t Ready_Lamp_Rd = { .port = 0, .pin = 6 };		/* Red Team Ready Status Lamp */
static const gpio_pin_t LED_Pin_SC = { .port = 0, .pin = 5 };			/* Strategy Change. */
static const gpio_pin_t LED_Pin_Opponent = { .port = 0, .pin = 4 };		/* Opponent detection. */
static const gpio_pin_t LED_Pin_LfFd = { .port = 0, .pin = 3 };			/* Motor_Left task.  Black -, Red + */
static const gpio_pin_t LED_Pin_LfBd = { .port = 0, .pin = 2 };			/* Motor_Left task. */
static const gpio_pin_t LED_Pin_RtFd = { .port = 0, .pin = 1 };			/* Motor_Right task. Black +, Red - */
static const gpio_pin_t LED_Pin_RtBd = { .port = 0, .pin = 0 };			/* Motor_Right task. */

/* Seven Segement Displays */
static const segment_digit_t displayA = {.segment = 0x0, .digit = 0x0 };
static const segment_digit_t displayB = {.segment = 0x0, .digit = 0x1 };
static const segment_digit_t displayC = {.segment = 0x0, .digit = 0x2 };
static const segment_digit_t displayD = {.segment = 0x0, .digit = 0x3 };

#endif /* PORTS_H_ */
