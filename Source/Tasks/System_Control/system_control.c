/**
 * System_Control.c
 *
 * Controls the state of the system and provides an emergency exit point
 * ---
 * Spec: The Update method should output the RPM speed for the Motor
 * ---
 * Period: 10 ticks
 * Offset: 0 ticks
 * Modes: ALL
 * ---
 */

#include "../../tte.h"
#include "system_control.h"
#include "../../global.h"
#include "../../ports.h"
#include <stdlib.h>

Mode System_Mode_G;
extern Timer Timer_Status;
extern Bearing movement_G;
extern Team Team_Colour;
extern Bool Script_update;

uint8_t debounce;

/**
 * Initialisation for the System_Control package.
 * This will be called from Tasks_Init by default.
 */
void System_Control_Init(void)
{
	/* Port initialisation */
	GPIO_Set_Direction(Red_Cord_SW, GPIO_INPUT);
	GPIO_Set_Direction(Blue_Cord_SW, GPIO_INPUT);

	GPIO_Set_Direction(Error_Lamp, GPIO_OUTPUT);
	GPIO_Write(Error_Lamp, GPIO_LOW);

	GPIO_Set_Direction(Ready_Lamp_Rd, GPIO_OUTPUT);
	GPIO_Write(Ready_Lamp_Rd, GPIO_LOW);

	GPIO_Set_Direction(Ready_Lamp_Bl, GPIO_OUTPUT);
	GPIO_Write(Ready_Lamp_Bl, GPIO_LOW);

	GPIO_Set_Direction(KILL_ALL, GPIO_OUTPUT);
	GPIO_Write(KILL_ALL, GPIO_HIGH);

	/* State and orientation initialisation */
	System_Mode_G = Ready;
	debounce = 0;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Main system mode control
 */
void System_Control_Update(void)
{
	switch(System_Mode_G)
	{
	/* Wait for pull cord to determine direction */
	case Ready:

		movement_G = Fd;
		System_Mode_G = Go;
		break;

	/* Wait for removal of pull cord to "Go" */
	case Set:
		if(
				(GPIO_Get(Red_Cord_SW) == GPIO_HIGH)
				&&
				(GPIO_Get(Blue_Cord_SW) == GPIO_HIGH)
				&&
				(++debounce > 100)
		)
		{
			System_Mode_G = Go;
		}
		break;

	/* Do nothing */
	case Go:
		GPIO_Write(Ready_Lamp_Rd, GPIO_HIGH);
		debounce = 0;
		break;

	/* Signal E Stop */
	case E_Stop:
		GPIO_Write(KILL_ALL, GPIO_LOW);
		//GPIO_Set_Direction(KILL_ALL, GPIO_INPUT);
		GPIO_Write(Error_Lamp, GPIO_HIGH);
		GPIO_Write(Ready_Lamp_Rd, GPIO_HIGH);
		GPIO_Write(Ready_Lamp_Bl, GPIO_HIGH);
		break;

	/* Signal Game Over */
	case Game_Over:
		GPIO_Write(KILL_ALL, GPIO_LOW);
		//GPIO_Set_Direction(KILL_ALL, GPIO_INPUT);
		GPIO_Write(Ready_Lamp_Rd, GPIO_LOW);
		GPIO_Write(Ready_Lamp_Bl, GPIO_LOW);
		System_Mode_G = Ready; // TODO: Remove!
		break;

	default:
		GPIO_Write(KILL_ALL, GPIO_LOW);
		//GPIO_Set_Direction(KILL_ALL, GPIO_INPUT);
		GPIO_Write(Ready_Lamp_Rd, GPIO_HIGH);
		GPIO_Write(Ready_Lamp_Bl, GPIO_HIGH);
		break;
	}
}
