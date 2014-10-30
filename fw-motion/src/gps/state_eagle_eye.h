/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file state_eagle_eye.h
 * @author TEAM::GPS
 * @brief implementation of scenario eagleEye:
 * 			the car receives its own position and target position in world system format
 * 			eagleEye calculates the needed path to the target and adjusts the steering values
 **/

#ifndef STATE_EAGLE_EYE_H_
#define STATE_EAGLE_EYE_H_

#include "help_methods.h"
#include "navigation.h"
#include "mapMatrix.h"

//method to start scenario eagleEye (called by task)
void eagleEye(gps_reducedData_t* ownCoords, gps_reducedData_t* aimCoords);

#endif /* STATE_EAGLE_EYE_H_ */
