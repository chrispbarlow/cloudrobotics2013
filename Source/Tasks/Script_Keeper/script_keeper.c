/**
 * Script_Keeper.c
 *
 * This task translates the Path into a Script.
 * ---
 * Spec: The compass-based, high-level Path needs to be translated into a direction-based, low-level Script.
 * ---
 * Period: 2 ticks
 * Offset: 101 ticks
 * Modes: ALL
 * ---
 */

#include "../../tte.h"
#include <stdlib.h>
#include "../../global.h"
#include "../../ports.h"
#include "script_keeper.h"

/* The Pathfinder generated path */
extern main_path Path_G;

/* The Script Keeper script */
extern script_path Script_G;
extern uint8_t Script_no_G;
extern Bool Script_update;
uint8_t Script_count;

/* Orientation tracking */
extern Compass Orientation_G;

/* Motor task information */
extern Direction Left_motor_direction_G;
extern uint16_t Left_motor_speed_G;
extern Bool End_Of_MoveLf;
extern Direction Right_motor_direction_G;
extern uint16_t Right_motor_speed_G;
extern Bool End_Of_MoveRt;

/**
 * Initialisation for the Script_Keeper package.
 * This will be called from Tasks_Init by default.
 */
void Script_Keeper_Init(void)
{
	/* Initialise the blank structure and counters */
	Script_count = 0;
	Script_no_G = 0;
	for(uint8_t i = 0; i < SCRIPT_MAX; i++)
	{
		Script_G.Movement[i] = Stp;
		Script_G.Enc_counts[i] = 0;
	}

	Script_update = False;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Converts the Path into a Script and monitors the Script state.
 */
void Script_Keeper_Update(void)
{
	/* Check an update is required*/
	if(Script_update == True)
	{
		/* Analyse the Path_G to generate the Script */
		for(uint8_t i = 0; i < PATH_MAX; i++)
		{
			/* Track the previous direction */
			Compass Previous_Direction;
			Previous_Direction = Orientation_G;

			/* Generate the Script based on the previous and "this" heading */
			switch(Path_G.Heading[i])
			{
			case North:
				/* TODO: Reverse manoeuvre based on NEGATIVE Path distances. Recommend using if(Path.distance < 0) */
				switch(Previous_Direction)
				{
				case North:
					if(Path_G.Distance[i] < 0)
					{
						backOff(i);
					}
					else
					{
						ahead(i);
					}
					break;
				case South:
					aboutTurn(i);
					break;
				case West:
					rightTurn(i);
					break;
				case East:
					leftTurn(i);
					break;
				case X:
				default:
					stopScript();
					break;
				}
				Orientation_G = North;
				break;
			case South:
				switch(Previous_Direction)
				{
				case South:
					if(Path_G.Distance[i] < 0)
					{
						backOff(i);
					}
					else
					{
						ahead(i);
					}
					break;
				case North:
					aboutTurn(i);
					break;
				case East:
					rightTurn(i);
					break;
				case West:
					leftTurn(i);
					break;
				case X:
				default:
					stopScript();
					break;
				}
				Orientation_G = South;
				break;
			case East:
				switch(Previous_Direction)
				{
				case East:
					if(Path_G.Distance[i] < 0)
					{
						backOff(i);
					}
					else
					{
						ahead(i);
					}
					break;
				case West:
					aboutTurn(i);
					break;
				case North:
					rightTurn(i);
					break;
				case South:
					leftTurn(i);
					break;
				case X:
				default:
					stopScript();
					break;
				}
				Orientation_G = East;
				break;
			case West:
				switch(Previous_Direction)
				{
				case West:
					if(Path_G.Distance[i] < 0)
					{
						backOff(i);
					}
					else
					{
						ahead(i);
					}
					break;
				case East:
					aboutTurn(i);
					break;
				case South:
					rightTurn(i);
					break;
				case North:
					leftTurn(i);
					break;
				case X:
				default:
					stopScript();
					break;
				}
				Orientation_G = West;
				break;
			case Bottle:
				bottleBash(i);
				break;
			case CollectRt:
				deployRight(i);
				break;
			case CollectLf:
				deployLeft(i);
				break;
			case X:
			default:
				stopScript();
				break;
			}
		}

		/* Update is not needed */
		Script_update = False;
	}
	/* Monitor the script stage */
	else
	{
		/* Check that both motor tasks have stopped their motors */
		if(
				End_Of_MoveLf == True
				&& End_Of_MoveRt == True
			)
		{
			Script_no_G++;
		}
		/* Check that both motor tasks have stopped their motors and the Script command is to Stop */
		else if(
				End_Of_MoveLf == False
				&& Left_motor_speed_G == 0
				&& End_Of_MoveRt == False
				&& Right_motor_speed_G == 0
				&& Script_G.Movement[Script_no_G] == Stp
				)
		{
			/* Request a Script update */
			//Script_update = True;
			//Script_no_G = 0;
			/* Reset the Script structure
			for(uint8_t i = 0; i < SCRIPT_MAX; i++)
			{
				Script_G.Movement[i] = Stp;
				Script_G.Enc_counts[i] = 0;
			}*/
		}
		else
		{
			Script_update = False;
		}
	}
}

/**
 * Insert the Forward command for a set distance.
 * @param path_pointer The Path pointer
 */
void ahead(uint32_t path_pointer)
{
	Script_G.Movement[Script_count] = Fd;
	Script_G.Enc_counts[Script_count] = (uint32_t)((Path_G.Distance[path_pointer] * ENC_PER_GRID) + 0.5);
	Script_count++;
}

/**
 * Reverse a set distance. The value in the Path is a negative number. Minimum Script.Enc_Count == 0.
 * @param path_pointer The Path pointer
 */
void backOff(uint32_t path_pointer)
{
	int32_t pathDistance;
	Script_G.Movement[Script_count] = Bd;
	pathDistance = -Path_G.Distance[path_pointer];
	if(pathDistance >= 0)
	{
		Script_G.Enc_counts[Script_count] = (uint32_t)(((pathDistance) * ENC_PER_GRID) + 0.5);
	}
	else
	{
		Script_G.Enc_counts[Script_count] = 0;
	}
	Script_count++;
}

/**
 * Insert a 180 deg rotation and forward movement.
 * @param path_pointer The Path pointer
 */
void aboutTurn(uint32_t path_pointer)
{
	Script_G.Movement[Script_count] = Lf;
	Script_G.Enc_counts[Script_count] = TURN_ENC_COUNT;
	Script_count++;
	Script_G.Movement[Script_count] = Lf;
	Script_G.Enc_counts[Script_count] = TURN_ENC_COUNT;
	Script_count++;
	Script_G.Movement[Script_count] = Fd;
	Script_G.Enc_counts[Script_count] = (uint32_t)((Path_G.Distance[path_pointer] * ENC_PER_GRID) + 0.5);
	Script_count++;
}

/**
 * Insert a left turn into the script with forward movement.
 * @param path_pointer The Path pointer
 */
void leftTurn(uint32_t path_pointer)
{
	int32_t pathDistance;

	Script_G.Movement[Script_count] = Lf;
	Script_G.Enc_counts[Script_count] = TURN_ENC_COUNT;
	Script_count++;

	if(Path_G.Distance[path_pointer] < 0)
	{
		Script_G.Movement[Script_count] = Bd;
		pathDistance = -Path_G.Distance[path_pointer];
		Script_G.Enc_counts[Script_count] = (uint32_t)(((pathDistance) * ENC_PER_GRID) + 0.5);
	}
	else if(Path_G.Distance[path_pointer] >= 0)
	{
		Script_G.Movement[Script_count] = Fd;
		Script_G.Enc_counts[Script_count] = (uint32_t)((Path_G.Distance[path_pointer] * ENC_PER_GRID) + 0.5);
	}
	Script_count++;
}

/**
 * Insert a right turn into the script with forward movement.
 * @param path_pointer The path pointer
 */
void rightTurn(uint32_t path_pointer)
{
	int32_t pathDistance;

	Script_G.Movement[Script_count] = Rt;
	Script_G.Enc_counts[Script_count] = TURN_ENC_COUNT;
	Script_count++;

	if(Path_G.Distance[path_pointer] < 0)
	{
		Script_G.Movement[Script_count] = Bd;
		pathDistance = -Path_G.Distance[path_pointer];
		Script_G.Enc_counts[Script_count] = (uint32_t)(((pathDistance) * ENC_PER_GRID) + 0.5);
	}
	else if(Path_G.Distance[path_pointer] >= 0)
	{
		Script_G.Movement[Script_count] = Fd;
		Script_G.Enc_counts[Script_count] = (uint32_t)((Path_G.Distance[path_pointer] * ENC_PER_GRID) + 0.5);
	}
	Script_count++;
}

void bottleBash(uint32_t path_pointer)
{
	Script_G.Movement[Script_count] = Press;
	Script_G.Enc_counts[Script_count] = Path_G.Distance[path_pointer];//(uint32_t)((Path_G.Distance[path_pointer] * ENC_PER_GRID) + 0.5);
	Script_count++;
}

void deployRight(uint32_t path_pointer)
{
	Script_G.Movement[Script_count] = DeployRt;
	Script_G.Enc_counts[Script_count] = Path_G.Distance[path_pointer];
	Script_count++;
}

void deployLeft(uint32_t path_pointer)
{
	Script_G.Movement[Script_count] = DeployLf;
	Script_G.Enc_counts[Script_count] = Path_G.Distance[path_pointer];
	Script_count++;
}

/**
 * Insert the Stop command into the Script.
 */
void stopScript(void)
{
	Script_G.Movement[Script_count] = Stp;
	Script_G.Enc_counts[Script_count] = 0;
}
