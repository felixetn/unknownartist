/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file navigation.h
 * @author Daniel	
 * @brief TODO - insert a brief description of that interface
 **/

#ifndef NAVIGATION_H_
#define NAVIGATION_H_

/*
 * Include files
 */

#include "base_type.h"

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

void Navigation_setElement_i(int8_t Element_i);

int8_t Navigation_getElement_i(int8_t Element_i);

void Navigation_Kurve_gefahren();
/**
 * @brief Put a message in the transmit buffer.
 * @param param one
 * @param param two
 * @return result
 */
extern uint8_t ModuleFoo_DooFoo(uint8_t* data, uint8_t len);


#endif /* NAVIGATION_H_ */
