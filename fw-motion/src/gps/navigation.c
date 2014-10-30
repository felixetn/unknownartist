/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file navigation.c
 * @author TEAM::GPS
 * @brief contains
 * 		- navigation method which calculates driving parameters from pre calculated differenceVector (KI of the car)
 * 		- driveTask which lets car drive for a specified duration
 * 		- method drive(speed, direction, duration) to activate driveTask
 *
 **/

/*
 * Include files
 */
#include "navigation.h"
#include "help_methods.h"
#include "../api/drive.h"
#include "../api/interrupts.h"
#include "../api/seg.h"
#include "../api/us.h"
#include "../api/util.h"
//#include "math.h"
#include "mapMatrix.h"
#include "GPS_Handler.h"
#include "../cartography/initValues.h"
#include "../cartography/calibDrive.h"
#include "../cartography/slam/SLAM_Algorithm.h"
#include "../cartography/slam/SLAM_Algorithm_initialize.h"
#include "../cartography/wall_follow.h"

/*
 * Global type definitions ('typedef')
 */
typedef struct {
	int8_t speed;
	int8_t direction;
	portTickType duration;
	portTickType startTime;
} driveParameters;

static int16_t DriveLength = 0, wheel_turns = 0;
static boolean_t distanceReached = FALSE;
static int16_t distance = 0, realdistance = 0, totalDistance = 0;
static int8_t start;
static int8_t hall = 0;
static int16_t accAngle = 0;
double DrivenLength = 0.0;
portTickType driveTime = 0;
static boolean_t init = FALSE, turn_Active = FALSE;
//static left_old, right_old, front_old;

/*
 * Global and local variable declarations
 */
xQueueHandle driveParameterBuffer;

//global variables
uint8_t gpsState = GPS_STATE_IDLE;
uint8_t ownID = UNDEFINED_ID, targetID = UNDEFINED_ID;

/*
 * Global and local functions
 */
static void driveTask(void);
//static void calc_position(void);
__interrupt void irq_hall(void);

///*
// * SLAM Approximation
// */
//void start_calc_position(void){
//		left_old = us_getSensorMapData(LEFT_SENSOR).Left_Distance;
//		front_old = us_getSensorMapData(FRONT_SENSOR).Front_Distance;
//		right_old = us_getSensorMapData(RIGHT_SENSOR).Right_Distance;
//
//}

/*
 * initialization of driveTask and driveParameterBuffer
 *
 * Initialize this, if you want to let the car drive
 * start driving by calling drive(speed, direction, duration) method
 */
void navigationInit(void) {
#ifdef NAVIGATION_THREAD
	interrupts_setHandler(24, 6, irq_hall);

	driveParameterBuffer = xQueueCreate(1, sizeof(driveParameters));

	//Digitalen Input aktivieren
	PIER01_IE5 = 1;
	//Input Mode aktivieren
	DDR01_D5 = 0;
//	xHandleDrive = 0;

	//Interrupts aktivieren
	PRRR0_INT7_R = 1;
	ENIR0_EN7 = 1; //Enable interrupts
	ELVR0_LB7 = 1;
	ELVR0_LA7 = PDR01_P5;
	EIRR0_ER7 = 0; // auf 0 setzen um auslösen des nächsten Interrupts zu ermöglichen

//	DDR01_D0 = 1;

	start = PDR01_P5;

#ifdef SLAM_SCENARIO
//	start_calc_position();
	SLAM_Algorithm_initialize();
	os_registerProcessStackPriority(driveTask, "driveTask", 2990, tskIDLE_PRIORITY + 5);
#else
	os_registerProcessStackPriority(driveTask, "driveTask", 1200, tskIDLE_PRIORITY + 5);
#endif
	drive(0, 0, 700);
#ifdef DEBUG
	printf("driveTask started\n\r");
#endif
#endif
}

/**
 * Überprüft ob sich die Räder drehen
 */
boolean_t is_driving(){
	portTickType lastTime = os_getTime();
	uint8_t first = PDR01_P5;

	while (os_getTime() - lastTime < 500){
		if (first == 1 && PDR01_P5 == 0) return TRUE;
		if (first == 0 && PDR01_P5 == 1) return TRUE;
	}
	return FALSE;
}

/**
 * Auf False setzen bevor gefahren wird
 * @param Ziel erreicht
 */
void set_distanceReached(boolean_t value){
	distanceReached = value;
}

/*
 * @return neues Ziel erreicht
 */
boolean_t get_distenceReached(){
	return distanceReached;
}

/**
 * Zu fahrende Distanz in cm setzen
 * @param Entfernung in cm
 */
void set_DriveLength(int16_t length_in_cm){
	DriveLength = length_in_cm;
}

/**
 * @return Länge der zuletzt gefahrenen Etappe in cm
 */
double get_lastDrivenLength(){
	return DrivenLength;
}

/**
 * @param Länge der zuletzt gefahrenen Etappe in cm
 */
void set_lastDrivenLength(double length){
	DrivenLength = length;
}

/**
 * Ingesamt gefahrene Distanz anhand der Radumdrehung seit der
 * letzten Initialisierung
 * @param Distanz in Radumdrehungen
 */
int16_t get_TotalDistance(){
	return totalDistance*HALL_RESOLUTION;
}

/**
 * Berechnet die Differenz zwischen altem und neuem Lenkwinkel und speichert die Addition der Differenz
 * mit dem alten Winkel in accAngle
 */
void accumulateSteeringAngle(int8_t new_angle){
	int16_t calc;
	if (new_angle == INT_LEAST8_MAX)
		accAngle = 0;
	else{
		calc = accAngle - new_angle;
		if (new_angle >= 0)
			if ((calc + accAngle) <= 360)
				accAngle = calc + accAngle;
			else{
				accAngle = accAngle - 360 + calc;
			}
		else{
			if ((calc + accAngle) >= 0)
				accAngle = (accAngle - new_angle) + accAngle;
			else{
				accAngle = 360 - accAngle + calc;
			}
		}
	}

}

/**
 * @return Durchschnitt des Winkels in der letzten Fahretappe
 */
int16_t get_accumulateSteeringAngle(){
	return accAngle;
}

/*
 * calculates drive variables from differenceVector for driveTask by
 *  	- deciding if target is in reachable area ahead
 *  	- adapt steering angle by linear approximation of differenceVector-angle
 *  	- drive backwards steering to opposite direction and driving backwards if target not in reachable area ahead
 */
void navigate(gps_reducedData_t* differenceVector) {
	int8_t speed, directionPercent;

	int16_t angle, angleAbs;
	portTickType duration_ms = 500;


	//calculate angle in degree and its absolute
	//norm to -180 to 180 degree
	angle = uint16AngleToDegree(differenceVector->angle);
	angleAbs = (int16_t) abs32((int32_t) angle);

//	if (angle < -45) turn45(1,0);
//	if (angle > 45) turn45(-1,0);

	/*** specify duration and speed ****/
	//set parameters speed and duration to standard values
	speed = 2;

	/*** specify steering angle and direction ****/
	//case 1: if aim is reachable within maximal steering angle
	if (angleAbs < MAX_STEERING_ANGLE) {
		directionPercent = angle * (100 / MAX_STEERING_ANGLE); //norm to percent
	}
	//case 2: car has to drive backwards bent to reach case 1
	else {
		if (angle > 0) {
			directionPercent = -100;
		} else {
			directionPercent = 100;
		}
		speed = -2; //drive backwards
	}

	//printf("\tspeed, direction and duration calculated\n\r");

	set_DriveLength(15);
	drive(speed, directionPercent, duration_ms+200);
#ifdef DEBUG
	printf("\tdrive: (speed %d,direction %d,duration_ms %d)\n\r", speed,
			directionPercent, duration_ms);
#endif
}

//set Parameters for driving and fill the driveParameterBuffer for intertask communication
void drive(int8_t speed, int8_t directionPercent, portTickType duration_ms) {
	driveParameters par;

#ifdef NAVIGATION_THREAD
	par.direction = directionPercent;
	par.speed = speed;
	par.duration = duration_ms;
	par.startTime = os_getTime();
	xQueueSendToFront(driveParameterBuffer, &par, (portTickType)10);
//#endif
#else
	Drive_SetServo(directionPercent);
	Drive_SetMotor(speed);
#endif
}

///**
// * SLAM Approximation. Verschiebt nur auf x-und y-Achse. Keine Verbesserung der Orientierung
// * Deutlich weniger Rechenaufwand als SLAM_Algorithm
// */
//void calc_position(void){
//	gps_reducedData_t *own;
//	int16_t xnew,ynew,anglenew;
//	uint16_t left,right,front;
//	double sinn, coss;
//	int16_t diffLeft, diffRight, diffFront;
//	double dist = DrivenLength;
//	portTickType time, lastWakeTime;
//	int32_t degree;
//
//	lastWakeTime = os_getTime();
////	for (;;){
//	Seg_Hex(0x01);
////		os_frequency(&lastWakeTime, 100);
//	time = os_getTime();
//
//		if (DrivenLength != 0){
//		own = get_ownCoords();
//
//		coss = angleToDegree(own->angle);
//		coss = coss + get_accumulateSteeringAngle();
//		degree = coss;
//		coss = AnglePlus(DegreeToUint16(coss), 16383);
//		sinn = coss;
//
//		anglenew = coss;
//		degree = degree % 360;
//		if (degree > 180) degree = 180 - degree;
//		coss = (double)cos_taylor(degree);
//		sinn = (double)sin_taylor(degree);
//
//		//Vermutete neue Koordinaten
//		xnew = (dist*coss)/100 + CoordinatesToMap(own->x);
//		ynew = (dist*sinn)/100 + CoordinatesToMap(own->y);
//
//
//		//Erwartete Sensorwerte
//		left_old = left_old - (dist*sinn)/100 + (dist*coss)/100;
//		right_old = right_old + (dist*sinn)/100 + (dist*coss)/100;
//		front_old = front_old + (dist*sinn)/100 + (dist*coss)/100;
//
//		//Aktuelle Sensorwerte
//		left = us_getSensorMapData(LEFT_SENSOR).Left_Distance;
//		front = us_getSensorMapData(FRONT_SENSOR).Front_Distance;
//		right = us_getSensorMapData(RIGHT_SENSOR).Right_Distance;
//
//		if (left_old < 570 && left < 570)
//			diffLeft = (left_old - left)/5;
//		else diffLeft = 0;
//		if (right_old < 570 && right < 570)
//			diffRight = (right_old - right)/5;
//		else diffRight = 0;
//		if (front_old < 570 && front < 570)
//			diffFront = (front_old - front)/5;
//		else diffFront = 0;
//
//		//Korrektur in x Richtung
//		xnew = xnew + (cos_taylor(degree)*diffLeft)/100
//		+ (cos_taylor(degree)*diffRight)/100
//		+ (cos_taylor(degree)*diffFront)/100;;
//		//Korrektur in y Richtung
//		ynew = ynew + (sin_taylor(degree)*diffLeft)/100
//		+ (sin_taylor(degree)*diffRight)/100
//		+ (sin_taylor(degree)*diffFront)/100;;
//
//
//		//Sensorwerte umspeichern
//		left_old = left;
//		right_old = right;
//		front_old = front;
//		set_ownCoords(MapToCoordinates(xnew),MapToCoordinates(ynew),MapToCoordinates(anglenew));
//
////		send_Element(22);
////		send_Element(xnew);
////		send_Element(ynew);
////		send_Element(-1);
//
//		DrivenLength = 0;
//		}
//}

/**
 * Hilfsmethode zum Fahren Etappen fester Länge
 */
void driveDistance(void){

	if (DriveLength > 0){

		if (wheel_turns <= 0){
			wheel_turns = (DriveLength)/HALL_RESOLUTION;
			realdistance = 0;
			//Zwei Wechsel der Sensorwerte = 1 Radumdrehung = 21,04cm
			if ((DriveLength*10) % (int16_t)(HALL_RESOLUTION*10) > ((DriveLength*10)/2)) wheel_turns++; //Faktor 10 weil kein Mod mit double möglich
			set_distanceReached(FALSE);
		}

		if (Drive_Speed > 3 && wheel_turns > 2) wheel_turns = wheel_turns - 2; //weil bremsen so lange dauert

		if ( init == FALSE){
			driveTime = os_getTime();
			init = TRUE;
		}

		if (distance - wheel_turns < 0){
			Seg_Hex(0x07);
		}

		if (distance - wheel_turns >= 0){
			//Werte zurücksetzen
			wheel_turns = 0;
			init = FALSE;
			drive(0,0,0);
			Drive_SetMotor(0);
			realdistance = 0;
			DriveLength = 0;
			set_distanceReached(TRUE);
			Seg_Hex(0x08);
		}
	}
}

/*
 * set drive parameters from driveParameterBuffer if new parameters are available,
 * otherwise stop the car when specified duration expires for security reasons
 */
static void driveTask(void) {
	portTickType duration_ms = 0, directionPercent = 0, currentTime = 0,
			lastWakeTime = 0, startTime = 0;
	driveParameters par;
	int8_t queueReturnValue, speed = 0;
	double Ergebnis[3];
	gps_reducedData_t *own;

	lastWakeTime = os_getTime();

	for (;;) {
//		calls[6]++;
#ifdef SLAM_SCENARIO
		os_frequency(&lastWakeTime, 1);
#else
		os_frequency(&lastWakeTime, 60);
#endif
		currentTime = os_getTime();

		//get new drive commands from queue
		queueReturnValue = xQueueReceive(driveParameterBuffer, &par, 10);
		if (queueReturnValue == pdTRUE) {
			directionPercent = par.direction;
			speed = par.speed;
			startTime = par.startTime;
			duration_ms = par.duration;
		}
		if (hall != start){
			start = !start;
			if (os_getTime() - driveTime > 400){
				distance++;
				#ifdef SLAM_SCENARIO
				Drive_SetMotor(0);
				os_wait(200);
				own = get_ownCoords();
				SLAM_Algorithm((int32_t)CoordinatesToMap((int32_t)own->x),(int32_t)CoordinatesToMap((int32_t)own->y),(int32_t)uint16DegreeRelativeToX(own->angle),get_accumulateSteeringAngle()/MAX_STEERING_ANGLE, (int32_t) HALL_RESOLUTION, Ergebnis);
//				  send_Element(22);
//				  send_Element((int16_t)Ergebnis[0]);
//				  send_Element((int16_t)Ergebnis[1]);
//				  send_Element(-1);
				set_ownCoords(MapToCoordinates((int16_t)Ergebnis[0]),MapToCoordinates((int16_t)Ergebnis[1]),MapToCoordinates((int16_t)Ergebnis[2]));
				accumulateSteeringAngle(INT_LEAST8_MAX);
				#endif
			}
		}

		//set speed and direction if specified duration not expired, else stop the car
		if (duration_ms < 600){
			if (currentTime - startTime < duration_ms) {
			#ifdef DEBUG2
				printf("\nDRIVE\nset motor speed to %d and angle to %d\n\r", speed,
						directionPercent);
			#endif
				Drive_SetServo(directionPercent);
				Drive_SetMotor(speed);
				driveDistance();
			} else {
				Drive_SetMotor(0); //stop if time period expired;
				distance = 0;
				realdistance = 0;
			}
		}
		else{
			Drive_SetServo(directionPercent);
			Drive_SetMotor(speed);
			if (speed == 0){
				distance = 0;
				realdistance = 0;
			}
			driveDistance();
		}
	}
}

/**
 * Interrupt um die Radumdrehungen zu zählen
 */
__interrupt void irq_hall(void){
	hall = PDR01_P5;
	PDR01_P0 = hall;
	newturn = TRUE;
	realdistance++;
	totalDistance++;

#ifdef CARTOGRAPHY_SCENARIO
	if (realdistance > wheel_turns && wheel_turns != 0 && turn_Active == FALSE){
		distance = wheel_turns;
		Drive_SetMotor(0);
		Seg_Hex(0x0F);
	}
#endif

#ifdef SLAM_SCENARIO
	if (turn_Active == FALSE)
		Drive_SetMotor(0);
#endif

	ELVR0_LA7 = hall;
	EIRR0_ER7 = 0;  // auf 0 setzen um auslösen des nächsten Interrupts zu ermöglichen
}

/**
 * Dreht das Auto auf der Stelle um 12°, nutzt die Radumdrehungen als Messwert
 * Input: Drehrichtung (Links/Rechts), Fahrtrichtung (Vorwärts/Rückwärts)
 */
void turn12(uint8_t repeats, uint8_t align, uint8_t firstDirection) {
       uint8_t i = 0;
//       uint8_t repeats = 4;
//       uint8_t time_wait = 100;
//       uint16_t time_forward = 400;
//       uint16_t time_backward = 280;
       int16_t help;
       int8_t factor_speed = 1;
       uint16_t turns;
       int8_t factor_direction = 1;
 //      updateDriveTime();
       turn_Active = TRUE;
       if (align != 1){
    	   factor_speed = -1; // factor = 1 means right, = -1 means left
       }

       if (firstDirection != TRUE)
       	{
    	   factor_direction = -1; // factor = 1 means forwards, = -1 means backwards
       		help = time_forward;
       		time_forward = time_backward; // swap values
       		time_backward = help;
       	}


       Drive_SetMotor(0);

       // turns by driving forward/backward and turning the wheels
       for (i=0; i < repeats; i++) {
           os_wait(100);
           Drive_SetServo(-100*factor_speed);
           os_wait(100);
           turns = totalDistance;

           //Fahre Rückwärts
           if (Us_Data.Rear_Distance > 10){
               if ( i == repeats-1 && repeats != 1) turns+=3;
               while (turns+1 > totalDistance)
            	   drive(-2,-100*factor_speed,500);

               os_wait(100);
               Drive_SetServo(-100*factor_speed);
               os_wait(100);
           }
           turns = totalDistance;

           //Fahre Vorwärts
           if (Us_Data.Front_Distance > 10){
               if ( i == repeats-1 && repeats != 1) turns-=1;
               while (turns+5 > totalDistance)
            	   drive(2,100*factor_speed,500);
           }
           Drive_SetServo(0);
           Drive_SetMotor(0);

       }
       distance = wheel_turns;
       drive(0,0,0);
       Drive_SetMotor(0);
       Drive_SetServo(0);
       os_wait(100);

       turn_Active = FALSE;

}

/**
 * @return: 1 = 14Uhr, 2 = 13Uhr, 3 = 11 Uhr, 4 = 10Uhr, 5 = 8Uhr, 6 = 7Uhr, 7 = 5Uhr, 8 = 4Uhr (Gegen den Uhrzeigersinn)
 */
uint8_t orientation_inWK(uint16_t orientierung){
	uint8_t abschnitt = 0;

//	if (orientierung >= 0 && orientierung <= 4095) abschnitt = 5;
//	else if (orientierung <= 8191) abschnitt = 6;
//	else if (orientierung <= 12287) abschnitt = 7;
//	else if (orientierung <= 16382) abschnitt = 8;
//	else if (orientierung <= 20478) abschnitt = 9;
//	else if (orientierung <= 24573) abschnitt = 10;
//	else if (orientierung <= 28669) abschnitt = 11;
//	else if (orientierung <= 32764) abschnitt = 12;
//	else if (orientierung <= 36860) abschnitt = 13;
//	else if (orientierung <= 40955) abschnitt = 14;
//	else if (orientierung <= 45051) abschnitt = 15;
//	else if (orientierung <= 49146) abschnitt = 16;
//	else if (orientierung <= 53242) abschnitt = 1;
//	else if (orientierung <= 57337) abschnitt = 2;
//	else if (orientierung <= 61433) abschnitt = 3;
//	else if (orientierung <= 65528) abschnitt = 4;
	if (orientierung >= 0 && orientierung <= 8191) abschnitt = 3;
	else if (orientierung <= 16382) abschnitt = 4;
	else if (orientierung <= 24573) abschnitt = 5;
	else if (orientierung <= 32764) abschnitt = 6;
	else if (orientierung <= 40955) abschnitt = 7;
	else if (orientierung <= 49146) abschnitt = 8;
	else if (orientierung <= 57337) abschnitt = 1;
	else if (orientierung <= 65528) abschnitt = 2;
	return abschnitt;
}

/**
 * Dreht das Auto so lange bis es in einem Zug (ohne zu rangieren) zum Ziel fahren kann
 * Input: x,y-Koordinaten des Ziels, x,y-Koordinaten des Autos, Orientierung des Autos
 */
void look_in_direction(int16_t zielx, int16_t ziely, int16_t ownx, int16_t owny, uint16_t orientierung){
	//Einteilung des Einheitskreises in 8 Abschnitte  -> in welchem Abschnitt befindet sich das Ziel?
	int16_t absolutx, absoluty, abschnitt, help1;
	int8_t position;

	absolutx = zielx-ownx;
	absoluty = ziely-owny;

//	Seg_Hex(0x0F);
	if (abs32(absolutx) > abs32(absoluty)) help1 = 1;
	else help1 = 0;

	// Bestimmen in welchem Abschnitt sich der Zielpunkt befindet. 8 Abschnitte
	if (absolutx > 0)
		if (absoluty > 0)
			if (help1)position = 1;
			else position = 2;
		else
			if (help1) position = 8;
			else position = 7;
	else
		if (absoluty > 0)
			if (help1) position = 4;
			else position = 3;
		else
			if (help1) position = 5;
			else position = 6;

	//Bestimmen in welchen Abschnitt das Auto schaut Intervallbreite = 8191,
	if (orientierung >= 0 && orientierung <= 8191) abschnitt = 3;
	else if (orientierung <= 16382) abschnitt = 4;
	else if (orientierung <= 24573) abschnitt = 5;
	else if (orientierung <= 32764) abschnitt = 6;
	else if (orientierung <= 40955) abschnitt = 7;
	else if (orientierung <= 49146) abschnitt = 8;
	else if (orientierung <= 57337) abschnitt = 1;
	else if (orientierung <= 65528) abschnitt = 2;

	if (abschnitt == 0) return;

	help1 = position-abschnitt;

	if (abs8(help1) > 4){		//Für die Fälle wenn "Überlauf" im Kreis
		position = 4 - abs8(help1) + 4;
		if (help1 > 0) help1 = position*(-1);
		else help1 = position;
	}

	if (help1 >= 0 && help1 <=4 ) position = -1;//drehe links rum
	else position = 1;//drehe rechts rum

	if (abs16(help1) > 1){		//wenn differenz kleiner ist nur eine drehung (oder keine) notwendig
		help1 = abs16(help1);
		while (help1 >= 0){		//drehe so lange bis im richtigen Abschnitt
			turn12(1,position,1);
			help1--;
		}
	}

}

/**
 * Dreht das Auto auf einen vorgegeben Winkel bezogen auf das WKs
 */
void turn_to_angle(uint16_t aimangle, uint16_t ownangle){
	int8_t phase, position, turndirection;
	int16_t help1;


	//Bestimmen in welchen Abschnitt das Auto schaut Intervallbreite = 8191,
	phase = orientation_inWK(ownangle);
	position = orientation_inWK(aimangle);

	if (position > phase) turndirection = 1;
	else turndirection = 0;

	help1 = position-phase;

	if (abs8(help1) > 4){		//Für die Fälle wenn "Überlauf" im Kreis
		position = 4 - abs8(help1) + 4;
		if (help1 > 0) help1 = position*(-1);
		else help1 = position;
	}

	if (help1 >= 0 && help1 <=4 ) position = -1;//drehe links rum
	else position = 1;//drehe rechts rum

	if (abs16(help1) > 1){		//wenn differenz kleiner ist nur eine drehung (oder keine) notwendig
		help1 = abs16(help1);
		while (help1 >= 0){		//drehe so lange bis im richtigen Abschnitt
			turn12(1,position,1);
			help1--;
		}
	}
}
