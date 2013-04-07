/*
 * An example configuration package for C projects.  This provides
 * constants and configuration for the system and all of its tasks.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "Scheduler/scheduler.h"
#include "Scheduler/interrupt.h"

#include "tte.h"

// Basic configuration settings for the hardware.
static const uint32_t Config_Oscillator_Frequency = 50000000;
static const uint32_t Config_Clock_Divider        = 1;

// The tick period of the scheduler, in microseconds.
static const uint32_t Config_Tick_Period = 1000;

// This must be TRUE in order to use timing analysis.
#define Config_Logging         TRUE
#define Config_Log_Buffer_Size 1000
#define Config_Log_Num_Ticks   100

// This will allow the scheduler to skip unnecessary ticks.
#define Config_Timeline_Scheduler FALSE

// This makes all scheduling operations occur in hardware.
#define Config_Hardware_Scheduler FALSE

// Puts the processor into idle until a task's WCET is complete.
#define Config_Hardware_Sandwich_Delays FALSE

// This will allow tasks to be executed from the ISR function.
#define Config_Hybrid_Tasks TRUE

// This specifies the timer to be used as the accurate timing source
#define Config_Scheduler_Timer_Number     0

// This specifies the interrupt number of the accurate timing source
#define Config_Scheduler_Interrupt_Number TIMER0

// The actual task array, which must be specified in config.c.
extern volatile task_t Tasks[];

// This is obtained directly from the tasks array in config.c.
extern const uint32_t Config_Num_Tasks;

#endif // !CONFIG_H_INCLUDED
