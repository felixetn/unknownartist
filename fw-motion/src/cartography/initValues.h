/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file initValues.h
 * @author dnotz
 * @brief defines initial values for individual car and update the values according to
 * 		  how full its battery is
 * 		  Initial config values are read and stored from/to the SD card
 **/

#ifndef INITVALUES_H_
#define INITVALUES_H_

#include "../api/api.h"
#include "../api/util.h"
#include "../api/linesensor.h"
#include "../api/battery.h"
#include "../api/adc.h"

#include "dataOutput.h"
//#include "line_follow.h"
#include "../api/printf_stdarg.h"

#include "../sdcard/sdcard.h"
#include "../api/defines.h"

extern uint8_t time_wait; // wait between forward and backward driving
extern uint16_t time_forward; // time for forward driving
extern uint16_t time_backward; // time for backward driving
extern int8_t balancedSteerAngle; // the angle for driving straight
extern uint16_t driveSpeedReg_n_Fac10; // parameter for linear function found by linear regression
extern uint8_t driveSpeedReg_m; // parameter for linear function found by linear regression

// the measured battery level, car parameters refer to battery_level = 400;
extern uint16_t battery_level;

// length of bits for the barcode reading
extern uint8_t bc_bit_len;
extern uint8_t bc_next_bit_len;

// calibrated sensor values for line sensor
extern uint16_t calibrated_sensor[SENSOR_COUNT];
// length of white and black bits of the barcode that are measured by the car
extern uint8_t bcBitLength[2];

//parameter, will be set to 1 when config init is done
extern int8_t configInitDone;

// Struct for config values of the car
typedef struct _initValConf {
	int8_t balancedSteerAngle;
	uint16_t lineSensCalib[5];
	uint8_t barcodeBitLen[2];
	uint8_t time_wait;
	uint16_t ticks;
	uint16_t time_forward;
	uint16_t time_backward;
	uint16_t driveSpeedReg_n_Fac10; //regression parameter: -mx+n
	uint8_t driveSpeedReg_m; //regression parameter: -mx+n
} initValConf;


/*
 * updates the time for forward / backward driving according to the current battery level
 */
void updateDriveTime(void);

/*
 * writes the current configuration to a file on the SD card
 * @return 0 if successful
 */
int writeConfigToFile(void);

/*
 * reads the configuration from a file on the SD card and sets it as current config
 * @return 0 if successful
 */
int readConfigFromFile(void);

/*
 * Internal function that is called periodically by the OS
 */
void UpdateDriveTimeThread(void);

/*
 * starts the update drive time module
 * --> measures the battery level and updates the time values every 100s
 */
void startUpdateDriveTime(void);

/*
 * stops the update drive time module
 */
void stopUpdateDriveTime(void);

/*
 * initializes the update drive time module
 */
void initUpdateDriveTime(void);

#endif /* INITVALUES_H_ */
