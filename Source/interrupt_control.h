/*
 * The interrupt control functionality.
 *
 * Copyright (c) TTE Systems Limited 2010.  All rights reserved.
 */


#include "../tte.h"


/**
 * Disable the main timer interrupt.  This may be called in a nested
 * or recursive fashion, but there should be one call to the Enable
 * function for every call to Disable.
 */
void Disable_Interrupt(void);

/**
 * Enable the main timer interrupt.  This may be called in a nested
 * or recursive fashion, but there should be one call to the Disable
 * function for every call to Enable.
 */
void Enable_Interrupt(void);
