/*
 * Provides logging functionality for RapidiTTy's timing analysis.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "../log.h"
#include "../scheduler.h"
#include "../interrupt.h"

#include "../../config.h"

typedef enum
{
	LOG_TASK_START,
	LOG_TASK_END,
	LOG_INTERRUPT_START,
	LOG_INTERRUPT_END,
	LOG_HYBRID_START,
	LOG_HYBRID_END
} log_type_t;

static const char_t log_char[] = { 'S', 'F', 'I', 'J', 'C', 'D' };

///////////////////////////////////////////////////////////////////////////////

volatile uint8_t Log_Buffer[Config_Log_Buffer_Size];

///////////////////////////////////////////////////////////////////////////////

static volatile uint32_t buffer_index = 0;
static volatile uint32_t idle_index   = UINT8_MAX;

///////////////////////////////////////////////////////////////////////////////

static uint32_t last_index = 0;
static uint32_t current_tick = 0;
static uint32_t previous_time = 0;
static uint32_t previous_isr_time = 0;

///////////////////////////////////////////////////////////////////////////////

uint32_t Get_Relative_Time(const uint32_t first, const uint32_t second)
{
	if (second >= first)
	{
		return second - first;
	}

	return second + (UINT32_MAX - first);
}

///////////////////////////////////////////////////////////////////////////////

void Write_8(const uint8_t value)
{
	Log_Buffer[buffer_index++] = value;
}

///////////////////////////////////////////////////////////////////////////////

void Write_32(const uint32_t value)
{
	Log_Buffer[buffer_index++] = (uint8_t) (value >> 24);
	Log_Buffer[buffer_index++] = (uint8_t) (value >> 16);
	Log_Buffer[buffer_index++] = (uint8_t) (value >> 8);
	Log_Buffer[buffer_index++] = (uint8_t) value;
}

///////////////////////////////////////////////////////////////////////////////

void Set_Watch(const uint8_t number, const uint32_t value)
{
	switch (number)
	{
	case 0:
		__asm__ volatile ("mtc0 %0, $18"::"r"(value));
		break;
	case 1:
		__asm__ volatile ("mtc0 %0, $19"::"r"(value));
		break;
	case 2:
		__asm__ volatile ("mtc0 %0, $20"::"r"(value));
		break;
	case 3:
		__asm__ volatile ("mtc0 %0, $21"::"r"(value));
		break;
	case 4:
		__asm__ volatile ("mtc0 %0, $22"::"r"(value));
		break;
	case 5:
		__asm__ volatile ("mtc0 %0, $23"::"r"(value));
		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////

uint32_t Get_Watch(const uint8_t number)
{
	uint32_t value = 0;

	switch (number)
	{
	case 0:
		__asm__ volatile ("mfc0 %0, $18":"=r"(value));
		break;
	case 1:
		__asm__ volatile ("mfc0 %0, $19":"=r"(value));
		break;
	case 2:
		__asm__ volatile ("mfc0 %0, $20":"=r"(value));
		break;
	case 3:
		__asm__ volatile ("mfc0 %0, $21":"=r"(value));
		break;
	case 4:
		__asm__ volatile ("mfc0 %0, $22":"=r"(value));
		break;
	case 5:
		__asm__ volatile ("mfc0 %0, $23":"=r"(value));
		break;
	default:
		value = 0;
	}

	return value;
}

///////////////////////////////////////////////////////////////////////////////

// Found in the startup.strt assembly file.
extern void LOG_Trigger_Breakpoint(void);

void Check_Buffer(const uint32_t size)
{
	if (Config_Log_Buffer_Size - buffer_index < size + 1)
	{
		Log_Buffer[buffer_index] = 'z';
		LOG_Trigger_Breakpoint();
		buffer_index = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////

void Log_Time(const log_type_t log_type, const uint32_t task_index,
			  const uint32_t cycle_count)
{
	const uint32_t time = Get_Relative_Time(previous_time, cycle_count);
	Check_Buffer(7);
	Write_8('y');
	Write_8(task_index);
	Write_8(log_char[log_type]);
	Write_32(time);
	previous_time = cycle_count;
}

///////////////////////////////////////////////////////////////////////////////

void Log_ISR_Time(const log_type_t log_type, const uint32_t task_index,
				  const uint32_t cycle_count)
{
	const uint32_t time = Get_Relative_Time(previous_isr_time, cycle_count);
	Check_Buffer(7);
	Write_8('y');
	Write_8(task_index);
	Write_8(log_char[log_type]);
	Write_32(time);

	// Update if task_index is not 255; nasty bit-fiddling to avoid branching.
	const int32_t update = task_index != 255;
	const int32_t no_update = !update;
	previous_isr_time = (-update & cycle_count) |
						(-no_update & previous_isr_time);
}

///////////////////////////////////////////////////////////////////////////////

void Log_Task_Address(const task_function_t task, const uint8_t priority)
{
	Check_Buffer(7);
	Write_8('X');
	Write_32((uint32_t) task);
	Write_8('p');
	Write_8(priority);
}

///////////////////////////////////////////////////////////////////////////////

void Log_Task_Start(const uint32_t task_index)
{
	Log_Time(LOG_TASK_START, task_index, Get_Watch(2));
}

///////////////////////////////////////////////////////////////////////////////

void Log_Task_End(const uint32_t task_index)
{
	Log_Time(LOG_TASK_END, task_index, Get_Watch(3));
}

///////////////////////////////////////////////////////////////////////////////

void Log_Hybrid_Start(const uint32_t task_index)
{
	Log_ISR_Time(LOG_HYBRID_START, task_index, Get_Watch(1));
}

///////////////////////////////////////////////////////////////////////////////

void Log_Hybrid_End(const uint32_t task_index)
{
	Log_ISR_Time(LOG_HYBRID_END, task_index, Get_Watch(0));
}

///////////////////////////////////////////////////////////////////////////////

void Log_Tick_Start(const uint32_t task_index)
{
	Log_ISR_Time(LOG_INTERRUPT_END, last_index, Get_Watch(1));
	Log_ISR_Time(LOG_INTERRUPT_END, idle_index, Get_Watch(4));

	last_index = task_index;

	Log_ISR_Time(LOG_INTERRUPT_START, task_index, Get_Watch(0));
}

///////////////////////////////////////////////////////////////////////////////

void Log_Tick_End(const uint32_t task_index)
{
	current_tick++;
	if (current_tick > Config_Log_Num_Ticks)
	{
		Check_Buffer(1);
		Write_8('g');
		LOG_Trigger_Breakpoint();
	}

	if (task_index >= Config_Num_Tasks)
	{
		idle_index = UINT8_MAX;
	}
}

///////////////////////////////////////////////////////////////////////////////

void Log_Set_Address(const boolean_t in_isr, const uint32_t start,
					 const uint32_t finish)
{
	idle_index = UINT8_MAX;
	if (in_isr)
	{
		Set_Watch(1, start);
		Set_Watch(0, finish);
	}
	else
	{
		Disable_Interrupt();
		Set_Watch(2, start);
		Set_Watch(3, finish);
		Enable_Interrupt();
	}
}

///////////////////////////////////////////////////////////////////////////////

// Found in the startup.strt assembly file.
extern void End_IRQ_Ptr(void);
extern void Sleep_Ptr(void);

void Log_Reset_Addresses(void)
{
	Set_Watch(0, 0x20000008);
	Set_Watch(1, (uint32_t) End_IRQ_Ptr);
	Set_Watch(4, (uint32_t) Sleep_Ptr);
}

///////////////////////////////////////////////////////////////////////////////

void Log_Idle(void)
{
	Disable_Interrupt();
	idle_index = Config_Num_Tasks;
	Enable_Interrupt();
}

///////////////////////////////////////////////////////////////////////////////

void Log_Init(void)
{
	Check_Buffer(5);
	Write_8('W');
	Write_32(Config_Oscillator_Frequency);

	last_index = Config_Num_Tasks;

	uint32_t priority = 0;
	for (uint32_t i = 0; i < Config_Num_Tasks; i++)
	{
		if (Tasks[i].exec_from_isr == TRUE)
		{
			priority = (last_index * 2) - i;
		}
		else
		{
			priority = last_index - i;
		}
		Log_Task_Address(Tasks[i].task_function, priority);
	}

	Log_Reset_Addresses();

	previous_time = Get_Watch(0);
	previous_isr_time = previous_time;
	Log_Time(LOG_TASK_START, last_index, 0);
}
