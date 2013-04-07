/**
 * global.h
 *
 * Definition of most of the shared memory variables for the tasks and constants
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include "tte.h"

/* Robot dimensions */
#define ROBOT_WIDTH (7)
#define ROBOT_ENC_TRACK_WIDTH_mm (170)
#define ROBOT_ENC_WHEEL_DIA_mm (54)
#define ROBOT_ENC_COUNTS_PER_REV (128)

/* Grid properties */
#define GRID_RESOLUTION_mm (50)
#define AVOID_SAFE_DISTANCE (40)
#define ENC_PER_GRID (1)

/* Size of Path and Script Structures */
#define PATH_MAX (16)
#define SCRIPT_MAX (64)

/*
 * Motor limits
 * The RPM_LIMIT requires calibration. This value is an indirect Current load demand rather than an actual RPM setting
 * The CORRECTION_VALUE will depend on the RPM_LIMIT value. Recommend ~20% of the RPM_LIMIT value.
 * CRAWL_SPEED is the lowest motor speed that is able to move the robot
 * RAMP_DOWN_GAIN controls the rate of deceleration when ramping down the motor speeds.
 */
#define RPM_LIMIT (20)
#define CORRECTION_VALUE (4)
#define CORRECTION_THRESHOLD (24)
#define CRAWL_SPEED (12)
#define BOTTLE_SPEED (24)
#define RPM_FACTOR (0.09)
#define RPM_FLOOR (8)

/*
 * Encoder count details
 * TURN_ENC_COUNT represents the number of Encoder counts required for a successful "turn"
 * ACC_COUNT represents the number of Encoder counts required for the Acceleration/Deceleration of the motors before "STOP".
 * BUFFER_COUNT represents the encoder buffer for the end of motor manoeuvres.
 * COURSE_CORRECTION_BUFFER represent the encoder buffer for motor manoeuvres.
 * BOTTLE_TIMEOUT represents the number of ticks before the timeout overflows
 */
#define TURN_ENC_COUNT (211)
#define ACC_COUNT (90)
#define BUFFER_COUNT (2)
#define COURSE_CORRECTION_BUFFER (1)
#define BOTTLE_TIMEOUT (5000)

/*
 * Sensor values
 */
#define US_THRESHOLD (16)
#define US_MIN (0)
#define IR_THRESHOLD (0x0090)

/* Main high-level system mode */
typedef enum {Ready, Set, Go, E_Stop, Game_Over} Mode;

/* Wheel / motor direction */
typedef enum {Off, Forwards, Reverse} Direction;

/* Robot rotation (viewed from above) */
typedef enum {Stop, Clockwise, Anticlockwise} Rotation;

/* Input / output state (may already be in GPIO.h */
typedef enum {Low, High} State;

/* Game timer */
typedef enum {Stopped, Init, Running} Timer;

/* Basic map (currently unused */
typedef enum {No_Obj, Obj} Map;

/* Boolean values */
typedef enum {False, True} Bool;

/* Flags for pathfinder */
typedef enum {Begin, Plotting, Trace, Complete, No_Path, Localisation_Error} Path;

/* Flags for drive control */
typedef enum {Next_Step, Turning, Driving, Halt, End} Drive_Control;

/* Flags for strategy control */
typedef enum {Defensive, Agressive} Strategy;

/* Red or Blue? */
typedef enum {Red, Blue} Team;
Team Team_Colour;

/*=== PathFinder ===*/
/* Strategy */
Strategy Strategy_G;
/* Compass direction for path structure (default X used as an indicator that the path is finished) */
typedef enum {X, North, South, East, West, CollectLf, CollectRt, Bottle} Compass;
/* Main path structure incorporating heading and distance */
typedef struct
	{
		Compass Heading[PATH_MAX];
		int32_t Distance[PATH_MAX];
	}
	main_path;
main_path Path_G;
uint8_t Path_no_G;
Compass Orientation_G;

/*=== Script_Keeper ===*/
/* Bearing for script_path structure. Stp represents the end of the Script */
typedef enum {Stp, Fd, Bd, Lf, Rt, DeployLf, DeployRt, Press} Bearing;
/* Script path structure */
typedef struct
	{
		Bearing Movement[SCRIPT_MAX];
		int32_t Enc_counts[SCRIPT_MAX];
	}
	script_path;
script_path Script_G;
uint8_t Script_no_G;
Bool Script_update;
Bool End_Of_MoveLf;
Bool End_Of_MoveRt;

/* Global movement state */

Bearing movement_G;

/* Motor counter tracker */
int32_t Motor_Lf_Enc_Track;
int32_t Motor_Rt_Enc_Track;

/* Course correction flags */
Bool Course_correction_Lf;
Bool Course_correction_Rt;

/* Opponent Robot detected */
Bool Opponent_detected;

/* global left and right motor speeds */
uint16_t Left_motor_speed_G;
uint16_t Right_motor_speed_G;

/* global left and right motor directions */
Direction Left_motor_direction_G;
Direction Right_motor_direction_G;

/* CAN parameters */
uint32_t canIDLf;
uint32_t canIDRt;
uint8_t candataLf[8];
uint8_t candataRt[8];
uint8_t servo[8];

/* CAN Read Buffer structures */
typedef enum {New, Used} buffer_status;
typedef struct {
		buffer_status status;
		uint32_t ID;
		uint8_t CAN_Data[8];
  } Receive_Buffers;

Receive_Buffers bufferRX0;
Receive_Buffers bufferRX1;

uint32_t canIDSensors;

typedef struct
	{
		uint32_t IRLeft;		/* Left infrared sensor */
		uint32_t IRRight;	/* Right infrared sensor */
		uint32_t USFwd;		/* Forward-facing ultrasonic sensor */
	}
	sensors;

sensors sensorReadings;

/* Robot current coordinates */
int robot_x_coord_G;
int robot_y_coord_G;
/* opponent's current coordinates */
float opp_x_coord_G;
float opp_y_coord_G;

/* goal cooridinates for pathfinder */
int next_x_coord_G;
int next_y_coord_G;

/* Robot position in map array */
int robot_x_map_pos_G;
int robot_y_map_pos_G;

/* Goal position in map array */
int next_x_map_pos_G;
int next_y_map_pos_G;

uint16_t delta_x;
uint16_t delta_y;

/* Robot current heading - used to calculate turn angle */
float heading_radians_G;

/* Straight line distance to travel and target encoder counts */
uint32_t next_distance_G;
uint16_t next_wheel_counts_G;

/* Independent target encoder counts */
int32_t Target_counts_Left_G;
int32_t Target_counts_Right_G;

/* Current encoder counts */
int32_t WheelCounts_Left_G;
int32_t WheelCounts_Right_G;

/* Control flags for pathfinder and next heading tasks */
Path Pathfinder_Control_G;
Drive_Control Pathdriver_Control_G;

#endif /* GLOBAL_H_ */
