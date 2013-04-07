/**
 * Pathfinder.c
 *
 * This task sets the Strategy based on the switch inputs.
 * ---
 * Spec: The Update method should set the Strategy based on the switch inputs.
 * ---
 * Period: 10 ticks
 * Offset: 100 ticks
 * Modes: ALL
 * ---
 */

#include "../../tte.h"
#include "pathfinder.h"
#include "../../global.h"
#include "../../ports.h"
#include <stdlib.h>

/*= Task Parameters =*/
extern main_path Path_G;
extern Strategy Strategy_G;
extern uint8_t Script_no_G;
extern Bool Opponent_detected;
extern Bool Script_update;
extern Mode System_Mode_G;
extern Team Team_Colour;


main_path StraightRed = {
		{Bottle, X},
		{128, 0}
};

main_path HomologationBlue = {
		{East, South, West, West, South, North, X},
		{ 390,   475,  350, -350,     1,     1, 0}
};

main_path HomologationRed = {
		{West, South, East, East, South, North, X},
		{ 390,   475,  350, -350,     1,     1, 0}
};

main_path Test = {
		{East, West, West, East, X},
		{512, 512, -256 , -256 , 0}
};

main_path Defense = {
		{South, West, North, East, X},
		{240, 240, 240, 240, 0}
};

main_path Offense = {
		{North, West, South, East, X},
		{128, 128, 128, 128, 0}
};

main_path BlueBottle = {
		{East, South, West, West, South, South, East, North, Bottle, X},
		{ 395,   475,  320, -330,   760,  -256,  950,    -1,   1200, 0}
};

main_path RedBottle = {
		{West, South, East, East, South, South, West, North, Bottle, X},
		{ 395,   475,  320, -330,   760,  -256,  950,    -1,   1200, 0}
};


void PathFinder_Init(void)
{
	/* Default Strategy */
	Strategy_G = Defensive;

	/* Clear Path */
	for(uint8_t i = 0; i < PATH_MAX; i++)
	{
		Path_G.Heading[i] = X;
		Path_G.Distance[i] = 0;
	}

	/* Initialise Switch IOs */
	GPIO_Set_Direction(Strategy0, GPIO_INPUT);
	GPIO_Set_Direction(Strategy1, GPIO_INPUT);
	GPIO_Set_Direction(Strategy2, GPIO_INPUT);

	GPIO_Set_Direction(LED_Pin_SC, GPIO_OUTPUT);
	GPIO_Write(LED_Pin_SC, GPIO_LOW);
	GPIO_Set_Direction(LED_Pin_Opponent, GPIO_OUTPUT);
	GPIO_Write(LED_Pin_SC, GPIO_LOW);
}

void PathFinder_Update(void)
{
	if(System_Mode_G == Ready)
	{
		if(GPIO_Get(Strategy0) == GPIO_HIGH)
		{
			Strategy_G = Defensive;
		}
		else if(GPIO_Get(Strategy1) == GPIO_HIGH)
		{
			Strategy_G = Agressive;
		}
		else if(GPIO_Get(Strategy2) == GPIO_HIGH)
		{
			Strategy_G = Defensive;
		}
	}

	if(Opponent_detected == True)
	{
		GPIO_Write(LED_Pin_Opponent, GPIO_HIGH);
	}
	else
	{
		GPIO_Write(LED_Pin_Opponent, GPIO_LOW);
	}

	switch(Strategy_G)
	{
	case Agressive:
		if((Opponent_detected == True && Script_no_G == 0))
		{
			Strategy_G = Defensive;
			Path_G = BlueBottle;
			Script_update = True;
			GPIO_Write(LED_Pin_SC, GPIO_HIGH);
		}
		else
		{
			Path_G = BlueBottle;
		}
		break;
	case Defensive:
	default:
		if(Team_Colour == Blue)
		{
			Path_G = BlueBottle;
		}
		if(Team_Colour == Red)
		{
			Path_G = RedBottle;
		}
		break;
	}
}
