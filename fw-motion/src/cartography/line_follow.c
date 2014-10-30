/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file line_follow.c
 * @author pnotz
 * @brief LineFollow provides the possibility to follow a black line on the floor using
 *        the linesensors
 **/

#include "line_follow.h"
#include "../api/api.h"
#include "../api/linesensor.h"
#include "initValues.h"

static long steer = 0;

static xTaskHandle xHandleLineFollow;

// Proportional part of control.
static long steerP = 0;
// Differential part of control.
static int steerD = 0;
// Integral part of control.
static long steerI = 0;

static long integ = 0;

static const uint8_t line_Kd = 3;
static const uint16_t line_Ki = 10000;

void initLineFollow(void) {
	xHandleLineFollow = 0;
}

void stopLineFollow(void) {
	if (xHandleLineFollow != 0)
		os_suspendTask(xHandleLineFollow);
}

void startLineFollow(void) {
#ifdef LINEFOLLOW_TASK
	if (xHandleLineFollow == 0)
		xHandleLineFollow = os_registerProcessStack(LineFollowThread, "LineFollow", 100);
	else
		os_resumeTask(xHandleLineFollow);
	Drive_SetMotor(1); //initial speed
#endif
}

void steerToLine(void) {

	uint16_t iMax = 30 * line_Ki;

	//propotional term of steering
	steerP = averagePos;
	steerP -= 3000;

	// Linear Steering. Quadratic steering does not work sufficiently better to justify
	// using it.
	steerP /= 20;
	steerP = -steerP;

	//derivative term of steering
	steerD = averagePosNew;
	steerD = (steerD - averagePosOld);
	steerD = -steerD / line_Kd;

	//integral term of steering with control variable limitation
	integ = integ + averagePos - 3000;
	if (integ > iMax)
		integ = iMax;
	if (integ < -iMax)
		integ = -iMax;
	steerI = -integ / line_Ki;

	//add all terms
	steer = steerP + steerD + steerI;
#ifdef LINESENSOR_DEBUG
	printf("Pos: %d | P-Anteil: %d | D-Anteil: %d | I-Anteil: %d | gesamt: %d\r\n",(int)averagePos,(int)steerP,(int)steerD,(int)steerI,(int)steer);
#endif

	drive_setSteeringAngleBalanced((int8_t) steer);

}

void LineFollowThread(void) {

	for (;;) {
		if (linefound == 1) {
			steerToLine();
			os_wait(10);
		} else
			os_wait(10 << 2); // if no line has been found, wait 4 times longer
	}

}
