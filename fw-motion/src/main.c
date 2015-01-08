/* Demo application includes. */
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



//@UnknownArtist
#include "api/transitionen.h"
#include "api/gablung.h"
#include "api/ausweichen.h"
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

//@UnknownArtist
//Variablen

//Aktueller Zustand
int8_t zustand = 0;
//0 = Parallel; 1 = Rückwärts;
int8_t parkZustand = 0;

void init(){

	Regler_set_Kp(0.6);
	Regler_set_Ki(0.0025);
	Regler_set_Kd(3.0);			//Wert suchen zwischen 1.5 < x < 3.0
	Regler_set_sollwert(380);
	zustand = 0;


	//Test Einparken
	//Transition_setParken(1);
	//rückwärts parken
	//parkZustand = 1;
}


void fahren(void) {
	portTickType lastWakeTime = os_getTime();

		//os_suspendTask(fahrenXTask);
		os_frequency(&lastWakeTime, 100);


		Drive_SetMotor(3);

		Regler_output(Regler_pid(Regler_get_sensor())); //Das Programm gibt ständig die Stellgröße aus, welche sich aus
														//dem PID-Regler mit Sollwert von 100 (Beispielwert), und dem Sensorwert

}

void gablung(void) {

	portTickType lastWakeTime = os_getTime();


		os_frequency(&lastWakeTime, 100);
	Gablung_entscheideRichtung();

}

void ausweichen(void) {
	portTickType lastWakeTime = os_getTime();

		os_frequency(&lastWakeTime, 100);
		ausweichmanoever();

}

void parken(void) {
	portTickType lastWakeTime = os_getTime();

	os_frequency(&lastWakeTime, 100);
	if(parkZustand == 0){
		PARKEN_parallel();
	}else{
		PARKEN_rueckwaerts();
	}


}




















/**
 * Implementierung eines endlichen Automates. Dieser Wechselt zwischen den Verschiedenen Zuständen des Autos
 * |Fahren|Gablung|Ausweichen|Parken|
 */
static void Automat(void) {
	portTickType lastWakeTime = os_getTime();

	//Einmalige Initialisierungen
	init();


	for (;;) {
		os_frequency(&lastWakeTime, 100);

		//wirelessFormattedDebugMessage(WI_IF_AMB8420, "Dist before: %d", (uint16_t) Drive_GetTotalDrivenDistance());

		zustand = Transitionen_getNextZustand(zustand);


		//fahren();
		//Seg_Dec(zustand);

		switch(zustand){
			case 0 : fahren();Seg_Dec(0);break;
			case 1 : gablung();Seg_Dec(1);break;
			case 2 : ausweichen();Seg_Dec(2);break;
			case 3 : parken();Seg_Dec(3);break;
		}



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
	//Init_AndroidBTControl();
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



	DDR03 = 0xff;
	PDR03 = 0x00;
	//2385 Byte ist der Maximale Stack
	os_registerProcessStack(Automat, "RuntimeStats", 2385);

	// don't remove!
	api_StartScheduler();
}
