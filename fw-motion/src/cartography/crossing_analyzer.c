/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file crossing_analyzer.c
 * @author dnotz
 * @brief CrossingAnalyzer is able to detect crossings and determine which type of crossing it is.
 *        For the detection of edges the Canny algorithm is used. When an edge has been detected,
 *        the current OS time is stored into a variable so that the width of crossings can be
 *        calculated and handed to the MappingController.
 **/


#include "crossing_analyzer.h"
#include "../api/us.h"
#include "initValues.h"
#include "dataOutput.h"
#include "calibDrive.h"
#include "wall_follow.h"
#include "create_map.h"
#include "stack.h"
#include "../api/wireless/wireless.h"
#include "../gps/help_methods.h"
#include "../gps/GPS_Handler.h"

// arrays for us sensor values
static uint16_t us_left_data[NUM_VALUES_CROSSING_ANALYZER] = {60,60,60,60,60,60};
static uint16_t us_right_data[NUM_VALUES_CROSSING_ANALYZER] = {60,60,60,60,60,60};
static uint16_t us_front_data[NUM_VALUES_CROSSING_ANALYZER/2] = {40,40,40};

// arrays needed for convolution
uint16_t data1[NUM_VALUES_CROSSING_ANALYZER], data2[NUM_VALUES_CROSSING_ANALYZER];
static boolean_t newSpaceLeft = TRUE, newSpaceRight = TRUE, onceleft = FALSE, onceright = FALSE;
boolean_t init = FALSE, init_init = FALSE;

portTickType t10, t11, t30, t31, t2;
int16_t width_right, width_left, width_rear, width_front, t311, t111;
int16_t x_startL, y_startL, x_startR, y_startR;

uint8_t initializedValues;
volatile uint8_t crossingDetected, crossingStarted;

const int8_t GAUSS_OP[OP_LENGTH] = { 2, 4, 5, 4, 2 };
const int8_t SOBEL1_OP[OP_LENGTH] = { 0, 1, 2, 1, 0 };
const int8_t SOBEL2_OP[OP_LENGTH] = { 0, -1, 0, 1, 0 };

xTaskHandle xHandleCrossingAnalyzer;

// needed for minimum time between two edges
portTickType time_edge = 0;
portTickType time_edge_old = 0;


void initCrossingAnalyzer(void) {
	gps_reducedData_t *own;

	if (init_init == FALSE){
		xHandleCrossingAnalyzer = 0;

		own = get_ownCoords();

		if (Us_Data.Left_Distance <= 70){
			newSpaceLeft = FALSE;
		}
		else newSpaceLeft = TRUE;
		x_startL = own->x;
		y_startL = own->y;

		if (Us_Data.Right_Distance <= 70){
			newSpaceRight = FALSE;
		}
		else newSpaceRight = TRUE;
		x_startR = own->x;
		y_startR = own->y;

		init_init = TRUE;
	}
	t111 = get_TotalDistance();
	t311 = get_TotalDistance();
}

void startCrossingAnalyzer(void) {
	// init time
	if (init == FALSE){
		init = TRUE;
		t10 = 0;
		t11 = 0;
		t30 = 0;
		t31 = 0;
		t2 = 0;

		// init width
		width_right = 0;
		width_left = 0;
		width_rear = 0;
		width_front = 0;

		// init status
		crossingStarted = 0;
		crossingDetected = 0;
		initializedValues = 0;

		time_edge_old = os_getTime();
		time_edge = time_edge_old;
	}

#ifdef CROSSINGANALYZER_TASK
	if (xHandleCrossingAnalyzer != 0)
		os_resumeTask(xHandleCrossingAnalyzer);
	else
		xHandleCrossingAnalyzer = os_registerProcessStackPriority(CrossingAnalyzerThread,
				"CrossingAnalyzer", 930, tskIDLE_PRIORITY + 4);

	if (createMap_isActivated() == FALSE){
		drive(2,13,200);
		Drive_SetMotor(2);
	}
#endif
}

void stopCrossingAnalyzer(void) {

	if (createMap_isActivated() == FALSE){
		drive(0,13,200);
		Drive_SetMotor(0);
	}
	init = FALSE;
	if (xHandleCrossingAnalyzer)
		os_suspendTask(xHandleCrossingAnalyzer);
}

//Kantendetektion
void convolution(uint16_t *data, const int8_t *op, uint16_t *result) {
	uint8_t i;
	int8_t j, k;
	int16_t sum;

	// iterate through the array and calculate the convolution
	for (i = 0; i < NUM_VALUES_CROSSING_ANALYZER; i++) {
		sum = 0;
		for (j = 0; j < OP_LENGTH; j++) {
			// data index
			k = i + j - 2;

			if (k < 0)
				k = 0;
			if (k >= NUM_VALUES_CROSSING_ANALYZER)
				k = NUM_VALUES_CROSSING_ANALYZER - 1;

			sum += data[k] * op[j];
		}
		result[i] = sum;
	}
}

void printArray(int16_t *d) {
	uint8_t i;

	// iterate through the array and print the values
	for (i = 0; i < NUM_VALUES_CROSSING_ANALYZER; i++)
		printf("%d ", d[i]);
	printf("\r\n");
}

/**
 * return 1 when obstacle in front
 */
uint8_t checkfront(){
	uint8_t ret = 0;
	uint8_t i;
	uint16_t frontDistance;

	if (Us_Data.Front_DataValid){
	for (i = NUM_VALUES_CROSSING_ANALYZER/2 - 1; i > 0; i--)
		us_front_data[i] = us_front_data[i - 1];
	us_front_data[0] =
			(Us_Data.Front_Distance > 200) ? 200 : Us_Data.Front_Distance;

	frontDistance = 0;
	// calculate the average distance
	for (i = 0; i < NUM_VALUES_CROSSING_ANALYZER/2 - 1; i++)
		frontDistance += us_front_data[i];
	frontDistance /= NUM_VALUES_CROSSING_ANALYZER/2;

	// front obstacle detected ?
	if (frontDistance < FRONT_DISTANCE_OBSTACLE && t2 == 0) {
		crossingStarted = 1;
		t2 = os_getTime();
#ifdef CROSSING_DEBUG
		printf("## FRONT OBSTACLE DETECTED\r\n");
#endif
		ret = 1; //return Wert setzen das Hindernis gefunden
		// calculate the width of the left and the right corridor
		if (t10 != 0)
			width_left = calculateDrivenLen(t2 - t10) + FRONT_DISTANCE_OBSTACLE;
		if (t30 != 0)
			width_right = calculateDrivenLen(t2 - t30) + FRONT_DISTANCE_OBSTACLE;

		// get the width of the front corridor
		if (width_front == 0) {
			width_front = us_left_data[NUM_VALUES_CROSSING_ANALYZER -1] + us_right_data[NUM_VALUES_CROSSING_ANALYZER - 1] + WIDTH_CAR;
			width_rear = 0;
		}

		// reset values
		handleEndOfCrossing();
	}
	}
	return ret;
}

void CrossingAnalyzerThread(void) {
	portTickType lastWakeTime;
	uint8_t i, j = 0, start = 0;
//	uint16_t right,left;
	uint16_t sen_right, sen_left;
	int8_t minMax; // shows whether the convolution has found a minimum or a maximum
	gps_reducedData_t *own;
	lastWakeTime = os_getTime();

	for (;;) {
//		calls[5]++;
		os_frequency(&lastWakeTime, 200);
		j++;
//		left = 0;
//		right = 0;

		sen_left = Us_Data.Left_Distance;
		sen_right = Us_Data.Right_Distance;
//		Seg_Hex(0x05);
		// get left / right us sensor data alternately
		// add one new value and shift the old values
		if (j == 1) { // left
			if (createMap_isActivated() == TRUE) sen_left = us_getSensorMapData(LEFT_SENSOR).Left_Distance;
			for (i = NUM_VALUES_CROSSING_ANALYZER - 1; i > 0; i--){
				us_left_data[i] = us_left_data[i - 1];
//				left += us_left_data[i];
			}
			us_left_data[0] = (sen_left > 200) ? 200 : sen_left; // limit to 200 //vm limit to 50
//			left += us_left_data[0];
		} else { // right
			if (createMap_isActivated() == TRUE) sen_right = us_getSensorMapData(RIGHT_SENSOR).Right_Distance;
			for (i = NUM_VALUES_CROSSING_ANALYZER - 1; i > 0; i--){
				us_right_data[i] = us_right_data[i - 1];
//				right += us_right_data[i]; //Todo: ist der aufruf sinnvoll?
			}
			us_right_data[0] = (sen_right > 200) ? 200 : sen_right; // limit to 200
//			right += us_right_data[0];
			j = 0;
		}
		// only check data for events if data is initialized!
		if (initializedValues < NUM_VALUES_CROSSING_ANALYZER + 2) {
			// wait for data to be ready
			initializedValues++;
			continue;
		}
//		Seg_Hex(0x07);
#ifdef NAVIGATION_SCENARIO
		// ---------------------
		// check FRONT
		checkfront();
//		Seg_Hex(0x06);
		// ---------------------
		// check LEFT / RIGHT side
		// Canny algorithm, see http://perso.limsi.fr/Individu/vezien/PAPIERS_ACS/canny1986.pdf

		if (j == 1)
			convolution(us_left_data, GAUSS_OP, data1);
		else
			convolution(us_right_data, GAUSS_OP, data1);

		convolution(data1, SOBEL1_OP, data2);
		convolution(data2, SOBEL2_OP, data1);

	// edge start or edge end detected ?
		minMax = 0;
		if (data1[3] + data1[4] < -2000){
//				storeElement(50);
			if (data1[2] > data1[3] && data1[5] > data1[4]){
				minMax = -1; // Minimum
			}
		}

		if (data1[3] + data1[4] > 2000)
			if (data1[2] < data1[3] && data1[5] < data1[4])
				minMax = 1; // Maximum
//		else{
//			if (newSpaceLeft == TRUE /*&& left/NUM_VALUES < 60*/ && sen_left <= 70){
//				newSpaceLeft = FALSE;
//			}
//			if (newSpaceRight == TRUE && /* right/NUM_VALUES < 60 */ sen_right <= 70){
//				newSpaceRight = FALSE;
//			}
//		}
//		minMax = 0;
//
//		if (sen_right > 70 && newSpaceRight == FALSE && j == 0/* && t31 == 0 */){
//			minMax = -1;
//			newSpaceRight = TRUE;
//		}
//		if (sen_left > 70 && newSpaceLeft == FALSE && j == 1 /* && t11 == 0 */){
//			minMax = -1;
//			newSpaceLeft = TRUE;
//		}
//		if ((sen_right < 70 && j == 0) || (sen_left < 70 && j == 1)){
//			minMax = 1;
////			Seg_Hex(0x07);
//		}

		// ---------------------
		// START OF EDGE
//		if (minMax == -1){
//			storeElement(4);
//			Seg_Hex(0x04);
//		}
//		if (minMax == 1){
//			storeElement(5);
//			Seg_Hex(0x05);
//		}
//		if (os_getTime() - time_edge > MIN_PAUSE) Seg_Hex(0x07);
		if (minMax == -1 && os_getTime() - time_edge > MIN_PAUSE &&
				((j == 1 && t10 == 0) || (j==0 && t30 == 0))) {
			crossingStarted = 1;
#ifdef CROSSING_DEBUG
			printf("EDGE START");
#endif
			time_edge_old = time_edge;
			time_edge = os_getTime();
			stopWallFollow();

			// Set width of joining aisle of crossing, w0
			if (width_front == 0)
				width_front = us_left_data[NUM_VALUES_CROSSING_ANALYZER -1] + us_right_data[NUM_VALUES_CROSSING_ANALYZER -1];

			own = get_ownCoords();
			if (j == 1) { // left edge has been detected
#ifdef CROSSING_DEBUG
				printf(" on LEFT\r\n");
#endif
				if (t10 == 0)
					t10 = os_getTime();
			}

			else { // right edge has been detected
#ifdef CROSSING_DEBUG
				printf(" on RIGHT\r\n");
#endif
				if (t30 == 0)
					t30 = os_getTime();
//				Stack_Push(get_OwnX(),get_OwnY(),RIGHT_SENSOR);
			}
			// update the alignment of the car, e.g. follow right wall now!
			updateAlignment();
		}
#else

		//Speichern von möglichen Abzweigungen
		if (createMap_isActivated() == TRUE){
			if (j == 1){
				if (newSpaceLeft == FALSE && sen_left > RANGESIDE+30){
					own = get_ownCoords();
					if (onceleft == FALSE){
						x_startL = own->x;
						y_startL = own->y;
						onceleft = TRUE;
					}
					if (get_TotalDistance() - t111 >= 50){
						//Speichern einer freien Fläche
						newSpaceLeft = TRUE;
						t111 = get_TotalDistance();
						onceleft = FALSE;
						Stack_Push(x_startL + (int16_t)(giveDifferenceAbsolute(x_startL , own->x)/2), y_startL + (int16_t)(giveDifferenceAbsolute(y_startL , own->y)/2),AngleMinus(own->angle, 16384));
						x_startL = own->x;
						y_startL = own->y;
					}
				} else {
					if (sen_left < 80) onceleft = FALSE;
					if (newSpaceLeft == TRUE && sen_left <= RANGESIDE-10){
						if (get_TotalDistance() - t111 >= 50){
							Seg_Hex(0x05);
							Stack_Push(x_startL + (int16_t)(giveDifferenceAbsolute(x_startL , own->x)/2),y_startL + (int16_t)(giveDifferenceAbsolute(y_startL , own->y)/2),AngleMinus(own->angle, 16384));
						}
						newSpaceLeft = FALSE;
						t111 = get_TotalDistance();
						x_startL = own->x;
						y_startL = own->y;
					}
				}
			}
			else {
				if (newSpaceRight == FALSE && sen_right > RANGESIDE+30){
					own = get_ownCoords();
					if (onceright == FALSE){
						x_startR = own->x;
						y_startR = own->y;
						onceright = TRUE;
					}
					if (get_TotalDistance() - t311 >= 50){
						newSpaceRight = TRUE;
						t311 = get_TotalDistance();
						onceright = FALSE;
						Stack_Push(x_startR + (int16_t)(giveDifferenceAbsolute(x_startR, own->x)/2), y_startR + (int16_t)(giveDifferenceAbsolute(y_startR,own->y)/2),AngleMinus(own->angle, 16384));
						x_startR = own->x;
						y_startR = own->y;
					}
				} else {
					if (sen_right < 80) onceright = FALSE;
					if (newSpaceRight == TRUE && sen_right <= RANGESIDE-10){
						if (get_TotalDistance() - t311 >= 50){
							Stack_Push(x_startR + (int16_t)(giveDifferenceAbsolute(x_startR, own->x)/2),y_startR + (int16_t)(giveDifferenceAbsolute(y_startR,own->y)/2),AngleMinus(own->angle, 16384));
						}
						newSpaceRight = FALSE;
						t311 = get_TotalDistance();
						x_startR = own->x;
						y_startR = own->y;
					}
				}
			}
		}
#endif

#ifdef NAVIGATION_SCENARIO
		// ---------------------
		// END OF EDGE
		if (minMax == 1 && os_getTime() - time_edge > MIN_PAUSE) {
#ifdef CROSSING_DEBUG
			printf("EDGE END");
			if (j == 1)
				printf(" on LEFT\r\n");
			else
				printf(" on RIGHT\r\n");
#endif

			stopWallFollow();

			if (j == 1) { // left edge
				if (t10 != 0) { // start edge has already been detected
					t11 = os_getTime();
					width_left = calculateDrivenLen(t11 - t10);
#ifdef CROSSING_DEBUG
					printf("WIDTH OF LEFT CROSSING: %d\r\n", width_left);
#endif
					t10 = 0; // reset values
					t11 = 0;

				}
#ifdef CROSSING_DEBUG
				else // no start edge had been detected, fault!
					printf("ERROR -- No start edge detected yet\r\n");
#endif
			}

			else { // right edge
				if (t30 != 0) { // start edge has already been detected
					t31 = os_getTime();
					width_right = calculateDrivenLen(t31 - t30);
#ifdef CROSSING_DEBUG
					printf("WIDTH OF RIGHT CROSSING: %d\r\n", width_right);
#endif
					t30 = 0;
					t31 = 0;
#ifdef CROSSING_DEBUG
				} else // no start edge had been detected, fault!
					printf("ERROR -- No start edge detected yet\r\n");
#endif
			}

			time_edge_old = time_edge;
			time_edge = os_getTime();

			// Set width of joining aisle of crossing, w2
			width_rear = us_left_data[0] + us_right_data[0] + WIDTH_CAR;
			handleEndOfCrossing(); //vm = von mir auskommentiert

		}
//		os_wait(freq);
#endif
	}
}


void handleEndOfCrossing(void) {
	int16_t backLength = 0;

	if (t10 == 0 && t11 == 0 && t30 == 0 && t31 == 0) { // end of the whole crossing has been detected
#ifdef CROSSING_DEBUG
		printf("Crossing dimensions:\r\n");
		printf("w0=%dcm, w2=%dcm\r\n", width_front, width_rear);
		printf("w1=%dcm, w3=%dcm\r\n", width_left, width_right);
#endif

		crossingDetected = 1;
		initializedValues = 0;

		stopCrossingAnalyzer();
	}
}


void updateAlignment(void) {
	if (t10 != 0 && t30 != 0) { // edges on both sides, drive straight ahead
#ifdef CROSSING_DEBUG
		printf("driving straight\r\n");
#endif
		drive_setSteeringAngleBalanced(0);
		drive(2,13,100);
		Drive_SetMotor(2);
	}
	else if (t10 != 0 && t30 == 0) // edge on the left side, follow right wall
		startWallFollow(1, width_front / 2);
	else if (t30 != 0 && t10 == 0) // edge on the right side, follow left wall
		startWallFollow(0, width_front / 2);
}


uint8_t getCrossingDetected() {
	return crossingDetected;
}

uint8_t getCrossingStarted() {
	return crossingStarted;
}

int16_t getWidthRight() {
	return width_right;
}

int16_t getWidthLeft() {
	return width_left;
}

int16_t getWidthFront() {
	return width_front;
}

int16_t getWidthRear() {
	return width_rear;
}

portTickType getTimeEdge() {
	return time_edge;
}

portTickType getTimeEdgeOld() {
	return time_edge_old;
}

void resetCrossingStarted() {
	crossingStarted = 0;
}
