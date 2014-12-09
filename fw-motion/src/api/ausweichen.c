/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file ausweichen.c
 * @author Nixi	
 * @brief TODO - insert a brief description of that module
 * @Version 1.0
 **/

#include "ausweichen.h"
#include "api.h"
#include "regler.h"

int8_t Drive_Speed = 0;
int8_t Counter = 0;
int8_t Distance = 0;


void ausweichmanoever(void){

	Counter = 0;
	Distance = 0;
	Drive_Speed = Drive_GetMotor();


	while( us_getFrontDistance() < (500 + Drive_Speed * 200)){

		//PD Regler Ausschalten!!!
		Drive_SetServo((1/Drive_Speed)* 2);
		Counter++;

	}

	Drive_SetServo(0);

	if(us_getRightDistance()  < 100){
		Distance = us_getRightDistance();
		Drive_SetServo(-Counter*(1/Drive_Speed)* 2);

	}

	if(us_getRightDistance() > us_getRightDistance()+20){

		setAusweichen();

	}



}

