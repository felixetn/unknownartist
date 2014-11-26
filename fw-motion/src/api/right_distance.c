/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file right_distance.c
 * @author Felix	
 * @brief TODO - insert a brief description of that module
 **/

/*
 * Include files
 */

#include "right_distance.h"
#include "api.c"
#include "base_type.h"
#include "wireless/wireless.h"

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

int16_t get_right_distance(){
	return right_distance;
}
void set_right_distance(int16_t d){
	right_distance = d;
}
