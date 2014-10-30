/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file wall_follow.c
 * @author pnotz
  * @brief WallFollow allows to drive with a specific distance to the left or the right wall
 *        or to the center. Therefore the side us sensors are used and the new steering angle is
 *        calculated by PID controller every 100ms.
 **/

/*
 * Include files
 */
#include "wall_follow.h"
#include "../gps/navigation.h"
#include "../api/us.h"
#include "../api/drive.h"
#include "initValues.h"
#include "dataOutput.h"
#include "calibDrive.h"
#include "../api/printf_stdarg.h"
#include "create_map.h"
#include "crossing_analyzer.h"
#include "../gps/mapMatrix.h"
#include "../gps/GPS_Handler.h"

xTaskHandle xHandleWallFollow;

void SteerToWallAlgorithm(void);

static int8_t wall_align = -1;
uint16_t wall_distance;
static int16_t localDistance;
int wall_steer = 0, wall_steer_old;
static int16_t us_left_data[NUM_VALUES_WALL_FOLLOW] = {100,100,100,100,100,100};
static int16_t us_right_data[NUM_VALUES_WALL_FOLLOW] = {100,100,100,100,100,100};
static portTickType drivenTime;

// Proportional part of control.
static int wall_steerP = 0;
// Differential part of control.
static int wall_steerD = 0;
// Integral part of control.
static int wall_steerI = 0;

int wall_integ = 0;
int usDistanceError = 0;
int driveToMiddle = 0;

// Reason for 2 old usDistanceError:
// Because of the high frequency, the difference between 2 usDistanceErrors
// is quite little --> the difference with the older value is calculated.
// The advantage: The importance of the difference increases without an
// increasing importance of fluctuation
int usDistanceErrorOld1 = 0;
int usDistanceErrorOld2 = 0;

static uint8_t wall_Kp = 8;
static uint8_t wall_Kd = 2;
static uint8_t wall_Ki = 2;
boolean_t newturn = TRUE;
const uint8_t wall_steerPMax = 80;
const uint8_t wall_steerDMax = 50;
const uint8_t wall_steerIMax = 30;

static boolean_t active = FALSE;


void initWallFollow(void) {
	xHandleWallFollow = 0;
}

/**
 * return: True wenn aktiviert
 */
boolean_t wall_Follow_isActivated(){
	return active;
}

void startWallFollow(int8_t align, uint16_t distance) {

	wall_align = align;
	wall_distance = distance;
	drivenTime = os_getTime();

	active = TRUE;
	if (createMap_isActivated() == TRUE){
		localDistance = get_TotalDistance();
		//Überprüfe ob eine Wand in der Nähe ist und wähle entsprechend aus
		if (us_getSensorMapData(RIGHT_SENSOR).Right_Distance <= (RANGESIDE + 30)) wall_align = 1;
		else if (us_getSensorMapData(LEFT_SENSOR).Left_Distance <= (RANGESIDE + 30)) wall_align = 0;
		else{
			wall_align = 2;
		}

		//Kollision auf der Seite vermeiden an der sich nicht orientiert wird
		if ((wall_align == 1 && Us_Data.Left_Distance < wall_distance) || (wall_align == 0 && Us_Data.Right_Distance < wall_distance)){
			wall_distance = (Us_Data.Left_Distance + Us_Data.Right_Distance)/2;
			wall_distance = wall_distance + 5;
		}
	}

#ifdef WALLFOLLOW_DEBUG
	printf("Follow wall on ");
	switch (align) {
		case 0:
			printf("left");
			break;
		case 1:
			printf("right");
			break;
		case 2:
			printf("Search for Wall");
			break;
		default:
			printf("middle");
			break;
	}
#endif

#ifdef WALLFOLLOW_TASK
		if (xHandleWallFollow != 0){
			os_resumeTask(xHandleWallFollow);
		}
		else
			xHandleWallFollow = os_registerProcessStackPriority(SteerToWallThread, "WallFollow", 900, tskIDLE_PRIORITY + 5);
#else
	SteerToWallAlgorithm();
#endif
#ifdef WALLFOLLOW_DEBUG
	printf(", d=%dcm\r\n", distance);
#endif

}

void stopWallFollow() {
	drive(0,18,700);
	wall_align = -1; // no wall follow
	active = FALSE;
	if (xHandleWallFollow && createMap_isActivated() == FALSE)
		os_suspendTask(xHandleWallFollow);
}

void SteerToWallAlgorithm(void){
	int16_t right,left, i;
	uint16_t sen_right, sen_left;
	gps_reducedData_t *own;

	portTickType lastWakeTime;

	lastWakeTime = os_getTime();

	if (createMap_isActivated() == TRUE){
		set_distanceReached(FALSE);
	}

	//Wenn keine Wand in der Nähe, fahre einfach geradeaus
	if (wall_align == 2 && createMap_isActivated() == TRUE){
		drive_setSteeringAngleBalanced(0);
		drive(2,Drive_Servo, 700);
		while (get_distenceReached() == FALSE){
			os_wait(20);
		}
		drive(0,0,700);
	}
	else while (get_distenceReached() == FALSE || createMap_isActivated() == FALSE){

	//Wenn bereits befahrener Bereich näher als nächster Sensorwert auf die Seite (als virtuelle Wand) dann passe Sensorwerte an
	sen_left = Us_Data.Left_Distance;
	sen_right = Us_Data.Right_Distance;


	//regelmäßiges Zwischenspeichern von hindernissen. Das übertragen auf die Karte erfolgt in Store_map()
	if (get_TotalDistance() - localDistance > 1 && createMap_isActivated() == TRUE){
		localDistance = get_TotalDistance();
		newturn = FALSE;
		lastWakeTime = os_getTime();
			if (sen_right < RANGESIDE+30){
				if (Sema != NULL){
					while ( xSemaphoreTake( Sema, ( portTickType ) 10 ) != pdTRUE ){
						os_wait(10);
					}
						own = get_ownCoords();
						set_obstacleRight(CoordinatesToMap(own->x), CoordinatesToMap(own->y),own->angle ,sen_right );
						xSemaphoreGive(Sema);
				}
			}
			if (sen_left < RANGESIDE+30){
				if (Sema != NULL){
					while ( xSemaphoreTake( Sema, ( portTickType ) 10 ) != pdTRUE ){
						os_wait(10);
					}
						own = get_ownCoords();
						set_obstacleLeft(CoordinatesToMap(own->x), CoordinatesToMap(own->y), own->angle, sen_left);
						xSemaphoreGive(Sema);
				}
			}
	}



	if (wall_align != 2){
		if (wall_align != 1) sen_left = us_getSensorMapData(LEFT_SENSOR).Left_Distance; //der rechte Sensor wird nicht gebraucht
		if (wall_align != 0) sen_right = us_getSensorMapData(RIGHT_SENSOR).Right_Distance;
	}

	drive_setSteeringAngleBalanced(balancedSteerAngle);

	left = 0;
	right = 0;

	for (i = NUM_VALUES_WALL_FOLLOW - 1; i > 0; i--){
		us_left_data[i] = us_left_data[i - 1];
		us_right_data[i] = us_right_data[i - 1];
		left += us_left_data[i];
		right += us_right_data[i];
	}
	us_left_data[0] = (sen_left > 200) ? 200 : sen_left;
	us_right_data[0] = (sen_right > 200) ? 200 : sen_right;


	left += us_left_data[0];
	right += us_right_data[0];

	os_wait(14);
	// Ist die Wand nicht mehr in Reichweite?
	if (wall_align == 0){ // LEFT
		if (left/NUM_VALUES_WALL_FOLLOW > 100 && createMap_isActivated() == TRUE){
			wall_align = 2;
		}
		usDistanceError = (int16_t) sen_left - (int16_t) wall_distance;
	}
	else if (wall_align == 1){ // RIGHT
		if (right/NUM_VALUES_WALL_FOLLOW > 100 && createMap_isActivated() == TRUE){
			wall_align = 2;
		}
		usDistanceError = (int16_t) sen_right - (int16_t) wall_distance;
	}
	else if (wall_align == -1 || wall_align == 2) // Wall Follow is not running
		break;
	else // CENTER
		usDistanceError = ((int16_t) sen_right - (int16_t) sen_left) / 2 - (int16_t) wall_distance;

	// calculate the steering angle

	// derivative term of steering
	wall_steerD = usDistanceError - usDistanceErrorOld2;

	// proportional term of steering with control variable limitation
	wall_steerP = usDistanceError * wall_Kp;

	// limit influence of steerP
	if (wall_steerP > wall_steerPMax)
		wall_steerP = wall_steerPMax;
	if (wall_steerP < -wall_steerPMax)
		wall_steerP = -wall_steerPMax;

	if (wall_align == 0) // opposite direction?
		wall_steerP *= -1;

	wall_steerD = wall_steerD * wall_Kd;
	// limit influence of steerD
	if (wall_steerD > wall_steerDMax)
		wall_steerD = wall_steerDMax;
	if (wall_steerD < -wall_steerDMax)
		wall_steerD = -wall_steerDMax;

	if (wall_align == 0) // opposite direction? //LEFT
		wall_steerD *= -1;

	// integral term of steering and decreasing importance of old values
	wall_integ = wall_integ * 0.9 + wall_steer_old;
	wall_steerI = -wall_integ / wall_Ki;
	// limit influence of steerI
	if (wall_steerI > wall_steerIMax)
		wall_steerI = wall_steerIMax;
	if (wall_steerI < -wall_steerIMax)
		wall_steerI = -wall_steerIMax;

	// add all terms
	wall_steer = wall_steerP + wall_steerD + wall_steerI;

	// limit the maximum angle
	if (wall_steer < -50)
		wall_steer = -50;
	if (wall_steer > 50)
		wall_steer = 50;

	if (wall_align != -1 && wall_align != 2 && createMap_isActivated() == TRUE){
		drive_setSteeringAngleBalanced(	(int8_t) wall_steer);
		accumulateSteeringAngle(wall_steer);
		drive(2,Drive_Servo,700);
	}else if (wall_align == 2){
		drive_setSteeringAngleBalanced(0);
		drive(2,Drive_Servo,700);
	}

	// set values for next iteration
	usDistanceErrorOld1 = usDistanceError;
	usDistanceErrorOld2 = usDistanceErrorOld1;
	wall_steer_old = wall_steer;
	}

	if (createMap_isActivated() == TRUE){
		stopCrossingAnalyzer();
		drive(0,0,500);
		wall_align = -1; // no wall follow
		active = FALSE;
	}
}

void SteerToWallThread(void) {
	portTickType lastWakeTime;

	lastWakeTime = os_getTime();
	for (;;) {
		os_frequency(&lastWakeTime, 55);

		SteerToWallAlgorithm();
	}
}

/**
 * @return wall_align
 */
int8_t getWallAlign() {
	return wall_align;
}

