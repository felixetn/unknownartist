/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file linesensor.c
 * @author Team Highway
 * @brief Provides access to line sensor functionality. This module offers:
 * 		1) access to sensor data
 * 		2) line recognition
 * 		3) line sensor calibration
 **/

#include "linesensor.h"

int16_t sensor[SENSOR_COUNT] = { 0 };
int16_t cal_sensor[SENSOR_COUNT] = { 0 };
uint16_t calibrated_sensor[SENSOR_COUNT] = { 0 };
int32_t pos[POSITION_COUNT] = { 0 };
int16_t final_sensor[SENSOR_COUNT] = {0};

int32_t averagePos = 0;
int32_t averagePosNew = 0;
int32_t averagePosOld = 0;

/**
 * Counts the number of ticks that passed without recognizing a line.
 */
uint8_t noline = 0;
uint8_t linefound = 0;

uint8_t bcLineFound = 0;

xTaskHandle xHandleCalcLinePos;

/**
 * Calculates the position of the line. The line position varies from 1000 to 5000.
 */
static void Linesensor_calcLinePosition(void);

void Linesensor_Init() {
#ifdef LINESENSOR_TASK
	PCSR4 = PERIOD; // always set cycle value PERIODE 1st
	PDUT4 = PERIOD / 2; // set duty value DUTY CYCLE
	PCNL4 = 0x02; // Output enable
	PCNH4 = 0xD0; //D4; // Count enable, SW-Trigger, Retrigger, CLK - div 4
	xHandleCalcLinePos = os_registerProcessStack(Linesensor_calcLinePosition, "CalcLinePos", 225);
#endif
}

void readSensorData(void) {
	int i = 0;
	for (i = 0; i < SENSOR_COUNT; i++) {
		sensor[i] = ADC_GetValue(11 + i);
	}
}

//berechnung der position
static void Linesensor_calcLinePosition(void) {
	unsigned int sum; //sum for position calculation
	unsigned char valid; //1 if line found, 0 else
	long minimum;
	int i;

	portTickType lastWakeTime;
	const int frequency = 10;

	lastWakeTime = os_getTime();

	for (;;) {
		os_frequency(&lastWakeTime, frequency);
		readSensorData();

#ifdef LINESENSOR_DEBUG
		printf("sensors before software calibration: %d %d %d %d %d \r\n",
				sensor[0], sensor[1], sensor[2], sensor[3], sensor[4]);
#endif

		minimum = sensor[0];
		valid = 0;
		for (i = 0; i < SENSOR_COUNT; i++) {
			sensor[i] -= calibrated_sensor[i];
			if (sensor[i] < minimum)
				minimum = sensor[i];
			if (sensor[i] < 0)
				cal_sensor[i] = 0;
			else
				cal_sensor[i] = sensor[i];
		}
		for (i = 0; i < SENSOR_COUNT; i++) {
			sensor[i] = sensor[i] - minimum - 30;
			if (sensor[i] < 0)
				sensor[i] = 0;
			if (sensor[i] > 100)
				valid = 1;

			final_sensor[i] = sensor[i];
		}

#ifdef LINESENSOR_DEBUG
		printf("calib_data: %d %d %d %d %d\r\n", (int) calibrated_sensor[0], (int) calibrated_sensor[1], (int) calibrated_sensor[2], (int) calibrated_sensor[3], (int) calibrated_sensor[4]);
		printf("sensors_2: %d %d %d %d %d\r\n", (int) sensor[0], (int) sensor[1], (int) sensor[2], (int) sensor[3], (int) sensor[4]);
#endif

		sum = cal_sensor[0] + cal_sensor[1] + cal_sensor[2] + cal_sensor[3]
		                                                                 + cal_sensor[4];
		if (sum > 5 * 20)
			bcLineFound = 1;
		else
			bcLineFound = 0;

		sum = sensor[0] + sensor[1] + sensor[2] + sensor[3] + sensor[4];

#ifdef LINESENSOR_DEBUG
		printf("| sum: %d\r\n", sum);
#endif

		if (valid == 1) {

			for (i = POSITION_COUNT - 1; i > 0; i--) {
				pos[i] = pos[i - 1];
			}
			noline = 0;

			// Calculate Position
			pos[0] = (int32_t) (sensor[0] + sensor[1] * 2 + sensor[2] * 3
					+ sensor[3] * 4 + sensor[4] * 5) * 1000 / sum;

			// pos[9] is only zero, if no line was recognized at the previous ticks
			if (pos[POSITION_COUNT - 1] != 0) {
				linefound = 1;
				averagePos = 0;
				averagePosNew = 0;
				averagePosOld = 0;
				for (i = 0; i < POSITION_COUNT; i++) {
					averagePos += (pos[i] / POSITION_COUNT);
					if (i < POSITION_COUNT / 2) {
						averagePosNew += (pos[i] / POSITION_COUNT / 2);
					} else {
						averagePosOld += (pos[i] / POSITION_COUNT / 2);
					}
				}
			}
#ifdef LINESENSOR_DEBUG
			printf("| avg now, new, old: %d %d %d\r\n", averagePos, averagePosNew, averagePosOld);
			printf("sensors: %d %d %d %d %d\r\n",(int)sensor[0],(int)sensor[1],(int)sensor[2],(int)sensor[3],(int)sensor[4]);
#endif

		} else {
			noline++;
			if (noline > NOLINE_TICKS) {
				linefound = 0;
				for (i = 0; i < POSITION_COUNT; i++) {
					pos[i] = 0;
				}
			}
		}
	}
}

void Linesensor_Calibrate(void) {
	int i;

	printf("Calibrating linesensor!\r\n");

	os_suspendTask(xHandleCalcLinePos);

	printf("----------Calibration----------\r\n");

	//initialize calibrated sensor data
	for (i = 0; i < 5; i++) {
		calibrated_sensor[i] = 0;
	}

	for (i = 0; i < CALIBRATION_TICKS; i++) {
		int j;
		readSensorData();
		for (j = 0; j < SENSOR_COUNT; j++) {
			calibrated_sensor[j] += sensor[j];
		}
	}
	for (i = 0; i < SENSOR_COUNT; i++) {
		calibrated_sensor[i] /= CALIBRATION_TICKS;
	}

	os_resumeTask(xHandleCalcLinePos);

#ifdef LINESENSOR_DEBUG
	printf("Calibrated Sensors: %d %d %d %d %d\r\n",
			calibrated_sensor[0], calibrated_sensor[1], calibrated_sensor[2], calibrated_sensor[3], calibrated_sensor[4]);
#endif

	return;
}
