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
#include "right_distance.h"


#define STANDARTDISTANCE 300 //Millimeter

uint8_t object = 0;
uint8_t objectpassed = 0;


int16_t ausweichmanoever(void){

if( (us_getFrontDistance() < 1000 ) && (us_getRightDistance()< 1000) && !(objectpassed))
{

	object = 1;
	return STANDARTDISTANCE ;

	if(object && (us_getRightDistance() < 100)){
		object = 0;
		objectpassed = 1;
	}
}
if( get_right_distance() - us_getRightDistance() < 10 ){
	objectpassed = 0;
	return 0;
}
return 0;
}

