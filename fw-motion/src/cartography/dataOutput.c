/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file dataOutput.c
 * @author dnotz	
 * @brief DataOutput just shows a number, which can consist of more than one digit, on the
 *        segment display.
 **/


#include "dataOutput.h"


void showOnSegmentDisplay(long number, int time_pause) {

	// get the number of digits
	int factor;
	int upperBound = 10;
	while (number > upperBound)
		upperBound *= 10;
	upperBound /= 10;

	// output
//	Seg_Hex(0xF); // begin
	os_wait(time_pause);

	// show every digit on the display and wait as long as specified in time_pause
	for (factor = upperBound; factor > 0; factor /= 10) {
		Seg_Dec((number / factor) % 10);
		os_wait(time_pause);
	}
//	Seg_Hex(0xF); // end
	os_wait(time_pause);
}

