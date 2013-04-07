/**
 * Motor_Left.c
 *
 * This task will control the speed of the Left Main Wheel Motor.
 * ---
 * Spec: The Update method should output the RPM speed for the Motor
 * ---
 * Period: 10 ticks
 * Offset:
 * WCET:
 * ---
 */

#ifndef MOTOR_LEFT_H_INCLUDED
#define MOTOR_LEFT_H_INCLUDED

#include "../../global.h"
/**
 * Initialisation for the Motor_Left package.
 * This will be called from Tasks_Init by default.
 */
void Motor_Left_Init(void);

/**
 * Motor_Left controller operation
 */
void Motor_Left_Update(void);
uint16_t rampDownLeft(int32_t, int32_t, Bool);
void visualsLf(void);

#endif // !MOTOR_LEFT_H_INCLUDED
