/*
 * Simple dummy task created by the automatic add task functionality.
 */

#ifndef READ_ENC_H_INCLUDED
#define READ_ENC_H_INCLUDED

/**
 * Initialisation for the Read_Enc package.
 * This will be called from Tasks_Init by default.
 */
void Read_Enc_Init(void);

/**
 * Dummy task with a software delay.  This delay can be 1000 cycles
 * or a random value up to that maximum - #define the preprocessor symbol
 * DUMMY_TASK_VARIABLE_EXECUTION_TIMES to alter this behaviour.
 */
void Read_Enc_Update(void);

#endif // !READ_ENC_H_INCLUDED
