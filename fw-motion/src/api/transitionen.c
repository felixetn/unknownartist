/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file transitionen.c
 * @author Daniel	
 * @brief TODO - insert a brief description of that module
 **/

/*
 * Include files
 */
#include "transitionen.h"
#include "regler.h"
#include "api.h"

/*
 * Global pre-processor symbols/macros ('#define')
 */
#define KURVEN_ERKENNEN_TRIGGER 800
/*
 * Global type definitions ('typedef')
 */

/*
 * Global and local variable declarations
 */

int8_t kannLinks = 0;
int8_t kannRechts = 0;
int8_t kannVorne = 0;


int8_t ausweichen = 0;
int8_t parken = 0;
int8_t inEinerKurve = 0;
int8_t interception = 0;

/*
 * Global and local functions
 */



int8_t Transitionen_fahrenTransition(){
	static start = 1;
	static int16_t letzterAbstandRechts 	= 0;
	static int16_t letzterAbstandLinks  	= 0;
	static int16_t aktuellerAbstandRechts 	= 0;
	static int16_t aktuellerAbstandLinks 	= 0;
	static int16_t dif = 0;
	//Init, damit das Auto beim Einschalten nicht versehentlich eine Kurve erkennt
	if(start == 1){
		letzterAbstandLinks = us_getLeftDistance();
		letzterAbstandRechts = Regler_get_sensor();
		aktuellerAbstandLinks = us_getLeftDistance();
		aktuellerAbstandRechts = Regler_get_sensor();
		start = 0;
	}

	letzterAbstandRechts   = aktuellerAbstandRechts;
	letzterAbstandLinks	   = aktuellerAbstandLinks;
	aktuellerAbstandLinks  = us_getLeftDistance();
	aktuellerAbstandRechts = us_getRightDistance();

	//Sensorkorrektur


	kannLinks = 0;
	kannRechts = 0;
	kannVorne = 0;

	dif = aktuellerAbstandLinks - letzterAbstandLinks;

	if ((dif) > KURVEN_ERKENNEN_TRIGGER){
		kannLinks = 1;
	}

	dif = letzterAbstandRechts - aktuellerAbstandRechts;

	if ((dif) > KURVEN_ERKENNEN_TRIGGER){
		kannRechts = 1;
	}

	if (us_getFrontDistance() > 500){
		kannVorne = 1;
	}

	if (kannLinks == 1 || kannRechts == 1){
		return 1;
	}

	//Kommt rein wenn die AusweichenMethode online ist
//	if (ausweichen == 1){
//		return 3;
//	}


	if (parken == 1){
		return 3;
	}


	return 0;
}

int8_t Transitionen_gablungTransition(){
	if(inEinerKurve == 1){
		return 1;
	}else{
		return 0;
	}
}

int8_t Transitionen_ausweichenTransition(){
	if(ausweichen == 1){
		return 2;
	}else{
		return 0;
	}
}

int8_t Transitionen_parkenTransition(){

	return 3;

}
































/*
 * Transitionen_getNextZustand ermittelt anhand der SensorWerte und der Manuell gesetzten Signale, welcher Zustand als n‰chstes dran ist.
 * Anschlieﬂend wird der n‰chste Zustand returned
 */
int8_t Transitionen_getNextZustand(int8_t aktuellerZustand){

	switch(aktuellerZustand){
		case 0 : return Transitionen_fahrenTransition();    //break;
		case 1 : return Transitionen_gablungTransition();   //break;
		case 2 : return Transitionen_ausweichenTransition();//break;
		case 3 : return Transitionen_parkenTransition();    //break;

	}
	//return 0 ;

}

////////////////////////////////GETTER////////////////////////////////

int8_t Transitionen_getKannLinks(){
	return kannLinks;
}

int8_t Transitionen_getKannRechts(){
	return kannRechts;
}

int8_t Transitionen_getKannVorne(){
	return kannVorne;
}
//////////////////////////////////////////////////////////////////////


////////////////////////////////SETTER////////////////////////////////

void Transition_setInEinerKurve(int8_t set){
	inEinerKurve = set;
}

void Transition_setAusweichen(int8_t set){
	ausweichen = set;
}

void Transition_setParken(int8_t set){
	parken = set;
}
//////////////////////////////////////////////////////////////////////


/**
 * @brief Put a message in the transmit buffer.
 * @param param one
 * @param param two
 * @return result
 */

