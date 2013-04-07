/**
 * System_Timer.c
 *
 * Keeps track of the time
 * ---
 * Spec: The Task should monitor the elapse of 90 seconds from when the "pull cord" is triggered.
 * ---
 * Period: 20 ticks
 * Offset: 0 ticks
 * Modes: GO
 * ---
 */

#include <tte.h>
#include <stdlib.h>

#include "system_timer.h"
#include "../../global.h"
#include "../../ports.h"
#include "../../config.h"

#define Ninety_Seconds (89000000 / Config_Tick_Period)
uint32_t Init_period_reload;

extern Mode System_Mode_G;
Timer Timer_Status;

/**
 * Initialisation for the System_Timer package.
 * This will be called from Tasks_Init by default.
 */
void System_timer_Init(void)
{
	Timer_Status = Stopped;
	Init_period_reload = Tasks[0].period_reload;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Waits for game to start (from System_Mode_G - switched by System_Control)
 * Changes period_reload to ninety seconds and switches system mode to Game_Over next time it is called
 */
void System_timer_Update(void)
{
	if(System_Mode_G == Go)
	{
		switch(Timer_Status)
		{
			case Stopped:
				Tasks[0].period_reload = Ninety_Seconds;
				Timer_Status = Init;
				break;

			case Init:
				Tasks[0].period_reload = 1;
				Timer_Status = Running;
				break;

			case Running:
			default:
				System_Mode_G = Game_Over;
				break;
		}
	}
	/* Allow System reset */
	else if(System_Mode_G == Ready)
	{
		Timer_Status = Stopped;
	}

}
