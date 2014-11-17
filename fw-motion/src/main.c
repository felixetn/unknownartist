/* Demo application includes. */
// TEST2122222222222muddi
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

#include "api/regler.h" // @unknownartist
#include "api/base_type.h"

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

	/*int16_t soll = 70;
	int16_t fehler = 0;
	int16_t fsum = 0;
	int16_t fdif = 0;
	int16_t ist = 0;
	int16_t fprev = us_getRightDistance();
	int16_t i;
	int16_t res = 0;
	int16_t count = 0;*/

	 Drive_SetMotor(3);
	 Drive_SetServo(0);

	Regler_set_Kp(0.4);
	Regler_set_Ki(0.0);
	Regler_set_Kd(1.0);

	for (;;) {
		os_frequency(&lastWakeTime, 100);

		Regler_output(Regler_pid(500, Regler_get_sensor())); //Das Programm gibt ständig die Stellgröße aus, welche sich aus
		                                     //dem PID-Regler mit Sollwert von 100 (Beispielwert), und dem Sensorwert

		/*if(us_getFrontDistance() > 0 && us_getFrontDistance() < 50){
			Drive_SetMotor(-1);
		}
		else if(us_getFrontDistance() >= 50){
			Drive_SetMotor(2);

		count++;

		ist= us_getRightDistance();
		fehler = ist-soll;
		if(count > 5)
			{
			fdif = ist - fprev;
			fsum = ( fsum + fehler);
			count = 0;

			}/*else{
				fdif = 0;
				fprev = ist;
			}*//*

		if(count == 0){
			fdif = 0;
			fprev = ist;
		}

		res = fdif*30 + 5*fehler + fsum/250;

		if(res < -100){
			res = -100;
		}
		if (res > 100){
			res = 100;
		}
		Drive_SetServo(res);

		}else if(Drive_Servo < 0){
			Drive_SetMotor(1);
			Drive_SetServo(-100);
		}else{
			Drive_SetMotor(1);
			Drive_SetServo(100);
		}
	}*/
		//wirelessFormattedDebugMessage(WI_IF_AMB8420, "Dist before: %d", (uint16_t) Drive_GetTotalDrivenDistance());
		//wirelessFormattedDebugMessage(WI_IF_AMB8420, "batterie power: %d", Battery_GetVoltage());
		//wirelessFormattedDebugMessage(WI_IF_AMB8420, "Distanz Vorne: %d", us_getFrontDistance());
		//		RuntimeStats_Print();
		//wirelessFormattedDebugMessage(WI_IF_AMB8420, "Fehler: %d", Regler_get_fehler());
		//wirelessFormattedDebugMessage(WI_IF_AMB8420, "Summe: %d", fsum);
		//wirelessFormattedDebugMessage(WI_IF_AMB8420, "Differenz: %d", Regler_get_fdif());
		//wirelessFormattedDebugMessage(WI_IF_AMB8420, "Lenkung: %d", res);
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

//	Drive_SetMotorForDistance(-1, 300);

	DDR03 = 0xff;
	PDR03 = 0x00;

	os_registerProcessStack(testtask, "RuntimeStats", 700);

	// don't remove!
	api_StartScheduler();
}
