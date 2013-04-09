/**
 * CAN_relay.c
 *
 * This task will send the CAN messages to the Motor Controllers and monitor the Proximity sensor output.
 * ---
 * Spec: Send the information from the motor tasks to the Motor Controllers. Monitor the Proximity sensor feedback.
 * ---
 * Period: 2 ticks
 * Offset: 103 ticks
 * ---
 */

#ifndef CAN_RELAY_H_INCLUDED
#define CAN_RELAY_H_INCLUDED

/**
 * Initialisation for the CAN_relay package.
 * This will be called from Tasks_Init by default.
 */
void CAN_relay_Init(void);

/**
 * Currently empty task, to be filled in.
 */
void CAN_relay_Update(void);

void readCAN(void);
void constructCANLf(void);
void constructCANRt(void);
void constructStopCAN(void);
void constructCANEnc(void);

#endif // !CAN_RELAY_H_INCLUDED
