/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file calibDrive.h
 * @author pnotz
 * @brief This file provides methods to move the car defined lengths or angles. This is possible
 * 		  by using measured (init) values and the battery level (move front, backward, turn)
 **/

#ifndef CALIBDRIVE_H_
#define CALIBDRIVE_H_

#include "../api/api.h"
#include "../api/util.h"
#include "initValues.h"

#define RIGHT TRUE
#define LEFT FALSE
#define FORWARDS TRUE
#define BACKWARDS FALSE

/*
 * driving a defined length forwards (len>0) or backwards (len<0) - len in cm
 * (should only be called within a thread, because of os_wait usage)
 * @param len_in_cm the desired length to drive
 */
void driveLen(int16_t len_in_cm);

portTickType getTimeforLength(int16_t len_in_cm);

/*
 * calculates the length the car drove within the time and with MotorSpeed = 1
 * @param time_in_ms the driven time in ms
 * @return the driven length in cm
 */
int calculateDrivenLen(portTickType time_in_ms);

/*
 * wrapper method for drive_setSteeringAngle that takes the balanced steering angle into account
 * @param percent steeringAngle (>0 turn right, <0 turn left)
 */
void drive_setSteeringAngleBalanced(int8_t percent);

/*
 * measures the steering angle for driving straight
 * will drive on a straight line using line follow
 * the average steering angle will be used as neutral position
 */
void measure_SteeringAngle(void);

/**
 * makes the car turn by a given angle (>0 to the right; <0 to the left)
 * the angle must be a multiple of five (rounded otherwise)
 * @param angle angle to be turned by
 */
void turn(int32_t angle);

/**
 * makes the car turn 45 degrees
 * @param align whether it shall turn 45 degrees to the right (1) or to the left (0)
 * @param direction whether it shall drive forward (1) or backward (0) first
 */
void turn45(boolean_t align, boolean_t direction);

/**
 * makes the car turn 90 degrees
 * @param align whether it shall turn 90 degrees right (1) or left (0)
 * @param direction whether it shall drive forward (1) or backward (0) first
 */
void turn90(boolean_t align, boolean_t direction);

/**
 * makes the car turn around (180 degrees)
 * @param align whether it shall turn 90 degrees right (1) or left (0)
 * @param direction whether it shall drive forward (1) or backward (0) first
 */
void turn180(boolean_t align, boolean_t direction);


#endif /* CALIBDRIVE_H_ */
