/*
 * Simple dummy task created by the automatic add task functionality.
 */

#include "../../tte.h"
#include "../../global.h"

#include "object_detection.h"

/* Sensor readings */
extern sensors sensorReadings;

extern Bearing movement_G;
/**
 * Initialisation for the object_detection package.
 * This will be called from Tasks_Init by default.
 */
void object_detection_Init(void)
{
	// Initialisation code goes here!
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Controls the robot movement based on readings from the distance sensors.
 */
void object_detection_Update(void)
{
	if((sensorReadings.IRLeft < sensorReadings.IRRight) && ((sensorReadings.IRLeft < IR_MIN)||(sensorReadings.USFwd < US_MIN)))
	{
		movement_G = Rt;
	}
	else if((sensorReadings.IRLeft > sensorReadings.IRRight) && ((sensorReadings.IRLeft < IR_MIN)||(sensorReadings.USFwd < US_MIN)))
	{
		movement_G = Lf;
	}
//	else if(sensorReadings.USFwd < US_MIN)
//	{
//		movement_G = Rt;
//	}
	else
	{
		movement_G = Fd;
	}

}
