/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file transitionen.h
 * @author Daniel	
 * @brief TODO - insert a brief description of that interface
 **/

#ifndef TRANSITIONEN_H_
#define TRANSITIONEN_H_

/*
 * Include files
 */
#include "base_type.h"
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
int8_t sucheParkluecke = 0;
/*
 * Global and local functions
 */
//GETTER
int8_t Transitionen_getNextZustand(int8_t aktuellerZustand);

int8_t Transitionen_getKannLinks();

int8_t Transitionen_getKannRechts();

int8_t Transitionen_getKannVorne();

//SETTER
void Transition_setInEinerKurve();

void Transition_setAusweichen();

void Transition_setParken(int8_t set);
/**
 * @brief Put a message in the transmit buffer.
 * @param param one
 * @param param two
 * @return result
 */
extern uint8_t ModuleFoo_DooFoo(uint8_t* data, uint8_t len);


#endif /* TRANSITIONEN_H_ */
