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



int8_t fahrenTransition(){
	static int16_t letzterAbstandRechts 	= 0;
	static int16_t letzterAbstandLinks  	= 0;
	static int16_t aktuellerAbstandRechts 	= 0;
	static int16_t aktuellerAbstandLinks 	= 0;


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
//	if (us_getFrontDistance() > 500){
//		kannVorne = 1;
//	}

	if (kannLinks == 1 || kannRechts == 1 || kannVorne == 1){
		return 2;
	}
	return 1;


	letzterAbstandLinks = aktuellerAbstandLinks;
	letzterAbstandRechts = aktuellerAbstandRechts;
}

int8_t gablungTransition(){

}

int8_t ausweichenTransition(){

}

int8_t parklueckeSuchenTransition(){

}

int8_t ParkenTransition(){

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
