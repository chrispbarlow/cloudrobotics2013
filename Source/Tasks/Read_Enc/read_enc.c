/**
 * Read_Enc.c
 *
 * Encoder Task
 * ---
 * Spec:
 * ---
 * Period: 1 ticks
 * Offset: 0 ticks
 * Modes: ALL
 * ---
 */
#include "../../global.h"
#include "../../tte.h"
#include "../../ports.h"
#include "read_enc.h"
#include "../../Drivers/gpio.h"

/* Task Parameters */
extern Mode System_Mode_G;

State EncSignal_A[2];
State EncSignal_B[2];
//State EncSignal_I[2];

State Pulse_Flag[2];
Direction WheelDirection[2];
State CountError_Flag[2];

uint32_t RevCount[2];
uint32_t EncoderCount[2];
int32_t WheelCount[2];

/* Global encoder counters */
extern int32_t WheelCounts_Left_G;
extern int32_t WheelCounts_Right_G;

extern script_path Script_G;
extern uint8_t Script_no_G;

/* Left Encoder count states */
extern State Left_A;
extern State Left_B;

/* Right Encoder count states */
extern State Right_A;
extern State Right_B;

uint8_t bufferTime;

/**
 * Initialisation for the Read_Enc package.
 * This will be called from Tasks_Init by default.
 */
void Read_Enc_Init(void)
{
	GPIO_Set_Direction(EncL_CH_A, GPIO_INPUT);
	GPIO_Set_Direction(EncL_CH_B, GPIO_INPUT);
	GPIO_Set_Direction(EncR_CH_A, GPIO_INPUT);
	GPIO_Set_Direction(EncR_CH_B, GPIO_INPUT);

	WheelCounts_Left_G = 0;
	WheelCounts_Right_G = 0;

	EncSignal_A[0] = 0;								/* Signal A from left encoder */
	EncSignal_B[0] = 0;								/* Signal B from left encoder */

	EncSignal_A[1] = 0;								/* Signal A from right encoder */
	EncSignal_B[1] = 0;								/* Signal B from right encoder */

	/* Left */
	RevCount[0] = 0;
	EncoderCount[0] = 0;
	WheelCount[0] = 0;

	/* Right */
	RevCount[1] = 0;
	EncoderCount[1] = 0;
	WheelCount[1] = 0;

	bufferTime = 0;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Reads the value of the Encoders
 */
void Read_Enc_Update(void)
{
	if(System_Mode_G == Go)
	{
		/* Read signals from encoders */
		EncSignal_A[0] = GPIO_Get(EncL_CH_A);	/* Left_A;	Signal A from left encoder */
		EncSignal_B[0] = GPIO_Get(EncL_CH_B);	/* Left_B;	Signal B from left encoder */

		EncSignal_A[1] = GPIO_Get(EncR_CH_A);	/* Right_A;	Signal A from right encoder */
		EncSignal_B[1] = GPIO_Get(EncR_CH_B);	/* Right_B;	Signal B from right encoder */

		/* Loop through both sides */
		for(uint8_t i = 0; i <= 1; i ++)
		{
			if(Pulse_Flag[i] == High)
			{
				/* Direction detection on single signal drop-off */
				if(EncSignal_A[i] != EncSignal_B[i])
				{
					switch(EncSignal_A[i])
					{
						case High:
							WheelDirection[i] = Reverse;
							break;
						case Low:
							WheelDirection[i] = Forwards;
							break;
						default:
							break;
					}
				}

				/* Full cycle detection when both signals dropped off */
				else if((EncSignal_A[i] == Low) && (EncSignal_B[i] == Low))
				{
					switch(WheelDirection[i])
					{
						case Forwards:
							EncoderCount[i] ++;
							break;
						case Reverse:
							EncoderCount[i] --;
							break;
						case Off:
							break;
						default:
							break;
					}
					Pulse_Flag[i] = Low;
				}
			}
			/* Wait for both A and B to be high to raise Pulse_Flag */
			else if((EncSignal_A[i] == High) && (EncSignal_B[i] == High))
			{
				Pulse_Flag[i] = High;
			}
			/* ET Padding to reduce jitter */
			else
			{
				EncoderCount[i] = EncoderCount[i];
				WheelDirection[i] = WheelDirection[i];
				Pulse_Flag[i] = Pulse_Flag[i];
			}

			/* Calculate total counts for wheel */
			WheelCount[i] = ((RevCount[i] * ROBOT_ENC_COUNTS_PER_REV) + EncoderCount[i]);
		}

		/* Output distance wheels have travelled to global variable */
		WheelCounts_Left_G = WheelCount[0];
		WheelCounts_Right_G = WheelCount[1];
	}
	else
	{
		WheelCount[0] = 0;
		WheelCount[1] = 0;
	}
}
