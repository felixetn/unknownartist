/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file map.c
 * @author mschnaubelt
 * @brief ModeController increases the usability of the car by providing the functionality
 *        for different modes between which can be switched by pressing the two switches
 *        on the car.
 */

#include "mode_controller.h"

uint8_t currentMode;

xTaskHandle xHandleModeController;

void (*startModeHandlers[NUM_MODES])(void) = {(void (*)(void))NULL};
void (*stopModeHandlers[NUM_MODES])(void) = {(void (*)(void))NULL};


void initModeController(void) {
#ifdef MODECONTROLLER_TASK
	uint8_t i;
	// HARDWARE INITIALIZATION
	// Setting Pin 43 to input in order to read switch SW2
	DDR07 &= 255 - (1 << 0);
	PIER07 |= (1 << 0);
	// Setting Pin 43 to input in order to read switch SW3
	DDR07 &= 255 - (1 << 1);
	PIER07 |= (1 << 1);

	// init modes with NULL
	for (i = 0; i < NUM_MODES; i++) {
		startModeHandlers[i] = (void(*)(void))NULL;
		stopModeHandlers[i] = (void(*)(void))NULL;
	}

	currentMode = 0;
	xHandleModeController = 0;
#endif
}

void startModeController() {
#ifdef MODECONTROLLER_TASK
	if (xHandleModeController != 0)
		os_resumeTask(xHandleModeController);
	else
		xHandleModeController = os_registerProcessStack(ModeControllerThread, "ModeController",940);

	// Start default mode
	if (startModeHandlers[0] != NULL)
		startModeHandlers[0]();
#endif
}

void stopModeController(void) {
	if (xHandleModeController)
		os_suspendTask(xHandleModeController);
}

void ModeControllerThread(void) {

	int8_t nextMode = 0;

	for (;;) {

		if ((PDR07 & (1 << 0)) == 0) { // Mode select switch pressed
			if ((PDR07 & (1 << 1)) == 0) { // Mode increment switch pressed
				nextMode++;
				if (nextMode >= NUM_MODES)
					nextMode = 0;
//				Seg_Hex(nextMode);
				os_wait(400); // prevent rushing mode numbers ;)
			}
			// Show next mode which will be started if SW2 is released
//			Seg_Hex(nextMode);
		} else if (nextMode != currentMode) {
			switchMode(nextMode);
			nextMode = currentMode;
		}
		os_wait(200);
	}
}

void registerMode(uint8_t modeNumber, void(*startFunc)(void), void(*stopFunc)(void)) {
	if (modeNumber >= NUM_MODES)
		return;
	if (startModeHandlers[modeNumber] != NULL) // modeNumber already refers to a mode
		return;

	startModeHandlers[modeNumber] = startFunc;
	stopModeHandlers[modeNumber] = stopFunc;
}

void switchMode(uint8_t newModeNumber) {
	uint8_t i;

	// stop the current mode
	if (stopModeHandlers[currentMode] != NULL)
		stopModeHandlers[currentMode]();

	if (newModeNumber >= NUM_MODES)
		newModeNumber = 0;

	// Flash nextMode a few times
	for (i = 10; i > 0; i--) {
		Seg_Hex(newModeNumber);
		os_wait(50 * i);
		Seg_Init();
		os_wait(50 * i);
	}

	// start the new mode
	currentMode = newModeNumber;
	if (startModeHandlers[currentMode] != NULL)
		startModeHandlers[currentMode]();

}
