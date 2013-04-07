/*
 * Basic scheduler functionality for Time-Triggered Co-operative (TTC)
 * and Time-Triggered Hybrid (TTH) scheduling of task functions.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef SCHEDULER_H_INCLUDED
#define SCHEDULER_H_INCLUDED

#include "../tte.h"

/**
 * Initialises the scheduler with a given tick interval, in microseconds.
 *
 * \param[in]	tick_us		The period of the main scheduler tick.
 */
void Scheduler_Init(const uint32_t tick_us);

/**
 * Starts the scheduler.
 */
void Scheduler_Start(void);

/**
 * Disables the scheduler and any interrupts and then enters idle mode.
 */
void Shutdown_System(void);

/**
 * Dispatches any ready tasks.  This should be called in a loop in Main.
 */
void Scheduler_Dispatch(void);

/**
 * Handles all system ticks and dispatches any released hybrid tasks.
 */
void Scheduler_Tick(void);

/**
 * Represents a task as a pointer to a procedure with no parameters.
 */
typedef void (*task_function_t)(void);

/**
 * Every task is represented as a function, a period, an initial delay
 * and a flag indicating whether it's launched from main or the ISR.
 */
typedef struct
{
	task_function_t task_function;
	uint32_t        period_reload;
	int32_t         delay_counter;
	uint32_t		wcet_estimate;
	task_function_t recovery_task;
	boolean_t       exec_from_isr;
} task_t;

#endif // !SCHEDULER_H_INCLUDED
