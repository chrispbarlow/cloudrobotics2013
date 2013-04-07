/*
 * The main timer interrupt handler and support functionality.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */

#include "../interrupt.h"
#include "../scheduler.h"
#include "../../config.h"

#include <tte.h>

// From the startup.strt assembly file:
extern void Disable_Interrupts(void);
extern void Enable_Interrupts(void);

///////////////////////////////////////////////////////////////////////////////

static uint32_t nesting_level = 0;

///////////////////////////////////////////////////////////////////////////////

extern uint32_t TableBase;
extern void InterruptHandler(void);

///////////////////////////////////////////////////////////////////////////////
/**
 * Status_Reg_Get()
 *
 * Returns the current value of the status register.
 *
 */
uint32_t Status_Reg_Get()
{
	uint32_t regValue;
	__asm volatile("mfc0 %0, $12" : "=d" (regValue) :);
	return  regValue;

}
///////////////////////////////////////////////////////////////////////////////
/**
 * Cause_Reg_Get()
 *
 * Returns the current value of the cause register.
 *
 */
uint32_t Cause_Reg_Get()
{
	uint32_t regValue;
	__asm volatile("mfc0 %0, $13" : "=d" (regValue):);
	return  regValue;

}
///////////////////////////////////////////////////////////////////////////////
/**
 * Cause_Reg_Get()
 *
 * Sets the value of the cause register.
 *
 * \param[in] The value to write.
 */
void Cause_Reg_Set(uint32_t value)
{
	__asm volatile("mtc0 %0, $13" :: "d" ((unsigned long)(value)));

}

///////////////////////////////////////////////////////////////////////////////
/**
 * Status_Reg_Set()
 *
 * Sets the value of the cause register.
 *
 * \param[in] The value to write.
 */
void Status_Reg_Set(uint32_t value)
{
	__asm volatile("mtc0 %0, $12" :: "d" ((unsigned long)(value)));

}
///////////////////////////////////////////////////////////////////////////////
//void Disable_Interrupt_IN_ISR(void)
//{
//	Cause_Reg_Set(Cause_Reg_Get() & ~0x3F);
//}
//////////////////////////////////////////////////////////////////////////////
void Clear_Pending_Interrupt(const uint32_t int_source)
{
	Cause_Reg_Set(Cause_Reg_Get()| (1 << (int_source+ 8)));
}

//////////////////////////////////////////////////////////////////////////////

/**
 * Enable_System_Interrupt()
 *
 * Enables a specific interrupt for the scheduler.
 *
 * @param int_source
 */
void Enable_System_Interrupt(const uint32_t int_source)
{
    Status_Reg_Set((Status_Reg_Get()& 0xFFFFF8FF) | (int_source << 8));
}
///////////////////////////////////////////////////////////////////////////////
/**
 * Check_System_Interrupt()
 *
 * Checks to see if the interrupt described by int_source has been triggered.
 *
 * @param int_source
 */
uint32_t Check_System_Interrupt(const uint32_t int_source)
{
    uint32_t value =  (1 << (int_source + 8));
	return (Cause_Reg_Get() & 0x0000FF00) == value;
}


///////////////////////////////////////////////////////////////////////////////


void Init_Interrupt(void)
{
	TableBase = (uint32_t)InterruptHandler;
	Enable_System_Interrupt(Config_Scheduler_Interrupt_Number);
}

///////////////////////////////////////////////////////////////////////////////

void Disable_Interrupt(void)
{
	Disable_Interrupts();
	nesting_level++;
}

///////////////////////////////////////////////////////////////////////////////

void Enable_Interrupt(void)
{
	if (nesting_level > 0)
	{
		nesting_level--;
	}

	if (nesting_level == 0)
	{
		Enable_Interrupts();
	}
}

///////////////////////////////////////////////////////////////////////////////

void Handle_Interrupts(void)
{
	Scheduler_Tick();
}
