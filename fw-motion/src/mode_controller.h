/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file mode_controller.h
 * @author mschnaubelt
 * @brief ModeController increases the usability of the car by providing the functionality
 *        for different modes between which can be switched by pressing the two switches
 *        on the car.
 **/

#ifndef MODE_CONTROLLER_H_
#define MODE_CONTROLLER_H_

#include "api/base_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "api/seg.h"
#include "api/api.h"
#include "api/defines.h"

extern xTaskHandle xHandleModeController;

// the number of different modes that can be selected
#define NUM_MODES	4

extern uint8_t currentMode;

extern void (*startModeHandlers[])(void);
extern void (*stopModeHandlers[])(void);

/*
 * initializes the mode controller module
 */
void initModeController(void);

/*
 * starts the mode controller module
 */
void startModeController(void);

/*
 * stops the mode controller module
 */
void stopModeController(void);

/*
 * Internal function that is called periodically by the OS.
 */
void ModeControllerThread(void);

/*
 * Registers a new mode
 * @param modeNumber the number of the mode that shall be registered
 * @param startFunc a function pointer to a function tostart the mode
 * @param a function pointer to a function to end the mode
 */
void registerMode(uint8_t modeNumber, void(*startFunc)(void),
		void(*stopFunc)(void));

/*
 * Switches to a new mode
 * @param newModeNumber the mode number to which shall be switched
 */
void switchMode(uint8_t newModeNumber);

#endif /* MODE_CONTROLLER_H_ */
