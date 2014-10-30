/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file state_eagle_eye.c
 * @author TEAM::GPS
 * @brief implementation of scenario eagleEye:
 * 			the car receives its own position and target position in world system format
 * 			eagleEye calculates the needed path to the target and adjusts the steering values
 **/
#include "state_eagle_eye.h"
#include "help_methods.h"
#include "GPS_Handler.h"
#include "followLowest.h"
#include "../api/seg.h"
#include "../api/printf_stdarg.h"


static uint8_t turnDone=0;

/**
 * Method to start scenario eagleEye (called by task)
 */
void eagleEye(gps_reducedData_t* ownCoords, gps_reducedData_t* aimCoords) {
	gps_reducedData_t differenceVector;

#ifdef DEBUG
	printf("Scenario EagleEye:\n\r");
#endif
	//Vor Navigieren in richtige Richtung schon mal ausrichten
	if(turnDone != targetID){
		look_in_direction( aimCoords->x, aimCoords->y,ownCoords->x, ownCoords->y,ownCoords->angle);
		turnDone = targetID;
	}

	calculateDifferenceVector(&differenceVector, ownCoords, aimCoords);

	//Berechnen, ob Auto schon in der Nähe des Ziels
	if(giveDifferenceAbsolute(aimCoords->x, ownCoords->x) > 380 || giveDifferenceAbsolute(aimCoords->y, ownCoords->y) > 380){
//		Seg_Hex(0x04);
		navigate(&differenceVector);

	}
	else{
		Seg_Hex(0x06);
		Drive_SetMotor(0);
		if(taskSet==CONTROL_FOLLOW_LOWEST_ID)
			increase_lowest_ID();
	}
#ifdef DEBUG
	printf("Scenario EagleEye - calculations done\n\r");
#endif
}
