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

#include "regler.h"
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

//Element 1 entscheidet, wohin das Auto an der nächsten Kreuzung fahren soll (wenn möglich)
//Element 2 entscheidet bei der übernächstenKreuzung
//USW...
int8_t* richtungen = malloc(4 * sizeof(int8_t));

/*
 * Global and local functions
 */
void aender_ites_Element(int8_t i, int8_t richtung){
	richtungen[i] = richtung;
}
//0 links
//1 gerade aus
//2 rechts
void kurve_gefahren(){
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
