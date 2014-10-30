#include "interrupts.h"
#include "../mcu-files/vectors.h"

extern void InitIrqLevels(void);

#define MAX_INT 128

// Locate: 0x7400 - 4*MAX_INT
#pragma segment DATA=ivt,locate=0x7200
#pragma segment FAR_DATA=ivt,locate=0x7200
static __far IRQHandler ivt[MAX_INT];

static uint8_t initialized = 0;

void interrupts_init(void) {
	uint16_t i;

	if (initialized == 0) {
		InitIrqLevels();

		for (i = 0; i < MAX_INT; i++) {
			ivt[i] = *((__far IRQHandler*) (0xFFFC00ul + (0x400ul-4*MAX_INT) + i * 4ul));
		}

		TBR = ((long)(ivt) & 0xfffc00l) >> 8;

		initialized = 1;

		interrupts_setLevel(7);
	}
}

void interrupts_setHandler(uint8_t interruptNr, uint8_t interruptLevel, IRQHandler handler) {
	if ((interruptNr < 12) || (interruptNr > MAX_INT-1))
		return;
	if (interruptLevel > 7)
		return;

	if (initialized == 0)
		interrupts_init();

	ICR = (interruptNr << 8) | interruptLevel;

	ivt[MAX_INT - 1 - interruptNr] = handler;
}

void interrupts_disableInterrupt(uint8_t interruptNr) {
	if ((interruptNr < 12) || (interruptNr > MAX_INT-1))
		return;

	if (initialized == 0)
		interrupts_init();

	ICR = (interruptNr << 8) | 7;
}
