/*
 * Simple dummy task created by the automatic add task functionality.
 */

#ifndef MOTOR_RIGHT_H_INCLUDED
#define MOTOR_RIGHT_H_INCLUDED

#include "../../global.h"
/**
 * Initialisation for the Motor_Right package.
 * This will be called from Tasks_Init by default.
 */
void Motor_Right_Init(void);

/**
 * Dummy task with a software delay.  This delay can be 1000 cycles
 * or a random value up to that maximum - #define the preprocessor symbol
 * DUMMY_TASK_VARIABLE_EXECUTION_TIMES to alter this behaviour.
 */
void Motor_Right_Update(void);
uint16_t rampDownRight(int32_t, int32_t, Bool);
void visualsRt(void);

#endif // !MOTOR_RIGHT_H_INCLUDED
