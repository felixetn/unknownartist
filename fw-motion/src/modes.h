/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file modes.h
 * @author pnotz
 * @brief Modes is used for defining and registering the different modes which then
 * can be selected via the two switches on the car.
 **/

#ifndef MODES_H_
#define MODES_H_

#include "api/defines.h"
/*
 * register the different modes
 */
void registerModes(void);


// for every mode a start and a stop method is needed
// which are called by the mode_controller
/*
void startBarcodeReaderMode(void);
void stopBarcodeReaderMode(void);
*/

#endif /* MODES_H_ */
