/**
 * Opponent_Tracker.c
 *
 * This task analyses the sensor information to determine the avoidance strategy.
 * ---
 * Spec: Use sensor information to determine opponent avoidance strategy.
 * ---
 * Period: 2 ticks
 * Offset: 103 ticks
 * Modes: GO
 * ---
 */

#include "../../tte.h"
#include "../../global.h"
#include "../../ports.h"
#include "opponent_tracker.h"

/* System status information */
extern Mode System_Mode_G;
extern script_path Script_G;
extern uint8_t Script_no_G;
extern Bool Opponent_detected;

/* Override motor directions if necessary */
extern Direction Left_motor_direction_G;
extern Direction Right_motor_direction_G;

/* Sensor information */
extern sensors sensorReadings;
uint8_t sensorDebounce;

/**
 * Initialisation for the Opponent_Tracker package.
 * This will be called from Tasks_Init by default.
 */
void Opponent_Tracker_Init(void)
{
	Opponent_detected = False;
	sensorDebounce = 0;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Analyse the Sensor Readings to determine if an opponent is present
 */
void Opponent_Tracker_Update(void)
{
	uint32_t reversible;

	/* Check to see messages received from forward Proximity sensor */
	if(
			(
					(sensorReadings.USFwd < US_THRESHOLD)
					&&
					(sensorReadings.USFwd > US_MIN)
			)
			||
			(sensorReadings.IRLeft > IR_THRESHOLD)
			||
			(sensorReadings.IRRight > IR_THRESHOLD)
		)
	{
		switch(Script_G.Movement[Script_no_G])
		{
		case DeployLf:
		case DeployRt:
		case Press:
		case Stp:
		case Lf:
		case Rt:
			Opponent_detected = False;
			break;
		case Bd:
			if(
					(
							(sensorReadings.IRLeft > IR_THRESHOLD)
							||
							(sensorReadings.IRRight > IR_THRESHOLD)
					)
					//&&
					//(++sensorDebounce > 3)
			)
			{
				Left_motor_direction_G = Off;
				Right_motor_direction_G = Off;
				Opponent_detected = True;
			}
			else
			{
				Opponent_detected = False;
				sensorDebounce = 0;
			}
			break;
		case Fd:
			if(
					(sensorReadings.USFwd < US_THRESHOLD)
					&&
					(sensorReadings.USFwd > US_MIN)
					//&&
					//(++sensorDebounce > 3)
			)
			{
				Left_motor_direction_G = Off;
				Right_motor_direction_G = Off;
				Opponent_detected = True;
			}
			else
			{
				sensorDebounce = 0;
				Opponent_detected = False;
			}
			break;
		default:
			Left_motor_direction_G = Off;
			Right_motor_direction_G = Off;
			Opponent_detected = True;
			break;
		}
	}
	else
	{
		Opponent_detected = False;
	}

	reversible = sensorReadings.USFwd;

	Segment_Write(displayA, (reversible & 0x000F) >> 0);
	Segment_Write(displayB, (reversible & 0x00F0) >> 4);
	Segment_Write(displayC, (reversible & 0x0F00) >> 8);
	Segment_Write(displayD, (reversible & 0xF000) >> 12);

}
