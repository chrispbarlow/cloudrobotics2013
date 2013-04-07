/**
 * Motor_Left.c
 *
 * This task will control the speed and direction of the Left Main Wheel Motor
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
#include "motor_left.h"
#include "../../global.h"
#include "../../ports.h"

/*= Task Parameters =*/
/* Global mode */
extern Mode System_Mode_G;

/* Motor control values */
extern Bearing movement_G;
extern Direction Left_motor_direction_G;
extern uint16_t Left_motor_speed_G;

/* Global count of the Encoder Wheels */
extern int32_t WheelCounts_Left_G;
extern Bool Course_correction_Lf;
extern int32_t Motor_Lf_Enc_Track;
extern Bool End_Of_MoveLf;

/* Internal tracker of Encoder counts */
int32_t Enc_Lf_Old;
int32_t Enc_Lf_Diff;

/**
 * Initialisation for the Motor_Left package.
 * This will be called from Tasks_Init by default.
 */
void Motor_Left_Init(void)
{
	/* Task initialisation */
	Left_motor_direction_G = Off;
	Left_motor_speed_G = 0;
	Motor_Lf_Enc_Track = 0;

	GPIO_Set_Direction(LED_Pin_LfFd, GPIO_OUTPUT);
	GPIO_Write(LED_Pin_LfFd, GPIO_LOW);
	GPIO_Set_Direction(LED_Pin_LfBd, GPIO_OUTPUT);
	GPIO_Write(LED_Pin_LfBd, GPIO_LOW);
	GPIO_Set_Direction(Left_SW, GPIO_INPUT);

	Segment_Enable(displayC);
	Segment_Write(displayC, 0x0);
	Segment_Clear_Decimal(displayC);
	Segment_Enable(displayD);
	Segment_Write(displayD, 0x0);
	Segment_Clear_Decimal(displayD);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Motor_Left controller operation.
 */
void Motor_Left_Update(void)
{

	if(System_Mode_G == Go)
	{
		/* Calculate the absolute value of encoder counts since previous update */
		Enc_Lf_Diff = (WheelCounts_Left_G - Enc_Lf_Old);

		/* Encoder counting */
		if(movement_G == Fd || movement_G == Rt)
		{
			Motor_Lf_Enc_Track += Enc_Lf_Diff;
		}
		else if(movement_G == Bd || movement_G == Lf)
		{
			Motor_Lf_Enc_Track -= Enc_Lf_Diff;
		}

		Enc_Lf_Old = WheelCounts_Left_G;

		/* Script reader */
		switch(movement_G)
		{
		case Fd:
		case Rt:
			/* Check the encoder tracker has reached the target count */
			Left_motor_direction_G = Forwards;
			Left_motor_speed_G = CRAWL_SPEED;

			break;
		case Bd:
		case Lf:
			Left_motor_direction_G = Reverse;
			Left_motor_speed_G = CRAWL_SPEED;

			break;
		case Stp:
		default:
			Left_motor_speed_G = 0;
			break;
		}
	}
	else
	{
		Left_motor_direction_G = Off;
		Left_motor_speed_G = 0;
		Motor_Lf_Enc_Track = 0;
		Enc_Lf_Diff = 0;
		Enc_Lf_Old = 0;
	}

	visualsLf();
}

uint16_t rampDownLeft(int32_t scriptCount, int32_t currentCount, Bool correction)
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

void visualsLf(void)
{
	if(System_Mode_G == Game_Over)
	{
		Segment_Set_Decimal(displayC);
		Segment_Set_Decimal(displayD);
	}
	else
	{
		Segment_Clear_Decimal(displayC);
		Segment_Clear_Decimal(displayD);
	}

	if(
		Left_motor_direction_G == Forwards
		&& Left_motor_speed_G != 0
		)
	{
		GPIO_Write(LED_Pin_LfFd, GPIO_HIGH);
	}
	else
	{
		GPIO_Write(LED_Pin_LfFd, GPIO_LOW);
	}

	if((Left_motor_direction_G == Reverse && Left_motor_speed_G != 0	) || (Course_correction_Lf == True))
	{
		GPIO_Write(LED_Pin_LfBd, GPIO_HIGH);
	}
	else
	{
		GPIO_Write(LED_Pin_LfBd, GPIO_LOW);
	}

	Segment_Write(displayC, (Motor_Lf_Enc_Track & 0x000F) >> 0);
	Segment_Write(displayD, (Motor_Lf_Enc_Track & 0x00F0) >> 4);

}
