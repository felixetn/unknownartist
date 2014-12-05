/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file ringbuffer.h
 * @author Felix	
 * @brief TODO - insert a brief description of that interface
 **/

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

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

extern void Ringbuffer_input(int16_t in);
extern int16_t Ringbuffer_mittelwert();
extern void Ringbuffer_init(int16_t w);


#endif /* RINGBUFFER_H_ */
