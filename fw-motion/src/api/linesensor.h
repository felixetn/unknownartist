/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file linesensor.h
 * @author Team Highway
 * @brief header file for line sensor module
 **/

#ifndef LINESENSOR_H_
#define LINESENSOR_H_

#include "api.h"
#include "adc.h"
#include "seg.h"
#include "printf_stdarg.h"
#include "defines.h"

// 500 µs for CLKP1 = 24 MHz, Divider = 4
#define PERIOD 2 // 2999
#define SENSOR_COUNT 5
#define POSITION_COUNT 10
#define CALIBRATION_TICKS 50 // <= 15!
#define NOLINE_TICKS 3

//#define PRINT_VALUES_FOR_HARDWARE_CALIBRATION

/**
 * 1 if a line was found, 0 otherwise.
 */
extern uint8_t linefound;

extern int32_t averagePos;
extern int32_t averagePosNew;
extern int32_t averagePosOld;
extern int16_t final_sensor[SENSOR_COUNT]; // = { 0 };

/**
 * Initialises Power Management for line sensors. Seems to be necessary even if
 * current is provided by VCC.
 */
void Linesensor_Init(void);

/**
 * Calibrate the line sensors. For calibration, the ground and lighting beneath the line sensor
 * should be as equal as possible. This implies that no line should be present.
 */
void Linesensor_Calibrate(void);

#endif /* LINESENSOR_H_ */
