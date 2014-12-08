/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file transition.c
 * @author Felix	
 * @brief TODO - insert a brief description of that module
 **/

/*
 * Include files
 */

#include "us.h"
#include "us_i2c.h"
#include "regler.h"




/*
 * Global pre-processor symbols/macros ('#define')
 */

/*
 * Global type definitions ('typedef')
 */

/*
 * Global and local variable declarations
 */

/*
 * Global and local functions
 */
int8_t kannLinks = 0;
int8_t kannRechts = 0;
int8_t kannVorne = 0;

int8_t sucheParkluecke = 0;
int8_t ausweichen = 0;
int8_t parkLueckeGefunden = 0;
int8_t inEinerKurve = 0;

int8_t transition_berechneNaechstenZustand(int8_t zustand){

	switch(zustand){


		//Zustände
		//0 IDLE
		//1 Fahren
		//2 Gablung
		//3 Ausweichen
		//4 Parklücke_Suchen KEIN EIGENER ZUSTAND -> EINE ERWEITERUNG ZU FAHREN
		//5 Parken

		case 0 : return 1;
		case 1 : return fahrenTransition();
		case 2 : return gablungTransition();
		case 3 : return ausweichenTransition();
		case 4 : return parklueckeSuchenTransition();
		case 5 : return ParkenTransition();
	}

	return 0;

}


int8_t fahrenTransition(){
	static int16_t letzterAbstandRechts 	= 0;
	static int16_t letzterAbstandLinks  	= 0;
	static int16_t aktuellerAbstandRechts 	= 0;
	static int16_t aktuellerAbstandLinks 	= 0;


	letzterAbstandRechts   = aktuellerAbstandRechts;
	letzterAbstandLinks	   = aktuellerAbstandLinks;
	aktuellerAbstandLinks  = us_getLeftDistance();
	aktuellerAbstandRechts = us_getRightDistance();

	kannLinks = 0;
	kannRechts = 0;
	kannVorne = 0;

	if ((aktuellerAbstandLinks - letzterAbstandLinks) > 2000 ){
		kannLinks = 1;
	}
	if ((aktuellerAbstandRechts - letzterAbstandRechts) > 2000){
		kannRechts = 1;
	}
	if (us_getFrontDistance() > 500){
		kannVorne = 1;
	}

	if (kannLinks == 1 || kannRechts == 1 ){
		return 2;
	}

	if (ausweichen == 1){
		return 3;
	}

	if (sucheParkluecke == 1){
		return 4;
	}


	return 1;

}

int8_t gablungTransition(){
	//TODO Daniel
	if (inEinerKurve == 0){
		return 1;
	}

	return 2;
}

int8_t ausweichenTransition(){

	if(ausweichen == 0){
		return 1;
	}

	//TODO Julian

	return 3;

}

int8_t parklueckeSuchenTransition(){
	if (sucheParkluecke == 1){
		return 4;
	}

	if (parkLueckeGefunden == 1){
		return 5;
	}

	return 1;
}

int8_t ParkenTransition(){
	//TODO Luka

	//Gehe in IDLE
	return 0;
}




/**
 * @brief Put a message in the transmit buffer.
 * @param param one
 * @param param two
 * @return result
 */
uint8_t ModuleFoo_DooFoo(uint8_t* data, uint8_t len) {
	return 1;
}
