/*
 * Provides logging functionality for RapidiTTy's timing analysis.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include "../tte.h"

/**
 * Logs the start time of the given task.
 *
 * \param[in]	task_index	The index of the current task.
 */
void Log_Task_Start(const uint32_t task_index);

/**
 * Logs the end time of the given task.
 *
 * \param[in]	task_index	The index of the current task.
 */
void Log_Task_End(const uint32_t task_index);

/**
 * Logs the start time of the given hybrid task.
 *
 * \param[in]	task_index	The index of the current task.
 */
void Log_Hybrid_Start(const uint32_t task_index);

/**
 * Logs the end time of the given hybrid task.
 *
 * \param[in]	task_index	The index of the current task.
 */
void Log_Hybrid_End(const uint32_t task_index);

/**
 * Logs the start time of the timer interrupt handler.
 *
 * \param[in]	task_index	The index of the current task.
 */
void Log_Tick_Start(const uint32_t task_index);

/**
 * Logs the end time of the timer interrupt handler.
 *
 * \param[in]	task_index	The index of the current task.
 */
void Log_Tick_End(const uint32_t task_index);

/**
 * Sets the address of the next task to be executed.
 *
 * \param[in]	in_isr	Specifies if this is called from an ISR or not.
 * \param[in]	start	The start address of the next task to run.
 * \param[in]	finish	The end address of the next task to run.
 */
void Log_Set_Address(const boolean_t in_isr, const uint32_t start,
					 const uint32_t finish);

/**
 * Resets addresses to the default, to reuse watchpoints.
 */
void Log_Reset_Addresses(void);

/**
 * Logs the time at which we enter idle mode.
 */
void Log_Idle(void);

/**
 * Initialises the logging subsystem.
 */
void Log_Init(void);

#endif // !LOG_H_INCLUDED
