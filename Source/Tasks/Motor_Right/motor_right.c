/**
 * Motor_Right.c
 *
 * This task will control the speed and direction of the Right Main Wheel Motor
 * based on the Script created by the Script Keeper.
 * ---
 * Spec: The Update method should set the RPM speed and motor direction.
 * ---
 * Period: 2 ticks
 * Offset: 103 ticks
 * Modes: GO
 * ---
 */

#include <stdlib.h>
#include "motor_right.h"
#include "../../global.h"
#include "../../ports.h"

/*= Task Parameters =*/
/* Global mode */
extern Mode System_Mode_G;
extern script_path Script_G;
extern uint8_t Script_no_G;
uint8_t Script_no_check_Rt;

/* Motor control values */
extern Direction Right_motor_direction_G;
extern uint16_t Right_motor_speed_G;

/* Global count of the Encoder Wheels */
extern int32_t WheelCounts_Right_G;
extern Bool Course_correction_Rt;
extern int32_t Motor_Rt_Enc_Track;
extern Bool End_Of_MoveRt;

/* Internal tracker of Encoder counts */
int32_t Enc_Rt_Old;
int32_t Enc_Rt_Diff;
uint16_t bottleTimeoutRt;
uint16_t scriptTimeoutRt;

/**
 * Initialisation for the Motor_Right package.
 * This will be called from Tasks_Init by default.
 */
void Motor_Right_Init(void)
{
	/* Task initialisation */
	Right_motor_direction_G = Off;
	Right_motor_speed_G = 0;
	Motor_Rt_Enc_Track = 0;
	Script_no_check_Rt = 0;

	End_Of_MoveRt = False;
	bottleTimeoutRt = 0;
	scriptTimeoutRt = 0;

	GPIO_Set_Direction(LED_Pin_RtFd, GPIO_OUTPUT);
	GPIO_Write(LED_Pin_RtFd, GPIO_LOW);
	GPIO_Set_Direction(LED_Pin_RtBd, GPIO_OUTPUT);
	GPIO_Write(LED_Pin_RtBd, GPIO_LOW);
	GPIO_Set_Direction(Right_SW, GPIO_INPUT);

	Segment_Enable(displayA);
	Segment_Write(displayA, 0x0);
	Segment_Clear_Decimal(displayA);
	Segment_Enable(displayB);
	Segment_Write(displayB, 0x0);
	Segment_Clear_Decimal(displayB);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Motor_Right controller operation
 */
void Motor_Right_Update(void)
{
	Bearing moveRt;

	if(System_Mode_G == Go)
	{
		moveRt = Script_G.Movement[Script_no_G];

		/* Calculate the absolute value of encoder counts since previous update */
		Enc_Rt_Diff = (WheelCounts_Right_G - Enc_Rt_Old);

		/* Encoder counting */
		if(moveRt == Fd || moveRt == Lf)
		{
			Motor_Rt_Enc_Track += Enc_Rt_Diff;
		}
		else if(moveRt == Bd || moveRt == Rt)
		{
			Motor_Rt_Enc_Track -= Enc_Rt_Diff;
		}

		Enc_Rt_Old = WheelCounts_Right_G;

		/* Check to see if the next step in the Script is to be executed */
		if(Script_no_check_Rt != Script_no_G)
		{
			Script_no_check_Rt = Script_no_G;
			Motor_Rt_Enc_Track = 0;
			scriptTimeoutRt = 0;
		}
		else
		{
			Script_no_check_Rt = Script_no_G;
			scriptTimeoutRt++;
		}

		if(scriptTimeoutRt > 5000)
		{
			moveRt = DeployLf;
		}

		/* Script reader */
		switch(moveRt)
		{
		case Fd:
		case Lf:
			/* Check the encoder tracker has reached the target count */
			if(Motor_Rt_Enc_Track < (Script_G.Enc_counts[Script_no_G] - BUFFER_COUNT))
			{
				End_Of_MoveRt = False;
				Right_motor_direction_G = Forwards;
				Right_motor_speed_G = rampDownRight(Script_G.Enc_counts[Script_no_G], Motor_Rt_Enc_Track, Course_correction_Rt);
			}
			/* Check for overshoot */
			else if(Motor_Rt_Enc_Track > (Script_G.Enc_counts[Script_no_G] + BUFFER_COUNT))
			{
				End_Of_MoveRt = False;
				Right_motor_direction_G = Reverse;
				Right_motor_speed_G = rampDownRight(Script_G.Enc_counts[Script_no_G], Motor_Rt_Enc_Track, Course_correction_Rt);
			}
			/* Stop the motor once the target is reached */
			else
			{
				End_Of_MoveRt = True;
				Right_motor_speed_G = 0;
				bottleTimeoutRt = 0;
			}
			break;
		case Bd:
		case Rt:
			if(
					Motor_Rt_Enc_Track < (Script_G.Enc_counts[Script_no_G] - BUFFER_COUNT)
					//&&
					//(GPIO_Get(Right_SW) != GPIO_LOW)
				)
			{
				End_Of_MoveRt = False;
				Right_motor_direction_G = Reverse;
				Right_motor_speed_G = rampDownRight(Script_G.Enc_counts[Script_no_G], Motor_Rt_Enc_Track, Course_correction_Rt);
			}
			else if(Motor_Rt_Enc_Track > (Script_G.Enc_counts[Script_no_G] + BUFFER_COUNT))
			{
				End_Of_MoveRt = False;
				Right_motor_direction_G = Forwards;
				Right_motor_speed_G = rampDownRight(Script_G.Enc_counts[Script_no_G], Motor_Rt_Enc_Track, Course_correction_Rt);
			}
			else
			{
				End_Of_MoveRt = True;
				Right_motor_speed_G = 0x0;
				bottleTimeoutRt = 0;
			}
			break;
		case Press:
			End_Of_MoveRt = False;
			Right_motor_direction_G = Reverse;
			Right_motor_speed_G = BOTTLE_SPEED;
			if(
					//(Motor_Rt_Enc_Track > Script_G.Enc_counts[Script_no_G])
					//||
					(++bottleTimeoutRt > Script_G.Enc_counts[Script_no_G])
			)
			{
				End_Of_MoveRt = True;
				Right_motor_speed_G = 0x0;
			}
			break;
		case DeployRt:
		case DeployLf:
			End_Of_MoveRt = True;
			Right_motor_speed_G = 0x0;
			break;
		case Stp:
		default:
			End_Of_MoveRt = False;
			Right_motor_speed_G = 0;
			break;
		}
	}
	else
	{
		Right_motor_direction_G = Off;
		Right_motor_speed_G = 0;
		Motor_Rt_Enc_Track = 0;
		Enc_Rt_Diff = 0;
		Enc_Rt_Old = 0;
	}

	visualsRt();
}

uint16_t rampDownRight(int32_t scriptCount, int32_t currentCount, Bool correction)
{
	double posDelta;
	int16_t speed;
	uint16_t result;

	if(scriptCount > currentCount)
	{
		posDelta = scriptCount - currentCount;
	}
	else if(scriptCount < currentCount)
	{
		posDelta = currentCount - scriptCount;
	}
	else
	{
		posDelta = 0.0;
	}

	speed = (RPM_FACTOR * posDelta) + RPM_FLOOR;

	if(speed >= RPM_LIMIT)
	{
		speed = RPM_LIMIT;
	}

	if(correction == True && speed > CORRECTION_THRESHOLD)
	{
		speed -= CORRECTION_VALUE;
	}

	if (speed <= CRAWL_SPEED)
	{
		speed = CRAWL_SPEED;
	}

	result = (uint16_t)(speed);
	return result;
}

void visualsRt(void)
{
	if(System_Mode_G == Go || System_Mode_G == Game_Over)
	{
		Segment_Set_Decimal(displayA);
		Segment_Set_Decimal(displayB);
	}
	else
	{
		Segment_Clear_Decimal(displayA);
		Segment_Clear_Decimal(displayB);
	}

	if(
		Right_motor_direction_G == Forwards
		&& Right_motor_speed_G != 0
		)
	{
		GPIO_Write(LED_Pin_RtFd, GPIO_HIGH);
	}
	else
	{
		GPIO_Write(LED_Pin_RtFd, GPIO_LOW);
	}

	if(
		Right_motor_direction_G == Reverse
		&& Right_motor_speed_G != 0
		)
	{
		GPIO_Write(LED_Pin_RtBd, GPIO_HIGH);
	}
	else
	{
		GPIO_Write(LED_Pin_RtBd, GPIO_LOW);
	}

	//Segment_Write(displayA, (Motor_Rt_Enc_Track & 0x000F) >> 0);
	//Segment_Write(displayB, (Motor_Rt_Enc_Track & 0x00F0) >> 4);
}