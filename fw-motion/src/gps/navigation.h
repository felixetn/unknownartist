/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file navigation.h
 * @author TEAM::GPS
 * @brief contains
 * 		- navigation method which calculates driving parameters from pre calculated differenceVector (KI of the car)
 * 		- driveTask which lets car drive for a specified duration
 * 		- method drive(speed, direction, duration) to activate driveTask (let the car drive)
 *
 **/

#ifndef NAVIGATION_H_
#define NAVIGATION_H_

/*
 * Include files
 */
#include "commonLibraries.h"
#include "../api/defines.h"
/*
 * Global pre-processor symbols/macros ('#define')
 */
#define MAX_STEERING_ANGLE 20	//in degree
#define DISTANCE_TOLERANCE 200

extern uint8_t gpsState;
extern uint8_t ownID;
extern uint8_t targetID;

/*
 * initialization of driveTask and driveParameterBuffer
 *
 * Initialize this, if you want to let the car drive
 * start driving by calling drive(speed, direction, duration) method
 */
void navigationInit(void);

/**
 * returns driven distance in cm
 * TODO: alle calculateDrivenLen ersetzen
 */
int16_t get_driveDistance();

/**
 * calculates drive variables from differenceVector for driveTask
 */
void navigate(gps_reducedData_t* differenceVector);

boolean_t get_distenceReached(void);
void set_distanceReached(boolean_t value);
void set_DriveLength(int16_t length_in_cm);
boolean_t is_driving(void);
double get_lastDrivenLength(void);
void set_lastDrivenLength(double length);
int16_t get_accumulateSteeringAngle(void);
void accumulateSteeringAngle(int8_t new_angle);
int16_t get_TotalDistance(void);

/**
 * set Parameters for driving and fill the driveParameterBuffer for intertask communication
 */
void drive(int8_t speed, int8_t directionPercent, portTickType duration_ms);

/**
 * Turns 12°
 */
void turn12(uint8_t repeats, uint8_t align, uint8_t firstDirection);


uint8_t orientation_inWK(uint16_t orientierung);

/**
 * Drehe das Fahrzeug so das es in die Richtung des nächsten Tags schaut
 */
void look_in_direction(int16_t zielx, int16_t ziely, int16_t ownx, int16_t owny, uint16_t orientierung);

/**
 * Dreht das Auto auf einen vorgegeben Winkel bezogen auf das WKs
 */
void turn_to_angle(uint16_t aimangle, uint16_t ownangle);


#endif /* NAVIGATION_H_ */
