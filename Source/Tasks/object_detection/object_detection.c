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
/**
 * Initialisation for the object_detection package.
 * This will be called from Tasks_Init by default.
 */
void object_detection_Init(void)
{
	objectFollower = Fd;
	nextMove = Fd;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Controls the robot movement based on readings from the distance sensors.
 */
void object_detection_Update(void)
{
	static uint16_t stop = 0;
	Bool detection = False;

	if(movement_G == Stp)
	{
		if(++stop >= HAMMER_TIME)
		{
			WheelCounts_Right_G = 0;
			WheelCounts_Left_G = 0;
			movement_G = nextMove;
			stop = 0;
		}
	}
	else if((movement_G == Fd) && (nextMove != Fd))
	{
		if(++stop >= 12*HAMMER_TIME)
		{
			movement_G = nextMove;
			nextMove = Fd;
			stop = 0;
		}
	}
	else
	{
		if((sensorReadings.IRLeft-sensorReadings.IRRight > IR_NOISE)||(sensorReadings.IRRight-sensorReadings.IRLeft > IR_NOISE))
		{
			if((sensorReadings.IRLeft < sensorReadings.IRRight) && ((sensorReadings.IRLeft < IR_MIN)||((sensorReadings.USFwd < US_MIN) && (sensorReadings.USFwd > US_NOISE))))
			{
				detection = True;
//				if(movement_G != Rt)
//				{
//					movement_G = Stp;
//				}
				movement_G = Rt;
				objectFollower = Rt;
			}
			else if((sensorReadings.IRLeft > sensorReadings.IRRight) && ((sensorReadings.IRRight < IR_MIN)||((sensorReadings.USFwd < US_MIN) && (sensorReadings.USFwd > US_NOISE))))
			{
				detection = True;
//				if(movement_G != Lf)
//				{
//					movement_G = Stp;
//				}
				movement_G = Lf;
				objectFollower = Lf;
			}
		}
		else if(((sensorReadings.USFwd < US_MIN) && (sensorReadings.USFwd > US_NOISE))&&(nextMove == Fd))
		{
			detection = True;
			if(objectFollower == Fd)
			{
				objectFollower = Lf;
			}
//			if(movement_G != objectFollower)
//			{
//				movement_G = Stp;
//			}

			movement_G = objectFollower;
		}

		if((detection == False)&&(movement_G != Fd)&&(nextMove == Fd))
		{
			movement_G = Stp;
		}
		else if(detection == False)
		{
			switch(objectFollower)
			{
			case Rt:
				if(sensorReadings.IRLeft > IR_MAX)
				{
					detection = True;
//					if(movement_G != Lf)
//					{
//						movement_G = Stp;
//					}
					nextMove = Lf;
					movement_G = Fd;
				}
				else
				{
					nextMove = Fd;
				}
				break;

			case Lf:
				if(sensorReadings.IRRight > IR_MAX)
				{
					detection = True;
//					if(movement_G != Rt)
//					{
//						movement_G = Stp;
//					}
					nextMove = Rt;
					movement_G = Fd;
				}
				else
				{
					nextMove = Fd;
				}
				break;
			default:
				nextMove = Fd;
				break;
			}
		}
	}
}
