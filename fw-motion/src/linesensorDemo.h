/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file linesensorDemo.h
 * @author Team Highway
 * @brief header file for linesensor demo
 **/

#ifndef LINESENSORDEMO_H_
#define LINESENSORDEMO_H_

#include "cartography/line_follow.h"
#include "api/defines.h"

void Init_LineSensorDemo(void);

///**
// * Steer towards the line.
// */
//void steerToLine(void);

/**
 * Change the lane. This method assumes that the car starts on the right lane.
 */
void laneChange(void);


/*
 * demo for following lines
 */
void runDemoLine(void);


#endif /* LINESENSORDEMO_H_ */
