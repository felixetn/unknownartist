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


	while(( us_getFrontDistance() <= (500 + Drive_Speed * 200)) & ( us_getFrontDistance() >= 200)){


		if(us_getFrontDistance() <= 200) //Notstop, sollte das Auto zu nah an einer Wand/Gegenstand sein
					{
						Drive_SetMotor(0);
						os_frequency(&lastWakeTime, 100); // Kurze Zeit warten
						Drive_SetServo(-50);
						Drive_SetMotorForDistance(-1 , 1000); //Einlenken Rückwärts fahren
						Drive_SetServo(50);
						Drive_SetMotorForDistance(1 , 1000); //Zurücklenken Vorwärts fahren
					}
		if(us_getFrontDistance() > 200) // Abstand noch groß genug für regulären Ausweichvorgang
					{
					Drive_SetServo((1/Drive_Speed)* 2);
					Counter++; //Counter um zu wissen wie oft man Lenkung verstellt hat zum zurücklenken später
					}
	}

	Drive_SetServo(0);

	if(us_getRightDistance()  < 100){
		Distance = us_getRightDistance();
		Drive_SetServo(-Counter*(1/Drive_Speed)* 2);
	}
	//PD Regler wieder einschalten
}

