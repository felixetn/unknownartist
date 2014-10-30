/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file initValues.c
 * @author dnotz
 * @brief defines initial values for individual car and update the values according to
 * 		  how full its battery is
 * 		  Initial config values are read and stored from/to the SD card
 **/

/*
 * Usual init values:
 * balanced steering angle: 16
 *
 * for line sensor calibration on white paper:
 * 45 - 45 - 62 - 45 - 44
 *
 * bcBitLength[0] estimate at 1->0 edge (black / 1-Bit Len) - 20
 * bcBitLength[1] estimate at 0->1 edge (white / 0-Bit Len) - 15
 *
 * time_wait = 100
 * ticks = 20231
 * time_forward = 240
 * time_backward = 175
 * driveSpeedReg_n_Fac10 = 5576
 * driveSpeedReg_m = 89
 *
 */

#include "initValues.h"
#include "../api/battery.h"

static uint16_t ticks = 20231; // for battery_level = 400;
static uint8_t time_wait = 100;
uint16_t time_forward = 240;
uint16_t time_backward = 240;
int8_t balancedSteerAngle = 25;	// 13-> 20
static uint16_t battery_level = 400;
uint16_t driveSpeedReg_n_Fac10 = 5576;
uint8_t driveSpeedReg_m = 89;

int8_t configInitDone = -1;
static xTaskHandle xHandleUpdateDriveTime;

/*
 * updates the time for forward / backward driving according to the current battery level
 */
void updateDriveTime() {
	uint16_t new_ticks;
	float factor = 1;
	uint16_t new_battery_level = Battery_GetVoltage();

	new_ticks = driveSpeedReg_n_Fac10*10 - driveSpeedReg_m * new_battery_level; // formula found by linear regression
	factor = (float) new_ticks / ticks;

	// adjustment because of acceleration time, empirical value
	if (factor > 1)
		factor -= (factor - 1) * 0.4;
	else
		factor += (1 - factor) * 0.5;

	// update the values
	time_forward *= factor;
	time_backward *= factor;

#ifdef INITVALUES_DEBUG
	printf("Updates:\r\n");
	printf("Time_forward: %d\r\n", time_forward);
	printf("Time_backward: %d\r\n", time_backward);
	printf("Battery_level: %d\r\n", new_battery_level);
#endif

	battery_level = new_battery_level;
	ticks = new_ticks;
}

/*
 * Internal function that is called periodically by the OS
 */
void UpdateDriveTimeThread(void) {
	updateDriveTime();
	os_wait(100000); // poll the battery level every 100sec
}

/*
 * starts the update drive time module
 * --> measures the battery level and updates the time values every 100s
 */
void startUpdateDriveTime(void) {

#ifdef UPDATEDRIVETIME_TASK
	if (xHandleUpdateDriveTime != 0)
		os_resumeTask(xHandleUpdateDriveTime);
	else
		xHandleUpdateDriveTime = os_registerProcessStack(UpdateDriveTimeThread, "UpdateDriveTime", 120);
#endif
}

/*
 * stops the update drive time module
 */
void stopUpdateDriveTime(void) {
	if (xHandleUpdateDriveTime)
		os_suspendTask(xHandleUpdateDriveTime);
}

/*
 * initializes the update drive time module
 */
void initUpdateDriveTime(void) {
	xHandleUpdateDriveTime = 0;
}

/*
 * reads the configuration from a file on the SD card and sets it as current config
 * @return 0 if successful
 */
int readConfigFromFile() {
	int j;
	initValConf myConf;

	//read config file into struct myConf
	if (ReadFromFile(-1, (char*) &myConf, 0, sizeof(initValConf), 1) != 0)
		return -1;

	//copy values from struct to corresponding variables
	balancedSteerAngle = myConf.balancedSteerAngle;

	for (j = 0; j < 5; j++) {
		calibrated_sensor[j] = myConf.lineSensCalib[j];
	}

	for (j = 0; j < 2; j++) {
		bcBitLength[j] = myConf.barcodeBitLen[j];
	}

	time_wait = myConf.time_wait;
	ticks = myConf.ticks;
	time_forward = myConf.time_forward;
	time_backward = myConf.time_backward;
	driveSpeedReg_n_Fac10 = myConf.driveSpeedReg_n_Fac10;
	driveSpeedReg_m = myConf.driveSpeedReg_m;

//	storeElement(22);
//	storeElement(time_forward);
//	storeElement(time_backward);
//	storeElement(balancedSteerAngle);
//	storeElement(-1);

#ifdef INITVALUES_DEBUG
	printf("Reading SD Card config file: \r\n");
/*	printf("lineSensCalib[0] : %d\r\n", calibrated_sensor[0]);
	printf("lineSensCalib[1] : %d\r\n", calibrated_sensor[1]);
	printf("lineSensCalib[2] : %d\r\n", calibrated_sensor[2]);
	printf("lineSensCalib[3] : %d\r\n", calibrated_sensor[3]);
	printf("lineSensCalib[4] : %d\r\n", calibrated_sensor[4]);
*/
	printf("balancedSteerAngle : %d\r\n", balancedSteerAngle);

//	printf("bcBitLength BLACK : %d\r\n", bcBitLength[0]);
//	printf("bcBitLength WHITE : %d\r\n", bcBitLength[1]);

	printf("time_wait : %d\r\n", time_wait);
	printf("ticks : %d\r\n", ticks);
	printf("time_forward : %d\r\n", time_forward);
	printf("time_backward : %d\r\n", time_backward);
	printf("driveSpeedReg_n_Fac10 : %d\r\n", driveSpeedReg_n_Fac10);
	printf("driveSpeedReg_m : %d\r\n", driveSpeedReg_m);

#endif

	configInitDone = 1;

	return 0;
}

/*
 * writes the current configuration to a file on the SD card
 * @return 0 if successful
 */
int writeConfigToFile() {
	int j;
	initValConf myConf;

	//copy values from variables to struct myConf

	myConf.balancedSteerAngle = balancedSteerAngle;

	for (j = 0; j < 5; j++) {
		myConf.lineSensCalib[j] = calibrated_sensor[j];
	}

	for (j = 0; j < 2; j++) {
		myConf.barcodeBitLen[j] = bcBitLength[j];
	}

	myConf.time_wait = time_wait;
	myConf.ticks = ticks;
	myConf.time_forward = time_forward;
	myConf.time_backward = time_backward;
	myConf.driveSpeedReg_n_Fac10 = driveSpeedReg_n_Fac10;
	myConf.driveSpeedReg_m = driveSpeedReg_m;

	//write myConf into config file on SD card
	return WriteToFile(-1, (char*) &myConf, sizeof(initValConf), 1);

}
