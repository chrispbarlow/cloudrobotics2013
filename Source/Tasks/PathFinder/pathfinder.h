/*
 * Simple dummy task created by the automatic add task functionality.
 */

#ifndef PATHFINDER_H_INCLUDED
#define PATHFINDER_H_INCLUDED

#define ROBOT 255
#define OPPONENT 256
#define OBJECT 1
#define SPACE 0
#define GOAL 2
#define X_MAP_EXTENTS 51
#define Y_MAP_EXTENTS 31


extern int Internal_map[32][52];

/**
 * Initialisation for the PathFinder package.
 * This will be called from Tasks_Init by default.
 */
void PathFinder_Init(void);

/**
 * Currently empty task, to be filled in.
 */
void PathFinder_Update(void);

void plot_opponent(void);

void paths(void);

void progressive(void);

void go_north(void);

void go_east(void);

void go_south(void);

void go_west(void);


#endif // !PATHFINDER_H_INCLUDED
