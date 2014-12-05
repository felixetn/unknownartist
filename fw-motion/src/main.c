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

//Globale Variablen

//Aktueller Zustand des Autos (z.B. Geradeaus, Kurve, Parken)
int8_t zustand = 0; // 0 = IDLE

//Jeder Task repräsentiert einen Zustand, oder eine Extension eines Zustandes
xTaskHandle pruefeZustandXTask;
xTaskHandle fahrenXTask;
xTaskHandle gablungXTask;
xTaskHandle ausweichenXTask;
xTaskHandle parklueckeSuchenXTask;
xTaskHandle parkenXTask;

/*
 * Initialisiere PID-Regler
 * Definiere bei welchem Zustand begonnen wird!
 */
void init(){

	Regler_set_Kp(0.4);
	Regler_set_Ki(0.0);
	Regler_set_Kd(1.0);
	zustand = 0;

}

/*
 *Checkt in welchem Zustand sich das Auto befindet und
 *Aktiviert bzw. deaktiviert Tasks.
 */
static void pruefeZustand(void) {
	portTickType lastWakeTime = os_getTime();
	init();
	//os_suspendTask(testi);


	for (;;) {
		os_frequency(&lastWakeTime, 100);



		//HIER DIE TRANSITIONEN
		//z.B. if (us_getFrontDistance() < 50){
		//			zustand = 5;
		//			}


		//Zustände
		//0 IDLE
		//1 Fahren
		//2 Gablung
		//3 Ausweichen
		//4 Parklücke_Suchen
		//5 Parken


		switch(zustand){
		//IDLE
			case 0: zustand = 1;break;
		//Fahren
			case 1: os_resumeTask(fahrenXTask);

					os_suspendTask(gablungXTask);
					os_suspendTask(ausweichenXTask);
					os_suspendTask(parklueckeSuchenXTask);
					os_suspendTask(parkenXTask);
					break;
		//Gablung
			case 2: os_resumeTask(gablungXTask);

					os_suspendTask(fahrenXTask);
					os_suspendTask(ausweichenXTask);
					os_suspendTask(parklueckeSuchenXTask);
					os_suspendTask(parkenXTask);
					break;
		//Ausweichen
			case 3:	os_resumeTask(ausweichenXTask);

					os_suspendTask(fahrenXTask);
					os_suspendTask(gablungXTask);
					os_suspendTask(parklueckeSuchenXTask);
					os_suspendTask(parkenXTask);
					break;
		//Parklücke_Suchen
			case 4: os_resumeTask(parklueckeSuchenXTask);
					os_resumeTask(fahrenXTask);

					os_suspendTask(gablungXTask);
					os_suspendTask(ausweichenXTask);
					os_suspendTask(parkenXTask);
					break;
		//Parken
			case 5:	os_resumeTask(parkenXTask);

					os_suspendTask(fahrenXTask);
					os_suspendTask(gablungXTask);
					os_suspendTask(parklueckeSuchenXTask);
					os_suspendTask(ausweichenXTask);
					break;


		}

	}
}

/*
 * Die Verschiedenen Tasks
 */

static void fahren(void) {
	portTickType lastWakeTime = os_getTime();


	for (;;) {
		os_frequency(&lastWakeTime, 100);
		Regler_output(Regler_pid(500, Regler_get_sensor())); //Das Programm gibt ständig die Stellgröße aus, welche sich aus
		                                     //dem PID-Regler mit Sollwert von 100 (Beispielwert), und dem Sensorwert


	}
}

static void gablung(void) {
	portTickType lastWakeTime = os_getTime();


}

static void ausweichen(void) {
	portTickType lastWakeTime = os_getTime();


}

static void parklueckeSuchen(void) {
	portTickType lastWakeTime = os_getTime();


}

static void parken(void) {
	portTickType lastWakeTime = os_getTime();


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

	//Tasks registrieren
	pruefeZustandXTask = os_registerProcessStack(pruefeZustand, "RuntimeStats", 700);
	fahrenXTask = os_registerProcessStack(fahren, "RuntimeStats", 700);
	gablungXTask = os_registerProcessStack(gablung, "RuntimeStats", 700);
	ausweichenXTask = os_registerProcessStack(ausweichen, "RuntimeStats", 700);
	parklueckeSuchenXTask = os_registerProcessStack(parklueckeSuchen, "RuntimeStats", 700);
	parkenXTask = os_registerProcessStack(parken, "RuntimeStats", 700);
	os_suspendTask(gablungXTask);
	os_suspendTask(ausweichenXTask);
	os_suspendTask(parklueckeSuchenXTask);
	os_suspendTask(parkenXTask);
	// don't remove!
	api_StartScheduler();
}
