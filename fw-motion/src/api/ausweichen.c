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
#include "base_type.h"
#include "drive.h"
#include "transitionen.h"

int8_t Drive_Speed1 = 0;
int8_t Counter = 0;
int8_t Distance = 0;
int8_t init = 0;
uint32_t aStart;

void ausweichmanoever(void){
//
//	if(init == 0){
//	Counter = 0;
//	Distance = 0;
//	Drive_Speed1 = Drive_GetMotor();
//	init = 1;
//	aStart = Drive_GetTotalDrivenDistance();
//	}
//
//	if(( us_getFrontDistance() <= (500 + Drive_Speed * 200)) & ( us_getFrontDistance() >= 200)){
//
//
//		if(us_getFrontDistance() <= 200) //Notstop, sollte das Auto zu nah an einer Wand/Gegenstand sein
//					{
//						Drive_SetMotor(0);
//					//	os_frequency(&lastWakeTime, 100); // Kurze Zeit warten
//						Drive_SetServo(-50);
//						Drive_SetMotorForDistance(-1 , 1000); //Einlenken Rückwärts fahren
//						Drive_SetServo(50);
//						Drive_SetMotorForDistance(1 , 1000); //Zurücklenken Vorwärts fahren
//					}
//		if(us_getFrontDistance() > 200) // Abstand noch groß genug für regulären Ausweichvorgang
//					{
//					Drive_SetServo((1/Drive_Speed)* 2);
//					Counter++; //Counter um zu wissen wie oft man Lenkung verstellt hat zum zurücklenken später
//					}
//	}else{
//
//	Drive_SetServo(0);
//	Transition_setAusweichen(0);
//
//	}
//	//PD Regler wieder einschalten
//
//
//
//
//
//
//
//	void Gablung_rechtsKurve(void){
//		Transition_setInEinerKurve(1);
//		inRechtsKurve = 1;
//
//		Drive_SetServo(100);
//		Drive_SetMotor(1);
//		if((Drive_GetTotalDrivenDistance() - start) > 1178){
//			//Drive_SetMotor(0);
//			Drive_SetServo(-100);
//
//			//Drive_SetMotor(-1);
//			Transition_setInEinerKurve(0);
//			inRechtsKurve = 0;
//
//		}
//		//TODO
//	}















}
