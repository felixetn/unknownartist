/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file help_methods.c
 * @author TEAM::GPS
 * @brief contains method calculateDifferenceVector which calculates the position of the target from the view of the car
 * 		this one is needed for the navigation
 *
 **/
#include "help_methods.h"
#include "../api/util.h"
#include "../api/printf_stdarg.h"

#define TWOPOW16 	65535
#define TWOPOW15 	(int32_t)32767


/*
 * calculates the position of the target from the view of the car by
 * 		- subtracting position of the car from target position -> x_dif, y_dif
 * 		- multiplying vector (x_dif, y_dif) with rotation matrix with negative angle of the car -> x_res, y_res
 * 		this leads to the coordinate of the target from view of the car, now the angle is needed
 * 		- angle approximation from -45° to 45° linearly to -RA to RA (RA=8192) by quotient of x_res/y_res
 * 			RA is used as stretching factor to represent values from -1 to 1 more accurately
 */
void calculateDifferenceVector(gps_reducedData_t* dif, gps_reducedData_t* carPos,
		gps_reducedData_t* targetPos) {
	int32_t cos_angle, sin_angle;
	int32_t x_dif, y_dif;
	int32_t x_res, y_res;
	int32_t quotient, quotientAbs;
	int32_t angle;

	//reachable area angle
	int32_t RA=8192;


	cos_angle=((int32_t)cos_(TWOPOW16-carPos->angle))-TWOPOW15;
	sin_angle=((int32_t)sin_(TWOPOW16-carPos->angle))-TWOPOW15;


	//distance to aim from view of the coordinate system
	x_dif=(int32_t)targetPos->x - (int32_t)carPos->x;
	y_dif=(int32_t)targetPos->y - (int32_t)carPos->y;

#ifdef DEBUG
	printf("\tdif_x = %d", x_dif);
	printf("\ty_dif = %d", y_dif);
	printf("\tangle = %u\n\r", carPos->angle);
#endif

	//distance to aim from view of the car
	x_res = (x_dif*cos_angle - y_dif*sin_angle)/TWOPOW15;
	y_res = (x_dif*sin_angle + y_dif*cos_angle)/TWOPOW15;

	//prevent zero division
	if(y_res==0){
		y_res=1;
	}

	/*calculation of angle by approximation depending on quotient x/y */

	//calculate quotient and quotientAbs
	quotient = (RA*x_res)/y_res;
	quotientAbs = abs32(quotient);

	//approximate angle to 45 degree linear, rest is 90 degree
	if(y_res>0 && quotientAbs<RA){
		angle = TWOPOW15+quotient;
	}
	else{
		if (x_res>0){
			angle =  3*TWOPOW15/2;
		}
		else{
			angle = TWOPOW15/2;
		}

	}

	//set resulting coordinates and angle
	setCoordinates(dif, targetPos->cam_id, targetPos->tag_id, (int16_t)x_res, (int16_t)y_res, (uint16_t)angle,
				targetPos->isWorld);
#ifdef DEBUG
	printf("\tresult dif: x %d\ty %d\tangle %u\n\r", dif->x, dif->y, dif->angle);
#endif
}


//initiates and returns struct coordinates
void setCoordinates(gps_reducedData_t* dest, uint8_t cam_id, uint8_t tag_id,
		int16_t x, int16_t y, uint16_t angle, uint8_t isWorld) {

	dest->cam_id = cam_id;
	dest->tag_id = tag_id;
	dest->x = x;
	dest->y = y;
	dest->angle = angle;
	dest->isWorld = isWorld;
}

/**
 * Können wir das löschen?
 */
void convertToCharArray(uint32_t test, char pkg[4]){
	uint8_t * bla = (uint8_t *) &test;

	pkg[3]= (char) *bla;
	pkg[2]= (char) *(bla+1);
	pkg[1]= (char) *(bla+2);
	pkg[0]= (char) *(bla+3);

}

/**
 * Konvertiert eine 32 Bit Integer Zahl in ein Charr array und gibt dieses zurück
 */
void convertToCharArraySurround(char s, uint32_t test, char pkg[8]){
	uint8_t * bla = (uint8_t *) &test;

	pkg[7]= s;
	pkg[6]= s-32;
	pkg[5]= (char) *bla;
	pkg[4]= (char) *(bla+1);
	pkg[3]= (char) *(bla+2);
	pkg[2]= (char) *(bla+3);
	pkg[1]= s-32;
	pkg[0]= s;

}

/**
 * Konvertiert zwei 16 Bit Integer Zahlen in ein Char array und gibt dieses zurück
 */
void convertToCharArraySurroundSmall(char s, uint16_t test1, uint16_t test2, char pkg[8]){
	uint8_t * bla1 = (uint8_t *) &test1;
	uint8_t * bla2 = (uint8_t *) &test2;

	pkg[7]= s;
	pkg[6]= s-32;
	pkg[5]= (char) *bla2;
	pkg[4]= (char) *(bla2+1);
	pkg[3]= (char) *bla1;
	pkg[2]= (char) *(bla1+1);
	pkg[1]= s-32;
	pkg[0]= s;

}

/**
 * Berechnet den Betrag der Differenz aus zwei 16 Bit Zahlen
 * Input: 16 Bit integer, 16 Bit integer
 */
uint16_t giveDifferenceAbsolute(int16_t zahl1, int16_t zahl2){
	if(zahl1 > zahl2){
		return zahl1-zahl2;
	}
	else
		return zahl2-zahl1;
}
