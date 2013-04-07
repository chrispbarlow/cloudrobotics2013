/*
 * Simple dummy task created by the automatic add task functionality.
 */

#ifndef SYSTEM_CONTROL_H_INCLUDED
#define SYSTEM_CONTROL_H_INCLUDED

/**
 * Initialisation for the System_Control package.
 * This will be called from Tasks_Init by default.
 */
void System_Control_Init(void);

/**
 * Dummy task with a software delay.  This delay can be 1000 cycles
 * or a random value up to that maximum - #define the preprocessor symbol
 * DUMMY_TASK_VARIABLE_EXECUTION_TIMES to alter this behaviour.
 */
void System_Control_Update(void);

#endif // !SYSTEM_CONTROL_H_INCLUDED
