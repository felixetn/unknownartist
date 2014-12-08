/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file gablung.h
 * @author Daniel	
 * @brief TODO - insert a brief description of that interface
 **/

#ifndef GABLUNG_H_
#define GABLUNG_H_

/*
 * Include files
 */
#include "api.c"
#include "api.h"
#include "navigation.h"
#include "transition.h"
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

void rechtsKurve();
void linksKurve();
void gablung_entscheideRichtung();

/**
 * @brief Put a message in the transmit buffer.
 * @param param one
 * @param param two
 * @return result
 */
extern uint8_t ModuleFoo_DooFoo(uint8_t* data, uint8_t len);


#endif /* GABLUNG_H_ */
