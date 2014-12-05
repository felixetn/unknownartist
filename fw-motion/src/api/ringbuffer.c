/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file ringbuffer.c
 * @author Felix	
 * @brief TODO - insert a brief description of that module
 **/

/*
 * Include files
 */
#include <stdio.h>
#include "base_type.h"
#include "ringbuffer.h"
#include "wireless/wireless.h"
/*
 * Global pre-processor symbols/macros ('#define')
 */

/*
 * Global type definitions ('typedef')
 */

#define BUFFER_SIZE 1

/*
 * Global and local variable declarations
 */

struct Buffer {
  int16_t data[BUFFER_SIZE];
  int16_t write; 					// zeigt immer auf das Feld, das am längsten nicht geändert worden ist
} buffer = {{0}, 0};;

void Ringbuffer_input(int16_t in){

  buffer.data[buffer.write] = in;

  buffer.write = buffer.write + 1;
  if (buffer.write >= BUFFER_SIZE)
    buffer.write = 0;
}

int16_t Ringbuffer_mittelwert(){
	int16_t i;
	int16_t e = 0;
	for(i = 0; i<BUFFER_SIZE; i++){
		e += buffer.data[i];
	    wirelessFormattedDebugMessage(WI_IF_AMB8420, "Buffer[%d]=%d", i, buffer.data[i]);

	}
	 wirelessFormattedDebugMessage(WI_IF_AMB8420, "Ergebnis=%d", (int16_t)(e/BUFFER_SIZE));
	return (int16_t)(e/BUFFER_SIZE);
}

void Ringbuffer_init(int16_t w){
	int i;
	for(i = 0; i<BUFFER_SIZE; i++){
		Ringbuffer_input(w);
	}
}
