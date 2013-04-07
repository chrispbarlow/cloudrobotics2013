/*
 * The main timer interrupt handler and support functionality.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */


#include "../tte.h"
#include "../interrupt_control.h"


#define TIMER0 7
#define TIMER1 6
#define UART0  5
#define UART1  4
/**
 * Initialises the interrupt vector for the system interrupt service
 * routine.
 */
void Init_Interrupt(void);
