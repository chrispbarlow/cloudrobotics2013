/*
 * Simple dummy task created by the automatic add task functionality.
 */

#ifndef SCRIPT_KEEPER_H_INCLUDED
#define SCRIPT_KEEPER_H_INCLUDED

#include <stdint.h>

/**
 * Initialisation for the Script_Keeper package.
 * This will be called from Tasks_Init by default.
 */
void Script_Keeper_Init(void);

/**
 * Currently empty task, to be filled in.
 */
void Script_Keeper_Update(void);

void ahead(uint32_t);
void backOff(uint32_t);
void aboutTurn(uint32_t);
void leftTurn(uint32_t);
void rightTurn(uint32_t);
void stopScript(void);
void bottleBash(uint32_t);
void deployRight(uint32_t);
void deployLeft(uint32_t);

#endif // !SCRIPT_KEEPER_H_INCLUDED
