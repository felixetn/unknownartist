/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file mapping_controller.h
 * @author mschnaubelt
 * @brief MappingController creates maps by getting information about the nodes from CrossingAnalyzer
 *        and collecting information about the edges. Mapping Controller is also responsible
 *        for defining the behavior for every CrossingType. This could be expanded by implementing
 *        AI so that the car is able to recognize crossings it has already visited and take
 *        another route.
 **/

#ifndef MAPPING_CONTROLLER_H_
#define MAPPING_CONTROLLER_H_

#include "FreeRTOS.h"
#include "task.h"
#include "../api/api.h"
#include "../api/printf_stdarg.h"
#include "../api/api.h"
#include "../api/defines.h"

extern xTaskHandle xHandleMappingController;

// 2^3 = 8 possible crossing types
typedef enum CrossingType {
	LeftOnly = 0,
	RightOnly = 1,
	StraightOnly = 2,
	LeftRight = 3,
	LeftStraight = 4,
	RightStraight = 5,
	All = 6,
	DeadEnd = 7
} CrossingType;

// the number of us sensor values for one side that shall be saved for every corridor (edge)
#define NUMBER_US_VALUES 10
extern uint8_t us_values[];

/*
 * initializes the mapping controller module
 */
void initMappingController(void);

/*
 * starts the mapping controller module
 */
void startMappingController(void);

/*
 * stops  the mapping controller module
 */
void stopMappingController(void);

/*
 * Internal function that is called periodically by the OS
 */
void MappingControllerThread(void);

/*
 * prints the detected crossing type
 * @param type the type of the detected crossing
 */
void noteCrossing(CrossingType type);

/*
 * resumes driving after a crossing has been handled
 * @param len the length that shall be driven to exit the crossing before wallFollow is called again
 */
void resumeDriving(int16_t len);

#endif /* MAPPING_CONTROLLER_H_ */
