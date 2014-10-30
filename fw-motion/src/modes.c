/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file modes.c
 * @author pnotz
 * @brief Modes is used for defining and registering the different modes which then
 * can be selected via the two switches on the car.
 **/

#include "modes.h"
#include "api/api.h"
#include "gps/GPS_Handler.h"
#include "api/seg.h"
#include "cartography/wall_follow.h"
//#include "cartography/barcode_reader.h"
#include "sdcard/sdcard.h"
#include "cartography/initValues.h"
#include "cartography/crossing_analyzer.h"
#include "gps/map.h"
#include "mode_controller.h"
#include "cartography/calibDrive.h"
#include "api/linesensor.h"

#ifdef CARTOGRAPHY_SCENARIO
static void startConfigurationMode(void);
static void stopConfigurationMode(void);

static void startWaitMode(void);
static void stopWaitMode(void);

static void startDemoMode(void);
static void stopDemoMode(void);
#endif

void registerModes(void) {
#ifdef CARTOGRAPHY_SCENARIO
	registerMode(0, &startWaitMode, &stopWaitMode);
	registerMode(1, &startDemoMode, &stopDemoMode);
//	registerMode(2, &startBarcodeReaderMode, &stopBarcodeReaderMode);
	registerMode(3, &startConfigurationMode, &stopConfigurationMode);
#endif
}
/*
 void startBarcodeReaderMode(void) {
 printf("started barcode reader mode \r\n");
 startWallFollow(2, 0);
 startBarcodeReader();
 }

 void stopBarcodeReaderMode(void) {
 stopWallFollow();
 stopBarcodeReader();
 printf("stopped barcode reader mode \r\n");
 }
 */
#ifdef CARTOGRAPHY_SCENARIO
/**
 * configure all parameters and write them on the SD card
 */
static void startConfigurationMode(void) {
//	Seg_Hex(0xc);
	Linesensor_Calibrate();// linesensor
	os_wait(200);
	measure_SteeringAngle();// steering angle, car has to be placed in front of a straight black line

	// there are no methods yet to calculate all values of the car
	// e.g. the parameters of the linear function have to be calculated by a self performed
	// series of measurements and linear regression.
	// such values can then be set here manually.

	writeConfigToFile();
	os_wait(1000);
}

static void stopConfigurationMode(void) {
	// Nothing to do here
}

static void startWaitMode(void) {

	printf("started wait mode \r\n");
}

static void stopWaitMode(void) {

	printf("stopped wait mode \r\n");
}

static void startDemoMode(void) {
	printf("started demo mode \r\n");

	startWallFollow(2, 0);
	startCrossingAnalyzer();
	startMappingController();
}

static void stopDemoMode(void) {
	stopWallFollow();
	stopCrossingAnalyzer();
	stopMappingController();
	printf("stopped demo mode \r\n");
}
#endif
