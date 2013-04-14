cloudrobotics2013
=================

**About this Repo**

This repo tracks the changes made to the main control code for the robot used for Cloud Robotics Hackathon 2013 team Telemetrons.

The robot started out life being used for the Eurobot UK 2012 competition, details here: 
http://chrisbarlow.wordpress.com/category/its-all-here/competition-entries/eurobot/eurobot-2012-treasure-island-team-r-me-r-t/

Progress throughout the week can be tracked using the tags, Day1, Day2, etc.

The Day1 tag shows the state of the original Eurobot codebase, as no coding work was done on the first daty.

**About the CRH 2013 Project**

The project we set ourselves for CRH was to use the existing hardware, encoders, range sensors, etc, along with an automotive telemetry device used by smith electric vehicles (http://blog.sevtelem.com) to store data about distance travelled and measured distances from objects in a database. A web site then polls the database for this data and constructs a map of the robot's surroundings.

**Original codebase**

In its Eurobot incarnation, the robot was programmed to follow a pre-defined path, in a known environment. It used wheel encoders to measure distance travelled and used ultrasonic and infra-red sensors to stop if the opponent robot crossed its path. As the competition table was always the same, we didn't need to 'discover and avoid' the environment boundaries.

The code is designed to run with a time-triggered scheduler. 
Tasks are saved in their respective folders in Source/Tasks/
Each task has an _init() function which is run once, and an _update() function which is run periodically according to the timing specified in Source/config.c

For more details on time-triggered scheduling see http://www.tte-systems.com/books/pttes

**Changes for CRH 2013**

For this competition, the functionality of the robot needed to be changed from a blind path-follower, to an autonomous rover. The robot was required to not only detect when it was approaching an object, but also to react to the objects. The majority of the changes were made by replacing tasks, *Opponent_Tracker/*, *PathFinder/* and *Script_Keeper/* with a single task, *object_detection/*.

This new task, written solely for the hackathon, takes the sensor readings (which are transmitted via CAN bus from a separate Arduino-based sensor node) and alters a global direction variable, **movement_G** in order to control the movement of the robot.

The two motor control tasks, **Motor_Left/** and **Motor_Right/** use this global direction to decide on the speed and direction for their respective motors.

The robot now behaves in the following way (highly simplified):

Move forward indefinitely
If one of the distance sensor readings is too low, turn away from that sensor until the reading is OK again.
If the sensor on the side that was last avoided increases suddenly, turn towards that sensor (external corner)
Continue moving forward and repeat.
