/*
 * Simple dummy task created by the automatic add task functionality.
 */

#ifndef COURSE_CORRECTOR_H_INCLUDED
#define COURSE_CORRECTOR_H_INCLUDED

/**
 * Initialisation for the Course_Corrector package.
 * This will be called from Tasks_Init by default.
 */
void Course_Corrector_Init(void);

/**
 * Dummy task with a software delay.  This delay can be 1000 cycles
 * or a random value up to that maximum - #define the preprocessor symbol
 * DUMMY_TASK_VARIABLE_EXECUTION_TIMES to alter this behaviour.
 */
void Course_Corrector_Update(void);

#endif // !COURSE_CORRECTOR_H_INCLUDED
