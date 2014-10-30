/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file dataOutput.h
 * @author dnotz	
 * @brief DataOutput just shows a number, which can consist of more than one digit, on the
 *        segment display.
 **/

#ifndef DATAOUTPUT_H_
#define DATAOUTPUT_H_

#include "../api/seg.h"
#include "../api/api.h"

/**
 * @brief Show the number on the 7-segment-display
 * @param number: number that shall be displayed
 * @param pause: milliseconds to wait for the next digit
 */
void showOnSegmentDisplay(long number, int time_pause);

#endif /* DATAOUTPUT_H_ */
