/*
 * An example configuration package for C projects.  This provides
 * constants and configuration for the system and all of its tasks.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "config.h"
#include "Scheduler/scheduler.h"

#include "Tasks/tasks.h"
#include "Tasks/Flashing_LED/flashing_led.h"
#include "Tasks/System_Control/system_control.h"
#include "Tasks/Read_Enc/read_enc.h"
#include "Tasks/Motor_Right/motor_right.h"
#include "Tasks/Motor_Left/motor_left.h"
#include "Tasks/Course_Corrector/course_corrector.h"
#include "Tasks/CAN_relay/can_relay.h"
#include "Tasks/Message_Sender/message_sender.h"
#include "Tasks/object_detection/object_detection.h"

// The actual task array, containing all tasks and their settings:
volatile task_t Tasks[] =
{
		{
				.task_function = Read_Enc_Update,
				.period_reload = 1,
				.delay_counter = 0,
				.wcet_estimate = 400,
				.recovery_task = Shutdown_System,
				.exec_from_isr = TRUE
		},

		{
				.task_function = System_Control_Update,
				.period_reload = 10,
				.delay_counter = 0,
				.wcet_estimate = 500,
				.recovery_task = Shutdown_System,
				.exec_from_isr = TRUE
		},

		{
				.task_function = CAN_relay_Update,
				.period_reload = 3,
				.delay_counter = 103,
				.wcet_estimate = 1000,
				.recovery_task = Shutdown_System,
				.exec_from_isr = FALSE
		},

		{
				.task_function = Message_Sender_Update,
				.period_reload = 3,
				.delay_counter = 104,
				.wcet_estimate = 1000,
				.recovery_task = Shutdown_System,
				.exec_from_isr = FALSE
		},

		{
				.task_function = object_detection_Update,
				.period_reload = 3,
				.delay_counter = 102,
				.wcet_estimate = 1000,
				.recovery_task = Shutdown_System,
				.exec_from_isr = FALSE
		},

		{
				.task_function = Course_Corrector_Update,
				.period_reload = 3,
				.delay_counter = 102,
				.wcet_estimate = 500,
				.recovery_task = Shutdown_System,
				.exec_from_isr = FALSE
		},

		{
				.task_function = Motor_Right_Update,
				.period_reload = 3,
				.delay_counter = 102,
				.wcet_estimate = 1000,
				.recovery_task = Shutdown_System,
				.exec_from_isr = FALSE
		},

		{
				.task_function = Motor_Left_Update,
				.period_reload = 3,
				.delay_counter = 102,
				.wcet_estimate = 1000,
				.recovery_task = Shutdown_System,
				.exec_from_isr = FALSE
		},

		{
				.task_function = Flashing_LED_Toggle,
				.period_reload = 500,
				.delay_counter = 109,
				.wcet_estimate = 500,
				.recovery_task = Shutdown_System,
				.exec_from_isr = FALSE
		}
};

const uint32_t Config_Num_Tasks = sizeof(Tasks) / sizeof(Tasks[0]);
