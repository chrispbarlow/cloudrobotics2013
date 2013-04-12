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

extern sensors sensorReadings;

/* Latest data held by the Receive buffers */
extern Receive_Buffers bufferRX0;


/* Motor control values */
extern Bearing movement_G;
extern Direction Right_motor_direction_G;
extern uint16_t Right_motor_speed_G;

/* Global count of the Encoder Wheels */
extern int32_t WheelCounts_Right_G;
extern Bool Course_correction_Rt;
extern Bool Course_correction_Lt;
extern int32_t Motor_Rt_Enc_Track;
extern Bool End_Of_MoveRt;

/* Internal tracker of Encoder counts */
int32_t Enc_Rt_Old;
int32_t Enc_Rt_Diff;

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
	if(System_Mode_G == Go)
	{
		/* Calculate the absolute value of encoder counts since previous update */
		Enc_Rt_Diff = (WheelCounts_Right_G - Enc_Rt_Old);

		/* Encoder counting */
		if(movement_G == Fd || movement_G == Lf)
		{
			Motor_Rt_Enc_Track += Enc_Rt_Diff;
		}
		else if(movement_G == Bd || movement_G == Rt)
		{
			Motor_Rt_Enc_Track -= Enc_Rt_Diff;
		}

		Enc_Rt_Old = WheelCounts_Right_G;

		/* Script reader */
		switch(movement_G)
		{
		case Fd:
			Right_motor_direction_G = Forwards;
			speedControlRight(Course_correction_Lf, Course_correction_Rt);
			break;
		case Lf:
			Right_motor_direction_G = Forwards;
			Right_motor_speed_G = CRAWL_SPEED-2;
			break;
		case Bd:
		case Rt:
			Right_motor_direction_G = Reverse;
			Right_motor_speed_G = CRAWL_SPEED-2;
			break;
		case Stp:
		default:
			Right_motor_speed_G = 0;
			Motor_Rt_Enc_Track = 0;
			Enc_Rt_Diff = 0;
			Enc_Rt_Old = 0;
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

uint16_t speedControlRight(Bool correctionL, Bool correctionR)
{
	static uint16_t speed;

	if(Right_motor_speed_G == 0)
	{
		Right_motor_speed_G = CRAWL_SPEED;
	}

	if((correctionL == False) && (correctionR == True))
	{
		Right_motor_speed_G++;
	}
	else if((correctionL == True) && (correctionR == False))
	{
		Right_motor_speed_G--;
	}

	if(Right_motor_speed_G < (CRAWL_SPEED - CORRECTION_VALUE_LOWER))
	{
		Right_motor_speed_G = (CRAWL_SPEED - CORRECTION_VALUE_LOWER);
	}

	if(Right_motor_speed_G > (CRAWL_SPEED + CORRECTION_VALUE_UPPER))
	{
		Right_motor_speed_G = CRAWL_SPEED + CORRECTION_VALUE_UPPER;
	}

	return speed;
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
		speed -= 1;
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

	if(( Right_motor_direction_G == Reverse && Right_motor_speed_G != 0) || (Course_correction_Rt == True))
	{
		GPIO_Write(LED_Pin_RtBd, GPIO_HIGH);
	}
	else
	{
		GPIO_Write(LED_Pin_RtBd, GPIO_LOW);
	}

//	Segment_Write(displayA, (sensorReadings.USFwd & 0x000F) >> 0);
//	Segment_Write(displayB, (sensorReadings.USFwd & 0x00F0) >> 4);
//	Segment_Write(displayC, (bufferRX0.ID & 0x000F) >> 0);

	Segment_Write(displayA, (sensorReadings.IRRight & 0x000F) >> 0);
	Segment_Write(displayB, (sensorReadings.IRRight & 0x00F0) >> 4);
	Segment_Write(displayC, (sensorReadings.IRLeft & 0x000F) >> 0);
	Segment_Write(displayD, (sensorReadings.IRLeft & 0x00F0) >> 4);

}
