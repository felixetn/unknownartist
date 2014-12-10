/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file transition.h
 * @author Felix	
 * @brief TODO - insert a brief description of that interface
 **/

#ifndef TRANSITION_H_
#define TRANSITION_H_

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
int8_t transition_berechneNaechstenZustand(int8_t zustand);

int8_t fahrenTransition();

int8_t gablungTransition();

int8_t ausweichenTransition();

int8_t parklueckeSuchenTransition();

int8_t ParkenTransition();

void transition_kollisionAbfrage();

void setAusweichen();
/**
 * @brief Put a message in the transmit buffer.
 * @param param one
 * @param param two
 * @return result
 */
extern uint8_t ModuleFoo_DooFoo(uint8_t* data, uint8_t len);


#endif /* TRANSITION_H_ */
