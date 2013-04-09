/*
 * Simple dummy task created by the automatic add task functionality.
 */

#include "../../tte.h"
#include "../../global.h"

#include "object_detection.h"

/* Sensor readings */
extern sensors sensorReadings;

extern int32_t WheelCounts_Right_G;
extern int32_t WheelCounts_Left_G;

extern Bearing movement_G;
Bearing objectFollower;
Bearing nextMove;

typedef enum{STRAIGHT,AVOID_L,AVOID_R,FOLLOW_L,FOLLOW_R,WAIT} avoidance_state_t;
avoidance_state_t avoidance;
avoidance_state_t lastAvoidance;
/**
 * Initialisation for the object_detection package.
 * This will be called from Tasks_Init by default.
 */
void object_detection_Init(void)
{
	objectFollower = Fd;
	nextMove = Fd;
	avoidance = STRAIGHT;
	lastAvoidance = STRAIGHT;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Controls the robot movement based on readings from the distance sensors.
 */
void object_detection_Update(void)
{
	static uint16_t stop = 0;

	switch(avoidance)
	{
	case STRAIGHT:
		movement_G = Fd;

		if((sensorReadings.IRLeft-sensorReadings.IRRight > IR_NOISE)||(sensorReadings.IRRight-sensorReadings.IRLeft > IR_NOISE))
		{
			if((sensorReadings.IRLeft < sensorReadings.IRRight) && ((sensorReadings.IRLeft < IR_MIN)||((sensorReadings.USFwd < US_MIN) && (sensorReadings.USFwd > US_NOISE))))
			{
				avoidance = AVOID_L;
			}
			else if((sensorReadings.IRLeft > sensorReadings.IRRight) && ((sensorReadings.IRRight < IR_MIN)||((sensorReadings.USFwd < US_MIN) && (sensorReadings.USFwd > US_NOISE))))
			{
				avoidance = AVOID_R;
			}
		}
		else if(((sensorReadings.USFwd < US_MIN) && (sensorReadings.USFwd > US_NOISE))&&(nextMove == Fd))
		{
			if(lastAvoidance == STRAIGHT)
			{
				lastAvoidance = AVOID_R;
			}
			avoidance = lastAvoidance;
		}
		else if((lastAvoidance == AVOID_L) && (sensorReadings.IRLeft > IR_MAX))
		{
			if(++stop >= 12*HAMMER_TIME)
			{
				avoidance = FOLLOW_L;
				stop = 0;
			}
		}
		else if((lastAvoidance == AVOID_R) && (sensorReadings.IRRight > IR_MAX))
		{
			if(++stop >= 12*HAMMER_TIME)
			{
				avoidance = FOLLOW_R;
				stop = 0;
			}
		}
		break;
	case AVOID_L:
		movement_G = Rt;
		lastAvoidance = AVOID_L;
		if((sensorReadings.IRLeft >= IR_MIN) && (sensorReadings.USFwd >= US_MIN))
		{
			avoidance = WAIT;
		}
		break;
	case AVOID_R:
		movement_G = Lf;
		lastAvoidance = AVOID_R;
		if((sensorReadings.IRRight >= IR_MIN) && (sensorReadings.USFwd >= US_MIN))
		{
			avoidance = WAIT;
		}
		break;
	case FOLLOW_L:
		movement_G = Lf;
		if(sensorReadings.IRLeft <= IR_MAX)
		{
			avoidance = WAIT;
		}
		break;
	case FOLLOW_R:
		movement_G = Rt;
		if(sensorReadings.IRRight <= IR_MAX)
		{
			avoidance = WAIT;
		}
		break;
	case WAIT:
		movement_G = Stp;
		if(++stop >= HAMMER_TIME)
		{
			avoidance = STRAIGHT;
			stop = 0;
		}
		break;
	}
}
