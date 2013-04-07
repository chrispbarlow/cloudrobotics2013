/**
 * CAN_relay.c
 *
 * This task will send the CAN messages to the Motor Controllers and monitor the Proximity sensor output.
 * ---
 * Spec: Send the information from the motor tasks to the Motor Controllers. Monitor the Proximity sensor feedback.
 * ---
 * Period: 2 ticks
 * Offset: 103 ticks
 * Modes: GO
 * ---
 */

#include "can_relay.h"
#include "../../global.h"
#include "../../ports.h"

/* Analyse the Motor task variables */
extern Direction Left_motor_direction_G;
extern uint16_t Left_motor_speed_G;
extern Direction Right_motor_direction_G;
extern uint16_t Right_motor_speed_G;

/* Check the System Mode */
extern Mode System_Mode_G;

/* Motor Speed values */
uint16_t motor_speed_Lf;
uint16_t motor_speed_Rt;

/* Motor direction values */
uint8_t motor_direction_Lf;
uint8_t motor_direction_Rt;

/* CAN parameters */
extern uint32_t canIDLf;
extern uint32_t canIDRt;
extern uint8_t candataLf[8];
extern uint8_t candataRt[8];

/* Latest data held by the Receive buffers */
extern Receive_Buffers bufferRX0;
extern Receive_Buffers bufferRX1;

/* Sensor readings */
extern sensors sensorReadings;

/**
 * Initialisation for the CAN_relay package.
 * This will be called from Tasks_Init by default.
 */
void CAN_relay_Init(void)
{
	/* Initialisation of variables */
	motor_speed_Lf = 0x0;
	motor_speed_Rt = 0x0;
	motor_direction_Lf = 0x0;
	motor_direction_Rt = 0x0;
	canIDLf = 0;
	canIDRt = 0;
	for(uint8_t i = 0; i < 8; i++)
	{
		candataLf[i] = 0;
		candataRt[i] = 0;
	}

	/* Initialisation of Sensor Readings */
	sensorReadings.IRLeft = 0xFF;
	sensorReadings.IRRight = 0xFF;
	sensorReadings.USFwd = 0xFF;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Retrieves and translates the Speed and Direction from the Motor Tasks into CAN messages.
 */
void CAN_relay_Update(void)
{
	/* If the System is in GO mode, perform the calculations and translation */
	//if(System_Mode_G == Go)
	{
		/* Read CAN buffer */
		readCAN();

		/* Motor direction control */
		switch(Left_motor_direction_G)
		{
		case Forwards:
			motor_direction_Lf = 0x1;
			break;
		case Reverse:
			motor_direction_Lf = 0x2;
			break;
		case Off:
		default:
			motor_direction_Lf = 0x0;
			break;
		}

		switch(Right_motor_direction_G)
		{
		case Forwards:
			motor_direction_Rt = 0x1;
			break;
		case Reverse:
			motor_direction_Rt = 0x2;
			break;
		case Off:
		default:
			motor_direction_Rt = 0x0;
			break;
		}

		/* Motor speed value has a resolution of 0.5 RPM per bit; multiply value by 2 */
		motor_speed_Lf = Left_motor_speed_G * 2;
		motor_speed_Rt = Right_motor_speed_G * 2;

		if(Opponent_detected == True)
		{
			motor_direction_Rt = 0x0;
			motor_direction_Lf = 0x0;
		}


		/* Construct the CAN messages based on the information */
		constructCANLf();
		constructCANRt();
	}
	/* Force the Robot to stop by flooding the CAN with the STOP message */
	//else
	{
		//constructStopCAN();
	}
}

void readCAN(void)
{
	/* Check to see data received from Proximity sensor */
	sensorReadings.IRLeft = bufferRX0.CAN_Data[3];
	sensorReadings.IRRight = bufferRX0.CAN_Data[5];
	sensorReadings.USFwd = ((((bufferRX0.CAN_Data[6] & 0x3F) << 6) | (bufferRX0.CAN_Data[7] & 0x3F)) & 0x0FFF);
}

void constructCANLf(void)
{
	/* CAN_ID
	 * Byte 0: Priority. (Bits 3,4,5) [Bits 0,1,2,6,7 excluded]
	 * Byte 1: PDU1 Proprietary A identifier for peer-to-peer communications.
	 * Byte 2: Destination Peer Address: Motor Controller Address. Configurable using MC configuration utility.
	 * Byte 3: Source Peer Address.
	 */
	canIDLf = 0x14EFD000;

	/* CAN_Data
	 * Byte 0: Motor direction
	 * Byte 2: Motor speed LO_BYTE
	 * Byte 3: Motor speed HI_BYTE
	 */
	candataLf[0] = motor_direction_Lf & 0x3;
	candataLf[2] = motor_speed_Lf & 0xFF;
	candataLf[3] = motor_speed_Lf >> 8;
}

void constructCANRt(void)
{
	canIDRt = 0x10EFD100;
	candataRt[0] = motor_direction_Rt & 0x3;
	candataRt[2] = motor_speed_Rt & 0xFF;
	candataRt[3] = motor_speed_Rt >> 8;
}

/**
 * Halts the Robot.
 */
void constructStopCAN(void)
{
	canIDLf = 0x10EFD000;
	candataLf[0] = 0x0;
	candataLf[2] = 0x0;
	candataLf[3] = 0x0;

	canIDRt = 0x14EFD100;
	candataRt[0] = 0x0;
	candataRt[2] = 0x0;
	candataRt[3] = 0x0;
}
