/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file linesensorDemo.c
 * @author Team Highway
 * @brief linesensor demo tasks
 **/

/*
 * Include files
 */
#include "linesensorDemo.h"
#include "api/api.h"
#include "api/drive.h"
#include "api/linesensor.h"

/**
 * Speed the car starts with.
 */
const int initialSpeed = 1;

static long steer = 0;
/**
 * Proportional part of control.
 */
static long steerP = 0;
/**
 * Differential part of control.
 */
static int steerD = 0;

/**
 * Integral part of control.
 */
static long steerI = 0;

static long integ = 0;

static xTaskHandle xHandleSteerToLine;

static const int Kd = 3;
static const int Ki = 10000;

void Init_LineSensorDemo(void) {
#ifdef LINESENSOR_DEMO
	xHandleSteerToLine = os_registerProcessStack(runDemoLine, "DemoLine",250);
#endif
}

static void steerToLineDemo(void) {

	long iMax = 30 * Ki;

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
	steerD = -steerD / Kd;

	//integral term of steering with control variable limitation
	integ = integ + averagePos - 3000;
	if (integ > iMax)
		integ = iMax;
	if (integ < -iMax)
		integ = -iMax;
	steerI = -integ / Ki;

	//add all terms
	steer = steerP + steerD + steerI;
	//printf("Pos: %d | P-Anteil: %d | D-Anteil: %d | I-Anteil: %d | gesamt: %d\r\n",(int)averagePos,(int)steerP,(int)steerD,(int)steerI,(int)steer);

	Drive_SetServo((int8_t) steer);

}

/**
 * function to change the lane
 */
void laneChange(void) {
	int steerLC = -40;

	os_suspendTask(xHandleSteerToLine);
	Drive_SetServo(steerLC);

	//Seg_Dec(0);

	while (linefound == 1) {
//		Linesensor_calcLinePosition();
		os_wait(1000);
	}

	//Seg_Dec(1);
	Drive_SetServo(0);

	if (steerLC < 0)
		steerLC = 60;
	else
		steerLC = -40;

	//Seg_Dec(2);
	os_resumeTask(xHandleSteerToLine);
}

void runDemoLine(void) {

	Linesensor_Calibrate();

	Drive_SetMotor(initialSpeed);

	for (;;) {
		if (linefound == 1) {
			steerToLineDemo();
		}
		os_wait(10);
	}
}
