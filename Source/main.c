/*
 * Application entry point for a project that does nothing...
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "Tasks/tasks.h"
#include "Scheduler/scheduler.h"
#include "config.h"
#include "tte.h"

#include <tte/target.h>

/**
 * Application entry point.
 *
 * This function should never exit, as embedded systems do not have an
 * operating system to return to.
 */
int main(void)
{

#ifdef TESTING_MODE
	Testing_Init();
	Testing_Start();
	while(TRUE)
	{

	}
	return 0;
#endif

	Target_Mapping_Init();

	Scheduler_Init(Config_Tick_Period);
	Tasks_Init();

	Scheduler_Start();

	while (TRUE)
	{
		Scheduler_Dispatch();
	}

	// Should never return -- no OS to go back to!
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
/**
 * Halts execution with an infinite loop. Only called when a debug assertion
 * is failed - this is removed from release builds and should never be called
 * in production code.
 *
 * \param[in]	file	The name of the file where the assertion failed.
 * \param[in]	line	The line number where the assertion failed.
 */
void DEBUG_Assert_Failed(TTE_UNUSED char* file, TTE_UNUSED int line)
{
	while (TRUE)
	{
		// Do nothing -- trap for failed assertions!
	}
}
#endif
