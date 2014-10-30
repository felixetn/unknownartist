/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file map.c
 * @author Tobias
 * @brief Modul zum erstellen einer Karte der Umgebung. In diesem Modul werden Sensoren verknüpft
 * und der Ablauf des Programms gesteuert
 **/

#include "../gps/navigation.h"
#include "../gps/help_methods.h"
#include "../api/seg.h"
#include "../api/us.h"
#include "math.h"
#include "../gps/GPS_Handler.h"
#include "create_map.h"
#include "wall_follow.h"
#include "crossing_analyzer.h"
#include "../gps/mapMatrix.h"
#include "../gps/state_eagle_eye.h"
#include "calibDrive.h"
#include "Stack.h"
#include "../gps/StartScenario.h"

static uint8_t init = 0;
static boolean_t active = FALSE, reached = FALSE, flag = FALSE;
static gps_reducedData_t *own;
static int16_t xold = 0, yold = 0, xold2 = 0, yold2 = 0;
static uint16_t angle_old = 0, distL = 0, distR = 0, distF = 0, angle_old2 = 0,
		distL2 = 0, distR2 = 0, distF2 = 0;
boolean_t checkDrivenArea = FALSE;

xTaskHandle xHandleCreateMap, xHandleStoreMap;

//signed portCHAR writeee[400]; //Array für RunTimeStats

void StoreMap(void);
void CreateMapThread(void);

int16_t get_OwnX(void);
int16_t get_OwnY(void);
uint16_t get_OwnOrientation(void);

void StoreMap(void);

void init_create_map(void) {
	xHandleCreateMap = 0;
	xHandleStoreMap = 0;

}

/**
 * liefert die aktuelle x-Koordinate des Autos
 */
int16_t get_OwnX() {
	own = get_ownCoords();
	return own->x;
}

/**
 * liefert die aktuelle y-Koordinate des Autos
 */
int16_t get_OwnY() {
	own = get_ownCoords();
	return own->y;
}

/**
 * liefert die aktuelle Orientierung des Autos
 */
uint16_t get_OwnOrientation() {
	own = get_ownCoords();
	return own->angle;
}

/**
 * Zustandsüberprüfung ob Kartographie aktiviert ist
 * @return True wenn aktiviert
 */
boolean_t createMap_isActivated() {
	return active;
}

/**
 * Zwischenspeicher der Informationen um diese erst im nächsten Fahrtabschnitt zu speichern
 */
void set_oldValues(int16_t x, int16_t y, uint16_t angl, uint16_t distRight,
		uint16_t distLeft, uint16_t distFront) {
	xold = x;
	yold = y;
	angle_old = angl;
	distR = distRight;
	distL = distLeft;
	distF = distFront;
}

/**
 * Zwischenspeicher der Informationen um diese erst im übernächsten Fahrtabschnitt zu speichern
 */
void set_newValues(int16_t x, int16_t y, uint16_t angl, uint16_t distRight,
		uint16_t distLeft, uint16_t distFront) {
	xold2 = x;
	yold2 = y;
	angle_old2 = angl;
	distR2 = distRight;
	distL2 = distLeft;
	distF2 = distFront;
}

/**
 * Setze flag auf True um auf neue Koordinaten zu warten
 */
void set_CoordinatesFlag(boolean_t f) {
	flag = f;
}

/**
 * @return flag
 */
boolean_t get_CoordinatesFlag(void) {
	return flag;
}

/**
 * Geradeaus fahren bis vorne ein Hindernis erkannt wird
 * Bereits gescannter Bereich zählt ebenfalls als ein Hindernis.
 */
void drive_to_obstacle(void) {
	uint16_t distance;

	if (!reached) { //Abbruchbedingung
		distance = us_getSensorMapData(FRONT_SENSOR).Front_Distance;
		if (distance >= (RANGESIDE - 5)) {

			if (distance > (RANGESIDE - 5 + 40)) {
				set_DriveLength(DRIVELENGTH);
				//Zwischenspeichern der Sensorwerte
				set_newValues(get_OwnX(), get_OwnY(), get_OwnOrientation(),
						us_getSensorMapData(RIGHT_SENSOR).Right_Distance,
						us_getSensorMapData(LEFT_SENSOR).Left_Distance, 50);
			} else {
//				set_DriveLength(distance-(RANGE-10));
				//Annähern an Wand
				if (distance > (DRIVELENGTH + 15)) {
					Seg_Hex(0x09);
					if (distance - DRIVELENGTH > DRIVELENGTH
						)
						set_DriveLength(DRIVELENGTH);
					else
						set_DriveLength(distance - DRIVELENGTH);
					if (distance - (RANGESIDE - 5) > 10) {
						set_newValues(
								get_OwnX(),
								get_OwnY(),
								get_OwnOrientation(),
								us_getSensorMapData(RIGHT_SENSOR).Right_Distance,
								us_getSensorMapData(LEFT_SENSOR).Left_Distance,
								distance - (RANGESIDE - 5));
					}
				} else {
					reached = TRUE;
					drive(0, 0, 700);
					set_DriveLength(0);
					stopWallFollow();
					stopCrossingAnalyzer();
					return;
				}
			}
			//Fahre neuen Abstand mithilfe des PID-Reglers
			Seg_Hex(0x04);
			startCrossingAnalyzer();
			startWallFollow(2, RANGESIDE);
			os_wait(300);
			drive(0, 0, 0);
			drive(0, 0, 700);
			if (distance - (RANGEFRONT - 5) > 20)
				StoreMap();

			//Auf aktuelle Koordinaten warten, nur in diesem Szenario notwendig
#ifdef CARTOGRAPHY_SCENARIO
			set_CoordinatesFlag(TRUE);
			while (flag == TRUE) {
				os_wait(100);
				Seg_Hex(0x02);
			}
#endif
		} else {
//			storeElement(3);	//Debugging
			Seg_Hex(0x00);
			reached = TRUE;
			drive(0, 0, 700);
			stopWallFollow();
			stopCrossingAnalyzer();
		}
	}
}

/**
 * Algorithmus um den Raum komplett abzufahren
 */
void start_create_map(gps_reducedData_t* ownCoords,
		gps_reducedData_t* aimCoords) {

#ifdef CREATEMAP_TASK
	own = ownCoords;

	if (init == 0) {
		active = TRUE; //setzen für CrossingAnalyzer, WallFollow
		Stack_Init();
		us = us_getSensorData();
		Stack_Push(own->x, own->y, own->angle);
		set_oldValues(get_OwnX(), get_OwnY(), get_OwnOrientation(), 0, 0, 0);
		set_newValues(get_OwnX(), get_OwnY(), get_OwnOrientation(),
				us.Right_Distance, us.Left_Distance, us.Front_Distance);
		drive_setSteeringAngleBalanced(0);
		init = 1;

		if (xHandleCreateMap != NULL
			)
			os_resumeTask(xHandleCreateMap);
		else
			xHandleCreateMap = os_registerProcessStackPriority(CreateMapThread,
					"Create Map", 1500, tskIDLE_PRIORITY + 5);

		//Alternativer Ansatz. Speichern der Karte in eigenem Thread. Kann genutzt werden um während der Fahrt abzuspeichern, sofern die Speicherzugriffe schnell genug erfolgen
//		if (xHandleStoreMap != NULL)
//					os_resumeTask(xHandleStoreMap);
//		else xHandleStoreMap = os_registerProcessStackPriority(StoreMapThread, "Store Map", 400, tskIDLE_PRIORITY + 5);
//		os_suspendTask(xHandleStoreMap);
	}
#endif
}

/**
 * Beende das Szenario create_map
 */
void stop_create_map() {
	active = FALSE;
	init = 0;
	stopCrossingAnalyzer();
	stopWallFollow();
	drive(0, 0, 500);
	if (xHandleCreateMap)
		os_suspendTask(xHandleCreateMap);

}

/**
 * Speichern der neu befahrenen Umgebung und der neu gefundenen Hindernisse
 */
void StoreMap(void) {
	int8_t i;

	//Semaphore wegen gleichzeitigem Zugriff in wall_follow
	if (Sema != NULL) {
		//Warte bis Semaphore freigegeben wird
		while (xSemaphoreTake( Sema, ( portTickType ) 10 ) != pdTRUE) {
			os_wait(10);
		}
		//Speichern der Hindernisse auf der rechten Seite
		for (i = 0; i < DRIVELENGTH / 5 - 1; i++) {
			if (obs_right.dist[i] != 0) {
				Seg_Hex(0x05);
				set_free_area(obs_right.x[i], obs_right.y[i], obs_right.dist[i],
						RIGHT_SENSOR, obs_right.angle[i], OBSTACLE);
				obs_right.dist[i] = 0;
			}
		}

		Seg_Hex(0x03);
		//Speichern des rechten Sensors
		set_free_area(CoordinatesToMap(xold), CoordinatesToMap(yold), distR,
				RIGHT_SENSOR, angle_old, DRIVEN_AREA);

		Seg_Hex(0x02);
		//Speichern des front Sensors
		set_free_area(CoordinatesToMap(xold), CoordinatesToMap(yold), distF,
				FRONT_SENSOR, angle_old, DRIVEN_AREA);
		Seg_Hex(0x03);
		//Speichern des linken Sensors
		set_free_area(CoordinatesToMap(xold), CoordinatesToMap(yold), distL,
				LEFT_SENSOR, angle_old, DRIVEN_AREA);

		//Speichern der Hindernisse auf der linken Seite
		for (i = 0; i < DRIVELENGTH / 5 - 1; i++) {
			if (obs_left.dist[i] != 0) {
				set_free_area(obs_left.x[i], obs_left.y[i], obs_left.dist[i],
						LEFT_SENSOR, obs_left.angle[i], OBSTACLE);
				obs_left.dist[i] = 0;
			}
		}

		xSemaphoreGive( Sema);
		set_oldValues(xold2, yold2, angle_old2, distR2, distL2, distF2);
	}
}

/**
 * Thread von Create_Map. Beinhaltet die Logik des Szenarios
 */
void CreateMapThread(void) {
	uint8_t i = 0, j = 3;
	int16_t aimx, aimy, ownx, owny;
	uint16_t direction, distance;
	portTickType lastWakeTime;
	boolean_t done = FALSE, Offroad_startet = FALSE;
	gps_reducedData_t *aim;
	portTickType time = os_getTime();
	uint16_t diffx, diffy;

	//Zum auslesen des Stacks pro Thread
	//	unsigned portBASE_TYPE watermark;
	//	watermark = uxTaskGetStackHighWaterMark( NULL );

	lastWakeTime = os_getTime();

	for (;;) {
//		calls[4]++;

		os_frequency(&lastWakeTime, 80);

		//Fertig wenn Stack leer (keine verpassten Abzweigungen) und kein freier Platz in Sensorreichweite
		if (done == FALSE) {

			if (Offroad_startet == FALSE
				) drive_to_obstacle();
			//Drehe Links (= 0) wenn möglich
			if (reached == TRUE) {
#ifdef CARTOGRAPHY_SCENARIO
				set_CoordinatesFlag(TRUE);
				while (flag == TRUE) {
					os_wait(100);
					Seg_Hex(0x02);
				}
#endif

				/*
				 //Speichern der RunTimeStats + Anzahl der Aufrufe pro Thread
				 //	vTaskGetRunTimeStats(writeee);
				 //	WriteToFile(++idd,(char*) &writeee, sizeof(writeee), 1);
				 //	WriteToFile(idd+1000,(char*) &calls, sizeof(calls), 1);
				 */

				/*
				 // Auslesen und sender von watermark
				 //		watermark = uxTaskGetStackHighWaterMark( NULL );
				 //		printf("wireless: %d ", (int8_t) watermark);
				 //		storeElement(22);
				 //		storeElement(1);
				 //		storeElement(watermark);
				 //		storeElement(-1);
				 */

				distance = us_getSensorMapData(LEFT_SENSOR).Left_Distance;
				if (distance > (RANGESIDE + 20) && Us_Data.Left_DataValid == 1
						&& Offroad_startet == FALSE) { //+ 20 ist toleranz wegen pid überschwingen
					//Biege links ab
					reached = FALSE;
					Seg_Hex(0x0A);
					stopCrossingAnalyzer();
					set_newValues(get_OwnX(), get_OwnY(), get_OwnOrientation(),
							us_getSensorMapData(RIGHT_SENSOR).Right_Distance,
							distance,
							us_getSensorMapData(FRONT_SENSOR).Front_Distance);
					turn90(0, 0);
					Drive_SetServo(0);
					drive(0, 0, 0);
					drive(0, 0, 700);
					StoreMap();
					set_newValues(get_OwnX(), get_OwnY(), get_OwnOrientation(),
							us_getSensorMapData(RIGHT_SENSOR).Right_Distance,
							us_getSensorMapData(LEFT_SENSOR).Left_Distance,
							us_getSensorMapData(FRONT_SENSOR).Front_Distance);
					//Fahre aus dem Drehbereich heraus, verhindert doppeltes Speichern der selben Position
					set_DriveLength(DRIVELENGTH);
					//Neustart der Kantenerennung
					initCrossingAnalyzer();
					startCrossingAnalyzer();
					startWallFollow(2, RANGESIDE);
					StoreMap();

#ifdef CARTOGRAPHY_SCENARIO
					set_CoordinatesFlag(TRUE);
					while (flag == TRUE) {
						os_wait(100);
						Seg_Hex(0x02);
					}
#endif
				} else {
					distance = us_getSensorMapData(RIGHT_SENSOR).Right_Distance;
					if (distance > (RANGESIDE + 20) && Us_Data.Right_DataValid == 1
							&& Offroad_startet == FALSE) {
						//Biege nach rechts ab
						reached = FALSE;
						Seg_Hex(0x0B);
						stopCrossingAnalyzer();
						set_newValues(
								get_OwnX(),
								get_OwnY(),
								get_OwnOrientation(),
								distance,
								us_getSensorMapData(LEFT_SENSOR).Left_Distance,
								us_getSensorMapData(FRONT_SENSOR).Front_Distance);
						turn90(1, 0);
						Drive_SetServo(0);
						drive(0, 0, 0);
						drive(0, 0, 700);
						StoreMap();
						set_newValues(
								get_OwnX(),
								get_OwnY(),
								get_OwnOrientation(),
								us_getSensorMapData(RIGHT_SENSOR).Right_Distance,
								us_getSensorMapData(LEFT_SENSOR).Left_Distance,
								us_getSensorMapData(FRONT_SENSOR).Front_Distance);
						set_DriveLength(DRIVELENGTH);
						initCrossingAnalyzer();
						startCrossingAnalyzer();
						startWallFollow(2, RANGESIDE);
						StoreMap();

#ifdef CARTOGRAPHY_SCENARIO
						set_CoordinatesFlag(TRUE);
						while (flag == TRUE) {
							os_wait(100);
							Seg_Hex(0x02);
						}
#endif
					} else {

#ifdef CARTOGRAPHY_SCENARIO
						set_CoordinatesFlag(TRUE);
						while (flag == TRUE) {
							os_wait(100);
							Seg_Hex(0x02);
						}
#endif
						//Um die letzten Sensorwerte noch abzuspeichern
						if (Offroad_startet == FALSE) {
							StoreMap();
							os_wait(300);
							StoreMap();
							os_wait(300);
							set_oldValues(get_OwnX(), get_OwnY(),
									get_OwnOrientation(), Us_Data.Right_Distance,
									Us_Data.Left_Distance, Us_Data.Front_Distance);
							StoreMap();
						}
						//Überprüfe auf vergangene Abzweigungen
						if (Stack_isEmpty() == FALSE) {
							Seg_Hex(0x0C);
							os_wait(300);
							stopWallFollow();
							stopCrossingAnalyzer();
//						Offroad_startet = TRUE;

							own = get_ownCoords();
							if (Offroad_startet == FALSE
									&& Stack_Top(&aimx, &aimy, &direction) != -1) {
								//Wenn Abzweigung vorhanden überprüfe ob diese angefahren werden muss
								drive(0, 0, 700);
								os_wait(300);
								stopWallFollow();
								stopCrossingAnalyzer();

								checkDrivenArea = TRUE;
								ownx = ((RANGEFRONT - 1)
										* cos_Taylor(
												uint16DegreeRelativeToX(
														direction))) / 100
										+ aimx;
								owny = ((RANGEFRONT - 1)
										* sin_Taylor(
												uint16DegreeRelativeToX(
														direction))) / 100
										+ aimy;

								aimx = (2
										* cos_Taylor(
												uint16DegreeRelativeToX(
														direction))) / 100
										+ aimx;
								aimy = (2
										* sin_Taylor(
												uint16DegreeRelativeToX(
														direction))) / 100
										+ aimy;

//							send_Element(22);
//							send_Element(4);
//							send_Element(aimx);
//							send_Element(aimy);
//							send_Element(direction-32768);
//							send_Element(-1);

								if (calc_reachability(&ownx, &owny,
										CoordinatesToMap(ownx),
										CoordinatesToMap(owny),
										CoordinatesToMap(aimx),
										CoordinatesToMap(aimy)) == 0) {
									active = FALSE;
									setCoordinates(aim, own->cam_id, 5, aimx,
											aimy, direction, own->isWorld);
									StartMapNavigationOffroad(own, aim);
									Offroad_startet = TRUE;
									active = TRUE;
								} else {
									Seg_Hex(0x08);
//								os_wait(1000);
									Stack_Pop();
								}
								checkDrivenArea = FALSE;
							} else if (Offroad_startet == TRUE) {
								diffx = giveDifferenceAbsolute(aim->x, own->x);
								diffy = giveDifferenceAbsolute(aim->y, own->y);
								//Vom Stack entfernen wenn Zielpunkt erreicht wurde
								if (get_targetReached() == 1
										|| (diffx < 1000 && diffy < 1000)) {
									Seg_Hex(0x06);
//								os_wait(3000);
									Stack_Pop();
									Offroad_startet = FALSE;
									set_DriveLength(DRIVELENGTH);
									initCrossingAnalyzer();
									startCrossingAnalyzer();
									startWallFollow(2, RANGESIDE);
								} else {
//								send_Element(22);
//								send_Element(3);
//								send_Element(aim->x);
//								send_Element(aim->y);
//								send_Element(aim->angle-32768);
//								send_Element(-1);
									active = FALSE;
									StartMapNavigationOffroad(own, aim);
									active = TRUE;
								}
							}
						} else {
							//Alle Zwischenpunkte abgefahren
							Seg_Hex(0x0d);
							os_wait(1000);
							done = TRUE;
						}
					}
				}
			}
		} else {
			Seg_Hex(0x01);
			stopWallFollow();
			stopCrossingAnalyzer();
			os_suspendTask(xHandleCreateMap);
		}
	}
}
