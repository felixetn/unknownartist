/* Demo application includes. */
// TEST2
#include "api/api.h"
#include "api/seg.h"
#include "AndroidBTControl.h"
#include "gps/GPS_Handler.h"
#include "cartography/wall_follow.h"
#include "sdcard/sdcard.h"
#include "cartography/initValues.h"
#include "cartography/line_follow.h"
#include "cartography/barcode_reader.h"
#include "cartography/crossing_analyzer.h"
#include "cartography/mapping_controller.h"
//#include "cartography/debugConfig.h"
#include "cartography/create_map.h"
#include "mode_controller.h"
#include "modes.h"

#include "api/drive.h"

#include "api/carid.h"
#include "api/interrupts.h"

#include "api/wireless/wireless.h"
#include "gps/navigation.h"

//#define ANDROID_BT_CONTROL
//#define GPSCONTROL_DEMO
//#define LINESENSOR_DEMO

#ifdef SD_TASK
static xTaskHandle xHandleInit;

void SDCardThread(void) {
//	calls[8]++;
	// initialization of the SD card, read config
	initSDCard(); //wenn init nicht ganz durchläuft kann der Scheduler blockiert werden -> Lösung Prios einsetzen, Scheduler läuft aber auslesen der Datei nicht zwingend erfolgreich
	// wait for SD Card and config
	while (!configInitDone)
		os_wait(100);

#ifdef SDCARD_DEBUG
	printf("Read config!\r\n");
#endif

	// initialize modules;
	initModeController();
	initLineFollow();
	initWallFollow();
	initBarcodeReader();
	initCrossingAnalyzer();
	init_create_map();
	initUpdateDriveTime();
	initMappingController();
	// register the different modes
	// start module threads
#ifdef NAVIGATION_SCENARIO
	registerModes();
	startUpdateDriveTime();
	os_wait(100);
	startModeController();
#endif

	// suspend oneself
	os_suspendTask(xHandleInit);
}
#endif

static void testtask(void) {
	portTickType lastWakeTime = os_getTime();
	uint8_t a = 0;

	Drive_SetMotor(0);
	Drive_SetServo(-100);
	for (;;) {
		os_frequency(&lastWakeTime, 1500);
		if(us_getLeftDistance()<=10 && a <=2){
			a++;
		}
		if(us_getLeftDistance()<=25 && a>2){
						a=0;
		}

		if(a==0){
			Drive_SetServo(0);
		}
		if(a==1){
			Drive_SetServo(100);
		}
		if(a==2){
					Drive_SetServo(-100);
		}
		wirelessFormattedDebugMessage(WI_IF_AMB8420, "Dist before: %d", (uint16_t) Drive_GetTotalDrivenDistance());
		wirelessFormattedDebugMessage(WI_IF_AMB8420, "batterie power: %d", Battery_GetVoltage());
		wirelessFormattedDebugMessage(WI_IF_AMB8420, "Distanz Vorne: %d", us_getFrontDistance());

		//		RuntimeStats_Print();
	}
}

void main(void) {
	//Initialisieren der Hardware
	api_init();

	//Intialisieren der SD-Karte
#ifdef SD_TASK
	xHandleInit = os_registerProcessStackPriority(SDCardThread, "SDCardThread", 580, tskIDLE_PRIORITY + 4); //Ohne Prio problem mit SD_Init
#endif

	wirelessDebugMessage(WI_IF_AMB8420, "RF modul ok");

	//Starte Szenarien
#ifdef ANDROID_SCENARIO
	Init_AndroidBTControl();
#endif
#ifdef GPS_SCENARIO
	Init_GPS_Handler();
#endif
#ifdef LINESENSOR_DEMO
	Init_LineSensorDemo();
#endif

#ifdef GPS_SCENARIO
	ownID = carid;
#endif

 Drive_SetMotor(5);
//	Drive_SetMotorForDistance(-1, 300);

	DDR03 = 0xff;
	PDR03 = 0x00;

	os_registerProcessStack(testtask, "RuntimeStats", 700);

	// don't remove!
	api_StartScheduler();
}
