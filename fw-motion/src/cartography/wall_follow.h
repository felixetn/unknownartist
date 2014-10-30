/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file wall_follow.h
 * @author pnotz
 * @brief WallFollow allows to drive with a specific distance to the left or the right wall
 *        or to the center. Therefore the side us sensors are used and the new steering angle is
 *        calculated by PID controller every 100ms.
 **/

#ifndef WALL_FOLLOW_H_
#define WALL_FOLLOW_H_


#include "FreeRTOS.h"
#include "task.h"
#include "../api/api.h"
#include "../api/defines.h"

extern int8_t wall_align;
extern uint16_t wall_distance;
extern boolean_t newturn;
#define NUM_VALUES_WALL_FOLLOW 6

extern xTaskHandle xHandleWallFollow;

/**
 * Initializes the wall follow module
 */
void initWallFollow(void);

boolean_t wall_Follow_isActivated(void);

/**
 * Start following a wall by resuming the corresponding thread and setting the speed to 2.
 * @param align: RIGHT (1) or LEFT (0) or NotRunning (-1) or CENTER (else)
 * @param distance: distance in cm to the wall or the center (positive values for the right hand side)
 */
void startWallFollow(int8_t align, uint16_t distance);

/**
 * Stops following the wall by suspending the thread and stopping the motor.
 */
void stopWallFollow(void);

/**
 * Internal function that is called periodically by the OS.
 */
void SteerToWallThread(void);

/**
 * Getter for the variable wall_align
 * @return variable wall_align
 */
int8_t getWallAlign(void);


#endif /* WALL_FOLLOW_H_ */
