/*
 * ShellTestTask.c
 *
 *  Created on: 01.07.2010
 * @Author: Frank Englert
 * @Reviewed by:
 * @Tested by:
 */

#include "GPSmessagetypes.h"
#include "GPS_Handler.h"
#include "navigation.h"
#include "state_eagle_eye.h"
#include "StartScenario.h"
#include "FollowLowest.h"
#include "mapMatrix.h"
#include "../cartography/create_map.h"
#include "../api/wireless/wireless.h"
#include "../api/wireless/bluetooth_package_types.h"
#include "../api/printf_stdarg.h"

#define OWN_COORD_SET_FLAG 1
#define TARGET_COORD_SET_FLAG 2

static gps_reducedData_t ownCoords, targetCoords;
//Zeigt an, ob die ID des Autos und des Ziels schon validiert wurden
uint8_t coordsValidFlags;

static uint8_t taskLowCounter = 0;

//Notwendig für Initialisierungsphase des Szenarios Mehrwegenavigation
uint8_t taskSet = 0;

#if ANALYZE_MEMORY == 1
extern long maxHeapUsage;
#endif

static void decodeGpsControlMessages(wireless_interface_t wiif, wirelessMessage_t* msg) {
	gps_controlMessage_t* controlMessage = (gps_controlMessage_t*) msg->data;

	switch (controlMessage->ctrlMsgType) {
		case CONTROL_YOUR_ID:
			//task: set this as the own car symbol ID
			ownID = controlMessage->data;
			coordsValidFlags = coordsValidFlags & ~OWN_COORD_SET_FLAG; //reset target flag

#ifdef DEBUG
			printf("CONTROL: your ID %d\n\r", ownID);
#endif
			break;

		case CONTROL_FOLLOW_THIS_ID:
			//task: set this as the target symbol ID
			targetID = controlMessage->data;
			coordsValidFlags = coordsValidFlags & ~TARGET_COORD_SET_FLAG; //reset target flag
#ifdef DEBUG
			printf("CONTROL: target ID %d\n\r", targetID);
#endif
			break;

		case CONTROL_START:
#ifdef DEBUG
			printf("CONTROL: START\n\r");
#endif
			//task: start doing the job if own ID is valid
			if (ownID != UNDEFINED_ID) {
				targetID = controlMessage->data; //set target ID delivered with the start command
				gpsState = GPS_STATE_NAVIGATING;
				coordsValidFlags = 0; //reset both flags
				taskSet = CONTROL_START;
			}
			break;

		case CONTROL_STOP:
#ifdef DEBUG
			printf("CONTROL: STOP\n\r");
#endif
#if ANALYZE_MEMORY == 1
			RuntimeStats_Print();
#endif

			//task: stop doing the job -> go to IDLE
			gpsState = GPS_STATE_IDLE;
			//TODO: Crashes?!
//			stop_create_map();
			break;

		case CONTROL_FOLLOW_LOWEST_ID:
#ifdef DEBUG
			printf("CONTROL: FOLLOW LOWEST\n\r");
#endif
			//task: start following the lowest ID after target ist set
			targetID = 254;
			taskSet = CONTROL_FOLLOW_LOWEST_ID;
			gpsState = GPS_STATE_NAVIGATING;
			coordsValidFlags = coordsValidFlags & ~TARGET_COORD_SET_FLAG; //reset target flag
			break;

		case CONTROL_FOLLOW_OFFROAD:
			if (ownID != UNDEFINED_ID) {
				targetID = controlMessage->data; //set target ID delivered with the start command
				gpsState = GPS_STATE_NAVIGATING;
				coordsValidFlags = 0; //reset both flags
				taskSet = CONTROL_FOLLOW_OFFROAD;
			}
			break;

		case CONTROL_FOLLOW_OFFROAD_DYN:
			//task: start obstacles for offroad mode
			targetID = 254;
			taskSet = CONTROL_FOLLOW_OFFROAD_DYN;
			gpsState = GPS_STATE_NAVIGATING;
			coordsValidFlags = coordsValidFlags & ~TARGET_COORD_SET_FLAG; //reset target flag
			break;
		case CONTROL_CARTOGRAPHY:
#ifdef DEBUG
			printf("CONTROL: CARTOGRAPHY\n\r");
#endif
			//task: start obstacles for offroad mode
			if (ownID != UNDEFINED_ID) {
				targetID = controlMessage->data;
				taskSet = CONTROL_CARTOGRAPHY;
				gpsState = GPS_STATE_NAVIGATING;
				coordsValidFlags = coordsValidFlags & ~TARGET_COORD_SET_FLAG; //reset target flag
			}
			break;
		case CONTROL_SLAM:
			StartScenario(&ownCoords, &targetCoords, CONTROL_SLAM);
			break;
	}
}

gps_reducedData_t* get_ownCoords() {
	return &ownCoords;
}

void set_ownCoords(int16_t x, int16_t y, uint16_t angle) {
	ownCoords.x = x;
	ownCoords.y = y;
	ownCoords.angle = angle;
}

//Task fetches TUIO messages from GpsQueue which is equal to the GPSreducedDataQueue and extracts useful information
//it sets and checks the flags whether to call the scenario eagle eye or not
void handleGpsCoordMessages(wireless_interface_t wiif, wirelessMessage_t* msg) {
	gps_reducedData_t* message = (gps_reducedData_t*) msg->data;

	//if received message contains own coordinates or coordinates of target
	if ((taskSet == CONTROL_FOLLOW_LOWEST_ID || taskSet == CONTROL_FOLLOW_OFFROAD_DYN) && taskLowCounter < 20) { //wenn dieses szenario und noch am anfang
		init_lowest_ID(msg);
		taskLowCounter = taskLowCounter + 1; //maximal 20 mal dann durchstarten mit ziel verfolgung
	} else if (taskSet == CONTROL_FOLLOW_OFFROAD_DYN && taskLowCounter < 40) {
		set_Obstacle(msg);
		taskLowCounter = taskLowCounter + 1;
	} else {
		if (message->tag_id == ownID) {
			//set own coordinates
			if (get_CoordinatesFlag() == TRUE
				) set_CoordinatesFlag(FALSE);
			ownCoords = *message;
			coordsValidFlags = coordsValidFlags | OWN_COORD_SET_FLAG; //set flag
		} else if (message->tag_id == targetID) {
			//set target coordinates
			targetCoords = *message;
			;
			coordsValidFlags = coordsValidFlags | TARGET_COORD_SET_FLAG; //set flag
		}
		//start navigation only if both flags are set

		//Ausgewähltes Szenario soll gestartet werden, wenn alle Flags gesetzt sind
		if (coordsValidFlags == (OWN_COORD_SET_FLAG | TARGET_COORD_SET_FLAG) && gpsState == GPS_STATE_NAVIGATING) {
			coordsValidFlags = coordsValidFlags & ~OWN_COORD_SET_FLAG; //reset own flag
			StartScenario(&ownCoords, &targetCoords, taskSet);
		}
	}
}

void Init_GPS_Handler(void) {
	ownCoords.x = 0;
	ownCoords.y = 0;
	ownCoords.angle = 0;
	wirelessSubscribe(decodeGpsControlMessages, PT_GPS_CONTROL);
	wirelessSubscribe(handleGpsCoordMessages, PT_GPS_UPDATE_COORD);
}
