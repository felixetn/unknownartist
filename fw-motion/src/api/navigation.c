/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file navigation.c
 * @author Daniel	
 * @brief TODO - insert a brief description of that module
 **/

/*
 * Include files
 */

#include "base_type.h"
#include <stdlib.h>

/*
 * Global pre-processor symbols/macros ('#define')
 */

/*
 * Global type definitions ('typedef')
 */

/*
 * Global and local variable declarations
 */
// 0 = links; 1 = gerade; 2 = rechts
int8_t richtungen[4] = {2, 2, 2, 2};

/*
 * Global and local functions
 */


void Navigation_setElement_i(int8_t Element_i, int8_t wert){
	richtungen[Element_i] = wert;
}

int8_t Navigation_getElement_i(int8_t Element_i){
	return richtungen[Element_i];
}
//0 links
//1 gerade aus
//2 rechts
void Navigation_Kurve_gefahren(){
	int8_t i = 0;
	for (i =0; i < 4; i++){
		if(i > 0){
			richtungen[i-1] = richtungen[i];
		}
	}

	richtungen[4] = rand() % 3;

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
