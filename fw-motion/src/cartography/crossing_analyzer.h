/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file crossing_analyzer.h
 * @author dnotz
 * @brief CrossingAnalyzer is able to detect crossings and determine which type of crossing it is.
 *        For the detection of edges the Canny algorithm is used. When an edge has been detected,
 *        the current OS time is stored into a variable so that the width of crossings can be
 *        calculated and handed to the MappingController.
 **/

#ifndef CROSSING_ANALYZER_H_
#define CROSSING_ANALYZER_H_


#include "FreeRTOS.h"
#include "task.h"
#include "../api/api.h"
#include "../api/printf_stdarg.h"
#include "../gps/navigation.h"
#include "../api/defines.h"

extern xTaskHandle xHandleCrossingAnalyzer;

#define NUM_VALUES_CROSSING_ANALYZER 6 // needed for crossing detection
#define MIN_PAUSE 500 // minimum time between two edges in ms
#define FRONT_DISTANCE_OBSTACLE 45 // front obstacle is detected if distance less than 45cm
#define WIDTH_CAR 20 // the width of the car

// OS times:
// t10: left edge start
// t11: left edge end
// t30: right edge start
// t31: right edge end
// t2: front obstacle detected
extern portTickType t10, t11, t30, t31, t2;
extern int16_t width_right, width_left, width_rear, width_front;

// just showing the current status
extern uint8_t initializedValues;
extern volatile uint8_t crossingDetected, crossingStarted;

// operators for the one-dimensional Canny algorithm
#define OP_LENGTH	5
extern const int8_t GAUSS_OP[];
extern const int8_t SOBEL1_OP[];
extern const int8_t SOBEL2_OP[];

/**
 * calculates the convolution of data with the operator
 * @param data the data that shall get convolved
 * @param op the operator for the convolution
 * @param result the result of the convolution
 */
void convolution(uint16_t *data, const int8_t *op, uint16_t *result);

/**
 * returns 1 if there is an obstacle in front
 */
uint8_t checkfront(void);

/**
 * printfs the first NUM_VALUES of a given array
 * @param d the array
 */
void printArray(int16_t *d);

/**
 * initializes the crossing analyzer module
 */
void initCrossingAnalyzer(void);

/**
 * starts the crossing analyzer module
 */
void startCrossingAnalyzer(void);

/**
 * stops the crossing analyzer module
 */
void stopCrossingAnalyzer(void);

/**
 * Internal function that is called periodically by the OS.
 */
void CrossingAnalyzerThread(void);

/**
 * updates the alignment of the car after an edge has been detected
 * e. g. the car will follow the right wall if only a left edge has been detected
 */
void updateAlignment(void);

/**
 * will stop the CrossingAnalyzerThread and set variables showing that the crossing has
 * been detected completely if the end of the crossing is reached
 */
void handleEndOfCrossing(void);

/**
 * Getter for the variable crossingDetected
 * @return variable crossingDetected
 */
uint8_t getCrossingDetected(void);

/**
 * Getter for the variable crossingStarted
 * @return variable crossingStarted
 */
uint8_t getCrossingStarted(void);

/**
 * Getter for the variable time_edge
 * @return variable time_edge
 */
portTickType getTimeEdge(void);

/**
 * Getter for the variable time_edge_old
 * @return variable time_edge_old
 */
portTickType getTimeEdgeOld(void);

/**
 * Getter for the variable width_right
 * @return variable width_right
 */
int16_t getWidthRight(void);

/**
 * Getter for the variable width_left
 * @return variable width_left
 */
int16_t getWidthLeft(void);

/**
 * Getter for the variable width_front
 * @return variable width_front
 */
int16_t getWidthFront(void);

/**
 * Getter for the variable width_rear
 * @return variable width_rear
 */
int16_t getWidthRear(void);

/**
 * Resets crossingStartet to 0
 */
void resetCrossingStarted(void);


#endif /* CROSSING_ANALYZER_H_ */
