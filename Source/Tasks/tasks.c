/*
 * Parent package that provides initialisation for all task packages.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "tasks.h"

#include "Flashing_LED/flashing_led.h"
#include "System_Control/system_control.h"
#include "Read_Enc/read_enc.h"
#include "Motor_Right/motor_right.h"
#include "Motor_Left/motor_left.h"
#include "Course_Corrector/course_corrector.h"
#include "CAN_relay/can_relay.h"
#include "Message_Sender/message_sender.h"
#include "object_detection/object_detection.h"

/**
 * Initialise all hardware for all tasks.
 */
void Tasks_Init(void)
{
	Flashing_LED_Init();
	System_Control_Init();
	Read_Enc_Init();
	Motor_Right_Init();
	Motor_Left_Init();
	Course_Corrector_Init();
	CAN_relay_Init();
	Message_Sender_Init();
	object_detection_Init();
}
