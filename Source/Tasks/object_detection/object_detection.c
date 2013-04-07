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

/**
 * Initialisation for the object_detection package.
 * This will be called from Tasks_Init by default.
 */
void object_detection_Init(void)
{
	objectFollower = Rt;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Controls the robot movement based on readings from the distance sensors.
 */
void object_detection_Update(void)
{
	Bool detection = False;

	if((sensorReadings.IRLeft-sensorReadings.IRRight > IR_NOISE)||(sensorReadings.IRRight-sensorReadings.IRLeft > IR_NOISE))
	{
		if((sensorReadings.IRLeft < sensorReadings.IRRight) && ((sensorReadings.IRLeft < IR_MIN)||((sensorReadings.USFwd < US_MIN) && (sensorReadings.USFwd > 5))))
		{
			detection = True;
			if(movement_G != Rt)
			{
				WheelCounts_Right_G = 0;
				WheelCounts_Left_G = 0;
			}
			movement_G = Rt;
			objectFollower = Rt;
		}
		else if((sensorReadings.IRLeft > sensorReadings.IRRight) && ((sensorReadings.IRRight < IR_MIN)||((sensorReadings.USFwd < US_MIN) && (sensorReadings.USFwd > 5))))
		{
			detection = True;
			if(movement_G != Lf)
			{
				WheelCounts_Right_G = 0;
				WheelCounts_Left_G = 0;
			}
			movement_G = Lf;
			objectFollower = Lf;
		}
	}
	else if((sensorReadings.USFwd < US_MIN) && (sensorReadings.USFwd > 5))
	{
		detection = True;
		if(movement_G != objectFollower)
		{
			WheelCounts_Right_G = 0;
			WheelCounts_Left_G = 0;
		}

		movement_G = objectFollower;
	}

	if((detection == False)&&(movement_G != Fd))
	{
		WheelCounts_Right_G = 0;
		WheelCounts_Left_G = 0;

		movement_G = Fd;
	}
}
