/*
 * Basic scheduler functionality for Time-Triggered Co-operative (TTC)
 * and Time-Triggered Hybrid (TTH) scheduling of task functions.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "../scheduler.h"
#include "../interrupt.h"
#include "../log.h"

#include "../../Drivers/timer.h"

#include "../../config.h"

static volatile uint32_t system_time  = 0;
static volatile uint32_t current_task = 0;

///////////////////////////////////////////////////////////////////////////////

static uint32_t single_tick_time = 0;
#if (Config_Hardware_Scheduler != TRUE)
static uint32_t tick_skip_time   = 1;
#endif

///////////////////////////////////////////////////////////////////////////////

#if (Config_Hardware_Scheduler == TRUE)
typedef struct
{
	uint32_t delay;
	uint32_t period;
	uint32_t vector;
	uint32_t enable;
	uint32_t gpt;
	uint32_t aot;
	uint32_t bvector;
	uint32_t tcount;
} raw_hwsch_register_t;

typedef volatile raw_hwsch_register_t * const hwsch_register_t;
static hwsch_register_t hwsch_register = (hwsch_register_t) 0x80010000;

typedef volatile uint32_t * const tmrbuf_register_t;
static tmrbuf_register_t tmrbuf_register = (tmrbuf_register_t) 0x80024000;
#endif

///////////////////////////////////////////////////////////////////////////////

void Scheduler_Init(const uint32_t tick_us)
{
	single_tick_time = Timer_Init(Config_Scheduler_Timer_Number, tick_us,
			Config_Oscillator_Frequency, Config_Clock_Divider);
#if (Config_Hardware_Scheduler == TRUE)
	const uint32_t cycles_per_us = Config_Oscillator_Frequency / 1000000;
	const uint32_t gpt = Config_Tick_Period * cycles_per_us;
	for (uint32_t i = 0; i < Config_Num_Tasks; i++)
	{
		hwsch_register[i].vector = (uint32_t) Tasks[i].task_function;
		hwsch_register[i].period = Tasks[i].period_reload - 1;
		hwsch_register[i].delay = Tasks[i].delay_counter;
		hwsch_register[i].enable = 0x01;
		if (Tasks[i].wcet_estimate == 0)
		{
			hwsch_register[i].gpt = gpt - 1;
		}
		else
		{
			hwsch_register[i].gpt = Tasks[i].wcet_estimate - 1;
		}
		hwsch_register[i].bvector = (uint32_t) Tasks[i].recovery_task;
		hwsch_register[i].aot = gpt - 1;
	}
#else
	Init_Interrupt();
	current_task = Config_Num_Tasks;
	for (uint32_t i = 0; i < Config_Num_Tasks; i++)
	{
		Tasks[i].delay_counter++;
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////

void Scheduler_Start(void)
{
#if (Config_Logging == TRUE && Config_Hardware_Scheduler != TRUE)
	Log_Init();
#endif

#if (Config_Hardware_Sandwich_Delays == TRUE)
	TTE_UNUSED const uint32_t enable_flag = 0x30000;
#else
	TTE_UNUSED const uint32_t enable_flag = 0x10000;
#endif

#if (Config_Hardware_Scheduler == TRUE)
	hwsch_register[0].enable |= enable_flag;
#if (Config_Logging == TRUE)
	if (Config_Log_Buffer_Size < 16384) // 2048 entries max.
	{
		tmrbuf_register[1] = (Config_Log_Buffer_Size / 8) - 1;
	}
	tmrbuf_register[2] = Config_Log_Num_Ticks;
#endif
#else
	Enable_Interrupt();
#endif

	Timer_Start(Config_Scheduler_Timer_Number);
}

///////////////////////////////////////////////////////////////////////////////

// Found in the startup.strt assembly file.
extern void Enter_Sleep_Mode(void);

void Shutdown_System(void)
{
#if (Config_Hardware_Scheduler == TRUE)
	hwsch_register[0].enable = 0;
#endif

	Disable_Interrupt();
	Enter_Sleep_Mode();
}

///////////////////////////////////////////////////////////////////////////////

static inline boolean_t Update_Required(void)
{
	boolean_t update = FALSE;

	Disable_Interrupt();
	if (system_time > 0)
	{
		update = TRUE;
		system_time--;
	}
	Enable_Interrupt();

	return update;
}

///////////////////////////////////////////////////////////////////////////////

static inline boolean_t Task_Ready(const uint32_t task_index,
								   TTE_UNUSED const boolean_t in_isr)
{
	// Avoid short-circuiting, to eliminate jitter.
	boolean_t ready = Tasks[task_index].task_function != NULL;
	ready &= Tasks[task_index].delay_counter <= 0;
#if (Config_Hybrid_Tasks == TRUE)
	ready &= Tasks[task_index].exec_from_isr == in_isr;
#endif

	return ready;
}

///////////////////////////////////////////////////////////////////////////////

static inline uint32_t Next_Tick(void)
{
	uint32_t next_counter = UINT32_MAX;

	for (uint32_t i = 0; i < Config_Num_Tasks; i++)
	{
		const int32_t task_delay = Tasks[i].delay_counter;
		const uint32_t task_reload = Tasks[i].period_reload;

		if (task_delay > 0 && (uint32_t) task_delay < next_counter)
			next_counter = task_delay;
		else if (task_delay <= 0 && task_reload < next_counter)
			next_counter = task_reload;
	}

	if (next_counter == UINT32_MAX)
		next_counter = 1;

	return next_counter;
}

///////////////////////////////////////////////////////////////////////////////

// Found in Scheduler_Dispatch, below.
extern void Task_End_Label(void);

void Scheduler_Dispatch(void)
{
#if (Config_Hardware_Scheduler != TRUE)
	Disable_Interrupt();
	while (Update_Required())
	{
		for (uint32_t i = 0; i < Config_Num_Tasks; i++)
		{
			if (Task_Ready(i, FALSE))
			{
				Tasks[i].delay_counter = Tasks[i].period_reload;
#if (Config_Logging == TRUE)
				Log_Set_Address(FALSE, (uint32_t) Tasks[i].task_function,
								(uint32_t) Task_End_Label);
#endif

				current_task = i;
				Enable_Interrupt();

				(*Tasks[i].task_function)();

				__asm__ volatile(".global Task_End_Label");
				__asm__ volatile("Task_End_Label:");

				Disable_Interrupt();
				current_task = Config_Num_Tasks;

#if (Config_Logging == TRUE)
				Log_Task_Start(i);
				Log_Task_End(i);
#endif
			}
		}
	}

#if (Config_Logging == TRUE)
	Log_Idle();
#endif

	Enable_Interrupt();
	Enter_Sleep_Mode();
#endif
}

///////////////////////////////////////////////////////////////////////////////

// Found in Scheduler_Tick, below.
extern void Hybrid_End_Label(void);

void Scheduler_Tick(void)
{
#if (Config_Hardware_Scheduler != TRUE)
#if (Config_Logging == TRUE)
	Log_Tick_Start(current_task);
#endif

	system_time += tick_skip_time;

	for (uint32_t i = 0; i < Config_Num_Tasks; i++)
	{
		Tasks[i].delay_counter -= tick_skip_time;
#if (Config_Hybrid_Tasks == TRUE)
		if (Task_Ready(i, TRUE))
		{
			Tasks[i].delay_counter = Tasks[i].period_reload;
#if (Config_Logging == TRUE)
			Log_Set_Address(TRUE, (uint32_t) Tasks[i].task_function,
							(uint32_t) Hybrid_End_Label);
#endif

			(*Tasks[i].task_function)();

			__asm__ volatile(".global Hybrid_End_Label");
			__asm__ volatile("Hybrid_End_Label:");

#if (Config_Logging == TRUE)
			Log_Hybrid_Start(i);
			Log_Hybrid_End(i);
#endif
		}
#endif
	}

#if (Config_Logging == TRUE)
	Log_Reset_Addresses();
	Log_Tick_End(current_task);
#endif

#if (Config_Timeline_Scheduler == TRUE)
	tick_skip_time = Next_Tick();
	Timer_Set_Match(Config_Scheduler_Timer_Number,
			tick_skip_time * single_tick_time);
#endif
#endif
}
