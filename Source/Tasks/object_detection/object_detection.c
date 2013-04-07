/*
 * Simple dummy task created by the automatic add task functionality.
 */

#include "../../tte.h"
#include "../../global.h"

#include "object_detection.h"

/* Sensor readings */
extern sensors sensorReadings;

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
	movement_G = Fd;

	if((sensorReadings.IRLeft-sensorReadings.IRRight > IR_NOISE)||(sensorReadings.IRRight-sensorReadings.IRLeft > IR_NOISE))
	{
		if((sensorReadings.IRLeft < sensorReadings.IRRight) && ((sensorReadings.IRLeft < IR_MIN)||(sensorReadings.USFwd < US_MIN)))
		{
			movement_G = Rt;
			objectFollower = Rt;
		}
		else if((sensorReadings.IRLeft > sensorReadings.IRRight) && ((sensorReadings.IRLeft < IR_MIN)||(sensorReadings.USFwd < US_MIN)))
		{
			movement_G = Lf;
			objectFollower = Lf;
		}
	}
	else if(sensorReadings.USFwd < US_MIN)
	{
		movement_G = objectFollower;
	}

}
