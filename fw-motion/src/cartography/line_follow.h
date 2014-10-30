/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file line_follow.h
 * @author pnotz
 * @brief LineFollow provides the possibility to follow a black line on the floor using
 *        the linesensors
 **/

#ifndef LINE_FOLLOW_H_
#define LINE_FOLLOW_H_

#include "FreeRTOS.h"
#include "task.h"
#include "../api/defines.h"
#include "calibDrive.h"

extern xTaskHandle xHandleLineFollow;

/**
 * Initialize line follow thread
 */
void initLineFollow(void);

/**
 * Steer towards the line.
 */
void steerToLine(void);

/**
 * Task to follow line
 */
void LineFollowThread(void);

/**
 * Start line follow function
 */
void startLineFollow(void);

/**
 * Suspend line follow task
 */
void stopLineFollow(void);

#endif /* LINE_FOLLOW_H_ */
