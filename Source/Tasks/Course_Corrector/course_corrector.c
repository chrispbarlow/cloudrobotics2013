/**
 * Course_Corrector.c
 *
 * Course Correction Task
 * ---
 * Spec: Monitor the Encoder counts for both Left and Right Encoders and determine if a speed modification is required.
 * ---
 * Period: 2 ticks
 * Offset: 103 ticks
 * Modes: Go
 * ---
 */

#include <tte.h>
#include <stdlib.h>
#include "course_corrector.h"
#include "../../global.h"
#include "../../ports.h"

/*= Task Parameters =*/
extern Mode System_Mode_G;
extern Bool Course_correction_Lf;
extern Bool Course_correction_Rt;

extern int32_t Motor_Lf_Enc_Track;
extern int32_t Motor_Rt_Enc_Track;

/**
 * Initialisation for the Course_Corrector package.
 * This will be called from Tasks_Init by default.
 */
void Course_Corrector_Init(void)
{
	/* Initialisation of course correction values */
	Course_correction_Lf = False;
	Course_correction_Rt = False;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Activates the course correction flag depending on the difference in encoder counts
 * Basic implementation: To reduce the speed of the faster side.
 * Advanced requirement: To recalculate the Script to compensate for deviation.
 */
void Course_Corrector_Update(void)
{
	if(System_Mode_G == Go)
	{
		/* Encoder monitor task */
		/* If the Left side is faster than the Right, activate the Left CC flag */
//		if(Motor_Lf_Enc_Track > Motor_Rt_Enc_Track)
//		{
//			Course_correction_Lf = False;
//			Course_correction_Rt = True;
//		}
//		/* If the Right side is faster than the Left, activate the Right CC flag */
//		else if(Motor_Lf_Enc_Track < Motor_Rt_Enc_Track)
//		{
//			Course_correction_Lf = True;
//			Course_correction_Rt = False;
//		}
//		/* If the difference is tolerable, deactivate the flags */
//		else
//		{
//			Course_correction_Lf = False;
//			Course_correction_Rt = False;
//		}

		if((Motor_Lf_Enc_Track - Motor_Rt_Enc_Track) > COURSE_CORRECTION_BUFFER)
		{
			Course_correction_Lf = False;
			Course_correction_Rt = True;
		}
		/* If the Right side is faster than the Left, activate the Right CC flag */
		else if((Motor_Rt_Enc_Track - Motor_Lf_Enc_Track) > COURSE_CORRECTION_BUFFER)
		{
			Course_correction_Lf = True;
			Course_correction_Rt = False;
		}
		/* If the difference is tolerable, deactivate the flags */
		else
		{
			Course_correction_Lf = False;
			Course_correction_Rt = False;
		}

	}
	/* Ensure default is False */
	else
	{
		Course_correction_Lf = False;
		Course_correction_Rt = False;
	}
}
